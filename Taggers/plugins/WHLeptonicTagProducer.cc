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
#include "flashgg/DataFormats/interface/WHLeptonicTag.h"
#include "flashgg/DataFormats/interface/Electron.h"
#include "flashgg/DataFormats/interface/Muon.h"
#include "flashgg/DataFormats/interface/Met.h"

#include "FWCore/Common/interface/TriggerNames.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "DataFormats/TrackReco/interface/HitPattern.h"
#include "flashgg/Taggers/interface/LeptonSelection.h"

#include "DataFormats/Math/interface/deltaR.h"

#include "flashgg/DataFormats/interface/VHTagTruth.h"
#include "flashgg/DataFormats/interface/TagTruthBase.h"
#include "DataFormats/Common/interface/RefToPtr.h"

#include <vector>
#include <algorithm>
#include <string>
#include <utility>
#include "TLorentzVector.h"
#include "TMath.h"
#include "TMVA/Reader.h"

using namespace std;
using namespace edm;


namespace flashgg {
    class WHLeptonicTagProducer : public EDProducer
    {

    public:
        WHLeptonicTagProducer( const ParameterSet & );
    private:
        void produce( Event &, const EventSetup & ) override;
        int  chooseCategory( float, float );
        int  computeStage1Kinematics( const WHLeptonicTag );

        EDGetTokenT<View<DiPhotonCandidate> > diPhotonToken_;
        EDGetTokenT<View<Electron> > electronToken_;
        EDGetTokenT<View<flashgg::Muon> > muonToken_;
        std::vector<edm::EDGetTokenT<View<flashgg::Jet> > > tokenJets_;
        std::vector<edm::InputTag> inputTagJets_;
        EDGetTokenT<View<flashgg::Met> > METToken_;
        EDGetTokenT<View<reco::Vertex> > vertexToken_;
        EDGetTokenT<double> rhoTag_;
        EDGetTokenT<View<DiPhotonMVAResult> > mvaResultToken_;
        EDGetTokenT<View<reco::GenParticle> > genParticleToken_;
        string systLabel_;

        typedef std::vector<edm::Handle<edm::View<flashgg::Jet> > > JetCollectionVector;

        //Thresholds
        double leadPhoOverMassThreshold_      ; 
        double subleadPhoOverMassThreshold_   ; 
        double PhoMVAThreshold_               ; 
        double MVAThreshold_                  ; 
        double leptonPtThreshold_             ; 
        vector<double> electronEtaThresholds_ ; 
        bool useElectronMVARecipe_            ; 
        bool useElectronLooseID_              ; 
        double deltaRPhoElectronThreshold_    ; 
        double DeltaRTrkElec_                 ; 
        double deltaMassElectronZThreshold_   ; 
        double muonEtaThreshold_              ; 
        double muPFIsoSumRelThreshold_        ; 
        double deltaRMuonPhoThreshold_        ; 
        double jetsNumberThreshold_           ; 
        double jetPtThreshold_                ; 
        double jetEtaThreshold_               ; 
        double deltaRPhoLeadJet_              ; 
        double deltaRPhoSubLeadJet_           ; 
        double deltaRJetMuonThreshold_        ; 
        double METThreshold_                  ; 

        //WHMVA
        unique_ptr<TMVA::Reader>WHMva_;

        float _pho1_eta     ; 
        float _pho2_eta     ; 
        float _pho1_ptoM    ; 
        float _pho2_ptoM    ; 
        float _min_phoId    ; 
        float _max_phoId    ; 
        float _pho1_psv     ; 
        float _pho2_psv     ; 
        float _dipho_cosphi ; 
        float _njet         ; 
        float _mtlnu        ; 
        float _dr_pho1_lep  ; 
        float _dr_pho2_lep  ; 
        float _theta        ; 
        float _met          ; 
        float _lep_pt       ; 
        float _lep_eta      ; 
        float _jet1_csv     ; 
        float _jet2_csv     ; 
        float _jet1_pt      ; 
        float _leptonType   ; 

        FileInPath WHMVAweightfile_ ;

        vector<double> boundaries_GT75;
        vector<double> boundaries_0_75;

    };

    WHLeptonicTagProducer::WHLeptonicTagProducer( const ParameterSet &iConfig ) :
        diPhotonToken_( consumes<View<flashgg::DiPhotonCandidate> >( iConfig.getParameter<InputTag> ( "DiPhotonTag" ) ) ),
        electronToken_( consumes<View<flashgg::Electron> >( iConfig.getParameter<InputTag> ( "ElectronTag" ) ) ),
        muonToken_( consumes<View<flashgg::Muon> >( iConfig.getParameter<InputTag>( "MuonTag" ) ) ),
        inputTagJets_( iConfig.getParameter<std::vector<edm::InputTag> >( "inputTagJets" ) ),
        METToken_( consumes<View<flashgg::Met> >( iConfig.getParameter<InputTag> ( "METTag" ) ) ),
        vertexToken_( consumes<View<reco::Vertex> >( iConfig.getParameter<InputTag> ( "VertexTag" ) ) ),
        rhoTag_( consumes<double>( iConfig.getParameter<InputTag>( "rhoTag" ) ) ),
        mvaResultToken_( consumes<View<flashgg::DiPhotonMVAResult> >( iConfig.getParameter<InputTag> ( "MVAResultTag" ) ) ),
        genParticleToken_( consumes<View<reco::GenParticle> >( iConfig.getParameter<InputTag> ( "GenParticleTag" ) ) ),
        systLabel_( iConfig.getParameter<string> ( "SystLabel" ) )
    {

        //Photon
        leadPhoOverMassThreshold_    = iConfig.getParameter<double>( "leadPhoOverMassThreshold");
        subleadPhoOverMassThreshold_ = iConfig.getParameter<double>( "subleadPhoOverMassThreshold");
        PhoMVAThreshold_             = iConfig.getParameter<double>( "PhoMVAThreshold");
        MVAThreshold_                = iConfig.getParameter<double>( "MVAThreshold");

        //Electron
        leptonPtThreshold_           = iConfig.getParameter<double>( "leptonPtThreshold");
        electronEtaThresholds_       = iConfig.getParameter<vector<double > >( "electronEtaThresholds");
        useElectronMVARecipe_        = iConfig.getParameter<bool>("useElectronMVARecipe");
        useElectronLooseID_          = iConfig.getParameter<bool>("useElectronLooseID");
        deltaRPhoElectronThreshold_  = iConfig.getParameter<double>( "deltaRPhoElectronThreshold");
        DeltaRTrkElec_               = iConfig.getParameter<double>( "DeltaRTrkElec");
        deltaMassElectronZThreshold_ = iConfig.getParameter<double>( "deltaMassElectronZThreshold");
        
        //Muon
        muonEtaThreshold_            = iConfig.getParameter<double>( "muonEtaThreshold");
        muPFIsoSumRelThreshold_      = iConfig.getParameter<double>( "muPFIsoSumRelThreshold");
        deltaRMuonPhoThreshold_      = iConfig.getParameter<double>( "deltaRMuonPhoThreshold");

        //Jet
        jetsNumberThreshold_         = iConfig.getParameter<double>( "jetsNumberThreshold");
        jetPtThreshold_              = iConfig.getParameter<double>( "jetPtThreshold");
        jetEtaThreshold_             = iConfig.getParameter<double>( "jetEtaThreshold");
        deltaRPhoLeadJet_            = iConfig.getParameter<double>( "deltaRPhoLeadJet");
        deltaRPhoSubLeadJet_         = iConfig.getParameter<double>( "deltaRPhoSubLeadJet");
        deltaRJetMuonThreshold_      = iConfig.getParameter<double>( "deltaRJetMuonThreshold");

        //Met
        METThreshold_                = iConfig.getParameter<double>( "METThreshold");

        for (unsigned i = 0 ; i < inputTagJets_.size() ; i++) {
            auto token = consumes<View<flashgg::Jet> >(inputTagJets_[i]);
            tokenJets_.push_back(token);
        }
        
        //WHMVA
        WHMVAweightfile_ = iConfig.getParameter<edm::FileInPath>( "WHMVAweightfile" );
       
        _pho1_eta     = -999.; 
        _pho2_eta     = -999.; 
        _pho1_ptoM    = -999.; 
        _pho2_ptoM    = -999.; 
        _min_phoId    = -999.; 
        _max_phoId    = -999.; 
        _pho1_psv     = -999.; 
        _pho2_psv     = -999.; 
        _dipho_cosphi = -999.; 
        _njet         = -999.; 
        _mtlnu        = -999.; 
        _dr_pho1_lep  = -999.; 
        _dr_pho2_lep  = -999.; 
        _theta        = -999.; 
        _met          = -999.; 
        _lep_pt       = -999.; 
        _lep_eta      = -999.; 
        _jet1_csv     = -999.; 
        _jet2_csv     = -999.; 
        _jet1_pt      = -999.; 
        _leptonType   = -999.; 

        WHMva_.reset( new TMVA::Reader( "!Color:!Silent" ) );
        WHMva_->AddVariable( "pho1_eta"          ,&_pho1_eta      ); 
        WHMva_->AddVariable( "pho2_eta"          ,&_pho2_eta      ); 
        WHMva_->AddVariable( "pho1_ptoM"         ,&_pho1_ptoM     ); 
        WHMva_->AddVariable( "pho2_ptoM"         ,&_pho2_ptoM     ); 
        WHMva_->AddVariable( "min_phoId"         ,&_min_phoId     ); 
        WHMva_->AddVariable( "max_phoId"         ,&_max_phoId     ); 
        WHMva_->AddVariable( "pho1_psv"          ,&_pho1_psv      ); 
        WHMva_->AddVariable( "pho2_psv"          ,&_pho2_psv      ); 
        WHMva_->AddVariable( "dipho_cosphi"      ,&_dipho_cosphi  ); 
        WHMva_->AddVariable( "njet"              ,&_njet          ); 
        WHMva_->AddVariable( "mtlnu"             ,&_mtlnu         ); 
        WHMva_->AddVariable( "dr_pho1_lep"       ,&_dr_pho1_lep   ); 
        WHMva_->AddVariable( "dr_pho2_lep"       ,&_dr_pho2_lep   ); 
        WHMva_->AddVariable( "theta"             ,&_theta         ); 
        WHMva_->AddVariable( "met"               ,&_met           ); 
        WHMva_->AddVariable( "lep_pt"            ,&_lep_pt        ); 
        WHMva_->AddVariable( "lep_eta"           ,&_lep_eta       ); 
        WHMva_->AddVariable( "jet1_csv"          ,&_jet1_csv      ); 
        WHMva_->AddVariable( "jet2_csv"          ,&_jet2_csv      ); 
        WHMva_->AddVariable( "jet1_pt"           ,&_jet1_pt       ); 
        WHMva_->AddVariable( "leptonType"        ,&_leptonType    ); 
        WHMva_->BookMVA( "BDT", WHMVAweightfile_.fullPath() );

        boundaries_GT75 = iConfig.getParameter<vector<double > >( "Boundaries_GT75" );
        boundaries_0_75 = iConfig.getParameter<vector<double > >( "Boundaries_0_75" );
        assert( is_sorted( boundaries_GT75.begin(), boundaries_GT75.end() ) ); // we are counting on ascending order - update this to give an error message or exception
        assert( is_sorted( boundaries_0_75.begin(), boundaries_0_75.end() ) ); // we are counting on ascending order - update this to give an error message or exception


        produces<vector<WHLeptonicTag> >();
        //produces<vector<VHTagTruth> >();
    }

    int WHLeptonicTagProducer::chooseCategory( float mva, float ptV )
    {
        // should return 0 if mva above all the numbers, 1 if below the first, ..., boundaries.size()-N if below the Nth, ...
        int n;
        int n_GT75 = boundaries_GT75.size();
        int n_0_75 = boundaries_0_75.size();

        if (ptV > 75.) {
            for( n = 0 ; n < n_GT75 ; n++ ) {
                if( ( double )mva > boundaries_GT75[n_GT75 - n - 1] ) { return n; }
            }
        } else if (ptV > 0.) {
            for( n = n_GT75 ; n < n_0_75 + n_GT75; n++ ) {
                if( ( double )mva > boundaries_0_75[n_0_75 + n_GT75 - n - 1] ) { return n; }
            }
        }

        return -1; // Does not pass, object will not be produced
    }

    void WHLeptonicTagProducer::produce( Event &evt, const EventSetup & )
    {
        Handle<View<flashgg::DiPhotonCandidate> > diPhotons;
        evt.getByToken( diPhotonToken_, diPhotons );

        Handle<View<flashgg::Electron> > theElectrons;
        evt.getByToken( electronToken_, theElectrons );

        Handle<View<flashgg::Muon> > theMuons;
        evt.getByToken( muonToken_, theMuons );

        JetCollectionVector Jets( inputTagJets_.size() );
        for( size_t j = 0; j < inputTagJets_.size(); ++j ) {
            evt.getByToken( tokenJets_[j], Jets[j] );
        }

        Handle<View<flashgg::Met> > METs;
        evt.getByToken( METToken_, METs );

        Handle<View<reco::Vertex> > vertices;
        evt.getByToken( vertexToken_, vertices );

        edm::Handle<double>  rho;
        evt.getByToken(rhoTag_,rho);
        double rho_    = *rho;

        Handle<View<flashgg::DiPhotonMVAResult> > mvaResults;
        evt.getByToken( mvaResultToken_, mvaResults );
        assert( diPhotons->size() == mvaResults->size() );

        Handle<View<reco::GenParticle> > genParticles;
        evt.getByToken( genParticleToken_, genParticles );


        std::unique_ptr<vector<WHLeptonicTag> > whleptonictags( new vector<WHLeptonicTag> );
        //std::unique_ptr<vector<VHTagTruth> > truths( new vector<VHTagTruth> );
        //edm::RefProd<vector<VHTagTruth> > rTagTruth = evt.getRefBeforePut<vector<VHTagTruth> >();

        bool associatedZ   = 0;
        bool associatedW   = 0;
        bool VhasDaughters = 0;
        bool VhasNeutrinos = 0;
        bool VhasLeptons   = 0;
        bool VhasHadrons   = 0;
        bool VhasMissingLeptons = 0;
        float Vpt = 0;

        if ( !evt.isRealData() ) {

            for( unsigned int genLoop = 0 ; genLoop < genParticles->size(); genLoop++ ) {
                int pdgid = genParticles->ptrAt( genLoop )->pdgId();
                int dpdgid[2] = {0, 0};

                //Search for Z boson
                if(pdgid ==23) {
                    associatedZ=1;
                    if( genParticles->ptrAt( genLoop )->numberOfDaughters()==2) {
                        VhasDaughters = 1;
                        dpdgid[0] = genParticles->ptrAt(genLoop)->daughter(0)->pdgId();
                        //dpdgid[1]=genParticles->ptrAt(genLoop)->daughter(1)->pdgId();
                        Vpt = genParticles->ptrAt( genLoop )->pt();
                        if(fabs(dpdgid[0]) == 12 || fabs(dpdgid[0]) == 14 || fabs(dpdgid[0]) == 16) VhasNeutrinos = 1;//look for neutrino decay of Z
                        if(fabs(dpdgid[0]) == 11 || fabs(dpdgid[0]) == 13 || fabs(dpdgid[0]) == 15) VhasLeptons = 1;//look for lepton decay of Z  
                        if(fabs(dpdgid[0]) > 0 && fabs(dpdgid[0]) < 9) VhasHadrons = 1;//look for quark decay of Z   
                    }
                }
                //Search for W boson
                if(fabs(pdgid)==24) {
                    associatedW=1;
                    if( genParticles->ptrAt( genLoop )->numberOfDaughters()==2) {
                        VhasDaughters = 1;
                        Vpt = genParticles->ptrAt( genLoop )->pt();
                        dpdgid[0] = genParticles->ptrAt(genLoop)->daughter(0)->pdgId();
                        //dpdgid[1]=genParticles->ptrAt(genLoop)->daughter(1)->pdgId();
                        if(fabs(dpdgid[0]) == 12 || fabs(dpdgid[0]) == 14 || fabs(dpdgid[0]) == 16) { VhasNeutrinos = 1; VhasLeptons = 1; }//look for neutrino decay of W
                        if(fabs(dpdgid[0]) == 11 || fabs(dpdgid[0]) == 13 || fabs(dpdgid[0]) == 15) { VhasNeutrinos = 1; VhasLeptons = 1; }//look for lepton decay of W
                        if(fabs(dpdgid[0]) > 0 && fabs(dpdgid[0]) < 9) VhasHadrons = 1;//look for quark decay of W 
                    }
                }

            }
        }
        
        unsigned int idx = 0;
        
        double idmva1 = 0.;
        double idmva2 = 0.;

        bool hasGoodElec  = false;
        bool hasGoodMuons = false;
        
        for( unsigned int diphoIndex = 0; diphoIndex < diPhotons->size(); diphoIndex++ ) {

            edm::Ptr<flashgg::DiPhotonCandidate> dipho = diPhotons->ptrAt( diphoIndex );
            edm::Ptr<flashgg::DiPhotonMVAResult> mvares = mvaResults->ptrAt( diphoIndex );

            WHLeptonicTag whleptonictags_obj( dipho, mvares );
            whleptonictags_obj.includeWeights( *dipho );

            idmva1 = dipho->leadingPhoton()->phoIdMvaDWrtVtx( dipho->vtx() );
            idmva2 = dipho->subLeadingPhoton()->phoIdMvaDWrtVtx( dipho->vtx() );
            
            if( dipho->leadingPhoton()->pt()    < dipho->mass() * leadPhoOverMassThreshold_ ) continue;
            if( dipho->subLeadingPhoton()->pt() < dipho->mass() * subleadPhoOverMassThreshold_ ) continue;
            if( idmva1 <= PhoMVAThreshold_ || idmva2 <= PhoMVAThreshold_ ) continue;
            if( mvares->result < MVAThreshold_ ) continue;

            // Lepton
            std::vector<edm::Ptr<flashgg::Muon> > goodMuons = selectMuons(  theMuons->ptrs(), 
                                                                            dipho, 
                                                                            vertices->ptrs(), 
                                                                            muonEtaThreshold_, 
                                                                            leptonPtThreshold_,
                                                                            muPFIsoSumRelThreshold_, 
                                                                            deltaRMuonPhoThreshold_, 
                                                                            deltaRMuonPhoThreshold_ 
                                                                            );
            
            std::vector<edm::Ptr<Electron> >goodElectrons = selectStdElectrons( theElectrons->ptrs(), 
                                                                                dipho,
                                                                                vertices->ptrs(), 
                                                                                leptonPtThreshold_, 
                                                                                electronEtaThresholds_,
                                                                                useElectronMVARecipe_,
                                                                                useElectronLooseID_,
                                                                                deltaRPhoElectronThreshold_,
                                                                                DeltaRTrkElec_,
                                                                                deltaMassElectronZThreshold_,
                                                                                rho_, 
                                                                                evt.isRealData() 
                                                                                );

            hasGoodElec  = ( goodElectrons.size() > 0 );
            hasGoodMuons = ( goodMuons.size() > 0 );
                        
            if( !hasGoodElec && !hasGoodMuons ) { continue; }
            //including SFs for leading muon or electron 
            if( goodMuons.size() > 0 ) {
                whleptonictags_obj.includeWeights( *goodMuons.at(0) );
            } else if ( goodElectrons.size() > 0 ) {
                whleptonictags_obj.includeWeights( *goodElectrons.at(0) );
            }

            //Jets
            std::vector<edm::Ptr<Jet> > tagJets;
            unsigned int jetCollectionIndex = diPhotons->ptrAt( diphoIndex )->jetCollectionIndex();
            for( unsigned int candIndex_outer = 0; candIndex_outer < Jets[jetCollectionIndex]->size() ; candIndex_outer++ ) {

                bool keepJet = true;
                edm::Ptr<flashgg::Jet> thejet = Jets[jetCollectionIndex]->ptrAt( candIndex_outer );
                if(!thejet->passesJetID  ( flashgg::Tight2017 ) ) { continue; }
                if( fabs( thejet->eta() ) > jetEtaThreshold_ ) { keepJet = false; }
                if( thejet->pt() < jetPtThreshold_ ) { keepJet = false; }
                float dRPhoLeadJet = deltaR( thejet->eta(), thejet->phi(), dipho->leadingPhoton()->superCluster()->eta(), dipho->leadingPhoton()->superCluster()->phi() ) ;
                float dRPhoSubLeadJet = deltaR( thejet->eta(), thejet->phi(), dipho->subLeadingPhoton()->superCluster()->eta(),
                                                dipho->subLeadingPhoton()->superCluster()->phi() );
                
                if( dRPhoLeadJet < deltaRPhoLeadJet_ || dRPhoSubLeadJet < deltaRPhoSubLeadJet_ ) { keepJet = false; }
                if( hasGoodElec ) 
                    for( unsigned int electronIndex = 0; electronIndex < goodElectrons.size(); electronIndex++ ) {
                            Ptr<flashgg::Electron> electron = goodElectrons[electronIndex];
                            float dRJetElectron = deltaR( thejet->eta(), thejet->phi(), electron->eta(), electron->phi() ) ;
                            if( dRJetElectron < deltaRJetMuonThreshold_ ) { keepJet = false; }
                    }
                if( hasGoodMuons ) 
                    for( unsigned int muonIndex = 0; muonIndex < goodMuons.size(); muonIndex++ ) {
                            Ptr<flashgg::Muon> muon = goodMuons[muonIndex];
                            float dRJetMuon = deltaR( thejet->eta(), thejet->phi(), muon->eta(), muon->phi() ) ;
                            if( dRJetMuon < deltaRJetMuonThreshold_ ) { keepJet = false; }
                    }
                if(keepJet) tagJets.push_back( thejet );
            }

            if (tagJets.size() > jetsNumberThreshold_) continue;

            //MET
            Ptr<flashgg::Met> theMET = METs->ptrAt( 0 );
            if ( theMET->getCorPt() < METThreshold_ ) continue;

            //WHMVA
            double ele_pt = -999.;
            double mu_pt  = -999.;

            if (goodElectrons.size() > 0) ele_pt = goodElectrons[0]->pt();
            if (goodMuons.size() > 0)     mu_pt  = goodMuons[0]->pt();

            TLorentzVector diphoP4(dipho->px(), dipho->py(), dipho->pz(), dipho->energy());
            if (ele_pt > mu_pt) {
                Ptr<flashgg::Electron> elec = goodElectrons[0];
                TLorentzVector elecP4(elec->px(), elec->py(), elec->pz(), elec->energy());
                _mtlnu         = sqrt( 2. * elec->et() * theMET->getCorPt() * ( 1 - cos( elec->phi() - theMET->getCorPhi() ) ) );
                _theta         = diphoP4.Angle(elecP4.Vect());
                _dr_pho1_lep = deltaR( elec->eta(), elec->phi(), dipho->leadingPhoton()->eta(), dipho->leadingPhoton()->phi() ) ;
                _dr_pho2_lep = deltaR( elec->eta(), elec->phi(), dipho->subLeadingPhoton()->eta(), dipho->subLeadingPhoton()->phi() ) ;
                _lep_pt        = elec->pt();
                _lep_eta       = elec->eta();
                _leptonType    = 11.;

            } else {
                Ptr<flashgg::Muon> mu = goodMuons[0];
                TLorentzVector muP4(mu->px(), mu->py(), mu->pz(), mu->energy());
                _mtlnu         = sqrt( 2. * mu->et() * theMET->getCorPt() * ( 1 - cos( mu->phi() - theMET->getCorPhi() ) ) );
                _theta         = diphoP4.Angle(muP4.Vect());
                _dr_pho1_lep = deltaR( mu->eta(), mu->phi(), dipho->leadingPhoton()->eta(), dipho->leadingPhoton()->phi() ) ;
                _dr_pho2_lep = deltaR( mu->eta(), mu->phi(), dipho->subLeadingPhoton()->eta(), dipho->subLeadingPhoton()->phi() ) ;
                _lep_pt        = mu->pt();
                _lep_eta       = mu->eta();
                _leptonType    = 13.;

            }

            _pho1_eta      = dipho->leadingPhoton()->eta();
            _pho2_eta      = dipho->subLeadingPhoton()->eta();
            _pho1_ptoM     = dipho->leadingPhoton()->pt() / dipho->mass();
            _pho2_ptoM     = dipho->subLeadingPhoton()->pt() / dipho->mass();
            _min_phoId     = TMath::Min(idmva1, idmva2);
            _max_phoId     = TMath::Max(idmva1, idmva2);
            _pho1_psv      = dipho->leadingPhoton()->hasPixelSeed() > 0.5 ? 0. : 1.;
            _pho2_psv      = dipho->subLeadingPhoton()->hasPixelSeed() > 0.5 ? 0. : 1.;

            _dipho_cosphi  = TMath::Cos( deltaPhi(dipho->leadingPhoton()->phi(), dipho->subLeadingPhoton()->phi()) );

            _njet          = tagJets.size();
            _met           = theMET->getCorPt();
            float deepCSV1 = tagJets.size() > 0 ? tagJets[0]->bDiscriminator("pfDeepCSVJetTags:probb") + tagJets[0]->bDiscriminator("pfDeepCSVJetTags:probbb") : -2;
            float deepCSV2 = tagJets.size() > 1 ? tagJets[1]->bDiscriminator("pfDeepCSVJetTags:probb") + tagJets[1]->bDiscriminator("pfDeepCSVJetTags:probbb") : -2;
            _jet1_csv      = deepCSV1 > -1 ? deepCSV1 : -2.;
            _jet2_csv      = deepCSV2 > -1 ? deepCSV2 : -2.;
            _jet1_pt       = tagJets.size() > 0 ? tagJets[0]->pt() : -50.;

            float whmva    = WHMva_->EvaluateMVA( "BDT" );
            float ptV = dipho->pt();

            // Categorization by WHMVA & ptV (> 75 GeV and < 75 GeV)
            int catnum = chooseCategory( whmva, ptV );

            if( catnum != -1 ) {
                whleptonictags_obj.setCategoryNumber( catnum );
                int chosenTag = computeStage1Kinematics( whleptonictags_obj );
                whleptonictags_obj.setStage1recoTag( chosenTag );
                whleptonictags_obj.setJets( tagJets );
                whleptonictags_obj.setMuons( goodMuons );
                whleptonictags_obj.setElectrons( goodElectrons );
                whleptonictags_obj.setDiPhotonIndex( diphoIndex );
                whleptonictags_obj.setSystLabel( systLabel_ );
                whleptonictags_obj.setMET( theMET );
                whleptonictags->push_back( whleptonictags_obj );
                if( ! evt.isRealData() ) {
                    VHTagTruth truth_obj;
                    truth_obj.setAssociatedZ( associatedZ );
                    truth_obj.setAssociatedW( associatedW );
                    truth_obj.setVhasDaughters( VhasDaughters );
                    truth_obj.setVhasNeutrinos( VhasNeutrinos );
                    truth_obj.setVhasLeptons( VhasLeptons );
                    truth_obj.setVhasHadrons( VhasHadrons );
                    truth_obj.setVhasMissingLeptons( VhasMissingLeptons );
                    truth_obj.setVpt( Vpt );
                    //truths->push_back( truth_obj );
                    //whleptonictags->back().setTagTruth( edm::refToPtr( edm::Ref<vector<VHTagTruth> >( rTagTruth, idx++ ) ) );
                }
            }
        }
        evt.put( std::move( whleptonictags ) );
        //evt.put( std::move( truths ) );
    }

    int WHLeptonicTagProducer::computeStage1Kinematics( const WHLeptonicTag tag_obj )
    {
        int chosenTag_ = DiPhotonTagBase::stage1recoTag::LOGICERROR;
        int catNum = tag_obj.categoryNumber();
        if ( catNum == 0 ) {
            chosenTag_ = DiPhotonTagBase::stage1recoTag::RECO_WH_LEP_HIGH_Tag0;
        }
        else if ( catNum == 1 ) {
            chosenTag_ = DiPhotonTagBase::stage1recoTag::RECO_WH_LEP_HIGH_Tag1;
        }
        else if ( catNum == 2 ) {
            chosenTag_ = DiPhotonTagBase::stage1recoTag::RECO_WH_LEP_HIGH_Tag2;
        }
        if ( catNum == 3 ) {
            chosenTag_ = DiPhotonTagBase::stage1recoTag::RECO_WH_LEP_LOW_Tag0;
        }
        else if ( catNum == 4 ) {
            chosenTag_ = DiPhotonTagBase::stage1recoTag::RECO_WH_LEP_LOW_Tag1;
        }
        else if ( catNum == 5 ) {
            chosenTag_ = DiPhotonTagBase::stage1recoTag::RECO_WH_LEP_LOW_Tag2;
        }
        return chosenTag_;
    }

}
typedef flashgg::WHLeptonicTagProducer FlashggWHLeptonicTagProducer;
DEFINE_FWK_MODULE( FlashggWHLeptonicTagProducer );
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

