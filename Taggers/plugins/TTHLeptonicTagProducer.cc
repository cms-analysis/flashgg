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
#include "flashgg/DataFormats/interface/Met.h"
#include "flashgg/DataFormats/interface/Photon.h"
#include "DataFormats/VertexReco/interface/Vertex.h"

#include "DataFormats/TrackReco/interface/HitPattern.h"
#include "flashgg/Taggers/interface/LeptonSelection2018.h"

#include "DataFormats/Math/interface/deltaR.h"

#include "DataFormats/Common/interface/RefToPtr.h"

#include "flashgg/Taggers/interface/BDT_resolvedTopTagger.h"
#include "flashgg/Taggers/interface/TTH_DNN_Helper.h"

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
    class TTHLeptonicTagProducer : public EDProducer
    {

    public:
        TTHLeptonicTagProducer( const ParameterSet & );
    private:
        void produce( Event &, const EventSetup & ) override;
        int  chooseCategory( float tthmvavalue , bool debug_ );
        int computeStage1Kinematics( const TTHLeptonicTag );
        int  chooseCategory_pt( float tthmvavalue , float pT );

        const  reco::GenParticle* motherID(const reco::GenParticle* gp);
        bool PassFrixione(Handle<View<reco::GenParticle> > genParticles, const reco::GenParticle* gp, int nBinsForFrix, double cone_frix);
        vector<int> IsPromptAfterOverlapRemove(Handle<View<reco::GenParticle> > genParticles, const edm::Ptr<reco::GenParticle> genPho);
        int  GenPhoIndex(Handle<View<reco::GenParticle> > genParticles, const flashgg::Photon* pho, int usedIndex);
        double NearestDr(Handle<View<reco::GenParticle> > genParticles, const reco::GenParticle* gp);

        struct Sorter {
            bool operator()( const std::pair<unsigned int, float>  pair1, const std::pair<unsigned int,float>  pair2 )
            {
                return ( pair1.second > pair2.second );
            };
        };
        

        std::vector<edm::EDGetTokenT<View<flashgg::Jet> > > tokenJets_;
        std::vector<std::vector<edm::EDGetTokenT<edm::View<flashgg::Jet>>>> jetTokens_;
        EDGetTokenT<View<DiPhotonCandidate> > diPhotonToken_;
        std::vector<edm::EDGetTokenT<edm::View<DiPhotonCandidate>>> diPhotonTokens_;
        std::vector<edm::EDGetTokenT<edm::View<DiPhotonMVAResult>>> mvaResultTokens_;
        //EDGetTokenT<View<Jet> > thejetToken_;
        std::vector<edm::InputTag> inputTagJets_;
        EDGetTokenT<View<Electron> > electronToken_;
        EDGetTokenT<View<flashgg::Muon> > muonToken_;
        EDGetTokenT<View<flashgg::Met> > METToken_;
        std::vector<edm::EDGetTokenT<edm::View<flashgg::Met>>> metTokens_;
        EDGetTokenT<View<DiPhotonMVAResult> > mvaResultToken_;
        EDGetTokenT<View<Photon> > photonToken_;
        EDGetTokenT<View<reco::Vertex> > vertexToken_;
        EDGetTokenT<View<reco::GenParticle> > genParticleToken_;
        string systLabel_;

        typedef std::vector<edm::Handle<edm::View<flashgg::Jet> > > JetCollectionVector;

        unique_ptr<TMVA::Reader> DiphotonMva_;
        FileInPath MVAweightfile_;

        FileInPath topTaggerXMLfile_;

        unique_ptr<TMVA::Reader>TThMva_RunII_;
        FileInPath tthMVA_RunII_weightfile_;

        FileInPath tthVsttGGDNNfile_;
        std::vector<double> tthVsttGGDNN_global_mean_;
        std::vector<double> tthVsttGGDNN_global_stddev_;
        std::vector<double> tthVsttGGDNN_object_mean_;
        std::vector<double> tthVsttGGDNN_object_stddev_; 

        FileInPath tthVstHDNNfile_;
        std::vector<double> tthVstHDNN_global_mean_;
        std::vector<double> tthVstHDNN_global_stddev_;
        std::vector<double> tthVstHDNN_object_mean_;
        std::vector<double> tthVstHDNN_object_stddev_; 

        //Thresholds

        double MuonEtaCut_;
        double MuonPtCut_;
        double MuonIsoCut_;
        double MuonPhotonDrCut_;
        int    MinNLep_;
        int    MaxNLep_;

        double ElePtCut_;
        std::vector<double> EleEtaCuts_;
        double ElePhotonDrCut_;
        double ElePhotonZMassCut_;
        double DeltaRTrkEle_;

        double LeptonsZMassCut_;

        double DiLeptonJetThreshold_;
        double DiLeptonbJetThreshold_;
        double DiLeptonMVAThreshold_;

        double leadPhoOverMassThreshold_;
        double subleadPhoOverMassThreshold_;
        vector<double> MVAThreshold_;
        vector<double> MVAThreshold_pt1_;
        vector<double> MVAThreshold_pt2_;
        vector<double> MVAThreshold_pt3_;
        vector<double> MVAThreshold_pt4_;
        vector<double> STXSPtBoundaries_pt1;
        vector<double> STXSPtBoundaries_pt2;
        vector<double> STXSPtBoundaries_pt3;
        vector<double> STXSPtBoundaries_pt4;
        double deltaRJetLeadPhoThreshold_;
        double deltaRJetSubLeadPhoThreshold_;
        double jetsNumberThreshold_;
        double bjetsNumberThreshold_;
        double jetPtThreshold_;
        double jetEtaThreshold_;
        double deltaRJetLepton_;
        double leadingJetPtThreshold_;
        vector<double> bDiscriminator_;
        string bTag_;
        double PhoMVAThreshold_;
        double tthVstHThreshold_;

        bool UseCutBasedDiphoId_;
        bool debug_;
        bool SplitDiLeptEv_;
        vector<double> CutBasedDiphoId_;

        float leadeta_;
        float subleadeta_;
        float leadptom_;
        float subleadptom_;
        float leadIDMVA_;
        float subleadIDMVA_;
        float deltaphi_;
        float leadPSV_;
        float subleadPSV_;
        float nJets_;
        float nJets_bTagMedium_;
        float jet_pt1_;
        float jet_pt2_;
        float jet_pt3_;
        float jet_eta1_;
        float jet_eta2_;
        float jet_eta3_;
        float bTag1_;
        float bTag2_;
        float MetPt_;
        float MetPhi_;
        float lepton_leadPt_;
        float lepton_leadEta_;

        float lepton_leadPhi_;
     
        float minPhoID_;
        float maxPhoID_;
        float maxBTagVal_noBB_;
        float secondMaxBTagVal_noBB_;

        float diPhoY_;
        float diPhoPtoM_;
        float diPhoCosPhi_;
        float diPhoDeltaR_;

        float btag_1_;
        float btag_noBB_1_;
        float jetPt_1_;
        float jetEta_1_;
        float jetPhi_1_;
        float btag_2_;
        float btag_noBB_2_;
        float jetPt_2_;
        float jetEta_2_;
        float jetPhi_2_;
        float btag_3_;
        float btag_noBB_3_;
        float jetPt_3_;
        float jetEta_3_;
        float jetPhi_3_;
        float btag_4_;
        float btag_noBB_4_;
        float jetPt_4_;
        float jetEta_4_;
        float jetPhi_4_;

        float ht_;
        float helicity_angle_;

        float lepton_nTight_;

        float dnn_score_0_;

        float ttH_vs_tH_dnn_score;

        float tthMvaVal_RunII_;

        BDT_resolvedTopTagger *topTagger;
        TTH_DNN_Helper* dnn;
        TTH_DNN_Helper* dnn_ttH_vs_tH;

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

    const reco::GenParticle* TTHLeptonicTagProducer::motherID(const reco::GenParticle* gp)
    {
        const reco::GenParticle* mom_lead = gp;
        //cout << "in id: " << gp->pdgId() << endl;
        while( mom_lead->numberOfMothers() > 0 ) {
             for(uint j = 0; j < mom_lead->numberOfMothers(); ++j) {
                 mom_lead = dynamic_cast<const reco::GenParticle*>( mom_lead->mother(j) );
                 //cout << j << "th id: " << mom_lead->pdgId() << ", gpid: " << gp->pdgId() << endl;
                 if( mom_lead->pdgId() != gp->pdgId() )
                     return mom_lead; 
                     //break;
                }
             }

        return mom_lead;
    }

    bool TTHLeptonicTagProducer::PassFrixione(Handle<View<reco::GenParticle> > genParticles, const reco::GenParticle* gp, int nBinsForFrix, double cone_frix)
    {

        bool passFrix = true;

        double pho_et = gp->p4().Et();
        double pho_eta = gp->p4().eta();
        double pho_phi = gp->p4().phi();
        const double initConeFrix = 1E-10;
        double ets[nBinsForFrix] = {};
   
        for( unsigned int genLoop = 0 ; genLoop < genParticles->size(); genLoop++ )
           {
           int status = genParticles->ptrAt( genLoop )->status();
           if (!(status >= 21 && status <=24)) continue;
           int pdgid = genParticles->ptrAt( genLoop )->pdgId();
           if (!(abs(pdgid) == 11 || abs(pdgid) == 13 || abs(pdgid) == 15 || abs(pdgid) < 10 || abs(pdgid) == 21) ) continue; 
           double et = genParticles->ptrAt( genLoop )->p4().Et();
           double eta = genParticles->ptrAt( genLoop )->p4().eta();
           double phi = genParticles->ptrAt( genLoop )->p4().phi();
           double dR = deltaR(pho_eta,pho_phi,eta,phi); 
           for (int j = 0; j < nBinsForFrix; j++) { 
               double cone_var = (initConeFrix + j*cone_frix/nBinsForFrix);
               if (dR < cone_var && cone_var < cone_frix) ets[j] += et/(1-cos(cone_var) )*(1-cos(cone_frix) );
               }
           }

           for (int k = 0; k < nBinsForFrix; k++) {
               if (ets[k] > pho_et) {
                  passFrix = false; break;
                  }
           }

        return passFrix;
    }


    double TTHLeptonicTagProducer::NearestDr(Handle<View<reco::GenParticle> > genParticles, const reco::GenParticle* gp)
    {

        double nearDr = 999;

        double pho_eta = gp->p4().eta();
        double pho_phi = gp->p4().phi();

        for( unsigned int genLoop = 0 ; genLoop < genParticles->size(); genLoop++ )
           {
           int status = genParticles->ptrAt( genLoop )->status();
           if (!(status >= 21 && status <=24)) continue;
           int pdgid = genParticles->ptrAt( genLoop )->pdgId();
           if (!(abs(pdgid) == 11 || abs(pdgid) == 13 || abs(pdgid) == 15 || abs(pdgid) < 10 || abs(pdgid) == 21) ) continue;
           double eta = genParticles->ptrAt( genLoop )->p4().eta();
           double phi = genParticles->ptrAt( genLoop )->p4().phi();
           double dR = deltaR(pho_eta,pho_phi,eta,phi);
           if (dR < nearDr) {
              nearDr = dR;
              }
           }

        return nearDr;
    }


    vector<int> TTHLeptonicTagProducer::IsPromptAfterOverlapRemove(Handle<View<reco::GenParticle> > genParticles, const edm::Ptr<reco::GenParticle> genPho)
    {

        vector<int> flags;
        bool isPromptAfterOverlapRemove = false;

        int simplemotherID = -1;
        int simplemotherStatus = -1;
        if (genPho->numberOfMothers() > 0) {
            simplemotherID = genPho->mother(0)->pdgId();
            simplemotherStatus = genPho->mother(0)->status();
        }
        const reco::GenParticle* mom = motherID(&(*genPho));
        const reco::GenParticle* mommom = motherID(mom);
        bool isMad = false;
        if (simplemotherID == 22 && simplemotherStatus >= 21 && simplemotherStatus <= 24) isMad = true;
        bool isFromWb = false;
        if (abs(mom->pdgId()) == 24 || abs(mommom->pdgId()) == 24 || (abs(mom->pdgId()) == 5 && abs(mommom->pdgId()) == 6) ) isFromWb = true;
        bool isFromQuark = false;
        if (abs(mom->pdgId()) == 21 || abs(mommom->pdgId()) == 21 || (abs(mom->pdgId()) <= 6 && abs(mommom->pdgId()) != 6 && abs(mommom->pdgId()) != 24 ) ) isFromQuark = true;
        bool isFromProton = false;
        if (abs(mom->pdgId()) == 2212) isFromProton = true;
        bool failFrix = !PassFrixione(genParticles, &(*genPho), 100, 0.05);
        bool isPythia = false;
        if (!isMad && genPho->isPromptFinalState() && ( isFromWb || (isFromQuark && failFrix) || isFromProton) ) isPythia = true;
        if (isMad || isPythia) isPromptAfterOverlapRemove = true;
        flags.push_back(isPromptAfterOverlapRemove ? 1 : 0);
        flags.push_back(isMad ? 1 : 0);
        flags.push_back(isPythia ? 1 : 0);
        flags.push_back((!failFrix) ? 1 : 0);
        flags.push_back(simplemotherID);
        flags.push_back(simplemotherStatus);
        flags.push_back(abs(mom->pdgId()));
        flags.push_back(abs(mommom->pdgId()));
        return flags;

    }


    int TTHLeptonicTagProducer::GenPhoIndex(Handle<View<reco::GenParticle> > genParticles, const flashgg::Photon* pho, int usedIndex)
    {
        double maxDr = 0.2;
        double ptDiffMax = 99e15;
        int index = -1;
                
        for( unsigned int genLoop = 0 ; genLoop < genParticles->size(); genLoop++ ) {
            int pdgid = genParticles->ptrAt( genLoop )->pdgId();
            if (pho->genMatchType() != 1) continue;
            if (int(genLoop) == usedIndex) continue;
            if (abs(pdgid) != 22) continue;
            if (genParticles->ptrAt( genLoop )->p4().pt() < 10) continue;
            if (!genParticles->ptrAt( genLoop )->isPromptFinalState()) continue;

            double gen_photon_candidate_pt = genParticles->ptrAt( genLoop )->p4().pt();
            double gen_photon_candidate_eta = genParticles->ptrAt( genLoop )->p4().eta();
            double gen_photon_candidate_phi = genParticles->ptrAt( genLoop )->p4().phi();
            double deltaR_ = deltaR(gen_photon_candidate_eta, gen_photon_candidate_phi, pho->p4().eta(), pho->p4().phi());

            if (deltaR_ > maxDr) continue;

            double ptdiff = abs(gen_photon_candidate_pt - pho->p4().pt());
            if (ptdiff < ptDiffMax) {
                ptDiffMax = ptdiff;
                index = genLoop;
            }
            
        } // end gen loop

        return index;
    }



    TTHLeptonicTagProducer::TTHLeptonicTagProducer( const ParameterSet &iConfig ) :
        diPhotonToken_( consumes<View<flashgg::DiPhotonCandidate> >( iConfig.getParameter<InputTag> ( "DiPhotonTag" ) ) ),
        //thejetToken_( consumes<View<flashgg::Jet> >( iConfig.getParameter<InputTag>( "JetTag" ) ) ),
        inputTagJets_( iConfig.getParameter<std::vector<edm::InputTag> >( "inputTagJets" ) ),
        electronToken_( consumes<View<flashgg::Electron> >( iConfig.getParameter<InputTag>( "ElectronTag" ) ) ),
        muonToken_( consumes<View<flashgg::Muon> >( iConfig.getParameter<InputTag>( "MuonTag" ) ) ),
        METToken_( consumes<View<flashgg::Met> >( iConfig.getParameter<InputTag>( "MetTag" ) ) ),
        mvaResultToken_( consumes<View<flashgg::DiPhotonMVAResult> >( iConfig.getParameter<InputTag> ( "MVAResultTag" ) ) ),
        vertexToken_( consumes<View<reco::Vertex> >( iConfig.getParameter<InputTag> ( "VertexTag" ) ) ),
        genParticleToken_( consumes<View<reco::GenParticle> >( iConfig.getParameter<InputTag> ( "GenParticleTag" ) ) ),
        systLabel_( iConfig.getParameter<string> ( "SystLabel" ) )
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
            jetTags[0].push_back(inputTagJets_[i]); // nominal jets
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

        leadPhoOverMassThreshold_ = iConfig.getParameter<double>( "leadPhoOverMassThreshold");
        subleadPhoOverMassThreshold_ = iConfig.getParameter<double>( "subleadPhoOverMassThreshold");
        MVAThreshold_ = iConfig.getParameter<std::vector<double>>( "MVAThreshold");
        MVAThreshold_pt1_ = iConfig.getParameter<std::vector<double>>( "MVAThreshold_pt1");
        MVAThreshold_pt2_ = iConfig.getParameter<std::vector<double>>( "MVAThreshold_pt2");
        MVAThreshold_pt3_ = iConfig.getParameter<std::vector<double>>( "MVAThreshold_pt3");
        MVAThreshold_pt4_ = iConfig.getParameter<std::vector<double>>( "MVAThreshold_pt4");
        STXSPtBoundaries_pt1 = iConfig.getParameter<vector<double > >( "STXSPtBoundaries_pt1" );
        STXSPtBoundaries_pt2 = iConfig.getParameter<vector<double > >( "STXSPtBoundaries_pt2" );
        STXSPtBoundaries_pt3 = iConfig.getParameter<vector<double > >( "STXSPtBoundaries_pt3" );
        STXSPtBoundaries_pt4 = iConfig.getParameter<vector<double > >( "STXSPtBoundaries_pt4" );
        //assert( is_sorted( STXSPtBoundaries_pt1.begin(), STXSBoundaries_pt1.end() ) ); // 
        //assert( is_sorted( STXSPtBoundaries_pt2.begin(), STXSBoundaries_pt2.end() ) ); // 
        PhoMVAThreshold_ = iConfig.getParameter<double>( "PhoMVAThreshold");
        jetsNumberThreshold_ = iConfig.getParameter<double>( "jetsNumberThreshold");
        bjetsNumberThreshold_ = iConfig.getParameter<double>( "bjetsNumberThreshold");
        jetPtThreshold_ = iConfig.getParameter<double>( "jetPtThreshold");
        jetEtaThreshold_ = iConfig.getParameter<double>( "jetEtaThreshold");
        deltaRJetLepton_ = iConfig.getParameter<double>( "deltaRJetLepton");
        leadingJetPtThreshold_ = iConfig.getParameter<double>("leadingJetPtThreshold");

        tthVstHThreshold_ = iConfig.getParameter<double>("tthVstHThreshold");

        MuonEtaCut_ = iConfig.getParameter<double>( "MuonEtaCut");
        MuonPtCut_ = iConfig.getParameter<double>( "MuonPtCut");
        MuonIsoCut_ = iConfig.getParameter<double>( "MuonIsoCut");
        MuonPhotonDrCut_ = iConfig.getParameter<double>( "MuonPhotonDrCut");

        MinNLep_ = iConfig.getParameter<int>( "MinNLep");
        MaxNLep_ = iConfig.getParameter<int>( "MaxNLep");
 
        EleEtaCuts_ = iConfig.getParameter<std::vector<double>>( "EleEtaCuts");
        ElePtCut_ = iConfig.getParameter<double>( "ElePtCut");
        ElePhotonDrCut_ = iConfig.getParameter<double>( "ElePhotonDrCut");
        ElePhotonZMassCut_ = iConfig.getParameter<double>( "ElePhotonZMassCut");
        DeltaRTrkEle_ = iConfig.getParameter<double>( "DeltaRTrkEle");

        LeptonsZMassCut_ = iConfig.getParameter<double>( "LeptonsZMassCut");

        DiLeptonJetThreshold_ = iConfig.getParameter<double>( "DiLeptonJetThreshold");
        DiLeptonbJetThreshold_ = iConfig.getParameter<double>( "DiLeptonbJetThreshold");
        DiLeptonMVAThreshold_ = iConfig.getParameter<double>( "DiLeptonMVAThreshold");

        deltaRJetLeadPhoThreshold_ = iConfig.getParameter<double>( "deltaRJetLeadPhoThreshold");
        deltaRJetSubLeadPhoThreshold_ = iConfig.getParameter<double>( "deltaRJetSubLeadPhoThreshold");

        bDiscriminator_ = iConfig.getParameter<vector<double > >( "bDiscriminator");
        bTag_ = iConfig.getParameter<string>( "bTag");

        UseCutBasedDiphoId_ = iConfig.getParameter<bool>( "UseCutBasedDiphoId" );
        debug_ = iConfig.getParameter<bool>( "debug" );
        SplitDiLeptEv_ = iConfig.getParameter<bool>( "SplitDiLeptEv" );
        CutBasedDiphoId_ = iConfig.getParameter<std::vector<double>>( "CutBasedDiphoId" );

        MVAweightfile_ = iConfig.getParameter<edm::FileInPath>( "MVAweightfile" );
        topTaggerXMLfile_ = iConfig.getParameter<edm::FileInPath>( "topTaggerXMLfile" );
        tthVsttGGDNNfile_ = iConfig.getParameter<edm::FileInPath>( "tthVsttGGDNNfile" );
        tthVsttGGDNN_global_mean_ = iConfig.getParameter<std::vector<double>>( "tthVsttGGDNN_global_mean" );
        tthVsttGGDNN_global_stddev_ = iConfig.getParameter<std::vector<double>>( "tthVsttGGDNN_global_stddev" );
        tthVsttGGDNN_object_mean_ = iConfig.getParameter<std::vector<double>>( "tthVsttGGDNN_object_mean" );
        tthVsttGGDNN_object_stddev_ = iConfig.getParameter<std::vector<double>>( "tthVsttGGDNN_object_stddev" );

        tthVstHDNNfile_ = iConfig.getParameter<edm::FileInPath>( "tthVstHDNNfile" );
        tthVstHDNN_global_mean_ = iConfig.getParameter<std::vector<double>>( "tthVstHDNN_global_mean" );
        tthVstHDNN_global_stddev_ = iConfig.getParameter<std::vector<double>>( "tthVstHDNN_global_stddev" );
        tthVstHDNN_object_mean_ = iConfig.getParameter<std::vector<double>>( "tthVstHDNN_object_mean" );
        tthVstHDNN_object_stddev_ = iConfig.getParameter<std::vector<double>>( "tthVstHDNN_object_stddev" );
            
        tthMVA_RunII_weightfile_ = iConfig.getParameter<edm::FileInPath>( "tthMVA_RunII_weightfile" );
        

        DiphotonMva_.reset( new TMVA::Reader( "!Color:Silent" ) );
        DiphotonMva_->AddVariable( "dipho_leadEta", &leadeta_ );
        DiphotonMva_->AddVariable( "dipho_subleadEta", &subleadeta_ );
        DiphotonMva_->AddVariable( "dipho_lead_ptoM", &leadptom_ );
        DiphotonMva_->AddVariable( "dipho_sublead_ptoM", &subleadptom_ );
        DiphotonMva_->AddVariable( "dipho_leadIDMVA", &leadIDMVA_ );
        DiphotonMva_->AddVariable( "dipho_subleadIDMVA", &subleadIDMVA_ );
        DiphotonMva_->AddVariable( "dipho_deltaphi", &deltaphi_ );
        DiphotonMva_->AddVariable( "dipho_lead_PSV", &leadPSV_ );
        DiphotonMva_->AddVariable( "dipho_sublead_PSV", &subleadPSV_ );
        DiphotonMva_->AddVariable( "nJets", &nJets_ );
        DiphotonMva_->AddVariable( "nJets_bTagMedium", &nJets_bTagMedium_ );
        DiphotonMva_->AddVariable( "jet1_pt", &jet_pt1_ );
        DiphotonMva_->AddVariable( "jet2_pt", &jet_pt2_ );
        DiphotonMva_->AddVariable( "jet3_pt", &jet_pt3_ );
        DiphotonMva_->AddVariable( "jet1_eta", &jet_eta1_ );
        DiphotonMva_->AddVariable( "jet2_eta", &jet_eta2_ );
        DiphotonMva_->AddVariable( "jet3_eta", &jet_eta3_ );
        DiphotonMva_->AddVariable( "bTag1", &bTag1_ );
        DiphotonMva_->AddVariable( "bTag2", &bTag2_ );
        DiphotonMva_->AddVariable( "MetPt", &MetPt_ );
        DiphotonMva_->AddVariable( "lepton_leadPt", &lepton_leadPt_ );
        DiphotonMva_->AddVariable( "lepton_leadEta", &lepton_leadEta_ );

        DiphotonMva_->BookMVA( "BDT", MVAweightfile_.fullPath() );

        TThMva_RunII_.reset( new TMVA::Reader( "!Color:Silent" ) );
 
        TThMva_RunII_->AddVariable("maxIDMVA_", &maxPhoID_);
        TThMva_RunII_->AddVariable("minIDMVA_", &minPhoID_);
        TThMva_RunII_->AddVariable("max2_btag_", &secondMaxBTagVal_noBB_);
        TThMva_RunII_->AddVariable("max1_btag_", &maxBTagVal_noBB_);
        TThMva_RunII_->AddVariable("dipho_delta_R", &diPhoDeltaR_);
        TThMva_RunII_->AddVariable("njets_", &nJets_);
        TThMva_RunII_->AddVariable("ht_", &ht_);
        TThMva_RunII_->AddVariable("leadptoM_", &leadptom_);
        TThMva_RunII_->AddVariable("subleadptoM_", &subleadptom_);
        TThMva_RunII_->AddVariable("lead_eta_", &leadeta_);
        TThMva_RunII_->AddVariable("sublead_eta_", &subleadeta_);
 
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
 
        TThMva_RunII_->AddVariable("leadPSV_", &leadPSV_);
        TThMva_RunII_->AddVariable("subleadPSV_", &subleadPSV_);
 
        TThMva_RunII_->AddVariable("dipho_cosphi_", &diPhoCosPhi_);
        TThMva_RunII_->AddVariable("dipho_rapidity_", &diPhoY_);
        TThMva_RunII_->AddVariable("met_", &MetPt_);
 
        TThMva_RunII_->AddVariable("dipho_pt_over_mass_", &diPhoPtoM_);
 
        TThMva_RunII_->AddVariable("helicity_angle_", &helicity_angle_);
        TThMva_RunII_->AddVariable("lep_pt_", &lepton_leadPt_);
        TThMva_RunII_->AddVariable("lep_eta_", &lepton_leadEta_); 
        TThMva_RunII_->AddVariable("n_lep_tight_", &lepton_nTight_);
 
        TThMva_RunII_->AddVariable("dnn_score_0", &dnn_score_0_);
         
        TThMva_RunII_->BookMVA( "BDT" , tthMVA_RunII_weightfile_.fullPath());     

        if (useLargeMVAs) {
            topTagger = new BDT_resolvedTopTagger(topTaggerXMLfile_.fullPath());
            dnn = new TTH_DNN_Helper(tthVsttGGDNNfile_.fullPath());
            dnn->SetInputShapes(19, 9, 8);
            dnn->SetPreprocessingSchemes(tthVsttGGDNN_global_mean_, tthVsttGGDNN_global_stddev_, tthVsttGGDNN_object_mean_, tthVsttGGDNN_object_stddev_);

            dnn_ttH_vs_tH = new TTH_DNN_Helper(tthVstHDNNfile_.fullPath());
            dnn_ttH_vs_tH->SetInputShapes(23, 9, 8);
            dnn_ttH_vs_tH->SetPreprocessingSchemes(tthVstHDNN_global_mean_, tthVstHDNN_global_stddev_, tthVstHDNN_object_mean_, tthVstHDNN_object_stddev_);
        }

        for (unsigned i = 0 ; i < inputTagJets_.size() ; i++) {
            auto token = consumes<View<flashgg::Jet> >(inputTagJets_[i]);
            tokenJets_.push_back(token);
        }

        if (modifySystematicsWorkflow) {
            for (auto & syst_name : systematicsLabels)
                produces<vector<TTHLeptonicTag>>(syst_name);
        }

        else {
            produces<vector<TTHLeptonicTag> >();
        }

    }

    int TTHLeptonicTagProducer::chooseCategory_pt( float tthmvavalue , float pT)
    {
        // should return 0 if mva above all the numbers, 1 if below the first, ..., boundaries.size()-N if below the Nth, ...
        if (pT > STXSPtBoundaries_pt1[0] && pT < STXSPtBoundaries_pt1[1]) {
            for(int n = 0 ; n < ( int )MVAThreshold_pt1_.size() ; n++ ) {
                if( ( double )tthmvavalue > MVAThreshold_pt1_[MVAThreshold_pt1_.size() - n - 1] ) {
                    //cout << "pT range: [" << STXSPtBoundaries_pt1[0] << ", " << STXSPtBoundaries_pt1[1] << "], Leptonic cat " << n << endl; 
                    return n; 
                }
            }
        }

        if (pT > STXSPtBoundaries_pt2[0] && pT < STXSPtBoundaries_pt2[1]) {
            for(int n = 0 ; n < ( int )MVAThreshold_pt2_.size() ; n++ ) {
                if( ( double )tthmvavalue > MVAThreshold_pt2_[MVAThreshold_pt2_.size() - n - 1] ) {
                    //cout << "pT range: [" << STXSPtBoundaries_pt2[0] << ", " << STXSPtBoundaries_pt2[1] << "], Leptonic cat " << n + MVAThreshold_pt1_.size() << endl; 
                    return n + MVAThreshold_pt1_.size(); 
                }
            }
        }

        if (pT > STXSPtBoundaries_pt3[0] && pT < STXSPtBoundaries_pt3[1]) {
            for(int n = 0 ; n < ( int )MVAThreshold_pt3_.size() ; n++ ) {
                if( ( double )tthmvavalue > MVAThreshold_pt3_[MVAThreshold_pt3_.size() - n - 1] ) {
                    //cout << "pT range: [" << STXSPtBoundaries_pt3[0] << ", " << STXSPtBoundaries_pt3[1] << "], Leptonic cat " << n + MVAThreshold_pt1_.size() << endl; 
                    return n + MVAThreshold_pt1_.size() + MVAThreshold_pt2_.size(); 
                }
            }
        }

        if (pT > STXSPtBoundaries_pt4[0] && pT < STXSPtBoundaries_pt4[1]) {
            for(int n = 0 ; n < ( int )MVAThreshold_pt4_.size() ; n++ ) {
                if( ( double )tthmvavalue > MVAThreshold_pt4_[MVAThreshold_pt4_.size() - n - 1] ) {
                    //cout << "pT range: [" << STXSPtBoundaries_pt4[0] << ", " << STXSPtBoundaries_pt4[1] << "], Leptonic cat " << n + MVAThreshold_pt1_.size() << endl; 
                    return n + MVAThreshold_pt1_.size() + MVAThreshold_pt2_.size() + MVAThreshold_pt3_.size();  
                }
            }
        }

        return -1; // Does not pass, object will not be produced
    }

    int TTHLeptonicTagProducer::chooseCategory( float tthmvavalue , bool debug_)
    {
        // should return 0 if mva above all the numbers, 1 if below the first, ..., boundaries.size()-N if below the Nth, ...
        for(int n = 0 ; n < ( int )MVAThreshold_.size() ; n++ ) {
            //if( ( double )tthmvavalue > MVAThreshold_[MVAThreshold_.size() - n - 1] ) { return n; }
            if( ( double )tthmvavalue > MVAThreshold_[MVAThreshold_.size() - n - 1] ) { cout << "Leptonic cat " << n; return n; }
        }

      if(debug_)
        {   cout << "Checking class, thresholds: ";
            for(unsigned int i=0; i<MVAThreshold_.size(); ++i)
                cout << MVAThreshold_[i] << " ";
        }

      
        return -1; // Does not pass, object will not be produced
    }

    void TTHLeptonicTagProducer::produce( Event &evt, const EventSetup & )
    {
        //Handle<View<flashgg::Jet> > theJets;
        //evt.getByToken( thejetToken_, theJets );
        //const PtrVector<flashgg::Jet>& jetPointers = theJets->ptrVector();
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

        Handle<View<flashgg::DiPhotonMVAResult> > mvaResults;
        if (!modifySystematicsWorkflow)
            evt.getByToken( mvaResultToken_, mvaResults );

        Handle<View<reco::GenParticle> > genParticles;

        Handle<View<reco::Vertex> > vertices;
        evt.getByToken( vertexToken_, vertices );

        Handle<View<flashgg::Met> > theMet_;
        if (!modifySystematicsWorkflow)
            evt.getByToken( METToken_, theMet_ );

        //std::unique_ptr<vector<TTHLeptonicTag> > tthltags( new vector<TTHLeptonicTag> );

        //assert( diPhotons->size() == mvaResults->size() );

        double idmva1 = 0.;
        double idmva2 = 0.;

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
                //Handle<View<flashgg::Met> > theMet_;
                if (vary_met) {
                    int met_syst_idx = syst_idx - (inputJetsSuffixes_.size() + inputDiPhotonSuffixes_.size());
                    evt.getByToken(metTokens_[met_syst_idx], theMet_);
                }
                else
                    evt.getByToken(metTokens_[0], theMet_);
            }

            assert( diPhotons->size() == mvaResults->size() );

            std::unique_ptr<vector<TTHLeptonicTag> > tthltags( new vector<TTHLeptonicTag> );

            if( ! evt.isRealData() )
            {
                evt.getByToken( genParticleToken_, genParticles );
            }


            for( unsigned int diphoIndex = 0; diphoIndex < diPhotons->size(); diphoIndex++ )
            {
                unsigned int jetCollectionIndex = diPhotons->ptrAt( diphoIndex )->jetCollectionIndex();

                edm::Ptr<flashgg::DiPhotonCandidate> dipho = diPhotons->ptrAt( diphoIndex );
                edm::Ptr<flashgg::DiPhotonMVAResult> mvares = mvaResults->ptrAt( diphoIndex );

                if( dipho->leadingPhoton()->pt() < ( dipho->mass() )*leadPhoOverMassThreshold_ ) { continue; }
                if( dipho->subLeadingPhoton()->pt() < ( dipho->mass() )*subleadPhoOverMassThreshold_ ) { continue; }
                idmva1 = dipho->leadingPhoton()->phoIdMvaDWrtVtx( dipho->vtx() );
                idmva2 = dipho->subLeadingPhoton()->phoIdMvaDWrtVtx( dipho->vtx() );

                if(debug_)
                    cout << "Photon pair with PhoIdMVA values: " << idmva1 << " " << idmva2 << endl;
     
                if( idmva1 < PhoMVAThreshold_ || idmva2 < PhoMVAThreshold_ ) { continue; }

                ht_ = 0.;
                btag_1_=-999;
                btag_noBB_1_ = -999;
                jetPt_1_=-999;
                jetEta_1_=-999;
                jetPhi_1_=-999;
                btag_2_=-999;
                btag_noBB_2_ = -999;
                jetPt_2_=-999;
                jetEta_2_=-999;
                jetPhi_2_=-999;
                btag_3_=-999;
                btag_noBB_3_ = -999;
                jetPt_3_=-999;
                jetEta_3_=-999;
                jetPhi_3_=-999;
                btag_4_=-999;
                btag_noBB_4_ = -999;
                jetPt_4_=-999;
                jetEta_4_=-999;
                jetPhi_4_=-999;

                bool passDiphotonSelection = true;
                if(UseCutBasedDiphoId_)
                {
                    assert(CutBasedDiphoId_.size()==6);
                    if(dipho->leadingPhoton()->pt()/dipho->mass() < CutBasedDiphoId_[0]) passDiphotonSelection = false;
                    if(dipho->subLeadingPhoton()->pt()/dipho->mass() < CutBasedDiphoId_[1]) passDiphotonSelection = false;
                    if(dipho->leadingPhoton()->phoIdMvaDWrtVtx( dipho->vtx() ) < CutBasedDiphoId_[2]) passDiphotonSelection = false;
                    if(dipho->subLeadingPhoton()->phoIdMvaDWrtVtx( dipho->vtx() ) < CutBasedDiphoId_[3]) passDiphotonSelection = false;
                    if(abs (dipho->leadingPhoton()->eta() - dipho->subLeadingPhoton()->eta()) > CutBasedDiphoId_[4]) passDiphotonSelection = false;
                    if(deltaPhi(dipho->leadingPhoton()->phi(), dipho->subLeadingPhoton()->phi() ) > CutBasedDiphoId_[5] ) passDiphotonSelection = false;
                }

                if(!passDiphotonSelection) continue;

                if(debug_)
                    cout << "Passed photon selection, checking leptons: " << idmva1 << " " << idmva2 << endl;

                std::vector<edm::Ptr<flashgg::Muon> >     Muons;
                std::vector<edm::Ptr<flashgg::Muon> >     MuonsTight;
                std::vector<edm::Ptr<flashgg::Electron> > Electrons;
                std::vector<edm::Ptr<flashgg::Electron> > ElectronsTight;
                
                std::vector<double> lepPt;
                std::vector<double> lepEta;
                std::vector<double> lepPhi;
                std::vector<double> lepE;
                std::vector<int>    lepType;

                if(theMuons->size()>0) {
                    Muons = selectMuons(theMuons->ptrs(), dipho, vertices->ptrs(), MuonPtCut_, MuonEtaCut_, MuonIsoCut_, MuonPhotonDrCut_, debug_);
                    MuonsTight = selectMuons(theMuons->ptrs(), dipho, vertices->ptrs(), MuonPtCut_, MuonEtaCut_, MuonIsoCut_, MuonPhotonDrCut_, debug_, 3);
                }
                if(theElectrons->size()>0) {
                    Electrons = selectElectrons(theElectrons->ptrs(), dipho, ElePtCut_, EleEtaCuts_, ElePhotonDrCut_, ElePhotonZMassCut_, DeltaRTrkEle_, debug_);
                    ElectronsTight = selectElectrons(theElectrons->ptrs(), dipho, ElePtCut_, EleEtaCuts_, ElePhotonDrCut_, ElePhotonZMassCut_, DeltaRTrkEle_, debug_, 3);
                }

                //If 2 same flavour leptons are found remove the pairs with mass compatible with a Z boson

                if(Muons.size()>=2)
                {
                    std::vector<edm::Ptr<flashgg::Muon>> Muons_0;
                    Muons_0 = Muons;
                    std::vector<int> badIndexes;

                    for(unsigned int i=0; i<Muons_0.size(); ++i)
                    {
                        for(unsigned int j=i+1; j<Muons_0.size(); ++j)
                        {
                            TLorentzVector l1, l2;
                            l1.SetPtEtaPhiE(Muons_0[i]->pt(), Muons_0[i]->eta(), Muons_0[i]->phi(), Muons_0[i]->energy());
                            l2.SetPtEtaPhiE(Muons_0[j]->pt(), Muons_0[j]->eta(), Muons_0[j]->phi(), Muons_0[j]->energy());

                            if(fabs((l1+l2).M() - 91.187) < LeptonsZMassCut_)
                            {
                                badIndexes.push_back(i);
                                badIndexes.push_back(j);
                            }
                        }
                    }

                    if(badIndexes.size()!=0)
                    {
                        Muons.clear();
                        for(unsigned int i=0; i<Muons_0.size(); ++i)
                        {
                           bool isBad = false;
                           for(unsigned int j=0; j<badIndexes.size(); ++j)
                           {
                              if(badIndexes[j]==(int)i)
                                   isBad = true;
                          }
                          if(!isBad) Muons.push_back(Muons_0[i]);
                        }
                    }
                }        

                if(Electrons.size()>=2)
                {
                    std::vector<int> badIndexes;
                    std::vector<edm::Ptr<flashgg::Electron> > Electrons_0;
                    Electrons_0 = Electrons;
                    for(unsigned int i=0; i<Electrons_0.size(); ++i)
                    {
                        for(unsigned int j=i+1; j<Electrons_0.size(); ++j)
                        {
                            TLorentzVector l1, l2;
                            l1.SetPtEtaPhiE(Electrons_0[i]->pt(), Electrons_0[i]->eta(), Electrons_0[i]->phi(), Electrons_0[i]->energy());
                            l2.SetPtEtaPhiE(Electrons_0[j]->pt(), Electrons_0[j]->eta(), Electrons_0[j]->phi(), Electrons_0[j]->energy());

                            if(fabs((l1+l2).M() - 91.187) < LeptonsZMassCut_)
                            {
                                badIndexes.push_back(i);
                                badIndexes.push_back(j);
                            }
                        }
                    }
                    if(badIndexes.size()!=0)
                    {
                        Electrons.clear();

                        for(unsigned int i=0; i<Electrons_0.size(); ++i)
                        {
                             bool isBad = false;
                             for(unsigned int j=0; j<badIndexes.size(); ++j)
                             {
                                 if(badIndexes[j]==(int)i)
                                     isBad = true;
                             }
                             if(!isBad) Electrons.push_back(Electrons_0[i]);
                        }
                     }
                 }        

                if( (Muons.size() + Electrons.size()) < (unsigned) MinNLep_ || (Muons.size() + Electrons.size()) > (unsigned) MaxNLep_) continue;

                // Fill lepton vectors            
                // ===================
                
                std::vector<std::pair< unsigned int, float > > sorter;

                if(debug_) cout<<" nMuons="<<Muons.size()<<" nElectrons="<<Electrons.size()<< endl;

                for(unsigned int i=0;i<Muons.size();i++){
                    float pt=Muons[i]->pt();
                    int index=100;
                    index+=i;
                    std::pair<unsigned int, float>pairToSort = std::make_pair(index, pt);
                    sorter.push_back( pairToSort );
                    if(debug_) cout<<" muon "<< i <<" pt="<<pt<< endl;
                }
                for(unsigned int i=0;i<Electrons.size();i++){
                    float pt=Electrons[i]->pt();
                    int index=200;
                    index+=i;
                    std::pair<unsigned int, float>pairToSort = std::make_pair(index, pt);
                    sorter.push_back( pairToSort );
                    if(debug_) cout<<" elec "<< i <<" pt="<<pt<< endl;
                }
                // sort map by pt

                std::sort( sorter.begin(), sorter.end(), Sorter() );
                    
                // fill vectors
                for (unsigned int i=0;i<sorter.size();i++){

                    if(debug_) cout<<" Filling lepton vector index:"<<sorter[i].first<<" pt:" <<sorter[i].second << endl;

                    lepPt.push_back(sorter[i].second);                

                    int type=0;
                    type=(sorter[i].first)/100;
                    int n=(sorter[i].first)%100;

                    lepType.push_back(type);

                    if(debug_) cout<<" type="<<type<<" n="<<n<< endl;
                        
                    if(type==1){
                        if(debug_) cout<<"MUON LEPPTCHECK "<<   sorter[i].second<<" "<<Muons[n]->pt()<< endl;
                        lepEta.push_back(Muons[n]->eta());
                        lepPhi.push_back(Muons[n]->phi());
                        lepE.push_back(Muons[n]->energy());
                        
                    }else if(type==2){
                        if(debug_) cout<<"ELEC LEPPTCHECK "<<   sorter[i].second<<" "<<Electrons[n]->pt()<< endl;
                        lepEta.push_back(Electrons[n]->eta());
                        lepPhi.push_back(Electrons[n]->phi());
                        lepE.push_back(Electrons[n]->energy());
                    }                
                }
                
                int njet_ = 0;
                int njets_btagloose_ = 0;
                int njets_btagmedium_ = 0;
                int njets_btagtight_ = 0;
                std::vector<edm::Ptr<flashgg::Jet>> tagJets;
                std::vector<edm::Ptr<flashgg::Jet>> tagBJets;
                std::vector<float> bTags;
                std::vector<float> bTags_noBB;

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

                for( unsigned int jetIndex = 0; jetIndex < Jets[jetCollectionIndex]->size() ; jetIndex++ )
                {
                    edm::Ptr<flashgg::Jet> thejet = Jets[jetCollectionIndex]->ptrAt( jetIndex );

                    if( fabs( thejet->eta() ) > jetEtaThreshold_ ) { continue; }
                    if(!thejet->passesJetID  ( flashgg::Loose ) ) { continue; }
                    if( thejet->pt() < jetPtThreshold_ ) { continue; }

                    float dRPhoLeadJet = deltaR( thejet->eta(), thejet->phi(), dipho->leadingPhoton()->eta(), dipho->leadingPhoton()->phi() ) ;
                    float dRPhoSubLeadJet = deltaR( thejet->eta(), thejet->phi(), dipho->subLeadingPhoton()->eta(), dipho->subLeadingPhoton()->phi() );

                    if( dRPhoLeadJet < deltaRJetLeadPhoThreshold_ || dRPhoSubLeadJet < deltaRJetSubLeadPhoThreshold_ ) { continue; }

                    bool passDrLeptons = 1;                

                    for( unsigned int muonIndex = 0; muonIndex < Muons.size(); muonIndex++ )
                    {
                        Ptr<flashgg::Muon> muon = Muons[muonIndex];

                        float dRLept = deltaR( thejet->eta(), thejet->phi(), muon->eta(),  muon->phi() ) ;
                        if( dRLept > deltaRJetLepton_) { continue; }

                        passDrLeptons = 0;
                        break;
                    }

                    if(passDrLeptons)
                    {   for( unsigned int eleIndex = 0; eleIndex < Electrons.size(); eleIndex++ )
                        {
                            Ptr<flashgg::Electron> ele = Electrons[eleIndex];
                        
                            float dRLept = deltaR( thejet->eta(), thejet->phi(), ele->eta(),  ele->phi() ) ;
                            if( dRLept > deltaRJetLepton_) { continue; }

                            passDrLeptons = 0;
                            break;
                        }
                    }

                    if(passDrLeptons)
                    {
                        njet_++;
                        ht_ += thejet->pt();
                        tagJets.push_back( thejet );
                        float bDiscriminatorValue = -2.;
                        if(bTag_ == "pfDeepCSV") bDiscriminatorValue = thejet->bDiscriminator("pfDeepCSVJetTags:probb")+thejet->bDiscriminator("pfDeepCSVJetTags:probbb") ;
                        else  bDiscriminatorValue = thejet->bDiscriminator( bTag_ );

                        float bDiscriminatorValue_noBB = -2;
                        if(bTag_ == "pfDeepCSV") bDiscriminatorValue_noBB = thejet->bDiscriminator("pfDeepCSVJetTags:probb");
                        else  bDiscriminatorValue_noBB = thejet->bDiscriminator( bTag_ );

                        bDiscriminatorValue >= 0. ? bTags.push_back(bDiscriminatorValue) : bTags.push_back(-1.);
                        bDiscriminatorValue_noBB >= 0. ? bTags_noBB.push_back(bDiscriminatorValue_noBB) : bTags_noBB.push_back(-1.);

                        if( bDiscriminatorValue > bDiscriminator_[0] ) njets_btagloose_++;
                        if( bDiscriminatorValue > bDiscriminator_[1] ) njets_btagmedium_++;
                        if( bDiscriminatorValue > bDiscriminator_[2] ) njets_btagtight_++;

                        if( bDiscriminatorValue > bDiscriminator_[1] )
                            tagBJets.push_back( thejet );
                    
                        if (useLargeMVAs) {
                          float cvsl = thejet->bDiscriminator("pfDeepCSVJetTags:probc") + thejet->bDiscriminator("pfDeepCSVJetTags:probudsg") ;
                          float cvsb = thejet->bDiscriminator("pfDeepCSVJetTags:probc") + thejet->bDiscriminator("pfDeepCSVJetTags:probb")+thejet->bDiscriminator("pfDeepCSVJetTags:probbb") ;
                          float ptD = thejet->userFloat("ptD") ;
                          float axis1 = thejet->userFloat("axis1") ;
                          int mult = thejet->userFloat("totalMult") ;

                          topTagger->addJet(thejet->pt(), thejet->eta(), thejet->phi(), thejet->mass(), bDiscriminatorValue, cvsl, cvsb, ptD, axis1, mult);
                        }

                    }
                     
                }

                if(njet_ < jetsNumberThreshold_ || njets_btagmedium_ < bjetsNumberThreshold_) continue;
     
                if(debug_)
                    cout << "Jets after selections " << njet_ << ", bJets " << njets_btagmedium_ << endl;

                std::sort(bTags.begin(),bTags.end(),std::greater<float>());
                std::sort(bTags_noBB.begin(),bTags_noBB.end(),std::greater<float>());

                // Set variables to compute MVA value
                if(tagJets.size()>0){
                    if(bTag_ == "pfDeepCSV") btag_1_=tagJets[0]->bDiscriminator("pfDeepCSVJetTags:probb")+tagJets[0]->bDiscriminator("pfDeepCSVJetTags:probbb") ;
                    else  btag_1_ = tagJets[0]->bDiscriminator( bTag_ );
                    if(bTag_ == "pfDeepCSV") btag_noBB_1_=tagJets[0]->bDiscriminator("pfDeepCSVJetTags:probb");
                    else  btag_noBB_1_ = tagJets[0]->bDiscriminator( bTag_ );
                    jetPt_1_=tagJets[0]->pt();
                    jetEta_1_=tagJets[0]->eta();
                    jetPhi_1_=tagJets[0]->phi();
                }

                if(tagJets.size()>1){
                    if(bTag_ == "pfDeepCSV") btag_2_=tagJets[1]->bDiscriminator("pfDeepCSVJetTags:probb")+tagJets[1]->bDiscriminator("pfDeepCSVJetTags:probbb") ;
                    else  btag_2_ = tagJets[1]->bDiscriminator( bTag_ );
                    if(bTag_ == "pfDeepCSV") btag_noBB_2_=tagJets[1]->bDiscriminator("pfDeepCSVJetTags:probb");
                    else  btag_noBB_2_ = tagJets[1]->bDiscriminator( bTag_ );
                    jetPt_2_=tagJets[1]->pt();
                    jetEta_2_=tagJets[1]->eta();
                    jetPhi_2_=tagJets[1]->phi();
                }

                if(tagJets.size()>2){
                    if(bTag_ == "pfDeepCSV") btag_3_=tagJets[2]->bDiscriminator("pfDeepCSVJetTags:probb")+tagJets[2]->bDiscriminator("pfDeepCSVJetTags:probbb") ;
                    else  btag_3_ = tagJets[2]->bDiscriminator( bTag_ );
                    if(bTag_ == "pfDeepCSV") btag_noBB_3_=tagJets[2]->bDiscriminator("pfDeepCSVJetTags:probb");
                    else  btag_noBB_3_ = tagJets[2]->bDiscriminator( bTag_ );
                    jetPt_3_=tagJets[2]->pt();
                    jetEta_3_=tagJets[2]->eta();
                    jetPhi_3_=tagJets[2]->phi();
                }
                /* kinematics of fourth leading jet not currently used in BDT
                if(tagJets.size()>3){
                    if(bTag_ == "pfDeepCSV") btag_4_=tagJets[3]->bDiscriminator("pfDeepCSVJetTags:probb")+tagJets[3]->bDiscriminator("pfDeepCSVJetTags:probbb") ;
                    else  btag_4_ = tagJets[3]->bDiscriminator( bTag_ );
                    if(bTag_ == "pfDeepCSV") btag_noBB_4_=tagJets[3]->bDiscriminator("pfDeepCSVJetTags:probb");
                    else  btag_noBB_4_ = tagJets[3]->bDiscriminator( bTag_ );
                    jetPt_4_=tagJets[3]->pt();
                    jetEta_4_=tagJets[3]->eta();
                    jetPhi_4_=tagJets[3]->phi();
                } */

                diPhoY_= dipho->rapidity();
                diPhoPtoM_= dipho->pt()/dipho->mass();
                diPhoCosPhi_=  abs(TMath::Cos( deltaPhi( dipho->leadingPhoton()->phi(), dipho->subLeadingPhoton()->phi() ) ));
                diPhoDeltaR_ = deltaR( dipho->leadingPhoton()->eta(),dipho->leadingPhoton()->phi(), dipho->subLeadingPhoton()->eta(),dipho->subLeadingPhoton()->phi());

                leadeta_ = dipho->leadingPhoton()->eta();
                subleadeta_ = dipho->subLeadingPhoton()->eta();
                leadptom_ = dipho->leadingPhoton()->pt()/dipho->mass();
                subleadptom_ = dipho->subLeadingPhoton()->pt()/dipho->mass();
                leadIDMVA_ = dipho->leadingPhoton()->phoIdMvaDWrtVtx( dipho->vtx() );
                subleadIDMVA_ = dipho->subLeadingPhoton()->phoIdMvaDWrtVtx( dipho->vtx() );
                deltaphi_ = deltaPhi( dipho->leadingPhoton()->phi(), dipho->subLeadingPhoton()->phi() );
                leadPSV_ = dipho->leadingPhoton()->hasPixelSeed();
                subleadPSV_ = dipho->subLeadingPhoton()->hasPixelSeed();
                nJets_ = njet_;
                nJets_bTagMedium_ = njets_btagmedium_;

                minPhoID_=TMath::Min( leadIDMVA_, subleadIDMVA_);
                maxPhoID_=TMath::Max( leadIDMVA_, subleadIDMVA_);

                maxBTagVal_noBB_ = bTags_noBB.size() > 0 ? bTags_noBB[0] : -1.;
                secondMaxBTagVal_noBB_ = bTags_noBB.size() > 1 ? bTags_noBB[1]: -1.;

                lepton_nTight_ = float(MuonsTight.size() + ElectronsTight.size());

                if(tagJets.size()==0)
                {
                    jet_pt1_ = -1;
                    jet_pt2_ = -1.;
                    jet_pt3_ = -1.;
                    jet_eta1_ = -5;
                    jet_eta2_ = -5.;
                    jet_eta3_ = -5.;

                    bTag1_ = -1;
                    bTag2_ = -1;
                
                }else if(tagJets.size()==1)
                {
                    jet_pt1_ = tagJets[0]->pt();
                    jet_pt2_ = -1.;
                    jet_pt3_ = -1.;
                    jet_eta1_ = tagJets[0]->eta();
                    jet_eta2_ = -5.;
                    jet_eta3_ = -5.;

                    bTag1_ = bTags[0];
                    bTag2_ = -1.;
                }
                else if(tagJets.size()==2)
                {
                    jet_pt1_ = tagJets[0]->pt();
                    jet_pt2_ = tagJets[1]->pt();
                    jet_pt3_ = -1.;
                    jet_eta1_ = tagJets[0]->eta();
                    jet_eta2_ = tagJets[1]->eta();
                    jet_eta3_ = -5.;

                    bTag1_ = bTags[0];
                    bTag2_ = bTags[1];
                }
                else
                {
                    jet_pt1_ = tagJets[0]->pt();
                    jet_pt2_ = tagJets[1]->pt();
                    jet_pt3_ = tagJets[2]->pt();
                    jet_eta1_ = tagJets[0]->eta();
                    jet_eta2_ = tagJets[1]->eta();
                    jet_eta3_ = tagJets[2]->eta();

                    bTag1_ = bTags[0];
                    bTag2_ = bTags[1];
                }

                if( theMet_ -> size() != 1 )
                    std::cout << "WARNING number of MET is not equal to 1" << std::endl;
                MetPt_ = theMet_->ptrAt( 0 ) -> getCorPt();
                MetPhi_ = theMet_->ptrAt( 0 ) -> phi();

                TLorentzVector pho1, pho2;
                pho1.SetPtEtaPhiE(dipho->leadingPhoton()->pt(), dipho->leadingPhoton()->eta(), dipho->leadingPhoton()->phi(), dipho->leadingPhoton()->energy());
                pho2.SetPtEtaPhiE(dipho->subLeadingPhoton()->pt(), dipho->subLeadingPhoton()->eta(), dipho->subLeadingPhoton()->phi(), dipho->subLeadingPhoton()->energy());
                helicity_angle_ = helicity(pho1, pho2);

                std::vector<double> global_features;
                global_features.resize(19);
                global_features[0] = dipho->leadingPhoton()->eta();
                global_features[1] = dipho->subLeadingPhoton()->eta();
                global_features[2] = dipho->leadingPhoton()->phi();
                global_features[3] = dipho->subLeadingPhoton()->phi();
                global_features[4] = leadptom_;
                global_features[5] = subleadptom_;
                global_features[6] = maxPhoID_;
                global_features[7] = minPhoID_;
                global_features[8] = log(MetPt_);
                global_features[9] = MetPhi_;
                global_features[10] = leadPSV_;
                global_features[11] = subleadPSV_;
                global_features[12] = diPhoY_;
                global_features[13] = diPhoPtoM_;
                global_features[14] = diPhoDeltaR_;
                global_features[15] = maxBTagVal_noBB_;
                global_features[16] = secondMaxBTagVal_noBB_;
                global_features[17] = nJets_;
                global_features[18] = lepton_nTight_;

                std::vector<double> global_features_ttH_vs_tH;
                global_features_ttH_vs_tH.resize(23);

                for (unsigned int i = 0; i < global_features.size(); i++)
                    global_features_ttH_vs_tH[i] = global_features[i];

                double forward_jet_pt, forward_jet_eta;
                calculate_forward_jet_features(forward_jet_pt, forward_jet_eta, tagJets, "pfDeepCSVJetTags:probb", maxBTagVal_noBB_);
  
                double lep1_charge, lep2_charge;
                calculate_lepton_charges(lep1_charge, lep2_charge, Muons, Electrons);

                global_features_ttH_vs_tH[19] = lep1_charge;
                global_features_ttH_vs_tH[20] = lep2_charge;
                global_features_ttH_vs_tH[21] = forward_jet_eta;
                global_features_ttH_vs_tH[22] = forward_jet_pt; 

                if (useLargeMVAs) {
                    dnn->SetInputs(tagJets, Muons, Electrons, global_features);
                    dnn_score_0_ = dnn->EvaluateDNN();

                    dnn_ttH_vs_tH->SetInputs(tagJets, Muons, Electrons, global_features_ttH_vs_tH);
                    ttH_vs_tH_dnn_score = dnn_ttH_vs_tH->EvaluateDNN();
                }

                vector<float> mvaEval; 
                if (useLargeMVAs) {
                    mvaEval = topTagger->EvalMVA();
                    topTagger->clear();
                }

                int leadMuIndex = 0;
                float leadMuPt = -1;
                int leadEleIndex = 0;
                float leadElePt = -1;

                for( unsigned int muonIndex = 0; muonIndex < Muons.size(); muonIndex++ )
                {
                    Ptr<flashgg::Muon> muon = Muons[muonIndex];

                    if(muon->pt()>leadMuPt)
                    {
                        leadMuPt = muon->pt();
                        leadMuIndex = muonIndex;
                    }
                }

                for( unsigned int eleIndex = 0; eleIndex < Electrons.size(); eleIndex++ )
                {
                    Ptr<flashgg::Electron> ele = Electrons[eleIndex];

                    if(ele->pt()>leadElePt)
                    {
                        leadElePt = ele->pt();
                        leadEleIndex = eleIndex;
                    }
                }

                if(leadMuPt>=leadElePt)
                {
                    lepton_leadPt_ = Muons[leadMuIndex]->pt();
                    lepton_leadEta_ = Muons[leadMuIndex]->eta();
                }
                else
                {
                    lepton_leadPt_ = Electrons[leadEleIndex]->pt();
                    lepton_leadEta_ = Electrons[leadEleIndex]->eta();
                }

                float mvaValue = DiphotonMva_-> EvaluateMVA( "BDT" );

                tthMvaVal_RunII_ = convert_tmva_to_prob(TThMva_RunII_->EvaluateMVA( "BDT" ));
                if (debug_) {
                  cout << "TTH Leptonic Tag -- input MVA variables for Run II MVA: " << endl;
                  cout << "--------------------------------------------------------" << endl;
                  cout << "maxIDMVA_: " << maxPhoID_ << endl;
                  cout << "minIDMVA_: " << minPhoID_ << endl;
                  cout << "max2_btag_: " << secondMaxBTagVal_noBB_ << endl;
                  cout << "max1_btag_: " << maxBTagVal_noBB_ << endl;
                  cout << "dipho_delta_R_: " << diPhoDeltaR_ << endl;

                  cout << "njets_: " << nJets_ << endl;
                  cout << "ht_: " << ht_ << endl;
                  cout << "leadptoM_: " << leadptom_ << endl;
                  cout << "subleadptoM_: " << subleadptom_ << endl;
                  cout << "lead_eta_: " << leadeta_ << endl;
                  cout << "sublead_eta_: " << subleadeta_ << endl;

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

                  cout << "leadPSV_: " << leadPSV_ << endl;
                  cout << "subleadPSV_: " << subleadPSV_ << endl;

                  cout << "dipho_cosphi_: " << diPhoCosPhi_ << endl;
                  cout << "dipho_rapidity_: " << diPhoY_ << endl;
                  cout << "met_: " << MetPt_ << endl;
                  cout << "dipho_pt_over_mass_: " << diPhoPtoM_ << endl;
                  cout << "helicity_angle_: " << helicity_angle_ << endl;

                  cout << "lep_pt_: " << lepton_leadPt_ << endl;
                  cout << "lep_eta_: " << lepton_leadEta_ << endl;
                  cout << "n_lep_tight_: " << lepton_nTight_ << endl;

                  cout << "lep1_charge: " << lep1_charge << endl;
                  cout << "lep2_charge: " << lep2_charge << endl;
                  cout << "forward_jet_eta: " << forward_jet_eta << endl;
                  cout << "forward_jet_pt: " << forward_jet_pt << endl;

                  cout << "ttH vs tH DNN Score: " << ttH_vs_tH_dnn_score << endl;

                  cout << "DNN Score 0: " << dnn_score_0_ << endl;
                  cout << endl;
                  cout << "BDT Score: " << tthMvaVal_RunII_ << endl;
              }

              if (ttH_vs_tH_dnn_score < tthVstHThreshold_) { 
                  if (debug_)
                      cout << "Rejecting event because ttH_vs_tH_dnn_score of " << ttH_vs_tH_dnn_score << " is below threshold." << endl;
                  continue;
              }

              global_features.clear();

              mvaValue = tthMvaVal_RunII_; // use Run II MVA for categorization

                //int catNumber = -1;
                //catNumber = chooseCategory( mvaValue , debug_);  
                int catNumber_pt = -1;
                catNumber_pt = chooseCategory_pt( mvaValue , dipho->pt());  

                if(debug_)
                    cout << "I'm going to check selections, mva value: " << mvaValue << endl;

                // Disable splitting of single lepton and dilepton events: new optimimization of signal regions considers them together
                /*
                if(SplitDiLeptEv_ && lepPt.size()>1 && njet_ >= DiLeptonJetThreshold_ && njets_btagmedium_ >= DiLeptonbJetThreshold_ && mvaValue > DiLeptonMVAThreshold_ ) 
                // Check DiLepton selection and assigne to purest cat if splitting is True
                {    catNumber = 0;
                     if(debug_)
                        cout << "DiLepton event with: " << njet_ << "jets, (threshold " << DiLeptonJetThreshold_ << ") " << njets_btagmedium_ << " bjets, (threshold " << DiLeptonbJetThreshold_  << ")" << mvaValue << " mva (threshold " << DiLeptonMVAThreshold_ << endl;

                }
                
                else if(lepPt.size()==1 && njet_ >= jetsNumberThreshold_ && njets_btagmedium_ >= bjetsNumberThreshold_) 
                // Check single lepton selections
                {    catNumber = chooseCategory( mvaValue, debug_ );  
                   if(debug_)
                        cout << "Single lepton event with: "<< njet_ << " jets, (threshold " << DiLeptonJetThreshold_ << ") " << njets_btagmedium_ << " bjets, (threshold " << DiLeptonbJetThreshold_  << ")" << mvaValue << " mva (thresholds "  << endl;
                }
                */

                if(debug_)
                { 
                    cout << "TTHLeptonicTag -- MVA iput variables: " << endl;
                    cout << "--------------------------------------" << endl;
                    cout << "Lead and sublead photon eta " << leadeta_ << " " << subleadeta_ << endl;
                    cout << "Lead and sublead photon pt/m " << leadptom_ << " " << subleadptom_ << endl;
                    cout << "Lead and sublead photon IdMVA " << leadIDMVA_ << " " << subleadIDMVA_ << endl;
                    cout << "Lead and sublead photon PSV " << leadPSV_ << " " << subleadPSV_ << endl;
                    cout << "Photon delta phi " << deltaphi_ << endl;
                    cout << "Number of jets " << nJets_ << endl;
                    cout << "Number of b-jets " << nJets_bTagMedium_  << endl;
                    cout << "Pt of the three leading jets " << jet_pt1_ << " " << jet_pt2_ << " " << jet_pt3_ << endl;
                    cout << "Eta of the three leading jets " << jet_eta1_ << " " << jet_eta2_ << " " << jet_eta3_ << endl;
                    cout << "Two highest bTag scores " << bTag1_ << " " << bTag2_ << endl;
                    cout << "MetPt " << MetPt_ << endl;
                    cout << "Lepton pT and Eta " << lepton_leadPt_ << " " << lepton_leadEta_ << endl;
                    cout << "--------------------------------------" << endl;
                    cout << "TTHLeptonicTag -- output MVA value " << mvaValue << " " << DiphotonMva_-> EvaluateMVA( "BDT" ) << ", category " << catNumber_pt << endl;
                }

                if(catNumber_pt!=-1)
                {
                    TTHLeptonicTag tthltags_obj( dipho, mvares );
                    tthltags_obj.setCategoryNumber(catNumber_pt);
                    //tthltags_obj.setCategoryNumber(catNumber);

                    int chosenTag = computeStage1Kinematics( tthltags_obj );
                    tthltags_obj.setStage1recoTag( chosenTag );

                    for( unsigned int i = 0; i < tagJets.size(); ++i )
                    {
                        tthltags_obj.includeWeightsByLabel( *tagJets[i] , "JetBTagReshapeWeight");
                    }


                    for( unsigned int i = 0; i < Muons.size(); ++i )
                        tthltags_obj.includeWeights( *Muons.at(i));

                    for( unsigned int i = 0; i < Electrons.size(); ++i )
                        tthltags_obj.includeWeights( *Electrons.at(i));

                    tthltags_obj.includeWeights( *dipho );
                    tthltags_obj.setJets( tagJets );
                    tthltags_obj.setMuons( Muons );
                    tthltags_obj.setElectrons( Electrons );
                    tthltags_obj.setDiPhotonIndex( diphoIndex );
                    std::string syst_label = modifySystematicsWorkflow ? systematicsLabels[syst_idx] : systLabel_;
                    tthltags_obj.setSystLabel( syst_label );
                    tthltags_obj.setMvaRes(mvaValue);
                    tthltags_obj.setLepPt( lepPt );
                    tthltags_obj.setLepE( lepE );
                    tthltags_obj.setLepEta( lepEta );
                    tthltags_obj.setLepPhi( lepPhi );
                    tthltags_obj.setLepType( lepType );

                    tthltags_obj.setLeadPrompt(-999);
                    tthltags_obj.setLeadMad(-999);
                    tthltags_obj.setLeadPythia(-999);
                    tthltags_obj.setLeadPassFrix(-999);
                    tthltags_obj.setLeadSimpleMomID(-999);
                    tthltags_obj.setLeadSimpleMomStatus(-999);
                    tthltags_obj.setLeadMomID(-999);
                    tthltags_obj.setLeadMomMomID(-999);
                    tthltags_obj.setLeadSmallestDr(-999);

                    tthltags_obj.setSubleadPrompt(-999);
                    tthltags_obj.setSubleadMad(-999);
                    tthltags_obj.setSubleadPythia(-999);
                    tthltags_obj.setSubleadPassFrix(-999);
                    tthltags_obj.setSubleadSimpleMomID(-999);
                    tthltags_obj.setSubleadSimpleMomStatus(-999);
                    tthltags_obj.setSubleadMomID(-999);
                    tthltags_obj.setSubleadMomMomID(-999);
                    tthltags_obj.setSubleadSmallestDr(-999);
                                
                    tthltags->push_back( tthltags_obj );
            
                    if( ! evt.isRealData() )
                    {
                        evt.getByToken( genParticleToken_, genParticles );
                        int gp_lead_index = GenPhoIndex(genParticles, dipho->leadingPhoton(), -1);
                        int gp_sublead_index = GenPhoIndex(genParticles, dipho->subLeadingPhoton(), gp_lead_index);
                        vector<int> leadFlags; leadFlags.clear();
                        vector<int> subleadFlags; subleadFlags.clear();

                        if (gp_lead_index != -1) {
                           const edm::Ptr<reco::GenParticle> gp_lead = genParticles->ptrAt(gp_lead_index);                
                           leadFlags = IsPromptAfterOverlapRemove(genParticles, gp_lead);
                           tthltags->back().setLeadPrompt(leadFlags[0]);
                           tthltags->back().setLeadMad(leadFlags[1]);
                           tthltags->back().setLeadPythia(leadFlags[2]);
                           tthltags->back().setLeadPassFrix(leadFlags[3]);
                           tthltags->back().setLeadSimpleMomID(leadFlags[4]);
                           tthltags->back().setLeadSimpleMomStatus(leadFlags[5]);
                           tthltags->back().setLeadMomID(leadFlags[6]);
                           tthltags->back().setLeadMomMomID(leadFlags[7]);
                           tthltags->back().setLeadSmallestDr(NearestDr(genParticles, &(*gp_lead)));
                           } 

                       if (gp_sublead_index != -1) {
                           const edm::Ptr<reco::GenParticle> gp_sublead = genParticles->ptrAt(gp_sublead_index);
                           subleadFlags = IsPromptAfterOverlapRemove(genParticles, gp_sublead);
                           tthltags->back().setSubleadPrompt(subleadFlags[0]);
                           tthltags->back().setSubleadMad(subleadFlags[1]);
                           tthltags->back().setSubleadPythia(subleadFlags[2]);
                           tthltags->back().setSubleadPassFrix(subleadFlags[3]);
                           tthltags->back().setSubleadSimpleMomID(subleadFlags[4]);
                           tthltags->back().setSubleadSimpleMomStatus(subleadFlags[5]);
                           tthltags->back().setSubleadMomID(subleadFlags[6]);
                           tthltags->back().setSubleadMomMomID(subleadFlags[7]);
                           tthltags->back().setSubleadSmallestDr(NearestDr(genParticles, &(*gp_sublead)));
                           }

                    }
                }
            } //diPho loop end !
            evt.put( std::move( tthltags ), systematicsLabels[syst_idx] );
        } // syst loop end !
    }

    int TTHLeptonicTagProducer::computeStage1Kinematics( const TTHLeptonicTag tag_obj )
    {
        int chosenTag_ = DiPhotonTagBase::stage1recoTag::LOGICERROR;
        int catNum = tag_obj.categoryNumber();
        if ( catNum == 0 ) {
            chosenTag_ = DiPhotonTagBase::stage1recoTag::RECO_TTH_LEP_PTH_0_60_Tag0;
        }
        else if ( catNum == 1 ) {
            chosenTag_ = DiPhotonTagBase::stage1recoTag::RECO_TTH_LEP_PTH_0_60_Tag1;
        }
        else if ( catNum == 2 ) {
            chosenTag_ = DiPhotonTagBase::stage1recoTag::RECO_TTH_LEP_PTH_0_60_Tag2;
        }
        else if ( catNum == 3 ) {
            chosenTag_ = DiPhotonTagBase::stage1recoTag::RECO_TTH_LEP_PTH_0_60_Tag3;
        }
        else if ( catNum == 4 ) {
            chosenTag_ = DiPhotonTagBase::stage1recoTag::RECO_TTH_LEP_PTH_60_120_Tag0;
        }
        else if ( catNum == 5 ) {
            chosenTag_ = DiPhotonTagBase::stage1recoTag::RECO_TTH_LEP_PTH_60_120_Tag1;
        }
        else if ( catNum == 6 ) {
            chosenTag_ = DiPhotonTagBase::stage1recoTag::RECO_TTH_LEP_PTH_120_200_Tag0;
        }
        else if ( catNum == 7 ) {
            chosenTag_ = DiPhotonTagBase::stage1recoTag::RECO_TTH_LEP_PTH_120_200_Tag1;
        }
        else if ( catNum == 8 ) {
            chosenTag_ = DiPhotonTagBase::stage1recoTag::RECO_TTH_LEP_PTH_GT200_Tag0;
        }
        else if ( catNum == 9 ) {
            chosenTag_ = DiPhotonTagBase::stage1recoTag::RECO_TTH_LEP_PTH_GT200_Tag1;
        }
        return chosenTag_;
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
