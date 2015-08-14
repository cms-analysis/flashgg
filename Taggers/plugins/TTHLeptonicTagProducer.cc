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
#include "flashgg/DataFormats/interface/TTHLeptonicTag.h"
#include "flashgg/DataFormats/interface/Electron.h"
#include "flashgg/DataFormats/interface/Muon.h"
#include "flashgg/DataFormats/interface/Photon.h"
#include "DataFormats/VertexReco/interface/Vertex.h"

#include "DataFormats/TrackReco/interface/HitPattern.h"
#include "flashgg/Taggers/interface/LeptonSelection.h"

#include "DataFormats/Math/interface/deltaR.h"

#include "flashgg/DataFormats/interface/TagTruthBase.h"
#include "DataFormats/Common/interface/RefToPtr.h"

#include <vector>
#include <algorithm>
#include <string>
#include <utility>
#include "TLorentzVector.h"
#include "TMath.h"

using namespace std;
using namespace edm;


namespace flashgg {
    class TTHLeptonicTagProducer : public EDProducer
    {

    public:
        typedef math::XYZPoint Point;

        TTHLeptonicTagProducer( const ParameterSet & );
    private:
        void produce( Event &, const EventSetup & ) override;

        EDGetTokenT<View<DiPhotonCandidate> > diPhotonToken_;
        //EDGetTokenT<View<Jet> > thejetToken_;
        std::vector<edm::InputTag> inputTagJets_;
        EDGetTokenT<View<Electron> > electronToken_;
        EDGetTokenT<View<flashgg::Muon> > muonToken_;
        EDGetTokenT<View<DiPhotonMVAResult> > mvaResultToken_;
        EDGetTokenT<View<Photon> > photonToken_;
        EDGetTokenT<View<reco::Vertex> > vertexToken_;
        EDGetTokenT<View<reco::GenParticle> > genParticleToken_;
        string systLabel_;

        typedef std::vector<edm::Handle<edm::View<flashgg::Jet> > > JetCollectionVector;

        //Thresholds
        double leptonPtThreshold_;
        double leptonEtaThreshold_;
        double leadPhoOverMassThreshold_;
        double subleadPhoOverMassThreshold_;
        double MVAThreshold_;
        double deltaRLepPhoThreshold_;
        double deltaRJetLepThreshold_;

        double deltaRJetLeadPhoThreshold_;
        double deltaRJetSubLeadPhoThreshold_;

        double LowPtEtaPhoThreshold_;
        double MidPtEtaPhoThreshold_;
        double HighEtaPhoThreshold_;


        double jetsNumberThreshold_;
        double bjetsNumberThreshold_;
        double jetPtThreshold_;
        double jetEtaThreshold_;

        vector<double> bDiscriminator_;
        string bTag_;
        double muPFIsoSumRelThreshold_;
        double deltaRMuonJetcountThreshold_;
        double deltaRElectronJetcountThreshold_;
        double PhoMVAThreshold_;
        double ElectronPtThreshold_;
        double DeltaRTrkElec_;
        double TransverseImpactParam_;
        double LongitudinalImpactParam_;

        double deltaRPhoElectronThreshold_;
        double Zmass_;
        double deltaMassElectronZThreshold_;

        double nonTrigMVAThreshold_;
        double electronIsoThreshold_;
        double electronNumOfHitsThreshold_;
        vector<double> EtaCuts_;

        bool hasGoodElec = false;
        bool hasGoodMuons = false;

    };

    TTHLeptonicTagProducer::TTHLeptonicTagProducer( const ParameterSet &iConfig ) :
        diPhotonToken_( consumes<View<flashgg::DiPhotonCandidate> >( iConfig.getParameter<InputTag> ( "DiPhotonTag" ) ) ),
        //thejetToken_( consumes<View<flashgg::Jet> >( iConfig.getParameter<InputTag>( "JetTag" ) ) ),
        inputTagJets_( iConfig.getParameter<std::vector<edm::InputTag> >( "inputTagJets" ) ),
        electronToken_( consumes<View<flashgg::Electron> >( iConfig.getParameter<InputTag>( "ElectronTag" ) ) ),
        muonToken_( consumes<View<flashgg::Muon> >( iConfig.getParameter<InputTag>( "MuonTag" ) ) ),
        mvaResultToken_( consumes<View<flashgg::DiPhotonMVAResult> >( iConfig.getParameter<InputTag> ( "MVAResultTag" ) ) ),
        vertexToken_( consumes<View<reco::Vertex> >( iConfig.getParameter<InputTag> ( "VertexTag" ) ) ),
        genParticleToken_( consumes<View<reco::GenParticle> >( iConfig.getParameter<InputTag> ( "GenParticleTag" ) ) ),
        systLabel_( iConfig.getParameter<string> ( "SystLabel" ) )
    {

        double default_leptonPtThreshold_ = 20.;
        double default_leptonEtaThreshold_ = 2.4;
        double default_leadPhoOverMassThreshold_ = 0.5;
        double default_subleadPhoOverMassThreshold_ = 0.25;
        double default_MVAThreshold_ = -0.6;
        double default_deltaRLepPhoThreshold_ = 0.5;
        double default_deltaRJetLepThreshold_ = 0.5;
        double default_jetsNumberThreshold_ = 2;
        double default_bjetsNumberThreshold_ = 1;
        double default_jetPtThreshold_ = 30.;
        double default_jetEtaThreshold_ = 2.4;


        double default_deltaRJetLeadPhoThreshold_ = 0.5;
        double default_deltaRJetSubLeadPhoThreshold_ = 0.5;


        vector<double> default_bDiscriminator_;
        default_bDiscriminator_.push_back( 0.244 );
        default_bDiscriminator_.push_back( 0.679 );

        string default_bTag_ = "combinedInclusiveSecondaryVertexV2BJetTags";
        double default_muPFIsoSumRelThreshold_ = 0.2;
        double default_deltaRMuonJetcountThreshold_ = 2.;
        double default_PhoMVAThreshold_ = -0.2;
        double default_ElectronPtThreshold_ = 20.;
        double default_DeltaRTrkElec_ = 1.;
        double default_TransverseImpactParam_ = 0.02;
        double default_LongitudinalImpactParam_ = 0.2;

        double default_LowPtEtaPhoThreshold_ = 1.4447;
        double default_MidPtEtaPhoThreshold_ = 1.566;
        double default_HighEtaPhoThreshold_ = 2.5;

        double default_deltaRPhoElectronThreshold_ = 1.;
        double default_Zmass_ = 91.9;
        double default_deltaMassElectronZThreshold_ = 10.;

        double default_nonTrigMVAThreshold_ = 0.9;
        double default_electronIsoThreshold_ = 0.15;
        double default_electronNumOfHitsThreshold_ = 1.;

        vector<double> default_EtaCuts_;
        default_EtaCuts_.push_back( 1.442 );
        default_EtaCuts_.push_back( 1.566 );
        default_EtaCuts_.push_back( 2.5 );

        leptonPtThreshold_ = iConfig.getUntrackedParameter<double>( "leptonPtThreshold", default_leptonPtThreshold_ );
        leptonEtaThreshold_ = iConfig.getUntrackedParameter<double>( "leptonEtaThreshold", default_leptonEtaThreshold_ );
        leadPhoOverMassThreshold_ = iConfig.getUntrackedParameter<double>( "leadPhoOverMassThreshold", default_leadPhoOverMassThreshold_ );
        subleadPhoOverMassThreshold_ = iConfig.getUntrackedParameter<double>( "subleadPhoOverMassThreshold", default_subleadPhoOverMassThreshold_ );
        MVAThreshold_ = iConfig.getUntrackedParameter<double>( "MVAThreshold", default_MVAThreshold_ );
        deltaRLepPhoThreshold_ = iConfig.getUntrackedParameter<double>( "deltaRLepPhoThreshold", default_deltaRLepPhoThreshold_ );
        deltaRJetLepThreshold_ = iConfig.getUntrackedParameter<double>( "deltaRJetLepThreshold", default_deltaRJetLepThreshold_ );
        jetsNumberThreshold_ = iConfig.getUntrackedParameter<double>( "jetsNumberThreshold", default_jetsNumberThreshold_ );
        bjetsNumberThreshold_ = iConfig.getUntrackedParameter<double>( "bjetsNumberThreshold", default_bjetsNumberThreshold_ );
        jetPtThreshold_ = iConfig.getUntrackedParameter<double>( "jetPtThreshold", default_jetPtThreshold_ );
        jetEtaThreshold_ = iConfig.getUntrackedParameter<double>( "jetEtaThreshold", default_jetEtaThreshold_ );

        deltaRJetLeadPhoThreshold_ = iConfig.getUntrackedParameter<double>( "deltaRJetLeadPhoThreshold", default_deltaRJetLeadPhoThreshold_ );
        deltaRJetSubLeadPhoThreshold_ = iConfig.getUntrackedParameter<double>( "deltaRJetSubLeadPhoThreshold", default_deltaRJetSubLeadPhoThreshold_ );

        bDiscriminator_ = iConfig.getUntrackedParameter<vector<double > >( "bDiscriminator", default_bDiscriminator_ );
        bTag_ = iConfig.getUntrackedParameter<string>( "bTag", default_bTag_ );

        muPFIsoSumRelThreshold_ = iConfig.getUntrackedParameter<double>( "muPFIsoSumRelThreshold", default_muPFIsoSumRelThreshold_ );
        deltaRMuonJetcountThreshold_ = iConfig.getUntrackedParameter<double>( "deltaRMuonJetcountThreshold", default_deltaRMuonJetcountThreshold_ );
        deltaRElectronJetcountThreshold_ = iConfig.getUntrackedParameter<double>( "deltaRMuonJetcountThreshold", default_deltaRMuonJetcountThreshold_ );
        PhoMVAThreshold_ = iConfig.getUntrackedParameter<double>( "PhoMVAThreshold", default_PhoMVAThreshold_ );
        ElectronPtThreshold_ = iConfig.getUntrackedParameter<double>( "ElectronPtThreshold", default_ElectronPtThreshold_ );
        DeltaRTrkElec_ = iConfig.getUntrackedParameter<double>( "DeltaRTrkElec", default_DeltaRTrkElec_ );
        TransverseImpactParam_ = iConfig.getUntrackedParameter<double>( "TransverseImpactParam", default_TransverseImpactParam_ );
        LongitudinalImpactParam_ = iConfig.getUntrackedParameter<double>( "LongitudinalImpactParam", default_LongitudinalImpactParam_ );

        LowPtEtaPhoThreshold_ = iConfig.getUntrackedParameter<double>( "LowPtEtaPhoThreshold", default_LowPtEtaPhoThreshold_ );
        MidPtEtaPhoThreshold_ = iConfig.getUntrackedParameter<double>( "MidPtEtaPhoThreshold", default_MidPtEtaPhoThreshold_ );
        HighEtaPhoThreshold_ = iConfig.getUntrackedParameter<double>( "HighEtaPhoThreshold", default_HighEtaPhoThreshold_ );

        deltaRPhoElectronThreshold_ = iConfig.getUntrackedParameter<double>( "deltaRPhoElectronThreshold", default_deltaRPhoElectronThreshold_ );
        Zmass_ = iConfig.getUntrackedParameter<double>( "Zmass_", default_Zmass_ );
        deltaMassElectronZThreshold_ = iConfig.getUntrackedParameter<double>( "deltaMassElectronZThreshold_", default_deltaMassElectronZThreshold_ );

        nonTrigMVAThreshold_ = iConfig.getUntrackedParameter<double>( "nonTrigMVAThreshold", default_nonTrigMVAThreshold_ );
        electronIsoThreshold_ = iConfig.getUntrackedParameter<double>( "electronIsoThreshold", default_electronIsoThreshold_ );
        electronNumOfHitsThreshold_ = iConfig.getUntrackedParameter<double>( "electronNumOfHitsThreshold", default_electronNumOfHitsThreshold_ );

        EtaCuts_ = iConfig.getUntrackedParameter<vector<double > >( "EtaCuts", default_EtaCuts_ );

        produces<vector<TTHLeptonicTag> >();
        produces<vector<TagTruthBase> >();
    }

    void TTHLeptonicTagProducer::produce( Event &evt, const EventSetup & )

    {

        //Handle<View<flashgg::Jet> > theJets;
        //evt.getByToken( thejetToken_, theJets );
        //const PtrVector<flashgg::Jet>& jetPointers = theJets->ptrVector();
        JetCollectionVector Jets( inputTagJets_.size() );
        for( size_t j = 0; j < inputTagJets_.size(); ++j ) {
            evt.getByLabel( inputTagJets_[j], Jets[j] );
        }

        Handle<View<flashgg::DiPhotonCandidate> > diPhotons;
        evt.getByToken( diPhotonToken_, diPhotons );
        //	const PtrVector<flashgg::DiPhotonCandidate>& diPhotonPointers = diPhotons->ptrVector();

        Handle<View<flashgg::Muon> > theMuons;
        evt.getByToken( muonToken_, theMuons );
        //		const PtrVector<flashgg::Muon>& muonPointers = theMuons->ptrVector();

        Handle<View<flashgg::Electron> > theElectrons;
        evt.getByToken( electronToken_, theElectrons );

        //		const PtrVector<flashgg::Electron>& electronPointers = theElectrons->ptrVector();

        Handle<View<flashgg::DiPhotonMVAResult> > mvaResults;
        evt.getByToken( mvaResultToken_, mvaResults );
        //		const PtrVector<flashgg::DiPhotonMVAResult>& mvaResultPointers = mvaResults->ptrVector();

        Handle<View<reco::GenParticle> > genParticles;
        evt.getByToken( genParticleToken_, genParticles );

        std::auto_ptr<vector<TTHLeptonicTag> > tthltags( new vector<TTHLeptonicTag> );

        Handle<View<reco::Vertex> > vertices;
        evt.getByToken( vertexToken_, vertices );
        //	const PtrVector<reco::Vertex>& vertexPointers = vertices->ptrVector();

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

        bool photonSelection = false;
        double idmva1 = 0.;
        double idmva2 = 0.;

        vector<int> numMuonJetsdR;
        vector<int> numElectronJetsdR;
        bool muonJets = false;
        bool ElectronJets = false;

        for( unsigned int diphoIndex = 0; diphoIndex < diPhotons->size(); diphoIndex++ ) {

            hasGoodElec = false;
            hasGoodMuons = false;

            unsigned int jetCollectionIndex = diPhotons->ptrAt( diphoIndex )->jetCollectionIndex();

            std::vector<edm::Ptr<Muon> > tagMuons;
            std::vector<edm::Ptr<Electron> > tagElectrons;
            std::vector<edm::Ptr<Jet> > tagJets;
            std::vector<edm::Ptr<Jet> > tagBJets;

            edm::Ptr<flashgg::DiPhotonCandidate> dipho = diPhotons->ptrAt( diphoIndex );
            edm::Ptr<flashgg::DiPhotonMVAResult> mvares = mvaResults->ptrAt( diphoIndex );

            TTHLeptonicTag tthltags_obj( dipho, mvares );

            if( dipho->leadingPhoton()->pt() < ( dipho->mass() )*leadPhoOverMassThreshold_ ) { continue; }

            if( dipho->subLeadingPhoton()->pt() < ( dipho->mass() )*subleadPhoOverMassThreshold_ ) { continue; }

            if( ( fabs( dipho->leadingPhoton()->superCluster()->eta() ) > LowPtEtaPhoThreshold_ &&
                    fabs( dipho->leadingPhoton()->superCluster()->eta() ) < MidPtEtaPhoThreshold_ ) ||
                    fabs( dipho->leadingPhoton()->superCluster()->eta() ) > HighEtaPhoThreshold_ ) { continue; }

            if( ( fabs( dipho->subLeadingPhoton()->superCluster()->eta() ) > LowPtEtaPhoThreshold_ &&
                    fabs( dipho->subLeadingPhoton()->superCluster()->eta() ) < MidPtEtaPhoThreshold_ ) ||
                    fabs( dipho->subLeadingPhoton()->superCluster()->eta() ) > HighEtaPhoThreshold_ ) { continue; }

            idmva1 = dipho->leadingPhoton()->phoIdMvaDWrtVtx( dipho->vtx() );
            idmva2 = dipho->subLeadingPhoton()->phoIdMvaDWrtVtx( dipho->vtx() );

            if( idmva1 <= PhoMVAThreshold_ || idmva2 <= PhoMVAThreshold_ ) { continue; }

            if( mvares->result < MVAThreshold_ ) { continue; }

            photonSelection = true;

            std::vector<edm::Ptr<flashgg::Muon> > goodMuons = selectMuons( theMuons->ptrs(), dipho, vertices->ptrs(), leptonEtaThreshold_ , leptonPtThreshold_,
                    muPFIsoSumRelThreshold_, deltaRLepPhoThreshold_, deltaRLepPhoThreshold_ );

            std::vector<edm::Ptr<Electron> > goodElectrons = selectElectrons( theElectrons->ptrs(), vertices->ptrs(), ElectronPtThreshold_, DeltaRTrkElec_,
                    TransverseImpactParam_, LongitudinalImpactParam_, nonTrigMVAThreshold_, electronIsoThreshold_, electronNumOfHitsThreshold_, EtaCuts_ );

            hasGoodElec = ( goodElectrons.size() > 0 );
            hasGoodMuons = ( goodMuons.size() > 0 );
            if( !hasGoodElec && !hasGoodMuons ) { continue; }

            numMuonJetsdR.clear();
            numElectronJetsdR.clear();
            muonJets = false;
            ElectronJets = false;

            if( hasGoodMuons ) {

                for( unsigned int muonIndex = 0; muonIndex < goodMuons.size(); muonIndex++ ) {

                    Ptr<flashgg::Muon> muon = goodMuons[muonIndex];

                    int deltaRMuonJetcount = 0;
                    double bDiscriminatorValue = -999.;


                    for( unsigned int candIndex_outer = 0; candIndex_outer < Jets[jetCollectionIndex]->size() ; candIndex_outer++ ) {
                        edm::Ptr<flashgg::Jet> thejet = Jets[jetCollectionIndex]->ptrAt( candIndex_outer );

                        if( !thejet->passesPuJetId( dipho ) ) { continue; }

                        if( fabs( thejet->eta() ) > jetEtaThreshold_ ) { continue; }

                        if( thejet->pt() < jetPtThreshold_ ) { continue; }

                        float dRPhoLeadJet = deltaR( thejet->eta(), thejet->phi(), dipho->leadingPhoton()->superCluster()->eta(), dipho->leadingPhoton()->superCluster()->phi() ) ;
                        float dRPhoSubLeadJet = deltaR( thejet->eta(), thejet->phi(), dipho->subLeadingPhoton()->superCluster()->eta(),
                                                        dipho->subLeadingPhoton()->superCluster()->phi() );

                        if( dRPhoLeadJet < deltaRJetLeadPhoThreshold_ || dRPhoSubLeadJet < deltaRJetSubLeadPhoThreshold_ ) { continue; }

                        float dRJetMuon = deltaR( thejet->eta(), thejet->phi(), muon->eta(), muon->phi() ) ;

                        if( dRJetMuon < deltaRJetLepThreshold_ ) { continue; }
                        deltaRMuonJetcount++;

                        tagJets.push_back( thejet );

                        bDiscriminatorValue = thejet->bDiscriminator( bTag_.c_str() );

                        if( bDiscriminatorValue > bDiscriminator_[1] ) {
                            tagBJets.push_back( thejet );
                        }


                    }//end of jets loop

                    numMuonJetsdR.push_back( deltaRMuonJetcount );
                    tagMuons.push_back( muon );

                }//end of muons loop

            }

            if( hasGoodElec ) {

                bool photon_veto = false;

                std::vector<const flashgg::Photon *> photons;

                photons.push_back( dipho->leadingPhoton() );
                photons.push_back( dipho->subLeadingPhoton() );


                for( unsigned int ElectronIndex = 0; ElectronIndex < goodElectrons.size(); ElectronIndex++ ) {

                    Ptr<Electron> Electron = goodElectrons[ElectronIndex];

                    TLorentzVector elec_p4;
                    elec_p4.SetXYZT( Electron->px(), Electron->py(), Electron->pz(), Electron->energy() );

                    float phi = Electron->superCluster()->phi();
                    float theta = ( 2 * atan( exp( -Electron->superCluster()->eta() ) ) );
                    float energy = Electron->ecalEnergy();
                    float px = energy * sin( theta ) * cos( phi );
                    float py = energy * sin( theta ) * sin( phi );
                    float pz = energy * cos( theta );

                    TLorentzVector elec_superClusterVect;

                    elec_superClusterVect.SetXYZT( px, py, pz, energy );

                    for( unsigned int phoIndex = 0; phoIndex < photons.size(); phoIndex++ ) {

                        TLorentzVector p;

                        p.SetXYZT( photons.at( phoIndex )->px(), photons.at( phoIndex )->py(), photons.at( phoIndex )->pz(), photons.at( phoIndex )->energy() );
                        if( p.DeltaR( elec_superClusterVect ) < 1. ) { continue; }

                        if( &( *photons.at( phoIndex )->superCluster() ) == &( *Electron->superCluster() ) ) {

                            float TrkElecSCDeltaR = sqrt( Electron->deltaEtaSuperClusterTrackAtVtx() * Electron->deltaEtaSuperClusterTrackAtVtx() +
                                                          Electron->deltaPhiSuperClusterTrackAtVtx() * Electron->deltaPhiSuperClusterTrackAtVtx() );
                            if( TrkElecSCDeltaR < DeltaRTrkElec_ ) { continue; }
                        }

                        if( p.DeltaR( elec_p4 ) <= deltaRPhoElectronThreshold_ ) { continue; }
                        TLorentzVector elep = elec_p4 + p;
                        if( fabs( elep.M() - Zmass_ ) <= deltaMassElectronZThreshold_ ) { continue; }

                        photon_veto = true;

                    }

                    int deltaRElectronJetcount = 0;
                    double bDiscriminatorValue = -999.;

                    if( !photon_veto ) { break; }

                    for( unsigned int candIndex_outer = 0; candIndex_outer < Jets[jetCollectionIndex]->size() ; candIndex_outer++ ) {
                        edm::Ptr<flashgg::Jet> thejet = Jets[jetCollectionIndex]->ptrAt( candIndex_outer );

                        if( !thejet->passesPuJetId( dipho ) ) { continue; }

                        //https://github.com/h2gglobe/h2gglobe/blob/master/PhotonAnalysis/src/PhotonAnalysis.cc#L5367
                        if( fabs( thejet->eta() ) > jetEtaThreshold_ ) { continue; }

                        //https://github.com/h2gglobe/h2gglobe/blob/master/PhotonAnalysis/src/PhotonAnalysis.cc#L5371
                        if( thejet->pt() < jetPtThreshold_ ) { continue; }

                        float dRJetElectron = deltaR( thejet->eta(), thejet->phi(), Electron->eta(), Electron->phi() ) ;

                        //https://github.com/njets_btagmediumh2gglobe/h2gglobe/blob/master/PhotonAnalysis/src/PhotonAnalysis.cc#L5370
                        if( dRJetElectron < deltaRJetLepThreshold_ ) { continue; }
                        deltaRElectronJetcount++;

                        tagJets.push_back( thejet );

                        bDiscriminatorValue = thejet->bDiscriminator( bTag_.c_str() );

                        if( bDiscriminatorValue > bDiscriminator_[1] ) {
                            tagBJets.push_back( thejet );
                        }

                    }//end of jets loop
                    numElectronJetsdR.push_back( deltaRElectronJetcount );

                    tagElectrons.push_back( Electron );

                }//end of electron loop

            }


            for( unsigned num = 0; num < numMuonJetsdR.size(); num++ ) {
                int check = numMuonJetsdR.at( num );
                if( check >= deltaRMuonJetcountThreshold_ ) {muonJets = true;}
            }

            for( unsigned num = 0; num < numElectronJetsdR.size(); num++ ) {
                int check = numElectronJetsdR.at( num );
                if( check >= deltaRElectronJetcountThreshold_ ) {ElectronJets = true;}
            }

            /*
            std::cout << " TTHLeptonicTagProducer tagBJets.size()=" << tagBJets.size()
                      << " tagJets.size()=" << tagJets.size()
                      << " photonSelection=" << photonSelection
                      << " tagMuons.size()=" << tagMuons.size() << " muonJets=" << muonJets
                      << " tagElectrons.size()="<< tagElectrons.size() << " ElectronJets=" << ElectronJets
                      << std::endl;
            */

            if( tagBJets.size() >= bjetsNumberThreshold_ && tagJets.size() >= jetsNumberThreshold_ && photonSelection
                    && ( ( tagMuons.size() > 0 && muonJets ) || ( tagElectrons.size() > 0 && ElectronJets ) ) ) {
                //                std::cout << " TTHLeptonicTagProducer TAGGED " << std::endl;
                if( tagElectrons.size() > 0 && ElectronJets ) {
                    //                    std::cout << "including electron weights" << std::endl;
                    tthltags_obj.includeWeights( *tagElectrons[0] );
                } else if( tagMuons.size() > 0 && muonJets ) {
                    //                    std::cout << "including muon weights" << std::endl;
                    tthltags_obj.includeWeights( *tagMuons[0] );
                }

                tthltags_obj.setJets( tagJets );
                tthltags_obj.setBJets( tagBJets );
                tthltags_obj.setMuons( tagMuons );
                tthltags_obj.setElectrons( tagElectrons );
                tthltags_obj.setDiPhotonIndex( diphoIndex );
                tthltags_obj.setSystLabel( systLabel_ );
                tthltags->push_back( tthltags_obj );
                TagTruthBase truth_obj;
                if( ! evt.isRealData() ) {
                    truth_obj.setGenPV( higgsVtx );
                    truths->push_back( truth_obj );
                    tthltags->back().setTagTruth( edm::refToPtr( edm::Ref<vector<TagTruthBase> >( rTagTruth, idx++ ) ) );
                }
            } else {
                //                std::cout << " TTHLeptonicTagProducer NO TAG " << std::endl;
            }

        }//diPho loop end !
        evt.put( tthltags );
        evt.put( truths );
    }

}
typedef flashgg::TTHLeptonicTagProducer FlashggTTHLeptonicTagProducer;
DEFINE_FWK_MODULE( FlashggTTHLeptonicTagProducer );
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

