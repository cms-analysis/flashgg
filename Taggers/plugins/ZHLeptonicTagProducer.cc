#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/EDMException.h"

#include "flashgg/DataFormats/interface/DiPhotonCandidate.h"
#include "flashgg/DataFormats/interface/ZHLeptonicTag.h"
#include "flashgg/DataFormats/interface/Electron.h"
#include "flashgg/DataFormats/interface/Muon.h"

#include "FWCore/Common/interface/TriggerNames.h"
#include "DataFormats/TrackReco/interface/HitPattern.h"
#include "flashgg/Taggers/interface/LeptonSelection.h"

#include "DataFormats/Math/interface/deltaR.h"
#include "flashgg/DataFormats/interface/VHTagTruth.h"
#include "DataFormats/Common/interface/RefToPtr.h"

#include <vector>
#include <algorithm>
#include <string>
#include <utility>
#include "TLorentzVector.h"
#include "DataFormats/Math/interface/LorentzVector.h"
#include "TMath.h"
#include "TMVA/Reader.h"

using namespace std;
using namespace edm;


namespace flashgg {
    class ZHLeptonicTagProducer : public EDProducer
    {

    public:
        ZHLeptonicTagProducer( const ParameterSet & );
    private:
        void produce( Event &, const EventSetup & ) override;
        int  chooseCategory( float );

        EDGetTokenT<View<DiPhotonCandidate> > diPhotonToken_;
        EDGetTokenT<View<flashgg::Electron> > electronToken_;
        EDGetTokenT<View<flashgg::Muon> > muonToken_;
        std::vector<edm::EDGetTokenT<View<flashgg::Jet> > > tokenJets_;
        std::vector<edm::InputTag> inputTagJets_;
        EDGetTokenT<View<reco::Vertex> > vertexToken_;
        EDGetTokenT<double> rhoTag_;
        EDGetTokenT<View<DiPhotonMVAResult> > mvaResultToken_;
        EDGetTokenT<View<reco::GenParticle> > genParticleToken_;
        string systLabel_;

        typedef std::vector<edm::Handle<edm::View<flashgg::Jet> > > JetCollectionVector;        
        
        //Thresholds
        double leadPhoOverMassThreshold_      ;
        double subleadPhoOverMassThreshold_   ;
        double MVAThreshold_                  ;
        double PhoMVAThreshold_               ;
        double leptonPtThreshold_             ;
        double invMassLepLowThreshold_        ;
        double invMassLepHighThreshold_       ;
        vector<double> electronEtaThresholds_ ;
        double DeltaRTrkElec_                 ;
        double deltaRPhoElectronThreshold_    ;
        double deltaMassElectronZThreshold_   ;
        bool useElectronMVARecipe_            ;
        bool useElectronLooseID_              ;
        double muonEtaThreshold_              ; 
        double deltaRMuonPhoThreshold_        ;
        double muPFIsoSumRelThreshold_        ;
        double jetPtThreshold_                ;
        double jetEtaThreshold_               ;
        double deltaRPhoLeadJet_              ;
        double deltaRPhoSubLeadJet_           ;
        double deltaRJetLepThreshold_         ;


        //WHMVA
        unique_ptr<TMVA::Reader>ZHMva_;

        float _pho1_eta     ; 
        float _pho2_eta     ; 
        float _pho1_ptoM    ; 
        float _pho2_ptoM    ; 
        float _min_phoId    ; 
        float _max_phoId    ; 
        float _pho1_psv     ; 
        float _pho2_psv     ; 
        float _dipho_cosphi ;
        float _lep1_pt      ; 
        float _lep2_pt      ; 
        float _lep1_eta     ; 
        float _lep2_eta     ; 
        float _dr_pho1_lep1 ; 
        float _dr_pho1_lep2 ; 
        float _dr_pho2_lep1 ; 
        float _dr_pho2_lep2 ; 
        float _dilep_mass   ; 
        float _theta        ; 
        float _njet         ; 
        float _jet1_pt      ; 
        float _jet1_csv     ; 
        float _leptonType   ; 
 
        FileInPath ZHMVAweightfile_ ;

        vector<double> boundaries;

    };

    ZHLeptonicTagProducer::ZHLeptonicTagProducer( const ParameterSet &iConfig ) :
        diPhotonToken_( consumes<View<flashgg::DiPhotonCandidate> >( iConfig.getParameter<InputTag> ( "DiPhotonTag" ) ) ),
        electronToken_( consumes<View<flashgg::Electron> >( iConfig.getParameter<InputTag> ( "ElectronTag" ) ) ),
        muonToken_( consumes<View<flashgg::Muon> >( iConfig.getParameter<InputTag>( "MuonTag" ) ) ),
        inputTagJets_( iConfig.getParameter<std::vector<edm::InputTag> >( "inputTagJets" ) ),
        vertexToken_( consumes<View<reco::Vertex> >( iConfig.getParameter<InputTag> ( "VertexTag" ) ) ),
        rhoTag_( consumes<double>( iConfig.getParameter<InputTag>( "rhoTag" ) ) ),
        mvaResultToken_( consumes<View<flashgg::DiPhotonMVAResult> >( iConfig.getParameter<InputTag> ( "MVAResultTag" ) ) ),
        genParticleToken_( consumes<View<reco::GenParticle> >( iConfig.getParameter<InputTag> ( "GenParticleTag" ) ) ),
        systLabel_( iConfig.getParameter<string> ( "SystLabel" ) )
    {
        //Photon
        leadPhoOverMassThreshold_    = iConfig.getParameter<double>("leadPhoOverMassThreshold");
        subleadPhoOverMassThreshold_ = iConfig.getParameter<double>("subleadPhoOverMassThreshold");
        MVAThreshold_                = iConfig.getParameter<double>("MVAThreshold");
        PhoMVAThreshold_             = iConfig.getParameter<double>("PhoMVAThreshold");

        //Lepton
        leptonPtThreshold_           = iConfig.getParameter<double>("leptonPtThreshold");
        invMassLepLowThreshold_      = iConfig.getParameter<double>("invMassLepLowThreshold");
        invMassLepHighThreshold_     = iConfig.getParameter<double>("invMassLepHighThreshold");

        //Electron
        electronEtaThresholds_       = iConfig.getParameter<vector<double > >( "electronEtaThresholds");
        DeltaRTrkElec_               = iConfig.getParameter<double>("DeltaRTrkElec");
        deltaRPhoElectronThreshold_  = iConfig.getParameter<double>("deltaRPhoElectronThreshold");
        deltaMassElectronZThreshold_ = iConfig.getParameter<double>("deltaMassElectronZThreshold");
        useElectronMVARecipe_        = iConfig.getParameter<bool>("useElectronMVARecipe");
        useElectronLooseID_          = iConfig.getParameter<bool>("useElectronLooseID");

        //Muon
        muonEtaThreshold_            = iConfig.getParameter<double>("muonEtaThreshold");
        deltaRMuonPhoThreshold_      = iConfig.getParameter<double>("deltaRMuonPhoThreshold");
        muPFIsoSumRelThreshold_      = iConfig.getParameter<double>("muPFIsoSumRelThreshold");

        //Jet
        jetPtThreshold_              = iConfig.getParameter<double>("jetPtThreshold"); 
        jetEtaThreshold_             = iConfig.getParameter<double>("jetEtaThreshold");      
        deltaRPhoLeadJet_            = iConfig.getParameter<double>("deltaRPhoLeadJet");     
        deltaRPhoSubLeadJet_         = iConfig.getParameter<double>("deltaRPhoSubLeadJet");
        deltaRJetLepThreshold_       = iConfig.getParameter<double>("deltaRJetLepThreshold");

        for (unsigned i = 0 ; i < inputTagJets_.size() ; i++) {
            auto token = consumes<View<flashgg::Jet> >(inputTagJets_[i]);
            tokenJets_.push_back(token);
        }

        //ZHMVA
        ZHMVAweightfile_ = iConfig.getParameter<edm::FileInPath>( "ZHMVAweightfile" );

        _pho1_eta     = -999.; 
        _pho2_eta     = -999.; 
        _pho1_ptoM    = -999.; 
        _pho2_ptoM    = -999.; 
        _min_phoId    = -999.; 
        _max_phoId    = -999.; 
        _pho1_psv     = -999.; 
        _pho2_psv     = -999.; 
        _dipho_cosphi = -999.; 
        _lep1_pt      = -999.; 
        _lep2_pt      = -999.; 
        _lep1_eta     = -999.; 
        _lep2_eta     = -999.; 
        _dr_pho1_lep1 = -999.; 
        _dr_pho1_lep2 = -999.; 
        _dr_pho2_lep1 = -999.; 
        _dr_pho2_lep2 = -999.; 
        _dilep_mass   = -999.; 
        _theta        = -999.; 
        _njet         = -999.; 
        _jet1_pt      = -999.; 
        _jet1_csv     = -999.; 
        _leptonType   = -999.; 

        ZHMva_.reset( new TMVA::Reader( "!Color:!Silent" ) );
        ZHMva_->AddVariable( "pho1_eta"      ,& _pho1_eta       ); 
        ZHMva_->AddVariable( "pho2_eta"      ,& _pho2_eta       ); 
        ZHMva_->AddVariable( "pho1_ptoM"     ,& _pho1_ptoM      ); 
        ZHMva_->AddVariable( "pho2_ptoM"     ,& _pho2_ptoM      ); 
        ZHMva_->AddVariable( "min_phoId"     ,& _min_phoId      ); 
        ZHMva_->AddVariable( "max_phoId"     ,& _max_phoId      ); 
        ZHMva_->AddVariable( "pho1_psv"      ,& _pho1_psv       ); 
        ZHMva_->AddVariable( "pho2_psv"      ,& _pho2_psv       ); 
        ZHMva_->AddVariable( "dipho_cosphi"  ,& _dipho_cosphi   ); 
        ZHMva_->AddVariable( "lep1_pt"       ,& _lep1_pt        ); 
        ZHMva_->AddVariable( "lep2_pt"       ,& _lep2_pt        ); 
        ZHMva_->AddVariable( "lep1_eta"      ,& _lep1_eta       ); 
        ZHMva_->AddVariable( "lep2_eta"      ,& _lep2_eta       ); 
        ZHMva_->AddVariable( "dr_pho1_lep1"  ,& _dr_pho1_lep1   ); 
        ZHMva_->AddVariable( "dr_pho1_lep2"  ,& _dr_pho1_lep2   ); 
        ZHMva_->AddVariable( "dr_pho2_lep1"  ,& _dr_pho2_lep1   ); 
        ZHMva_->AddVariable( "dr_pho2_lep2"  ,& _dr_pho2_lep2   ); 
        ZHMva_->AddVariable( "dilep_mass"    ,& _dilep_mass     ); 
        ZHMva_->AddVariable( "theta"         ,& _theta          ); 
        ZHMva_->AddVariable( "njet"          ,& _njet           ); 
        ZHMva_->AddVariable( "jet1_pt"       ,& _jet1_pt        ); 
        ZHMva_->AddVariable( "jet1_csv"      ,& _jet1_csv       ); 
        ZHMva_->AddVariable( "leptonType"    ,& _leptonType     ); 
        ZHMva_->BookMVA( "BDT", ZHMVAweightfile_.fullPath() );

        boundaries = iConfig.getParameter<vector<double > >( "Boundaries" );
        assert( is_sorted( boundaries.begin(), boundaries.end() ) ); // we are counting on ascending order - update this to give an error message or exception

        produces<vector<ZHLeptonicTag> >();
        //produces<vector<VHTagTruth> >();
    }

    int ZHLeptonicTagProducer::chooseCategory( float mva )
    {
        // should return 0 if mva above all the numbers, 1 if below the first, ..., boundaries.size()-N if below the Nth, ...
        int n;
        for( n = 0 ; n < ( int )boundaries.size() ; n++ ) {
            if( ( double )mva > boundaries[boundaries.size() - n - 1] ) { return n; }
        }
        return -1; // Does not pass, object will not be produced
    }

    void ZHLeptonicTagProducer::produce( Event &evt, const EventSetup & )
    {
        Handle<View<flashgg::DiPhotonCandidate> > diPhotons;
        evt.getByToken( diPhotonToken_, diPhotons );

        Handle<View<flashgg::Electron> > theElectrons;
        evt.getByToken( electronToken_, theElectrons );

        Handle<View<flashgg::Muon> > theMuons;
        evt.getByToken( muonToken_, theMuons );

        JetCollectionVector Jets( inputTagJets_.size() );
        for( size_t j = 0; j < inputTagJets_.size(); ++j ) 
            evt.getByToken( tokenJets_[j], Jets[j] );

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

        std::unique_ptr<vector<ZHLeptonicTag> > ZHLeptonicTags( new vector<ZHLeptonicTag> );
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

        if( ! evt.isRealData() ) {
            for( unsigned int genLoop = 0 ; genLoop < genParticles->size(); genLoop++ ) {
                int pdgid = genParticles->ptrAt( genLoop )->pdgId();
                int dpdgid[2] = {0, 0};

                //Search for Z boson
                if(pdgid == 23) {
                    associatedZ = 1;
                    if(genParticles->ptrAt( genLoop )->numberOfDaughters() == 2) {
                        VhasDaughters = 1;
                        dpdgid[0] = genParticles->ptrAt(genLoop)->daughter(0)->pdgId();
                        Vpt = genParticles->ptrAt( genLoop )->pt();
                        if(fabs(dpdgid[0]) == 12 || fabs(dpdgid[0]) == 14 || fabs(dpdgid[0]) == 16) VhasNeutrinos = 1;//look for neutrino decay of Z
                        if(fabs(dpdgid[0]) == 11 || fabs(dpdgid[0]) == 13 || fabs(dpdgid[0]) == 15) VhasLeptons = 1;//look for lepton decay of Z
                        if(fabs(dpdgid[0]) >0 && fabs(dpdgid[0]) < 9) VhasHadrons = 1; //look for quark decay of Z
                    }
                }

                //Search for W boson
                if(fabs(pdgid) == 24) {
                    associatedW = 1;
                    if(genParticles->ptrAt( genLoop )->numberOfDaughters() == 2) {
                        VhasDaughters = 1;
                        Vpt = genParticles->ptrAt( genLoop )->pt();
                        dpdgid[0] = genParticles->ptrAt(genLoop)->daughter(0)->pdgId();
                        //dpdgid[1]=genParticles->ptrAt(genLoop)->daughter(1)->pdgId();
                        if(fabs(dpdgid[0]) == 12 || fabs(dpdgid[0]) == 14 || fabs(dpdgid[0]) == 16) { VhasNeutrinos = 1; VhasLeptons = 1; }//look for neutrino decay of W
                        if(fabs(dpdgid[0]) == 11 || fabs(dpdgid[0]) == 13 || fabs(dpdgid[0]) == 15) { VhasNeutrinos = 1; VhasLeptons = 1; }//look for lepton decay of W
                        if(fabs(dpdgid[0]) > 0 && fabs(dpdgid[0]) < 9) { VhasHadrons = 1; }//look for quark decay of W
                    }
                }
            }
        }

        unsigned int idx = 0;

        double idmva1 = 0.;
        double idmva2 = 0.;
        bool isDiMuon = false;
        bool isDiElectron = false;
        for( unsigned int diphoIndex = 0; diphoIndex < diPhotons->size(); diphoIndex++ ) {

            std::vector<edm::Ptr<flashgg::Muon> > tagMuonsTemp;
            std::vector<edm::Ptr<flashgg::Electron> > tagElectronsTemp;
            std::vector<edm::Ptr<flashgg::Muon> > tagMuons;
            std::vector<edm::Ptr<flashgg::Electron> > tagElectrons;

            isDiMuon = false;
            isDiElectron = false;
            
            edm::Ptr<flashgg::DiPhotonCandidate> dipho = diPhotons->ptrAt( diphoIndex );
            edm::Ptr<flashgg::DiPhotonMVAResult> mvares = mvaResults->ptrAt( diphoIndex );
            ZHLeptonicTag ZHLeptonicTags_obj( dipho, mvares );
            ZHLeptonicTags_obj.includeWeights( *dipho );
            
            if( dipho->leadingPhoton()->pt() < ( dipho->mass() )*leadPhoOverMassThreshold_ ) { continue; }
            if( dipho->subLeadingPhoton()->pt() < ( dipho->mass() )*subleadPhoOverMassThreshold_ ) { continue; }
            idmva1 = dipho->leadingPhoton()->phoIdMvaDWrtVtx( dipho->vtx() );
            idmva2 = dipho->subLeadingPhoton()->phoIdMvaDWrtVtx( dipho->vtx() );
            if( idmva1 <= PhoMVAThreshold_ || idmva2 <= PhoMVAThreshold_ ) { continue; }
            if( mvares->result < MVAThreshold_ ) { continue; }

            tagMuonsTemp = selectMuons( theMuons->ptrs(), 
                                        dipho, 
                                        vertices->ptrs(), 
                                        muonEtaThreshold_, 
                                        leptonPtThreshold_, 
                                        muPFIsoSumRelThreshold_,
                                        deltaRMuonPhoThreshold_, 
                                        deltaRMuonPhoThreshold_ 
                                        );
            tagElectronsTemp = selectStdElectrons( theElectrons->ptrs(), 
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

            //check for two good muons
            if( tagMuonsTemp.size() >= 2 ) {
                for(uint i = 0; i < tagMuonsTemp.size(); i++) {
                    for(uint j = i + 1; j < tagMuonsTemp.size(); j++) {
                        math::XYZTLorentzVector leptonPair = tagMuonsTemp[i]->p4() + tagMuonsTemp[j]->p4();

                        if( leptonPair.M() < invMassLepHighThreshold_  && leptonPair.M() > invMassLepLowThreshold_ ) {
                            tagMuons.push_back(tagMuonsTemp[i]);
                            tagMuons.push_back(tagMuonsTemp[j]);
                            isDiMuon = true;
                        }
                    }
                }
            }

            //check for two good electrons
            if( tagElectronsTemp.size() >= 2 ) {
                for(uint i = 0; i < tagElectronsTemp.size(); i++) {
                    for(uint j = i + 1; j < tagElectronsTemp.size(); j++) {
                        math::XYZTLorentzVector leptonPair = tagElectronsTemp[i]->p4()+tagElectronsTemp[j]->p4();
                        if( leptonPair.M() < invMassLepHighThreshold_  && leptonPair.M() > invMassLepLowThreshold_ ) {
                            tagElectrons.push_back(tagElectronsTemp[i]);
                            tagElectrons.push_back(tagElectronsTemp[j]);
                            isDiElectron = true;
                        }
                    }
                }
            }

            if ( !isDiElectron && !isDiMuon ) continue;

            //Jets
            std::vector<edm::Ptr<Jet> > tagJets;
            unsigned int jetCollectionIndex = diPhotons->ptrAt( diphoIndex )->jetCollectionIndex();
            for( unsigned int candIndex_outer = 0; candIndex_outer < Jets[jetCollectionIndex]->size() ; candIndex_outer++ ) {
 
                bool keepJet = true;
                edm::Ptr<flashgg::Jet> thejet = Jets[jetCollectionIndex]->ptrAt( candIndex_outer );
                if( !thejet->passesJetID( flashgg::Tight2017 ) ) { keepJet = false; }
                if( fabs( thejet->eta() ) > jetEtaThreshold_ ) { keepJet = false; }
                if( thejet->pt() < jetPtThreshold_ ) { keepJet = false; }
                float dRPhoLeadJet = deltaR( thejet->eta(), thejet->phi(), dipho->leadingPhoton()->superCluster()->eta(), dipho->leadingPhoton()->superCluster()->phi() ) ;
                float dRPhoSubLeadJet = deltaR( thejet->eta(), thejet->phi(), dipho->subLeadingPhoton()->superCluster()->eta(),
                                                dipho->subLeadingPhoton()->superCluster()->phi() );
                
                if( dRPhoLeadJet < deltaRPhoLeadJet_ || dRPhoSubLeadJet < deltaRPhoSubLeadJet_ ) { keepJet = false; }
                if( isDiElectron ) 
                    for( unsigned int electronIndex = 0; electronIndex < tagElectrons.size(); electronIndex++ ) {
                            Ptr<flashgg::Electron> electron = tagElectrons[electronIndex];
                            float dRJetElectron = deltaR( thejet->eta(), thejet->phi(), electron->eta(), electron->phi() ) ;
                            if( dRJetElectron < deltaRJetLepThreshold_ ) { keepJet = false; }
                    }
                if( isDiMuon ) 
                    for( unsigned int muonIndex = 0; muonIndex < tagMuons.size(); muonIndex++ ) {
                            Ptr<flashgg::Muon> muon = tagMuons[muonIndex];
                            float dRJetMuon = deltaR( thejet->eta(), thejet->phi(), muon->eta(), muon->phi() ) ;
                            if( dRJetMuon < deltaRJetLepThreshold_ ) { keepJet = false; }
                    }
                if(keepJet) tagJets.push_back( thejet );
            }

            //ZHMVA

            TLorentzVector diphoP4(dipho->px(), dipho->py(), dipho->pz(), dipho->energy());
            if (isDiElectron) {
                Ptr<flashgg::Electron> elec1 = tagElectrons[0];
                Ptr<flashgg::Electron> elec2 = tagElectrons[1];
                TLorentzVector elec1P4(elec1->px(), elec1->py(), elec1->pz(), elec1->energy());
                TLorentzVector elec2P4(elec2->px(), elec2->py(), elec2->pz(), elec2->energy());
                TLorentzVector diEleP4 = elec1P4 + elec2P4;
                _dr_pho1_lep1 = deltaR( elec1->eta(), elec1->phi(), dipho->leadingPhoton()->eta(), dipho->leadingPhoton()->phi() ) ;
                _dr_pho1_lep2 = deltaR( elec2->eta(), elec2->phi(), dipho->leadingPhoton()->eta(), dipho->leadingPhoton()->phi() ) ;
                _dr_pho2_lep1 = deltaR( elec1->eta(), elec1->phi(), dipho->subLeadingPhoton()->eta(), dipho->subLeadingPhoton()->phi() ) ;
                _dr_pho2_lep2 = deltaR( elec2->eta(), elec2->phi(), dipho->subLeadingPhoton()->eta(), dipho->subLeadingPhoton()->phi() ) ;
                _lep1_pt      = elec1->pt();
                _lep2_pt      = elec2->pt();
                _lep1_eta     = elec1->eta();
                _lep2_eta     = elec2->eta();
                _dilep_mass   = diEleP4.M();
                _theta        = diphoP4.Angle(diEleP4.Vect());
                _leptonType   = 11.;
            } else {
                Ptr<flashgg::Muon> mu1 = tagMuons[0];
                Ptr<flashgg::Muon> mu2 = tagMuons[1];
                TLorentzVector mu1P4(mu1->px(), mu1->py(), mu1->pz(), mu1->energy());
                TLorentzVector mu2P4(mu2->px(), mu2->py(), mu2->pz(), mu2->energy());
                TLorentzVector diMuP4 = mu1P4 + mu2P4;
                _dr_pho1_lep1 = deltaR( mu1->eta(), mu1->phi(), dipho->leadingPhoton()->eta(), dipho->leadingPhoton()->phi() ) ;
                _dr_pho1_lep2 = deltaR( mu2->eta(), mu2->phi(), dipho->leadingPhoton()->eta(), dipho->leadingPhoton()->phi() ) ;
                _dr_pho2_lep1 = deltaR( mu1->eta(), mu1->phi(), dipho->subLeadingPhoton()->eta(), dipho->subLeadingPhoton()->phi() ) ;
                _dr_pho2_lep2 = deltaR( mu2->eta(), mu2->phi(), dipho->subLeadingPhoton()->eta(), dipho->subLeadingPhoton()->phi() ) ;
                _lep1_pt      = mu1->pt();
                _lep2_pt      = mu2->pt();
                _lep1_eta     = mu1->eta();
                _lep2_eta     = mu2->eta();
                _dilep_mass   = diMuP4.M();
                _theta        = diphoP4.Angle(diMuP4.Vect());
                _leptonType = 13.;
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
            float deepCSV1 = tagJets.size() > 0 ? tagJets[0]->bDiscriminator("pfDeepCSVJetTags:probb") + tagJets[0]->bDiscriminator("pfDeepCSVJetTags:probbb") : -2;
            _jet1_csv      = deepCSV1 > -1 ? deepCSV1 : -2.;
            _jet1_pt       = tagJets.size() > 0 ? tagJets[0]->pt() : -50.;

            float zhmva    = ZHMva_->EvaluateMVA( "BDT" );
 
            // Categorization by ZHMVA
            int catnum = chooseCategory( zhmva );

            if( catnum != -1 ) {
                ZHLeptonicTags_obj.setCategoryNumber( catnum );
                ZHLeptonicTags_obj.setMuons( tagMuons );
                ZHLeptonicTags_obj.setElectrons( tagElectrons );
                ZHLeptonicTags_obj.setJets( tagJets );
                //including SFs for muons or electrons
                if(isDiMuon){
                    ZHLeptonicTags_obj.includeWeights( *tagMuons.at(0) );
                    ZHLeptonicTags_obj.includeWeights( *tagMuons.at(1) );
                } else if(isDiElectron){
                    ZHLeptonicTags_obj.includeWeights( *tagElectrons.at(0) );
                    ZHLeptonicTags_obj.includeWeights( *tagElectrons.at(1) );
                }
                ZHLeptonicTags_obj.setDiPhotonIndex( diphoIndex );
                ZHLeptonicTags_obj.setSystLabel( systLabel_ );
                if( catnum == 0 ) { 
                    ZHLeptonicTags_obj.setStage1recoTag( DiPhotonTagBase::stage1recoTag::RECO_ZH_LEP_Tag0 );
                } else if ( catnum == 1 ) {
                    ZHLeptonicTags_obj.setStage1recoTag( DiPhotonTagBase::stage1recoTag::RECO_ZH_LEP_Tag1 );
                }
                ZHLeptonicTags->push_back( ZHLeptonicTags_obj );
                if( ! evt.isRealData() ){
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
                    //ZHLeptonicTags->back().setTagTruth( edm::refToPtr( edm::Ref<vector<VHTagTruth> >( rTagTruth, idx++ ) ) );
                }
            }
        }
        evt.put( std::move( ZHLeptonicTags ) );
        //evt.put( std::move( truths ) );
    }
}
typedef flashgg::ZHLeptonicTagProducer FlashggZHLeptonicTagProducer;
DEFINE_FWK_MODULE( FlashggZHLeptonicTagProducer );
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4
