// Abe Tishelman-Charny
// November 2019
// Derived from HH->WWgg event dumper and HH->bbgg tagger 
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
    TH1F* passDR_num;
    TH1F* diphoton_idx_h;
    TH1F* diPhotons_size_h; 
    TH1F* indexes;
    TH1F* electron_cut;
    TH1F* num_MetPtCut;
    TH2D* lepPt2DCut;
    TH2D* MVA_2D_Cut;
    TH2D* MVA_2D_Cut_lep_cut;
    TH1F* num_MetPtCut_lep_cut;
    TH1F* DiPhotonBDT_Scan;
  private:
    double genTotalWeight;
    bool checkPassMVAs(const flashgg::Photon*& leading_photon, const flashgg::Photon*& subleading_photon, edm::Ptr<reco::Vertex>& diphoton_vertex,double EB_MVA,double EE_MVA);
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
    Handle<View<flashgg::DiPhotonCandidate> > diPhotons;

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

    bool hasGoodElec = false;
    bool hasGoodMuons = false;

    vector<double> nonTrigMVAThresholds_;
    vector<double> nonTrigMVAEtaCuts_;

    double electronIsoThreshold_;
    double electronNumOfHitsThreshold_;
    vector<double> electronEtaThresholds_;
    bool useElectronMVARecipe_;
    bool useElectronLooseID_;
    bool doHHWWggTagCutFlowAnalysis_;
    bool SaveOthers_;
    bool Savejet;
    //FullLep
    double lep1ptThre_;
    double lep2ptThre_;
    double lep3ptThre_;
    double DiLepMassThre_;
    double DiLepPtThre_;
    double MassTThre_;
    double MassT_l2Thre_;
    double sig_mass_low_;
    double sig_mass_high_;
    double btagThresh_;
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
    cc_( consumesCollector() ), // need absence of comma on last entry 
    globalVariablesComputer_(pSet.getParameter<edm::ParameterSet>("globalVariables"), cc_)
    // idSelector_( pSet.getParameter<ParameterSet> ( "idSelection" ), cc_ )
    {

      inputDiPhotonName_= pSet.getParameter<std::string > ( "DiPhotonName" );
      inputDiPhotonSuffixes_= pSet.getParameter<std::vector<std::string> > ( "DiPhotonSuffixes" );
      std::vector<edm::InputTag>  diPhotonTags;
      for (auto & suffix : inputDiPhotonSuffixes_){ 
          systematicsLabels.push_back(suffix);
          std::string inputName = inputDiPhotonName_;
          inputName.append(suffix);
          if (!suffix.empty()) diPhotonTags.push_back(edm::InputTag(inputName));
          else  diPhotonTags.push_back(edm::InputTag(inputDiPhotonName_));
      }
      for( auto & tag : diPhotonTags ) { diPhotonTokens_.push_back( consumes<edm::View<flashgg::DiPhotonCandidate> >( tag ) ); }

      bool breg = 0;

      inputJetsName_= pSet.getParameter<std::string> ( "JetsName" );
      inputJetsCollSize_= pSet.getParameter<unsigned int> ( "JetsCollSize" );
      inputJetsSuffixes_= pSet.getParameter<std::vector<std::string> > ( "JetsSuffixes" );
      // cout << "inputJetsCollSize_ = " << inputJetsCollSize_ << endl;
      if (breg){
        std::vector<edm::InputTag>  jetTags; // With bregression on /T
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
      indexes = fs->make<TH1F> ("indexes","indexes",5,0,5);
      diPhotons_size_h = fs ->make<TH1F> ("diPhotons_size_h","diPhotons_size_h",20,-1,19);
      diphoton_idx_h = fs ->make<TH1F> ("diphoton_idx_h","diphoton_idx_h",20,0,20);
      electron_cut = fs ->make<TH1F> ("electron_cut","electron_cut",5,0,5);
      lepPt2DCut = fs ->make<TH2D> ("lepPt2DCut","lepPt2DCut",50,0,50,70,0,70);
      MVA_2D_Cut = fs->make<TH2D> ("MVA_2D_Cut","MVA_2D_Cut",200,-1.,1.,200,-1.,1.);
      MVA_2D_Cut_lep_cut = fs->make<TH2D> ("MVA_2D_Cut_lep_cut","MVA_2D_Cut_lep_cut",200,-1.,1.,200,-1.,1.);
      num_MetPtCut = fs->make<TH1F> ("num_MetPtCut","num_MetPtCut",70,0,70);
      num_MetPtCut_lep_cut = fs->make<TH1F> ("num_MetPtCut_lep_cut","num_MetPtCut_lep_cut",70,0,70);
      passDR_num = fs->make<TH1F> ("passDR_num","passDR_num",100,0.,1.);
      DiPhotonBDT_Scan = fs->make<TH1F> ("DiPhotonBDT_Scan","DiPhotonBDT_Scan",200,-1,1);
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

      nonTrigMVAThresholds_ =  pSet.getParameter<vector<double > >( "nonTrigMVAThresholds");
      nonTrigMVAEtaCuts_ =  pSet.getParameter<vector<double > >( "nonTrigMVAEtaCuts");
      electronIsoThreshold_ = pSet.getParameter<double>( "electronIsoThreshold");
      electronNumOfHitsThreshold_ = pSet.getParameter<double>( "electronNumOfHitsThreshold");
      electronEtaThresholds_ = pSet.getParameter<vector<double > >( "electronEtaThresholds");
      useElectronMVARecipe_=pSet.getParameter<bool>("useElectronMVARecipe");
      useElectronLooseID_=pSet.getParameter<bool>("useElectronLooseID");
      SaveOthers_ = pSet.getParameter<bool>("SaveOthers"); 
      btagThresh_ = pSet.getParameter<double>( "btagThresh");
      //FullLep
      lep1ptThre_ = pSet.getParameter<double>("lep1ptThre");//means > lep1pt
      lep2ptThre_ = pSet.getParameter<double>("lep2ptThre");//means > lep2pt
      lep3ptThre_ = pSet.getParameter<double>("lep3ptThre");//means < lep3pt
      DiLepPtThre_ = pSet.getParameter<double>("DiLepPtThre");
      DiLepMassThre_ = pSet.getParameter<double>("DiLepMassThre");
      MassTThre_ = pSet.getParameter<double>("MassTThre");
      MassT_l2Thre_ = pSet.getParameter<double>("MassT_l2Thre");
      sig_mass_high_ = pSet.getParameter<double>("sig_mass_high");
      sig_mass_low_ = pSet.getParameter<double>("sig_mass_low");

      doHHWWggTagCutFlowAnalysis_ = pSet.getParameter<bool>( "doHHWWggTagCutFlowAnalysis");
      // produces<vector<HHWWggTag>>();
      for (auto & systname : systematicsLabels) {
          produces<vector<HHWWggTag>>(systname);
      }
      produces<vector<TagTruthBase>>();
      // cout << "**************************** in HHWWggTagProducer.cc **********************************************" << endl;
    }

    // bool HHWWggTagProducer::PassMVASelections()
    // {
// 
    // }
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

      // Get eta values
      leading_pho_eta = leading_photon->p4().eta();
      sub_leading_pho_eta = subleading_photon->p4().eta();

      // leading photon 
      // EB 
      if (( abs(leading_pho_eta) > 0) && ( abs(leading_pho_eta) < 1.4442)){
        // if (lead_pho_EG_MVA_ > 0.42) lead_pass_TightPhoID = 1; 
        if (lp_Hgg_MVA > EB_MVA) lead_pass_TightPhoID = 1; 
      }

      // EE 
      else if (( abs(leading_pho_eta) > 1.566) && ( abs(leading_pho_eta) < 2.5)){
        // if (lead_pho_EG_MVA_ > 0.14) lead_pass_TightPhoID = 1;
        if (lp_Hgg_MVA > EE_MVA) lead_pass_TightPhoID = 1;
      }

      // SubLeading Photon
      // EB 
      if (( abs(sub_leading_pho_eta) > 0) && ( abs(sub_leading_pho_eta) < 1.4442)){
        // if (sublead_pho_EG_MVA_ > 0.42) sublead_pass_TightPhoID = 1; 
        if (slp_Hgg_MVA > EB_MVA) sublead_pass_TightPhoID = 1; 
      }

      // EE 
      else if (( abs(sub_leading_pho_eta) > 1.566) && ( abs(sub_leading_pho_eta) < 2.5)){
        // if (sublead_pho_EG_MVA_ > 0.14) sublead_pass_TightPhoID = 1;
        if (slp_Hgg_MVA > EE_MVA) sublead_pass_TightPhoID = 1;
      }

      if (lead_pass_TightPhoID && sublead_pass_TightPhoID){
        return 1;
    }

    else return 0; 

    }

    void HHWWggTagProducer::produce( Event &event, const EventSetup & )
    {
      cout<<"start Tag HHWWggTagProducer"<<endl;
      // cout << "[HHWWggTagProducer.cc] - Beginning of HHWWggTagProducer::produce" << endl;
      Savejet=1;
      // update global variables
      globalVariablesComputer_.update(event);

      // Get particle objects
      event.getByToken( photonToken_, photons );
      event.getByToken( diphotonToken_, diPhotons );
      // event.getByToken( genParticleToken_, genParticle );
      event.getByToken( electronToken_, electrons );
      event.getByToken( muonToken_, muons );
      event.getByToken( METToken_, METs );
      event.getByToken( mvaResultToken_, mvaResults );
      event.getByToken( vertexToken_, vertices );
      event.getByToken( rhoTag_, rho);

      double rho_    = *rho;

      // Set cut booleaGns
      // std::vector<double> Cut_Results = {1.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0}; // Cut_Results[i] = 1: Event Passed Cut i 
      std::vector<double> Cut_Variables(20,0.0); // Cut_Results[i] = 1.0: Event Passed Cut i 
      std::vector<double> Vertex_Variables(20,0.0); // Cut_Results[i] = 1.0: Event Passed Cut i 

      // Cut Variables 
      int n_METs = METs->size(); // Should be 1, but using as a way to obtain met four vector 
      int n_good_electrons = 0;
      int n_good_muons = 0;
      int n_good_leptons = 0;
      //chuw//int n_good_jets = 0;
      bool hasHighbTag = 0;
      float btagVal = 0;
      double diphoMVA = -99;
      double lead_pho_Hgg_MVA = -99, sublead_pho_Hgg_MVA = -99;
      double CMS_hgg_mass = -99;
      bool passMVAs = 0; // True if leading and subleading photons pass MVA selections 

      // Saved Objects after selections
      std::vector<flashgg::Jet> tagJets_;
      std::vector<flashgg::Muon> goodMuons_;
      std::vector<flashgg::Electron> goodElectrons_; 
      std::vector<flashgg::Met> theMET_;
      std::vector<flashgg::DiPhotonCandidate> diphoVector_;
      reco::GenParticle::Point genVertex;

      std::unique_ptr<vector<TagTruthBase> > truths( new vector<TagTruthBase> );
      edm::RefProd<vector<TagTruthBase> > rTagTruth = event.getRefBeforePut<vector<TagTruthBase> >();

//-----------------------------------------------------------------------------------------------------------

      // Vertex variables
      // double gen_vertex_z = -999;
      // double hgg_vertex_z = -999;
      double zero_vertex_z = -999;
      // double vertex_diff_zeroeth = -999;
      // double vertex_diff_hgg = -999;
      // double num_vertices = -999;

      int diphoton_vertex_index = -99;
      // const edm::Ptr<reco::Vertex> dipho_vertex;
      edm::Ptr<reco::Vertex> diphoton_vertex;
      edm::Ptr<reco::Vertex> zero_vertex;
      // num_vertices = (double)vertices->size();
      // cout << "vertices->size() = " << vertices->size() << endl;
      if (vertices->size() > 0){
        zero_vertex = vertices->ptrAt( 0 );
      }

      // MC truth
      TagTruthBase truth_obj;
      // double genMhh=0.;
      if( ! event.isRealData() ) {
          Handle<View<reco::GenParticle> > genParticles;
          std::vector<edm::Ptr<reco::GenParticle> > selHiggses;
          event.getByToken( genParticleToken_, genParticles );
          reco::GenParticle::Point higgsVtx(0.,0.,0.);
          for( unsigned int genLoop = 0 ; genLoop < genParticles->size(); genLoop++ ) {
              int pdgid = genParticles->ptrAt( genLoop )->pdgId(); 
              // if( pdgid == 25 || pdgid == 22 ) { // not so sure if this is correct for HHWWgg because of potential photons from hadronization 
              if( pdgid == 25 ) { 
                  higgsVtx = genParticles->ptrAt( genLoop )->vertex();
                  // gen_vertex_z = higgsVtx.z();
                  break;
              }
          }
          for( unsigned int genLoop = 0 ; genLoop < genParticles->size(); genLoop++ ) {
              edm::Ptr<reco::GenParticle> genPar = genParticles->ptrAt(genLoop);
              if (selHiggses.size()>1) break;
            if (genPar->pdgId()==25 && genPar->isHardProcess()){
                selHiggses.push_back(genPar);
            }   
          }
          truth_obj.setGenPV( higgsVtx );
          truths->push_back( truth_obj );
      }

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
      for( unsigned int i = 0; i < triggerNames.triggerNames().size(); i++ )
          {
              if(!triggerBits->accept(i))
                  for(size_t j=0;j<flagList.size();j++)
                      {
                          if(flagList[j]==triggerNames.triggerName(i))
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

      double num_FL_dr = 0;
      double num_FH_dr = 0;
      float dr_ll = 0;

      if(doHHWWggTagCutFlowAnalysis_) {Cut_Variables[0] = 1.0;}
      //for(int i =0;i<5;i++){
      //diphoton_idx_h->Fill(i);}
      // read diphotons
      //chuw/for (unsigned int diphoton_idx = 0; diphoton_idx < diPhotonTokens_.size(); diphoton_idx++) { //looping over all diphoton systematics
        // cout << "diphoton_idx = " << diphoton_idx << endl;
        //diphoton_idx_h->Fill(diphoton_idx);
        Handle<View<flashgg::DiPhotonCandidate> > diphotons;
        event.getByToken( diphotonToken_, diPhotons ); // for each diphoton systematic 
      
        // event.getByToken( diphotonToken_, diphotons );
        //unsigned int loopOverJets = 1;
        //if (inputDiPhotonSuffixes_[diphoton_idx].empty()) loopOverJets = inputJetsSuffixes_.size();
        //chuw for (unsigned int jet_col_idx = 0; jet_col_idx < loopOverJets; jet_col_idx++) {//looping over all jet systematics, only for nominal diphotons
          // cout << "jet_col_idx = " << jet_col_idx << endl;
        std::unique_ptr<vector<HHWWggTag> > tags( new vector<HHWWggTag> );

        //diPhotons_size_h->Fill(diPhotons->size());
        if (diPhotons->size() > 0){ // for each systematic 
        // if (diphotons->size() > 0){
        for( unsigned int diphoIndex = 0; diphoIndex < 1; diphoIndex++ ) { // only look at highest pt dipho
          // std::unique_ptr<vector<HHWWggTag> > tags( new vector<HHWWggTag> );
          // edm::Ptr<flashgg::DiPhotonCandidate> dipho = Corrdiphoton;
          //diPhotons_size_h->Fill(diphoIndex);
          edm::Ptr<flashgg::DiPhotonCandidate> dipho = diPhotons->ptrAt( diphoIndex ); // systematic loop
          // edm::Ptr<flashgg::DiPhotonCandidate> dipho = diphotons->ptrAt( diphoIndex ); 
          //diPhotons_size_h->Fill(diphoIndex);
          edm::Ptr<flashgg::DiPhotonMVAResult> mvares = mvaResults->ptrAt( diphoIndex );    
          // cout << "dipho energy1 = " << dipho->genP4().E() << endl; 
          diphoton_vertex = dipho->vtx();
          diphoton_vertex_index = dipho->vertexIndex();
          // cout << "vertex index = " << diphoton_vertex_index << endl;
          indexes->Fill(diphoton_vertex_index);
          //indexes->Fill(diphoIndex); 
          //int i = 1;
          //diPhotons_size_h->Fill(i);
          //diphoton_idx_h->Fill(i);
          if(dipho->mass()<180. && dipho->mass()>100.){
          diphoton_idx_h->Fill(1);
          }
          // kinematic cuts on diphotons
          const flashgg::Photon* leadPho = dipho->leadingPhoton();
          const flashgg::Photon* subleadPho = dipho->subLeadingPhoton();

          diphoton_vertex = dipho->vtx(); 
          cout<<"dipho"<<endl;
          if(doHHWWggTagCutFlowAnalysis_ && 1){
            cout<<"stat mva scan"<<endl;
          for(double i=-1.;i<1.;i=i+0.01){
            double EB_MVA=i;
            for(double j=-1.;j<1.;j=j+0.01){
            double EE_MVA=j;
          passMVAs = 0;
          passMVAs = checkPassMVAs(leadPho, subleadPho, diphoton_vertex, EB_MVA, EE_MVA);
          if(passMVAs && ((dipho->mass()>100. && dipho->mass()<sig_mass_low_) || (dipho->mass()<180. && dipho->mass()>sig_mass_high_))){
          MVA_2D_Cut->Fill(EB_MVA,EE_MVA);
          }
            
            }
          }
          }
          passMVAs = 0;
          passMVAs = checkPassMVAs(leadPho, subleadPho, diphoton_vertex, EB_MVA_Threshold_, EB_MVA_Threshold_);
          
         /* if(doHHWWggTagCutFlowAnalysis_){
          for(double i=-1.;i<1.;i=i+0.01){
            double EB_MVA=i;
            for(double j=-1.;j<1.;j=j+0.01){
            double EE_MVA=j;
          passMVAs = 0;
          passMVAs = checkPassMVAs(leadPho, subleadPho, diphoton_vertex, EB_MVA, EE_MVA);
          if(passMVAs && n_good_leptons == 2)
          MVA_2D_Cut_lep_cut->Fill(EB_MVA,EE_MVA);
          
            
            }
          }
          }*/
          if(doHHWWggTagCutFlowAnalysis_ && 1){
                      passMVAs = 0;
                      passMVAs = checkPassMVAs(leadPho, subleadPho, diphoton_vertex, EB_MVA_Threshold_, EE_MVA_Threshold_);
                      if(!passMVAs) {Cut_Variables[1] = 0.0;
                      diPhotons_size_h->Fill(0);
                      }

                      else {Cut_Variables[1] = 1.0;
                      diPhotons_size_h->Fill(1);// passed photon MVAs (and all photon selections)
                    }}
          else{       
                      passMVAs = 0;
                      passMVAs = checkPassMVAs(leadPho, subleadPho, diphoton_vertex, EB_MVA_Threshold_, EE_MVA_Threshold_);
                      if(!passMVAs) continue; // Do not save event if leading and subleading photons don't pass MVA cuts 
                    }      

          hasGoodElec = false;
          hasGoodMuons = false;
          
          // Electrons 
          std::vector<edm::Ptr<Electron> > goodElectrons = selectStdElectrons( electrons->ptrs(), dipho, vertices->ptrs(), leptonPtThreshold_, electronEtaThresholds_,
                                                                            useElectronMVARecipe_,useElectronLooseID_,
                                                                            deltaRPhoElectronThreshold_,DeltaRTrkElec_,deltaMassElectronZThreshold_,
                                                                            rho_, event.isRealData() );

          // Muons                                                                   
          std::vector<edm::Ptr<flashgg::Muon> > goodMuons = selectMuons( muons->ptrs(), dipho, vertices->ptrs(), muonEtaThreshold_, leptonPtThreshold_,
          muPFIsoSumRelThreshold_, deltaRMuonPhoThreshold_, deltaRMuonPhoThreshold_ );
          
          n_good_electrons = goodElectrons.size();
          n_good_muons = goodMuons.size();
          n_good_leptons = n_good_electrons + n_good_muons;
          hasGoodElec = ( goodElectrons.size() > 0 );
          hasGoodMuons = ( goodMuons.size() > 0 );
                
          // FL: Require at dr(l,l) > 0.4 
          // For which pairs of the >=2 good leptons should dr be greater than 4?
          if(doHHWWggTagCutFlowAnalysis_ && 1){
          cout<<"start MVA 2lep scan"<<endl;
            if(n_good_leptons ==2 ){
          for(double i=-1.;i<1.;i=i+0.01){
          double EB_MVA=i;
          for(double j=-1.;j<1.;j=j+0.01){
          double EE_MVA=j;
          passMVAs = 0;
          passMVAs = checkPassMVAs(leadPho, subleadPho, diphoton_vertex, EB_MVA, EE_MVA);
          if(passMVAs && ((dipho->mass()>100. && dipho->mass()<sig_mass_low_) || (dipho->mass()<180. && dipho->mass()>sig_mass_high_)))
          MVA_2D_Cut_lep_cut->Fill(EB_MVA,EE_MVA);
          }
          }
          }
          }
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
          unsigned int jetCollectionIndex = diPhotons->at( diphoIndex ).jetCollectionIndex(); // diphoton collection for each systematic 
          // unsigned int jetCollectionIndex = diphotons->at( diphoIndex ).jetCollectionIndex();
          size_t vtx = (size_t)dipho->jetCollectionIndex();
          edm::Handle<edm::View<flashgg::Jet> > Jets_;

//          unsigned int jet_token_index = jet_col_idx*inputJetsCollSize_+vtx;
          // cout << "jet_token_index = " << jet_token_index << endl;
          // cout << "jetCollectionIndex = " << jetCollectionIndex << endl;

          event.getByToken( jetTokens_[jetCollectionIndex], Jets_); // testing 
          // event.getByToken( jetTokens_[jet_col_dx*inputJetsCollSize_+vtx], Jets_);  //take the corresponding vertex of current systematic //WORKS 
          // cout << "right after getbytoken jettokens jets" << endl;

          std::vector<edm::Ptr<Jet> > tagJets;
          Savejet=1;
        cout<<"Savejet"<<Savejet<<"Jets:"<<Jets_->size()<<endl;
          // Jet Selections
          if(Jets_->size()>0){
          for( unsigned int candIndex_outer = 0; candIndex_outer <  Jets_->size() ; candIndex_outer++ ) 
              {
                  bool keepJet=true; 
                  edm::Ptr<flashgg::Jet> thejet = Jets_->ptrAt( candIndex_outer );
                  // if(!thejet->passesJetID  ( flashgg::Tight2017 ) ) { continue; }
                  if(thejet->passesJetID  ( flashgg::Tight2017 ) ) {
                      // number_passed_jetid += 1;
                    }
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
             hasHighbTag = 0;
                  for (unsigned int j = 0; j < tagJets.size(); j++){
                  Ptr<flashgg::Jet> jet_ = tagJets[j];
           //       btagVal = jet_->bDiscriminator("pfDeepCSVJetTags:probb") + jet_->bDiscriminator("pfDeepCSVJetTags:probbb");
              btagVal = jet_->bDiscriminator("mini_pfDeepFlavourJetTags:probb");
                  if (jet_->pt()>20 && btagVal > btagThresh_) hasHighbTag = 1;
                  }
            cout<<"HasbJet:"<<hasHighbTag<<endl;
            if(doHHWWggTagCutFlowAnalysis_){
                      }
            else {if(hasHighbTag){ 

            cout<<"b tag judje"<<endl;
             Savejet=0;}
              };
          }
          cout<<"has high b jet ?"<<Savejet<<endl;
         // chuw//n_good_jets = tagJets.size();

          // FH: Require at least one delta r pair with dr > 0.4 
          /*float dr_jj = 0;
          // bool one_FH_dr = false;
          if (tagJets.size() >= 2){
            for (unsigned int ji = 0; ji < tagJets.size() - 1; ji++){
              Ptr<flashgg::Jet> jeti = tagJets[ji];
              for (unsigned int jj = ji + 1; jj < tagJets.size(); jj++){
                Ptr<flashgg::Jet> jetj = tagJets[jj];
                dr_jj = deltaR(jeti->eta(), jeti->phi(), jetj->eta(), jetj->phi()); 
                if (dr_jj > 0.4){ 
                  // one_FH_dr = true;
                  num_FH_dr += 1.0;
                  // break; 
                } 
              }
            }
          }*/

          // MET 
          if( METs->size() != 1 ) { std::cout << "WARNING - #MET is not 1" << std::endl;}
          Ptr<flashgg::Met> theMET = METs->ptrAt( 0 );
          if(doHHWWggTagCutFlowAnalysis_){
          if(n_good_leptons >= 2) 
          Cut_Variables[7] = 1.0; 
          else 
          Cut_Variables[7] = 0.0;  
          }
          if(doHHWWggTagCutFlowAnalysis_){
          if(n_good_leptons == 1) 
          Cut_Variables[8] = 1.0; 
          else 
          Cut_Variables[8] = 0.0;  
          }
          if(doHHWWggTagCutFlowAnalysis_){
          if(n_good_leptons>0) 
          Cut_Variables[9] = 1.0; 
          else 
          Cut_Variables[9] = 0.0;  
          }
          if(doHHWWggTagCutFlowAnalysis_){
                        if(num_FL_dr>=1) 
                          Cut_Variables[10] = 1.0; 
                        else 
                          Cut_Variables[10] = 0.0;  
                      }
          if(doHHWWggTagCutFlowAnalysis_ && 0){
          for(float i=0.0;i<=70.0;i++){
            if(theMET->getCorPt() >= i && n_good_leptons==2)
            num_MetPtCut_lep_cut->Fill(i);
          //if(theMET->getCorPt() >= MetPtThreshold_)
            //Cut_Variables[12] = 1.0;
          //else 
            //Cut_Variables[12] = 0.0;
          }
          }
          if(doHHWWggTagCutFlowAnalysis_ && 1){
          for(float i=0.0;i<=70.;i++){
            if(theMET->getCorPt() >= i && ((dipho->mass()<180. && dipho->mass()>sig_mass_high_) || (dipho->mass()<sig_mass_low_ && dipho->mass()>100.)))
            num_MetPtCut->Fill(i);
          //if(theMET->getCorPt() >= MetPtThreshold_)
            //Cut_Variables[12] = 1.0;
          //else 
            //Cut_Variables[12] = 0.0;
          }
          }
          if(doHHWWggTagCutFlowAnalysis_ && 1){
          for(float i=-1.;i<=1.;i=i+0.01){
            diphoMVA=mvares->result;
          if(diphoMVA >= i && ((dipho->mass()<180. && dipho->mass()>sig_mass_high_) || (dipho->mass()<sig_mass_low_ && dipho->mass()>100.))){
           DiPhotonBDT_Scan->Fill(i); 
          } 
          }

          }
          if(doHHWWggTagCutFlowAnalysis_ && 0){
          for(int i=0;i<=5;i++){
            if(n_good_leptons >=i )
          electron_cut->Fill(i);
          }
          }
          /*if(doHHWWggTagCutFlowAnalysis_){
          for(float i=0.0;i<=deltaRLeps_;i=i+0.01){
            if(passDR_num >= i)
            passDR_num->Fill(i);
          //if(theMET->getCorPt() >= MetPtThreshold_)
            //Cut_Variables[12] = 1.0;
          //else 
            //Cut_Variables[12] = 0.0;
          }
          }*/
         



          if(doHHWWggTagCutFlowAnalysis_){
          if(theMET->getCorPt() >= MetPtThreshold_)
            Cut_Variables[12] = 1.0;
          else
            Cut_Variables[12] = 0.0;
          }
          //-- Tag object 
          if ( (n_good_leptons >=2 ) && (theMET->getCorPt() >= MetPtThreshold_) && num_FL_dr>=1 ){
            int catnum = 0;
            
            //diPhotons_size_h->Fill(1);
            Ptr<flashgg::Met> theMET = METs->ptrAt( 0 );
     
            if(doHHWWggTagCutFlowAnalysis_){
                        if(n_good_leptons == 2) 
                          Cut_Variables[11] = 1.0; 
                        else 
                          Cut_Variables[11] = 0.0;  
                      }
            if(doHHWWggTagCutFlowAnalysis_){
                        if(n_good_electrons == 2)
                          Cut_Variables[4] = 1.0;
                        else 
                          Cut_Variables[4] = 0.0;
            }
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
              if(doHHWWggTagCutFlowAnalysis_ && 1){
                 cout<<"start lep pt scan"<<endl;
                 cout<<tag_electron2->pt()<<tag_electron1->pt()<<endl;
              for(double i=0.;i<=min(50.,tag_electron2->pt());i++){
                cout<<i<<endl;
                for(double j=i;j<=min(70.,tag_electron1->pt());j++ ){
                if(tag_electron2->pt() >= i && tag_electron1->pt() >= j && ((dipho->mass()<180. && dipho->mass()>sig_mass_high_) || (dipho->mass()<sig_mass_low_ && dipho->mass()>100.)))
                lepPt2DCut->Fill(i,j);
                }
              }
              }
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
                tags->push_back( tag_obj ); 
                cout<<"ele push back"<<endl;
                if( ! event.isRealData() ) {
                  tags->back().setTagTruth( edm::refToPtr( edm::Ref<vector<TagTruthBase> >( rTagTruth, 0 ) ) );                 
                }  
                }
            }
            if(doHHWWggTagCutFlowAnalysis_){
                        if(n_good_muons == 2) 
                          Cut_Variables[5] = 1.0; 
                        else 
                          Cut_Variables[5] = 0.0;  
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

              if(doHHWWggTagCutFlowAnalysis_ && 1){
                 cout<<"start lep pt scan"<<endl;
                 cout<<tag_muon2->pt()<<tag_muon1->pt()<<endl;;
              for(double i=0.;i<=min(tag_muon2->pt(),50.);i++){
              for(double j=i;j<=min(tag_muon1->pt(),70.);j++){
                cout<<j<<endl;
if(tag_muon2->pt() >= i && tag_muon1->pt() >= j && ((dipho->mass()<sig_mass_low_ && dipho->mass()>100.) || (dipho->mass()<180. && dipho->mass()>sig_mass_high_)) )
  lepPt2DCut->Fill(i,j);
              }
              }
              }
              MassT= sqrt(2*DiMuLV.Pt()*theMET->pt()*(1-cos(abs(DiMuLV.Phi()-theMET->phi()))));
              MassT_l2 = sqrt(2*Mu2LV.Pt()*theMET->pt()*(1-cos(abs(Mu2LV.Phi()-theMET->phi()))));
                if((tag_muon1->charge()*tag_muon2->charge() == -1) && tag_muon1->pt()>=lep1ptThre_ && tag_muon2->pt()>=lep2ptThre_  && DiMuLV.Pt()>DiLepPtThre_ && DiMuLV.M()>DiLepMassThre_ && MassT>MassTThre_ && MassT_l2>MassT_l2Thre_ && Save == 1. && Savejet==1){
                HHWWggTag tag_obj(dipho, tag_muon1, tag_muon2, theMET,Cut_Variables);  
                tag_obj.setSystLabel(systLabel_);

                tag_obj.setDiPhotonIndex( diphoIndex );           
                tag_obj.setMVA( -0.9 );
                tag_obj.setCategoryNumber( 0 );
                tags->push_back( tag_obj ); 

                if( ! event.isRealData() ) {
                  tags->back().setTagTruth( edm::refToPtr( edm::Ref<vector<TagTruthBase> >( rTagTruth, 0 ) ) );                 
                }  
            }           
           }

                if(doHHWWggTagCutFlowAnalysis_ && 1){
                        if(n_good_electrons == 1 && n_good_muons == 1) 
                        {Cut_Variables[6] = 1.0; }
                        else {
                          Cut_Variables[6] = 0.0;}
                        diphoton_idx_h->Fill(1);
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

              if(doHHWWggTagCutFlowAnalysis_ && 1){
                cout<<"start lep pt scan"<<endl;
              for(double i=0.;i<50.;i++){
              for(double j=i;j<70.;j++){
              if(tag_muon1->pt()>tag_electron1->pt()){
              if(tag_electron1->pt() >= i && tag_muon1->pt() >= j && ((dipho->mass()>100. && dipho->mass()<sig_mass_low_) || (dipho->mass()<180. && dipho->mass()>sig_mass_high_))){
              lepPt2DCut->Fill(i,j);
              }
              }
              else{
              if(tag_muon1->pt() >= i && tag_electron1->pt() >= j && ((dipho->mass()>100. && dipho->mass()<sig_mass_low_) || (dipho->mass()<180. && dipho->mass()>sig_mass_high_))){
              lepPt2DCut->Fill(i,j);
              }
              }
              }
              }
              }
              if((((tag_electron1->pt()>=lep1ptThre_) && (tag_muon1->pt()>=lep2ptThre_))||((tag_muon1->pt()>=lep1ptThre_) && (tag_electron1->pt()>=lep2ptThre_))) && (tag_muon1->charge()*tag_electron1->charge()==-1) && (DiLepLV.M()>DiLepMassThre_) && (DiLepLV.Pt()>DiLepPtThre_) && (MassT_l2>MassTThre_) && (MassT>MassT_l2Thre_) && (Save==1.) &&(Savejet==1)){
                HHWWggTag tag_obj(dipho, tag_electron1, tag_muon1, theMET,Cut_Variables);
                tag_obj.setSystLabel(systLabel_);
                cout<<"MassT:"<<MassT<<"DiLeppt:"<<DiLepLV.Pt()<<endl;
                tag_obj.setDiPhotonIndex( diphoIndex );           
                tag_obj.setMVA( -0.9 );
                tag_obj.setCategoryNumber( 0 );
                tags->push_back( tag_obj ); 

                if( ! event.isRealData() ) {
                  tags->back().setTagTruth( edm::refToPtr( edm::Ref<vector<TagTruthBase> >( rTagTruth, 0 ) ) );                 
                }  
            }
          } 
          }//
          else if(n_good_leptons ==1 ) 
          {
            if(doHHWWggTagCutFlowAnalysis_ && SaveOthers_)
            { 
               //diPhotons_size_h->Fill(1);
               //Ptr<flashgg::Muon> tag_muon = goodMuons[0];
               if(n_good_electrons == 1 )
              {int catnum =3;
               Ptr<flashgg::Electron> tag_electron = goodElectrons[0];
               HHWWggTag tag_obj(dipho,tag_electron,Cut_Variables);
               //HHWWggTag tag_obj(dipho,Cut_Variables);
//               std::cout<<"dipho1"<<std::endl;

               //diPhotons_size_h->Fill(Cut_Variables[0]);
               //HHWWggTag tag_obj(Cut_Variables);
               //tag_obj.setSystLabel( inputDiPhotonSuffixes_[diphoton_idx] );
               tag_obj.setDiPhotonIndex( diphoIndex );
               tag_obj.setMVA( -0.9 );
               tag_obj.setCategoryNumber( catnum );
               tags->push_back( tag_obj );
               if( ! event.isRealData() ) {
               tags->back().setTagTruth( edm::refToPtr( edm::Ref<vector<TagTruthBase> >( rTagTruth, 0 ) ) );
              }
              }
            
              if(n_good_muons ==1 ){
                int catnum =3;
               Ptr<flashgg::Muon> tag_muon = goodMuons[0];
               HHWWggTag tag_obj(dipho,tag_muon,Cut_Variables);
               tag_obj.setDiPhotonIndex( diphoIndex );
               tag_obj.setMVA( -0.9 );
               tag_obj.setCategoryNumber( catnum );
               tags->push_back( tag_obj );
               if( ! event.isRealData() ) {
               tags->back().setTagTruth( edm::refToPtr( edm::Ref<vector<TagTruthBase> >( rTagTruth, 0 ) ) );
               }
               }
            }
          }
  //        diPhotons_size_h->Fill(Cut_Variables[0]);
          //}

          else 
          {
          if(doHHWWggTagCutFlowAnalysis_ && SaveOthers_)
          {  int catnum =3;
          HHWWggTag tag_obj(dipho,Cut_Variables);
          tag_obj.setDiPhotonIndex( diphoIndex );
          tag_obj.setMVA( -0.9 );
          tag_obj.setCategoryNumber( catnum );
          tags->push_back( tag_obj );
       //   cout<< tag_obj<<endl;
          if( ! event.isRealData() ) {
          tags->back().setTagTruth( edm::refToPtr( edm::Ref<vector<TagTruthBase> >( rTagTruth, 0 ) ) );
            }
          }
          
          } 
        } // Diphoton loop //add cut flow below this line 
      } // if at least 1 PS diphoton 
      event.put( std::move( tags ) );
      cout<<"tag_obj has moved"<<endl;
        //}// looping over jet systematics 
    // cout << "Just left looping over jet systematics" << endl;

     //looping over all diphoton systematics
    // cout << "Just left looping over all diphoton systematics" << endl;

      event.put( std::move( truths ) );

    } // HHWWggTagProducer::produce

  } // namespace flashgg

  typedef flashgg::HHWWggTagProducer FlashggHHWWggTagProducer;
  DEFINE_FWK_MODULE( FlashggHHWWggTagProducer );
