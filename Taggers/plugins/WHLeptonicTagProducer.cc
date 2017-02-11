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

using namespace std;
using namespace edm;


namespace flashgg {
    class WHLeptonicTagProducer : public EDProducer
    {

    public:
        typedef math::XYZPoint Point;

        WHLeptonicTagProducer( const ParameterSet & );
    private:
        void produce( Event &, const EventSetup & ) override;

        std::vector<edm::EDGetTokenT<View<flashgg::Jet> > > tokenJets_;
        EDGetTokenT<View<DiPhotonCandidate> > diPhotonToken_;
        std::vector<edm::InputTag> inputTagJets_;

        EDGetTokenT<View<Electron> > electronToken_;
        EDGetTokenT<View<flashgg::Muon> > muonToken_;
        EDGetTokenT<View<DiPhotonMVAResult> > mvaResultToken_;
        EDGetTokenT<View<flashgg::Met> > METToken_;
        EDGetTokenT<View<reco::Vertex> > vertexToken_;
        EDGetTokenT<View<reco::GenParticle> > genParticleToken_;
        EDGetTokenT<int> stage0catToken_, stage1catToken_, njetsToken_;
        EDGetTokenT<float> pTHToken_,pTVToken_;
        EDGetTokenT<double> rhoTag_;
        string systLabel_;
        edm::EDGetTokenT<edm::TriggerResults> triggerRECO_;
        edm::EDGetTokenT<edm::TriggerResults> triggerPAT_;
        edm::EDGetTokenT<edm::TriggerResults> triggerFLASHggMicroAOD_;

        typedef std::vector<edm::Handle<edm::View<flashgg::Jet> > > JetCollectionVector;

        //Thresholds
        double leptonPtThreshold_;
        double muonEtaThreshold_;
        double leadPhoOverMassThreshold_;
        double subleadPhoOverMassThreshold_;
        double MVAThreshold_;
        double deltaRMuonPhoThreshold_;
        double jetsNumberThreshold_;
        double jetPtThreshold_;
        double jetEtaThreshold_;
        double muPFIsoSumRelThreshold_;
        double PhoMVAThreshold_;
        double METThreshold_;
        bool useVertex0only_;
        double deltaRJetMuonThreshold_;
        double deltaRPhoLeadJet_;
        double deltaRPhoSubLeadJet_;


        double DeltaRTrkElec_;
        double TransverseImpactParam_;
        double LongitudinalImpactParam_;

        double deltaRPhoElectronThreshold_;
        double deltaMassElectronZThreshold_;

        bool hasGoodElec = false;
        bool hasGoodMuons = false;

        vector<double> nonTrigMVAThresholds_;
        vector<double> nonTrigMVAEtaCuts_;

        double electronIsoThreshold_;
        double electronNumOfHitsThreshold_;
        vector<double> electronEtaThresholds_;
        bool useElectronMVARecipe_;
        bool useElectronLooseID_;
    };

    WHLeptonicTagProducer::WHLeptonicTagProducer( const ParameterSet &iConfig ) :
        diPhotonToken_( consumes<View<flashgg::DiPhotonCandidate> >( iConfig.getParameter<InputTag> ( "DiPhotonTag" ) ) ),
        inputTagJets_( iConfig.getParameter<std::vector<edm::InputTag> >( "inputTagJets" ) ),
        electronToken_( consumes<View<flashgg::Electron> >( iConfig.getParameter<InputTag> ( "ElectronTag" ) ) ),
        muonToken_( consumes<View<flashgg::Muon> >( iConfig.getParameter<InputTag>( "MuonTag" ) ) ),
        mvaResultToken_( consumes<View<flashgg::DiPhotonMVAResult> >( iConfig.getParameter<InputTag> ( "MVAResultTag" ) ) ),
        METToken_( consumes<View<flashgg::Met> >( iConfig.getParameter<InputTag> ( "METTag" ) ) ),
        vertexToken_( consumes<View<reco::Vertex> >( iConfig.getParameter<InputTag> ( "VertexTag" ) ) ),
        genParticleToken_( consumes<View<reco::GenParticle> >( iConfig.getParameter<InputTag> ( "GenParticleTag" ) ) ),
        rhoTag_( consumes<double>( iConfig.getParameter<InputTag>( "rhoTag" ) ) ),
        systLabel_( iConfig.getParameter<string> ( "SystLabel" ) ),
        triggerRECO_( consumes<edm::TriggerResults>(iConfig.getParameter<InputTag>("RECOfilters") ) ),
        triggerPAT_( consumes<edm::TriggerResults>(iConfig.getParameter<InputTag>("PATfilters") ) ),
        triggerFLASHggMicroAOD_( consumes<edm::TriggerResults>( iConfig.getParameter<InputTag>("FLASHfilters") ) )
    {


        leptonPtThreshold_ = iConfig.getParameter<double>( "leptonPtThreshold");
        muonEtaThreshold_ = iConfig.getParameter<double>( "muonEtaThreshold");
        leadPhoOverMassThreshold_ = iConfig.getParameter<double>( "leadPhoOverMassThreshold");
        subleadPhoOverMassThreshold_ = iConfig.getParameter<double>( "subleadPhoOverMassThreshold");
        MVAThreshold_ = iConfig.getParameter<double>( "MVAThreshold");
        deltaRMuonPhoThreshold_ = iConfig.getParameter<double>( "deltaRMuonPhoThreshold");
        jetsNumberThreshold_ = iConfig.getParameter<double>( "jetsNumberThreshold");
        jetPtThreshold_ = iConfig.getParameter<double>( "jetPtThreshold");
        jetEtaThreshold_ = iConfig.getParameter<double>( "jetEtaThreshold");
        muPFIsoSumRelThreshold_ = iConfig.getParameter<double>( "muPFIsoSumRelThreshold");
        PhoMVAThreshold_ = iConfig.getParameter<double>( "PhoMVAThreshold");
        METThreshold_ = iConfig.getParameter<double>( "METThreshold");
        useVertex0only_              = iConfig.getParameter<bool>("useVertex0only");
        deltaRJetMuonThreshold_ = iConfig.getParameter<double>( "deltaRJetMuonThreshold");
        deltaRPhoLeadJet_ = iConfig.getParameter<double>( "deltaRPhoLeadJet");
        deltaRPhoSubLeadJet_ = iConfig.getParameter<double>( "deltaRPhoSubLeadJet");

        DeltaRTrkElec_ = iConfig.getParameter<double>( "DeltaRTrkElec");
        TransverseImpactParam_ = iConfig.getParameter<double>( "TransverseImpactParam");
        LongitudinalImpactParam_ = iConfig.getParameter<double>( "LongitudinalImpactParam");

        deltaRPhoElectronThreshold_ = iConfig.getParameter<double>( "deltaRPhoElectronThreshold");
        deltaMassElectronZThreshold_ = iConfig.getParameter<double>( "deltaMassElectronZThreshold");

        nonTrigMVAThresholds_ =  iConfig.getParameter<vector<double > >( "nonTrigMVAThresholds");
        nonTrigMVAEtaCuts_ =  iConfig.getParameter<vector<double > >( "nonTrigMVAEtaCuts");
        electronIsoThreshold_ = iConfig.getParameter<double>( "electronIsoThreshold");
        electronNumOfHitsThreshold_ = iConfig.getParameter<double>( "electronNumOfHitsThreshold");
        electronEtaThresholds_ = iConfig.getParameter<vector<double > >( "electronEtaThresholds");
        useElectronMVARecipe_=iConfig.getParameter<bool>("useElectronMVARecipe");
        useElectronLooseID_=iConfig.getParameter<bool>("useElectronLooseID");
        
        ParameterSet HTXSps = iConfig.getParameterSet( "HTXSTags" );
        stage0catToken_ = consumes<int>( HTXSps.getParameter<InputTag>("stage0cat") );
        stage1catToken_ = consumes<int>( HTXSps.getParameter<InputTag>("stage1cat") );
        njetsToken_ = consumes<int>( HTXSps.getParameter<InputTag>("njets") );
        pTHToken_ = consumes<float>( HTXSps.getParameter<InputTag>("pTH") );
        pTVToken_ = consumes<float>( HTXSps.getParameter<InputTag>("pTV") );

        for (unsigned i = 0 ; i < inputTagJets_.size() ; i++) {
            auto token = consumes<View<flashgg::Jet> >(inputTagJets_[i]);
            tokenJets_.push_back(token);
        }
        produces<vector<WHLeptonicTag> >();
        produces<vector<VHTagTruth> >();
    }

    void WHLeptonicTagProducer::produce( Event &evt, const EventSetup & )
    {
        Handle<int> stage0cat, stage1cat, njets;
        Handle<float> pTH, pTV;
        evt.getByToken(stage0catToken_, stage0cat);
        evt.getByToken(stage1catToken_,stage1cat);
        evt.getByToken(njetsToken_,njets);
        evt.getByToken(pTHToken_,pTH);
        evt.getByToken(pTVToken_,pTV);

        JetCollectionVector Jets( inputTagJets_.size() );
        for( size_t j = 0; j < inputTagJets_.size(); ++j ) {
            evt.getByToken( tokenJets_[j], Jets[j] );
        }

        Handle<View<flashgg::DiPhotonCandidate> > diPhotons;
        evt.getByToken( diPhotonToken_, diPhotons );

        Handle<View<flashgg::Muon> > theMuons;
        evt.getByToken( muonToken_, theMuons );

        Handle<View<flashgg::Electron> > theElectrons;
        evt.getByToken( electronToken_, theElectrons );

        edm::Handle<double>  rho;
        evt.getByToken(rhoTag_,rho);
        double rho_    = *rho;

        Handle<View<flashgg::DiPhotonMVAResult> > mvaResults;
        evt.getByToken( mvaResultToken_, mvaResults );

        Handle<View<reco::GenParticle> > genParticles;

        std::auto_ptr<vector<WHLeptonicTag> > whleptonictags( new vector<WHLeptonicTag> );

        Handle<View<flashgg::Met> > METs;
        evt.getByToken( METToken_, METs );

        Handle<View<reco::Vertex> > vertices;
        evt.getByToken( vertexToken_, vertices );

        assert( diPhotons->size() == mvaResults->size() );

        std::auto_ptr<vector<VHTagTruth> > truths( new vector<VHTagTruth> );
        Point higgsVtx;
        bool associatedZ=0;
        bool associatedW=0;
        bool VhasDaughters=0;
        bool VhasNeutrinos=0;
        bool VhasLeptons=0;
        bool VhasHadrons=0;
        bool VhasMissingLeptons=0;
        float Vpt=0;
        bool passMETfilters=1;
        //Get trigger results relevant to MET filters                                                                                                                                              

        edm::Handle<edm::TriggerResults> triggerBits;
        if(! evt.isRealData() )
            evt.getByToken( triggerPAT_, triggerBits );
        else
            evt.getByToken( triggerRECO_, triggerBits );

        edm::Handle<edm::TriggerResults> triggerFLASHggMicroAOD;
        evt.getByToken( triggerFLASHggMicroAOD_, triggerFLASHggMicroAOD );
        const edm::TriggerNames &triggerNames = evt.triggerNames( *triggerBits );


        std::vector<std::string> flagList {"Flag_HBHENoiseFilter","Flag_HBHENoiseIsoFilter","Flag_EcalDeadCellTriggerPrimitiveFilter","Flag_goodVertices","Flag_eeBadScFilter"};
        for( unsigned int i = 0; i < triggerNames.triggerNames().size(); i++ )
            {
                if(!triggerBits->accept(i))
                    for(size_t j=0;j<flagList.size();j++)
                        {
                            if(flagList[j]==triggerNames.triggerName(i))
                                {
                                    passMETfilters=0;
                                    break;
                                }
                        }
            }

        std::vector<std::string> flashggFlagList {"flag_BadChargedCandidateFilter","flag_BadPFMuonFilter","flag_globalTightHalo2016Filter"};
        const edm::TriggerNames &flashggtriggerNames = evt.triggerNames( *triggerFLASHggMicroAOD );
        for( unsigned int i = 0; i < flashggtriggerNames.triggerNames().size(); i++ )
            {
                if(!triggerFLASHggMicroAOD->accept(i))
                    for(size_t j=0;j<flagList.size();j++)
                        {
                            if(flagList[j]==flashggtriggerNames.triggerName(i))
                                {
                                    passMETfilters=0;
                                    break;
                                }
                        }
            }
        
        if( ! evt.isRealData() )
            {
                evt.getByToken( genParticleToken_, genParticles );
                for( unsigned int genLoop = 0 ; genLoop < genParticles->size(); genLoop++ )
                    {
                        int pdgid = genParticles->ptrAt( genLoop )->pdgId();
                        int dpdgid[2] = {0,0};
                        if(pdgid ==23) //z-boson
                            {
                                associatedZ=1;
                                if( genParticles->ptrAt( genLoop )->numberOfDaughters()==2)
                                    {
                                        VhasDaughters=1;
                                        dpdgid[0]=genParticles->ptrAt(genLoop)->daughter(0)->pdgId();
                                        //dpdgid[1]=genParticles->ptrAt(genLoop)->daughter(1)->pdgId();
                                        Vpt=genParticles->ptrAt( genLoop )->pt();
                                        if(fabs(dpdgid[0])==12||fabs(dpdgid[0])==14||fabs(dpdgid[0])==16) //look for neutrino decay of Z
                                            {VhasNeutrinos=1;}
                                        if(fabs(dpdgid[0])==11||fabs(dpdgid[0])==13||fabs(dpdgid[0])==15) //look for lepton decay of Z  
                                            {VhasLeptons=1;}
                                        if(fabs(dpdgid[0])>0&&fabs(dpdgid[0])<9) //look for quark decay of Z   
                                            {VhasHadrons=1;}
                                    }
                            }
                        
                        if(fabs(pdgid)==24) //look for W 
                            {
                                associatedW=1;
                                if( genParticles->ptrAt( genLoop )->numberOfDaughters()==2)
                                    {
                                        VhasDaughters=1;
                                        Vpt=genParticles->ptrAt( genLoop )->pt();
                                        dpdgid[0]=genParticles->ptrAt(genLoop)->daughter(0)->pdgId();
                                        //dpdgid[1]=genParticles->ptrAt(genLoop)->daughter(1)->pdgId();
                                        if(fabs(dpdgid[0])==12||fabs(dpdgid[0])==14||fabs(dpdgid[0])==16) //look for neutrino decay of W
                                            {VhasNeutrinos=1;VhasLeptons=1;}
                                        if(fabs(dpdgid[0])==11||fabs(dpdgid[0])==13||fabs(dpdgid[0])==15) //look for lepton decay of W
                                            {VhasNeutrinos=1;VhasLeptons=1;}
                                        if(fabs(dpdgid[0])>0&&fabs(dpdgid[0])<9) //look for quark decay of W 
                                            {VhasHadrons=1;}
                                        
                                    }
                            }
                        if( pdgid == 25 || pdgid == 22 )
                            {
                                higgsVtx = genParticles->ptrAt( genLoop )->vertex();
                                continue;
                            }
                    }
            }
        
        edm::RefProd<vector<VHTagTruth> > rTagTruth = evt.getRefBeforePut<vector<VHTagTruth> >();
        unsigned int idx = 0;
        
        bool photonSelection = false;
        double idmva1 = 0.;
        double idmva2 = 0.;
        
        
        
        for( unsigned int diphoIndex = 0; diphoIndex < diPhotons->size(); diphoIndex++ ) {
            hasGoodElec = false;
            hasGoodMuons = false;
            if(!passMETfilters) {continue;}
            if(useVertex0only_)
                if(diPhotons->ptrAt(diphoIndex)->vertexIndex()!=0)
                    {continue;}
            unsigned int jetCollectionIndex = diPhotons->ptrAt( diphoIndex )->jetCollectionIndex();

            std::vector<edm::Ptr<Jet> > tagJets;
            edm::Ptr<flashgg::Met>  tagMETs;

            edm::Ptr<flashgg::DiPhotonCandidate> dipho = diPhotons->ptrAt( diphoIndex );
            edm::Ptr<flashgg::DiPhotonMVAResult> mvares = mvaResults->ptrAt( diphoIndex );

            WHLeptonicTag whleptonictags_obj( dipho, mvares );
            whleptonictags_obj.includeWeights( *dipho );
            
            if( dipho->leadingPhoton()->pt() < ( dipho->mass() )*leadPhoOverMassThreshold_ ) { continue; }
            if( dipho->subLeadingPhoton()->pt() < ( dipho->mass() )*subleadPhoOverMassThreshold_ ) { continue; }
          
            idmva1 = dipho->leadingPhoton()->phoIdMvaDWrtVtx( dipho->vtx() );
            idmva2 = dipho->subLeadingPhoton()->phoIdMvaDWrtVtx( dipho->vtx() );
            
            if( idmva1 <= PhoMVAThreshold_ || idmva2 <= PhoMVAThreshold_ ) { continue; }
            if( mvares->result < MVAThreshold_ ) { continue; }
            
            photonSelection = true;
            std::vector<edm::Ptr<flashgg::Muon> > goodMuons = selectMuons( theMuons->ptrs(), dipho, vertices->ptrs(), muonEtaThreshold_, leptonPtThreshold_,
                    muPFIsoSumRelThreshold_, deltaRMuonPhoThreshold_, deltaRMuonPhoThreshold_ );
            
            std::vector<edm::Ptr<Electron> >goodElectrons = selectStdElectrons( theElectrons->ptrs(), dipho,vertices->ptrs(), leptonPtThreshold_, electronEtaThresholds_,
                                                                                useElectronMVARecipe_,useElectronLooseID_,
                                                                                deltaRPhoElectronThreshold_,DeltaRTrkElec_,deltaMassElectronZThreshold_,
                                                                                rho_, evt.isRealData() );
            
            hasGoodElec = ( goodElectrons.size() > 0 );
            hasGoodMuons = ( goodMuons.size() > 0 );
                        
            if( !hasGoodElec && !hasGoodMuons ) { continue; }
            
            for( unsigned int candIndex_outer = 0; candIndex_outer < Jets[jetCollectionIndex]->size() ; candIndex_outer++ ) 
                {
                    bool keepJet=true;
                    edm::Ptr<flashgg::Jet> thejet = Jets[jetCollectionIndex]->ptrAt( candIndex_outer );
                    if( ! thejet->passesPuJetId( dipho ) ) { keepJet=false; }
                    if( fabs( thejet->eta() ) > jetEtaThreshold_ ) { keepJet=false; }
                    if( thejet->pt() < jetPtThreshold_ ) { keepJet=false; }
                    float dRPhoLeadJet = deltaR( thejet->eta(), thejet->phi(), dipho->leadingPhoton()->superCluster()->eta(), dipho->leadingPhoton()->superCluster()->phi() ) ;
                    float dRPhoSubLeadJet = deltaR( thejet->eta(), thejet->phi(), dipho->subLeadingPhoton()->superCluster()->eta(),
                                                    dipho->subLeadingPhoton()->superCluster()->phi() );
                    
                    if( dRPhoLeadJet < deltaRPhoLeadJet_ || dRPhoSubLeadJet < deltaRPhoSubLeadJet_ ) { keepJet=false; }
                    if( hasGoodElec ) 
                        for( unsigned int electronIndex = 0; electronIndex < goodElectrons.size(); electronIndex++ ) 
                            {
                                Ptr<flashgg::Electron> electron = goodElectrons[electronIndex];
                                float dRJetElectron = deltaR( thejet->eta(), thejet->phi(), electron->eta(), electron->phi() ) ;
                                if( dRJetElectron < deltaRJetMuonThreshold_ ) { keepJet=false; }
                            }
                    if( hasGoodMuons ) 
                        for( unsigned int muonIndex = 0; muonIndex < goodMuons.size(); muonIndex++ ) 
                            {
                                Ptr<flashgg::Muon> muon = goodMuons[muonIndex];
                                float dRJetMuon = deltaR( thejet->eta(), thejet->phi(), muon->eta(), muon->phi() ) ;
                                if( dRJetMuon < deltaRJetMuonThreshold_ ) { keepJet=false; }
                            }
                    if(keepJet)
                        tagJets.push_back( thejet );
                }
            //------>MET info
            if( METs->size() != 1 ) { std::cout << "WARNING - #MET is not 1" << std::endl;}
            Ptr<flashgg::Met> theMET = METs->ptrAt( 0 );

            //has Nj<thresh; photonSelection; muons>=1||electrons>=1; metPt>thresh
            if( (tagJets.size() < jetsNumberThreshold_) && photonSelection && theMET->getCorPt()>METThreshold_) {
                whleptonictags_obj.setJets( tagJets );
                whleptonictags_obj.setMuons( goodMuons );
                whleptonictags_obj.setElectrons( goodElectrons );
                whleptonictags_obj.setDiPhotonIndex( diphoIndex );
                whleptonictags_obj.setSystLabel( systLabel_ );
                whleptonictags_obj.setMET( theMET );
                whleptonictags->push_back( whleptonictags_obj );
                if( ! evt.isRealData() ) 
                    {
                        VHTagTruth truth_obj;
                        truth_obj.setGenPV( higgsVtx );
                        if ( stage0cat.isValid() ) {
                            truth_obj.setHTXSInfo( *( stage0cat.product() ),
                                                   *( stage1cat.product() ),
                                                   *( njets.product() ),
                                                   *( pTH.product() ),
                                                   *( pTV.product() ) );
                        } else {
                            truth_obj.setHTXSInfo( 0, 0, 0, 0., 0. );
                        }
                        truth_obj.setAssociatedZ( associatedZ );
                        truth_obj.setAssociatedW( associatedW );
                        truth_obj.setVhasDaughters( VhasDaughters );
                        truth_obj.setVhasNeutrinos( VhasNeutrinos );
                        truth_obj.setVhasLeptons( VhasLeptons );
                        truth_obj.setVhasHadrons( VhasHadrons );
                        truth_obj.setVhasMissingLeptons( VhasMissingLeptons );
                        truth_obj.setVpt( Vpt );
                        truths->push_back( truth_obj );
                        whleptonictags->back().setTagTruth( edm::refToPtr( edm::Ref<vector<VHTagTruth> >( rTagTruth, idx++ ) ) );
                    }
            }
        }
        evt.put( whleptonictags );
        evt.put( truths );
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

