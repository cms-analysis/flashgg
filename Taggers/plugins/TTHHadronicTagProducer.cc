#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/EDMException.h"

#include "DataFormats/PatCandidates/interface/Jet.h"
#include "flashgg/DataFormats/interface/Jet.h"
#include "flashgg/DataFormats/interface/DiPhotonCandidate.h"
#include "flashgg/DataFormats/interface/TTHHadronicTag.h"
#include "flashgg/DataFormats/interface/DiPhotonMVAResult.h"

#include "flashgg/DataFormats/interface/TagTruthBase.h"
#include "DataFormats/Common/interface/RefToPtr.h"

#include <vector>
#include <algorithm>
#include <string>
#include <utility>

using namespace std;
using namespace edm;

namespace flashgg {

    class TTHHadronicTagProducer : public EDProducer
    {

    public:
        typedef math::XYZPoint Point;

        TTHHadronicTagProducer( const ParameterSet & );
    private:
        void produce( Event &, const EventSetup & ) override;

        EDGetTokenT<View<DiPhotonCandidate> > diPhotonToken_;
        //EDGetTokenT<View<Jet> > thejetToken_;
        std::vector<edm::InputTag> inputTagJets_;
        EDGetTokenT<View<DiPhotonMVAResult> > mvaResultToken_;
        EDGetTokenT<View<reco::GenParticle> > genParticleToken_;
        string systLabel_;


        typedef std::vector<edm::Handle<edm::View<flashgg::Jet> > > JetCollectionVector;
        //---thresholds---
        //---photons
        double MVAThreshold_;
        double leadPhoPtThreshold_;
        bool   leadPhoUseVariableTh_;
        double leadPhoOverMassThreshold_;
        double subleadPhoPtThreshold_;
        bool   subleadPhoUseVariableTh_;
        double subleadPhoOverMassThreshold_;
        //---jets
        double jetPtThreshold_;
        double jetEtaCut_;
        double dRJetPhoLeadCut_;
        double dRJetPhoSubleadCut_;
        double bDiscriminatorLoose_;
        double bDiscriminatorMedium_;
        double jetsNumberThreshold_;
        double bjetsNumberThreshold_;
        string bTag_;
    };

    TTHHadronicTagProducer::TTHHadronicTagProducer( const ParameterSet &iConfig ) :
        diPhotonToken_( consumes<View<flashgg::DiPhotonCandidate> >( iConfig.getParameter<InputTag> ( "DiPhotonTag" ) ) ),
        //thejetToken_( consumes<View<flashgg::Jet> >( iConfig.getParameter<InputTag>( "JetTag" ) ) ),
        inputTagJets_( iConfig.getParameter<std::vector<edm::InputTag> >( "inputTagJets" ) ),
        mvaResultToken_( consumes<View<flashgg::DiPhotonMVAResult> >( iConfig.getParameter<InputTag>( "MVAResultTag" ) ) ),
        genParticleToken_( consumes<View<reco::GenParticle> >( iConfig.getParameter<InputTag> ( "GenParticleTag" ) ) ),
        systLabel_( iConfig.getParameter<string> ( "SystLabel" ) )
    {
        MVAThreshold_ = iConfig.getParameter<double>( "MVAThreshold" );
        leadPhoPtThreshold_ = iConfig.getParameter<double>( "leadPhoPtThreshold" );
        leadPhoUseVariableTh_ = iConfig.getParameter<bool>( "leadPhoUseVariableTh" );
        leadPhoOverMassThreshold_ = iConfig.getParameter<double>( "leadPhoOverMassThreshold" );
        subleadPhoPtThreshold_ = iConfig.getParameter<double>( "leadPhoPtThreshold" );
        subleadPhoUseVariableTh_ = iConfig.getParameter<bool>( "subleadPhoUseVariableTh" );
        subleadPhoOverMassThreshold_ = iConfig.getParameter<double>( "subleadPhoOverMassThreshold" );
        jetPtThreshold_ = iConfig.getParameter<double>( "jetPtThreshold" );
        jetEtaCut_ = iConfig.getParameter<double>( "jetEtaCut" );
        dRJetPhoLeadCut_ = iConfig.getParameter<double>( "dRJetPhoLeadCut" );
        dRJetPhoSubleadCut_ = iConfig.getParameter<double>( "dRJetPhoSubleadCut" );
        bDiscriminatorLoose_ = iConfig.getParameter<double>( "bDiscriminatorLoose" );
        bDiscriminatorMedium_ = iConfig.getParameter<double>( "bDiscriminatorMedium" );
        jetsNumberThreshold_ = iConfig.getParameter<int>( "jetsNumberThreshold" );
        bjetsNumberThreshold_ = iConfig.getParameter<int>( "bjetsNumberThreshold" );
        bTag_ = iConfig.getParameter<string> ( "bTag" );

        produces<vector<TTHHadronicTag> >();
        produces<vector<TagTruthBase> >();
    }

    void TTHHadronicTagProducer::produce( Event &evt, const EventSetup & )
    {

        //Handle<View<flashgg::Jet> > theJets;
        //evt.getByToken( thejetToken_, theJets );
        // const PtrVector<flashgg::Jet>& jetPointers = theJets->ptrVector();
        JetCollectionVector Jets( inputTagJets_.size() );
        for( size_t j = 0; j < inputTagJets_.size(); ++j ) {
            evt.getByLabel( inputTagJets_[j], Jets[j] );
        }

        Handle<View<flashgg::DiPhotonCandidate> > diPhotons;
        evt.getByToken( diPhotonToken_, diPhotons );
        // const PtrVector<flashgg::DiPhotonCandidate>& diPhotonPointers = diPhotons->ptrVector();

        Handle<View<flashgg::DiPhotonMVAResult> > mvaResults;
        evt.getByToken( mvaResultToken_, mvaResults );
        // const PtrVector<flashgg::DiPhotonMVAResult>& mvaResultPointers = mvaResults->ptrVector();

        Handle<View<reco::GenParticle> > genParticles;

        std::auto_ptr<vector<TTHHadronicTag> > tthhtags( new vector<TTHHadronicTag> );
        std::auto_ptr<vector<TagTruthBase> > truths( new vector<TagTruthBase> );

        Point higgsVtx;
        if( ! evt.isRealData() ) {
            evt.getByToken( genParticleToken_, genParticles );
            for( unsigned int genLoop = 0 ; genLoop < genParticles->size(); genLoop++ ) {
                int pdgid = genParticles->ptrAt( genLoop )->pdgId();
                if( pdgid == 25 || pdgid == 22 ) {
                    higgsVtx = genParticles->ptrAt( genLoop )->vertex();
                    break;
                }
            }
        }

        edm::RefProd<vector<TagTruthBase> > rTagTruth = evt.getRefBeforePut<vector<TagTruthBase> >();
        unsigned int idx = 0;

        for( unsigned int diphoIndex = 0; diphoIndex < diPhotons->size(); diphoIndex++ ) {

            int jetcount = 0;
            int njets_btagloose = 0;
            int njets_btagmedium = 0;

            unsigned int jetCollectionIndex = diPhotons->ptrAt( diphoIndex )->jetCollectionIndex();

            std::vector<edm::Ptr<flashgg::Jet> > JetVect;
            std::vector<edm::Ptr<flashgg::Jet> > BJetVect;

            edm::Ptr<flashgg::DiPhotonCandidate> dipho = diPhotons->ptrAt( diphoIndex );
            edm::Ptr<flashgg::DiPhotonMVAResult> mvares = mvaResults->ptrAt( diphoIndex );

            if( !dipho->leadingPhoton()->passElectronVeto() || !dipho->subLeadingPhoton()->passElectronVeto() ) { continue; }

            double leadPhoPtCut = leadPhoPtThreshold_;
            double subleadPhoPtCut = subleadPhoPtThreshold_;
            if( leadPhoUseVariableTh_ )
            { leadPhoPtCut = leadPhoOverMassThreshold_ * dipho->mass(); }
            if( subleadPhoUseVariableTh_ )
            { subleadPhoPtCut = subleadPhoOverMassThreshold_ * dipho->mass(); }

            if( dipho->leadingPhoton()->pt() < leadPhoPtCut && dipho->subLeadingPhoton()->pt() < subleadPhoPtCut ) { continue; }
            if( mvares->mvaValue() < MVAThreshold_ ) { continue; }

            for( unsigned int jetIndex = 0; jetIndex < Jets[jetCollectionIndex]->size() ; jetIndex++ ) {
                edm::Ptr<flashgg::Jet> thejet = Jets[jetCollectionIndex]->ptrAt( jetIndex );
                if( fabs( thejet->eta() ) > jetEtaCut_ ) { continue; }

                float bDiscriminatorValue = 0;

                float dEtaLead = thejet->eta() - dipho->leadingPhoton()->eta();
                float dEtaSublead = thejet->eta() - dipho->subLeadingPhoton()->eta();

                float dPhiLead = deltaPhi( thejet->phi(), dipho->leadingPhoton()->phi() );
                float dPhiSublead = deltaPhi( thejet->phi(), dipho->subLeadingPhoton()->phi() );

                float dRJetPhoLead = sqrt( dEtaLead * dEtaLead + dPhiLead * dPhiLead );
                float dRJetPhoSubLead = sqrt( dEtaSublead * dEtaSublead + dPhiSublead * dPhiSublead );

                if( dRJetPhoLead < dRJetPhoLeadCut_ || dRJetPhoSubLead < dRJetPhoSubleadCut_ ) { continue; }
                if( thejet->pt() < jetPtThreshold_ ) { continue; }

                jetcount++;
                JetVect.push_back( thejet );

                bDiscriminatorValue = thejet->bDiscriminator( bTag_ );

                if( bDiscriminatorValue > bDiscriminatorLoose_ ) { njets_btagloose++; }
                if( bDiscriminatorValue > bDiscriminatorMedium_ ) {

                    njets_btagmedium++;
                    JetVect.pop_back();
                    BJetVect.push_back( thejet );
                }
            }

            if( njets_btagmedium > bjetsNumberThreshold_ && jetcount > jetsNumberThreshold_ ) {
                TTHHadronicTag tthhtags_obj( dipho, mvares, JetVect, BJetVect );
                tthhtags_obj.setNBLoose( njets_btagloose );
                tthhtags_obj.setNBMedium( njets_btagmedium );
                tthhtags_obj.setDiPhotonIndex( diphoIndex );
                tthhtags_obj.setSystLabel( systLabel_ );
                tthhtags_obj.includeWeights( *dipho );
                tthhtags->push_back( tthhtags_obj );
                if( ! evt.isRealData() ) {
                    TagTruthBase truth_obj;
                    truth_obj.setGenPV( higgsVtx );
                    truths->push_back( truth_obj );
                    tthhtags->back().setTagTruth( edm::refToPtr( edm::Ref<vector<TagTruthBase> >( rTagTruth, idx++ ) ) );
                }
                // count++;
            }
        }
        evt.put( tthhtags );
        evt.put( truths );
        // cout << "tagged events = " << count << endl;
    }
}
typedef flashgg::TTHHadronicTagProducer FlashggTTHHadronicTagProducer;
DEFINE_FWK_MODULE( FlashggTTHHadronicTagProducer );

// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

