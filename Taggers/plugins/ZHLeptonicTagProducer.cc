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

using namespace std;
using namespace edm;


namespace flashgg {
    class ZHLeptonicTagProducer : public EDProducer
    {

    public:
        ZHLeptonicTagProducer( const ParameterSet & );
    private:
        void produce( Event &, const EventSetup & ) override;

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

        produces<vector<ZHLeptonicTag> >();
        //produces<vector<VHTagTruth> >();
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
            std::vector<edm::Ptr<Electron> > tagElectronsTemp;
            std::vector<edm::Ptr<flashgg::Muon> > tagMuons;
            std::vector<edm::Ptr<Electron> > tagElectrons;

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

            if( tagElectronsTemp.size() < 2 && tagMuonsTemp.size() < 2 ) continue;
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

            //Jets
            std::vector<edm::Ptr<Jet> > tagJets;
            unsigned int jetCollectionIndex = diPhotons->ptrAt( diphoIndex )->jetCollectionIndex();
            for( unsigned int candIndex_outer = 0; candIndex_outer < Jets[jetCollectionIndex]->size() ; candIndex_outer++ ) {
 
                bool keepJet=true;
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

            if( isDiMuon  || isDiElectron ) {
                ZHLeptonicTags_obj.setMuons( tagMuons );
                ZHLeptonicTags_obj.setElectrons( tagElectrons );
                ZHLeptonicTags_obj.setJets( tagJets );
                //including SFs for muons or electrons
                if(isDiMuon){
                    ZHLeptonicTags_obj.includeWeightsByLabel( *tagMuons.at(0), "MuonWeight");
                    ZHLeptonicTags_obj.includeWeightsByLabel( *tagMuons.at(1), "MuonWeight");
                } else if(isDiElectron){
                    ZHLeptonicTags_obj.includeWeights( *tagElectrons.at(0) );
                    ZHLeptonicTags_obj.includeWeights( *tagElectrons.at(1) );
                }
                ZHLeptonicTags_obj.setDiPhotonIndex( diphoIndex );
                ZHLeptonicTags_obj.setSystLabel( systLabel_ );
                ZHLeptonicTags_obj.setStage1recoTag( DiPhotonTagBase::stage1recoTag::RECO_ZH_LEP );
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
