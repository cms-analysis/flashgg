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
        TTHHadronicTagProducer( const ParameterSet & );
    private:
        void produce( Event &, const EventSetup & ) override;

        EDGetTokenT<View<DiPhotonCandidate> > diPhotonToken_;
        EDGetTokenT<View<Jet> > thejetToken_;
        EDGetTokenT<View<DiPhotonMVAResult> > mvaResultToken_;

        int count = 0;
        string bTag_;
    };

    TTHHadronicTagProducer::TTHHadronicTagProducer( const ParameterSet &iConfig ) :
        diPhotonToken_( consumes<View<flashgg::DiPhotonCandidate> >( iConfig.getUntrackedParameter<InputTag> ( "DiPhotonTag", InputTag( "flashggDiPhotons" ) ) ) ),
        thejetToken_( consumes<View<flashgg::Jet> >( iConfig.getUntrackedParameter<InputTag>( "TTHJetTag", InputTag( "flashggJets" ) ) ) ),
        mvaResultToken_( consumes<View<flashgg::DiPhotonMVAResult> >( iConfig.getUntrackedParameter<InputTag>( "MVAResultTag", InputTag( "flashggDiPhotonMVA" ) ) ) )
    {
        string default_bTag_ = "combinedInclusiveSecondaryVertexV2BJetTags";
        bTag_ = iConfig.getUntrackedParameter<string>( "bTag", default_bTag_ );

        produces<vector<TTHHadronicTag> >();
    }

    void TTHHadronicTagProducer::produce( Event &evt, const EventSetup & )
    {

        Handle<View<flashgg::Jet> > theJets;
        evt.getByToken( thejetToken_, theJets );
//		const PtrVector<flashgg::Jet>& jetPointers = theJets->ptrVector();

        Handle<View<flashgg::DiPhotonCandidate> > diPhotons;
        evt.getByToken( diPhotonToken_, diPhotons );
        //	const PtrVector<flashgg::DiPhotonCandidate>& diPhotonPointers = diPhotons->ptrVector();

        Handle<View<flashgg::DiPhotonMVAResult> > mvaResults;
        evt.getByToken( mvaResultToken_, mvaResults );
//		const PtrVector<flashgg::DiPhotonMVAResult>& mvaResultPointers = mvaResults->ptrVector();


        std::auto_ptr<vector<TTHHadronicTag> > tthhtags( new vector<TTHHadronicTag> );


        for( unsigned int diphoIndex = 0; diphoIndex < diPhotons->size(); diphoIndex++ ) {

            int jetcount = 0;
            int njets_btagloose = 0;
            int njets_btagmedium = 0;

            std::vector<edm::Ptr<flashgg::Jet> > JetVect;
            std::vector<edm::Ptr<flashgg::Jet> > BJetVect;

            edm::Ptr<flashgg::DiPhotonCandidate> dipho = diPhotons->ptrAt( diphoIndex );
            edm::Ptr<flashgg::DiPhotonMVAResult> mvares = mvaResults->ptrAt( diphoIndex );

            if( !dipho->leadingPhoton()->passElectronVeto() || !dipho->subLeadingPhoton()->passElectronVeto() ) { continue; }

            if( dipho->leadingPhoton()->pt() < ( 60 * ( dipho->mass() ) ) / 120. && dipho->subLeadingPhoton()->pt() < 25. && dipho->subLeadingPhoton()->pt() < 33. ) { continue; }
            if( mvares->mvaValue() < .2 ) { continue; }

            for( unsigned int jetIndex = 0; jetIndex < theJets->size() ; jetIndex++ ) {
                edm::Ptr<flashgg::Jet> thejet = theJets->ptrAt( jetIndex );
                if( fabs( thejet->eta() ) > 2.4 ) { continue; }

                float bDiscriminatorValue = 0;

                float dEtaLead = thejet->eta() - dipho->leadingPhoton()->eta();
                float dEtaSublead = thejet->eta() - dipho->subLeadingPhoton()->eta();

                float dPhiLead = deltaPhi( thejet->phi(), dipho->leadingPhoton()->phi() );
                float dPhiSublead = deltaPhi( thejet->phi(), dipho->subLeadingPhoton()->phi() );

                float dRJetPhoLead = sqrt( dEtaLead * dEtaLead + dPhiLead * dPhiLead );
                float dRJetPhoSubLead = sqrt( dEtaSublead * dEtaSublead + dPhiSublead * dPhiSublead );


                if( dRJetPhoLead < 0.5 || dRJetPhoSubLead < 0.5 ) { continue; }
                if( thejet->pt() < 30. ) { continue; }

                jetcount++;
                JetVect.push_back( thejet );

                bDiscriminatorValue = thejet->bDiscriminator( bTag_ );

                if( bDiscriminatorValue > 0.244 ) { njets_btagloose++; }
                if( bDiscriminatorValue > 0.679 ) {

                    njets_btagmedium++;
                    JetVect.pop_back();
                    BJetVect.push_back( thejet );
                }
            }

            if( njets_btagmedium > 0 && jetcount >= 5 ) {

                TTHHadronicTag tthhtags_obj( dipho, mvares, JetVect, BJetVect );
                tthhtags_obj.setNBLoose( njets_btagloose );
                tthhtags_obj.setNBMedium( njets_btagmedium );
                tthhtags_obj.setDiPhotonIndex( diphoIndex );
                tthhtags->push_back( tthhtags_obj );
                count++;

            }
        }
        evt.put( tthhtags );
        cout << "tagged events = " << count << endl;
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

