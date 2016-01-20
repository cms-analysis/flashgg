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
#include "flashgg/DataFormats/interface/Electron.h"
#include "flashgg/DataFormats/interface/Muon.h"
#include "flashgg/Taggers/interface/LeptonSelection.h"

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

        std::vector<edm::EDGetTokenT<View<flashgg::Jet> > > tokenJets_;
        EDGetTokenT<View<DiPhotonCandidate> > diPhotonToken_;
        //EDGetTokenT<View<Jet> > thejetToken_;
        std::vector<edm::InputTag> inputTagJets_;
        EDGetTokenT<View<Electron> > electronToken_;
        EDGetTokenT<View<flashgg::Muon> > muonToken_;
        EDGetTokenT<View<reco::Vertex> > vertexToken_;
        EDGetTokenT<View<DiPhotonMVAResult> > mvaResultToken_;
        EDGetTokenT<View<reco::GenParticle> > genParticleToken_;
        string systLabel_;


        typedef std::vector<edm::Handle<edm::View<flashgg::Jet> > > JetCollectionVector;
        //---thresholds---
        //---photons
        double MVAThreshold_;
        double PhoMVAThreshold_;
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
        vector<double> bDiscriminator_;
        double jetsNumberThreshold_;
        double bjetsNumberThreshold_;
        string bTag_;
        //leptons
        double leptonPtThreshold_;
        double leptonEtaThreshold_;
        vector<double> nonTrigMVAThresholds_;
        vector<double> nonTrigMVAEtaCuts_;
        double electronIsoThreshold_;
        double electronNumOfHitsThreshold_;
        vector<double>  electronEtaCuts_;
        double muPFIsoSumRelThreshold_;
        double TransverseImpactParam_;
        double LongitudinalImpactParam_;
    };

    TTHHadronicTagProducer::TTHHadronicTagProducer( const ParameterSet &iConfig ) :
        diPhotonToken_( consumes<View<flashgg::DiPhotonCandidate> >( iConfig.getParameter<InputTag> ( "DiPhotonTag" ) ) ),
        //thejetToken_( consumes<View<flashgg::Jet> >( iConfig.getParameter<InputTag>( "JetTag" ) ) ),
        inputTagJets_( iConfig.getParameter<std::vector<edm::InputTag> >( "inputTagJets" ) ),
        electronToken_( consumes<View<flashgg::Electron> >( iConfig.getParameter<InputTag>( "ElectronTag" ) ) ),
        muonToken_( consumes<View<flashgg::Muon> >( iConfig.getParameter<InputTag>( "MuonTag" ) ) ),
        vertexToken_( consumes<View<reco::Vertex> >( iConfig.getParameter<InputTag> ( "VertexTag" ) ) ),
        mvaResultToken_( consumes<View<flashgg::DiPhotonMVAResult> >( iConfig.getParameter<InputTag>( "MVAResultTag" ) ) ),
        genParticleToken_( consumes<View<reco::GenParticle> >( iConfig.getParameter<InputTag> ( "GenParticleTag" ) ) ),
        systLabel_( iConfig.getParameter<string> ( "SystLabel" ) )
    {

        double default_MVAThreshold_ = -1.0;
        double default_PhoMVAThreshold_ = -0.9;

        double default_leptonPtThreshold_ = 20.;
        double default_leptonEtaThreshold_ = 2.4;
        vector<double> default_nonTrigMVAThresholds_;
        default_nonTrigMVAThresholds_.push_back(0.913286);
        default_nonTrigMVAThresholds_.push_back(0.805013);
        default_nonTrigMVAThresholds_.push_back(0.358969);

        vector<double> default_nonTrigMVAEtaCuts_;
        default_nonTrigMVAEtaCuts_.push_back(0.8);
        default_nonTrigMVAEtaCuts_.push_back(1.479);
        default_nonTrigMVAEtaCuts_.push_back(2.5);

        vector<double> default_electronEtaCuts_;
        default_electronEtaCuts_.push_back( 1.4442 );
        default_electronEtaCuts_.push_back( 1.566 );
        default_electronEtaCuts_.push_back( 2.5 );

        double default_electronIsoThreshold_ = 0.15;
        double default_electronNumOfHitsThreshold_ = 1.;
        double default_muPFIsoSumRelThreshold_ = 0.25;

        double default_TransverseImpactParam_ = 0.02;
        double default_LongitudinalImpactParam_ = 0.2;

        vector<double> default_bDiscriminator_;
        default_bDiscriminator_.push_back( 0.605 );
        default_bDiscriminator_.push_back( 0.890 );


        MVAThreshold_ = iConfig.getUntrackedParameter<double>( "MVAThreshold_",default_MVAThreshold_ );
        PhoMVAThreshold_ = iConfig.getUntrackedParameter<double>( "PhoMVAThreshold", default_PhoMVAThreshold_ );

        leptonPtThreshold_ = iConfig.getUntrackedParameter<double>( "leptonPtThreshold", default_leptonPtThreshold_ );
        leptonEtaThreshold_ = iConfig.getUntrackedParameter<double>( "leptonEtaThreshold", default_leptonEtaThreshold_ );
        leadPhoPtThreshold_ = iConfig.getUntrackedParameter<double>( "leadPhoPtThreshold", 33 );
        leadPhoUseVariableTh_ = iConfig.getUntrackedParameter<bool>( "leadPhoUseVariableTh", false );
        leadPhoOverMassThreshold_ = iConfig.getUntrackedParameter<double>( "leadPhoOverMassThreshold", 0.5 );
        subleadPhoPtThreshold_ = iConfig.getUntrackedParameter<double>( "leadPhoPtThreshold", 25 );
        subleadPhoUseVariableTh_ = iConfig.getUntrackedParameter<bool>( "subleadPhoUseVariableTh", false );
        subleadPhoOverMassThreshold_ = iConfig.getUntrackedParameter<double>( "subleadPhoOverMassThreshold", 0.25 );
        jetPtThreshold_ = iConfig.getUntrackedParameter<double>( "jetPtThreshold", 30 );
        jetEtaCut_ = iConfig.getUntrackedParameter<double>( "jetEtaCut", 2.4 );
        dRJetPhoLeadCut_ = iConfig.getUntrackedParameter<double>( "dRJetPhoLeadCut", 0.5 );
        dRJetPhoSubleadCut_ = iConfig.getUntrackedParameter<double>( "dRJetPhoSubleadCut", 0.5 );
        bDiscriminator_ = iConfig.getUntrackedParameter<vector<double > >( "bDiscriminator", default_bDiscriminator_ );
        jetsNumberThreshold_ = iConfig.getUntrackedParameter<int>( "jetsNumberThreshold", 5 );
        bjetsNumberThreshold_ = iConfig.getUntrackedParameter<int>( "bjetsNumberThreshold", 1 );
        bTag_ = iConfig.getUntrackedParameter<string> ( "bTag", "combinedInclusiveSecondaryVertexV2BJetTags" );
        muPFIsoSumRelThreshold_ = iConfig.getUntrackedParameter<double>( "muPFIsoSumRelThreshold", default_muPFIsoSumRelThreshold_ );
        nonTrigMVAThresholds_ =  iConfig.getUntrackedParameter<vector<double > >( "nonTrigMVAThresholds", default_nonTrigMVAThresholds_ );
        nonTrigMVAEtaCuts_ =  iConfig.getUntrackedParameter<vector<double > >( "nonTrigMVAEtaCuts", default_nonTrigMVAEtaCuts_ );
        electronIsoThreshold_ = iConfig.getUntrackedParameter<double>( "electronIsoThreshold", default_electronIsoThreshold_ );
        electronNumOfHitsThreshold_ = iConfig.getUntrackedParameter<double>( "electronNumOfHitsThreshold", default_electronNumOfHitsThreshold_ );
        TransverseImpactParam_ = iConfig.getUntrackedParameter<double>( "TransverseImpactParam", default_TransverseImpactParam_ );
        LongitudinalImpactParam_ = iConfig.getUntrackedParameter<double>( "LongitudinalImpactParam", default_LongitudinalImpactParam_ );
        electronEtaCuts_ = iConfig.getUntrackedParameter<vector<double > >( "electronEtaCuts",default_electronEtaCuts_);
        for (unsigned i = 0 ; i < inputTagJets_.size() ; i++) {
            auto token = consumes<View<flashgg::Jet> >(inputTagJets_[i]);
            tokenJets_.push_back(token);
        }

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
            evt.getByToken( tokenJets_[j], Jets[j] );
        }

        Handle<View<flashgg::DiPhotonCandidate> > diPhotons;
        evt.getByToken( diPhotonToken_, diPhotons );
        // const PtrVector<flashgg::DiPhotonCandidate>& diPhotonPointers = diPhotons->ptrVector();

        Handle<View<flashgg::Muon> > theMuons;
        evt.getByToken( muonToken_, theMuons );

        Handle<View<flashgg::Electron> > theElectrons;
        evt.getByToken( electronToken_, theElectrons );

        Handle<View<reco::Vertex> > vertices;
        evt.getByToken( vertexToken_, vertices );

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

        std::vector<edm::Ptr<flashgg::Muon> > goodMuons = selectAllMuons( theMuons->ptrs(), vertices->ptrs(), leptonEtaThreshold_ , leptonPtThreshold_,
                                                                       muPFIsoSumRelThreshold_ );
        
        std::vector<edm::Ptr<Electron> > goodElectrons = selectAllElectrons( theElectrons->ptrs(), vertices->ptrs(), leptonPtThreshold_, 
                                                                          TransverseImpactParam_, LongitudinalImpactParam_, nonTrigMVAThresholds_, nonTrigMVAEtaCuts_,
                                                                          electronIsoThreshold_, electronNumOfHitsThreshold_, electronEtaCuts_ );
        
       


        for( unsigned int diphoIndex = 0; diphoIndex < diPhotons->size(); diphoIndex++ ) {

            if( goodElectrons.size() > 0 ||  goodMuons.size() > 0 )  continue; 

            int jetcount = 0;
            int njets_btagloose = 0;
            int njets_btagmedium = 0;
            double idmva1 = 0.;
            double idmva2 = 0.;

            unsigned int jetCollectionIndex = diPhotons->ptrAt( diphoIndex )->jetCollectionIndex();

            std::vector<edm::Ptr<flashgg::Jet> > JetVect;
            std::vector<edm::Ptr<flashgg::Jet> > BJetVect;

            edm::Ptr<flashgg::DiPhotonCandidate> dipho = diPhotons->ptrAt( diphoIndex );

            idmva1 = dipho->leadingPhoton()->phoIdMvaDWrtVtx( dipho->vtx() );
            idmva2 = dipho->subLeadingPhoton()->phoIdMvaDWrtVtx( dipho->vtx() );

            if( idmva1 <= PhoMVAThreshold_ || idmva2 <= PhoMVAThreshold_ ) { continue; }

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

                if( bDiscriminatorValue > bDiscriminator_[0] ) njets_btagloose++;
                if( bDiscriminatorValue > bDiscriminator_[1] ){
                    
                    njets_btagmedium++;
                    JetVect.pop_back();
                    BJetVect.push_back( thejet );
                }
            }

            if( njets_btagmedium >= bjetsNumberThreshold_ && jetcount >= jetsNumberThreshold_ ) {
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

