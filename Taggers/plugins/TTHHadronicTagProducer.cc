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

#include "flashgg/Taggers/interface/LeptonSelection2018.h"
#include "flashgg/DataFormats/interface/Met.h"

#include "DataFormats/Math/interface/deltaR.h"

#include "DataFormats/Common/interface/RefToPtr.h"
#include "FWCore/Common/interface/TriggerNames.h"
#include "DataFormats/Common/interface/TriggerResults.h"

#include "flashgg/Taggers/interface/BDT_resolvedTopTagger.h"
#include "flashgg/Taggers/interface/TTH_DNN_Helper.h"

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
        TTHHadronicTagProducer( const ParameterSet & );
    private:
        void produce( Event &, const EventSetup & ) override;
        int  chooseCategory( float );
        int  computeStage1Kinematics( const TTHHadronicTag );
        int  chooseCategory_pt( float, float );

        std::vector<edm::EDGetTokenT<View<flashgg::Jet> > > tokenJets_;
        std::vector<std::vector<edm::EDGetTokenT<edm::View<flashgg::Jet>>>> jetTokens_;
        EDGetTokenT<View<DiPhotonCandidate> > diPhotonToken_;
        std::vector<edm::EDGetTokenT<edm::View<DiPhotonCandidate>>> diPhotonTokens_;
        std::vector<edm::EDGetTokenT<edm::View<DiPhotonMVAResult>>> mvaResultTokens_;
        std::vector<edm::InputTag> inputTagJets_;
        EDGetTokenT<View<Electron> > electronToken_;
        EDGetTokenT<View<flashgg::Muon> > muonToken_;
        EDGetTokenT<View<reco::Vertex> > vertexToken_;
        EDGetTokenT<View<DiPhotonMVAResult> > mvaResultToken_;
        EDGetTokenT<View<flashgg::Met> > METToken_;
        std::vector<edm::EDGetTokenT<edm::View<flashgg::Met>>> metTokens_;
        EDGetTokenT<double> rhoTag_;
        EDGetTokenT<edm::TriggerResults> triggerRECO_;
        string systLabel_;

        typedef std::vector<edm::Handle<edm::View<flashgg::Jet> > > JetCollectionVector;
        bool useTTHHadronicMVA_;
        bool applyMETfilters_;

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

        double MuonEtaCut_;
        double MuonPtCut_;
        double MuonIsoCut_;
        double MuonPhotonDrCut_;

        double ElePtCut_;
        std::vector<double> EleEtaCuts_;
        double ElePhotonDrCut_;
        double ElePhotonZMassCut_;
        double DeltaRTrkEle_;
        bool debug_;

        unique_ptr<TMVA::Reader>TThMva_;
        FileInPath tthMVAweightfile_;
        string _MVAMethod;
        FileInPath topTaggerXMLfile_;
        FileInPath tthVsDiphoDNNfile_;
        std::vector<double> tthVsDiphoDNN_global_mean_;
        std::vector<double> tthVsDiphoDNN_global_stddev_;
        std::vector<double> tthVsDiphoDNN_object_mean_;
        std::vector<double> tthVsDiphoDNN_object_stddev_;
        FileInPath tthVsttGGDNNfile_;
        std::vector<double> tthVsttGGDNN_global_mean_;
        std::vector<double> tthVsttGGDNN_global_stddev_;
        std::vector<double> tthVsttGGDNN_object_mean_;
        std::vector<double> tthVsttGGDNN_object_stddev_;
        unique_ptr<TMVA::Reader>TThMva_RunII_;
        FileInPath tthMVA_RunII_weightfile_;

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
        float pho1_sigmaEOverE_;
        float pho2_sigmaEOverE_;
        float pho1_scphi_;
        float pho2_scphi_;
        float pho1_hasPixelSeed_;
        float pho2_hasPixelSeed_;
        float diPhoY_;
        float diPhoPtoM_;
        float diPhoCosPhi_;
        float nbloose_;

        float btag_1_;
        float jetPt_1_;
        float jetEta_1_;
        float jetPhi_1_;
        float btag_2_;
        float jetPt_2_;
        float jetEta_2_;
        float jetPhi_2_;
        float btag_3_;
        float jetPt_3_;
        float jetEta_3_;
        float jetPhi_3_;
        float btag_4_;
        float jetPt_4_;
        float jetEta_4_;
        float jetPhi_4_;
      
        float MET_;
      
        float tthMvaVal_;
        float tthMvaVal_RunII_;
        
        float maxBTagVal_noBB_;
        float secondMaxBTagVal_noBB_;

        float pho1_eta_;
        float pho2_eta_;

        float diPhoDeltaR_;
        float btag_noBB_1_;
        float btag_noBB_2_;
        float btag_noBB_3_;
        float btag_noBB_4_;

        float ht_;
        float helicity_angle_;
        float top_tag_score_;

        float dnn_score_0_;
        float dnn_score_1_;

        vector<double> boundaries;

        vector<double> boundaries_pt1;
        vector<double> STXSPtBoundaries_pt1;

        vector<double> boundaries_pt2;
        vector<double> STXSPtBoundaries_pt2;

        vector<double> boundaries_pt3;
        vector<double> STXSPtBoundaries_pt3;

        vector<double> boundaries_pt4;
        vector<double> STXSPtBoundaries_pt4;

        BDT_resolvedTopTagger *topTagger;
        TTH_DNN_Helper* dnn_dipho;
        TTH_DNN_Helper* dnn_ttGG;

        bool modifySystematicsWorkflow;
        std::vector<std::string> systematicsLabels;

        std::string inputDiPhotonName_;
        std::vector<std::string> inputDiPhotonSuffixes_;

        std::string inputMVAResultName_;

        std::string nominalJetsName_;
        std::string systematicsJetsName_;
        std::vector<std::string> inputJetsSuffixes_;
        unsigned int inputJetsCollSize_;

        std::string nominalMetName_;
        std::string systematicsMetName_;
        std::vector<std::string> inputMetSuffixes_;

        bool useLargeMVAs;

    };

    TTHHadronicTagProducer::TTHHadronicTagProducer( const ParameterSet &iConfig ) :
        diPhotonToken_( consumes<View<flashgg::DiPhotonCandidate> >( iConfig.getParameter<InputTag> ( "DiPhotonTag" ) ) ),
        inputTagJets_( iConfig.getParameter<std::vector<edm::InputTag> >( "inputTagJets" ) ),
        electronToken_( consumes<View<flashgg::Electron> >( iConfig.getParameter<InputTag>( "ElectronTag" ) ) ),
        muonToken_( consumes<View<flashgg::Muon> >( iConfig.getParameter<InputTag>( "MuonTag" ) ) ),
        vertexToken_( consumes<View<reco::Vertex> >( iConfig.getParameter<InputTag> ( "VertexTag" ) ) ),
        mvaResultToken_( consumes<View<flashgg::DiPhotonMVAResult> >( iConfig.getParameter<InputTag>( "MVAResultTag" ) ) ),
        METToken_( consumes<View<flashgg::Met> >( iConfig.getParameter<InputTag> ( "METTag" ) ) ),
        rhoTag_( consumes<double>( iConfig.getParameter<InputTag>( "rhoTag" ) ) ),
	    triggerRECO_( consumes<edm::TriggerResults>(iConfig.getParameter<InputTag>("RECOfilters") ) ),
        systLabel_( iConfig.getParameter<string> ( "SystLabel" ) ),
        _MVAMethod( iConfig.getParameter<string> ( "MVAMethod" ) )
    {
        systematicsLabels.push_back("");
        modifySystematicsWorkflow = iConfig.getParameter<bool> ( "ModifySystematicsWorkflow" );

        useLargeMVAs = iConfig.getParameter<bool> ( "UseLargeMVAs" );

        // Get diphoton candidates corresponding to each systematic
        inputDiPhotonName_= iConfig.getParameter<std::string>( "DiPhotonName" );
        inputDiPhotonSuffixes_= iConfig.getParameter<std::vector<std::string> > ( "DiPhotonSuffixes" );
        std::vector<edm::InputTag>  diPhotonTags;

        inputMVAResultName_ = iConfig.getParameter<std::string>( "MVAResultName" );
        std::vector<edm::InputTag>  mvaResultTags;

        diPhotonTags.push_back(edm::InputTag(inputDiPhotonName_)); // nominal diphotons
        mvaResultTags.push_back(edm::InputTag(inputMVAResultName_)); // nominal mva results

        for (auto & suffix : inputDiPhotonSuffixes_) {
            systematicsLabels.push_back(suffix);
            std::string inputName = inputDiPhotonName_;
            inputName.append(suffix);
            diPhotonTags.push_back(edm::InputTag(inputName));

            std::string mvaName = inputMVAResultName_;
            mvaName.append(suffix);
            mvaResultTags.push_back(edm::InputTag(mvaName));
        }

        for (auto & tag : diPhotonTags)
            diPhotonTokens_.push_back(consumes<edm::View<flashgg::DiPhotonCandidate>>(tag));

        for (auto & tag : mvaResultTags)
            mvaResultTokens_.push_back(consumes<edm::View<flashgg::DiPhotonMVAResult>>(tag));

        // Get jet collections corresponding to each systematic
        nominalJetsName_ = iConfig.getParameter<std::string> ( "JetsName" );
        systematicsJetsName_ = iConfig.getParameter<std::string> ( "SystematicsJetsName" );
        inputJetsCollSize_= iConfig.getParameter<unsigned int> ( "JetsCollSize" );
        inputJetsSuffixes_= iConfig.getParameter<std::vector<std::string> > ( "JetsSuffixes" );
        std::vector<std::vector<edm::InputTag>>  jetTags;

        jetTags.push_back(std::vector<edm::InputTag>(0));
        for (unsigned int i = 0; i < inputJetsCollSize_ ; i++) {
            jetTags[0].push_back(inputTagJets_[i]);  // nominal jets
        }

        for (auto & suffix : inputJetsSuffixes_) {
            systematicsLabels.push_back(suffix);
            jetTags.push_back(std::vector<edm::InputTag>(0));
            int suffix_idx = jetTags.size() - 1;
            for (unsigned int i = 0; i < inputJetsCollSize_ ; i++) {
                std::string jetsName = systematicsJetsName_;
                jetsName.append(std::to_string(i));
                jetTags[suffix_idx].push_back(edm::InputTag(jetsName,suffix));
            }
        }

        for (unsigned int i = 0; i < jetTags.size(); i++) {
            jetTokens_.push_back(std::vector<edm::EDGetTokenT<edm::View<flashgg::Jet>>>(0));
            for (auto & tag : jetTags[i])
                jetTokens_[i].push_back(consumes<edm::View<flashgg::Jet>>(tag));
        }

        // Get MET corresponding to each systematic
        nominalMetName_ = iConfig.getParameter<std::string> ( "MetName" );
        systematicsMetName_ = iConfig.getParameter<std::string> ( "SystematicsMetName" );
        inputMetSuffixes_ = iConfig.getParameter<std::vector<std::string>> ( "MetSuffixes" );
        std::vector<edm::InputTag> metTags;
        metTags.push_back(edm::InputTag(nominalMetName_)); // nominal MET
        for (auto & suffix : inputMetSuffixes_) {
            systematicsLabels.push_back(suffix);
            metTags.push_back(edm::InputTag(systematicsMetName_, suffix));
        }

        for (auto & tag : metTags)
           metTokens_.push_back(consumes<edm::View<flashgg::Met>>(tag)); 

        boundaries = iConfig.getParameter<vector<double > >( "Boundaries" );
        boundaries_pt1 = iConfig.getParameter<vector<double > >( "Boundaries_pt1" );
        boundaries_pt2 = iConfig.getParameter<vector<double > >( "Boundaries_pt2" );
        boundaries_pt3 = iConfig.getParameter<vector<double > >( "Boundaries_pt3" );
        boundaries_pt4 = iConfig.getParameter<vector<double > >( "Boundaries_pt4" );
        STXSPtBoundaries_pt1 = iConfig.getParameter<vector<double > >( "STXSPtBoundaries_pt1" );
        STXSPtBoundaries_pt2 = iConfig.getParameter<vector<double > >( "STXSPtBoundaries_pt2" );
        STXSPtBoundaries_pt3 = iConfig.getParameter<vector<double > >( "STXSPtBoundaries_pt3" );
        STXSPtBoundaries_pt4 = iConfig.getParameter<vector<double > >( "STXSPtBoundaries_pt4" );

        assert( is_sorted( boundaries.begin(), boundaries.end() ) ); // 
        assert( is_sorted( boundaries_pt1.begin(), boundaries_pt1.end() ) ); // 
        assert( is_sorted( boundaries_pt2.begin(), boundaries_pt2.end() ) ); // 
        assert( is_sorted( boundaries_pt3.begin(), boundaries_pt3.end() ) ); // 
        assert( is_sorted( boundaries_pt4.begin(), boundaries_pt4.end() ) ); // 
        //assert( is_sorted( STXSPtBoundaries_pt1.begin(), STXSBoundaries_pt1.end() ) ); // 
        //assert( is_sorted( STXSPtBoundaries_pt2.begin(), STXSBoundaries_pt2.end() ) ); // 

        MVAThreshold_ = iConfig.getParameter<double>( "MVAThreshold");
        MVATTHHMVAThreshold_ = iConfig.getParameter<double>( "MVATTHHMVAThreshold");
        PhoMVAThreshold_ = iConfig.getParameter<double>( "PhoMVAThreshold");

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
        MuonEtaCut_ = iConfig.getParameter<double>( "MuonEtaCut");
        MuonPtCut_ = iConfig.getParameter<double>( "MuonPtCut");
        MuonIsoCut_ = iConfig.getParameter<double>( "MuonIsoCut");
        MuonPhotonDrCut_ = iConfig.getParameter<double>( "MuonPhotonDrCut");
 
        EleEtaCuts_ = iConfig.getParameter<std::vector<double>>( "EleEtaCuts");
        ElePtCut_ = iConfig.getParameter<double>( "ElePtCut");
        ElePhotonDrCut_ = iConfig.getParameter<double>( "ElePhotonDrCut");
        ElePhotonZMassCut_ = iConfig.getParameter<double>( "ElePhotonZMassCut");
        DeltaRTrkEle_ = iConfig.getParameter<double>( "DeltaRTrkEle");

        debug_ = iConfig.getParameter<bool>( "debug" );

        useTTHHadronicMVA_ = iConfig.getParameter<bool>( "useTTHHadronicMVA");
        applyMETfilters_   = iConfig.getParameter<bool>( "applyMETfilters");
        bjetsLooseNumberThreshold_ = iConfig.getParameter<int>( "bjetsLooseNumberThreshold");
        jetsNumberTTHHMVAThreshold_ = iConfig.getParameter<int>( "jetsNumberTTHHMVAThreshold");
        bjetsNumberTTHHMVAThreshold_ = iConfig.getParameter<int>( "bjetsNumberTTHHMVAThreshold");
        bjetsLooseNumberTTHHMVAThreshold_ = iConfig.getParameter<int>( "bjetsLooseNumberTTHHMVAThreshold");
        secondMaxBTagTTHHMVAThreshold_ = iConfig.getParameter<double>( "secondMaxBTagTTHHMVAThreshold");

        tthMVAweightfile_ = iConfig.getParameter<edm::FileInPath>( "tthMVAweightfile" ); 
        topTaggerXMLfile_ = iConfig.getParameter<edm::FileInPath>( "topTaggerXMLfile" );
        tthVsDiphoDNNfile_ = iConfig.getParameter<edm::FileInPath>( "tthVsDiphoDNNfile" );
        tthVsDiphoDNN_global_mean_ = iConfig.getParameter<std::vector<double>>( "tthVsDiphoDNN_global_mean" );
        tthVsDiphoDNN_global_stddev_ = iConfig.getParameter<std::vector<double>>( "tthVsDiphoDNN_global_stddev" );
        tthVsDiphoDNN_object_mean_ = iConfig.getParameter<std::vector<double>>( "tthVsDiphoDNN_object_mean" );
        tthVsDiphoDNN_object_stddev_ = iConfig.getParameter<std::vector<double>>( "tthVsDiphoDNN_object_stddev" );
        tthVsttGGDNNfile_ = iConfig.getParameter<edm::FileInPath>( "tthVsttGGDNNfile" );
        tthVsttGGDNN_global_mean_ = iConfig.getParameter<std::vector<double>>( "tthVsttGGDNN_global_mean" );
        tthVsttGGDNN_global_stddev_ = iConfig.getParameter<std::vector<double>>( "tthVsttGGDNN_global_stddev" );
        tthVsttGGDNN_object_mean_ = iConfig.getParameter<std::vector<double>>( "tthVsttGGDNN_object_mean" );
        tthVsttGGDNN_object_stddev_ = iConfig.getParameter<std::vector<double>>( "tthVsttGGDNN_object_stddev" );
        tthMVA_RunII_weightfile_ = iConfig.getParameter<edm::FileInPath>( "tthMVA_RunII_weightfile" );

        nJets_ = 0;
        leadJetPt_ = 0.;
        leadJetBTag_ = -1.;
        subLeadJetPt_ = 0.;
        sumJetPt_ = 0.;

        maxBTagVal_ = -3.;
        secondMaxBTagVal_ = -3.;
        thirdMaxBTagVal_ = -3.;
        fourthMaxBTagVal_ = -3.;

        mindRPhoLeadJet_ = -999;
        maxdRPhoLeadJet_= -999;

        minPhoID_= -999.;
        maxPhoID_= -999.;
        pho1_ptoM_= -999.;
        pho2_ptoM_= -999.;
        pho1_sceta_= -999.;
        pho2_sceta_= -999.;
        pho1_scphi_= -999.;
        pho2_scphi_= -999.;
        pho1_sigmaEOverE_= -999.;
        pho2_sigmaEOverE_= -999.;

        pho1_hasPixelSeed_=-999;
        pho2_hasPixelSeed_=-999;

        diPhoY_= -999.;
        diPhoPtoM_= -999.;
        diPhoCosPhi_= -999.;
        nbloose_=-999;

        btag_1_=-1;
        jetPt_1_=-1;
        jetEta_1_=-6;
        jetPhi_1_=-6;
        btag_2_=-1;
        jetPt_2_=-1;
        jetEta_2_=-6;
        jetPhi_2_=-6;
        btag_3_=-1;
        jetPt_3_=-1;
        jetEta_3_=-6;
        jetPhi_3_=-6;
        btag_4_=-1;
        jetPt_4_=-1;
        jetEta_4_=-6;
        jetPhi_4_=-6;
                
        MET_=-1;

        maxBTagVal_noBB_ = -3.;
        secondMaxBTagVal_noBB_ = -3.; 

        pho1_eta_ = -999.;
        pho2_eta_ = -999.;

        diPhoDeltaR_ = -999.;
        
        btag_noBB_1_ = -1;
        btag_noBB_2_ = -1;
        btag_noBB_3_ = -1;
        btag_noBB_4_ = -1;

        ht_ = 0.;
        helicity_angle_ = -999.;

        top_tag_score_ = -999.;
        dnn_score_0_ = -999.;
        dnn_score_1_ = -999.;

        if (_MVAMethod != ""){
            TThMva_.reset( new TMVA::Reader( "!Color:Silent" ) );

            TThMva_->AddVariable( "nJets", &nJets_);
            TThMva_->AddVariable( "sumJetPt", &sumJetPt_);
            TThMva_->AddVariable( "maxBTagVal",&maxBTagVal_);
            TThMva_->AddVariable( "secondMaxBTagVal", &secondMaxBTagVal_);
            TThMva_->AddVariable( "pho1_ptoM", &pho1_ptoM_);
            TThMva_->AddVariable( "pho2_ptoM", &pho2_ptoM_);
            TThMva_->AddVariable( "pho1_sceta", &pho1_sceta_);
            TThMva_->AddVariable( "pho2_sceta", &pho2_sceta_);
            TThMva_->AddVariable( "pho1_scphi", &pho1_scphi_);
            TThMva_->AddVariable( "pho2_scphi", &pho2_scphi_);
            TThMva_->AddVariable( "diPhoY", &diPhoY_);

            TThMva_->AddVariable( "minPhoID", &minPhoID_);
            TThMva_->AddVariable( "maxPhoID", &maxPhoID_);
            TThMva_->AddVariable( "diPhoPtoM", &diPhoPtoM_);  

            TThMva_->AddVariable( "btag_1", &btag_1_);       
            TThMva_->AddVariable( "btag_2", &btag_2_);     
            TThMva_->AddVariable( "btag_3", &btag_3_);   
            TThMva_->AddVariable( "btag_4", &btag_4_);    
            TThMva_->AddVariable( "jetPt_1", &jetPt_1_);      
            TThMva_->AddVariable( "jetPt_2", &jetPt_2_);     
            TThMva_->AddVariable( "jetPt_3", &jetPt_3_);     
            TThMva_->AddVariable( "jetPt_4", &jetPt_4_);   
            TThMva_->AddVariable( "jetEta_1", &jetEta_1_);   
            TThMva_->AddVariable( "jetEta_2", &jetEta_2_);   
            TThMva_->AddVariable( "jetEta_3", &jetEta_3_);   

            TThMva_->AddVariable( "jetEta_4", &jetEta_4_);       
            TThMva_->AddVariable( "pho1_hasPixelSeed",&pho1_hasPixelSeed_);
            TThMva_->AddVariable( "pho2_hasPixelSeed",&pho2_hasPixelSeed_);
            TThMva_->AddVariable( "thirdMaxBTagVal", &thirdMaxBTagVal_);          
            TThMva_->AddVariable( "MET",&MET_);

            TThMva_->BookMVA( _MVAMethod.c_str() , tthMVAweightfile_.fullPath() );

            TThMva_RunII_.reset( new TMVA::Reader( "!Color:Silent" ) );
     
            TThMva_RunII_->AddVariable("maxIDMVA_", &maxPhoID_);
            TThMva_RunII_->AddVariable("minIDMVA_", &minPhoID_);
            TThMva_RunII_->AddVariable("max2_btag_", &secondMaxBTagVal_noBB_);
            TThMva_RunII_->AddVariable("max1_btag_", &maxBTagVal_noBB_);
            TThMva_RunII_->AddVariable("dipho_delta_R", &diPhoDeltaR_);
            TThMva_RunII_->AddVariable("njets_", &nJets_);
            TThMva_RunII_->AddVariable("ht_", &ht_);
            TThMva_RunII_->AddVariable("leadptoM_", &pho1_ptoM_);
            TThMva_RunII_->AddVariable("subleadptoM_", &pho2_ptoM_);
            TThMva_RunII_->AddVariable("lead_eta_", &pho1_eta_);
            TThMva_RunII_->AddVariable("sublead_eta_", &pho2_eta_);
     
            TThMva_RunII_->AddVariable("jet1_pt_", &jetPt_1_);
            TThMva_RunII_->AddVariable("jet1_eta_", &jetEta_1_);
            TThMva_RunII_->AddVariable("jet1_btag_", &btag_noBB_1_);
            TThMva_RunII_->AddVariable("jet2_pt_", &jetPt_2_);
            TThMva_RunII_->AddVariable("jet2_eta_", &jetEta_2_);
            TThMva_RunII_->AddVariable("jet2_btag_", &btag_noBB_2_);
            TThMva_RunII_->AddVariable("jet3_pt_", &jetPt_3_);
            TThMva_RunII_->AddVariable("jet3_eta_", &jetEta_3_);
            TThMva_RunII_->AddVariable("jet3_btag_", &btag_noBB_3_);
            TThMva_RunII_->AddVariable("jet4_pt_", &jetPt_4_);
            TThMva_RunII_->AddVariable("jet4_eta_", &jetEta_4_);
            TThMva_RunII_->AddVariable("jet4_btag_", &btag_noBB_4_);
     
            TThMva_RunII_->AddVariable("leadPSV_", &pho1_hasPixelSeed_);
            TThMva_RunII_->AddVariable("subleadPSV_", &pho2_hasPixelSeed_);
     
            TThMva_RunII_->AddVariable("dipho_cosphi_", &diPhoCosPhi_);
            TThMva_RunII_->AddVariable("dipho_rapidity_", &diPhoY_);
            TThMva_RunII_->AddVariable("met_", &MET_);
     
            TThMva_RunII_->AddVariable("dipho_pt_over_mass_", &diPhoPtoM_);
     
            TThMva_RunII_->AddVariable("helicity_angle_", &helicity_angle_);
            TThMva_RunII_->AddVariable("top_tag_score_", &top_tag_score_);
            TThMva_RunII_->AddVariable("dnn_score_0", &dnn_score_0_);
            TThMva_RunII_->AddVariable("dnn_score_1", &dnn_score_1_);

            TThMva_RunII_->BookMVA(_MVAMethod.c_str(), tthMVA_RunII_weightfile_.fullPath()); 
        
        }       

        if (useLargeMVAs) {
            topTagger = new BDT_resolvedTopTagger(topTaggerXMLfile_.fullPath());

            dnn_dipho = new TTH_DNN_Helper(tthVsDiphoDNNfile_.fullPath());
            dnn_ttGG  = new TTH_DNN_Helper(tthVsttGGDNNfile_.fullPath());

            dnn_dipho->SetInputShapes(18, 8, 8);
            dnn_ttGG->SetInputShapes(18, 8, 8);

            dnn_dipho->SetPreprocessingSchemes(tthVsDiphoDNN_global_mean_, tthVsDiphoDNN_global_stddev_, tthVsDiphoDNN_object_mean_, tthVsDiphoDNN_object_stddev_);
            dnn_ttGG->SetPreprocessingSchemes(tthVsttGGDNN_global_mean_, tthVsttGGDNN_global_stddev_, tthVsttGGDNN_object_mean_, tthVsttGGDNN_object_stddev_);
        }

        for (unsigned i = 0 ; i < inputTagJets_.size() ; i++) {
            auto token = consumes<View<flashgg::Jet> >(inputTagJets_[i]);
            tokenJets_.push_back(token);
        }

        if (modifySystematicsWorkflow) {
            for (auto & syst_name : systematicsLabels)
                produces<vector<TTHHadronicTag>>(syst_name);
        }

        else {
            produces<vector<TTHHadronicTag> >();
        }
    }

    int TTHHadronicTagProducer::chooseCategory_pt( float tthmvavalue, float pT)
    {
        // should return 0 if mva above all the numbers, 1 if below the first, ..., boundaries.size()-N if below the Nth, ...
        if (pT > STXSPtBoundaries_pt1[0] && pT < STXSPtBoundaries_pt1[1]) {
            for(int n = 0 ; n < ( int )boundaries_pt1.size() ; n++ ) {
                if( ( double )tthmvavalue > boundaries_pt1[boundaries_pt1.size() - n - 1] ) {
                    //cout << "pT range: [" << STXSPtBoundaries_pt1[0] << ", " << STXSPtBoundaries_pt1[1] << "], Hadronic cat " << n << endl; 
                    return n; 
                }
            }
        }

        if (pT > STXSPtBoundaries_pt2[0] && pT < STXSPtBoundaries_pt2[1]) {
            for(int n = 0 ; n < ( int )boundaries_pt2.size() ; n++ ) {
                if( ( double )tthmvavalue > boundaries_pt2[boundaries_pt2.size() - n - 1] ) {
                    //cout << "pT range: [" << STXSPtBoundaries_pt2[0] << ", " << STXSPtBoundaries_pt2[1] << "], Hadronic cat " << n + boundaries_pt1.size() << endl; 
                    return n + boundaries_pt1.size(); 
                }
            }
        }

        if (pT > STXSPtBoundaries_pt3[0] && pT < STXSPtBoundaries_pt3[1]) {
            for(int n = 0 ; n < ( int )boundaries_pt3.size() ; n++ ) {
                if( ( double )tthmvavalue > boundaries_pt3[boundaries_pt3.size() - n - 1] ) {
                    //cout << "pT range: [" << STXSPtBoundaries_pt3[0] << ", " << STXSPtBoundaries_pt3[1] << "], Hadronic cat " << n + boundaries_pt1.size() << endl; 
                    return n + boundaries_pt1.size() + boundaries_pt2.size(); 
                }
            }
        }

        if (pT > STXSPtBoundaries_pt4[0] && pT < STXSPtBoundaries_pt4[1]) {
            for(int n = 0 ; n < ( int )boundaries_pt4.size() ; n++ ) {
                if( ( double )tthmvavalue > boundaries_pt4[boundaries_pt4.size() - n - 1] ) {
                    //cout << "pT range: [" << STXSPtBoundaries_pt4[0] << ", " << STXSPtBoundaries_pt4[1] << "], Hadronic cat " << n + boundaries_pt1.size() << endl; 
                    return n + boundaries_pt1.size() + boundaries_pt2.size() + boundaries_pt3.size();  
                }
            }
        }

        return -1; // Does not pass, object will not be produced
    }

    int TTHHadronicTagProducer::chooseCategory( float tthmvavalue )
    {
        // should return 0 if mva above all the numbers, 1 if below the first, ..., boundaries.size()-N if below the Nth, ...
        int n;
        for( n = 0 ; n < ( int )boundaries.size() ; n++ ) {
            //if( ( double )tthmvavalue > boundaries[boundaries.size() - n - 1] ) { return n; }
            if( ( double )tthmvavalue > boundaries[boundaries.size() - n - 1] ) { cout << "Hadronic cat " << n << endl; return n; }
        }
        return -1; // Does not pass, object will not be produced
    }

    void TTHHadronicTagProducer::produce( Event &evt, const EventSetup & )
    {

        //Handle<View<flashgg::Jet> > theJets;
        //evt.getByToken( thejetToken_, theJets );
        // const PtrVector<flashgg::Jet>& jetPointers = theJets->ptrVector();
        JetCollectionVector Jets(inputJetsCollSize_);
        if (!modifySystematicsWorkflow) {
            for( size_t j = 0; j < inputTagJets_.size(); ++j ) {
                evt.getByToken( tokenJets_[j], Jets[j] );
            }
        }

        Handle<View<flashgg::DiPhotonCandidate> > diPhotons;
        if (!modifySystematicsWorkflow)
            evt.getByToken( diPhotonToken_, diPhotons );

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
        if (!modifySystematicsWorkflow)
            evt.getByToken( mvaResultToken_, mvaResults );
        // const PtrVector<flashgg::DiPhotonMVAResult>& mvaResultPointers = mvaResults->ptrVector();

        Handle<View<flashgg::Met> > METs;
        if (!modifySystematicsWorkflow)
            evt.getByToken( METToken_, METs );


	    //Get trigger results relevant to MET filters
        bool passMETfilters = 1;
        edm::Handle<edm::TriggerResults> triggerBits;
        evt.getByToken( triggerRECO_, triggerBits );
        const edm::TriggerNames &triggerNames = evt.triggerNames( *triggerBits );
        std::vector<std::string> flagList = {"Flag_goodVertices", "Flag_globalTightHalo2016Filter", "Flag_HBHENoiseFilter", "Flag_HBHENoieIsoFilter", "Flag_EcalDeadCellTriggerPrimitiveFilter", "Flag_BadPFMuonFilter", "Flag_BadChargedCandidateFilter","Flag_eeBadScFilter", "Flag_ecalBadCalibFilter"};
        if( ! evt.isRealData() ) {
          flagList = {"Flag_goodVertices", "Flag_globalTightHalo2016Filter", "Flag_HBHENoiseFilter", "Flag_HBHENoiseIsoFilter", "Flag_EcalDeadCellTriggerPrimitiveFilter", "Flag_BadPFMuonFilter", "Flag_BadChargedCandidateFilter", "Flag_ecalBadCalibFilter"};
        }
        for( unsigned int i = 0; i < triggerNames.triggerNames().size(); i++ )
      {
	    if(!triggerBits->accept(i)) {
	      for(size_t j=0;j<flagList.size();j++)
		{
		  if(flagList[j]==triggerNames.triggerName(i))
		    {
		      passMETfilters=0;
		      break;
		    }
		}
	    }
	  }
	
    

        //std::unique_ptr<vector<TTHHadronicTag> > tthhtags( new vector<TTHHadronicTag> );
        
        // Here we loop over all systematics (DiPhoton, Jets, Met), with the following logic:
        // idx = 0:                                                                     Nominal DiPhoton | Nominal Jets | Nominal Met
        // 0 < idx <= # DiPho Systs:                                                    SystVar DiPhoton | Nominal Jets | Nominal Met
        // # DiPho Systs < idx <= # DiPho Systs + Jet Systs:                            Nominal DiPhoton | SystVar Jets | Nominal Met
        // # Jet Systs + # DiPho Systs < idx <= # DiPho Systs + Jet Systs + Met Systs:  Nominal DiPhoton | Nominal Jets | SystVar Met
        if (!modifySystematicsWorkflow)
            systematicsLabels = {""};

        //cout << "Looping over " << systematicsLabels.size() << "systematics variations" << endl;
        //cout << inputDiPhotonSuffixes_.size() << "for diphotons, " << inputJetsSuffixes_.size() << " for jets, and " << inputMetSuffixes_.size() << " for met" << endl;
        for (unsigned int syst_idx = 0; syst_idx < systematicsLabels.size(); syst_idx++) {

            bool vary_dipho = ((syst_idx > 0) && (syst_idx <= inputDiPhotonSuffixes_.size()));
            bool vary_jets  = ((syst_idx > inputDiPhotonSuffixes_.size()) && (syst_idx <= (inputJetsSuffixes_.size() + inputDiPhotonSuffixes_.size())));
            bool vary_met   = (syst_idx > (inputJetsSuffixes_.size() + inputDiPhotonSuffixes_.size()));

            /*
            cout << "Syst idx: " << syst_idx << ", " << systematicsLabels[syst_idx] << endl;
            std::string vary_dipho_s = vary_dipho ? "No" : "Yes";
            std::string vary_jets_s = vary_jets ? "No" : "Yes";
            std::string vary_met_s = vary_met ? "No" : "Yes";
            cout << "Taking nominal dipho: " << vary_dipho_s << endl;
            cout << "Taking nominal jets:  " << vary_jets_s << endl;
            cout << "Taking nominal met:   " << vary_met_s << endl;
            if (vary_dipho)
                cout << "Choosing this diphoton systematic: " << inputDiPhotonSuffixes_[syst_idx-1] << endl;
            if (vary_jets)
                cout << "Choosing this jet systematic: " << inputJetsSuffixes_[syst_idx - (1 + inputDiPhotonSuffixes_.size())] << endl;
            if (vary_met)
                cout << "Choosing this met systematic: " << inputMetSuffixes_[syst_idx - (1 + inputJetsSuffixes_.size() + inputDiPhotonSuffixes_.size())] << endl; 
            */

            // Select appropriate diphotons
            //Handle<View<flashgg::DiPhotonCandidate> > diPhotons;
            if (modifySystematicsWorkflow) {
                if (vary_dipho) {
                    evt.getByToken(diPhotonTokens_[syst_idx], diPhotons);
                    evt.getByToken(mvaResultTokens_[syst_idx], mvaResults);
                }
                else {
                    evt.getByToken(diPhotonTokens_[0], diPhotons);
                    evt.getByToken(mvaResultTokens_[0], mvaResults);
                }
                // Select appropriate MET
                //Handle<View<flashgg::Met> > METs;
                if (vary_met) {
                    int met_syst_idx = syst_idx - (inputJetsSuffixes_.size() + inputDiPhotonSuffixes_.size());
                    evt.getByToken(metTokens_[met_syst_idx], METs);
                }
                else
                    evt.getByToken(metTokens_[0], METs);
            }

            std::unique_ptr<vector<TTHHadronicTag> > tthhtags( new vector<TTHHadronicTag> );

            assert( diPhotons->size() == mvaResults->size() );

            for(unsigned int diphoIndex = 0; diphoIndex < diPhotons->size(); diphoIndex++ ) {

                edm::Ptr<flashgg::DiPhotonCandidate> dipho = diPhotons->ptrAt( diphoIndex );

                if(!passMETfilters && applyMETfilters_) continue;

                std::vector<edm::Ptr<flashgg::Muon> >     Muons;
                std::vector<edm::Ptr<flashgg::Electron> > Electrons;

                if(theMuons->size()>0)
                    Muons = selectMuons(theMuons->ptrs(), dipho, vertices->ptrs(), MuonPtCut_, MuonEtaCut_, MuonIsoCut_, MuonPhotonDrCut_, debug_);
                if(theElectrons->size()>0)
                    Electrons = selectElectrons(theElectrons->ptrs(), dipho, ElePtCut_, EleEtaCuts_, ElePhotonDrCut_, ElePhotonZMassCut_, DeltaRTrkEle_, debug_);

                if( (Muons.size() + Electrons.size()) != 0) continue;

                jetcount_ = 0;

                nJets_ = 0;
                njets_btagloose_ = 0;
                njets_btagmedium_ = 0;
                njets_btagtight_ = 0;

                idmva1_ = -999.;
                idmva2_ = -999.;

                leadJetPt_ = 0.;
                leadJetBTag_ = -1.;
                subLeadJetPt_ = 0.;
                sumJetPt_ = 0.;

                maxBTagVal_ = -3.;
                secondMaxBTagVal_ = -3.;
                thirdMaxBTagVal_ = -3.;
                fourthMaxBTagVal_ = -3.;
            
                mindRPhoLeadJet_ = -999;
                maxdRPhoLeadJet_= -999;
                
                minPhoID_= -999.;
                maxPhoID_= -999.;
                pho1_ptoM_= -999.;
                pho2_ptoM_= -999.;
                pho1_sceta_= -999.;
                pho2_sceta_= -999.;
                pho1_scphi_= -999.;
                pho2_scphi_= -999.;
                pho1_hasPixelSeed_=-999;
                pho2_hasPixelSeed_=-999;

                pho1_sigmaEOverE_= -999.;
                pho2_sigmaEOverE_= -999.;
                diPhoY_= -999.;
                diPhoPtoM_= -999.;
                diPhoCosPhi_= -999.;
                nbloose_=-999;

                btag_1_=-999;
                jetPt_1_=-999;
                jetEta_1_=-999;
                jetPhi_1_=-999;
                btag_2_=-999;
                jetPt_2_=-999;
                jetEta_2_=-999;
                jetPhi_2_=-999;
                btag_3_=-999;
                jetPt_3_=-999;
                jetEta_3_=-999;
                jetPhi_3_=-999;
                btag_4_=-999;
                jetPt_4_=-999;
                jetEta_4_=-999;
                jetPhi_4_=-999;
                        
                MET_=-1;
            
                tthMvaVal_ = -999.;

                maxBTagVal_noBB_ = -3.;
                secondMaxBTagVal_noBB_ = -3.;

                diPhoDeltaR_ = -999.;

                ht_ = 0.;
                helicity_angle_ = -999.;
                top_tag_score_ = -999.;
                dnn_score_0_ = -999.;
                dnn_score_1_ = -999.;
                tthMvaVal_RunII_ = -999.;

                unsigned int jetCollectionIndex = diPhotons->ptrAt( diphoIndex )->jetCollectionIndex();

                std::vector<edm::Ptr<flashgg::Jet> > JetVect;
                JetVect.clear();
                std::vector<edm::Ptr<flashgg::Jet> > BJetVect;
                BJetVect.clear();
                std::vector<edm::Ptr<flashgg::Jet> > BJetTTHHMVAVect;
                BJetTTHHMVAVect.clear();
                
                std::vector<float> JetBTagVal;
                JetBTagVal.clear();
            
                idmva1_ = dipho->leadingPhoton()->phoIdMvaDWrtVtx( dipho->vtx() );
                idmva2_ = dipho->subLeadingPhoton()->phoIdMvaDWrtVtx( dipho->vtx() );

                if( idmva1_ <= PhoMVAThreshold_ || idmva2_ <= PhoMVAThreshold_ ) { continue; }

                edm::Ptr<flashgg::DiPhotonMVAResult> mvares = mvaResults->ptrAt( diphoIndex );
            
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

                //JetCollectionVector Jets(inputJetsCollSize_);
                if (modifySystematicsWorkflow) {
                    int jet_syst_idx;
                    if (vary_jets)
                        jet_syst_idx = syst_idx - (inputDiPhotonSuffixes_.size());
                    else
                        jet_syst_idx = 0;

                    for (unsigned int i = 0; i < inputJetsCollSize_; i++)
                        evt.getByToken(jetTokens_[jet_syst_idx][i], Jets[i]);
                }


                for( unsigned int jetIndex = 0; jetIndex < Jets[jetCollectionIndex]->size() ; jetIndex++ ) {
                    edm::Ptr<flashgg::Jet> thejet = Jets[jetCollectionIndex]->ptrAt( jetIndex );
                    if( fabs( thejet->eta() ) > jetEtaThreshold_ ) { continue; }
                    if(!thejet->passesJetID  ( flashgg::Tight2017 ) ) { continue; }
                    float dRPhoLeadJet = deltaR( thejet->eta(), thejet->phi(), dipho->leadingPhoton()->superCluster()->eta(), dipho->leadingPhoton()->superCluster()->phi() ) ;
                    float dRPhoSubLeadJet = deltaR( thejet->eta(), thejet->phi(), dipho->subLeadingPhoton()->superCluster()->eta(),
                                                    dipho->subLeadingPhoton()->superCluster()->phi() );

                    if( dRPhoLeadJet < dRJetPhoLeadCut_ || dRPhoSubLeadJet < dRJetPhoSubleadCut_ ) { continue; }
                    if( thejet->pt() < jetPtThreshold_ ) { continue; }

                    jetcount_++;
                    nJets_ = jetcount_;
                    JetVect.push_back( thejet );

                    ht_ += thejet->pt();
                    
                    float bDiscriminatorValue = -2.;
                    if(bTag_ == "pfDeepCSV") bDiscriminatorValue = thejet->bDiscriminator("pfDeepCSVJetTags:probb")+thejet->bDiscriminator("pfDeepCSVJetTags:probbb") ;
                    else  bDiscriminatorValue = thejet->bDiscriminator( bTag_ );

                    float bDiscriminatorValue_noBB = -2;
                    if(bTag_ == "pfDeepCSV") bDiscriminatorValue_noBB = thejet->bDiscriminator("pfDeepCSVJetTags:probb");
                    else  bDiscriminatorValue_noBB = thejet->bDiscriminator( bTag_ );
             
                    if (useLargeMVAs) {
                      float cvsl = thejet->bDiscriminator("pfDeepCSVJetTags:probc") + thejet->bDiscriminator("pfDeepCSVJetTags:probudsg") ;
                      float cvsb = thejet->bDiscriminator("pfDeepCSVJetTags:probc") + thejet->bDiscriminator("pfDeepCSVJetTags:probb")+thejet->bDiscriminator("pfDeepCSVJetTags:probbb") ;
                      float ptD = thejet->userFloat("ptD") ;
                      float axis1 = thejet->userFloat("axis1") ;
                      int mult = thejet->userFloat("totalMult") ;
             
                      topTagger->addJet(thejet->pt(), thejet->eta(), thejet->phi(), thejet->mass(), bDiscriminatorValue, cvsl, cvsb, ptD, axis1, mult);           
                    }                


                    float jetPt = thejet->pt();
                    if(jetPt > leadJetPt_){
                        if(leadJetPt_ > subLeadJetPt_) { subLeadJetPt_ = leadJetPt_; }
                        leadJetPt_ = jetPt;
                        leadJetBTag_=  bDiscriminatorValue;
                    } else if(jetPt > subLeadJetPt_){
                        subLeadJetPt_ = jetPt;
                    }
                    sumJetPt_ += jetPt;
                    
                    if(bDiscriminatorValue_noBB > maxBTagVal_noBB_){
                         if(maxBTagVal_noBB_ > secondMaxBTagVal_noBB_) { secondMaxBTagVal_noBB_ = maxBTagVal_noBB_; }
                         maxBTagVal_noBB_ = bDiscriminatorValue_noBB;
     
                    } else if(bDiscriminatorValue_noBB > secondMaxBTagVal_noBB_){
                         secondMaxBTagVal_noBB_ = bDiscriminatorValue_noBB;
                    }
                    
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

                vector<float> mvaEval; 
                if (useLargeMVAs) {
                    mvaEval = topTagger->EvalMVA();
                    topTagger->clear();
                }

                if( METs->size() != 1 ) { std::cout << "WARNING - #MET is not 1" << std::endl;}
                Ptr<flashgg::Met> theMET = METs->ptrAt( 0 );
                
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
                    pho1_scphi_= dipho->leadingPhoton()->superCluster()->phi();
                    pho2_scphi_= dipho->subLeadingPhoton()->superCluster()->phi();

                    pho1_hasPixelSeed_= dipho->leadingPhoton()->hasPixelSeed();
                    pho2_hasPixelSeed_= dipho->subLeadingPhoton()->hasPixelSeed();

                    pho1_sigmaEOverE_= dipho->leadingPhoton()->sigEOverE();
                    pho2_sigmaEOverE_= dipho->subLeadingPhoton()->sigEOverE();

                    diPhoY_= dipho->rapidity();
                    diPhoPtoM_= dipho->pt()/dipho->mass();
                    diPhoCosPhi_=  abs(TMath::Cos( deltaPhi( dipho->leadingPhoton()->phi(), dipho->subLeadingPhoton()->phi() ) ));
                    nbloose_=float(njets_btagloose_);
                    MET_ = theMET->getCorPt();

                    pho1_eta_= dipho->leadingPhoton()->eta();
                    pho2_eta_= dipho->subLeadingPhoton()->eta();

                    diPhoDeltaR_ = deltaR( dipho->leadingPhoton()->eta(),dipho->leadingPhoton()->phi(), dipho->subLeadingPhoton()->eta(),dipho->subLeadingPhoton()->phi());


                    if(JetVect.size()>0){
                        if(bTag_ == "pfDeepCSV") btag_1_=JetVect[0]->bDiscriminator("pfDeepCSVJetTags:probb")+JetVect[0]->bDiscriminator("pfDeepCSVJetTags:probbb") ;
                        else  btag_1_ = JetVect[0]->bDiscriminator( bTag_ );
                        if(bTag_ == "pfDeepCSV") btag_noBB_1_=JetVect[0]->bDiscriminator("pfDeepCSVJetTags:probb");
                        else  btag_noBB_1_ = JetVect[0]->bDiscriminator( bTag_ );
                        jetPt_1_=JetVect[0]->pt();
                        jetEta_1_=JetVect[0]->eta();
                        jetPhi_1_=JetVect[0]->phi();
                    }

                    if(JetVect.size()>1){
                        if(bTag_ == "pfDeepCSV") btag_2_=JetVect[1]->bDiscriminator("pfDeepCSVJetTags:probb")+JetVect[1]->bDiscriminator("pfDeepCSVJetTags:probbb") ;
                        else  btag_2_ = JetVect[1]->bDiscriminator( bTag_ );
                        if(bTag_ == "pfDeepCSV") btag_noBB_2_=JetVect[1]->bDiscriminator("pfDeepCSVJetTags:probb");
                        else  btag_noBB_2_ = JetVect[1]->bDiscriminator( bTag_ );
                        jetPt_2_=JetVect[1]->pt();
                        jetEta_2_=JetVect[1]->eta();
                        jetPhi_2_=JetVect[1]->phi();
                    }

                    if(JetVect.size()>2){
                        if(bTag_ == "pfDeepCSV") btag_3_=JetVect[2]->bDiscriminator("pfDeepCSVJetTags:probb")+JetVect[2]->bDiscriminator("pfDeepCSVJetTags:probbb") ;
                        else  btag_3_ = JetVect[2]->bDiscriminator( bTag_ );
                        if(bTag_ == "pfDeepCSV") btag_noBB_3_=JetVect[2]->bDiscriminator("pfDeepCSVJetTags:probb");
                        else  btag_noBB_3_ = JetVect[2]->bDiscriminator( bTag_ );
                        jetPt_3_=JetVect[2]->pt();
                        jetEta_3_=JetVect[2]->eta();
                        jetPhi_3_=JetVect[2]->phi();
                    }
                    if(JetVect.size()>3){
                        if(bTag_ == "pfDeepCSV") btag_4_=JetVect[3]->bDiscriminator("pfDeepCSVJetTags:probb")+JetVect[3]->bDiscriminator("pfDeepCSVJetTags:probbb") ;
                        else  btag_4_ = JetVect[3]->bDiscriminator( bTag_ );
                        if(bTag_ == "pfDeepCSV") btag_noBB_4_=JetVect[3]->bDiscriminator("pfDeepCSVJetTags:probb");
                        else  btag_noBB_4_ = JetVect[3]->bDiscriminator( bTag_ );	    
                        jetPt_4_=JetVect[3]->pt();
                        jetEta_4_=JetVect[3]->eta();
                        jetPhi_4_=JetVect[3]->phi();
                    }


                    if(secondMaxBTagVal_ >= secondMaxBTagTTHHMVAThreshold_ && njets_btagloose_ >= bjetsLooseNumberTTHHMVAThreshold_ && njets_btagmedium_ >= bjetsNumberTTHHMVAThreshold_ && jetcount_ >= jetsNumberTTHHMVAThreshold_ && _MVAMethod != ""){
                        
                      if(debug_){
                        cout << "TTHHadronicTag -- input MVA variables : " << endl;
                        cout << "----------------------------------------" << endl;

                        cout << "nJets_ = " << nJets_ <<" jetcount"<< jetcount_<< endl;
                        cout << "sumJetPt_ = " << sumJetPt_ << endl;                    
                        cout << "maxBTagVal_ = " << maxBTagVal_ << endl;
                        cout << "secondMaxBTagVal_ = " << secondMaxBTagVal_ << endl;
                        cout << "thirdMaxBTagVal_ = " << thirdMaxBTagVal_ << endl;
                        //                    cout << "leadJetPt_ = " << leadJetPt_ << endl;
                        //                    cout << "leadJetBTag_ = " << leadJetBTag_ << endl;

                        cout << "minPhoID_ = " << minPhoID_ << endl;
                        cout << "maxPhoID_ = " << maxPhoID_ << endl;
                        //                    cout << "mindRPhoLeadJet_ = " << mindRPhoLeadJet_ << endl;
                        //                    cout << "maxdRPhoLeadJet_ = " << maxdRPhoLeadJet_ << endl;
                        cout << "pho1_ptoM_ = " << pho1_ptoM_ << endl;
                        cout << "pho2_ptoM_ = " << pho2_ptoM_ << endl;
                        cout << "pho1_sceta_ = " << pho1_sceta_ << endl;
                        cout << "pho2_sceta_ = " << pho2_sceta_ << endl;
                        cout << "pho1_scphi_ = " << pho1_scphi_ << endl;
                        cout << "pho2_scphi_ = " << pho2_scphi_ << endl;

                        cout << "pho1_hasPixelSeed_ = " << pho1_hasPixelSeed_ << endl;
                        cout << "pho2_hasPixelSeed_ = " << pho2_hasPixelSeed_ << endl;
                //                    cout << "pho1_sigmaEoE_ = " << pho1_sigmaEOverE_ << endl;
                //                    cout << "pho2_sigmaEoE_ = " << pho2_sigmaEOverE_ << endl;

                        cout << "diPhoY_ = " << diPhoY_ << endl;
                        //cout << "diPhoCosPhi_ = " << diPhoCosPhi_ << endl;
                        cout << "diPhoPtoM_ = " << diPhoPtoM_ << endl;

                //                    cout << "nBLoose_ = " << njets_btagloose_ << " "<<nbloose_<<endl;
                        cout << "btag_1_ = " <<btag_1_ << endl;
                        cout << "jetPt_1_ = " <<jetPt_1_ << endl;
                        cout << "jetEta_1_ = " <<jetEta_1_ << endl;
                //                    cout << "jetPhi_1_ = " <<jetPhi_1_ << endl;
                        cout << "btag_2_ = " <<btag_2_ << endl;
                        cout << "jetPt_2_ = " <<jetPt_2_ << endl;
                        cout << "jetEta_2_ = " <<jetEta_2_ << endl;
                //                    cout << "jetPhi_2_ = " <<jetPhi_2_ << endl;
                        cout << "btag_3_ = " <<btag_3_ << endl;
                        cout << "jetPt_3_ = " <<jetPt_3_ << endl;
                        cout << "jetEta_3_ = " <<jetEta_3_ << endl;
                //                    cout << "jetPhi_3_ = " <<jetPhi_3_ << endl;
                        cout << "btag_4_ = " <<btag_4_ << endl;
                        cout << "jetPt_4_ = " <<jetPt_4_ << endl;
                        cout << "jetEta_4_ = " <<jetEta_4_ << endl;
                //                    cout << "jetPhi_4_ = " <<jetPhi_4_ << endl;
                        cout << "MET_ = " <<MET_ << endl;
                        cout << "---------------------------------------" << endl;
                      }
                      tthMvaVal_ = TThMva_->EvaluateMVA( _MVAMethod.c_str() );

                        //cout << "mva result :" << endl;
                      if(debug_)  cout << " TTHHadronicTag -- output MVA value = " << tthMvaVal_  << endl;
                        //cout << "tthMvaVal_ = " << tthMvaVal_  << " "<< boundaries[0]<<" "<< boundaries[1]<< endl;
                         
                     }
                }

                if(useTTHHadronicMVA_) {
                  std::vector<double> global_features;
                  global_features.resize(18);
                  global_features[0] = dipho->leadingPhoton()->eta();
                  global_features[1] = dipho->subLeadingPhoton()->eta();
                  global_features[2] = dipho->leadingPhoton()->phi();
                  global_features[3] = dipho->subLeadingPhoton()->phi();
                  global_features[4] = pho1_ptoM_;
                  global_features[5] = pho2_ptoM_;
                  global_features[6] = maxPhoID_;
                  global_features[7] = minPhoID_;
                  global_features[8] = log((float)theMET->pt());
                  global_features[9] = (float)theMET->phi();
                  global_features[10] = pho1_hasPixelSeed_;
                  global_features[11] = pho2_hasPixelSeed_;
                  global_features[12] = diPhoY_;
                  global_features[13] = diPhoPtoM_;
                  global_features[14] = diPhoDeltaR_;
                  global_features[15] = maxBTagVal_noBB_;
                  global_features[16] = secondMaxBTagVal_noBB_;
                  global_features[17] = nJets_;

                  float dnn_score_dipho(0.5), dnn_score_ttGG(0.5);
                  if (useLargeMVAs) {
                      dnn_dipho->SetInputs(JetVect, global_features);
                      dnn_score_dipho = dnn_dipho->EvaluateDNN();

                      dnn_ttGG->SetInputs(JetVect, global_features);
                      dnn_score_ttGG = dnn_ttGG->EvaluateDNN();
                  }


                  TLorentzVector pho1, pho2;
                  pho1.SetPtEtaPhiE(dipho->leadingPhoton()->pt(), dipho->leadingPhoton()->eta(), dipho->leadingPhoton()->phi(), dipho->leadingPhoton()->energy());
                  pho2.SetPtEtaPhiE(dipho->subLeadingPhoton()->pt(), dipho->subLeadingPhoton()->eta(), dipho->subLeadingPhoton()->phi(), dipho->subLeadingPhoton()->energy());
                  helicity_angle_ = helicity(pho1, pho2);

                  top_tag_score_ = mvaEval.size() > 0 ? (mvaEval[0] != - 99 ? mvaEval[0] : -1) : - 1;
                  dnn_score_0_ = dnn_score_dipho;
                  dnn_score_1_ = dnn_score_ttGG;

                  tthMvaVal_RunII_ = convert_tmva_to_prob(TThMva_RunII_->EvaluateMVA( _MVAMethod.c_str() ));
                  if (debug_) {
                    cout << "TTH Hadronic Tag -- input MVA variables for Run II MVA: " << endl;
                    cout << "--------------------------------------------------------" << endl;
                    cout << "maxIDMVA_: " << maxPhoID_ << endl;
                    cout << "minIDMVA_: " << minPhoID_ << endl;
                    cout << "max1_btag_: " << maxBTagVal_noBB_ << endl;
                    cout << "max2_btag_: " << secondMaxBTagVal_noBB_ << endl;
                    cout << "dipho_delta_R_: " << diPhoDeltaR_ << endl;

                    cout << "njets_: " << nJets_ << endl;
                    cout << "ht_: " << ht_ << endl;
                    cout << "leadptoM_: " << pho1_ptoM_ << endl;
                    cout << "subleadptoM_: " << pho2_ptoM_ << endl;
                    cout << "lead_eta_: " << pho1_eta_ << endl;
                    cout << "sublead_eta_: " << pho2_eta_ << endl;

                    cout << "jet1_pt_: " << jetPt_1_ << endl;
                    cout << "jet1_eta_: " << jetEta_1_ << endl;
                    cout << "jet1_btag_: " << btag_noBB_1_ << endl;
                    cout << "jet2_pt_: " << jetPt_2_ << endl;
                    cout << "jet2_eta_: " << jetEta_2_ << endl;
                    cout << "jet2_btag_: " << btag_noBB_2_ << endl;
                    cout << "jet3_pt_: " << jetPt_3_ << endl;
                    cout << "jet3_eta_: " << jetEta_3_ << endl;
                    cout << "jet3_btag_: " << btag_noBB_3_ << endl;
                    cout << "jet4_pt_: " << jetPt_4_ << endl;
                    cout << "jet4_eta_: " << jetEta_4_ << endl;
                    cout << "jet4_btag_: " << btag_noBB_4_ << endl;

                    cout << "leadPSV_: " << pho1_hasPixelSeed_ << endl;
                    cout << "subleadPSV_: " << pho2_hasPixelSeed_ << endl;

                    cout << "dipho_cosphi_: " << diPhoCosPhi_ << endl;
                    cout << "dipho_rapidity_: " << diPhoY_ << endl;
                    cout << "met_: " << MET_ << endl;
                    cout << "dipho_pt_over_mass_: " << diPhoPtoM_ << endl;
                    cout << "helicity_angle_: " << helicity_angle_ << endl;
                    cout << "top_tag_score_: " << top_tag_score_ << endl;

                    cout << "DNN Score 0: " << dnn_score_0_ << endl;
                    cout << "DNN Score 1: " << dnn_score_1_ << endl;
                    cout << endl;
                    cout << "BDT Score: " << tthMvaVal_RunII_ << endl;
                  }

                  global_features.clear();

                }

                tthMvaVal_ = tthMvaVal_RunII_; // use Run II MVA

                bool isTTHHadronicTagged = false;
                //int catnum =-1;
                int catnum_pt =-1;
                if( !useTTHHadronicMVA_ && njets_btagloose_ >= bjetsLooseNumberThreshold_ && njets_btagmedium_ >= bjetsNumberThreshold_ && jetcount_ >= jetsNumberThreshold_ ) {

                    catnum_pt=0;
                    isTTHHadronicTagged = true;
                    
                } else if ( useTTHHadronicMVA_  && njets_btagloose_ >= bjetsLooseNumberTTHHMVAThreshold_ && njets_btagmedium_ >= bjetsNumberTTHHMVAThreshold_ && jetcount_ >= jetsNumberTTHHMVAThreshold_ ) {
                    //&& tthMvaVal_ >= tthHadMVAThresholdMin_  && tthMvaVal_ < tthHadMVAThresholdMax_ ) 
                    
                    catnum_pt = chooseCategory_pt( tthMvaVal_, dipho->pt() );                
                    //catnum = chooseCategory( tthMvaVal_ );                
                    //                cout<<" catNum="<<catnum<<endl;
                    if(catnum_pt>=0){
                    //if(catnum>=0){
                        isTTHHadronicTagged = true;
                        //                    cout<<" TAGGED "<< endl;
                    }
                }
                
                if( isTTHHadronicTagged ) {

                    TTHHadronicTag tthhtags_obj( dipho, mvares, JetVect, BJetVect );
                    tthhtags_obj.setCategoryNumber(catnum_pt  );
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
                    std::string syst_label = modifySystematicsWorkflow ? systematicsLabels[syst_idx] : systLabel_;
                    tthhtags_obj.setSystLabel( syst_label ); 
                    tthhtags_obj.setMVAres(tthMvaVal_);
                    tthhtags_obj.setMET( theMET );

                    int chosenTag = computeStage1Kinematics( tthhtags_obj );
                    tthhtags_obj.setStage1recoTag( chosenTag );

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

                    tthhtags->push_back( tthhtags_obj );
                }
            }
        evt.put( std::move( tthhtags ), systematicsLabels[syst_idx] );
        }
    }

    int TTHHadronicTagProducer::computeStage1Kinematics( const TTHHadronicTag tag_obj )
    {
        int chosenTag_ = DiPhotonTagBase::stage1recoTag::LOGICERROR;
        int catNum = tag_obj.categoryNumber();
        if ( catNum == 0 ) {
            chosenTag_ = DiPhotonTagBase::stage1recoTag::RECO_TTH_HAD_PTH_0_60_Tag0;
        }
        else if ( catNum == 1 ) {
            chosenTag_ = DiPhotonTagBase::stage1recoTag::RECO_TTH_HAD_PTH_0_60_Tag1;
        }
        else if ( catNum == 2 ) {
            chosenTag_ = DiPhotonTagBase::stage1recoTag::RECO_TTH_HAD_PTH_0_60_Tag2;
        }
        else if ( catNum == 3 ) {
            chosenTag_ = DiPhotonTagBase::stage1recoTag::RECO_TTH_HAD_PTH_0_60_Tag3;
        }
        else if ( catNum == 4 ) {
            chosenTag_ = DiPhotonTagBase::stage1recoTag::RECO_TTH_HAD_PTH_60_120_Tag0;
        }
        else if ( catNum == 5 ) {
            chosenTag_ = DiPhotonTagBase::stage1recoTag::RECO_TTH_HAD_PTH_60_120_Tag1;
        }
        else if ( catNum == 6 ) {
            chosenTag_ = DiPhotonTagBase::stage1recoTag::RECO_TTH_HAD_PTH_60_120_Tag2;
        }
        else if ( catNum == 7 ) {
            chosenTag_ = DiPhotonTagBase::stage1recoTag::RECO_TTH_HAD_PTH_60_120_Tag3;
        }
        else if ( catNum == 8 ) {
            chosenTag_ = DiPhotonTagBase::stage1recoTag::RECO_TTH_HAD_PTH_120_200_Tag0;
        }
        else if ( catNum == 9 ) {
            chosenTag_ = DiPhotonTagBase::stage1recoTag::RECO_TTH_HAD_PTH_120_200_Tag1;
        }
        else if ( catNum == 10 ) {
            chosenTag_ = DiPhotonTagBase::stage1recoTag::RECO_TTH_HAD_PTH_120_200_Tag2;
        }
        else if ( catNum == 11 ) {
            chosenTag_ = DiPhotonTagBase::stage1recoTag::RECO_TTH_HAD_PTH_120_200_Tag3;
        }
        else if ( catNum == 12 ) {
            chosenTag_ = DiPhotonTagBase::stage1recoTag::RECO_TTH_HAD_PTH_GT200_Tag0;
        }
        else if ( catNum == 13 ) {
            chosenTag_ = DiPhotonTagBase::stage1recoTag::RECO_TTH_HAD_PTH_GT200_Tag1;
        }
        else if ( catNum == 14 ) {
            chosenTag_ = DiPhotonTagBase::stage1recoTag::RECO_TTH_HAD_PTH_GT200_Tag2;
        }
        else if ( catNum == 15 ) {
            chosenTag_ = DiPhotonTagBase::stage1recoTag::RECO_TTH_HAD_PTH_GT200_Tag3;
        }
        return chosenTag_;
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
