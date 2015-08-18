#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/EDMException.h"
#include "DataFormats/TrackReco/interface/HitPattern.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/Math/interface/deltaR.h"

#include "flashgg/DataFormats/interface/Jet.h"
#include "flashgg/DataFormats/interface/DiPhotonCandidate.h"
#include "flashgg/DataFormats/interface/VHHadronicTag.h"

#include "flashgg/DataFormats/interface/TagTruthBase.h"
#include "DataFormats/Common/interface/RefToPtr.h"

#include <vector>
#include <algorithm>
#include <string>
#include <utility>
#include <TLorentzVector.h>
#include "TMath.h"

using namespace std;
using namespace edm;

typedef std::pair< edm::Ptr<flashgg::Jet>, edm::Ptr<flashgg::Jet> > jetptrpair;


namespace flashgg {

    class VHHadronicTagProducer : public EDProducer
    {

    public:
        typedef math::XYZPoint Point;

        VHHadronicTagProducer( const ParameterSet & );
    private:

        void produce( Event &, const EventSetup & ) override;

        EDGetTokenT<View<DiPhotonCandidate> > diPhotonToken_;
        //EDGetTokenT<View<Jet> > thejetToken_;
        EDGetTokenT<View<DiPhotonMVAResult> > mvaResultToken_;
        EDGetTokenT<View<reco::GenParticle> > genParticleToken_;

        std::vector<edm::InputTag> inputTagJets_;
        typedef std::vector<edm::Handle<edm::View<flashgg::Jet> > > JetCollectionVector;

        //Thresholds
        double leadPhoOverMassThreshold_;
        double subleadPhoOverMassThreshold_;
        double diphoMVAThreshold_;
        double jetsNumberThreshold_;
        double jetPtThreshold_;
        double jetEtaThreshold_;
        double dRJetToPhoLThreshold_;
        double dRJetToPhoSThreshold_;
        double dijetMassLowThreshold_;
        double dijetMassHighThreshold_;
        double cosThetaStarThreshold_;
        double phoIdMVAThreshold_;

        string systLabel_;



    };

    VHHadronicTagProducer::VHHadronicTagProducer( const ParameterSet &iConfig ) :

        diPhotonToken_( consumes<View<flashgg::DiPhotonCandidate> >( iConfig.getParameter<InputTag> ( "DiPhotonTag" ) ) ),
        //thejetToken_     ( consumes<View<flashgg::Jet> >( iConfig.getParameter<InputTag>( "JetTag" ) ) ),
        mvaResultToken_( consumes<View<flashgg::DiPhotonMVAResult> >( iConfig.getParameter<InputTag> ( "MVAResultTag" ) ) ),
        genParticleToken_( consumes<View<reco::GenParticle> >( iConfig.getParameter<InputTag> ( "GenParticleTag" ) ) ),
        systLabel_( iConfig.getParameter<string> ( "SystLabel" ) )
    {

        // ***** define thresholds ***********************

        double default_leadPhoOverMassThreshold_    = 0.375;
        double default_subleadPhoOverMassThreshold_ = 0.25;
        double default_diphoMVAThreshold_           = -1.0;  // CHECK THIS
        double default_jetsNumberThreshold_         = 2;
        double default_jetPtThreshold_              = 40.;
        double default_jetEtaThreshold_             = 2.4;
        double default_dRJetToPhoLThreshold_        = 0.5;
        double default_dRJetToPhoSThreshold_        = 0.5;
        double default_dijetMassLowThreshold_       = 60;
        double default_dijetMassHighThreshold_      = 120;
        double default_cosThetaStarThreshold_       = 0.5;
        double default_phoIdMVAThreshold_           = -0.2; // assumes we apply the same cut as for all other VH categories


        leadPhoOverMassThreshold_    = iConfig.getUntrackedParameter<double>( "leadPhoOverMassThreshold", default_leadPhoOverMassThreshold_ );
        subleadPhoOverMassThreshold_ = iConfig.getUntrackedParameter<double>( "subleadPhoOverMassThreshold", default_subleadPhoOverMassThreshold_ );
        diphoMVAThreshold_           = iConfig.getUntrackedParameter<double>( "diphoMVAThreshold", default_diphoMVAThreshold_ );
        jetsNumberThreshold_         = iConfig.getUntrackedParameter<double>( "jetsNumberThreshold", default_jetsNumberThreshold_ );
        jetPtThreshold_              = iConfig.getUntrackedParameter<double>( "jetPtThreshold", default_jetPtThreshold_ );
        jetEtaThreshold_             = iConfig.getUntrackedParameter<double>( "jetEtaThreshold", default_jetEtaThreshold_ );
        dRJetToPhoLThreshold_        = iConfig.getUntrackedParameter<double>( "dRJetToPhoLThreshold", default_dRJetToPhoLThreshold_ );
        dRJetToPhoSThreshold_        = iConfig.getUntrackedParameter<double>( "dRJetToPhoSThreshold", default_dRJetToPhoSThreshold_ );
        dijetMassLowThreshold_       = iConfig.getUntrackedParameter<double>( "dijetMassLowThreshold", default_dijetMassLowThreshold_ );
        dijetMassHighThreshold_      = iConfig.getUntrackedParameter<double>( "dijetMassHighThreshold", default_dijetMassHighThreshold_ );
        cosThetaStarThreshold_       = iConfig.getUntrackedParameter<double>( "cosThetaStarThreshold", default_cosThetaStarThreshold_ );
        phoIdMVAThreshold_           = iConfig.getUntrackedParameter<double>( "phoIdMVAThreshold", default_phoIdMVAThreshold_ );

        // yacine: new recipe for flashgg jets
        inputTagJets_                = iConfig.getParameter<std::vector<edm::InputTag> >( "inputTagJets" );

        // *************************************************

        produces<vector<VHHadronicTag> >();
        produces<vector<TagTruthBase> >();
    }

    void VHHadronicTagProducer::produce( Event &evt, const EventSetup & )
    {

        Handle<View<flashgg::DiPhotonCandidate> > diPhotons;
        evt.getByToken( diPhotonToken_, diPhotons );
        //  const PtrVector<flashgg::DiPhotonCandidate>& diPhotonPointers = diPhotons->ptrVector();

        //  Yacine : this is not longer supported
        //  Handle<View<flashgg::Jet> > theJets;
        //  evt.getByToken( thejetToken_, theJets );
        //  const PtrVector<flashgg::Jet>& jetPointers = theJets->ptrVector();

        JetCollectionVector Jets( inputTagJets_.size() );
        for( size_t j = 0; j < inputTagJets_.size(); ++j ) {
            evt.getByLabel( inputTagJets_[j], Jets[j] );
        }

        Handle<View<flashgg::DiPhotonMVAResult> > mvaResults;
        evt.getByToken( mvaResultToken_, mvaResults );
        //   const PtrVector<flashgg::DiPhotonMVAResult>& mvaResultPointers = mvaResults->ptrVector();

        Handle<View<reco::GenParticle> > genParticles;
        evt.getByToken( genParticleToken_, genParticles );

        std::auto_ptr<vector<VHHadronicTag> > vhhadtags( new vector<VHHadronicTag> );
        std::auto_ptr<vector<TagTruthBase> > truths( new vector<TagTruthBase> );

        Point higgsVtx;
        if( ! evt.isRealData() )
            for( unsigned int genLoop = 0 ; genLoop < genParticles->size(); genLoop++ ) {
                int pdgid = genParticles->ptrAt( genLoop )->pdgId();
                if( pdgid == 25 || pdgid == 22 ) {
                    higgsVtx = genParticles->ptrAt( genLoop )->vertex();
                    break;
                }
            }

        edm::RefProd<vector<TagTruthBase> > rTagTruth = evt.getRefBeforePut<vector<TagTruthBase> >();
        unsigned int idx = 0;

        assert( diPhotons->size() == mvaResults->size() );

        double idmva1 = 0.;
        double idmva2 = 0.;

        for( unsigned int diphoIndex = 0; diphoIndex < diPhotons->size(); diphoIndex++ ) {

            edm::Ptr<flashgg::DiPhotonCandidate> dipho  = diPhotons->ptrAt( diphoIndex );
            edm::Ptr<flashgg::DiPhotonMVAResult> mvares = mvaResults->ptrAt( diphoIndex );

            // ********** photon ID and diphoton requirements: *********

            if( dipho->leadingPhoton()->pt() < ( dipho->mass() )*leadPhoOverMassThreshold_ ) { continue; }
            if( dipho->subLeadingPhoton()->pt() < ( dipho->mass() )*subleadPhoOverMassThreshold_ ) { continue; }

            idmva1 = dipho->leadingPhoton()->phoIdMvaDWrtVtx( dipho->vtx() );
            idmva2 = dipho->subLeadingPhoton()->phoIdMvaDWrtVtx( dipho->vtx() );
            if( idmva1 <= phoIdMVAThreshold_ || idmva2 <= phoIdMVAThreshold_ ) { continue; }

            if( mvares->result < diphoMVAThreshold_ ) { continue; }

            std::vector<edm::Ptr<flashgg::Jet> > goodJets;

            unsigned int jetCollectionIndex = diPhotons->ptrAt( diphoIndex )->jetCollectionIndex();

            for( size_t ijet = 0; ijet < Jets[jetCollectionIndex]->size(); ijet++ ) {

                edm::Ptr<flashgg::Jet> thejet = Jets[jetCollectionIndex]->ptrAt( ijet );

                if( !thejet->passesPuJetId( dipho ) )           { continue; }
                if( fabs( thejet->eta() ) > jetEtaThreshold_ )  { continue; }
                if( thejet->pt() < jetPtThreshold_ )            { continue; }

                float dPhiJetToPhoL = deltaPhi( dipho->leadingPhoton()->phi(), thejet->phi() );
                float dEtaJetToPhoL = dipho->leadingPhoton()->eta() - thejet->eta();
                float dRJetToPhoL   = sqrt( dEtaJetToPhoL * dEtaJetToPhoL + dPhiJetToPhoL * dPhiJetToPhoL );

                float dPhiJetToPhoS = deltaPhi( dipho->subLeadingPhoton()->phi(), thejet->phi() );
                float dEtaJetToPhoS = dipho->subLeadingPhoton()->eta() - thejet->eta();
                float dRJetToPhoS   = sqrt( dEtaJetToPhoS * dEtaJetToPhoS + dPhiJetToPhoS * dPhiJetToPhoS );

                if( abs( dRJetToPhoL ) < dRJetToPhoLThreshold_ )            { continue; }
                if( abs( dRJetToPhoS ) < dRJetToPhoSThreshold_ )            { continue; }

                goodJets.push_back( thejet );
            }

            // *********************************************************************

            if( goodJets.size() < 2 ) { continue; }

            TLorentzVector jetl, jets, dijet, phol, phos, diphoton, vstar;

            phol.SetPtEtaPhiE( dipho->leadingPhoton()->pt(), dipho->leadingPhoton()->eta(), dipho->leadingPhoton()->phi(), dipho->leadingPhoton()->energy() );
            phos.SetPtEtaPhiE( dipho->subLeadingPhoton()->pt(), dipho->subLeadingPhoton()->eta(), dipho->subLeadingPhoton()->phi(), dipho->subLeadingPhoton()->energy() );
            jetl.SetPtEtaPhiE( goodJets[0]->pt(), goodJets[0]->eta(), goodJets[0]->phi(), goodJets[0]->energy() );
            jets.SetPtEtaPhiE( goodJets[1]->pt(), goodJets[1]->eta(), goodJets[1]->phi(), goodJets[1]->energy() );

            diphoton = phol + phos;
            dijet = jetl + jets;
            vstar = diphoton + dijet;

            float invmass = dijet.M();

            if( invmass < dijetMassLowThreshold_ || invmass > dijetMassHighThreshold_ ) { continue; }  // go out of diphoton loop

            diphoton.Boost( -vstar.BoostVector() );
            float costhetastar = -diphoton.CosTheta();
            if( abs( costhetastar ) > cosThetaStarThreshold_ ) { continue; }

            VHHadronicTag vhhadtag_obj( dipho, mvares );
            vhhadtag_obj.setJets( goodJets[0], goodJets[1] );
            vhhadtag_obj.setDiPhotonIndex( diphoIndex );
            vhhadtag_obj.setSystLabel( systLabel_ );
            vhhadtags->push_back( vhhadtag_obj );

            TagTruthBase truth_obj;
            truth_obj.setGenPV( higgsVtx );
            if( ! evt.isRealData() ) {
                truths->push_back( truth_obj );
                vhhadtags->back().setTagTruth( edm::refToPtr( edm::Ref<vector<TagTruthBase> >( rTagTruth, idx++ ) ) );
            }
        }
        evt.put( vhhadtags );
        evt.put( truths );
    }



}
typedef flashgg::VHHadronicTagProducer FlashggVHHadronicTagProducer;
DEFINE_FWK_MODULE( FlashggVHHadronicTagProducer );
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

