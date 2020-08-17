#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/EDMException.h"

#include "flashgg/DataFormats/interface/DiPhotonCandidate.h"
#include "flashgg/DataFormats/interface/ZPlusJetTag.h"
//#include "flashgg/DataFormats/interface/ZPlusJetTagTruth.h"
#include "flashgg/DataFormats/interface/Jet.h"

#include "DataFormats/Common/interface/RefToPtr.h"

#include "DataFormats/Math/interface/deltaR.h"
#include "DataFormats/Math/interface/deltaPhi.h"


#include <vector>
#include <algorithm>

using namespace std;
using namespace edm;

namespace flashgg {

    class ZPlusJetTagProducer : public EDProducer
    {

    public:
        typedef math::XYZPoint Point;

        ZPlusJetTagProducer( const ParameterSet & );

    private:
        void produce( Event &, const EventSetup & ) override;

        EDGetTokenT<View<DiPhotonCandidate> >      diPhotonToken_;
        EDGetTokenT<View<DiPhotonMVAResult> >      mvaResultToken_;
        EDGetTokenT<View<reco::GenParticle> >      genPartToken_;
        EDGetTokenT<View<reco::GenJet> >           genJetToken_;
        std::vector<edm::EDGetTokenT<View<flashgg::Jet> > > tokenJets_;
        std::vector<edm::InputTag> inputTagJets_;
        string systLabel_;

        typedef std::vector<edm::Handle<edm::View<flashgg::Jet> > > JetCollectionVector;
    };

    ZPlusJetTagProducer::ZPlusJetTagProducer( const ParameterSet &iConfig ) :
        diPhotonToken_( consumes<View<flashgg::DiPhotonCandidate> >( iConfig.getParameter<InputTag> ( "DiPhotonTag" ) ) ),
        mvaResultToken_( consumes<View<flashgg::DiPhotonMVAResult> >( iConfig.getParameter<InputTag> ( "MVAResultTag" ) ) ),
        genPartToken_( consumes<View<reco::GenParticle> >( iConfig.getParameter<InputTag> ( "GenParticleTag" ) ) ),
        genJetToken_ ( consumes<View<reco::GenJet> >( iConfig.getParameter<InputTag> ( "GenJetTag" ) ) ),
        inputTagJets_ ( iConfig.getParameter<std::vector<edm::InputTag> >( "inputTagJets" ) ),
        systLabel_   ( iConfig.getParameter<string> ( "SystLabel" ) )
    {
        for (unsigned i = 0 ; i < inputTagJets_.size() ; i++) {
            auto token = consumes<View<flashgg::Jet> >(inputTagJets_[i]);
            tokenJets_.push_back(token);
        }

        produces<vector<ZPlusJetTag> >();
        //        produces<vector<ZPlusJetTagTruth> >();
    }

    void ZPlusJetTagProducer::produce( Event &evt, const EventSetup & )
    {

        Handle<View<flashgg::DiPhotonCandidate> > diPhotons;
        evt.getByToken( diPhotonToken_, diPhotons );
        
        Handle<View<flashgg::DiPhotonMVAResult> > mvaResults;
        evt.getByToken( mvaResultToken_, mvaResults );
        
        Handle<View<reco::GenParticle> > genParticles;
        Handle<View<reco::GenJet> > genJets;

        JetCollectionVector Jets( inputTagJets_.size() );
        for( size_t j = 0; j < inputTagJets_.size(); ++j ) {
            evt.getByToken( tokenJets_[j], Jets[j] );
        }
       
        std::unique_ptr<vector<ZPlusJetTag> >      tags  ( new vector<ZPlusJetTag> );
        //        std::unique_ptr<vector<ZPlusJetTagTruth> > truths( new vector<ZPlusJetTagTruth> );

        //        unsigned int idx = 0;
        //        edm::RefProd<vector<ZPlusJetTagTruth> > rTagTruth = evt.getRefBeforePut<vector<ZPlusJetTagTruth> >();

        Point higgsVtx;
        
        ///std::cout << "+++++ test +++++" << std::endl;
        
        if( ! evt.isRealData() ) {
            evt.getByToken( genPartToken_, genParticles );
            evt.getByToken( genJetToken_, genJets );
            for( unsigned int genLoop = 0 ; genLoop < genParticles->size(); genLoop++ ) {
                int pdgid = genParticles->ptrAt( genLoop )->pdgId();
                if( pdgid == 25 || pdgid == 22 ) {
                    higgsVtx = genParticles->ptrAt( genLoop )->vertex();
                    break;
                }
            }
            /*
            for( unsigned int genLoop = 0 ; genLoop < genParticles->size(); genLoop++ ) {
                edm::Ptr<reco::GenParticle> part = genParticles->ptrAt( genLoop );
            }
            */
        }

        assert( diPhotons->size() == mvaResults->size() ); // We are relying on corresponding sets - update this to give an error/exception

        for( unsigned int candIndex = 0; candIndex < diPhotons->size() ; candIndex++ ) {
            edm::Ptr<flashgg::DiPhotonMVAResult>      mvares          = mvaResults->ptrAt( candIndex );
            edm::Ptr<flashgg::DiPhotonCandidate>      dipho           = diPhotons->ptrAt( candIndex );

            unsigned njets = 0;
            Ptr<flashgg::Jet> leadingJet;

            for( unsigned jetLoop = 0; jetLoop < Jets[candIndex]->size() ; jetLoop++ ) {
                Ptr<flashgg::Jet> jet  = Jets[candIndex]->ptrAt( jetLoop );
                //                if(!jet->passesJetID  ( flashgg::Tight2017 ) ) { continue; }
                if (jet->pt() < 20.) continue;

                // close to lead photon?
                float dPhi = deltaPhi( jet->phi(), dipho->leadingPhoton()->phi() );
                float dEta = jet->eta() - dipho->leadingPhoton()->eta();
                if( sqrt( dPhi * dPhi + dEta * dEta ) < 0.4 ) { continue; }

                // close to sublead photon?
                dPhi = deltaPhi( jet->phi(), dipho->subLeadingPhoton()->phi() );
                dEta = jet->eta() - dipho->subLeadingPhoton()->eta();
                if( sqrt( dPhi * dPhi + dEta * dEta ) < 0.4 ) { continue; }

                if ( njets == 0 ) {
                    leadingJet = jet;
                }
                njets++;
            }

            //if (njets > 0) {
                    ZPlusJetTag tag_obj( dipho, mvares, leadingJet, njets );
                    tag_obj.setDiPhotonIndex( candIndex );
                    tag_obj.setSystLabel    ( systLabel_ );
                    tag_obj.setIsGold ( evt.run() );
                    tag_obj.includeWeights( *dipho );
                    if (njets > 0) 
                        tag_obj.includeWeights( *leadingJet );
                    
                    //truth_obj.setGenPV( higgsVtx );

                    //std::cout << "The jet's eta, HF hadronic energy fraction values are " << tag_obj.jetEta() << ", " << tag_obj.jet_HFHadronEnergyFraction() << endl << endl;
                    //std::cout << "The Z mass, pt values are " << tag_obj.zMass() << ", " << tag_obj.zPt() << endl << endl;
                    //std::cout << "The Z jet deltaPhi value is " << tag_obj.deltaPhiZJet() << endl << endl;
                    
                    // saving the collection
                    tags->push_back( tag_obj );
                    //                if( ! evt.isRealData() ) {
                    //                    truths->push_back( truth_obj );
                    //                    tags->back().setTagTruth( edm::refToPtr( edm::Ref<vector<ZPlusJetTagTruth> >( rTagTruth, idx++ ) ) );
                    //                }
                    // }
        }
        evt.put( std::move( tags ) );
        //        evt.put( std::move( truths ) );
    }
}

typedef flashgg::ZPlusJetTagProducer FlashggZPlusJetTagProducer;
DEFINE_FWK_MODULE( FlashggZPlusJetTagProducer );
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

