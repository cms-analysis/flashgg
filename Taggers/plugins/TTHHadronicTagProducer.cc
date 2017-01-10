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

#include "DataFormats/Math/interface/deltaR.h"

#include "flashgg/DataFormats/interface/TagTruthBase.h"
#include "DataFormats/Common/interface/RefToPtr.h"

#include <vector>
#include <algorithm>
#include <string>
#include <utility>

#include "TMVA/Reader.h"

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
        std::vector<edm::InputTag> inputTagJets_;
        EDGetTokenT<View<Electron> > electronToken_;
        EDGetTokenT<View<flashgg::Muon> > muonToken_;
        EDGetTokenT<View<reco::Vertex> > vertexToken_;
        EDGetTokenT<View<DiPhotonMVAResult> > mvaResultToken_;
        EDGetTokenT<View<reco::GenParticle> > genParticleToken_;
        string systLabel_;


        typedef std::vector<edm::Handle<edm::View<flashgg::Jet> > > JetCollectionVector;
        bool useTTHHadronicMVA_;
        double tthHadMVAThreshold_;
        //---thresholds---
        //---photons
        double MVAThreshold_;
        double MVATTHHMVAThreshold_;
        double PhoMVAThreshold_;
        double leadPhoPtThreshold_;
        bool   leadPhoUseVariableTh_;
        double leadPhoOverMassThreshold_;
        double leadPhoOverMassTTHHMVAThreshold_;
        double subleadPhoPtThreshold_;
        bool   subleadPhoUseVariableTh_;
        double subleadPhoOverMassThreshold_;
        //---jets
        double jetPtThreshold_;
        double jetEtaThreshold_;
        double dRJetPhoLeadCut_;
        double dRJetPhoSubleadCut_;
        vector<double> bDiscriminator_;
        double jetsNumberThreshold_;
        double bjetsNumberThreshold_;
        double bjetsLooseNumberThreshold_;
        double jetsNumberTTHHMVAThreshold_;
        double bjetsNumberTTHHMVAThreshold_;
        double bjetsLooseNumberTTHHMVAThreshold_;
        string bTag_;
        //leptons
        double leptonPtThreshold_;
        double muonEtaThreshold_;
        vector<double> nonTrigMVAThresholds_;
        vector<double> nonTrigMVAEtaCuts_;
        double electronIsoThreshold_;
        double elMiniIsoEBThreshold_;
        double elMiniIsoEEThreshold_;
        double electronNumOfHitsThreshold_;
        vector<double>  electronEtaThresholds_;
        double muPFIsoSumRelThreshold_;
        double muMiniIsoSumRelThreshold_;
        double TransverseImpactParam_EB;
        double LongitudinalImpactParam_EB;
        double TransverseImpactParam_EE;
        double LongitudinalImpactParam_EE;

        bool useStdLeptonID_;
        bool useElectronMVARecipe_;
        bool useElectronLooseID_;

        unique_ptr<TMVA::Reader>TThMva_;
        FileInPath tthMVAweightfile_;
        string _MVAMethod;

    };

    TTHHadronicTagProducer::TTHHadronicTagProducer( const ParameterSet &iConfig ) :
        diPhotonToken_( consumes<View<flashgg::DiPhotonCandidate> >( iConfig.getParameter<InputTag> ( "DiPhotonTag" ) ) ),
        inputTagJets_( iConfig.getParameter<std::vector<edm::InputTag> >( "inputTagJets" ) ),
        electronToken_( consumes<View<flashgg::Electron> >( iConfig.getParameter<InputTag>( "ElectronTag" ) ) ),
        muonToken_( consumes<View<flashgg::Muon> >( iConfig.getParameter<InputTag>( "MuonTag" ) ) ),
        vertexToken_( consumes<View<reco::Vertex> >( iConfig.getParameter<InputTag> ( "VertexTag" ) ) ),
        mvaResultToken_( consumes<View<flashgg::DiPhotonMVAResult> >( iConfig.getParameter<InputTag>( "MVAResultTag" ) ) ),
        genParticleToken_( consumes<View<reco::GenParticle> >( iConfig.getParameter<InputTag> ( "GenParticleTag" ) ) ),
        systLabel_( iConfig.getParameter<string> ( "SystLabel" ) ),
        _MVAMethod( iConfig.getParameter<string> ( "MVAMethod" ) )
    {


        MVAThreshold_ = iConfig.getParameter<double>( "MVAThreshold");
        MVATTHHMVAThreshold_ = iConfig.getParameter<double>( "MVATTHHMVAThreshold");
        PhoMVAThreshold_ = iConfig.getParameter<double>( "PhoMVAThreshold");

        leptonPtThreshold_ = iConfig.getParameter<double>( "leptonPtThreshold");
        muonEtaThreshold_ = iConfig.getParameter<double>( "muonEtaThreshold");
        leadPhoPtThreshold_ = iConfig.getParameter<double>( "leadPhoPtThreshold");
        leadPhoUseVariableTh_ = iConfig.getParameter<bool>( "leadPhoUseVariableThreshold");
        leadPhoOverMassThreshold_ = iConfig.getParameter<double>( "leadPhoOverMassThreshold");
        leadPhoOverMassTTHHMVAThreshold_ = iConfig.getParameter<double>( "leadPhoOverMassTTHHMVAThreshold");
        subleadPhoPtThreshold_ = iConfig.getParameter<double>( "subleadPhoPtThreshold");
        subleadPhoUseVariableTh_ = iConfig.getParameter<bool>( "subleadPhoUseVariableThreshold");
        subleadPhoOverMassThreshold_ = iConfig.getParameter<double>( "subleadPhoOverMassThreshold");
        jetPtThreshold_ = iConfig.getParameter<double>( "jetPtThreshold");
        jetEtaThreshold_ = iConfig.getParameter<double>( "jetEtaThreshold");
        dRJetPhoLeadCut_ = iConfig.getParameter<double>( "dRJetPhoLeadCut");
        dRJetPhoSubleadCut_ = iConfig.getParameter<double>( "dRJetPhoSubleadCut");
        bDiscriminator_ = iConfig.getParameter<vector<double > >( "bDiscriminator");
        jetsNumberThreshold_ = iConfig.getParameter<int>( "jetsNumberThreshold");
        bjetsNumberThreshold_ = iConfig.getParameter<int>( "bjetsNumberThreshold");
        bTag_ = iConfig.getParameter<string> ( "bTag");

        useTTHHadronicMVA_ = iConfig.getParameter<bool>( "useTTHHadronicMVA");
        tthHadMVAThreshold_ =  iConfig.getParameter<double>( "tthHadMVAThreshold");
        bjetsLooseNumberThreshold_ = iConfig.getParameter<int>( "bjetsLooseNumberThreshold");
        jetsNumberTTHHMVAThreshold_ = iConfig.getParameter<int>( "jetsNumberTTHHMVAThreshold");
        bjetsNumberTTHHMVAThreshold_ = iConfig.getParameter<int>( "bjetsNumberTTHHMVAThreshold");
        bjetsLooseNumberTTHHMVAThreshold_ = iConfig.getParameter<int>( "bjetsLooseNumberTTHHMVAThreshold");

        muPFIsoSumRelThreshold_ = iConfig.getParameter<double>( "muPFIsoSumRelThreshold");
        muMiniIsoSumRelThreshold_ = iConfig.getParameter<double>( "muMiniIsoSumRelThreshold");
        nonTrigMVAThresholds_ =  iConfig.getParameter<vector<double > >( "nonTrigMVAThresholds");
        nonTrigMVAEtaCuts_ =  iConfig.getParameter<vector<double > >( "nonTrigMVAEtaCuts");
        electronIsoThreshold_ = iConfig.getParameter<double>( "electronIsoThreshold");
        elMiniIsoEBThreshold_ = iConfig.getParameter<double>( "elMiniIsoEBThreshold");
        elMiniIsoEEThreshold_ = iConfig.getParameter<double>( "elMiniIsoEEThreshold");
        electronNumOfHitsThreshold_ = iConfig.getParameter<double>( "electronNumOfHitsThreshold");
        TransverseImpactParam_EB = iConfig.getParameter<double>( "TransverseImpactParamEB");
        LongitudinalImpactParam_EB = iConfig.getParameter<double>( "LongitudinalImpactParamEB");
        TransverseImpactParam_EE = iConfig.getParameter<double>( "TransverseImpactParamEE");
        LongitudinalImpactParam_EE = iConfig.getParameter<double>( "LongitudinalImpactParamEE");
        electronEtaThresholds_ = iConfig.getParameter<vector<double > >( "electronEtaThresholds");
        useStdLeptonID_=iConfig.getParameter<bool>("useStdLeptonID");
        useElectronMVARecipe_=iConfig.getParameter<bool>("useElectronMVARecipe");
        useElectronLooseID_=iConfig.getParameter<bool>("useElectronLooseID");
        

        tthMVAweightfile_ = iConfig.getParameter<edm::FileInPath>( "tthMVAweightfile" );        

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

        std::vector<edm::Ptr<flashgg::Muon> > goodMuons;
        if( !useStdLeptonID_) {
            goodMuons = selectAllMuonsSum16( theMuons->ptrs(), vertices->ptrs(), muonEtaThreshold_ , leptonPtThreshold_, muMiniIsoSumRelThreshold_ );
        } else {
            goodMuons = selectAllMuons( theMuons->ptrs(), vertices->ptrs(), muonEtaThreshold_ , leptonPtThreshold_, muPFIsoSumRelThreshold_ );
        }
        
       
        std::vector<edm::Ptr<Electron> > goodElectrons ;

        if( !useStdLeptonID_) {
             // goodElectrons = selectAllElectrons( theElectrons->ptrs(), vertices->ptrs(), leptonPtThreshold_, 
             //                                     TransverseImpactParam_, LongitudinalImpactParam_, nonTrigMVAThresholds_, nonTrigMVAEtaCuts_,
             //                                     electronIsoThreshold_, electronNumOfHitsThreshold_, electronEtaThresholds_ );
            //goodElectrons = selectAllElectronsSum16( theElectrons->ptrs(), vertices->ptrs(), 
            //                                       leptonPtThreshold_, electronEtaThresholds_,
            //                                       true, true, elMiniIsoEBThreshold_, elMiniIsoEEThreshold_);
            goodElectrons = selectAllElectronsSum16( theElectrons->ptrs(), vertices->ptrs(), leptonPtThreshold_, electronEtaThresholds_,
                                                     true, true, elMiniIsoEBThreshold_, elMiniIsoEEThreshold_,
                                                     TransverseImpactParam_EB, LongitudinalImpactParam_EB, TransverseImpactParam_EE, LongitudinalImpactParam_EE);
        } else {
            goodElectrons = selectStdAllElectrons(theElectrons->ptrs(), vertices->ptrs(), leptonPtThreshold_, electronEtaThresholds_,
                                                  useElectronMVARecipe_, useElectronLooseID_);
        }
        
        
        for( unsigned int diphoIndex = 0; diphoIndex < diPhotons->size(); diphoIndex++ ) {

            if( goodElectrons.size() > 0 ||  goodMuons.size() > 0 )  continue; 

            int jetcount = 0;
            float nJets = 0;
            int njets_btagloose = 0;
            int njets_btagmedium = 0;
            int njets_btagtight = 0;
            double idmva1 = 0.;
            double idmva2 = 0.;
            float leadJetPt = 0.;
            float subLeadJetPt = 0.;
            float sumJetPt = 0.;
            float maxBTagVal = -10.;
            float secondMaxBTagVal = -10.;
            float tthMvaVal = -999.;

            if (_MVAMethod != ""){
                TThMva_.reset( new TMVA::Reader( "!Color:Silent" ) );
                TThMva_->AddVariable( "nJetsTTH", &nJets);
                TThMva_->AddVariable( "maxBTagVal",&maxBTagVal);
                TThMva_->AddVariable( "secondMaxBTagVal", &secondMaxBTagVal);
                TThMva_->AddVariable( "leadJetPt", &leadJetPt);

                TThMva_->BookMVA( _MVAMethod.c_str() , tthMVAweightfile_.fullPath() );

            }

            unsigned int jetCollectionIndex = diPhotons->ptrAt( diphoIndex )->jetCollectionIndex();

            std::vector<edm::Ptr<flashgg::Jet> > JetVect;
            JetVect.clear();
            std::vector<edm::Ptr<flashgg::Jet> > BJetVect;
            BJetVect.clear();
            std::vector<edm::Ptr<flashgg::Jet> > BJetTTHHMVAVect;
            BJetTTHHMVAVect.clear();
            std::vector<float> JetBTagVal;
            JetBTagVal.clear();

            edm::Ptr<flashgg::DiPhotonCandidate> dipho = diPhotons->ptrAt( diphoIndex );

            idmva1 = dipho->leadingPhoton()->phoIdMvaDWrtVtx( dipho->vtx() );
            idmva2 = dipho->subLeadingPhoton()->phoIdMvaDWrtVtx( dipho->vtx() );

            if( idmva1 <= PhoMVAThreshold_ || idmva2 <= PhoMVAThreshold_ ) { continue; }

            edm::Ptr<flashgg::DiPhotonMVAResult> mvares = mvaResults->ptrAt( diphoIndex );

            if( !dipho->leadingPhoton()->passElectronVeto() || !dipho->subLeadingPhoton()->passElectronVeto() ) { continue; }

            double leadPhoPtCut = leadPhoPtThreshold_;
            double subleadPhoPtCut = subleadPhoPtThreshold_;
            if( leadPhoUseVariableTh_ )
            { 
                leadPhoPtCut = leadPhoOverMassThreshold_ * dipho->mass(); 
                if(useTTHHadronicMVA_){
                    leadPhoPtCut = leadPhoOverMassTTHHMVAThreshold_ * dipho->mass();
                }
            }
            if( subleadPhoUseVariableTh_ )
            { subleadPhoPtCut = subleadPhoOverMassThreshold_ * dipho->mass(); }
            double diphoMVAcut = MVAThreshold_;
            if(useTTHHadronicMVA_){
                    diphoMVAcut = MVATTHHMVAThreshold_;
            }

            if( dipho->leadingPhoton()->pt() < leadPhoPtCut || dipho->subLeadingPhoton()->pt() < subleadPhoPtCut ) { continue; }
            if( mvares->mvaValue() < diphoMVAcut ) { continue; }

            for( unsigned int jetIndex = 0; jetIndex < Jets[jetCollectionIndex]->size() ; jetIndex++ ) {
                edm::Ptr<flashgg::Jet> thejet = Jets[jetCollectionIndex]->ptrAt( jetIndex );
                if( fabs( thejet->eta() ) > jetEtaThreshold_ ) { continue; }
                
                float dRPhoLeadJet = deltaR( thejet->eta(), thejet->phi(), dipho->leadingPhoton()->superCluster()->eta(), dipho->leadingPhoton()->superCluster()->phi() ) ;
                float dRPhoSubLeadJet = deltaR( thejet->eta(), thejet->phi(), dipho->subLeadingPhoton()->superCluster()->eta(),
                                                dipho->subLeadingPhoton()->superCluster()->phi() );

                if( dRPhoLeadJet < dRJetPhoLeadCut_ || dRPhoSubLeadJet < dRJetPhoSubleadCut_ ) { continue; }
                if( thejet->pt() < jetPtThreshold_ ) { continue; }

                jetcount++;
                nJets = jetcount;
                JetVect.push_back( thejet );
                
                float jetPt = thejet->pt();
                if(jetPt > leadJetPt){
                    if(leadJetPt > subLeadJetPt) { subLeadJetPt = leadJetPt; }
                    leadJetPt = jetPt;
                } else if(jetPt > subLeadJetPt){
                    subLeadJetPt = jetPt;
                }
                sumJetPt += jetPt;
                
                float bDiscriminatorValue = -2.;
                bDiscriminatorValue = thejet->bDiscriminator( bTag_ );

                if(bDiscriminatorValue > maxBTagVal){
                    BJetTTHHMVAVect.insert( BJetTTHHMVAVect.begin(), thejet );
                    if(BJetTTHHMVAVect.size() >= 3){ BJetTTHHMVAVect.pop_back(); }
                    if(maxBTagVal > secondMaxBTagVal) { secondMaxBTagVal = maxBTagVal; }
                    maxBTagVal = bDiscriminatorValue;

                } else if(bDiscriminatorValue > secondMaxBTagVal){
                    secondMaxBTagVal = bDiscriminatorValue;
                    if(BJetTTHHMVAVect.size() >= 2){BJetTTHHMVAVect.pop_back();} 
                    BJetTTHHMVAVect.push_back( thejet );
                }
                
                JetBTagVal.push_back( bDiscriminatorValue );
                if( bDiscriminatorValue > bDiscriminator_[0] ) njets_btagloose++;
                if( bDiscriminatorValue > bDiscriminator_[1] ){
                    
                    njets_btagmedium++;
                    //JetVect.pop_back();
                    BJetVect.push_back( thejet );
                }
                if( bDiscriminatorValue > bDiscriminator_[2] ) njets_btagtight++;
            }

            if(useTTHHadronicMVA_){
                BJetVect.clear();
                BJetVect = BJetTTHHMVAVect;

                 if(njets_btagloose >= bjetsLooseNumberTTHHMVAThreshold_ && njets_btagmedium >= bjetsNumberTTHHMVAThreshold_ && jetcount >= jetsNumberTTHHMVAThreshold_ && _MVAMethod != ""){

                     tthMvaVal = TThMva_->EvaluateMVA( _MVAMethod.c_str() );
                     //                mvares_tth = TThMva_->EvaluateMVA( "BDT" );

                     /*
                    cout << "input variables : " << endl;
                    cout << "nJets = " << nJets << endl;
                    cout << "maxBTagVal = " << maxBTagVal << endl;
                    cout << "secondMaxBTagVal = " << secondMaxBTagVal << endl;
                    cout << "leadJetPt = " << leadJetPt << endl;

                    cout << "mva result :" << endl;
                    cout << "tthMvaVal = " << tthMvaVal  << endl;
                     */
                 }
            }

            bool isTTHHadronicTagged = false;
            
            if( !useTTHHadronicMVA_ && njets_btagloose >= bjetsLooseNumberThreshold_ && njets_btagmedium >= bjetsNumberThreshold_ && jetcount >= jetsNumberThreshold_ ) {
                
                isTTHHadronicTagged = true;
            
            } else if ( useTTHHadronicMVA_ && njets_btagloose >= bjetsLooseNumberTTHHMVAThreshold_ && njets_btagmedium >= bjetsNumberTTHHMVAThreshold_ && jetcount >= jetsNumberTTHHMVAThreshold_ && tthMvaVal > tthHadMVAThreshold_ ) {
                
                isTTHHadronicTagged = true;
            }

            if( isTTHHadronicTagged ) {
                TTHHadronicTag tthhtags_obj( dipho, mvares, JetVect, BJetVect );
                tthhtags_obj.setNjet( jetcount );
                tthhtags_obj.setNBLoose( njets_btagloose );
                tthhtags_obj.setNBMedium( njets_btagmedium );
                tthhtags_obj.setNBTight( njets_btagtight );
                tthhtags_obj.setDiPhotonIndex( diphoIndex );
                tthhtags_obj.setLeadJetPt( leadJetPt );
                tthhtags_obj.setSubLeadJetPt( subLeadJetPt );
                tthhtags_obj.setSumJetPt( sumJetPt );
                tthhtags_obj.setMaxBTagVal( maxBTagVal );
                tthhtags_obj.setSecondMaxBTagVal( secondMaxBTagVal );
                tthhtags_obj.setSystLabel( systLabel_ );
                tthhtags_obj.setMVAres(tthMvaVal);
                if(!useTTHHadronicMVA_){
                    for( unsigned num = 0; num < JetVect.size(); num++ ) {
                        tthhtags_obj.includeWeightsByLabel( *JetVect[num] , "JetBTagCutWeight");
                        //tthhtags_obj.includeWeightsByLabel( *JetVect[num] , "JetBTagReshapeWeight");
                    }
                } else {
                    for( unsigned num = 0; num < BJetTTHHMVAVect.size(); num++ ) {
                        tthhtags_obj.includeWeightsByLabel( *BJetTTHHMVAVect[num] , "JetBTagReshapeWeight");
                    }                    
                }
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

