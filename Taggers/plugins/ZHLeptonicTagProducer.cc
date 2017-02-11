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
        typedef math::XYZPoint Point;

        ZHLeptonicTagProducer( const ParameterSet & );
    private:
        void produce( Event &, const EventSetup & ) override;

        EDGetTokenT<View<DiPhotonCandidate> > diPhotonToken_;

        EDGetTokenT<View<flashgg::Electron> > electronToken_;
        EDGetTokenT<View<flashgg::Muon> > muonToken_;
        EDGetTokenT<View<DiPhotonMVAResult> > mvaResultToken_;
        EDGetTokenT<View<reco::Vertex> > vertexToken_;
        EDGetTokenT<View<reco::GenParticle> > genParticleToken_;
        EDGetTokenT<double> rhoTag_;
        EDGetTokenT<int> stage0catToken_, stage1catToken_, njetsToken_;
        EDGetTokenT<float> pTHToken_,pTVToken_;
        string systLabel_;
        

        //Thresholds
        double leptonPtThreshold_;
        double muonEtaThreshold_;
        double leadPhoOverMassThreshold_;
        double subleadPhoOverMassThreshold_;
        double MVAThreshold_;
        double deltaRMuonPhoThreshold_;
        double muPFIsoSumRelThreshold_;
        double PhoMVAThreshold_;
        bool useVertex0only_;
        
        double invMassLepLowThreshold_;
        double invMassLepHighThreshold_;
        double deltaRLowPtMuonPhoThreshold_;


        double ElectronPtThreshold_;
        double DeltaRTrkElec_;
        double TransverseImpactParam_;
        double LongitudinalImpactParam_;

        double deltaRPhoElectronThreshold_;
        double deltaMassElectronZThreshold_;


        vector<double> nonTrigMVAThresholds_;
        vector<double> nonTrigMVAEtaCuts_;
        double electronIsoThreshold_;
        double electronNumOfHitsThreshold_;
        vector<double> electronEtaThresholds_;
        bool useElectronMVARecipe_;
        bool useElectronLooseID_;
        
    };

    ZHLeptonicTagProducer::ZHLeptonicTagProducer( const ParameterSet &iConfig ) :
        diPhotonToken_( consumes<View<flashgg::DiPhotonCandidate> >( iConfig.getParameter<InputTag> ( "DiPhotonTag" ) ) ),
        electronToken_( consumes<View<flashgg::Electron> >( iConfig.getParameter<InputTag> ( "ElectronTag" ) ) ),
        muonToken_( consumes<View<flashgg::Muon> >( iConfig.getParameter<InputTag>( "MuonTag" ) ) ),
        mvaResultToken_( consumes<View<flashgg::DiPhotonMVAResult> >( iConfig.getParameter<InputTag> ( "MVAResultTag" ) ) ),
        vertexToken_( consumes<View<reco::Vertex> >( iConfig.getParameter<InputTag> ( "VertexTag" ) ) ),
        genParticleToken_( consumes<View<reco::GenParticle> >( iConfig.getParameter<InputTag> ( "GenParticleTag" ) ) ),
        rhoTag_( consumes<double>( iConfig.getParameter<InputTag>( "rhoTag" ) ) ),
        systLabel_( iConfig.getParameter<string> ( "SystLabel" ) )
    {
        
        leptonPtThreshold_ = iConfig.getParameter<double>( "leptonPtThreshold");
        muonEtaThreshold_ = iConfig.getParameter<double>( "muonEtaThreshold");
        leadPhoOverMassThreshold_ = iConfig.getParameter<double>( "leadPhoOverMassThreshold");
        subleadPhoOverMassThreshold_ = iConfig.getParameter<double>( "subleadPhoOverMassThreshold");
        MVAThreshold_ = iConfig.getParameter<double>( "MVAThreshold");
        deltaRMuonPhoThreshold_ = iConfig.getParameter<double>( "deltaRMuonPhoThreshold");
        muPFIsoSumRelThreshold_ = iConfig.getParameter<double>( "muPFIsoSumRelThreshold");
        PhoMVAThreshold_ = iConfig.getParameter<double>( "PhoMVAThreshold");
        useVertex0only_              = iConfig.getParameter<bool>("useVertex0only");
        
        invMassLepLowThreshold_ = iConfig.getParameter<double>( "invMassLepLowThreshold");
        invMassLepHighThreshold_ = iConfig.getParameter<double>( "invMassLepHighThreshold");
        deltaRLowPtMuonPhoThreshold_ = iConfig.getParameter<double>( "deltaRLowPtMuonPhoThreshold");

        ElectronPtThreshold_ = iConfig.getParameter<double>( "ElectronPtThreshold");
        DeltaRTrkElec_ = iConfig.getParameter<double>( "DeltaRTrkElec");
        TransverseImpactParam_ = iConfig.getParameter<double>( "TransverseImpactParam");
        LongitudinalImpactParam_ = iConfig.getParameter<double>( "LongitudinalImpactParam");
        nonTrigMVAThresholds_ =  iConfig.getParameter<vector<double > >( "nonTrigMVAThresholds");
        nonTrigMVAEtaCuts_ =  iConfig.getParameter<vector<double > >( "nonTrigMVAEtaCuts");
        deltaRPhoElectronThreshold_ = iConfig.getParameter<double>( "deltaRPhoElectronThreshold");
        deltaMassElectronZThreshold_ = iConfig.getParameter<double>( "deltaMassElectronZThreshold");
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

        
        produces<vector<ZHLeptonicTag> >();
        produces<vector<VHTagTruth> >();
    }

    void ZHLeptonicTagProducer::produce( Event &evt, const EventSetup & )
    {
        Handle<int> stage0cat, stage1cat, njets;
        Handle<float> pTH, pTV;
        evt.getByToken(stage0catToken_, stage0cat);
        evt.getByToken(stage1catToken_,stage1cat);
        evt.getByToken(njetsToken_,njets);
        evt.getByToken(pTHToken_,pTH);
        evt.getByToken(pTVToken_,pTV);

        
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

        std::auto_ptr<vector<ZHLeptonicTag> > ZHLeptonicTags( new vector<ZHLeptonicTag> );
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
                                        Vpt=genParticles->ptrAt( genLoop )->pt();
                                        if(fabs(dpdgid[0])==12||fabs(dpdgid[0])==14||fabs(dpdgid[0])==16) //look for neutrino decay of Z
                                            {VhasNeutrinos=1; }
                                        if(fabs(dpdgid[0])==11||fabs(dpdgid[0])==13||fabs(dpdgid[0])==15) //look for lepton decay of Z  
                                            { VhasLeptons=1; }
                                        if(fabs(dpdgid[0])>0&&fabs(dpdgid[0])<9) //look for quark decay of Z  
                                            { VhasHadrons=1;}
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


        Handle<View<reco::Vertex> > vertices;
        evt.getByToken( vertexToken_, vertices );

        assert( diPhotons->size() == mvaResults->size() );

        bool photonSelection = false;
        double idmva1 = 0.;
        double idmva2 = 0.;
        bool isDiMuon = false;
        bool isDiElectron = false;
        for( unsigned int diphoIndex = 0; diphoIndex < diPhotons->size(); diphoIndex++ ) {

            if(useVertex0only_)
                if(diPhotons->ptrAt(diphoIndex)->vertexIndex()!=0)
                    {continue;}
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
            photonSelection = true;
            tagMuonsTemp = selectMuons( theMuons->ptrs(), dipho, vertices->ptrs(), muonEtaThreshold_, leptonPtThreshold_, muPFIsoSumRelThreshold_,
                                           deltaRMuonPhoThreshold_, deltaRMuonPhoThreshold_ );
            tagElectronsTemp = selectStdElectrons( theElectrons->ptrs(), dipho, vertices->ptrs(), leptonPtThreshold_,  electronEtaThresholds_,
                                                      useElectronMVARecipe_,useElectronLooseID_,
                                                   deltaRPhoElectronThreshold_,DeltaRTrkElec_,deltaMassElectronZThreshold_,
                                                   rho_, evt.isRealData() );
            
            if( tagElectronsTemp.size() < 2 && tagMuonsTemp.size()<2) { continue; }
            //check for two good muons
            if( tagMuonsTemp.size() >= 2 ) 
                {
                    for(uint i=0;i<tagMuonsTemp.size()-1;i++)
                        for(uint j=1;j<tagMuonsTemp.size();j++)
                            {
                                math::XYZTLorentzVector leptonPair = tagMuonsTemp[i]->p4()+tagMuonsTemp[j]->p4();
                                
                                if( leptonPair.M() < invMassLepHighThreshold_  && leptonPair.M() > invMassLepLowThreshold_ ) 
                                    {
                                        tagMuons.push_back(tagMuonsTemp[i]);
                                        tagMuons.push_back(tagMuonsTemp[j]);
                                        isDiMuon = true;
                                    }
                            }
                }
            //check for two good electrons
            if( tagElectronsTemp.size() >= 2 ) 
                {
                    for(uint i=0;i<tagElectronsTemp.size()-1;i++)
                        for(uint j=1;j<tagElectronsTemp.size();j++)
                            {
                                math::XYZTLorentzVector leptonPair = tagElectronsTemp[i]->p4()+tagElectronsTemp[j]->p4();
                                if( leptonPair.M() < invMassLepHighThreshold_  && leptonPair.M() > invMassLepLowThreshold_ ) 
                                    {
                                        tagElectrons.push_back(tagElectronsTemp[i]);
                                        tagElectrons.push_back(tagElectronsTemp[j]);
                                        isDiElectron = true;
                                    }
                            }
                }
            if( photonSelection && ( isDiMuon  || isDiElectron )) {
                ZHLeptonicTags_obj.setMuons( tagMuons );
                ZHLeptonicTags_obj.setElectrons( tagElectrons );
                ZHLeptonicTags_obj.setDiPhotonIndex( diphoIndex );
                ZHLeptonicTags_obj.setSystLabel( systLabel_ );
                ZHLeptonicTags->push_back( ZHLeptonicTags_obj );
                if( ! evt.isRealData() ){
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
                    ZHLeptonicTags->back().setTagTruth( edm::refToPtr( edm::Ref<vector<VHTagTruth> >( rTagTruth, idx++ ) ) );
                }
            }
        }
        evt.put( ZHLeptonicTags );
        evt.put( truths );
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

