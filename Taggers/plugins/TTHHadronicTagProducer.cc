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

#include "TMath.h"
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
        int  chooseCategory( float );

        std::vector<edm::EDGetTokenT<View<flashgg::Jet> > > tokenJets_;
        EDGetTokenT<View<DiPhotonCandidate> > diPhotonToken_;
        std::vector<edm::InputTag> inputTagJets_;
        EDGetTokenT<View<Electron> > electronToken_;
        EDGetTokenT<View<flashgg::Muon> > muonToken_;
        EDGetTokenT<View<reco::Vertex> > vertexToken_;
        EDGetTokenT<View<DiPhotonMVAResult> > mvaResultToken_;
        EDGetTokenT<View<reco::GenParticle> > genParticleToken_;
        EDGetTokenT<int> stage0catToken_, stage1catToken_, njetsToken_;
        EDGetTokenT<float> pTHToken_,pTVToken_;
        EDGetTokenT<double> rhoTag_;
        string systLabel_;


        typedef std::vector<edm::Handle<edm::View<flashgg::Jet> > > JetCollectionVector;
        bool useTTHHadronicMVA_;
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
        double secondMaxBTagTTHHMVAThreshold_;
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

        int jetcount_;
        float nJets_;
        int njets_btagloose_;
        int njets_btagmedium_;
        int njets_btagtight_;
        double idmva1_;
        double idmva2_;
        float leadJetPt_;
        float leadJetBTag_;
        float subLeadJetPt_;
        float sumJetPt_;
        float maxBTagVal_;
        float secondMaxBTagVal_;
        float thirdMaxBTagVal_;
        float fourthMaxBTagVal_;

        float mindRPhoLeadJet_;
        float maxdRPhoLeadJet_;

        float minPhoID_;
        float maxPhoID_;
        float pho1_ptoM_;
        float pho2_ptoM_;
        float pho1_sceta_;
        float pho2_sceta_;
        float diPhoY_;
        float diPhoPtoM_;
        float diPhoCosPhi_;
        float nbloose_;
        
        float tthMvaVal_;

        vector<double> boundaries;

    };

    TTHHadronicTagProducer::TTHHadronicTagProducer( const ParameterSet &iConfig ) :
        diPhotonToken_( consumes<View<flashgg::DiPhotonCandidate> >( iConfig.getParameter<InputTag> ( "DiPhotonTag" ) ) ),
        inputTagJets_( iConfig.getParameter<std::vector<edm::InputTag> >( "inputTagJets" ) ),
        electronToken_( consumes<View<flashgg::Electron> >( iConfig.getParameter<InputTag>( "ElectronTag" ) ) ),
        muonToken_( consumes<View<flashgg::Muon> >( iConfig.getParameter<InputTag>( "MuonTag" ) ) ),
        vertexToken_( consumes<View<reco::Vertex> >( iConfig.getParameter<InputTag> ( "VertexTag" ) ) ),
        mvaResultToken_( consumes<View<flashgg::DiPhotonMVAResult> >( iConfig.getParameter<InputTag>( "MVAResultTag" ) ) ),
        genParticleToken_( consumes<View<reco::GenParticle> >( iConfig.getParameter<InputTag> ( "GenParticleTag" ) ) ),
        rhoTag_( consumes<double>( iConfig.getParameter<InputTag>( "rhoTag" ) ) ),
        systLabel_( iConfig.getParameter<string> ( "SystLabel" ) ),
        _MVAMethod( iConfig.getParameter<string> ( "MVAMethod" ) )
    {
        boundaries = iConfig.getParameter<vector<double > >( "Boundaries" );
        assert( is_sorted( boundaries.begin(), boundaries.end() ) ); // 

        ParameterSet HTXSps = iConfig.getParameterSet( "HTXSTags" );
        stage0catToken_ = consumes<int>( HTXSps.getParameter<InputTag>("stage0cat") );
        stage1catToken_ = consumes<int>( HTXSps.getParameter<InputTag>("stage1cat") );
        njetsToken_ = consumes<int>( HTXSps.getParameter<InputTag>("njets") );
        pTHToken_ = consumes<float>( HTXSps.getParameter<InputTag>("pTH") );
        pTVToken_ = consumes<float>( HTXSps.getParameter<InputTag>("pTV") );

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
        bjetsLooseNumberThreshold_ = iConfig.getParameter<int>( "bjetsLooseNumberThreshold");
        jetsNumberTTHHMVAThreshold_ = iConfig.getParameter<int>( "jetsNumberTTHHMVAThreshold");
        bjetsNumberTTHHMVAThreshold_ = iConfig.getParameter<int>( "bjetsNumberTTHHMVAThreshold");
        bjetsLooseNumberTTHHMVAThreshold_ = iConfig.getParameter<int>( "bjetsLooseNumberTTHHMVAThreshold");
        secondMaxBTagTTHHMVAThreshold_ = iConfig.getParameter<double>( "secondMaxBTagTTHHMVAThreshold");

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

        nJets_ = 0;
        leadJetPt_ = 0.;
        leadJetBTag_ = -999.;
        subLeadJetPt_ = 0.;
        sumJetPt_ = 0.;

        maxBTagVal_ = -999.;
        secondMaxBTagVal_ = -999.;
        thirdMaxBTagVal_ = -999.;
        fourthMaxBTagVal_ = -999.;
        mindRPhoLeadJet_ = -999;
        maxdRPhoLeadJet_= -999;

        minPhoID_= -999.;
        maxPhoID_= -999.;
        pho1_ptoM_= -999.;
        pho2_ptoM_= -999.;
        pho1_sceta_= -999.;
        pho2_sceta_= -999.;
        diPhoY_= -999.;
        diPhoPtoM_= -999.;
        diPhoCosPhi_= -999.;
        nbloose_=-999;

        if (_MVAMethod != ""){
            TThMva_.reset( new TMVA::Reader( "!Color:Silent" ) );

            TThMva_->AddVariable( "leadJetPt", &leadJetPt_);
            TThMva_->AddVariable( "nJets", &nJets_);
            TThMva_->AddVariable( "sumJetPt", &sumJetPt_);
            TThMva_->AddVariable( "maxBTagVal",&maxBTagVal_);
            TThMva_->AddVariable( "secondMaxBTagVal", &secondMaxBTagVal_);
            TThMva_->AddVariable( "pho1_ptoM", &pho1_ptoM_);
            TThMva_->AddVariable( "pho2_ptoM", &pho2_ptoM_);
            TThMva_->AddVariable( "pho1_sceta", &pho1_sceta_);
            TThMva_->AddVariable( "pho2_sceta", &pho2_sceta_);
            TThMva_->AddVariable( "diPhoY", &diPhoY_);
            TThMva_->AddVariable( "diPhoCosPhi", &diPhoCosPhi_);      
            TThMva_->AddVariable( "minPhoID", &minPhoID_);
            TThMva_->AddVariable( "maxPhoID", &maxPhoID_);
            TThMva_->AddVariable( "diPhoPtoM", &diPhoPtoM_);     
            TThMva_->AddVariable( "mindRPhoLeadJet", &mindRPhoLeadJet_);
            TThMva_->AddVariable( "maxdRPhoLeadJet", &maxdRPhoLeadJet_);
            TThMva_->AddVariable( "nJetsBl", &nbloose_);          
            //TThMva_->AddVariable( "leadJetBTag", &leadJetBTag_);         
            //TThMva_->AddVariable( "thirdMaxBTagVal", &thirdMaxBTagVal_);          


            TThMva_->BookMVA( _MVAMethod.c_str() , tthMVAweightfile_.fullPath() );
        
        }       

        for (unsigned i = 0 ; i < inputTagJets_.size() ; i++) {
            auto token = consumes<View<flashgg::Jet> >(inputTagJets_[i]);
            tokenJets_.push_back(token);
        }

        produces<vector<TTHHadronicTag> >();
        produces<vector<TagTruthBase> >();
    }

    int TTHHadronicTagProducer::chooseCategory( float tthmvavalue )
    {
        // should return 0 if mva above all the numbers, 1 if below the first, ..., boundaries.size()-N if below the Nth, ...
        int n;
        for( n = 0 ; n < ( int )boundaries.size() ; n++ ) {
            if( ( double )tthmvavalue > boundaries[boundaries.size() - n - 1] ) { return n; }
        }
        return -1; // Does not pass, object will not be produced
    }

    void TTHHadronicTagProducer::produce( Event &evt, const EventSetup & )
    {

        Handle<int> stage0cat, stage1cat, njets;
        Handle<float> pTH, pTV;
        evt.getByToken(stage0catToken_, stage0cat);
        evt.getByToken(stage1catToken_,stage1cat);
        evt.getByToken(njetsToken_,njets);
        evt.getByToken(pTHToken_,pTH);
        evt.getByToken(pTVToken_,pTV);

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

        edm::Handle<double>  rho;
        evt.getByToken(rhoTag_,rho);
        double rho_    = *rho;

        Handle<View<reco::Vertex> > vertices;
        evt.getByToken( vertexToken_, vertices );

        Handle<View<flashgg::DiPhotonMVAResult> > mvaResults;
        evt.getByToken( mvaResultToken_, mvaResults );
        // const PtrVector<flashgg::DiPhotonMVAResult>& mvaResultPointers = mvaResults->ptrVector();

        Handle<View<reco::GenParticle> > genParticles;

        std::unique_ptr<vector<TTHHadronicTag> > tthhtags( new vector<TTHHadronicTag> );
        std::unique_ptr<vector<TagTruthBase> > truths( new vector<TagTruthBase> );

        Point higgsVtx;
        if( ! evt.isRealData() ) {
            evt.getByToken( genParticleToken_, genParticles );
            /*
            for( unsigned int genLoop = 0 ; genLoop < genParticles->size(); genLoop++ ) {
                int pdgid = genParticles->ptrAt( genLoop )->pdgId();
                if( pdgid == 25 ){
                    cout << "Higgs found " <<endl;
                    if ( genParticles->ptrAt( genLoop )->numberOfDaughters() == 2 ){ 
                        const reco::Candidate * d1 =  genParticles->ptrAt( genLoop )->daughter( 0 );
                        const reco::Candidate * d2 =  genParticles->ptrAt( genLoop )->daughter( 1 );
                        cout << "Higgs with status = " <<  genParticles->ptrAt( genLoop )->status() << " has two daughters with pdgId: " << endl;
                        cout << "d1 pdgId = " << d1->pdgId() << "   d2 pdgId = "<< d2->pdgId() <<endl;
                    }
                }
            }
            */
                //if (d1->pdgId()!=22 || d2->pdgId()!=22) continue;
                
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

        goodElectrons = selectStdAllElectrons(theElectrons->ptrs(), vertices->ptrs(), leptonPtThreshold_, electronEtaThresholds_,
                                              useElectronMVARecipe_, useElectronLooseID_,
                                              rho_, evt.isRealData() );
        
        
        for( unsigned int diphoIndex = 0; diphoIndex < diPhotons->size(); diphoIndex++ ) {

            if( goodElectrons.size() > 0 ||  goodMuons.size() > 0 )  continue; 

            jetcount_ = 0;
            nJets_ = 0;

            njets_btagloose_ = 0;
            njets_btagmedium_ = 0;
            njets_btagtight_ = 0;
            idmva1_ = -999.;
            idmva2_ = -999.;
            leadJetPt_ = 0.;
            leadJetBTag_ = -999.;
            subLeadJetPt_ = 0.;
            sumJetPt_ = 0.;
            maxBTagVal_ = -999.;
            secondMaxBTagVal_ = -999.;
            thirdMaxBTagVal_ = -999.;
            fourthMaxBTagVal_ = -999.;
            tthMvaVal_ = -999.;

            unsigned int jetCollectionIndex = diPhotons->ptrAt( diphoIndex )->jetCollectionIndex();

            std::vector<edm::Ptr<flashgg::Jet> > JetVect;
            JetVect.clear();
            std::vector<edm::Ptr<flashgg::Jet> > BJetVect;
            BJetVect.clear();
            std::vector<edm::Ptr<flashgg::Jet> > BJetTTHHMVAVect;
            BJetTTHHMVAVect.clear();
            
            std::vector<edm::Ptr<reco::GenJet> > genJetVect;
            genJetVect.clear();

            std::vector<float> JetBTagVal;
            JetBTagVal.clear();

            edm::Ptr<flashgg::DiPhotonCandidate> dipho = diPhotons->ptrAt( diphoIndex );
        
            idmva1_ = dipho->leadingPhoton()->phoIdMvaDWrtVtx( dipho->vtx() );
            idmva2_ = dipho->subLeadingPhoton()->phoIdMvaDWrtVtx( dipho->vtx() );

            if( idmva1_ <= PhoMVAThreshold_ || idmva2_ <= PhoMVAThreshold_ ) { continue; }

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
                if(!thejet->passesJetID  ( flashgg::Loose ) ) { continue; }
                float dRPhoLeadJet = deltaR( thejet->eta(), thejet->phi(), dipho->leadingPhoton()->superCluster()->eta(), dipho->leadingPhoton()->superCluster()->phi() ) ;
                float dRPhoSubLeadJet = deltaR( thejet->eta(), thejet->phi(), dipho->subLeadingPhoton()->superCluster()->eta(),
                                                dipho->subLeadingPhoton()->superCluster()->phi() );

                if( dRPhoLeadJet < dRJetPhoLeadCut_ || dRPhoSubLeadJet < dRJetPhoSubleadCut_ ) { continue; }
                if( thejet->pt() < jetPtThreshold_ ) { continue; }

                jetcount_++;
                nJets_ = jetcount_;
                JetVect.push_back( thejet );
                
                //genJetVect.push_back( thejet->genJet());
                //cout<<"TTH Jet "<< jetcount_<<" Pt:"<<thejet->pt()<<" genPt:"<<thejet->genJet()->pt()<<" hflav: "<<thejet->hadronFlavour()<<" pFlav:"<<thejet->partonFlavour()<< endl;

                float bDiscriminatorValue = -2.;
                if(bTag_ == "pfDeepCSV") bDiscriminatorValue = thejet->bDiscriminator("pfDeepCSVJetTags:probb")+thejet->bDiscriminator("pfDeepCSVJetTags:probbb") ;
                else  bDiscriminatorValue = thejet->bDiscriminator( bTag_ );

                

                float jetPt = thejet->pt();
                if(jetPt > leadJetPt_){
                    if(leadJetPt_ > subLeadJetPt_) { subLeadJetPt_ = leadJetPt_; }
                    leadJetPt_ = jetPt;
                    leadJetBTag_=  bDiscriminatorValue;
                } else if(jetPt > subLeadJetPt_){
                    subLeadJetPt_ = jetPt;
                }
                sumJetPt_ += jetPt;
                
                
                if(bDiscriminatorValue > maxBTagVal_){ 

                    BJetTTHHMVAVect.insert( BJetTTHHMVAVect.begin(), thejet );
                    if(BJetTTHHMVAVect.size() >= 3){ BJetTTHHMVAVect.pop_back(); }
                    
                    if(thirdMaxBTagVal_>fourthMaxBTagVal_) { fourthMaxBTagVal_= thirdMaxBTagVal_;}
                    if(secondMaxBTagVal_>thirdMaxBTagVal_){ thirdMaxBTagVal_= secondMaxBTagVal_; }
                    if(maxBTagVal_ > secondMaxBTagVal_) { secondMaxBTagVal_ = maxBTagVal_; }

                    maxBTagVal_ = bDiscriminatorValue;

                } else if(bDiscriminatorValue > secondMaxBTagVal_){

                    if(BJetTTHHMVAVect.size() >= 2){BJetTTHHMVAVect.pop_back();} 
                    BJetTTHHMVAVect.push_back( thejet );

                    if(thirdMaxBTagVal_>fourthMaxBTagVal_) { fourthMaxBTagVal_= thirdMaxBTagVal_;}
                    if(secondMaxBTagVal_>thirdMaxBTagVal_) { thirdMaxBTagVal_= secondMaxBTagVal_;}
                    secondMaxBTagVal_ = bDiscriminatorValue;

                }else if(bDiscriminatorValue > thirdMaxBTagVal_){

                    if(thirdMaxBTagVal_>fourthMaxBTagVal_) { fourthMaxBTagVal_= thirdMaxBTagVal_;}
                    thirdMaxBTagVal_ = bDiscriminatorValue;

                }else if(bDiscriminatorValue > fourthMaxBTagVal_){
                   fourthMaxBTagVal_ = bDiscriminatorValue;
                }

                JetBTagVal.push_back( bDiscriminatorValue );

                if( bDiscriminatorValue > bDiscriminator_[0] ) njets_btagloose_++;
                if( bDiscriminatorValue > bDiscriminator_[1] ){
                    
                    njets_btagmedium_++;
                    BJetVect.push_back( thejet );
                }
                if( bDiscriminatorValue > bDiscriminator_[2] ) njets_btagtight_++;
            }
        
            if(useTTHHadronicMVA_){

                BJetVect.clear();
                BJetVect = BJetTTHHMVAVect;

                if(JetVect.size()>1) mindRPhoLeadJet_=TMath::Min(deltaR( dipho->leadingPhoton()->eta(),dipho->leadingPhoton()->phi(), JetVect[0]->eta(),JetVect[0]->phi()) ,
                                                                deltaR( dipho->leadingPhoton()->eta(),dipho->leadingPhoton()->phi(), JetVect[1]->eta(),JetVect[1]->phi()));
                if(JetVect.size()>1) maxdRPhoLeadJet_=TMath::Max(deltaR( dipho->leadingPhoton()->eta(),dipho->leadingPhoton()->phi(), JetVect[0]->eta(),JetVect[0]->phi()) ,
                                                                deltaR( dipho->leadingPhoton()->eta(),dipho->leadingPhoton()->phi(), JetVect[1]->eta(),JetVect[1]->phi()));


                minPhoID_=TMath::Min( idmva1_, idmva2_);
                maxPhoID_=TMath::Max( idmva1_, idmva2_);
                pho1_ptoM_= dipho->leadingPhoton()->pt()/dipho->mass();
                pho2_ptoM_= dipho->subLeadingPhoton()->pt()/dipho->mass();

                pho1_sceta_= dipho->leadingPhoton()->superCluster()->eta();
                pho2_sceta_= dipho->subLeadingPhoton()->superCluster()->eta();

                diPhoY_= dipho->rapidity();
                diPhoPtoM_= dipho->pt()/dipho->mass();
                diPhoCosPhi_=  TMath::Cos( deltaPhi( dipho->leadingPhoton()->phi(), dipho->subLeadingPhoton()->phi() ) );
                nbloose_=float(njets_btagloose_);

                if(secondMaxBTagVal_ >= secondMaxBTagTTHHMVAThreshold_ && njets_btagloose_ >= bjetsLooseNumberTTHHMVAThreshold_ && njets_btagmedium_ >= bjetsNumberTTHHMVAThreshold_ && jetcount_ >= jetsNumberTTHHMVAThreshold_ && _MVAMethod != ""){
                    
                    /*
                    cout << "input variables : " << endl;
                    cout << "nJets_ = " << nJets_ <<" jetcount"<< jetcount_<< endl;
                    cout << "maxBTagVal_ = " << maxBTagVal_ << endl;
                    cout << "secondMaxBTagVal_ = " << secondMaxBTagVal_ << endl;
                    cout << "thirdMaxBTagVal_ = " << thirdMaxBTagVal_ << endl;
                    cout << "leadJetPt_ = " << leadJetPt_ << endl;
                    cout << "leadJetBTag_ = " << leadJetBTag_ << endl;
                    cout << "sumJetPt_ = " << sumJetPt_ << endl;
                    cout << "minPhoID_ = " << minPhoID_ << endl;
                    cout << "maxPhoID_ = " << maxPhoID_ << endl;
                    cout << "mindRPhoLeadJet_ = " << mindRPhoLeadJet_ << endl;
                    cout << "maxdRPhoLeadJet_ = " << maxdRPhoLeadJet_ << endl;
                    cout << "pho1_ptoM_ = " << pho1_ptoM_ << endl;
                    cout << "pho2_ptoM_ = " << pho2_ptoM_ << endl;
                    cout << "pho1_sceta_ = " << pho1_sceta_ << endl;
                    cout << "pho2_sceta_ = " << pho2_sceta_ << endl;
                    cout << "diPhoY_ = " << diPhoY_ << endl;
                    cout << "diPhoCosPhi_ = " << diPhoCosPhi_ << endl;
                    cout << "diPhoPtoM_ = " << diPhoPtoM_ << endl;
                    cout << "nBLoose_ = " << njets_btagloose_ << " "<<nbloose_<<endl;
                    */
                    tthMvaVal_ = TThMva_->EvaluateMVA( _MVAMethod.c_str() );

                    //cout << "mva result :" << endl;
                    //cout << "tthMvaVal_ = " << tthMvaVal_  << " "<< boundaries[0]<<" "<< boundaries[1]<< endl;
                     
                     
                 }
            }

            bool isTTHHadronicTagged = false;
            int catnum =-1;
            if( !useTTHHadronicMVA_ && njets_btagloose_ >= bjetsLooseNumberThreshold_ && njets_btagmedium_ >= bjetsNumberThreshold_ && jetcount_ >= jetsNumberThreshold_ ) {

                catnum=0;
                isTTHHadronicTagged = true;
                
            } else if ( useTTHHadronicMVA_  && njets_btagloose_ >= bjetsLooseNumberTTHHMVAThreshold_ && njets_btagmedium_ >= bjetsNumberTTHHMVAThreshold_ && jetcount_ >= jetsNumberTTHHMVAThreshold_ ) {
                //&& tthMvaVal_ >= tthHadMVAThresholdMin_  && tthMvaVal_ < tthHadMVAThresholdMax_ ) {
                
                catnum = chooseCategory( tthMvaVal_ );                
                //                cout<<" catNum="<<catnum<<endl;
                if(catnum>=0){
                    isTTHHadronicTagged = true;
                    //                    cout<<" TAGGED "<< endl;
                }
            }
            
            if( isTTHHadronicTagged ) {

                TTHHadronicTag tthhtags_obj( dipho, mvares, JetVect, BJetVect );
                tthhtags_obj.setCategoryNumber(catnum  );
                tthhtags_obj.setNjet( jetcount_ );
                tthhtags_obj.setNBLoose( njets_btagloose_ );
                tthhtags_obj.setNBMedium( njets_btagmedium_ );
                tthhtags_obj.setNBTight( njets_btagtight_ );
                tthhtags_obj.setDiPhotonIndex( diphoIndex );
                tthhtags_obj.setLeadJetPt( leadJetPt_ );
                tthhtags_obj.setSubLeadJetPt( subLeadJetPt_ );
                tthhtags_obj.setSumJetPt( sumJetPt_ );
                tthhtags_obj.setMaxBTagVal( maxBTagVal_ );
                tthhtags_obj.setSecondMaxBTagVal( secondMaxBTagVal_ );
                tthhtags_obj.setThirdMaxBTagVal( thirdMaxBTagVal_ );
                tthhtags_obj.setFourthMaxBTagVal( fourthMaxBTagVal_ );
                tthhtags_obj.setSystLabel( systLabel_ );
                tthhtags_obj.setMVAres(tthMvaVal_);

                if(!useTTHHadronicMVA_){
                    for( unsigned num = 0; num < JetVect.size(); num++ ) {
                        tthhtags_obj.includeWeightsByLabel( *JetVect[num] , "JetBTagCutWeight");
                        //tthhtags_obj.includeWeightsByLabel( *JetVect[num] , "JetBTagReshapeWeight");
                    }
                } else {
                    for( unsigned num = 0; num < JetVect.size(); num++ ) {
                        tthhtags_obj.includeWeightsByLabel( *JetVect[num] , "JetBTagReshapeWeight");
                    }                    
                }
                tthhtags_obj.includeWeights( *dipho );
                
                //                cout<<" BEFORE TO PUSH BACK TAG "<< endl;

                if(tthhtags_obj.categoryNumber() >= 0)    
                    { 
                        tthhtags->push_back( tthhtags_obj );
                        if( ! evt.isRealData() ) {
                            TagTruthBase truth_obj;
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
                            //truth_obj.setGenJets(genJetVect);
                            truths->push_back( truth_obj );
                            tthhtags->back().setTagTruth( edm::refToPtr( edm::Ref<vector<TagTruthBase> >( rTagTruth, idx++ ) ) );
                        }
                    }
                // count++;
            }
        }
        evt.put( std::move( tthhtags ) );
        evt.put( std::move( truths ) );
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

