// The purpose of this plugin is to tag HH->WWgg events

#include "stdlib.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/EDMException.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

#include "FWCore/Common/interface/TriggerNames.h"
#include "flashgg/DataFormats/interface/DiPhotonCandidate.h"
#include "flashgg/DataFormats/interface/SinglePhotonView.h"
#include "flashgg/DataFormats/interface/Photon.h"
#include "flashgg/DataFormats/interface/Electron.h"
#include "flashgg/DataFormats/interface/Muon.h"
#include "flashgg/DataFormats/interface/Met.h"
#include "flashgg/DataFormats/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "flashgg/DataFormats/interface/DiPhotonMVAResult.h"
#include "flashgg/DataFormats/interface/HHWWggTag.h"
#include "flashgg/DataFormats/interface/TagTruthBase.h"
#include "DataFormats/Common/interface/RefToPtr.h"
#include "DataFormats/Math/interface/deltaR.h"
#include "DataFormats/HepMCCandidate/interface/GenParticleFwd.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"

#include "flashgg/MicroAOD/interface/CutBasedDiPhotonObjectSelector.h"

#include "SimDataFormats/HTXS/interface/HiggsTemplateCrossSections.h"
#include "flashgg/DataFormats/interface/WHLeptonicTag.h"

#include "flashgg/Taggers/interface/LeptonSelection.h"

#include <vector>
#include <algorithm>
#include "TGraph.h"
#include "TLorentzVector.h"

using namespace std;
using namespace edm;

namespace flashgg {
  inline bool compEle(const edm::Ptr<flashgg::Electron>& a, const edm::Ptr<flashgg::Electron>& b);
  inline bool compMu(const edm::Ptr<flashgg::Muon>& a, const edm::Ptr<flashgg::Muon>& b);
  class HHWWggTagProducer : public EDProducer
  {
  public:
    //---typedef
    typedef math::XYZTLorentzVector LorentzVector;

    //---ctors
    // HHWWggTagProducer();
    HHWWggTagProducer( const ParameterSet & );

    //---Outtree
    edm::Service<TFileService> fs;

    // TH1F* indexes;

  private:
    double genTotalWeight;
    bool checkPassMVAs(const flashgg::Photon*& leading_photon, const flashgg::Photon*& subleading_photon, edm::Ptr<reco::Vertex>& diphoton_vertex ,double EB_MVA,double EE_MVA);
    std::vector<double> GetMuonVars(const std::vector<edm::Ptr<flashgg::Muon> > &muonPointers, const std::vector<edm::Ptr<reco::Vertex> > &vertexPointers);
    std::vector<double> GetJetVars(const std::vector<edm::Ptr<flashgg::Jet> > &jetPointers, const edm::Ptr<flashgg::DiPhotonCandidate> dipho);
    vector<Ptr<flashgg::Jet>> GetFHminWHJets(bool doHHWWggDebug_, std::vector<edm::Ptr<Jet> > tagJets_);
    void produce( Event &, const EventSetup & ) override;
    std::vector<edm::EDGetTokenT<edm::View<DiPhotonCandidate> > > diPhotonTokens_;
    std::string inputDiPhotonName_;

    std::string inputJetsName_;
    std::vector<std::string> inputJetsSuffixes_;
    unsigned int inputJetsCollSize_;

    // Adding Jets
    std::vector<edm::EDGetTokenT<edm::View<flashgg::Jet> > > jetTokens_;

    EDGetTokenT<View<Photon> > photonToken_;
    Handle<View<flashgg::Photon> > photons;

    EDGetTokenT<View<DiPhotonCandidate> > diphotonToken_;
    Handle<View<flashgg::DiPhotonCandidate> > diphotons;

    EDGetTokenT<View<reco::Vertex> > vertexToken_;
    Handle<View<reco::Vertex> > vertex;

    EDGetTokenT<View<reco::GenParticle> > genParticleToken_;
    Handle<View<reco::GenParticle> > genParticle;

    EDGetTokenT<View<Electron> > electronToken_;
    Handle<View<flashgg::Electron> > electrons;

    EDGetTokenT<View<Muon> > muonToken_;
    Handle<View<flashgg::Muon> > muons;

    EDGetTokenT<View<flashgg::Met> > METToken_;
    Handle<View<flashgg::Met> > METs;

    EDGetTokenT<View<DiPhotonMVAResult> > mvaResultToken_;
    Handle<View<flashgg::DiPhotonMVAResult> > mvaResults;

    Handle<View<reco::Vertex> > vertices;

    EDGetTokenT<double> rhoTag_;
    edm::EDGetTokenT<edm::TriggerResults> triggerRECO_;
    edm::EDGetTokenT<edm::TriggerResults> triggerPAT_;
    edm::EDGetTokenT<edm::TriggerResults> triggerFLASHggMicroAOD_;
    string systLabel_;
    edm::Handle<double>  rho;

    std::vector< std::string > systematicsLabels;
    std::vector<std::string> inputDiPhotonSuffixes_;

    //---ID selector
    ConsumesCollector cc_;
    GlobalVariablesComputer globalVariablesComputer_;
    // CutBasedDiPhotonObjectSelector idSelector_;

    //----output collection
    // auto_ptr<vector<HHWWggCandidate> > HHWWggColl_;

    // variables from WHLeptonicTagProducer
    double EB_MVA_Threshold_;
    double EE_MVA_Threshold_;
    double MetPtThreshold_;
    double deltaRLeps_;
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
    double deltaMassElectronZ_FL_Threshold_;
    bool hasGoodElec = false;
    bool hasGoodMuons = false;

    vector<double> nonTrigMVAThresholds_;
    vector<double> nonTrigMVAEtaCuts_;

    double electronIsoThreshold_;
    double electronNumOfHitsThreshold_;
    vector<double> electronEtaThresholds_;
    bool useElectronMVARecipe_;
    bool useElectronLooseID_;
    // string bTag_;
    double btagThresh_;
    bool doHHWWggTagCutFlowAnalysis_;
    bool doHHWWggNonResAnalysis_;
    bool doHHWWggFHptOrdered_;
    bool doHHWWggDebug_;
    string HHWWggAnalysisChannel_;

    //full-lep
    bool SaveOthers_;
    bool Savejet;
    double lep1ptThre_;
    double lep2ptThre_;
    double lep3ptThre_;
    double DiLepMassThre_;
    double DiLepPtThre_;
    double MassTThre_;
    double MassT_l2Thre_;
    edm::InputTag genInfo_;
    edm::EDGetTokenT<GenEventInfoProduct> genInfoToken_;
  };

    //---standard
    HHWWggTagProducer::HHWWggTagProducer( const ParameterSet & pSet):
    photonToken_( consumes<View<Photon> >( pSet.getParameter<InputTag> ( "PhotonTag" ) ) ),
    diphotonToken_( consumes<View<flashgg::DiPhotonCandidate> >( pSet.getParameter<InputTag> ( "DiPhotonTag" ) ) ),
    vertexToken_( consumes<View<reco::Vertex> >( pSet.getParameter<InputTag> ( "VertexTag" ) ) ),
    genParticleToken_( consumes<View<reco::GenParticle> >( pSet.getParameter<InputTag> ( "GenParticleTag" ) ) ),
    electronToken_( consumes<View<Electron> >( pSet.getParameter<InputTag> ( "ElectronTag" ) ) ),
    muonToken_( consumes<View<Muon> >( pSet.getParameter<InputTag> ( "MuonTag" ) ) ),
    METToken_( consumes<View<Met> >( pSet.getParameter<InputTag> ( "METTag" ) ) ),
    mvaResultToken_( consumes<View<flashgg::DiPhotonMVAResult> >( pSet.getParameter<InputTag> ( "MVAResultTag" ) ) ),
    rhoTag_( consumes<double>( pSet.getParameter<InputTag>( "rhoTag" ) ) ),
    triggerRECO_( consumes<edm::TriggerResults>(pSet.getParameter<InputTag>("RECOfilters") ) ),
    triggerPAT_( consumes<edm::TriggerResults>(pSet.getParameter<InputTag>("PATfilters") ) ),
    triggerFLASHggMicroAOD_( consumes<edm::TriggerResults>( pSet.getParameter<InputTag>("FLASHfilters") ) ),
    systLabel_( pSet.getParameter<string> ( "SystLabel" ) ),
    cc_( consumesCollector() ),
    globalVariablesComputer_(pSet.getParameter<edm::ParameterSet>("globalVariables"), cc_) // need absence of comma on last entry

    {
      inputDiPhotonName_= pSet.getParameter<std::string > ( "DiPhotonName" );
      inputDiPhotonSuffixes_= pSet.getParameter<std::vector<std::string> > ( "DiPhotonSuffixes" );
      std::vector<edm::InputTag>  diPhotonTags;
      for (auto & suffix : inputDiPhotonSuffixes_) {
          systematicsLabels.push_back(suffix);
          std::string inputName = inputDiPhotonName_;
          inputName.append(suffix);
          if (!suffix.empty()) diPhotonTags.push_back(edm::InputTag(inputName));
          else  diPhotonTags.push_back(edm::InputTag(inputDiPhotonName_));
      }
      for ( auto & tag : diPhotonTags ) { diPhotonTokens_.push_back( consumes<edm::View<flashgg::DiPhotonCandidate> >( tag ) ); }

      bool breg = 0;

      inputJetsName_= pSet.getParameter<std::string> ( "JetsName" );
      inputJetsCollSize_= pSet.getParameter<unsigned int> ( "JetsCollSize" );
      inputJetsSuffixes_= pSet.getParameter<std::vector<std::string> > ( "JetsSuffixes" );
      // cout << "inputJetsCollSize_ = " << inputJetsCollSize_ << endl;
      if (breg){
        std::vector<edm::InputTag>  jetTags; // With bregression on
        for (auto & suffix : inputJetsSuffixes_) {
            if (!suffix.empty()) systematicsLabels.push_back(suffix);  //nominal is already put in the diphoton loop
            for (unsigned int i = 0; i < inputJetsCollSize_ ; i++) {
                  std::string bregtag = suffix;
                  bregtag.append(std::to_string(i));
                  if (breg) jetTags.push_back(edm::InputTag(inputJetsName_,bregtag)); // With bregression on
            }
        }

        for( auto & tag : jetTags ) { jetTokens_.push_back( consumes<edm::View<flashgg::Jet> >( tag ) ); } // With bregression on
      }

      // Jets without bregression
      if (!breg){
        auto jetTags = pSet.getParameter<std::vector<edm::InputTag> > ( "JetTags" );
        for( auto & tag : jetTags ) { jetTokens_.push_back( consumes<edm::View<flashgg::Jet> >( tag ) ); }
      }

      genInfo_ = pSet.getUntrackedParameter<edm::InputTag>( "genInfo", edm::InputTag("generator") );
      genInfoToken_ = consumes<GenEventInfoProduct>( genInfo_ );
      // numDiphoCand = fs->make<TH1F> ("numDiphoCand","numDiphoCand",10,0,10);

      // WTags = fs->make<TH1F> ("WTags","W Tags",3,0,3);
      EB_MVA_Threshold_ =pSet.getParameter<double>( "EB_MVA_Threshold");
      EE_MVA_Threshold_ =pSet.getParameter<double>( "EE_MVA_Threshold");
      MetPtThreshold_ =pSet.getParameter<double>( "MetPtThreshold");
      deltaRLeps_ =pSet.getParameter<double>( "deltaRLeps");
      leptonPtThreshold_ = pSet.getParameter<double>( "leptonPtThreshold");
      muonEtaThreshold_ = pSet.getParameter<double>( "muonEtaThreshold");
      leadPhoOverMassThreshold_ = pSet.getParameter<double>( "leadPhoOverMassThreshold");
      subleadPhoOverMassThreshold_ = pSet.getParameter<double>( "subleadPhoOverMassThreshold");
      MVAThreshold_ = pSet.getParameter<double>( "MVAThreshold");
      deltaRMuonPhoThreshold_ = pSet.getParameter<double>( "deltaRMuonPhoThreshold");
      jetsNumberThreshold_ = pSet.getParameter<double>( "jetsNumberThreshold");
      jetPtThreshold_ = pSet.getParameter<double>( "jetPtThreshold");
      jetEtaThreshold_ = pSet.getParameter<double>( "jetEtaThreshold");
      muPFIsoSumRelThreshold_ = pSet.getParameter<double>( "muPFIsoSumRelThreshold");
      PhoMVAThreshold_ = pSet.getParameter<double>( "PhoMVAThreshold");
      METThreshold_ = pSet.getParameter<double>( "METThreshold");
      useVertex0only_              = pSet.getParameter<bool>("useVertex0only");
      deltaRJetMuonThreshold_ = pSet.getParameter<double>( "deltaRJetMuonThreshold");
      deltaRPhoLeadJet_ = pSet.getParameter<double>( "deltaRPhoLeadJet");
      deltaRPhoSubLeadJet_ = pSet.getParameter<double>( "deltaRPhoSubLeadJet");

      DeltaRTrkElec_ = pSet.getParameter<double>( "DeltaRTrkElec");
      TransverseImpactParam_ = pSet.getParameter<double>( "TransverseImpactParam");
      LongitudinalImpactParam_ = pSet.getParameter<double>( "LongitudinalImpactParam");

      deltaRPhoElectronThreshold_ = pSet.getParameter<double>( "deltaRPhoElectronThreshold");
      deltaMassElectronZThreshold_ = pSet.getParameter<double>( "deltaMassElectronZThreshold");
      deltaMassElectronZ_FL_Threshold_ = pSet.getParameter<double>( "deltaMassElectronZ_FL_Threshold");
      nonTrigMVAThresholds_ =  pSet.getParameter<vector<double > >( "nonTrigMVAThresholds");
      nonTrigMVAEtaCuts_ =  pSet.getParameter<vector<double > >( "nonTrigMVAEtaCuts");
      electronIsoThreshold_ = pSet.getParameter<double>( "electronIsoThreshold");
      electronNumOfHitsThreshold_ = pSet.getParameter<double>( "electronNumOfHitsThreshold");
      electronEtaThresholds_ = pSet.getParameter<vector<double > >( "electronEtaThresholds");
      useElectronMVARecipe_=pSet.getParameter<bool>("useElectronMVARecipe");
      useElectronLooseID_=pSet.getParameter<bool>("useElectronLooseID");
      // bTag_ = pSet.getParameter<string> ( "bTag");
      btagThresh_ = pSet.getParameter<double>( "btagThresh");
      doHHWWggTagCutFlowAnalysis_ = pSet.getParameter<bool>( "doHHWWggTagCutFlowAnalysis");
      doHHWWggNonResAnalysis_ = pSet.getParameter<bool>( "doHHWWggNonResAnalysis" );
      doHHWWggFHptOrdered_ = pSet.getParameter<bool>( "doHHWWggFHptOrdered" );
      doHHWWggDebug_ = pSet.getParameter<bool>( "doHHWWggDebug" );
      HHWWggAnalysisChannel_ = pSet.getParameter<string>( "HHWWggAnalysisChannel" );
      SaveOthers_ = pSet.getParameter<bool>("SaveOthers");
      lep1ptThre_ = pSet.getParameter<double>("lep1ptThre");//means > lep1pt
      lep2ptThre_ = pSet.getParameter<double>("lep2ptThre");//means > lep2pt
      lep3ptThre_ = pSet.getParameter<double>("lep3ptThre");//means < lep3pt
      DiLepPtThre_ = pSet.getParameter<double>("DiLepPtThre");
      DiLepMassThre_ = pSet.getParameter<double>("DiLepMassThre");
      MassTThre_ = pSet.getParameter<double>("MassTThre");
      MassT_l2Thre_ = pSet.getParameter<double>("MassT_l2Thre");
      produces<vector<HHWWggTag>>();
      // for (auto & systname : systematicsLabels) { // to deal with systematics in producer
      //     produces<vector<HHWWggTag>>(systname);
      // }
      produces<vector<TagTruthBase>>();
    }



    bool compEle(const edm::Ptr<flashgg::Electron>& a, const edm::Ptr<flashgg::Electron>& b)
              {
              return a->pt() > b->pt();
              }

    bool compMu(const edm::Ptr<flashgg::Muon>& a, const edm::Ptr<flashgg::Muon>& b)
              {
              return a->pt() > b->pt();
              }
    bool HHWWggTagProducer::checkPassMVAs( const flashgg::Photon*& leading_photon, const flashgg::Photon*& subleading_photon, edm::Ptr<reco::Vertex>& diphoton_vertex,double EB_MVA,double EE_MVA){

      // MVA Check variables
      bool lead_pass_TightPhoID = 0, sublead_pass_TightPhoID = 0;
      double lp_Hgg_MVA = -99, slp_Hgg_MVA = -99;
      double leading_pho_eta = -99, sub_leading_pho_eta = -99;

      // Get MVA values wrt diphoton vertex
      lp_Hgg_MVA = leading_photon->phoIdMvaDWrtVtx( diphoton_vertex );
      slp_Hgg_MVA = subleading_photon->phoIdMvaDWrtVtx( diphoton_vertex );
    cout<<"lp_Hgg_MVA,"<<lp_Hgg_MVA<<"slp_Hgg_MVA,"<<slp_Hgg_MVA<<endl;
      // Get eta values
      leading_pho_eta = leading_photon->p4().eta();
      sub_leading_pho_eta = subleading_photon->p4().eta();
      // Debug
      // if(debug_mva){
      //   cout << "leading mva: " << lp_Hgg_MVA << endl;
      //   cout << "subleading mva: " << slp_Hgg_MVA << endl;
      // }
      // leading photon
      // EB
      if (( abs(leading_pho_eta) > 0) && ( abs(leading_pho_eta) < 1.4442)){
        // if (lead_pho_EG_MVA_ > 0.42) lead_pass_TightPhoID = 1;
        if (lp_Hgg_MVA > EB_MVA) lead_pass_TightPhoID = 1;
        cout<<"EB lp"<<EB_MVA<<","<<lead_pass_TightPhoID<<endl;
      }

      // EE
      else if (( abs(leading_pho_eta) > 1.566) && ( abs(leading_pho_eta) < 2.5)){
        // if (lead_pho_EG_MVA_ > 0.14) lead_pass_TightPhoID = 1;
        if (lp_Hgg_MVA > EE_MVA) lead_pass_TightPhoID = 1;
        cout<<"EE lp"<<EE_MVA<<","<<lead_pass_TightPhoID<<endl;
      }

      // SubLeading Photon
      // EB
      if (( abs(sub_leading_pho_eta) > 0) && ( abs(sub_leading_pho_eta) < 1.4442)){
        // if (sublead_pho_EG_MVA_ > 0.42) sublead_pass_TightPhoID = 1;
        if (slp_Hgg_MVA > EB_MVA) sublead_pass_TightPhoID = 1;
        cout<<"EB slp"<<EB_MVA<<","<<sublead_pass_TightPhoID<<endl;
      }

      // EE
      else if (( abs(sub_leading_pho_eta) > 1.566) && ( abs(sub_leading_pho_eta) < 2.5)){
        // if (sublead_pho_EG_MVA_ > 0.14) sublead_pass_TightPhoID = 1;
        if (slp_Hgg_MVA > EE_MVA) sublead_pass_TightPhoID = 1;
        cout<<"EE slp"<<EB_MVA<<","<<sublead_pass_TightPhoID<<endl;
      }

      if (lead_pass_TightPhoID && sublead_pass_TightPhoID){
        // if(debug_mva) cout << "PASS MVA Selections" << endl;
        return 1;
    }

    else{
      // if(debug_mva) cout << "FAIL MVA Selections" << endl;
      return 0;
    }

    }

    std::vector<double> HHWWggTagProducer::GetMuonVars(const std::vector<edm::Ptr<flashgg::Muon> > &muonPointers, const std::vector<edm::Ptr<reco::Vertex> > &vertexPointers)
    {
      unsigned int maxMuons = 5; // Shouldn't need more than this
      unsigned int numVars = 6; // 5 IDs + isolation
      unsigned int numVecEntries = maxMuons * numVars;
      std::vector<double> MuonVars_(numVecEntries,-999); // initialize vector with -999 vals
      double isLooseMuon = -999, isMediumMuon = -999, isTightMuon = -999, isSoftMuon = -999, isHighPtMuon = -999;
      double muonIso = -999;
      int vtxInd = 0;
      double dzmin = 9999;

      for( unsigned int muonIndex = 0; muonIndex < muonPointers.size(); muonIndex++ ) {
          if(muonIndex >= maxMuons) continue; // only save info from 5 highest pT muon objects
          isLooseMuon = -999, isMediumMuon = -999, isTightMuon = -999, isSoftMuon = -999, isHighPtMuon = -999;
          muonIso = -999;
          Ptr<flashgg::Muon> muon = muonPointers[muonIndex];
          vtxInd = 0;
          dzmin = 9999;
          // If no innertrack, set medium, tight, soft, highpt vals to -999 as they can't be calculated without it
          // I think this is correct because there are non-zero isolation values when isTightMuon is -999
          if( !muon->innerTrack() ){
            isLooseMuon = muon::isLooseMuon( *muon );
            muonIso = ( muon->pfIsolationR04().sumChargedHadronPt
                                      + max( 0.,muon->pfIsolationR04().sumNeutralHadronEt
                                            + muon->pfIsolationR04().sumPhotonEt - 0.5 * muon->pfIsolationR04().sumPUPt ) ) / ( muon->pt() );
            MuonVars_[muonIndex*numVars + 0] = isLooseMuon;
            MuonVars_[muonIndex*numVars + 1] = isMediumMuon;
            MuonVars_[muonIndex*numVars + 2] = isTightMuon;
            MuonVars_[muonIndex*numVars + 3] = isSoftMuon;
            MuonVars_[muonIndex*numVars + 4] = isHighPtMuon;
            MuonVars_[muonIndex*numVars + 5] = muonIso;
            continue;
          }
          for( size_t ivtx = 0 ; ivtx < vertexPointers.size(); ivtx++ ) {
              Ptr<reco::Vertex> vtx = vertexPointers[ivtx];
              if( fabs( muon->innerTrack()->vz() - vtx->position().z() ) < dzmin ) {
                  dzmin = fabs( muon->innerTrack()->vz() - vtx->position().z() );
                  vtxInd = ivtx;
              }
          }
          Ptr<reco::Vertex> best_vtx = vertexPointers[vtxInd];
          isLooseMuon = muon::isLooseMuon( *muon );
          isMediumMuon = muon::isMediumMuon( *muon );
          isTightMuon = muon::isTightMuon( *muon, *best_vtx );
          isSoftMuon = muon::isSoftMuon( *muon, *best_vtx );
          isHighPtMuon = muon::isHighPtMuon( *muon, *best_vtx );

          muonIso = ( muon->pfIsolationR04().sumChargedHadronPt
                                    + max( 0.,muon->pfIsolationR04().sumNeutralHadronEt
                                          + muon->pfIsolationR04().sumPhotonEt - 0.5 * muon->pfIsolationR04().sumPUPt ) ) / ( muon->pt() );

            MuonVars_[muonIndex*numVars + 0] = isLooseMuon;
            MuonVars_[muonIndex*numVars + 1] = isMediumMuon;
            MuonVars_[muonIndex*numVars + 2] = isTightMuon;
            MuonVars_[muonIndex*numVars + 3] = isSoftMuon;
            MuonVars_[muonIndex*numVars + 4] = isHighPtMuon;
            MuonVars_[muonIndex*numVars + 5] = muonIso;

          // if(isLooseMuon==0 && isTightMuon==1){
          //   cout << "--------------------------------" << endl;
          //   cout << "Muon passes tight but not loose" << endl;
          //   cout << "muon->isPFMuon():" << muon->isPFMuon() << endl;
          //   cout << "muon->isGlobalMuon():" << muon->isGlobalMuon() << endl;
          //   cout << "muon->isTrackerMuon():" << muon->isTrackerMuon() << endl;
          // }
      }

      return MuonVars_;
    }

    std::vector<double> HHWWggTagProducer::GetJetVars(const std::vector<edm::Ptr<flashgg::Jet> > &jetPointers, const edm::Ptr<flashgg::DiPhotonCandidate> dipho)
    {
      unsigned int maxJets = 5; // Shouldn't need more than this
      unsigned int numVars = 4; // 4 IDs + Jet PU ID
      // unsigned int numVars = 5; // 4 IDs + Jet PU ID
      // unsigned int numVars = 12; // 4 IDs + 8 PUjetIDs
      unsigned int numVecEntries = maxJets * numVars;
      std::vector<double> JetVars_(numVecEntries,-999); // initialize vector with -999 vals
      double passLoose = -999, passTight = -999, passTight2017 = -999, passTight2018 = -999;
      // double passesJetPUIdLoose = -999;
      // double passesJetPuIdnone = -999, passesJetPuIdloose = -999, passesJetPuIdmedium = -999, passesJetPuIdtight = -999;
      // double passesJetPuIdmixed = -999, passesJetPuIdforward_loose = -999, passesJetPuIdforward_medium = -999, passesJetPuIdforward_tight = -999;

      for( unsigned int jetIndex = 0; jetIndex < jetPointers.size(); jetIndex++ ) {
          if(jetIndex >= maxJets) continue; // only save info from 5 highest pT Jet objects
          passLoose = -999, passTight = -999, passTight2017 = -999, passTight2018 = -999;
          Ptr<flashgg::Jet> jet = jetPointers[jetIndex];

          // passesJetPuId
          // one=0, loose=1, medium=2, tight=3, mixed=4, forward_loose=5, forward_medium=6, forward_tight=7

          // Loose=0, Tight=1, Tight2017=2, Tight2018=3
          passLoose = jet->passesJetID  ( flashgg::Loose );
          passTight = jet->passesJetID  ( flashgg::Tight );
          passTight2017 = jet->passesJetID  ( flashgg::Tight2017 );
          passTight2018 = jet->passesJetID  ( flashgg::Tight2018 );

          // passesJetPUIdLoose = jet->passesPuJetId(dipho, PileupJetIdentifier::kLoose);

          // passesJetPuIdnone = jet->passesPuJetId  ( flashgg::none );
          // passesJetPuIdloose = jet->passesPuJetId  ( flashgg::loose );
          // passesJetPuIdmedium = jet->passesPuJetId  ( flashgg::medium );
          // passesJetPuIdtight = jet->passesPuJetId  ( flashgg::tight );
          // passesJetPuIdmixed = jet->passesPuJetId  ( flashgg::mixed );
          // passesJetPuIdforward_loose = jet->passesPuJetId  ( flashgg::forward_loose );
          // passesJetPuIdforward_medium = jet->passesPuJetId  ( flashgg::forward_medium );
          // passesJetPuIdforward_tight = jet->passesPuJetId  ( flashgg::forward_tight );

          JetVars_[jetIndex*numVars + 0] = passLoose;
          JetVars_[jetIndex*numVars + 1] = passTight;
          JetVars_[jetIndex*numVars + 2] = passTight2017;
          JetVars_[jetIndex*numVars + 3] = passTight2018;
          // JetVars_[jetIndex*numVars + 4] = passesJetPUIdLoose;

          // JetVars_[jetIndex*numVars + 4] = passesJetPuIdnone;
          // JetVars_[jetIndex*numVars + 5] = passesJetPuIdloose;
          // JetVars_[jetIndex*numVars + 6] = passesJetPuIdmedium;
          // JetVars_[jetIndex*numVars + 7] = passesJetPuIdtight;
          // JetVars_[jetIndex*numVars + 8] = passesJetPuIdmixed;
          // JetVars_[jetIndex*numVars + 9] = passesJetPuIdforward_loose;
          // JetVars_[jetIndex*numVars + 10] = passesJetPuIdforward_medium;
          // JetVars_[jetIndex*numVars + 11] = passesJetPuIdforward_tight;

      }

      return JetVars_;
    }

    vector<Ptr<flashgg::Jet> > HHWWggTagProducer::GetFHminWHJets(bool doHHWWggDebug, std::vector<edm::Ptr<Jet> > tagJets_)
    {
      // get 4 jets for FH final state with minWH vals
      std::vector<edm::Ptr<flashgg::Jet> > FHJets_;
      bool DEBUG = doHHWWggDebug;
      double TempMinWMass = 999999.0;
      double TempMinHMass = 999999.0;

      int OnShellW_LeadingJetIndex = -1;
      int OnShellW_SubLeadingJetIndex = -1;
      int OffShellW_LeadingJetIndex = -1;
      int OffShellW_SubLeadingJetIndex = -1;

      Ptr<flashgg::Jet> jet11;
      Ptr<flashgg::Jet> jet12;
      Ptr<flashgg::Jet> jet1;
      Ptr<flashgg::Jet> jet2;
      Ptr<flashgg::Jet> jet3;
      Ptr<flashgg::Jet> jet4;

      int nTagJets = tagJets_.size();

      for (int CountJet1 = 0; CountJet1 < nTagJets-1; CountJet1++) {
      for (int CountJet2 = CountJet1+1; CountJet2 < nTagJets; CountJet2++) {
        if (DEBUG) std::cout << "(CountJet1,CountJet2) = (" << CountJet1 << "," << CountJet2 << ")" << std::endl;
        jet11 = tagJets_[CountJet1];
        jet12 = tagJets_[CountJet2];

        double deltaMass =  abs((jet11->p4() + jet12->p4()).M() - 80.0);
        if (DEBUG) std::cout << "deltaMass = " << deltaMass << "\t TempMinWMass = " << TempMinWMass << std::endl;
        if ( deltaMass < TempMinWMass)
        {
          if  (jet11->p4().pt() > jet12->p4().pt()) {
            OnShellW_LeadingJetIndex = CountJet1;
            OnShellW_SubLeadingJetIndex = CountJet2;
          } else {
            OnShellW_LeadingJetIndex = CountJet2;
            OnShellW_SubLeadingJetIndex = CountJet1;
          }
          TempMinWMass = deltaMass;
          if (DEBUG) std::cout << "==> (CountJet1,CountJet2) = (" << CountJet1 << "," << CountJet2 << ")" << std::endl;
        }
      }
      }
      if (DEBUG) std::cout << "[INFO] Print Jet Index (After W-Selection): " << OnShellW_LeadingJetIndex << "\t" << OnShellW_SubLeadingJetIndex << "\t" << OffShellW_LeadingJetIndex << "\t" << OffShellW_SubLeadingJetIndex  << std::endl;

      for (int CountJet1 = 0; CountJet1 < nTagJets-1; CountJet1++) {
      if (CountJet1 == OnShellW_LeadingJetIndex || CountJet1 == OnShellW_SubLeadingJetIndex)
        continue;
      for (int CountJet2 = CountJet1+1; CountJet2 < nTagJets; CountJet2++) {
        if (CountJet2 == OnShellW_LeadingJetIndex || CountJet2 == OnShellW_SubLeadingJetIndex)
          continue;
        if (DEBUG) std::cout << "(CountJet1,CountJet2) = (" << CountJet1 << "," << CountJet2 << ")" << std::endl;
        jet11 = tagJets_[CountJet1];
        jet12 = tagJets_[CountJet2];

        double deltaMass =  abs((jet11->p4() + jet12->p4() + tagJets_[OnShellW_LeadingJetIndex]->p4() + tagJets_[OnShellW_SubLeadingJetIndex]->p4() ).M() - 125.0);
        if (DEBUG) std::cout << "deltaMass = " << deltaMass << "\t TempMinHMass = " << TempMinHMass << std::endl;
        if ( deltaMass < TempMinHMass)
        {
          if  (jet11->p4().pt() > jet12->p4().pt()) {
            OffShellW_LeadingJetIndex = CountJet1;
            OffShellW_SubLeadingJetIndex = CountJet2;
          } else {
            OffShellW_LeadingJetIndex = CountJet2;
            OffShellW_SubLeadingJetIndex = CountJet1;
          }
          TempMinHMass = deltaMass;
          if (DEBUG) std::cout << "==> (CountJet1,CountJet2) = (" << CountJet1 << "," << CountJet2 << ")" << std::endl;
        }
      }
      }
      jet1 = tagJets_[OnShellW_LeadingJetIndex];
      jet2 = tagJets_[OnShellW_SubLeadingJetIndex];
      jet3 = tagJets_[OffShellW_LeadingJetIndex];
      jet4 = tagJets_[OffShellW_SubLeadingJetIndex];

      if (DEBUG) std::cout << "[INFO] Print pt of 4 selected jets: " << OnShellW_LeadingJetIndex << "\t" << OnShellW_SubLeadingJetIndex << "\t" << OffShellW_LeadingJetIndex << "\t" << OffShellW_SubLeadingJetIndex  << std::endl;
      if (DEBUG) std::cout << "[INFO] jet1 pT = " << jet1->p4().pt() << std::endl;
      if (DEBUG) std::cout << "[INFO] jet2 pT = " << jet2->p4().pt() << std::endl;
      if (DEBUG) std::cout << "[INFO] jet3 pT = " << jet3->p4().pt() << std::endl;
      if (DEBUG) std::cout << "[INFO] jet4 pT = " << jet4->p4().pt() << std::endl;

      FHJets_.push_back(jet1);
      FHJets_.push_back(jet2);
      FHJets_.push_back(jet3);
      FHJets_.push_back(jet4);

      return FHJets_;
    }
    double Event_num = 1;
    void HHWWggTagProducer::produce( Event &event, const EventSetup & )
    {
      bool DEBUG = true;

      if (1) cout << "[HHWWggTagProducer.cc] - Beginning of HHWWggTagProducer::produce" <<Event_num<< endl;

      // Get particle objects
      event.getByToken( photonToken_, photons );
      event.getByToken( diphotonToken_, diphotons );
      Savejet=1;
      // update global variables
      globalVariablesComputer_.update(event);

      // Get particle objects
      // event.getByToken( genParticleToken_, genParticle );
      event.getByToken( electronToken_, electrons );
      event.getByToken( muonToken_, muons );
      event.getByToken( METToken_, METs );
      event.getByToken( mvaResultToken_, mvaResults );
      event.getByToken( vertexToken_, vertices );
      event.getByToken( rhoTag_, rho);

      double rho_    = *rho;

      // Set cut booleans
      // std::vector<double> Cut_Results = {1.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0}; // Cut_Results[i] = 1: Event Passed Cut i
      std::vector<double> Cut_Variables(20,0.0); // Cut_Results[i] = 1.0: Event Passed Cut i
      // std::vector<double> Vertex_Variables(20,0.0); // Cut_Results[i] = 1.0: Event Passed Cut i
      std::vector<double> MuonVars; // For saving Muon ID's and isolation
      std::vector<double> JetVars;

      // Cut Variables
      // double has_PS_Dipho = 0, pass_METfilters = 0, dipho_vertex_is_zero = 0, pass_leadPhoOverMassThreshold = 0, pass_subleadPhoOverMassThreshold = 0,
      double pass_leadPhoOverMassThreshold = 0, pass_subleadPhoOverMassThreshold = 0;

      //---output collection
      // std::unique_ptr<vector<HHWWggTag> > tags( new vector<HHWWggTag> );
      int n_good_electrons = 0;
      int n_good_muons = 0;
      int n_good_leptons = 0;
      int n_good_jets = 0;
      bool hasHighbTag = 0;
      float btagVal = 0;
      double leadPho_pt = 0;
      double subleadPho_pt = 0;
      double sumpT = 0;

      // Electrons

      double num_FL_dr = 0;
      double num_FH_dr = 0;
      float dr_ll = 0;

      bool passMVAs = 0; // True if leading and subleading photons pass MVA selections

      int catnum = 4; // category number. default to untagged

      // Saved Objects after selections
      std::vector<edm::Ptr<flashgg::Jet> > allJets;
      std::vector<edm::Ptr<flashgg::Jet> > FHJets; // four jets for fully hadronic tag
      std::vector<edm::Ptr<flashgg::Electron> > allElectrons;
      std::vector<edm::Ptr<flashgg::Muon> > allMuons;
      reco::GenParticle::Point genVertex;

      // Four jets for the fully hadronic analysis. First two variables also used for SL final state jets
      Ptr<flashgg::Jet> jet1;
      Ptr<flashgg::Jet> jet2;
      Ptr<flashgg::Jet> jet3;
      Ptr<flashgg::Jet> jet4;


      int n_METs = METs->size(); // Should be 1, but using as a way to obtain met four vector
      double diphoMVA = -99;
      double lead_pho_Hgg_MVA = -99, sublead_pho_Hgg_MVA = -99;

      // Saved Objects after selections
      std::vector<flashgg::Jet> tagJets_;
      std::vector<flashgg::Muon> goodMuons_;
      std::vector<flashgg::Electron> goodElectrons_;
      std::vector<flashgg::Met> theMET_;
      std::vector<flashgg::DiPhotonCandidate> diphoVector_;

      std::unique_ptr<vector<TagTruthBase> > truths( new vector<TagTruthBase> );
      edm::RefProd<vector<TagTruthBase> > rTagTruth = event.getRefBeforePut<vector<TagTruthBase> >();

//-----------------------------------------------------------------------------------------------------------

      // Vertex variables
      edm::Ptr<reco::Vertex> diphoton_vertex;
      edm::Ptr<reco::Vertex> zero_vertex;

      if (vertices->size() > 0){
        zero_vertex = vertices->ptrAt( 0 );
      }

      //-- MC truth
      TagTruthBase truth_obj;
      if( ! event.isRealData() ) {
          Handle<View<reco::GenParticle> > genParticles;
          std::vector<edm::Ptr<reco::GenParticle> > selHiggses;
          event.getByToken( genParticleToken_, genParticles );
          reco::GenParticle::Point higgsVtx(0.,0.,0.);

          for( unsigned int genLoop = 0 ; genLoop < genParticles->size(); genLoop++ ) {
              int pdgid = genParticles->ptrAt( genLoop )->pdgId();
              // pdgId->Fill(pdgid);

              // if( pdgid == 25 || pdgid == 22 ) { // not so sure if this is correct for HHWWgg because of potential photons from hadronization
              if( pdgid == 25 ) {
                  higgsVtx = genParticles->ptrAt( genLoop )->vertex();
                  // gen_vertex_z = higgsVtx.z();
                  break;
              }
          }

          truth_obj.setGenPV( higgsVtx );
          truths->push_back( truth_obj );
      }

      // METfilters
      // bool passMETfilters=1;
      //Get trigger results relevant to MET filters

      edm::Handle<edm::TriggerResults> triggerBits;
      if(! event.isRealData() )
          event.getByToken( triggerPAT_, triggerBits );
      else
          event.getByToken( triggerRECO_, triggerBits );

      edm::Handle<edm::TriggerResults> triggerFLASHggMicroAOD;
      event.getByToken( triggerFLASHggMicroAOD_, triggerFLASHggMicroAOD );
      const edm::TriggerNames &triggerNames = event.triggerNames( *triggerBits );

      //check if passMETfilters
      std::vector<std::string> flagList {"Flag_HBHENoiseFilter","Flag_HBHENoiseIsoFilter","Flag_EcalDeadCellTriggerPrimitiveFilter","Flag_goodVertices","Flag_eeBadScFilter"};
      for (unsigned int i = 0; i < triggerNames.triggerNames().size(); i++ )
      {
        if (!triggerBits->accept(i))
        for (size_t j=0;j<flagList.size();j++)
        {
          if (flagList[j]==triggerNames.triggerName(i))
          {
            // passMETfilters=0;
            break;
          }
        }
      }

      std::vector<std::string> flashggFlagList {"flag_BadChargedCandidateFilter","flag_BadPFMuonFilter","flag_globalTightHalo2016Filter"};
      const edm::TriggerNames &flashggtriggerNames = event.triggerNames( *triggerFLASHggMicroAOD );
      for( unsigned int i = 0; i < flashggtriggerNames.triggerNames().size(); i++ )
      {
        if(!triggerFLASHggMicroAOD->accept(i))
        for(size_t j=0;j<flagList.size();j++)
        {
          if(flagList[j]==flashggtriggerNames.triggerName(i))
          {
            // passMETfilters=0;
            break;
          }
        }
      }

      if(doHHWWggTagCutFlowAnalysis_) Cut_Variables[0] = 1.0; // passed diphoton preselection
      cout<<"passed dipho preselection"<<endl;
      // read diphotons
      // for (unsigned int diphoton_idx = 0; diphoton_idx < diPhotonTokens_.size(); diphoton_idx++) { //looping over all diphoton systematics
        // cout << "diphoton_idx = " << diphoton_idx << endl;
        // diphoton_idx_h->Fill(diphoton_idx);
        // Handle<View<flashgg::DiPhotonCandidate> > diPhotons;

        //---
        // event.getByToken( diPhotonTokens_[diphoton_idx], diPhotons ); // for each diphoton systematic
        // event.getByToken( diphotonToken_, diphotons ); // without looping over diphoton systematics
        //---

        // unsigned int loopOverJets = 1;
        // if (inputDiPhotonSuffixes_[diphoton_idx].empty()) loopOverJets = inputJetsSuffixes_.size();

        // for (unsigned int jet_col_idx = 0; jet_col_idx < loopOverJets; jet_col_idx++) {//looping over all jet systematics, only for nominal diphotons
          // cout << "jet_col_idx = " << jet_col_idx << endl;
        std::unique_ptr<vector<HHWWggTag> > HHWWggtags( new vector<HHWWggTag> );

        // if (diPhotons->size() > 0){ // for each systematic

        if (diphotons->size() > 0){
        for( unsigned int diphoIndex = 0; diphoIndex < 1; diphoIndex++ )
        { // only look at highest sumpt dipho
          // edm::Ptr<flashgg::DiPhotonCandidate> dipho = Corrdiphoton;
          // edm::Ptr<flashgg::DiPhotonCandidate> dipho = diPhotons->ptrAt( diphoIndex ); // systematic loop
          edm::Ptr<flashgg::DiPhotonCandidate> dipho = diphotons->ptrAt( diphoIndex ); // without systematic look (?)
          edm::Ptr<flashgg::DiPhotonMVAResult> mvares = mvaResults->ptrAt( diphoIndex );
          // cout << "dipho pt = " << dipho->genP4().pt() << endl;
          diphoton_vertex = dipho->vtx();

          // diphoton_vertex_index = dipho->vertexIndex();
          // cout << "diphoton vertex index = " << diphoton_vertex_index << endl;
          // cout << "diphoton vertex x = " << diphoton_vertex->x() << endl;
          // cout << "diphoton vertex y = " << diphoton_vertex->y() << endl;
          // cout << "diphoton vertex z = " << diphoton_vertex->z() << endl;

          // indexes->Fill(diphoton_vertex_index); // running

          //-- Get Photons
          const flashgg::Photon* leadPho = dipho->leadingPhoton();
          const flashgg::Photon* subleadPho = dipho->subLeadingPhoton();

          if(doHHWWggNonResAnalysis_){
            leadPho_pt = leadPho->pt();
            subleadPho_pt = subleadPho->pt();
            sumpT = leadPho_pt + subleadPho_pt;
            // if(sumpT < 100) continue;
            cout<<"sumPT:"<<sumpT<<endl;
            if(!doHHWWggTagCutFlowAnalysis_ && sumpT < 100.) {
              cout<<"failed sumPT"<<"sumPT:"<<sumpT<<endl;
              continue;} // if not doing cut flow analysis to save events, remove event
          }

          //-- MVA selections
          diphoton_vertex = dipho->vtx();

          passMVAs = 0;
          passMVAs = checkPassMVAs(leadPho, subleadPho, diphoton_vertex, EB_MVA_Threshold_, EE_MVA_Threshold_);

          // leading/subleading photon pt
          if( dipho->leadingPhoton()->pt() > ( dipho->mass() )*leadPhoOverMassThreshold_ ){
              pass_leadPhoOverMassThreshold = 1;
            }
          if( dipho->subLeadingPhoton()->pt() > ( dipho->mass() )*subleadPhoOverMassThreshold_ ) {
              pass_subleadPhoOverMassThreshold = 1;
            }

          // Doing cut flow analysis: Don't skip event, but check if photon selections are passed
          if(doHHWWggTagCutFlowAnalysis_){
            if(!passMVAs || !pass_leadPhoOverMassThreshold || !pass_subleadPhoOverMassThreshold) Cut_Variables[1] = 0.0; // failed photon selections
            else Cut_Variables[1] = 1.0; // passed photon selections
          }

          // Not doing cut flow analysis: Skip event if photon selections are not passed
          else{
            if(!passMVAs || !pass_leadPhoOverMassThreshold || !pass_subleadPhoOverMassThreshold) {
              cout<<"failed mva or pt/gg"<<"MVA"<<passMVAs<<",leadPhoOverMass"<<pass_leadPhoOverMassThreshold<<",subleadPhoOverMass"<<pass_subleadPhoOverMassThreshold<<endl;
              continue;} // Do not save event if leading and subleading photons don't pass MVA cuts or pt/mgg cuts
          }


          // Check MET Filters
          // if(passMETfilters){
          //   pass_METfilters = 1;
          // }






          hasGoodElec = false;
          hasGoodMuons = false;

          // Electrons
          std::vector<edm::Ptr<Electron> > goodElectrons = selectStdElectrons( electrons->ptrs(), dipho, vertices->ptrs(), leptonPtThreshold_, electronEtaThresholds_,
                                                                            useElectronMVARecipe_,useElectronLooseID_,
                                                                            deltaRPhoElectronThreshold_,DeltaRTrkElec_,deltaMassElectronZThreshold_,
                                                                            rho_, event.isRealData() );

          if(doHHWWggTagCutFlowAnalysis_){
            for( unsigned int ei = 0; ei <  electrons->size() ; ei++ ){
              edm::Ptr<flashgg::Electron> theElectron = electrons->ptrAt( ei );
              allElectrons.push_back(theElectron);
            }
            for( unsigned int mi = 0; mi <  muons->size() ; mi++ ){
              edm::Ptr<flashgg::Muon> theMuon = muons->ptrAt( mi );
              allMuons.push_back(theMuon);
            }
          }

          // Muons
          std::vector<edm::Ptr<flashgg::Muon> > goodMuons = selectMuons( muons->ptrs(), dipho, vertices->ptrs(), muonEtaThreshold_, leptonPtThreshold_,
          muPFIsoSumRelThreshold_, deltaRMuonPhoThreshold_, deltaRMuonPhoThreshold_ );

          // If doing cut flow analysis, save Muon IDs
          if(doHHWWggTagCutFlowAnalysis_) MuonVars = GetMuonVars(muons->ptrs(), vertices->ptrs());

          n_good_electrons = goodElectrons.size();
          n_good_muons = goodMuons.size();
          n_good_leptons = n_good_electrons + n_good_muons;
          hasGoodElec = ( goodElectrons.size() > 0 );
          hasGoodMuons = ( goodMuons.size() > 0 );

          // FL: Require at dr(l,l) > 0.4
          // For which pairs of the >=2 good leptons should dr be greater than 4?


          // FL: Require at dr(l,l) > 0.4
          // For which pairs of the >=2 good leptons should dr be greater than 4?
          if(doHHWWggTagCutFlowAnalysis_){
          if(hasGoodElec)
          Cut_Variables[2] = 1.0; // pass goodEle
          else
          Cut_Variables[2] = 0.0; // do not pass goodEle
          }
          if(doHHWWggTagCutFlowAnalysis_){
          if(hasGoodMuons)
          Cut_Variables[3] = 1.0; //  pass goodMuon
          else
          Cut_Variables[3] = 0.0; // do not pass goodMuon
          }
          if (hasGoodElec && hasGoodMuons){
            for (unsigned int ei = 0; ei < goodElectrons.size(); ei++){
              Ptr<flashgg::Electron> electron = goodElectrons[ei];
              for (unsigned int mi = 0; mi < goodMuons.size(); mi++){
                Ptr<flashgg::Muon> muon = goodMuons[mi];
                dr_ll = deltaR(electron->eta(), electron->phi(), muon->eta(), muon->phi());
                if (dr_ll > deltaRLeps_){
                  // one_FL_dr = true;
                  num_FL_dr += 1.0;
                  // break;
                }
              }
            }
          }

          else if(hasGoodElec && !hasGoodMuons){
            for (unsigned int ei = 0; ei < goodElectrons.size() - 1; ei++){ // the last electron cannot be the first one in the dR calculation
              Ptr<flashgg::Electron> electroni = goodElectrons[ei];
              for (unsigned int ej = ei + 1; ej < goodElectrons.size(); ej++){
                Ptr<flashgg::Electron> electronj = goodElectrons[ej];
                dr_ll = deltaR(electroni->eta(), electroni->phi(), electronj->eta(), electronj->phi());
                if (dr_ll > deltaRLeps_){
                  // one_FL_dr = true;
                  num_FL_dr += 1.0;
                  // break;
                }
              }
            }
          }

          else if(!hasGoodElec && hasGoodMuons){
            for (unsigned int mi = 0; mi < goodMuons.size() - 1; mi++){
              Ptr<flashgg::Muon> muoni = goodMuons[mi];
              for (unsigned int mj = mi + 1; mj < goodMuons.size(); mj++){
                Ptr<flashgg::Muon> muonj = goodMuons[mj];
                dr_ll = deltaR(muoni->eta(), muoni->phi(), muonj->eta(), muonj->phi());
                if (dr_ll > deltaRLeps_){
                  // one_FL_dr = true;
                  num_FL_dr += 1.0;
                  // break;
                }
              }
            }
          }

          // Jets
          // unsigned int jetCollectionIndex = diPhotons->at( diphoIndex ).jetCollectionIndex(); // diphoton collection for each systematic
          unsigned int jetCollectionIndex = diphotons->at( diphoIndex ).jetCollectionIndex(); // not looping over systematics
          // size_t vtx = (size_t)dipho->jetCollectionIndex();
          edm::Handle<edm::View<flashgg::Jet> > Jets_;

          event.getByToken( jetTokens_[jetCollectionIndex], Jets_); // testing
          // event.getByToken( jetTokens_[jet_col_idx*inputJetsCollSize_+vtx], Jets_);  //take the corresponding vertex of current systematic //WORKS
          // cout << "right after getbytoken jettokens jets" << endl;

          std::vector<edm::Ptr<Jet> > tagJets;

          // If doing cut flow analysis, save Jet IDs
          if(doHHWWggTagCutFlowAnalysis_) JetVars = GetJetVars(Jets_->ptrs(), dipho);

          // Jet Selections
          for( unsigned int candIndex_outer = 0; candIndex_outer <  Jets_->size() ; candIndex_outer++ )
              {
                  bool keepJet=true;
                  edm::Ptr<flashgg::Jet> thejet = Jets_->ptrAt( candIndex_outer );
                  allJets.push_back(thejet);

                  // JetID sleection
                  // if(!thejet->passesJetID  ( flashgg::Tight2017 ) ){ cout<<"failed JetID"<<endl;
                    // continue; }//nead to fix
                  if(thejet->passesJetID  ( flashgg::Tight2017 ) ) {}
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

          // If jet collection has a jet suspected to be a bjet, don't save the event
          hasHighbTag = 0;
          Savejet = 1;
          for (unsigned int j = 0; j < tagJets.size(); j++){
            Ptr<flashgg::Jet> jet_ = tagJets[j];
            btagVal = jet_->bDiscriminator("mini_pfDeepFlavourJetTags:probb");
            if (btagVal > btagThresh_) hasHighbTag = 1;
            if (  jet_->pt() >20 && (btagVal > btagThresh_)) Savejet = 0;
            cout<<"failed jet pt or btag"<<endl;
          }

          // If doing cut flow analysis, don't skip event
          if(doHHWWggTagCutFlowAnalysis_){
            if(hasHighbTag)
              Cut_Variables[2] = 0.0; // does not pass bveto
            else
              Cut_Variables[2] = 1.0; // passes bveto
          }
          else if(hasHighbTag) {
            cout<<"faild b Veto"<<endl;
            continue;} // Skip event if it has at least one jet with a btag above threshold, and you're not doing a cut flow analysis
          n_good_jets = tagJets.size();

          // MET
          if( METs->size() != 1 ) { std::cout << "WARNING - #MET is not 1" << std::endl;}
          Ptr<flashgg::Met> theMET = METs->ptrAt( 0 );

          // how to go from pointer 'jet' to object '*thisJetPointer'
          //flashgg::Jet * thisJetPointer = const_cast<flashgg::Jet *>(jet.get());
          //JetVector.push_back(*thisJetPointer);

          // for (unsigned int i = 0; i < tagJets.size(); i++){
            // auto tag_jet = tagJets[i];
            // flashgg::Jet * thisJetPointer = const_cast<flashgg::Jet *>(tag_jet.get());
            // tagJets_.push_back(*thisJetPointer);
          // }

          if (doHHWWggTagCutFlowAnalysis_){
            if (n_good_leptons == 1) Cut_Variables[3] = 1.0; // exactly one good lepton
            else Cut_Variables[3] = 0.0;
            if (n_good_jets >= 2) Cut_Variables[4] = 1.0; // at least 2 good jets
            else Cut_Variables[4] = 0.0;
            if (n_good_jets >= 4) Cut_Variables[5] = 1.0; // at least 4 good jets
            else Cut_Variables[5] = 0.0;
            if (n_good_leptons==0 && n_good_jets>=4) Cut_Variables[6] = 1.0;  // At least 4 good jets without any good leptons
            else Cut_Variables[6] = 0.0;
            if (n_good_leptons >= 2) Cut_Variables[12] = 1.0; // At least 2 good lepton
            else Cut_Variables[12] = 0.0;
            if (n_good_electrons == 1 && n_good_muons == 1 ) Cut_Variables[13] = 1.0; // 2 diff flavor lepton
            else Cut_Variables[13] = 0.0;
            if (n_good_muons == 2 ) Cut_Variables[14] = 1.0; // 2 good muon
            else Cut_Variables[14] = 0.0;
            if (n_good_electrons == 2 ) Cut_Variables[15] = 1.0; //  2 good ele
            else Cut_Variables[15] = 0.0;
            if(num_FL_dr>=1) Cut_Variables[16] = 1.0;
            else Cut_Variables[16] = 0.0;
            if(theMET->getCorPt() >= MetPtThreshold_) Cut_Variables[12] = 1.0;
            else Cut_Variables[17] = 0.0;
          }

          /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


          if(doHHWWggTagCutFlowAnalysis_){

          }
          //-- Categorize Events
          if (DEBUG) std::cout << "[INFO] n_good_leptons = " << n_good_leptons << ",\t n_good_jets = " << n_good_jets << std::endl;

          //-- Semi-Leptonic Final state tags
          if(HHWWggAnalysisChannel_ == "SL")
          {
            if ( (n_good_leptons == 1) && (n_good_jets >= 2)) {
              jet1 = tagJets[0];
              Ptr<flashgg::Met> theMET = METs->ptrAt( 0 );

              if (n_good_electrons == 1){
                Ptr<flashgg::Electron> tag_electron = goodElectrons[0];
                catnum = 0;
                  jet2 = tagJets[1];
                  HHWWggTag tag_obj;
                  if (doHHWWggTagCutFlowAnalysis_){
                    HHWWggTag tag_obj_(dipho, tag_electron, allElectrons, goodElectrons, allMuons, theMET, jet1, jet2, allJets, tagJets, Cut_Variables, MuonVars, JetVars);
                    tag_obj = tag_obj_;
                  }
                  else{
                    HHWWggTag tag_obj_(dipho, tag_electron, theMET, jet1, jet2); // diphoton, electron, MET, jet1, jet2
                    tag_obj = tag_obj_;
                  }
                  tag_obj.setSystLabel( systLabel_);
                  tag_obj.setDiPhotonIndex( diphoIndex );
                  tag_obj.setCategoryNumber( catnum );
                  tag_obj.includeWeights( *dipho );
                  HHWWggtags->push_back( tag_obj );
                  if( ! event.isRealData() ) {
                    HHWWggtags->back().setTagTruth( edm::refToPtr( edm::Ref<vector<TagTruthBase> >( rTagTruth, 0 ) ) );
                  }

              }

              if (n_good_muons == 1){
                Ptr<flashgg::Muon> tag_muon = goodMuons[0];
                catnum = 1;
                  jet2 = tagJets[1];
                  HHWWggTag tag_obj;
                  if (doHHWWggTagCutFlowAnalysis_){
                    HHWWggTag tag_obj_(dipho, allElectrons, tag_muon, allMuons, goodMuons, theMET, jet1, jet2, allJets, tagJets, Cut_Variables, MuonVars, JetVars);
                    tag_obj = tag_obj_;
                  }
                  else{
                    HHWWggTag tag_obj_(dipho, tag_muon, theMET, jet1, jet2); // diphoton, muon, MET, jet1, jet2
                    tag_obj = tag_obj_;
                  }

                  tag_obj.setSystLabel(systLabel_);
                  tag_obj.setDiPhotonIndex( diphoIndex );
                  tag_obj.setCategoryNumber( catnum ); // 1 for muon events
                  tag_obj.includeWeights( *dipho );
                  HHWWggtags->push_back( tag_obj );

                  if( ! event.isRealData() ) {
                    HHWWggtags->back().setTagTruth( edm::refToPtr( edm::Ref<vector<TagTruthBase> >( rTagTruth, 0 ) ) );
                  }
              }
            } // if ( (n_good_leptons == 1) && (n_good_jets >= 2) )


          } // Semileptonic cateogries

          //-- Fully Hadronic Final State Tags

          if(HHWWggAnalysisChannel_ == "FH")
          {

            if (n_good_leptons==0 && n_good_jets>=4)
            {
              catnum = 2;
              Ptr<flashgg::Met> theMET = METs->ptrAt( 0 );

              // Define four jets with WH min method, or just take four leading pT
              if(doHHWWggFHptOrdered_){
                jet1 = tagJets[0];
                jet2 = tagJets[1];
                jet3 = tagJets[2];
                jet4 = tagJets[3];
              }
              else{
                FHJets = GetFHminWHJets(doHHWWggDebug_, tagJets);
                jet1 = FHJets[0];
                jet2 = FHJets[1];
                jet3 = FHJets[2];
                jet4 = FHJets[3];
              }

              HHWWggTag tag_obj;

              if (doHHWWggTagCutFlowAnalysis_){
                // Save cut flow variables
                if ( (jet1->p4() + jet2->p4()).M() > 40 && (jet1->p4() + jet2->p4()).M() < 160 ) Cut_Variables[7] = 1.0;
                if ( (jet1->p4() + jet2->p4()).M() > 65 && (jet1->p4() + jet2->p4()).M() < 105 ) Cut_Variables[8] = 1.0;
                if ( (jet3->p4() + jet4->p4()).M() > 0 && (jet3->p4() + jet4->p4()).M() < 160 ) Cut_Variables[9] = 1.0;
                if ( (jet1->p4() + jet2->p4() + jet3->p4() + jet4->p4()).M() > 105 && (jet1->p4() + jet2->p4() + jet3->p4() + jet4->p4()).M() < 160 ) Cut_Variables[10] = 1.0;
                if ( (jet1->p4() + jet2->p4() + jet3->p4() + jet4->p4()).M() > 40 && (jet1->p4() + jet2->p4() + jet3->p4() + jet4->p4()).M() < 210 ) Cut_Variables[11] = 1.0;

                // save tag
                HHWWggTag tag_obj_(dipho, theMET, jet1, jet2, jet3, jet4, allJets, tagJets, Cut_Variables, JetVars);
                tag_obj = tag_obj_;
              }
              else{
                HHWWggTag tag_obj_(dipho, theMET, jet1, jet2, jet3, jet4 ); // diphoton, electron, MET, jet1, jet2
                tag_obj = tag_obj_;
              }

              tag_obj.setSystLabel( systLabel_);
              tag_obj.setDiPhotonIndex( diphoIndex );
              tag_obj.setCategoryNumber( catnum );
              tag_obj.includeWeights( *dipho );
              HHWWggtags->push_back( tag_obj );
              if( ! event.isRealData() ) {
                HHWWggtags->back().setTagTruth( edm::refToPtr( edm::Ref<vector<TagTruthBase> >( rTagTruth, 0 ) ) );
              }
            }  // if (n_good_leptons==0 && n_good_jets>=4)
          } // Fully Hadronic Categories
cout<<"n_good_leptons"<<n_good_leptons<<","<<n_good_muons<<","<<n_good_electrons<<endl;
          if(HHWWggAnalysisChannel_ == "FL")
          {  int catnum = 3;
          Ptr<flashgg::Met> theMET = METs->ptrAt( 0 );
          if ( (n_good_leptons >=2 ) && (theMET->getCorPt() >= MetPtThreshold_) && num_FL_dr>=1 && (leadPho->p4().pt()+subleadPho->p4().pt())>100){

            double Save=0.;
            double MassT;
            double MassT_l2;
            double lep3pt = 0.;
            Ptr<flashgg::Electron> tag_electron1;
            Ptr<flashgg::Electron> tag_electron3;
            Ptr<flashgg::Electron> tag_electron2;
            Ptr<flashgg::Muon> tag_muon1;
            Ptr<flashgg::Muon> tag_muon3;
            Ptr<flashgg::Muon> tag_muon2;
            sort(goodElectrons.begin(),goodElectrons.end(),compEle);
            sort(goodMuons.begin(),goodMuons.end(),compMu);
            if (n_good_electrons >= 2){
              cout<<"Ele tag"<<endl;
              cout << "size"<<goodElectrons.size()<< endl;
              cout << "n_good_ele:"<<n_good_electrons<< endl;
              cout<<"n_good_muon:"<<n_good_muons<<endl;
              //std::cout<<"ele1:"<<goodElectrons[0]->pt()<<"ele2:"<<goodElectrons[1]->pt()<<"ele3:"<<goodElectrons[2]->pt()<<std::endl;
              if(n_good_electrons == 2 && n_good_muons < 1){
              tag_electron1 = goodElectrons[0];
              tag_electron2 = goodElectrons[1];
              Save = 1.;
              cout<<"n_good_electrons == 2 && n_good_muons < 1"<<endl;
              }
              else if(n_good_electrons > 2 && n_good_muons < 1){
              tag_electron1 = goodElectrons[0];
              tag_electron2 = goodElectrons[1];
              tag_electron3 = goodElectrons[2];
              Save = 1.;
              cout<<"n_good_electrons > 2 && n_good_muons < 1"<<endl;
              }
              else if(n_good_electrons ==2 && n_good_muons >=1){
              cout<<"n_good_electrons == 2 && n_good_muons >= 1"<<endl;
              if(goodMuons[0]->pt()<lep3ptThre_){
              tag_electron1 = goodElectrons[0];
              tag_electron2 = goodElectrons[1];
              Save=1.;
              }
              else {
                tag_electron1 = goodElectrons[0];
                tag_electron2 = goodElectrons[1];
                Save=0.;}
              }
              else if(n_good_electrons >2 && n_good_muons >=1){
              if(goodMuons[0]->pt()<lep3ptThre_){
              tag_electron1 = goodElectrons[0];
              tag_electron2 = goodElectrons[1];
              tag_electron3 = goodElectrons[2];
              Save = 1.;
              }
              else{
              tag_electron1 = goodElectrons[0];
              tag_electron2 = goodElectrons[1];
              tag_electron3 = goodElectrons[2];
              Save = 0.;
              }
              cout<<"n_good_electrons > 2 && n_good_muons >= 1"<<endl;
              }
              reco::Candidate::LorentzVector Ele1LV = tag_electron1->p4();
              reco::Candidate::LorentzVector Ele2LV = tag_electron2->p4();
              reco::Candidate::LorentzVector DiEleLV = Ele1LV +  Ele2LV;
              MassT= sqrt(2*DiEleLV.Pt()*theMET->pt()*(1-cos(abs(DiEleLV.Phi()-theMET->phi()))));
              MassT_l2 = sqrt(2*Ele2LV.Pt()*theMET->pt()*(1-cos(abs(Ele2LV.Phi()-theMET->phi()))));
              cout<<"MassT:"<<MassT<<"MassT_l2:"<<MassT_l2<<endl;
                if((tag_electron1->charge()*tag_electron2->charge()==-1) && tag_electron1->pt()>=lep1ptThre_ && tag_electron2->pt()>=lep1ptThre_  && DiEleLV.Pt()>DiLepPtThre_ && DiEleLV.M()>DiLepMassThre_ && MassT>MassTThre_ && MassT_l2 >MassT_l2Thre_ && Save == 1. && Savejet==1){
                HHWWggTag tag_obj(dipho, tag_electron1, tag_electron2, theMET,Cut_Variables); // HHWWggTag need to be updated
                cout<<"ele has taged"<<endl;
                tag_obj.setSystLabel(systLabel_);

                tag_obj.setDiPhotonIndex( diphoIndex );
                tag_obj.setMVA( -0.9 );
                tag_obj.setCategoryNumber( catnum );
                HHWWggtags->push_back( tag_obj );
                cout<<"ele push back"<<endl;
                if( ! event.isRealData() ) {
                  HHWWggtags->back().setTagTruth( edm::refToPtr( edm::Ref<vector<TagTruthBase> >( rTagTruth, 0 ) ) );
                }
                }
            }

            if (n_good_muons >= 2  ){
              //passMVAs = 0;
              //passMVAs = checkPassMVAs(leadPho, subleadPho, diphoton_vertex, EB_MVA_Threshold_, EE_MVA_Threshold_);
              //if(!passMVAs) continue;
              cout<<"muon tag"<<endl;
              cout<<"n_good_muons:"<<n_good_muons<<"n_good_eles:"<<n_good_electrons<<endl;
              if (n_good_muons == 2 && n_good_electrons <1){
              tag_muon1 = goodMuons[0];
              tag_muon2 = goodMuons[1];
              Save =1.;}
              else if(n_good_muons >2 && n_good_electrons <1 ){
              tag_muon1 = goodMuons[0];
              tag_muon2 = goodMuons[1];
              tag_muon3 = goodMuons[2];
              Save = 1.;
              }
              else if(n_good_muons ==2 && n_good_electrons >=1){
                cout<<"goodElePt:"<<goodElectrons[0]->pt()<<endl;
                if(goodElectrons[0]->pt()<lep3ptThre_){
                tag_muon1 = goodMuons[0];
                tag_muon2 = goodMuons[1];
                Save = 1.;
              }
                else{
                  tag_muon1 = goodMuons[0];
                  tag_muon2 = goodMuons[1];
                Save = 0.;}
              }

              else if(n_good_muons >2 && n_good_electrons >=1){
              if(goodElectrons[0]->pt()<lep3ptThre_){
              tag_muon1 = goodMuons[0];
              tag_muon2 = goodMuons[1];
              tag_muon3 = goodMuons[2];
              Save = 1.;
              }
              else{
                tag_muon1 = goodMuons[0];
                tag_muon2 = goodMuons[1];
                tag_muon3 = goodMuons[2];
                Save = 0.;
                }
              }
              reco::Candidate::LorentzVector Mu1LV = tag_muon1->p4();
              reco::Candidate::LorentzVector Mu2LV = tag_muon2->p4();
              reco::Candidate::LorentzVector DiMuLV = Mu1LV +  Mu2LV;
              MassT= sqrt(2*DiMuLV.Pt()*theMET->pt()*(1-cos(abs(DiMuLV.Phi()-theMET->phi()))));
              MassT_l2 = sqrt(2*Mu2LV.Pt()*theMET->pt()*(1-cos(abs(Mu2LV.Phi()-theMET->phi()))));
                if((tag_muon1->charge()*tag_muon2->charge() == -1) && tag_muon1->pt()>=lep1ptThre_ && tag_muon2->pt()>=lep2ptThre_  && DiMuLV.Pt()>DiLepPtThre_ && DiMuLV.M()>DiLepMassThre_ && MassT>MassTThre_ && MassT_l2>MassT_l2Thre_ && Save == 1. && Savejet==1){
                HHWWggTag tag_obj(dipho, tag_muon1, tag_muon2, theMET,Cut_Variables);
                tag_obj.setSystLabel(systLabel_);

                tag_obj.setDiPhotonIndex( diphoIndex );
                tag_obj.setMVA( -0.9 );
                tag_obj.setCategoryNumber( catnum);
                HHWWggtags->push_back( tag_obj );

                if( ! event.isRealData() ) {
                  HHWWggtags->back().setTagTruth( edm::refToPtr( edm::Ref<vector<TagTruthBase> >( rTagTruth, 0 ) ) );
                }
            }
           }



            if (n_good_muons >=1 && n_good_electrons >= 1){
            if (n_good_electrons == 1 && n_good_muons == 1){
              cout<< "diff flavor tag" <<endl;
              tag_electron1 = goodElectrons[0];
              tag_muon1 = goodMuons[0];
              Save=1.;
              }
            else if (n_good_electrons>1 && n_good_muons ==1){
            if(goodElectrons[1]->pt()<lep3ptThre_){
              tag_electron1 = goodElectrons[0];
              tag_muon1 = goodMuons[0];
              Save=1.;
            }
            else{
            tag_electron1 = goodElectrons[0];
            tag_muon1 = goodMuons[0];
            Save=0.;
            }
            }
            else if(n_good_electrons == 1 && n_good_muons > 1){
            if(goodMuons[1]->pt()<lep3ptThre_){
            tag_electron1 = goodElectrons[0];
            tag_muon1 = goodMuons[0];
            Save=1.;
            }
            else{
            tag_electron1 = goodElectrons[0];
            tag_muon1 = goodMuons[0];
            Save=0.;}
            }
            else if(n_good_electrons > 1 && n_good_muons > 1){
            if(goodMuons[1]->pt()<lep3ptThre_ && goodElectrons[1]->pt()<lep3ptThre_){
            tag_electron1 = goodElectrons[0];
            tag_muon1 = goodMuons[0];
            Save=1.;
            }
            else{tag_electron1 = goodElectrons[0];
              tag_muon1 = goodMuons[0];
              Save=0.;}
            }
            reco::Candidate::LorentzVector MuLV = tag_muon1->p4();
              reco::Candidate::LorentzVector EleLV = tag_electron1->p4();
              reco::Candidate::LorentzVector DiLepLV = EleLV +  MuLV;
              MassT= sqrt(2*DiLepLV.Pt()*theMET->pt()*(1-cos(abs(DiLepLV.Phi()-theMET->phi()))));
              MassT_l2= sqrt(2*MuLV.Pt()*theMET->pt()*(1-cos(abs(MuLV.Phi()-theMET->phi()))));
              if((((tag_electron1->pt()>=lep1ptThre_) && (tag_muon1->pt()>=lep2ptThre_))||((tag_muon1->pt()>=lep1ptThre_) && (tag_electron1->pt()>=lep2ptThre_))) && (tag_muon1->charge()*tag_electron1->charge()==-1) && (DiLepLV.M()>DiLepMassThre_) && (DiLepLV.Pt()>DiLepPtThre_) && (MassT_l2>MassTThre_) && (MassT>MassT_l2Thre_) && (Save==1.) &&(Savejet==1)){
                HHWWggTag tag_obj(dipho, tag_electron1, tag_muon1, theMET,Cut_Variables);
                tag_obj.setSystLabel(systLabel_);
                cout<<"MassT:"<<MassT<<"DiLeppt:"<<DiLepLV.Pt()<<endl;
                tag_obj.setDiPhotonIndex( diphoIndex );
                tag_obj.setMVA( -0.9 );
                tag_obj.setCategoryNumber( catnum );
                HHWWggtags->push_back( tag_obj );

                if( ! event.isRealData() ) {
                  HHWWggtags->back().setTagTruth( edm::refToPtr( edm::Ref<vector<TagTruthBase> >( rTagTruth, 0 ) ) );
                }
            }
          }
        }
        }//FL selection

          // Untagged category
          else {
            if(doHHWWggTagCutFlowAnalysis_){
              catnum = 4;
              HHWWggTag tag_obj(dipho, allElectrons, goodElectrons, allMuons, goodMuons, theMET, allJets, tagJets, Cut_Variables, MuonVars, JetVars);
              tag_obj.setSystLabel(systLabel_);
              tag_obj.setDiPhotonIndex( diphoIndex );
              tag_obj.setCategoryNumber( catnum ); // Untagged category. Does not meet any selection criteria but want to save event
              tag_obj.includeWeights( *dipho );
              HHWWggtags->push_back( tag_obj );

              if( ! event.isRealData() ) {
                HHWWggtags->back().setTagTruth( edm::refToPtr( edm::Ref<vector<TagTruthBase> >( rTagTruth, 0 ) ) );
              }
            }
          } // Untagged category








        } // Diphoton loop //add cut flow below this line
      } // if at least 1 PS diphoton
      event.put( std::move( HHWWggtags ) );
      cout<<"tag_obj has moved"<<endl;
      event.put( std::move( truths ) );
      Event_num=Event_num + 1;
    } // HHWWggTagProducer::produce

  } // namespace flashgg

  typedef flashgg::HHWWggTagProducer FlashggHHWWggTagProducer;
  DEFINE_FWK_MODULE( FlashggHHWWggTagProducer );
