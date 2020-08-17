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
#include "flashgg/DataFormats/interface/VHTightTag.h"
#include "flashgg/DataFormats/interface/Met.h"
#include "flashgg/DataFormats/interface/Electron.h"
#include "flashgg/DataFormats/interface/Muon.h"

#include "FWCore/Common/interface/TriggerNames.h"
#include "DataFormats/Common/interface/TriggerResults.h"
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
#include "TMath.h"

using namespace std;
using namespace edm;


namespace flashgg {
    class VHTightTagProducer : public EDProducer
    {

    public:
        typedef math::XYZPoint Point;

        VHTightTagProducer( const ParameterSet & );
    private:
        void produce( Event &, const EventSetup & ) override;

        std::vector<edm::EDGetTokenT<View<flashgg::Jet> > > tokenJets_;
        EDGetTokenT<View<DiPhotonCandidate> > diPhotonToken_;
        //EDGetTokenT<View<Jet> > thejetToken_;
        std::vector<edm::InputTag> inputTagJets_;

        EDGetTokenT<View<flashgg::Electron> > electronToken_;
        EDGetTokenT<View<flashgg::Muon> > muonToken_;
        EDGetTokenT<View<DiPhotonMVAResult> > mvaResultToken_;
        EDGetTokenT<View<flashgg::Met> > METToken_;
        EDGetTokenT<View<reco::Vertex> > vertexToken_;
        EDGetTokenT<View<reco::GenParticle> > genParticleToken_;
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
        //			double PuIDCutoffThreshold_;
        double PhoMVAThreshold_;
        double METThreshold_;

        double deltaRJetMuonThreshold_;
        double deltaRJetElectronThreshold_;
        double invMassLepLowThreshold_;
        double invMassLepHighThreshold_;
        double numberOfLowPtMuonsThreshold_;
        double numberOfHighPtMuonsThreshold_;
        double leptonLowPtThreshold_;
        double deltaRLowPtMuonPhoThreshold_;

        double deltaRPhoLeadJet_;
        double deltaRPhoSubLeadJet_;

        double ElectronPtThreshold_;
        double DeltaRTrkElec_;
        double TransverseImpactParam_;
        double LongitudinalImpactParam_;

        double deltaRPhoElectronThreshold_;
        double deltaMassElectronZThreshold_;

        bool hasGoodMuons_highPt = false;
        bool hasGoodMuons_lowPt = false;

        bool hasGoodElectrons_highPt = false;
        bool hasGoodElectrons_lowPt = false;

        vector<double> nonTrigMVAThresholds_;
        vector<double> nonTrigMVAEtaCuts_;
        double electronIsoThreshold_;
        double electronNumOfHitsThreshold_;
        vector<double> electronEtaThresholds_;
        bool useElectronMVARecipe_;
        bool useElectronLooseID_;
        
    };

    VHTightTagProducer::VHTightTagProducer( const ParameterSet &iConfig ) :
        diPhotonToken_( consumes<View<flashgg::DiPhotonCandidate> >( iConfig.getParameter<InputTag> ( "DiPhotonTag" ) ) ),
        //thejetToken_( consumes<View<flashgg::Jet> >( iConfig.getParameter<InputTag>( "JetTag" ) ) ),
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

        deltaRJetMuonThreshold_ = iConfig.getParameter<double>( "deltaRJetMuonThreshold");
        deltaRJetElectronThreshold_ = iConfig.getParameter<double>( "deltaRJetElectronThreshold");
        invMassLepLowThreshold_ = iConfig.getParameter<double>( "invMassLepLowThreshold");
        invMassLepHighThreshold_ = iConfig.getParameter<double>( "invMassLepHighThreshold");
        numberOfLowPtMuonsThreshold_ = iConfig.getParameter<double>( "numberOfLowPtMuonsThreshold");
        numberOfHighPtMuonsThreshold_ = iConfig.getParameter<double>( "numberOfHighPtMuonsThreshold");
        leptonLowPtThreshold_ = iConfig.getParameter<double>( "leptonLowPtThreshold");
        deltaRLowPtMuonPhoThreshold_ = iConfig.getParameter<double>( "deltaRLowPtMuonPhoThreshold");
        deltaRPhoLeadJet_ = iConfig.getParameter<double>( "deltaRPhoLeadJet");
        deltaRPhoSubLeadJet_ = iConfig.getParameter<double>( "deltaRPhoSubLeadJet");

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
        
        for (unsigned i = 0 ; i < inputTagJets_.size() ; i++) {
            auto token = consumes<View<flashgg::Jet> >(inputTagJets_[i]);
            tokenJets_.push_back(token);
        }
        produces<vector<VHTightTag> >();
        produces<vector<VHTagTruth> >();
    }

    void VHTightTagProducer::produce( Event &evt, const EventSetup & )
    {

        //Handle<View<flashgg::Jet> > theJets;
        //evt.getByToken( thejetToken_, theJets );
        //const PtrVector<flashgg::Jet>& jetPointers = theJets->ptrVector();
        JetCollectionVector Jets( inputTagJets_.size() );
        for( size_t j = 0; j < inputTagJets_.size(); ++j ) {
            evt.getByToken( tokenJets_[j], Jets[j] );
        }
        
        Handle<View<flashgg::DiPhotonCandidate> > diPhotons;
        evt.getByToken( diPhotonToken_, diPhotons );
        //const PtrVector<flashgg::DiPhotonCandidate>& diPhotonPointers = diPhotons->ptrVector();

        Handle<View<flashgg::Muon> > theMuons;
        evt.getByToken( muonToken_, theMuons );
        //const PtrVector<flashgg::Muon>& muonPointers = theMuons->ptrVector();

        Handle<View<flashgg::Electron> > theElectrons;
        evt.getByToken( electronToken_, theElectrons );
        //const PtrVector<flashgg::Electron>& electronPointers = theElectrons->ptrVector();

        edm::Handle<double>  rho;
        evt.getByToken(rhoTag_,rho);
        double rho_    = *rho;

        Handle<View<flashgg::DiPhotonMVAResult> > mvaResults;
        evt.getByToken( mvaResultToken_, mvaResults );
        //const PtrVector<flashgg::DiPhotonMVAResult>& mvaResultPointers = mvaResults->ptrVector();

        Handle<View<reco::GenParticle> > genParticles;

        std::unique_ptr<vector<VHTightTag> > VHTightTags( new vector<VHTightTag> );
        std::unique_ptr<vector<VHTagTruth> > truths( new vector<VHTagTruth> );

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
                            //if(flagList(j)==triggerNames.triggerName(i))                                                                                                                         
                            if(flagList[j]==triggerNames.triggerName(i))
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
                                            {
                                                VhasNeutrinos=1;
                                            }
                                        if(fabs(dpdgid[0])==11||fabs(dpdgid[0])==13||fabs(dpdgid[0])==15) //look for lepton decay of Z  
                                            {
                                                VhasLeptons=1;
                                            }
                                        if(fabs(dpdgid[0])>0&&fabs(dpdgid[0])<9) //look for quark decay of Z  
                                            {
                                                VhasHadrons=1;
                                            }
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
                                            {
                                                VhasNeutrinos=1;
                                                VhasLeptons=1;
                                            }
                                        if(fabs(dpdgid[0])==11||fabs(dpdgid[0])==13||fabs(dpdgid[0])==15) //look for lepton decay of W  
                                            {
                                                VhasNeutrinos=1;
                                                VhasLeptons=1;
                                            }
                                        if(fabs(dpdgid[0])>0&&fabs(dpdgid[0])<9) //look for quark decay of W   
                                            {
                                                VhasHadrons=1;
                                            }

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

        Handle<View<flashgg::Met> > METs;
        evt.getByToken( METToken_, METs );
//const PtrVector<pat::MET>& METPointers = METs->ptrVector();

        Handle<View<reco::Vertex> > vertices;
        evt.getByToken( vertexToken_, vertices );
//const PtrVector<reco::Vertex>& vertexPointers = vertices->ptrVector();

        assert( diPhotons->size() == mvaResults->size() );

        bool photonSelection = false;
        double idmva1 = 0.;
        double idmva2 = 0.;

        bool isMuonHighPt = false;
        bool isMuonLowPt = false;
        bool isInvMassOK = false;

        bool isElectronHighPt = false;
        bool isElectronLowPt = false;
        bool isInvMassOK_elec = false;

        for( unsigned int diphoIndex = 0; diphoIndex < diPhotons->size(); diphoIndex++ ) {
            hasGoodMuons_highPt = false;
            hasGoodMuons_lowPt = false;
            hasGoodElectrons_highPt = false;
            hasGoodElectrons_lowPt = false;
            if(!passMETfilters)
                {
                    continue;
                }
            unsigned int jetCollectionIndex = diPhotons->ptrAt( diphoIndex )->jetCollectionIndex();
            
            std::vector<edm::Ptr<flashgg::Muon> > tagMuons_highPt;
            std::vector<edm::Ptr<flashgg::Muon> > tagMuons_lowPt;
            std::vector<edm::Ptr<flashgg::Muon> > tagMuons;
            std::vector<edm::Ptr<Electron> > tagElectrons_highPt;
            std::vector<edm::Ptr<Electron> > tagElectrons_lowPt;
            std::vector<edm::Ptr<Electron> > tagElectrons;

            isMuonHighPt = false;
            isMuonLowPt = false;
            isInvMassOK = false;

            isElectronHighPt = false;
            isElectronLowPt = false;
            isInvMassOK_elec = false;

            std::vector<edm::Ptr<Jet> > tagJets;
            //std::vector<edm::Ptr<flashgg::Met> > tagMETs;

            edm::Ptr<flashgg::DiPhotonCandidate> dipho = diPhotons->ptrAt( diphoIndex );
            edm::Ptr<flashgg::DiPhotonMVAResult> mvares = mvaResults->ptrAt( diphoIndex );

            VHTightTag VHTightTags_obj( dipho, mvares );
            VHTightTags_obj.includeWeights( *dipho );
            if( dipho->leadingPhoton()->pt() < ( dipho->mass() )*leadPhoOverMassThreshold_ ) { continue; }
            if( dipho->subLeadingPhoton()->pt() < ( dipho->mass() )*subleadPhoOverMassThreshold_ ) { continue; }
            idmva1 = dipho->leadingPhoton()->phoIdMvaDWrtVtx( dipho->vtx() );
            idmva2 = dipho->subLeadingPhoton()->phoIdMvaDWrtVtx( dipho->vtx() );
            if( idmva1 <= PhoMVAThreshold_ || idmva2 <= PhoMVAThreshold_ ) { continue; }
            if( mvares->result < MVAThreshold_ ) { continue; }

            photonSelection = true;
            tagMuons_highPt = selectMuons( theMuons->ptrs(), dipho, vertices->ptrs(), muonEtaThreshold_, leptonPtThreshold_, muPFIsoSumRelThreshold_,
                                           deltaRMuonPhoThreshold_, deltaRMuonPhoThreshold_ );
            tagMuons_lowPt = selectMuons( theMuons->ptrs(), dipho, vertices->ptrs(), muonEtaThreshold_, leptonLowPtThreshold_, muPFIsoSumRelThreshold_,
                                          deltaRLowPtMuonPhoThreshold_, deltaRLowPtMuonPhoThreshold_ );
            hasGoodMuons_highPt = ( tagMuons_highPt.size() > 0 );
            hasGoodMuons_lowPt = ( tagMuons_lowPt.size() > 0 );

            tagElectrons_highPt = selectStdElectrons( theElectrons->ptrs(), dipho, vertices->ptrs(), leptonPtThreshold_,  electronEtaThresholds_,
                                                      useElectronMVARecipe_,useElectronLooseID_,
                                                      deltaRPhoElectronThreshold_,DeltaRTrkElec_,deltaMassElectronZThreshold_,
                                                      rho_, evt.isRealData() );
            
            tagElectrons_lowPt = selectStdElectrons( theElectrons->ptrs(), dipho, vertices->ptrs(), leptonLowPtThreshold_, electronEtaThresholds_,
                                                     useElectronMVARecipe_,useElectronLooseID_,
                                                     deltaRPhoElectronThreshold_,DeltaRTrkElec_,deltaMassElectronZThreshold_,
                                                     rho_, evt.isRealData() );

        
            //std::vector<edm::Ptr<Electron> > goodElectrons = selectStdElectrons( theElectrons->ptrs(), dipho, vertices->ptrs(), leptonPtThreshold_, 
            //                                                                 TransverseImpactParam_, LongitudinalImpactParam_, nonTrigMVAThresholds_, nonTrigMVAEtaCuts_,
            //                                                                    electronIsoThreshold_, electronNumOfHitsThreshold_, electronEtaThresholds_ ,
            //                                                                    deltaRPhoElectronThreshold_,DeltaRTrkElec_,deltaMassElectronZThreshold_);

            
            hasGoodElectrons_highPt = ( tagElectrons_highPt.size() > 0 );
            hasGoodElectrons_lowPt = ( tagElectrons_lowPt.size() > 0 );

            if( !hasGoodMuons_highPt && !hasGoodMuons_lowPt && !hasGoodElectrons_highPt && !hasGoodElectrons_lowPt ) { continue; }
            //std::cout << "tight tag has leptons" << std::endl;
            if( !hasGoodMuons_highPt && !hasGoodElectrons_highPt && !hasGoodElectrons_lowPt ) {
                if( tagMuons_lowPt.size() >= 2 ) 
                    {
                        isMuonLowPt = true;
                        float invMass2 = 2 * tagMuons_lowPt[0]->pt() * tagMuons_lowPt[1]->pt() * ( cosh( tagMuons_lowPt[0]->eta() - tagMuons_lowPt[1]->eta() ) - cos(tagMuons_lowPt[0]->phi() - tagMuons_lowPt[1]->phi() ) );
                        if( sqrt( invMass2 ) < invMassLepHighThreshold_  && sqrt( invMass2 ) > invMassLepLowThreshold_ ) {
                            tagMuons = tagMuons_lowPt;
                            isInvMassOK = true;
                        }
                    }
            }

            if( hasGoodMuons_highPt && !hasGoodElectrons_highPt && !hasGoodElectrons_lowPt ) {
                if( tagMuons_highPt.size() == 1 ) {
                    tagMuons = tagMuons_highPt;
                    isMuonHighPt = true;
                }
                if( tagMuons_highPt.size() >= 2 ) {
                    isMuonLowPt = true;
                    float invMass2_high = 2 * tagMuons_highPt[0]->pt() * tagMuons_highPt[1]->pt() * ( cosh( tagMuons_highPt[0]->eta() - tagMuons_highPt[1]->eta() ) - cos(tagMuons_highPt[0]->phi() - tagMuons_highPt[1]->phi() ) );
                    if( sqrt( invMass2_high ) < invMassLepHighThreshold_ && sqrt( invMass2_high ) > invMassLepLowThreshold_ ) {
                        tagMuons = tagMuons_highPt;
                        isInvMassOK = true;
                    }

                }
            }

            
            if( !hasGoodElectrons_highPt && !hasGoodMuons_highPt && !hasGoodMuons_lowPt ) {
                if( tagElectrons_lowPt.size() >= 2 ) {
                    isElectronLowPt = true;
                    float invMass2 = 2 * tagElectrons_lowPt[0]->pt() * tagElectrons_lowPt[1]->pt() * ( cosh( tagElectrons_lowPt[0]->eta() - tagElectrons_lowPt[1]->eta() ) - cos(
                                         tagElectrons_lowPt[0]->phi() - tagElectrons_lowPt[1]->phi() ) );
                    if( sqrt( invMass2 ) < invMassLepHighThreshold_  && sqrt( invMass2 ) > invMassLepLowThreshold_ ) {
                        tagElectrons = tagElectrons_lowPt;
                        isInvMassOK_elec = true;
                    }
                }
            }
            if( hasGoodElectrons_highPt && !hasGoodMuons_highPt && !hasGoodMuons_lowPt ) {
                if( tagElectrons_highPt.size() == 1 ) {
                    tagElectrons = tagElectrons_highPt;
                    isElectronHighPt = true;
                }
                if( tagElectrons_highPt.size() >= 2 ) {
                    isElectronLowPt = true;
                    float invMass2_high = 2 * tagElectrons_highPt[0]->pt() * tagElectrons_highPt[1]->pt() * ( cosh( tagElectrons_highPt[0]->eta() - tagElectrons_highPt[1]->eta() )
                                          - cos( tagElectrons_highPt[0]->phi() - tagElectrons_highPt[1]->phi() ) );
                    if( sqrt( invMass2_high ) < invMassLepHighThreshold_ && sqrt( invMass2_high ) > invMassLepLowThreshold_ ) {
                        tagElectrons = tagElectrons_highPt;
                        isInvMassOK_elec = true;
                    }
                }
            }
            
            for( unsigned int candIndex_outer = 0; candIndex_outer < Jets[jetCollectionIndex]->size() ; candIndex_outer++ ) 
                {
                    bool keepJet=true;
                    edm::Ptr<flashgg::Jet> thejet = Jets[jetCollectionIndex]->ptrAt( candIndex_outer );
                    if(!thejet->passesJetID  ( flashgg::Tight2017 ) ) { keepJet=false; }
                    if( fabs( thejet->eta() ) > jetEtaThreshold_ ) { keepJet=false; }
                    if( thejet->pt() < jetPtThreshold_ ) { keepJet=false; }
                    float dRPhoLeadJet = deltaR( thejet->eta(), thejet->phi(), dipho->leadingPhoton()->superCluster()->eta(), dipho->leadingPhoton()->superCluster()->phi() ) ;
                    float dRPhoSubLeadJet = deltaR( thejet->eta(), thejet->phi(), dipho->subLeadingPhoton()->superCluster()->eta(),
                                                    dipho->subLeadingPhoton()->superCluster()->phi() );
                    if( dRPhoLeadJet < deltaRPhoLeadJet_ || dRPhoSubLeadJet < deltaRPhoSubLeadJet_ ) { keepJet=false; }
                    if( tagElectrons.size() > 0 ) 
                        {
                            for( unsigned int elecIndex = 0; elecIndex < tagElectrons.size(); elecIndex++ ) 
                                {
                                    Ptr<Electron> elec = tagElectrons[elecIndex];
                                    float dRJetElec = deltaR( thejet->eta(), thejet->phi(), elec->eta(), elec->phi() ) ;
                                    if( dRJetElec < deltaRJetElectronThreshold_ ) { keepJet=false; }
                                }
                            
                        }
                    if( tagMuons.size() > 0 ) 
                        {
                            for( unsigned int muonIndex = 0; muonIndex < tagMuons.size(); muonIndex++ ) 
                                {
                                    Ptr<flashgg::Muon> muon = tagMuons[muonIndex];
                                    float dRJetMuon = deltaR( thejet->eta(), thejet->phi(), muon->eta(), muon->phi() ) ;
                                    if( dRJetMuon < deltaRJetMuonThreshold_ ) { keepJet=false; }
                                }   
                        }
                    if(keepJet)
                        tagJets.push_back( thejet );        
                }
            
            if( METs->size() != 1 ) { std::cout << "WARNING - #MET is not 1" << std::endl;}
            Ptr<flashgg::Met> theMET = METs->ptrAt( 0 );
            //if( ( isMuonHighPt || isElectronHighPt ) && theMET->pt() > METThreshold_ ) {
            //tagMETs.push_back( theMET );
            // }
            //            std::cout << "tight met value: " << theMET->getCorPt() << std::endl;
            if( photonSelection && ( ( ( isMuonHighPt && theMET->getCorPt() >METThreshold_  && tagJets.size() < jetsNumberThreshold_ ) || ( isMuonLowPt && isInvMassOK ) ) ||
                                     ( isElectronHighPt && theMET->getCorPt() > METThreshold_ && tagJets.size() < jetsNumberThreshold_ ) || ( isElectronLowPt && isInvMassOK_elec ) ) ) {
                VHTightTags_obj.setJets( tagJets );
                VHTightTags_obj.setMuons( tagMuons );
                VHTightTags_obj.setElectrons( tagElectrons );
                //                VHTightTags_obj.setMET( tagMETs );
                VHTightTags_obj.setMET( theMET );
                VHTightTags_obj.setDiPhotonIndex( diphoIndex );
                VHTightTags_obj.setSystLabel( systLabel_ );
                VHTightTags->push_back( VHTightTags_obj );
                if( ! evt.isRealData() ) {
                    VHTagTruth truth_obj;
                    truth_obj.setGenPV( higgsVtx );
                    truth_obj.setAssociatedZ( associatedZ );
                    truth_obj.setAssociatedW( associatedW );
                    truth_obj.setVhasDaughters( VhasDaughters );
                    truth_obj.setVhasNeutrinos( VhasNeutrinos );
                    truth_obj.setVhasLeptons( VhasLeptons );
                    truth_obj.setVhasHadrons( VhasHadrons );
                    truth_obj.setVhasMissingLeptons( VhasMissingLeptons );
                    truth_obj.setVpt( Vpt );
                    truths->push_back( truth_obj );
                    VHTightTags->back().setTagTruth( edm::refToPtr( edm::Ref<vector<VHTagTruth> >( rTagTruth, idx++ ) ) );
                }
            }
        }
        evt.put( std::move( VHTightTags ) );
        evt.put( std::move( truths ) );
    }

}
typedef flashgg::VHTightTagProducer FlashggVHTightTagProducer;
DEFINE_FWK_MODULE( FlashggVHTightTagProducer );
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

