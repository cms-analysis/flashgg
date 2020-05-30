// Abe Tishelman-Charny
// November 2019
// Derived from HH->WWgg event dumper and HH->bbgg tagger 

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
    // TH1F* btags;

  private:
    double genTotalWeight;
    bool checkPassMVAs(const flashgg::Photon*& leading_photon, const flashgg::Photon*& subleading_photon, edm::Ptr<reco::Vertex>& diphoton_vertex);
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

    // std::vector<edm::EDGetTokenT<View<flashgg::Jet> > > JetToken_;


    std::vector< std::string > systematicsLabels;
    std::vector<std::string> inputDiPhotonSuffixes_;

    //---ID selector
    ConsumesCollector cc_;
    GlobalVariablesComputer globalVariablesComputer_;
    // CutBasedDiPhotonObjectSelector idSelector_;

    //----output collection
    // auto_ptr<vector<HHWWggCandidate> > HHWWggColl_;

    // variables from WHLeptonicTagProducer
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
    // string bTag_;
    double btagThresh_;
    bool doHHWWggTagCutFlowAnalysis_;


    edm::InputTag genInfo_;
    edm::EDGetTokenT<GenEventInfoProduct> genInfoToken_;
  };

  //---constructors
  // HHWWggTagProducer::HHWWggTagProducer( ):
  // photonToken_(),
  // diphotonToken_()
  // genParticleToken_(),
  // electronToken_(),
  // muonToken_(),
  // METToken_(),
  // cc_( consumesCollector() )
  // // idSelector_( ParameterSet(), cc_ )

  // {}

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
      // diphoton_idx_h = fs->make<TH1F> ("diphoton_idx_h","diphoton_idx_h",20,0,20); 
      // diPhotons_size_h = fs->make<TH1F> ("diPhotons_size_h","diPhotons_size_h",20,0,20); 

      // indexes = fs->make<TH1F> ("indexes","indexes",5,0,5);
      // btags = fs->make<TH1F> ("btags","btags",100,0,1);

      // numEvents = fs->make<TH1F> ("numEvents","numEvents",1,0,10);

      // gen_weights = fs->make<TH1F> ("gen_weights","gen_weights",1000,-2,2);
      // vars = fs->make<TH1F> ("vars","vars",10,0,10);
      // cutFlow = fs->make<TH1F> ("cutFlow","Cut Flow",10,0,10);
      // WTags = fs->make<TH1F> ("WTags","W Tags",3,0,3);

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
      // bTag_ = pSet.getParameter<string> ( "bTag");
      btagThresh_ = pSet.getParameter<double>( "btagThresh");
      doHHWWggTagCutFlowAnalysis_ = pSet.getParameter<bool>( "doHHWWggTagCutFlowAnalysis");

      produces<vector<HHWWggTag>>();
      // for (auto & systname : systematicsLabels) { // to deal with systematics in producer 
      //     produces<vector<HHWWggTag>>(systname);
      // }
      produces<vector<TagTruthBase>>();
    }

    bool HHWWggTagProducer::checkPassMVAs( const flashgg::Photon*& leading_photon, const flashgg::Photon*& subleading_photon, edm::Ptr<reco::Vertex>& diphoton_vertex){

      bool debug_mva = 0; 

      // MVA Check variables 
      double lp_mva_thresh = 0.07;
      double slp_mva_thresh = -0.03;

      bool lead_pass_TightPhoID = 0, sublead_pass_TightPhoID = 0;
      double lp_Hgg_MVA = -99, slp_Hgg_MVA = -99; 
      double leading_pho_eta = -99, sub_leading_pho_eta = -99;

      // Get MVA values wrt diphoton vertex
      lp_Hgg_MVA = leading_photon->phoIdMvaDWrtVtx( diphoton_vertex ); 
      slp_Hgg_MVA = subleading_photon->phoIdMvaDWrtVtx( diphoton_vertex ); 

      // Get eta values
      leading_pho_eta = leading_photon->p4().eta();
      sub_leading_pho_eta = subleading_photon->p4().eta();

      // Debug
      if(debug_mva){
        cout << "leading mva: " << lp_Hgg_MVA << endl;
        cout << "subleading mva: " << slp_Hgg_MVA << endl;
      }
      // leading photon 
      // EB 
      if (( abs(leading_pho_eta) > 0) && ( abs(leading_pho_eta) < 1.4442)){
        // if (lead_pho_EG_MVA_ > 0.42) lead_pass_TightPhoID = 1; 
        if (lp_Hgg_MVA > lp_mva_thresh) lead_pass_TightPhoID = 1; 
      }

      // EE 
      else if (( abs(leading_pho_eta) > 1.566) && ( abs(leading_pho_eta) < 2.5)){
        // if (lead_pho_EG_MVA_ > 0.14) lead_pass_TightPhoID = 1;
        if (lp_Hgg_MVA > slp_mva_thresh) lead_pass_TightPhoID = 1;
      }

      // SubLeading Photon
      // EB 
      if (( abs(sub_leading_pho_eta) > 0) && ( abs(sub_leading_pho_eta) < 1.4442)){
        // if (sublead_pho_EG_MVA_ > 0.42) sublead_pass_TightPhoID = 1; 
        if (slp_Hgg_MVA > lp_mva_thresh) sublead_pass_TightPhoID = 1; 
      }

      // EE 
      else if (( abs(sub_leading_pho_eta) > 1.566) && ( abs(sub_leading_pho_eta) < 2.5)){
        // if (sublead_pho_EG_MVA_ > 0.14) sublead_pass_TightPhoID = 1;
        if (slp_Hgg_MVA > slp_mva_thresh) sublead_pass_TightPhoID = 1;
      }

      if (lead_pass_TightPhoID && sublead_pass_TightPhoID){
        if(debug_mva) cout << "PASS MVA Selections" << endl;
        return 1;
    }

    else{
      if(debug_mva) cout << "FAIL MVA Selections" << endl;
      return 0; 
    }

    }

    void HHWWggTagProducer::produce( Event &event, const EventSetup & )
    {

      // cout << "[HHWWggTagProducer.cc] - Beginning of HHWWggTagProducer::produce" << endl;

      // update global variables
      // globalVariablesComputer_.update(event);

      // Get particle objects
      event.getByToken( photonToken_, photons );
      event.getByToken( diphotonToken_, diphotons );
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

      // Cut Variables 
      // double has_PS_Dipho = 0, pass_METfilters = 0, dipho_vertex_is_zero = 0, pass_leadPhoOverMassThreshold = 0, pass_subleadPhoOverMassThreshold = 0,
      //   pass_LeadPhoton_MVA = 0, pass_SubLeadPhoton_MVA = 0, pass_dipho_MVA = 0, number_passed_jetid = 0;
      // double dipho_vertex_is_zero = -999;
      // double SLW_Tag = 0.; // Semi-Leptonic W Tag  
      // double FLW_Tag = 0.; // Fully-Leptonic W Tag
      // double FHW_Tag = 0.; // Fully-Hadronic W Tag 
      // bool PS_dipho_tag = 0; // preselected diphoton 

      //---output collection
      // std::unique_ptr<vector<HHWWggCandidate> > HHWWggColl_( new vector<HHWWggCandidate> );
      // std::unique_ptr<vector<HHWWggTag> > tags( new vector<HHWWggTag> );
      // int n_METs = METs->size(); // Should be 1, but using as a way to obtain met four vector 
      int n_good_electrons = 0;
      int n_good_muons = 0;
      int n_good_leptons = 0;
      int n_good_jets = 0;
      bool hasHighbTag = 0;
      float btagVal = 0;
      // double dipho_MVA = -99;
      // double lead_pho_Hgg_MVA = -99, sublead_pho_Hgg_MVA = -99;
      // double CMS_hgg_mass = -99;
      // float bDiscriminatorValue = -2.;

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
      // double zero_vertex_z = -999;
      // double vertex_diff_zeroeth = -999;
      // double vertex_diff_hgg = -999;
      // double num_vertices = -999;

      // int diphoton_vertex_index = -99;
      // const edm::Ptr<reco::Vertex> dipho_vertex;
      edm::Ptr<reco::Vertex> diphoton_vertex;
      edm::Ptr<reco::Vertex> zero_vertex;
      // num_vertices = (double)vertices->size();
      // cout << "vertices->size() = " << vertices->size() << endl;
      if (vertices->size() > 0){
        zero_vertex = vertices->ptrAt( 0 );
      }

      //-- MC truth
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
          // if (selHiggses.size()==2){
          //     TLorentzVector H1,H2;
          //     H1.SetPtEtaPhiE(selHiggses[0]->p4().pt(),selHiggses[0]->p4().eta(),selHiggses[0]->p4().phi(),selHiggses[0]->p4().energy());
          //     H2.SetPtEtaPhiE(selHiggses[1]->p4().pt(),selHiggses[1]->p4().eta(),selHiggses[1]->p4().phi(),selHiggses[1]->p4().energy());
          //     genMhh  = (H1+H2).M();
          // }
          truth_obj.setGenPV( higgsVtx );
          truths->push_back( truth_obj );
      }

      // // Get Gen vertex 
      // bool got_gen_vertex = 0;
      // if (! event.isRealData()){
      //   for( auto &part : *genParticle ) {
      //     if( ( part.pdgId() != 2212 || part.vertex().z() != 0.) && (!got_gen_vertex) ){
      //       genVertex = part.vertex();
      //       gen_vertex_z = genVertex.z();
      //       got_gen_vertex = 1;
      //       // cout << "Gen vertex z: " << genVertex.z() << endl;
      //     }
      //   }
      //   if (!got_gen_vertex){
      //     cout << "**********WARNING: Did not obtain non-zero GEN vertex from GEN particles" << endl;
      //   }
      // }

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

      // bool photonSelection = false;
      // double idmva1 = 0.;
      // double idmva2 = 0.;
      // bool checked_first = false; 
      // Pass_PS = false;
      // bool one_FH_dr = false;
      // bool one_FL_dr = false;
      // double num_FL_dr = 0;
      // double num_FH_dr = 0;
      // float dr_ll = 0;

      // int n_ps_dpho = diphotons->size(); // number of preselected diphotons in event 

      // for( unsigned int diphoIndex = 0; diphoIndex < diphotons->size(); diphoIndex++ ) { // look at all diphotons 

      // If there is one diphoton, and its vertex is not the zeroeth, recompute photon quantities relative to zeroeth vertex 
      // Then create recomputed diphoton object 

      // flashgg::Photon* lpho;
      // flashgg::Photon* slpho;
      // edm::Ptr<flashgg::Photon>
      // edm::Ptr<flashgg::Photon>
      
      // if ( (n_ps_dpho == 1)){
      //   edm::Ptr<flashgg::DiPhotonCandidate> unCorr_diphoton = diphotons->ptrAt( 0 );
      //   // cout << "photons->size() = " << photons->size() << endl;

      //   cout << "++++++++++++++++++++++++++++++++++++" << endl;
      //   for (unsigned int i = 0; i < photons->size(); i++){
      //     edm::Ptr<flashgg::Photon> pton = photons->ptrAt(i);
      //     // cout << "photons->ptrAt(" << i << ") = " << pton << endl;
      //   }
      //   cout << "++++++++++++++++++++++++++++++++++++" << endl;

      //   // edm::Ptr<flashgg::Photon> a = photons->ptrAt(0);

      // for( unsigned int diphoIndex = 0; diphoIndex < diphotons->size(); diphoIndex++ ) { // only look at highest pt dipho
      //   edm::Ptr<flashgg::DiPhotonCandidate> dipho_ = diphotons->ptrAt( diphoIndex );
        // diphoton_vertex_index = dipho_->vertexIndex();
      //   // cout << "vertex index = " << diphoton_vertex_index << endl;
      //   indexes->Fill(diphoton_vertex_index);
      //   // if (diphoton_vertex_index != 0){
      //   //   cout << "********************************************" << endl;
      //   //   cout << "********************************************" << endl;
      //   //   cout << "********************************************" << endl;
      //   //   cout << "diphoton vertex index not 0." << endl;
      //   //   cout << "Index: " << diphoton_vertex_index << endl;
      //   //   cout << "********************************************" << endl;
      //   //   cout << "********************************************" << endl;
      //   //   cout << "********************************************" << endl;
      //   // }
      // }

      // cout << "in HHWWggTagProducer.cc: Right before diphoton loop" << endl;

      // cout << "diphotons->size() = " << diphotons->size() << endl;

      if(doHHWWggTagCutFlowAnalysis_) Cut_Variables[0] = 1.0; // passed diphoton preselection (?) use value to check 
  
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

        // if(diphotons->size() > 1){
        //   cout << "diphotons->size(): " << diphotons->size() << endl;
        //   for (unsigned int diphoIndex = 0; diphoIndex < diphotons->size(); diphoIndex++){
        //     edm::Ptr<flashgg::DiPhotonCandidate> dipho = diphotons->ptrAt( diphoIndex ); 
        //     // cout << "dipho pt = " << dipho->genP4().pt() << endl; 
        //     // cout << "diphoton->genP4().pt(): " << dipho->genP4().pt() << endl;
        //     // cout << "dipho->genP4().pt(): " << dipho->genP4().pt() << endl;
        //     cout << "dipho->pt(): " << dipho->pt() << endl;
        //     cout << "diphoton->sumPt(): " << dipho->sumPt() << endl;
        //     // cout << "dipho->leadingPhoton()->pt(): " << dipho->leadingPhoton()->pt() << endl;
        //     // cout << "dipho->subleadingPhoton()->pt(): " << dipho->subLeadingPhoton()->pt() << endl;
        //   }
        // }

        if (diphotons->size() > 0){ // without systematics (?)
        for( unsigned int diphoIndex = 0; diphoIndex < 1; diphoIndex++ ) { // only look at highest sumpt dipho
          // cout << "****In diphoton loop***" << endl;
          // std::unique_ptr<vector<HHWWggTag> > tags( new vector<HHWWggTag> );
          // edm::Ptr<flashgg::DiPhotonCandidate> dipho = Corrdiphoton; 
          // edm::Ptr<flashgg::DiPhotonCandidate> dipho = diPhotons->ptrAt( diphoIndex ); // systematic loop 
          edm::Ptr<flashgg::DiPhotonCandidate> dipho = diphotons->ptrAt( diphoIndex ); // without systematic look (?)
          // cout << "****got***" << endl;
          // edm::Ptr<flashgg::DiPhotonCandidate> dipho = diphotons->ptrAt( diphoIndex ); 
          edm::Ptr<flashgg::DiPhotonMVAResult> mvares = mvaResults->ptrAt( diphoIndex );    
          // cout << "dipho energy = " << dipho->genP4().E() << endl; 
          // cout << "dipho pt = " << dipho->genP4().pt() << endl; 
          // cout << "dipho eta = " << dipho->genP4().eta() << endl; 
          // cout << "dipho phi = " << dipho->genP4().phi() << endl; 
          diphoton_vertex = dipho->vtx();

          // diphoton_vertex_index = dipho->vertexIndex();
          // cout << "diphoton vertex index = " << diphoton_vertex_index << endl;
          // cout << "diphoton vertex x = " << diphoton_vertex->x() << endl;
          // cout << "diphoton vertex y = " << diphoton_vertex->y() << endl;
          // cout << "diphoton vertex z = " << diphoton_vertex->z() << endl;

          // indexes->Fill(diphoton_vertex_index); // running 

          //-- MVA selections 
          const flashgg::Photon* leadPho = dipho->leadingPhoton();
          const flashgg::Photon* subleadPho = dipho->subLeadingPhoton();

          diphoton_vertex = dipho->vtx();   

          passMVAs = 0;
          passMVAs = checkPassMVAs(leadPho, subleadPho, diphoton_vertex);

          if(doHHWWggTagCutFlowAnalysis_){
            if(!passMVAs) Cut_Variables[1] = 0.0;
            else Cut_Variables[1] = 1.0; // passed photon MVAs (and all photon selections)
          }

          else{
            if(!passMVAs) continue; // Do not save event if leading and subleading photons don't pass MVA cuts 
          }
          if(!passMVAs && !doHHWWggTagCutFlowAnalysis_) cout << "[HHWWggTagProducer.cc] - *********************************************problem" << endl;
          // if(!passMVAs) continue; // Do not save event if leading and subleading photons don't pass MVA cuts 
          // remove for cutflow 

          // if(!passMVAs) Cut_Variables[1] = 0.0;
          // else Cut_Variables[1] = 1.0; // passed photon MVAs (and all photon selections)

          hasGoodElec = false;
          hasGoodMuons = false;

          // Check MET Filters
          // if(passMETfilters){
          //   pass_METfilters = 1;
          // }

          // if(diphotons->ptrAt(diphoIndex)->vertexIndex()==0){
          //   dipho_vertex_is_zero = 1;
          // }

          // Check if useVertex0only_
          // if(useVertex0only_) // If only 0th vertex of diphotons is used, continue on all other vertex indices 
          //     if(diphotons->ptrAt(diphoIndex)->vertexIndex()==0){
          //       dipho_vertex_is_zero = 1;
          //     }
                      

          // leading/subleading photon pt 
          // if( dipho->leadingPhoton()->pt() > ( dipho->mass() )*leadPhoOverMassThreshold_ ){ 
          //     pass_leadPhoOverMassThreshold = 1;
          //   }
          // if( dipho->subLeadingPhoton()->pt() > ( dipho->mass() )*subleadPhoOverMassThreshold_ ) { 
          //     pass_subleadPhoOverMassThreshold = 1;
          //   }

          // leading/subleading photon MVA
          // idmva1 = dipho->leadingPhoton()->phoIdMvaDWrtVtx( dipho->vtx() ); // can choose the Hgg MVA score with respect to a vertex 
          // idmva2 = dipho->subLeadingPhoton()->phoIdMvaDWrtVtx( dipho->vtx() ); // 
          // lead_pho_Hgg_MVA = idmva1;
          // sublead_pho_Hgg_MVA = idmva2;
          // if (idmva1 > PhoMVAThreshold_) pass_LeadPhoton_MVA = 1;
          // if (idmva2 > PhoMVAThreshold_) pass_SubLeadPhoton_MVA = 1;

          // if( idmva1 <= PhoMVAThreshold_ || idmva2 <= PhoMVAThreshold_ ) {
          //     pass_LeadPhoton_MVA = 1;
          //     // continue; 
          //    } // isn't this already applied in preselection? 

          // Diphoton MVA 
          // if ( mvares->result >= MVAThreshold_ ){
          //   pass_dipho_MVA = 1;
          // }
          // if( mvares->result < MVAThreshold_ ) { continue; }


          // dipho_MVA = mvares->result; 


          // cout << "diphomva = " << mvares->result << endl;
          // photonSelection = true;

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

          /*
          if (hasGoodElec && hasGoodMuons){
            for (unsigned int ei = 0; ei < goodElectrons.size(); ei++){
              Ptr<flashgg::Electron> electron = goodElectrons[ei];
              for (unsigned int mi = 0; mi < goodMuons.size(); mi++){
                Ptr<flashgg::Muon> muon = goodMuons[mi];
                dr_ll = deltaR(electron->eta(), electron->phi(), muon->eta(), muon->phi()); 
                if (dr_ll > 0.4){ 
                  // one_FL_dr = true;
                  num_FL_dr += 1.0;
                  // break;
                }
              }
            }
          }

          else if (hasGoodElec && !hasGoodMuons){
            for (unsigned int ei = 0; ei < goodElectrons.size() - 1; ei++){ // the last electron cannot be the first one in the dR calculation 
              Ptr<flashgg::Electron> electroni = goodElectrons[ei];
              for (unsigned int ej = ei + 1; ej < goodElectrons.size(); ej++){
                Ptr<flashgg::Electron> electronj = goodElectrons[ej];
                dr_ll = deltaR(electroni->eta(), electroni->phi(), electronj->eta(), electronj->phi()); 
                if (dr_ll > 0.4){ 
                  // one_FL_dr = true;
                  num_FL_dr += 1.0;
                  // break;
                }
              }
            }
          }
          

          else if (!hasGoodElec && hasGoodMuons){
            for (unsigned int mi = 0; mi < goodMuons.size() - 1; mi++){
              Ptr<flashgg::Muon> muoni = goodMuons[mi];
              for (unsigned int mj = mi + 1; mj < goodMuons.size(); mj++){
                Ptr<flashgg::Muon> muonj = goodMuons[mj];
                dr_ll = deltaR(muoni->eta(), muoni->phi(), muonj->eta(), muonj->phi()); 
                if (dr_ll > 0.4){ 
                  // one_FL_dr = true;
                  num_FL_dr += 1.0;
                  // break;
                }
              }
            }
          }
          */

          // Jets 
          // unsigned int jetCollectionIndex = diPhotons->at( diphoIndex ).jetCollectionIndex(); // diphoton collection for each systematic 
          unsigned int jetCollectionIndex = diphotons->at( diphoIndex ).jetCollectionIndex(); // not looping over systematics 
          // size_t vtx = (size_t)dipho->jetCollectionIndex();
          edm::Handle<edm::View<flashgg::Jet> > Jets_;

          // unsigned int jet_token_index = jet_col_idx*inputJetsCollSize_+vtx;
          // cout << "jet_token_index = " << jet_token_index << endl;
          // cout << "jetCollectionIndex = " << jetCollectionIndex << endl;

          event.getByToken( jetTokens_[jetCollectionIndex], Jets_); // testing 
          // event.getByToken( jetTokens_[jet_col_idx*inputJetsCollSize_+vtx], Jets_);  //take the corresponding vertex of current systematic //WORKS 
          // cout << "right after getbytoken jettokens jets" << endl;

          std::vector<edm::Ptr<Jet> > tagJets;

          // Jet Selections
          for( unsigned int candIndex_outer = 0; candIndex_outer <  Jets_->size() ; candIndex_outer++ ) 
              {
                  bool keepJet=true;
                  edm::Ptr<flashgg::Jet> thejet = Jets_->ptrAt( candIndex_outer );

                  // if(!thejet->passesJetID  ( flashgg::Tight2017 ) ) { continue; } // remove for cut flow 

                  // if(thejet->passesJetID  ( flashgg::Tight2017 ) ) {
                  //     // number_passed_jetid += 1;
                  //   }

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
          for (unsigned int j = 0; j < tagJets.size(); j++){
            Ptr<flashgg::Jet> jet_ = tagJets[j];
            btagVal = jet_->bDiscriminator("mini_pfDeepFlavourJetTags:probb");
            if (btagVal > btagThresh_) hasHighbTag = 1;
          }

          // If doing cut flow analysis, don't skip event  
          if(doHHWWggTagCutFlowAnalysis_){
            if(hasHighbTag) 
              Cut_Variables[2] = 0.0; // does not pass bveto 
            else 
              Cut_Variables[2] = 1.0; // passes bveto 
          }
          else if(hasHighbTag) continue; // Skip event if it has at least one jet with a btag above threshold, and you're not doing a cut flow analysis 

          n_good_jets = tagJets.size();

          // MET 
          if( METs->size() != 1 ) { std::cout << "WARNING - #MET is not 1" << std::endl;}
          Ptr<flashgg::Met> theMET = METs->ptrAt( 0 );

          // how to go from pointer 'jet' to object '*thisJetPointer'
          //flashgg::Jet * thisJetPointer = const_cast<flashgg::Jet *>(jet.get());
          //JetVector.push_back(*thisJetPointer);

          for (unsigned int i = 0; i < tagJets.size(); i++){
            auto tag_jet = tagJets[i]; 
            flashgg::Jet * thisJetPointer = const_cast<flashgg::Jet *>(tag_jet.get());
            tagJets_.push_back(*thisJetPointer);
          }

          if (doHHWWggTagCutFlowAnalysis_){
            if (n_good_leptons == 1) Cut_Variables[3] = 1.0; // exactly one good lepton 
            else Cut_Variables[3] = 0.0;
            if (n_good_jets >= 2) Cut_Variables[4] = 1.0; // at least 2 good jets 
            else Cut_Variables[4] = 0.0; 
          }
          else
            if ((n_good_leptons != 1) || (n_good_jets <= 1)) continue;



          //-- Tag object 
          if ( (n_good_leptons == 1) && (n_good_jets >= 2)){
            
            int catnum = 0;
            Ptr<flashgg::Jet> jet1 = tagJets[0];
            Ptr<flashgg::Met> theMET = METs->ptrAt( 0 );

            if (n_good_electrons == 1){
              Ptr<flashgg::Electron> tag_electron = goodElectrons[0];
              catnum = 0;

              // if (n_good_jets == 2){
                Ptr<flashgg::Jet> jet2 = tagJets[1];
                HHWWggTag tag_obj; 
                // HHWWggTag tag_obj_0;
                if (doHHWWggTagCutFlowAnalysis_){
                  // Save electrons, muons, goodElectrons, goodMuons, jets, goodjets 
                  HHWWggTag tag_obj_(dipho, tag_electron, theMET, jet1, jet2, Cut_Variables); // electron, MET, jet1, jet2 
                  tag_obj = tag_obj_;
                } 
                else{
                  HHWWggTag tag_obj_(dipho, tag_electron, theMET, jet1, jet2); // diphoton, electron, MET, jet1, jet2 
                  tag_obj = tag_obj_;
                }
                // HHWWggTag tag_obj(dipho, tag_electron, theMET, jet1, jet2, tagJets_, Cut_Variables); // electron, MET, jet1, jet2 
                // HHWWggTag tag_obj(dipho, tag_electron, theMET, jet1, jet2); // electron, MET, jet1, jet2 
                // if (loopOverJets == 1) tag_obj.setSystLabel( inputDiPhotonSuffixes_[diphoton_idx] );
                // else tag_obj.setSystLabel( inputJetsSuffixes_[jet_col_idx]);
                tag_obj.setSystLabel( systLabel_);
                tag_obj.setDiPhotonIndex( diphoIndex );           
                // tag_obj.setMVA( -0.9 );
                tag_obj.setCategoryNumber( catnum );
                tag_obj.includeWeights( *dipho );
                // tag_obj.setEventNumber(event.id().event() );
                // cout << "Pushing back tag object w/ electron" << endl;
                HHWWggtags->push_back( tag_obj );

                // Also push back to zeroeth category containing all tags 
                // tag_obj_0 = tag_obj;
                // tag_obj_0.setCategoryNumber(0);
                // HHWWggtags->push_back( tag_obj_0 );
                
                if( ! event.isRealData() ) {
                  HHWWggtags->back().setTagTruth( edm::refToPtr( edm::Ref<vector<TagTruthBase> >( rTagTruth, 0 ) ) );                 
                }  

            }
            
            if (n_good_muons == 1){
              Ptr<flashgg::Muon> tag_muon = goodMuons[0];
              catnum = 1;
              // catnum = 0;
                Ptr<flashgg::Jet> jet2 = tagJets[1];
                HHWWggTag tag_obj; 
                // HHWWggTag tag_obj_0;
                if (doHHWWggTagCutFlowAnalysis_){
                  // HHWWggTag tag_obj_(dipho, tag_muon, theMET, jet1, jet2, tagJets_, Cut_Variables); // muon, MET, jet1, jet2 
                  HHWWggTag tag_obj_(dipho, tag_muon, theMET, jet1, jet2, Cut_Variables); // muon, MET, jet1, jet2 
                  tag_obj = tag_obj_;
                } 
                else{
                  HHWWggTag tag_obj_(dipho, tag_muon, theMET, jet1, jet2); // diphoton, muon, MET, jet1, jet2 
                  tag_obj = tag_obj_;
                }


                // HHWWggTag tag_obj(dipho, tag_muon, theMET, jet1, jet2, tagJets_, Cut_Variables); // electron, MET, jet1, jet2 
                // if (loopOverJets == 1) tag_obj.setSystLabel( inputDiPhotonSuffixes_[diphoton_idx] );
                // else tag_obj.setSystLabel( inputJetsSuffixes_[jet_col_idx]);

                tag_obj.setSystLabel(systLabel_);

                tag_obj.setDiPhotonIndex( diphoIndex );           
                // tag_obj.setMVA( -0.9 );
                tag_obj.setCategoryNumber( catnum ); // 1 for muon events 
                tag_obj.includeWeights( *dipho );

                // tag_obj.setEventNumber(event.id().event() );
                // cout << "Pushing back tag object w/ muon" << endl;
                HHWWggtags->push_back( tag_obj ); 

                // Also push back to zeroeth category containing all tags 
                // tag_obj_0 = tag_obj;
                // tag_obj_0.setCategoryNumber(0);
                // HHWWggtags->push_back( tag_obj_0 );                

                if( ! event.isRealData() ) {
                  HHWWggtags->back().setTagTruth( edm::refToPtr( edm::Ref<vector<TagTruthBase> >( rTagTruth, 0 ) ) );                 
                }  
                    
            }
            
          } // if ( (n_good_leptons == 1) && (n_good_jets >= 2) )

          // Untagged category
          // Don't have semileptonic selections. Only results in further analysis if doHHWWggTagCutFlowAnalysis_ == True
          else {
            if(doHHWWggTagCutFlowAnalysis_){
              // cout << "*******************************tagging untagged *********************" << endl;
              // HHWWggTag tag_obj(dipho, tagJets_, Cut_Variables);
              // HHWWggTag tag_obj(dipho, tagJets_, theMET, Cut_Variables);
              // HHWWggTag tag_obj(dipho, tagJets_, theMET, Cut_Variables);
              HHWWggTag tag_obj(dipho, theMET, Cut_Variables);
                  // if (loopOverJets == 1) tag_obj.setSystLabel( inputDiPhotonSuffixes_[diphoton_idx] );
                  // else tag_obj.setSystLabel( inputJetsSuffixes_[jet_col_idx]);
                  tag_obj.setSystLabel(systLabel_);

                  tag_obj.setDiPhotonIndex( diphoIndex );           
                  // tag_obj.setMVA( -0.9 );
                  tag_obj.setCategoryNumber( 2 ); // 2: Untagged category. Does not meet any selection criteria but want to save event 
                  tag_obj.includeWeights( *dipho );
                  HHWWggtags->push_back( tag_obj ); 

                  if( ! event.isRealData() ) {
                    HHWWggtags->back().setTagTruth( edm::refToPtr( edm::Ref<vector<TagTruthBase> >( rTagTruth, 0 ) ) );                 
                  } 
            }
          }

        } // only look at highest pt dipho

      } // if at least 1 PS diphoton 

      event.put( std::move( HHWWggtags ) );
      event.put( std::move( truths ) );

    } // HHWWggTagProducer::produce

  } // namespace flashgg

  typedef flashgg::HHWWggTagProducer FlashggHHWWggTagProducer;
  DEFINE_FWK_MODULE( FlashggHHWWggTagProducer );