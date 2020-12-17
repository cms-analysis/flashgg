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
    HHWWggTagProducer( const ParameterSet & );

    //---Outtree
    edm::Service<TFileService> fs;

  private:
    bool checkPassMVAs(const flashgg::Photon*& leading_photon, const flashgg::Photon*& subleading_photon, edm::Ptr<reco::Vertex>& diphoton_vertex, double EB_Photon_MVA_Threshold, double EE_Photon_MVA_Threshold);
    std::vector<double> GetMuonVars(const std::vector<edm::Ptr<flashgg::Muon> > &muonPointers, const std::vector<edm::Ptr<reco::Vertex> > &vertexPointers);
    std::vector<double> GetJetVars(const std::vector<edm::Ptr<flashgg::Jet> > &jetPointers, const edm::Ptr<flashgg::DiPhotonCandidate> dipho);
    vector<Ptr<flashgg::Jet>> GetFHPtOrderedJets(bool doHHWWggDebug_, std::vector<edm::Ptr<Jet> > tagJets_);
    vector<Ptr<flashgg::Jet>> GetFHminWHJets(bool doHHWWggDebug_, std::vector<edm::Ptr<Jet> > tagJets_);
    vector<Ptr<flashgg::Jet>> GetFHminWHLead2Jet(bool doHHWWggDebug_, std::vector<edm::Ptr<Jet> > tagJets_);
    vector<Ptr<flashgg::Jet>> GetFHminHiggsMassOnly(bool doHHWWggDebug_, std::vector<edm::Ptr<Jet> > tagJets_);
    int GetNumFLDR(std::vector<edm::Ptr<flashgg::Electron> >, std::vector<edm::Ptr<flashgg::Muon> >, double); 
    float getGenCosThetaStar_CS(TLorentzVector h1, TLorentzVector h2);
    template <class flashggPtr> 
    void PrintScaleFactorsPtr(flashggPtr); 
    template <class flashggObj> 
    void PrintScaleFactorsObj(flashggObj);     
    HHWWggTag SetCentralUpDownWeights(HHWWggTag, std::vector<edm::Ptr<flashgg::Electron> >, std::vector<edm::Ptr<flashgg::Muon> >, std::vector<edm::Ptr<flashgg::Jet> >, edm::Ptr<flashgg::DiPhotonCandidate>, bool);

    void produce( Event &, const EventSetup & ) override;
    std::vector<edm::EDGetTokenT<edm::View<DiPhotonCandidate> > > diPhotonTokens_;
    std::string inputDiPhotonName_;

    std::vector<std::string> inputJetsSuffixes_;

    std::vector<edm::EDGetTokenT<edm::View<flashgg::Jet> > > jetTokens_;

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

    string JetIDLevel_; 

    //---ID selector
    ConsumesCollector cc_;

    //----output collection
    double EB_Photon_MVA_Threshold_;
    double EE_Photon_MVA_Threshold_;
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
    double deltaRJetMuonThreshold_;
    double deltaRPhoLeadJet_;
    double deltaRPhoSubLeadJet_;

    double DeltaRTrkElec_;
    double TransverseImpactParam_;
    double LongitudinalImpactParam_;

    double deltaRPhoElectronThreshold_;
    double deltaMassElectronZThreshold_;
    double DiLepMassHigThre_;
    double DiLepMassLowThre_;
    double FL_Dipho_pT_Thre_;
    double FH_Dipho_pT_Thre_;
    bool hasGoodElec = false;
    bool hasGoodMuons = false;

    vector<double> nonTrigMVAThresholds_;
    vector<double> nonTrigMVAEtaCuts_;

    double electronIsoThreshold_;
    vector<double> electronEtaThresholds_;
    bool useElectronMVARecipe_;
    bool useElectronLooseID_;
    double btagThresh_;
    vector<string> BTagTypes_;
    bool doHHWWggTagCutFlowAnalysis_;
    bool doHHWWggNonResAnalysis_;
    bool doHHWWggFHptOrdered_;
    bool doHHWWggFHminWHJets_;
    bool doHHWWggFHminWHLead2Jet_;
    bool doHHWWggFHminHiggsMassOnly_;
    bool doHHWWggDebug_;
    string HHWWggAnalysisChannel_;

    //full-lep
    bool SaveOthers_;
    bool Savejet;
    double lep1ptThre_;
    double lep2ptThre_;
    double lep3ptThre_;
    double DiLepPtThre_;
    double MassTThre_;
    double MassT_l2Thre_;
    double dipho_MVA;
    edm::InputTag genInfo_;
    edm::EDGetTokenT<GenEventInfoProduct> genInfoToken_;
    // TH1F* nEvents;

    bool HHWWgguseZeroVtx_; 
  };

    //---standard
    HHWWggTagProducer::HHWWggTagProducer( const ParameterSet & pSet):
    diphotonToken_( consumes<View<flashgg::DiPhotonCandidate> >( pSet.getParameter<InputTag> ( "DiPhotonTag" ) ) ),
    vertexToken_( consumes<View<reco::Vertex> >( pSet.getParameter<InputTag> ( "VertexTag" ) ) ),
    genParticleToken_( consumes<View<reco::GenParticle> >( pSet.getParameter<InputTag> ( "GenParticleTag" ) ) ),
    electronToken_( consumes<View<flashgg::Electron> >( pSet.getParameter<InputTag> ( "ElectronTag" ) ) ),
    muonToken_( consumes<View<flashgg::Muon> >( pSet.getParameter<InputTag> ( "MuonTag" ) ) ),
    METToken_( consumes<View<Met> >( pSet.getParameter<InputTag> ( "METTag" ) ) ),
    mvaResultToken_( consumes<View<flashgg::DiPhotonMVAResult> >( pSet.getParameter<InputTag> ( "MVAResultTag" ) ) ),
    rhoTag_( consumes<double>( pSet.getParameter<InputTag>( "rhoTag" ) ) ),
    triggerRECO_( consumes<edm::TriggerResults>(pSet.getParameter<InputTag>("RECOfilters") ) ),
    triggerPAT_( consumes<edm::TriggerResults>(pSet.getParameter<InputTag>("PATfilters") ) ),
    triggerFLASHggMicroAOD_( consumes<edm::TriggerResults>( pSet.getParameter<InputTag>("FLASHfilters") ) ),
    systLabel_( pSet.getParameter<string> ( "SystLabel" ) ),
    JetIDLevel_( pSet.getParameter<string> ( "JetIDLevel" ) ),    
    cc_( consumesCollector() )

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

      inputJetsSuffixes_= pSet.getParameter<std::vector<std::string> > ( "JetsSuffixes" );

      auto jetTags = pSet.getParameter<std::vector<edm::InputTag> > ( "JetTags" );
      for( auto & tag : jetTags ) { jetTokens_.push_back( consumes<edm::View<flashgg::Jet> >( tag ) ); }

      genInfo_ = pSet.getUntrackedParameter<edm::InputTag>( "genInfo", edm::InputTag("generator") );
      genInfoToken_ = consumes<GenEventInfoProduct>( genInfo_ );

      // nEvents = fs->make<TH1F> ("nEvents", "nEvents", 2,0,2);

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
      electronEtaThresholds_ = pSet.getParameter<vector<double > >( "electronEtaThresholds");
      useElectronMVARecipe_=pSet.getParameter<bool>("useElectronMVARecipe");
      useElectronLooseID_=pSet.getParameter<bool>("useElectronLooseID");
      btagThresh_ = pSet.getParameter<double>( "btagThresh");
      BTagTypes_ = pSet.getParameter<vector<string>>( "BTagTypes" );
      doHHWWggTagCutFlowAnalysis_ = pSet.getParameter<bool>( "doHHWWggTagCutFlowAnalysis");
      doHHWWggNonResAnalysis_ = pSet.getParameter<bool>( "doHHWWggNonResAnalysis" );
      doHHWWggFHptOrdered_ = pSet.getParameter<bool>( "doHHWWggFHptOrdered" );
      doHHWWggFHminWHJets_ = pSet.getParameter<bool>( "doHHWWggFHminWHJets" );
      doHHWWggFHminWHLead2Jet_ = pSet.getParameter<bool>( "doHHWWggFHminWHLead2Jet" );
      doHHWWggFHminHiggsMassOnly_ = pSet.getParameter<bool>( "doHHWWggFHminHiggsMassOnly" );
      doHHWWggDebug_ = pSet.getParameter<bool>( "doHHWWggDebug" );
      HHWWggAnalysisChannel_ = pSet.getParameter<string>( "HHWWggAnalysisChannel" );
      lep1ptThre_ = pSet.getParameter<double>("lep1ptThre");//means > lep1pt
      lep2ptThre_ = pSet.getParameter<double>("lep2ptThre");//means > lep2pt
      lep3ptThre_ = pSet.getParameter<double>("lep3ptThre");//means < lep3pt
      DiLepPtThre_ = pSet.getParameter<double>("DiLepPtThre");
      DiLepMassHigThre_ = pSet.getParameter<double>("DiLepMassHigThre");
      DiLepMassLowThre_ = pSet.getParameter<double>("DiLepMassLowThre");
      FL_Dipho_pT_Thre_ = pSet.getParameter<double>("FL_Dipho_pT_Thre");
      FH_Dipho_pT_Thre_ = pSet.getParameter<double>("FH_Dipho_pT_Thre");
      MassTThre_ = pSet.getParameter<double>("MassTThre");
      MassT_l2Thre_ = pSet.getParameter<double>("MassT_l2Thre");
      HHWWgguseZeroVtx_ = pSet.getParameter<bool>("HHWWgguseZeroVtx");
      produces<vector<HHWWggTag>>();
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
    bool HHWWggTagProducer::checkPassMVAs( const flashgg::Photon*& leading_photon, const flashgg::Photon*& subleading_photon, edm::Ptr<reco::Vertex>& diphoton_vertex, double EB_Photon_MVA_Threshold, double EE_Photon_MVA_Threshold){

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
        if (lp_Hgg_MVA > EB_Photon_MVA_Threshold) lead_pass_TightPhoID = 1;
      }

      // EE
      else if (( abs(leading_pho_eta) > 1.566) && ( abs(leading_pho_eta) < 2.5)){
        if (lp_Hgg_MVA > EE_Photon_MVA_Threshold) lead_pass_TightPhoID = 1;
      }

      // SubLeading Photon
      // EB
      if (( abs(sub_leading_pho_eta) > 0) && ( abs(sub_leading_pho_eta) < 1.4442)){
        if (slp_Hgg_MVA > EB_Photon_MVA_Threshold) sublead_pass_TightPhoID = 1;
      }

      // EE
      else if (( abs(sub_leading_pho_eta) > 1.566) && ( abs(sub_leading_pho_eta) < 2.5)){
        if (slp_Hgg_MVA > EE_Photon_MVA_Threshold) sublead_pass_TightPhoID = 1;
      }

      if (lead_pass_TightPhoID && sublead_pass_TightPhoID){
        return 1;
    }

    else{
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
      unsigned int maxJets = 5; // Max number of jets to store information for 
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

      // Select 2 jets whose mass closest to W-boson mass
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
      if (CountJet1 == OnShellW_LeadingJetIndex || CountJet1 == OnShellW_SubLeadingJetIndex) continue;
      for (int CountJet2 = CountJet1+1; CountJet2 < nTagJets; CountJet2++) {
        if (CountJet2 == OnShellW_LeadingJetIndex || CountJet2 == OnShellW_SubLeadingJetIndex) continue;
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

    vector<Ptr<flashgg::Jet> > HHWWggTagProducer::GetFHminWHLead2Jet(bool doHHWWggDebug, std::vector<edm::Ptr<Jet> > tagJets_)
    {
      // get 4 jets for FH final state with minWH vals
      std::vector<edm::Ptr<flashgg::Jet> > FHJets_;
      bool DEBUG = doHHWWggDebug;
      double TempMinWMass = 999999.0;

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
      if (DEBUG) std::cout << "Size of jets: " << nTagJets << std::endl;

      // Select 2 jets whose mass closest to W-boson mass
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

      // 2 Off-shell w-boson jets; out of remaning jets select leading 2 jets.
      // Here, I am selecting first two jets after excluding already selected jets.
      int TempJetCounter = 0;
      for (int CountJet1 = 0; CountJet1 < nTagJets; CountJet1++) {
        if (CountJet1 == OnShellW_LeadingJetIndex || CountJet1 == OnShellW_SubLeadingJetIndex) continue;

        jet11 = tagJets_[CountJet1];
        if (DEBUG) std::cout << "(CountJet1) = (" << CountJet1 << "), pT(jet): " << jet11->p4().pt() << ",\tTempJetCounter: " << TempJetCounter << std::endl;

        if (TempJetCounter==0) OffShellW_LeadingJetIndex = CountJet1;
        if (TempJetCounter==1) OffShellW_SubLeadingJetIndex = CountJet1;

        TempJetCounter +=1;
        if (TempJetCounter>1) break;
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

    vector<Ptr<flashgg::Jet> > HHWWggTagProducer::GetFHminHiggsMassOnly(bool doHHWWggDebug, std::vector<edm::Ptr<Jet> > tagJets_)
    {
      // get 4 jets for FH final state with minWH vals
      std::vector<edm::Ptr<flashgg::Jet> > FHJets_;
      bool DEBUG = doHHWWggDebug;
      double TempMinWMass = 999999.0;

      int OnShellW_LeadingJetIndex = -1;
      int OnShellW_SubLeadingJetIndex = -1;
      int OffShellW_LeadingJetIndex = -1;
      int OffShellW_SubLeadingJetIndex = -1;

      int TempJet[4] = {-1,-1,-1,-1};

      Ptr<flashgg::Jet> jet11;
      Ptr<flashgg::Jet> jet12;
      Ptr<flashgg::Jet> jet13;
      Ptr<flashgg::Jet> jet14;
      Ptr<flashgg::Jet> jet1;
      Ptr<flashgg::Jet> jet2;
      Ptr<flashgg::Jet> jet3;
      Ptr<flashgg::Jet> jet4;

      int nTagJets = tagJets_.size();

      // Select 2 jets whose mass closest to W-boson mass
      for (int CountJet1 = 0; CountJet1 < nTagJets-3; CountJet1++) {
        for (int CountJet2 = CountJet1+1; CountJet2 < nTagJets-2; CountJet2++) {
          for (int CountJet3 = CountJet2+1; CountJet3 < nTagJets-1; CountJet3++) {
            for (int CountJet4 = CountJet3+1; CountJet4 < nTagJets; CountJet4++) {
              if (DEBUG) std::cout << "(CountJet1,CountJet2,CountJet3,CountJet4) = (" << CountJet1 << "," << CountJet2 <<"," << CountJet3 <<"," << CountJet4 << ")" << std::endl;
              jet11 = tagJets_[CountJet1];
              jet12 = tagJets_[CountJet2];
              jet13 = tagJets_[CountJet3];
              jet14 = tagJets_[CountJet4];

              double deltaMass =  abs((jet11->p4() + jet12->p4() + jet13->p4() + jet14->p4()).M() - 125.1);
              if (DEBUG) std::cout << "deltaMass = " << deltaMass << "\t TempMinWMass = " << TempMinWMass << std::endl;
              if ( deltaMass < TempMinWMass)
              {
                  TempJet[0] = CountJet1;
                  TempJet[1] = CountJet2;
                  TempJet[2] = CountJet3;
                  TempJet[3] = CountJet4;
                  TempMinWMass = deltaMass;
                  if (DEBUG) std::cout << "==> (CountJet1,CountJet2) = (" << CountJet1 << "," << CountJet2 << ")" << std::endl;
              }
            }
          }
        }
      }
      if (DEBUG) std::cout << "[INFO] Print Jet Index (After W-Selection): " << TempJet[0] << "\t" << TempJet[1] << "\t" << TempJet[2] << "\t" << TempJet[3]  << std::endl;

      // 2 Off-shell w-boson jets; out of remaning jets select leading 2 jets.
      TempMinWMass = 9999.0;
      for (int i = 0; i < 3; ++i)
      {
        for (int j = i+1; i < 4; ++i)
        {
          jet11 = tagJets_[TempJet[i]];
          jet12 = tagJets_[TempJet[j]];
          double deltaMass = abs( (jet11->p4() + jet12->p4()).M() - 80.0);
          if (deltaMass < TempMinWMass)
          {
            OnShellW_LeadingJetIndex = TempJet[i];
            OnShellW_SubLeadingJetIndex = TempJet[j];
            TempMinWMass = deltaMass;
          }
        }
      }

      int TempJetCounter = 0;
      for (int i = 0; i < 4; ++i)
      {
        if (i == OnShellW_LeadingJetIndex || i == OnShellW_SubLeadingJetIndex) continue;
        TempJetCounter +=1;
        if (TempJetCounter == 1) OffShellW_LeadingJetIndex = i;
        if (TempJetCounter == 2) OffShellW_SubLeadingJetIndex = i;
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


    vector<Ptr<flashgg::Jet> > HHWWggTagProducer::GetFHPtOrderedJets(bool doHHWWggDebug, std::vector<edm::Ptr<Jet> > tagJets_)
    {
      // get 4 jets for FH final state with minWH vals
      std::vector<edm::Ptr<flashgg::Jet> > FHJets_;
      bool DEBUG = doHHWWggDebug;
      double TempMinWMass = 999999.0;

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

      TempMinWMass = 9999.0;
      for (int i = 0; i < 3; ++i)
      {
        for (int j = i+1; i < 4; ++i)
        {
          jet11 = tagJets_[i];
          jet12 = tagJets_[j];
          double deltaMass = abs( (jet11->p4() + jet12->p4()).M() - 80.0);
          if (deltaMass < TempMinWMass)
          {
            OnShellW_LeadingJetIndex = i;
            OnShellW_SubLeadingJetIndex = j;
            TempMinWMass = deltaMass;
          }
        }
      }
      if (DEBUG) std::cout << "[INFO] Print pt of 2 selected jets: " << OnShellW_LeadingJetIndex << "\t" << OnShellW_SubLeadingJetIndex << "\t" << OffShellW_LeadingJetIndex << "\t" << OffShellW_SubLeadingJetIndex  << std::endl;

      int TempJetCounter = 0;
      for (int i = 0; i < 4; ++i)
      {
        if (i == OnShellW_LeadingJetIndex || i == OnShellW_SubLeadingJetIndex) continue;
        TempJetCounter +=1;
        if (TempJetCounter == 1) OffShellW_LeadingJetIndex = i;
        if (TempJetCounter == 2) OffShellW_SubLeadingJetIndex = i;
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

    // Compute number of pairs of leptons with a minimum delta R 
    int HHWWggTagProducer::GetNumFLDR(std::vector<edm::Ptr<flashgg::Electron> > goodElectrons_, std::vector<edm::Ptr<flashgg::Muon> > goodMuons_, double deltaRLeps)
    { 
      int num_FL_dr_ = 0; 
      double dr_ll = 0;
      bool hasGoodElec = goodElectrons_.size() > 0; 
      bool hasGoodMuons = goodMuons_.size() > 0;
      if (hasGoodElec && hasGoodMuons){
        for (unsigned int ei = 0; ei < goodElectrons_.size(); ei++){
          Ptr<flashgg::Electron> electron = goodElectrons_[ei];
          for (unsigned int mi = 0; mi < goodMuons_.size(); mi++){
            Ptr<flashgg::Muon> muon = goodMuons_[mi];
            dr_ll = deltaR(electron->eta(), electron->phi(), muon->eta(), muon->phi());
            if (dr_ll > deltaRLeps){
              num_FL_dr_ += 1;
            }
          }
        }
      }

      else if(hasGoodElec && !hasGoodMuons){
        for (unsigned int ei = 0; ei < goodElectrons_.size() - 1; ei++){ // the last electron cannot be the first one in the dR calculation
          Ptr<flashgg::Electron> electroni = goodElectrons_[ei];
          for (unsigned int ej = ei + 1; ej < goodElectrons_.size(); ej++){
            Ptr<flashgg::Electron> electronj = goodElectrons_[ej];
            dr_ll = deltaR(electroni->eta(), electroni->phi(), electronj->eta(), electronj->phi());
            if (dr_ll > deltaRLeps){
              num_FL_dr_ += 1;
            }
          }
        }
      }

      else if(!hasGoodElec && hasGoodMuons){
        for (unsigned int mi = 0; mi < goodMuons_.size() - 1; mi++){
          Ptr<flashgg::Muon> muoni = goodMuons_[mi];
          for (unsigned int mj = mi + 1; mj < goodMuons_.size(); mj++){
            Ptr<flashgg::Muon> muonj = goodMuons_[mj];
            dr_ll = deltaR(muoni->eta(), muoni->phi(), muonj->eta(), muonj->phi());
            if (dr_ll > deltaRLeps){
              num_FL_dr_ += 1;
            }
          }
        }
      }      
      return num_FL_dr_; 
    }

    float HHWWggTagProducer::getGenCosThetaStar_CS(TLorentzVector h1, TLorentzVector h2)
    {
    // cos theta star angle in the Collins Soper frame
        TLorentzVector hh = h1 + h2;
        h1.Boost(-hh.BoostVector());                     
        return h1.CosTheta();
    }

    template<typename flashggPtr> void HHWWggTagProducer::PrintScaleFactorsPtr(flashggPtr PtrToObject)
    {
      for (auto it = PtrToObject->weightListBegin() ; it != PtrToObject->weightListEnd(); it++) {
          string key = *it; 
          // if(key.find("Central")!=string::npos) std::cout << " Scale Factor: " << *it << " " << PtrToObject->weight(*it) << std::endl; // Central Weight only 
          std::cout << " Scale Factor: " << *it << " " << PtrToObject->weight(*it) << std::endl;
        }   
    }

    // If input weighted objected not a pointer, need to access weightList* methods with "." rather than "->" 
    template<typename flashggObj> void HHWWggTagProducer::PrintScaleFactorsObj(flashggObj Obj)
    {
      for (auto it = Obj.weightListBegin() ; it != Obj.weightListEnd(); it++) {
          string key = *it; 
          // if(key.find("Central")!=string::npos) std::cout << " Scale Factor: " << *it << " " << Obj.weight(*it) << std::endl; // Central Weight only 
          std::cout << " Scale Factor: " << *it << " " << Obj.weight(*it) << std::endl;
        }   
    }    

    // Set Central object weights in output trees for each scale factor 
    HHWWggTag HHWWggTagProducer::SetCentralUpDownWeights(HHWWggTag tag_obj_, std::vector<edm::Ptr<flashgg::Electron> > goodElectrons, std::vector<edm::Ptr<flashgg::Muon> > goodMuons, 
                                                         std::vector<edm::Ptr<flashgg::Jet> > tagJets, edm::Ptr<flashgg::DiPhotonCandidate> dipho, bool doHHWWggDebug_)
    {

      //-- Apply scale factors from all weighted objects
      // Diphoton 
      tag_obj_.includeWeights( *dipho );

      // Electrons  
      for (unsigned int elec_i = 0; elec_i < goodElectrons.size(); elec_i++){
        tag_obj_.includeWeights(*goodElectrons.at(elec_i));               
      }

      // Muons 
      for (unsigned int muon_i = 0; muon_i < goodMuons.size(); muon_i++){
        tag_obj_.includeWeights(*goodMuons.at(muon_i));
      }           

      // Jets 
      for (unsigned int TagJet_i = 0; TagJet_i < tagJets.size(); TagJet_i++){
        tag_obj_.includeWeightsByLabel(*tagJets.at(TagJet_i),"JetBTagReshapeWeightCentral");
        tag_obj_.includeWeightsByLabel(*tagJets.at(TagJet_i),"UnmatchedPUWeightCentral");
        // tag_obj.includeWeights(*tagJets.at(TagJet_i));
      }

      //-- Save central, up/down scale factors in output trees for checks / flexibility 
      //-- Initialize Values with 1 incase there are no good electrons, muons or jets since in end values are set to tag object SF values  
      double TagObjCentralWeight = 1; // Central weight of HH->WWgg tag object 

      // Diphoton
      double LooseMvaSF = 1, PreselSF = 1, TriggerWeight = 1, electronVetoSF = 1, prefireWeight = 1, diPhoCentralWeight = 1; // Diphoton 
      double LooseMvaSF_up = 1, PreselSF_up = 1, TriggerWeight_up = 1, electronVetoSF_up = 1, prefireWeight_up = 1; 
      double LooseMvaSF_down = 1, PreselSF_down = 1, TriggerWeight_down = 1, electronVetoSF_down = 1, prefireWeight_down = 1;

      // Electrons 
      double ElectronIDWeight = 1, ElectronRecoWeight = 1;  
      double ElectronIDWeight_up = 1, ElectronRecoWeight_up = 1; 
      double ElectronIDWeight_down = 1, ElectronRecoWeight_down = 1; 

      // Muons 
      double MuonIDWeight = 1, MuonRelISOWeight = 1;  
      double MuonIDWeight_up = 1, MuonRelISOWeight_up = 1;  
      double MuonIDWeight_down = 1, MuonRelISOWeight_down = 1;  

      // Jets
      double JetBTagReshapeWeight = 1, UnmatchedPUWeight = 1;  
      double JetBTagReshapeWeight_up = 1, UnmatchedPUWeight_up = 1;  
      double JetBTagReshapeWeight_down = 1, UnmatchedPUWeight_down = 1;  

      // Electrons 
      for (unsigned int elec_i = 0; elec_i < goodElectrons.size(); elec_i++){
          ElectronIDWeight = ElectronIDWeight * goodElectrons.at(elec_i)->weight("ElectronIDWeightCentral");
          ElectronRecoWeight = ElectronRecoWeight * goodElectrons.at(elec_i)->weight("ElectronRecoWeightCentral"); 

          ElectronIDWeight_up = ElectronIDWeight_up * goodElectrons.at(elec_i)->weight("ElectronIDWeightUp01sigma");
          ElectronRecoWeight_up = ElectronRecoWeight_up * goodElectrons.at(elec_i)->weight("ElectronRecoWeightUp01sigma");  

          ElectronIDWeight_down = ElectronIDWeight_down * goodElectrons.at(elec_i)->weight("ElectronIDWeightDown01sigma");
          ElectronRecoWeight_down = ElectronRecoWeight_down * goodElectrons.at(elec_i)->weight("ElectronRecoWeightDown01sigma");                                      
        }
      tag_obj_.setWeight("ElectronIDWeightCentral",ElectronIDWeight);
      tag_obj_.setWeight("ElectronRecoWeightCentral",ElectronRecoWeight);

      tag_obj_.setWeight("ElectronIDWeightUp01sigma",ElectronIDWeight_up);
      tag_obj_.setWeight("ElectronRecoWeightUp01sigma",ElectronRecoWeight_up);

      tag_obj_.setWeight("ElectronIDWeightDown01sigma",ElectronIDWeight_down);
      tag_obj_.setWeight("ElectronRecoWeightDown01sigma",ElectronRecoWeight_down);            

      // Muons 
      for (unsigned int muon_i = 0; muon_i < goodMuons.size(); muon_i++){
          MuonIDWeight = MuonIDWeight * goodMuons.at(muon_i)->weight("MuonTightIDWeightCentral");
          MuonRelISOWeight = MuonRelISOWeight * goodMuons.at(muon_i)->weight("MuonTightRelISOWeightCentral");    

          MuonIDWeight_up = MuonIDWeight_up * goodMuons.at(muon_i)->weight("MuonTightIDWeightUp01sigma");
          MuonRelISOWeight_up = MuonRelISOWeight_up * goodMuons.at(muon_i)->weight("MuonTightRelISOWeightUp01sigma");   

          MuonIDWeight_down = MuonIDWeight_down * goodMuons.at(muon_i)->weight("MuonTightIDWeightDown01sigma");
          MuonRelISOWeight_down = MuonRelISOWeight_down * goodMuons.at(muon_i)->weight("MuonTightRelISOWeightDown01sigma");                                    
        }
      tag_obj_.setWeight("MuonTightIDWeightCentral",MuonIDWeight);
      tag_obj_.setWeight("MuonTightRelISOWeightCentral",MuonRelISOWeight); 

      tag_obj_.setWeight("MuonTightIDWeightUp01sigma",MuonIDWeight_up);
      tag_obj_.setWeight("MuonTightRelISOWeightUp01sigma",MuonRelISOWeight_up); 

      tag_obj_.setWeight("MuonTightIDWeightDown01sigma",MuonIDWeight_down);
      tag_obj_.setWeight("MuonTightRelISOWeightDown01sigma",MuonRelISOWeight_down);             

      // Jets
      for (unsigned int TagJet_i = 0; TagJet_i < tagJets.size(); TagJet_i++){
          // tag_obj.includeWeightsByLabel(*tagJets.at(TagJet_i),"JetBTagCutWeightCentral");
          JetBTagReshapeWeight = JetBTagReshapeWeight * tagJets.at(TagJet_i)->weight("JetBTagReshapeWeightCentral");
          UnmatchedPUWeight = UnmatchedPUWeight * tagJets.at(TagJet_i)->weight("UnmatchedPUWeightCentral");

          JetBTagReshapeWeight_up = JetBTagReshapeWeight_up * tagJets.at(TagJet_i)->weight("JetBTagReshapeWeightUp01sigma");
          UnmatchedPUWeight_up = UnmatchedPUWeight_up * tagJets.at(TagJet_i)->weight("UnmatchedPUWeightUp01sigma");

          JetBTagReshapeWeight_down = JetBTagReshapeWeight_down * tagJets.at(TagJet_i)->weight("JetBTagReshapeWeightDown01sigma");
          UnmatchedPUWeight_down = UnmatchedPUWeight_down * tagJets.at(TagJet_i)->weight("UnmatchedPUWeightDown01sigma");                    
        }  
      tag_obj_.setWeight("JetBTagReshapeWeightCentral",JetBTagReshapeWeight);
      tag_obj_.setWeight("UnmatchedPUWeightCentral",UnmatchedPUWeight);  

      tag_obj_.setWeight("JetBTagReshapeWeightUp01sigma",JetBTagReshapeWeight_up);
      tag_obj_.setWeight("UnmatchedPUWeightUp01sigma",UnmatchedPUWeight_up);  

      tag_obj_.setWeight("JetBTagReshapeWeightDown01sigma",JetBTagReshapeWeight_down);
      tag_obj_.setWeight("UnmatchedPUWeightDown01sigma",UnmatchedPUWeight_down);                              

      // Diphoton 
      LooseMvaSF = dipho->weight("LooseMvaSFCentral");
      PreselSF = dipho->weight("PreselSFCentral");
      TriggerWeight = dipho->weight("TriggerWeightCentral");
      electronVetoSF = dipho->weight("electronVetoSFCentral");
      prefireWeight = dipho->weight("prefireWeightCentral");

      LooseMvaSF_up = dipho->weight("LooseMvaSFUp01sigma");
      PreselSF_up = dipho->weight("PreselSFUp01sigma");
      TriggerWeight_up = dipho->weight("TriggerWeightUp01sigma");
      electronVetoSF_up = dipho->weight("electronVetoSFUp01sigma");
      prefireWeight_up = dipho->weight("prefireWeightUp01sigma");

      LooseMvaSF_down = dipho->weight("LooseMvaSFDown01sigma");
      PreselSF_down = dipho->weight("PreselSFDown01sigma");
      TriggerWeight_down = dipho->weight("TriggerWeightDown01sigma");
      electronVetoSF_down = dipho->weight("electronVetoSFDown01sigma");
      prefireWeight_down = dipho->weight("prefireWeightDown01sigma");    

      diPhoCentralWeight = dipho->centralWeight();

      if(prefireWeight == diPhoCentralWeight) {
        prefireWeight = 1; 
        prefireWeight_up = 1; 
        prefireWeight_down = 1; 
      }
      
      tag_obj_.setWeight("LooseMvaSFCentral",LooseMvaSF);
      tag_obj_.setWeight("PreselSFCentral",PreselSF);
      tag_obj_.setWeight("TriggerWeightCentral",TriggerWeight);
      tag_obj_.setWeight("electronVetoSFCentral",electronVetoSF);
      tag_obj_.setWeight("prefireWeightCentral",prefireWeight);

      tag_obj_.setWeight("LooseMvaSFUp01sigma",LooseMvaSF_up);
      tag_obj_.setWeight("PreselSFUp01sigma",PreselSF_up);
      tag_obj_.setWeight("TriggerWeightUp01sigma",TriggerWeight_up);
      tag_obj_.setWeight("electronVetoSFUp01sigma",electronVetoSF_up);
      tag_obj_.setWeight("prefireWeightUp01sigma",prefireWeight_up);

      tag_obj_.setWeight("LooseMvaSFDown01sigma",LooseMvaSF_down);
      tag_obj_.setWeight("PreselSFDown01sigma",PreselSF_down);
      tag_obj_.setWeight("TriggerWeightDown01sigma",TriggerWeight_down);
      tag_obj_.setWeight("electronVetoSFDown01sigma",electronVetoSF_down);
      tag_obj_.setWeight("prefireWeightDown01sigma",prefireWeight_down);            

      // Multiply prefire weight by hand  
      TagObjCentralWeight = tag_obj_.centralWeight();
      TagObjCentralWeight = TagObjCentralWeight * prefireWeight;
      tag_obj_.setCentralWeight(TagObjCentralWeight);   

      return tag_obj_;    
    }

    void HHWWggTagProducer::produce( Event &event, const EventSetup & )
    {

      if (doHHWWggDebug_) cout << "[HHWWggTagProducer.cc - HHWWggTagProducer::produce] - systLabel: " << systLabel_ << endl;  
      // nEvents->Fill(1.0);

      // Set cut booleans
      std::vector<double> Cut_Variables(30,0.0); // Cut_Variables[i] = 1.0: Event Passed Cut i
      std::vector<double> MuonVars; // For saving Muon ID's and isolation
      std::vector<double> JetVars;

      // Cut Variables
      double pass_leadPhoOverMassThreshold = 0, pass_subleadPhoOverMassThreshold = 0;

      // Tag Variable
      bool FilledTag = 0; 

      // Get particle objects
      // event.getByToken( photonToken_, photons );
      event.getByToken( diphotonToken_, diphotons );
      event.getByToken( electronToken_, electrons );
      event.getByToken( muonToken_, muons );
      event.getByToken( METToken_, METs );
      event.getByToken( mvaResultToken_, mvaResults );
      event.getByToken( vertexToken_, vertices );
      event.getByToken( rhoTag_, rho);
      double rho_    = *rho;

      // For FL. Should this be in dipho loop? 
      Savejet=1;

      //---output collection
      int n_good_electrons = 0;
      int n_good_muons = 0;
      int n_good_leptons = 0;
      int n_good_jets = 0;
      bool hasHighbTag = 0;
      float btagVal = 0;

      // Vertex 
      double GenVtx_z = -999; 
      double HggVtx_z = -999;
      double ZeroVtx_z = -999; 

      // Misc
      int num_FL_dr = 0;
      int catnum = 3; // category number. default to untagged
      double DiphoCentralWeight = 1;
      double prefireWeight = 1; // If you are setting prefire weight by hand 
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
      bool keepJet = true;

      // Saved Objects after selections
      std::vector<edm::Ptr<flashgg::Electron> > goodElectrons;
      std::vector<edm::Ptr<flashgg::Muon> > goodMuons; 

      std::unique_ptr<vector<TagTruthBase> > truths( new vector<TagTruthBase> );
      edm::RefProd<vector<TagTruthBase> > rTagTruth = event.getRefBeforePut<vector<TagTruthBase> >();

//-----------------------------------------------------------------------------------------------------------

      // Vertex variables
      edm::Ptr<reco::Vertex> diphoton_vertex;
      edm::Ptr<reco::Vertex> zero_vertex;

      if (vertices->size() > 0){
        zero_vertex = vertices->ptrAt( 0 );
        ZeroVtx_z = zero_vertex->z();
      }

      //-- MC truth
      TagTruthBase truth_obj;
      double genMhh = 0.;
      double genCosThetaStar_CS = 0.;      
      if( ! event.isRealData() ) {
          Handle<View<reco::GenParticle> > genParticles;
          std::vector<edm::Ptr<reco::GenParticle> > selHiggses;
          event.getByToken( genParticleToken_, genParticles );
          reco::GenParticle::Point higgsVtx(0.,0.,0.);

          for( unsigned int genLoop = 0 ; genLoop < genParticles->size(); genLoop++ ) {
               edm::Ptr<reco::GenParticle> genPar = genParticles->ptrAt(genLoop);
               if (selHiggses.size()>1) break;
              if (genPar->pdgId()==25 && genPar->isHardProcess()){
                  selHiggses.push_back(genPar); // to obtain diHiggs 
                  higgsVtx = genParticles->ptrAt( genLoop )->vertex(); // for dZ calculation
                  GenVtx_z = higgsVtx.z();
              } 
          }
          if (selHiggses.size()==2){
              TLorentzVector H1,H2;
              H1.SetPtEtaPhiE(selHiggses[0]->p4().pt(),selHiggses[0]->p4().eta(),selHiggses[0]->p4().phi(),selHiggses[0]->p4().energy());
              H2.SetPtEtaPhiE(selHiggses[1]->p4().pt(),selHiggses[1]->p4().eta(),selHiggses[1]->p4().phi(),selHiggses[1]->p4().energy());
              genMhh  = (H1+H2).M();
              genCosThetaStar_CS = getGenCosThetaStar_CS(H1,H2);   
          }
          truth_obj.setGenPV( higgsVtx );
          truths->push_back( truth_obj );
      }

      // METfilters
      // bool passMETfilters=1;
      // Get trigger results relevant to MET filters

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

        std::unique_ptr<vector<HHWWggTag> > HHWWggtags( new vector<HHWWggTag> );

        //-- Loop through preselected diphotons. If more than one, it's possible the 2nd preselected diphoton will be tagged but not the 1st.
        for( unsigned int diphoIndex = 0; diphoIndex < diphotons->size(); diphoIndex++ )
        { 

          //-- Get Diphoton and Diphoton MVA 
          edm::Ptr<flashgg::DiPhotonCandidate> dipho = diphotons->ptrAt( diphoIndex ); // without systematic look (?)
          edm::Ptr<flashgg::DiPhotonMVAResult> mvares = mvaResults->ptrAt( diphoIndex );

          //-- Get Photons
          const flashgg::Photon* leadPho = dipho->leadingPhoton();
          const flashgg::Photon* subleadPho = dipho->subLeadingPhoton();

           //-- Vertex Info 
          diphoton_vertex = dipho->vtx();
          if(HHWWgguseZeroVtx_) HggVtx_z = -999; 
          else HggVtx_z = diphoton_vertex->z();
          
          // leading/subleading photon pt
          if( dipho->leadingPhoton()->pt() > ( dipho->mass() )*leadPhoOverMassThreshold_ ){
              pass_leadPhoOverMassThreshold = 1;
            }
          if( dipho->subLeadingPhoton()->pt() > ( dipho->mass() )*subleadPhoOverMassThreshold_ ) {
              pass_subleadPhoOverMassThreshold = 1;
            }

          // // Doing cut flow analysis: Don't skip event, but check if photon selections are passed
          if(doHHWWggTagCutFlowAnalysis_){
            if( !pass_leadPhoOverMassThreshold || !pass_subleadPhoOverMassThreshold ) Cut_Variables[1] = 0.0; // failed photon selections
            else Cut_Variables[1] = 1.0; // passed photon selections
          }

          // // Not doing cut flow analysis: Skip event if photon selections are not passed
          else{
            if( !pass_leadPhoOverMassThreshold || !pass_subleadPhoOverMassThreshold ) continue; // Do not save event if leading and subleading photons don't pass pt/mgg cuts
          }

          // Check MET Filters
          //-- Is this done in a separate module? 
          // if(passMETfilters){
          //   pass_METfilters = 1;
          // }

          hasGoodElec = false;
          hasGoodMuons = false;
          dipho_MVA = mvares->result;

          // Electrons
          goodElectrons = selectStdElectrons( electrons->ptrs(), dipho, vertices->ptrs(), leptonPtThreshold_, electronEtaThresholds_,
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
          goodMuons = selectMuons( muons->ptrs(), dipho, vertices->ptrs(), muonEtaThreshold_, leptonPtThreshold_,
          muPFIsoSumRelThreshold_, deltaRMuonPhoThreshold_, deltaRMuonPhoThreshold_ );

          // If doing cut flow analysis, save Muon IDs
          if(doHHWWggTagCutFlowAnalysis_) MuonVars = GetMuonVars(muons->ptrs(), vertices->ptrs());

          n_good_electrons = goodElectrons.size();
          n_good_muons = goodMuons.size();
          n_good_leptons = n_good_electrons + n_good_muons;
          hasGoodElec = ( goodElectrons.size() > 0 );
          hasGoodMuons = ( goodMuons.size() > 0 );

          // Jets
          unsigned int jetCollectionIndex = diphotons->at( diphoIndex ).jetCollectionIndex(); // not looping over systematics
          edm::Handle<edm::View<flashgg::Jet> > Jets_;
          event.getByToken( jetTokens_[jetCollectionIndex], Jets_); 
          std::vector<edm::Ptr<Jet> > tagJets;

          // If doing cut flow analysis, save Jet IDs
          if(doHHWWggTagCutFlowAnalysis_) JetVars = GetJetVars(Jets_->ptrs(), dipho);

          // Jet Selections
          for( unsigned int candIndex_outer = 0; candIndex_outer <  Jets_->size() ; candIndex_outer++ )
              {
                  keepJet = true;
                  edm::Ptr<flashgg::Jet> thejet = Jets_->ptrAt( candIndex_outer );
                  allJets.push_back(thejet);

                  // JetID selection
                  // Do not save jet in goodJets if it doesn't pass the recommended ID 
                  if( JetIDLevel_ == "Loose" && !thejet->passesJetID  ( flashgg::Loose ) ) continue;
                  if( JetIDLevel_ == "Tight" && !thejet->passesJetID  ( flashgg::Tight ) ) continue;
                  if( JetIDLevel_ == "Tight2017" && !thejet->passesJetID  ( flashgg::Tight2017 ) ) continue;
                  if( JetIDLevel_ == "Tight2018" && !thejet->passesJetID  ( flashgg::Tight2018 ) ) continue;                   

                  // Eta selection                 
                  if( fabs( thejet->eta() ) > jetEtaThreshold_ ) { continue; }

                  // pT Selection
                  if( thejet->pt() < jetPtThreshold_ ) { continue; }

                  // dR Selections
                  float dRPhoLeadJet = deltaR( thejet->eta(), thejet->phi(), dipho->leadingPhoton()->superCluster()->eta(), dipho->leadingPhoton()->superCluster()->phi() ) ;
                  float dRPhoSubLeadJet = deltaR( thejet->eta(), thejet->phi(), dipho->subLeadingPhoton()->superCluster()->eta(),
                                                  dipho->subLeadingPhoton()->superCluster()->phi() );

                  if( dRPhoLeadJet < deltaRPhoLeadJet_ || dRPhoSubLeadJet < deltaRPhoSubLeadJet_ ) { continue; }

                  if( hasGoodElec )
                      for( unsigned int electronIndex = 0; electronIndex < goodElectrons.size(); electronIndex++ )
                          {
                              Ptr<flashgg::Electron> electron = goodElectrons[electronIndex];
                              float dRJetElectron = deltaR( thejet->eta(), thejet->phi(), electron->eta(), electron->phi() ) ;
                              if( dRJetElectron < deltaRJetMuonThreshold_ ){ keepJet = false; }
                          }

                  if( hasGoodMuons )
                      for( unsigned int muonIndex = 0; muonIndex < goodMuons.size(); muonIndex++ )
                          {
                              Ptr<flashgg::Muon> muon = goodMuons[muonIndex];
                              float dRJetMuon = deltaR( thejet->eta(), thejet->phi(), muon->eta(), muon->phi() ) ;
                              if( dRJetMuon < deltaRJetMuonThreshold_ ) { keepJet = false; }
                          }

                  if(keepJet)
                      tagJets.push_back( thejet );

              }

          // If jet collection has a jet suspected to be a bjet, don't save the event
          // Need to use working point 
          hasHighbTag = 0; // collection has jet with high b score 
          Savejet = 1;
          for (unsigned int j = 0; j < tagJets.size(); j++){
            Ptr<flashgg::Jet> jet_ = tagJets[j];
            btagVal = 0; 
            for(unsigned int BTagTypes_i = 0; BTagTypes_i < BTagTypes_.size(); BTagTypes_i ++){
              btagVal += jet_->bDiscriminator(BTagTypes_[BTagTypes_i]);
            }
            if (btagVal > btagThresh_) hasHighbTag = 1;
            if (  jet_->pt() >20 && (btagVal > btagThresh_)) Savejet = 0;
          }

          if(doHHWWggDebug_){
            if(Savejet == 0){
              cout << "*****************************************************************************************************" << endl;
              cout << "HHWWgg DEBUG - SaveJet = 0. This only makes sense if bthreshold is between 0-1 " << endl; 
              cout << "*****************************************************************************************************" << endl;
            }
          }

          // If doing cut flow analysis, don't skip event
          if(doHHWWggTagCutFlowAnalysis_){
            if(hasHighbTag)
              Cut_Variables[2] = 0.0; // does not pass bveto
            else
              Cut_Variables[2] = 1.0; // passes bveto
          }
          n_good_jets = tagJets.size(); 

          // MET
          if( METs->size() != 1 ) { std::cout << "WARNING - #MET is not 1" << std::endl;}
          Ptr<flashgg::Met> theMET = METs->ptrAt( 0 );

          //-- Categorize Events
          // In the semi-leptonic final state, because the categorization is done with MVAs, will just save all events orthogonal to the other two final states 
          if (doHHWWggDebug_) std::cout << "[HHWWggTagProducer] n_good_leptons = " << n_good_leptons << ",\t n_good_jets = " << n_good_jets << std::endl;

          // Three final state tags should be orthogonal because don't want to use same event in multiple background models because then it's incorrect to combine them 

          //-- Semi-Leptonic Final state tags
          if( (HHWWggAnalysisChannel_ == "SL" || HHWWggAnalysisChannel_ == "all") && FilledTag == 0)
          {
            if ( n_good_leptons == 1 ) {
              catnum = 0;
              if(doHHWWggDebug_) cout << "Filling Semi-Leptonic category..." << endl;
              HHWWggTag tag_obj;
              if(doHHWWggTagCutFlowAnalysis_){
                HHWWggTag tag_obj_(dipho, allElectrons, goodElectrons, allMuons, goodMuons, theMET, allJets, tagJets, Cut_Variables, MuonVars, JetVars);
                tag_obj_.setGenVtx_z(GenVtx_z);
                tag_obj_.setHggVtx_z(HggVtx_z);
                tag_obj_.setZeroVtx_z(ZeroVtx_z);                  
                tag_obj = tag_obj_;                
              }

              // If not doing cutflow analysis, save the minimum which is just the dipho information for the final fit, to keep process and output lightweight 
              else{
                HHWWggTag tag_obj_(dipho); // diphoton
                tag_obj = tag_obj_;
              } 

              // Set tag object attributes 
              tag_obj.setSystLabel( systLabel_);
              tag_obj.setDiPhotonIndex( diphoIndex );
              tag_obj.setCategoryNumber( catnum );
              tag_obj.setGenMhh( genMhh );
              tag_obj.setGenCosThetaStar_CS( genCosThetaStar_CS ); 
              DiphoCentralWeight = dipho->centralWeight();
              prefireWeight = dipho->weight("prefireWeightCentral"); // if setting pre fire weight by hand 
              DiphoCentralWeight = DiphoCentralWeight * prefireWeight; 
              tag_obj.SetDiphoCentralWeight(DiphoCentralWeight);

              //-- Include Scale Factors 
              // Set CentralWeight values for each SF to access in trees 
              tag_obj = SetCentralUpDownWeights(tag_obj, goodElectrons, goodMuons, tagJets, dipho, doHHWWggDebug_);   

              // if(doHHWWggDebug_){
              //   cout << "**************************************************" << endl; 
              //   cout << " Tag Object weights: " << endl; 
              //   PrintScaleFactorsObj(tag_obj);
              // }

              // Push back tag object 
              HHWWggtags->push_back( tag_obj );
              FilledTag = 1; 
              if( ! event.isRealData() ) {
                HHWWggtags->back().setTagTruth( edm::refToPtr( edm::Ref<vector<TagTruthBase> >( rTagTruth, 0 ) ) );
              }

            } // if (n_good_leptons == 1)
          } // Semileptonic cateogries

          //-- Fully Hadronic Final State Tags
          if( (HHWWggAnalysisChannel_ == "FH" || HHWWggAnalysisChannel_ == "all") && (FilledTag == 0))
          {
            double diPho_pT=dipho->pt();
            if (n_good_leptons==0 && n_good_jets>=4 && diPho_pT > FH_Dipho_pT_Thre_)
            {
              catnum = 1;
              // Ptr<flashgg::Met> theMET = METs->ptrAt( 0 );

              // Define four jets with WH min method, or just take four leading pT
              if(doHHWWggFHptOrdered_){
                if (doHHWWggDebug_) std::cout << "\n\n=============> doHHWWggFHptOrdered_ ==================\n\n" << std::endl;
                FHJets = GetFHPtOrderedJets(doHHWWggDebug_, tagJets);
                jet1 = FHJets[0];
                jet2 = FHJets[1];
                jet3 = FHJets[2];
                jet4 = FHJets[3];
              }
              else if (doHHWWggFHminWHJets_)
              {
                if (doHHWWggDebug_) std::cout << "\n\n=============> doHHWWggFHminWHJets_ ==================\n\n" << std::endl;
                FHJets = GetFHminWHJets(doHHWWggDebug_, tagJets);
                jet1 = FHJets[0];
                jet2 = FHJets[1];
                jet3 = FHJets[2];
                jet4 = FHJets[3];
              }
              else if (doHHWWggFHminWHLead2Jet_)
              {
                if (doHHWWggDebug_) std::cout << "\n\n=============> doHHWWggFHminWHLead2Jet_ ==================\n\n" << std::endl;
                FHJets = GetFHminWHLead2Jet(doHHWWggDebug_, tagJets);
                jet1 = FHJets[0];
                jet2 = FHJets[1];
                jet3 = FHJets[2];
                jet4 = FHJets[3];
              }
              else if (doHHWWggFHminHiggsMassOnly_) // kept doHHWWggFHminHiggsMassOnly_ to true in Taggers/python/flashggHHWWggTag_cfi.py; this ensures that this won't eneter in the last else
              {
                if (doHHWWggDebug_) std::cout << "\n\n=============> doHHWWggFHminHiggsMassOnly_ ==================\n\n" << std::endl;
                FHJets = GetFHminHiggsMassOnly(doHHWWggDebug_, tagJets);
                jet1 = FHJets[0];
                jet2 = FHJets[1];
                jet3 = FHJets[2];
                jet4 = FHJets[3];
              }
              else {
                std::cout << "You need to switch on any one hadronic selections..." << std::endl;
                std::cout << "==> HELP: doHHWWggFHptOrdered=1  or "                 << std::endl;
                std::cout << "          doHHWWggFHminWHJets=1  or "                 << std::endl;
                std::cout << "          doHHWWggFHminWHLead2Jet=1  or "             << std::endl;
                std::cout << "          doHHWWggFHminHiggsMassOnly=1 "              << std::endl;
                std::cout << "==> Please you any one options while running "        << std::endl;
                exit(0);
              }
              if(doHHWWggDebug_) cout << "Filling Fully Hadronic category..." << endl;
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
                tag_obj.setGenVtx_z(GenVtx_z);
                tag_obj.setHggVtx_z(HggVtx_z);
                tag_obj.setZeroVtx_z(ZeroVtx_z);                  
              }
              else{
                HHWWggTag tag_obj_(dipho); // diphoton, electron, MET, jet1, jet2
                tag_obj = tag_obj_;
              }
              tag_obj.setSystLabel( systLabel_);
              tag_obj.setDiPhotonIndex( diphoIndex );
              tag_obj.setCategoryNumber( catnum );
              tag_obj.setGenMhh( genMhh );
              tag_obj.setGenCosThetaStar_CS( genCosThetaStar_CS );      

              //-- Include Scale Factors 
              DiphoCentralWeight = dipho->centralWeight();
              prefireWeight = dipho->weight("prefireWeightCentral"); // if setting pre fire weight by hand 
              DiphoCentralWeight = DiphoCentralWeight * prefireWeight; 
              tag_obj.SetDiphoCentralWeight(DiphoCentralWeight);
                 
              //-- Include Scale Factors 
              // Set CentralWeight values for each SF to access in trees 
              tag_obj = SetCentralUpDownWeights(tag_obj, goodElectrons, goodMuons, tagJets, dipho, doHHWWggDebug_);                

              HHWWggtags->push_back( tag_obj );
              FilledTag = 1;
              if( ! event.isRealData() ) {
                HHWWggtags->back().setTagTruth( edm::refToPtr( edm::Ref<vector<TagTruthBase> >( rTagTruth, 0 ) ) );
              }
            }  // if (n_good_leptons==0 && n_good_jets>=4)
          } // Fully Hadronic Categories

          //-- Fully-Leptonic Final State 

          if( (HHWWggAnalysisChannel_ == "FL" || HHWWggAnalysisChannel_ == "all") && FilledTag == 0)
          {  
            double diPho_pT=dipho->pt();
            if(n_good_leptons >= 2 && diPho_pT > FL_Dipho_pT_Thre_ ){

            num_FL_dr = GetNumFLDR(goodElectrons, goodMuons, deltaRLeps_);

            if ( (n_good_leptons >=2 ) && (theMET->getCorPt() >= MetPtThreshold_) && num_FL_dr>=1 && (leadPho->p4().pt()+subleadPho->p4().pt())>0){
              if(doHHWWggDebug_) cout << "Filling Fully-Leptonic category..." << endl;
              catnum = 2; 
              double Save=0.;
              double MassT;
              double MassT_l2;
              // double lep3pt = 0.;
              Ptr<flashgg::Electron> tag_electron1;
              Ptr<flashgg::Electron> tag_electron3;
              Ptr<flashgg::Electron> tag_electron2;
              Ptr<flashgg::Muon> tag_muon1;
              Ptr<flashgg::Muon> tag_muon3;
              Ptr<flashgg::Muon> tag_muon2;
              sort(goodElectrons.begin(),goodElectrons.end(),compEle);
              sort(goodMuons.begin(),goodMuons.end(),compMu);
              if (n_good_electrons >= 2){
                //std::cout<<"ele1:"<<goodElectrons[0]->pt()<<"ele2:"<<goodElectrons[1]->pt()<<"ele3:"<<goodElectrons[2]->pt()<<std::endl;
                if(n_good_electrons == 2 && n_good_muons < 1){
                tag_electron1 = goodElectrons[0];
                tag_electron2 = goodElectrons[1];
                Save = 1.;
                }
                else if(n_good_electrons > 2 && n_good_muons < 1){
                tag_electron1 = goodElectrons[0];
                tag_electron2 = goodElectrons[1];
                tag_electron3 = goodElectrons[2];
                Save = 1.;
                }
                else if(n_good_electrons ==2 && n_good_muons >=1){
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
                }
                reco::Candidate::LorentzVector Ele1LV = tag_electron1->p4();
                reco::Candidate::LorentzVector Ele2LV = tag_electron2->p4();
                reco::Candidate::LorentzVector DiEleLV = Ele1LV +  Ele2LV;
                MassT= sqrt(2*DiEleLV.Pt()*theMET->pt()*(1-cos(abs(DiEleLV.Phi()-theMET->phi()))));
                MassT_l2 = sqrt(2*Ele2LV.Pt()*theMET->pt()*(1-cos(abs(Ele2LV.Phi()-theMET->phi()))));
                  if((tag_electron1->charge()*tag_electron2->charge()==-1) && tag_electron1->pt()>=lep1ptThre_ && tag_electron2->pt()>=lep1ptThre_  && DiEleLV.Pt()>DiLepPtThre_ && (DiEleLV.M() > DiLepMassHigThre_ || DiEleLV.M() < DiLepMassLowThre_ )  && MassT>MassTThre_ && MassT_l2 >MassT_l2Thre_ && Save == 1. && Savejet==1){
                    HHWWggTag tag_obj;

                    if(doHHWWggTagCutFlowAnalysis_){
                      Cut_Variables[18]=0.;
                      HHWWggTag tag_obj_(dipho, tag_electron1, tag_electron2, theMET, Cut_Variables, dipho_MVA); 
                      tag_obj = tag_obj_;    
                      tag_obj.SetGoodJets(tagJets); 
                      tag_obj.SetAllJets(allJets); 
                      tag_obj.setGenVtx_z(GenVtx_z);
                      tag_obj.setHggVtx_z(HggVtx_z);
                      tag_obj.setZeroVtx_z(ZeroVtx_z);          
                    }

                    else{
                      HHWWggTag tag_obj_(dipho);
                      tag_obj = tag_obj_;
                    }

                  tag_obj.setSystLabel(systLabel_);
                  tag_obj.setDiPhotonIndex( diphoIndex );
                  tag_obj.setMVA( -0.9 );
                  tag_obj.setCategoryNumber( catnum );
                  DiphoCentralWeight = dipho->centralWeight();
                  prefireWeight = dipho->weight("prefireWeightCentral"); // if setting pre fire weight by hand 
                  DiphoCentralWeight = DiphoCentralWeight * prefireWeight; 
                  tag_obj.SetDiphoCentralWeight(DiphoCentralWeight);
                    
                  //-- Include Scale Factors 
                  // Set CentralWeight values for each SF to access in trees 
                  tag_obj = SetCentralUpDownWeights(tag_obj, goodElectrons, goodMuons, tagJets, dipho, doHHWWggDebug_);    

                  tag_obj.setGenMhh( genMhh );
                  tag_obj.setGenCosThetaStar_CS( genCosThetaStar_CS );                  
                  HHWWggtags->push_back( tag_obj );
                  FilledTag = 1;
                  if( ! event.isRealData() ) {
                    HHWWggtags->back().setTagTruth( edm::refToPtr( edm::Ref<vector<TagTruthBase> >( rTagTruth, 0 ) ) );
                  }

                }
              }

              if (n_good_muons >= 2  ){
                if (n_good_muons == 2 && n_good_electrons <1){
                  tag_muon1 = goodMuons[0];
                  tag_muon2 = goodMuons[1];
                  Save =1.;
                }
                else if(n_good_muons >2 && n_good_electrons <1 ){
                tag_muon1 = goodMuons[0];
                tag_muon2 = goodMuons[1];
                tag_muon3 = goodMuons[2];
                Save = 1.;
                }
                else if(n_good_muons ==2 && n_good_electrons >=1){
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
                if((tag_muon1->charge()*tag_muon2->charge() == -1) && tag_muon1->pt()>=lep1ptThre_ && tag_muon2->pt()>=lep2ptThre_  && DiMuLV.Pt()>DiLepPtThre_ && (DiMuLV.M() > DiLepMassHigThre_ || DiMuLV.M() < DiLepMassLowThre_ ) && MassT>MassTThre_ && MassT_l2>MassT_l2Thre_ && Save == 1. && Savejet==1){
                  HHWWggTag tag_obj;

                  if(doHHWWggTagCutFlowAnalysis_){
                    Cut_Variables[18]=1.;
                    HHWWggTag tag_obj_(dipho, tag_muon1, tag_muon2, theMET, Cut_Variables, dipho_MVA);
                    tag_obj = tag_obj_;
                    tag_obj.SetGoodJets(tagJets); 
                    tag_obj.SetAllJets(allJets);    
                    tag_obj.setGenVtx_z(GenVtx_z);
                    tag_obj.setHggVtx_z(HggVtx_z);
                    tag_obj.setZeroVtx_z(ZeroVtx_z);        
                  }
                  else{
                    HHWWggTag tag_obj_(dipho);
                    tag_obj = tag_obj_;
                  }
                  tag_obj.setSystLabel(systLabel_);
                  tag_obj.setDiPhotonIndex( diphoIndex );
                  tag_obj.setMVA( -0.9 );
                  tag_obj.setCategoryNumber( catnum);
                  //-- Include Scale Factors 
                  DiphoCentralWeight = dipho->centralWeight();
                  prefireWeight = dipho->weight("prefireWeightCentral"); // if setting pre fire weight by hand 
                  DiphoCentralWeight = DiphoCentralWeight * prefireWeight; 
                  tag_obj.SetDiphoCentralWeight(DiphoCentralWeight);
                    
                  //-- Include Scale Factors 
                  // Set CentralWeight values for each SF to access in trees 
                  tag_obj = SetCentralUpDownWeights(tag_obj, goodElectrons, goodMuons, tagJets, dipho, doHHWWggDebug_);    

                  tag_obj.setGenMhh( genMhh );
                  tag_obj.setGenCosThetaStar_CS( genCosThetaStar_CS );                  
                  HHWWggtags->push_back( tag_obj );
                  FilledTag = 1;
                  if( ! event.isRealData() ) {
                    HHWWggtags->back().setTagTruth( edm::refToPtr( edm::Ref<vector<TagTruthBase> >( rTagTruth, 0 ) ) );
                  }
              }
            }



              if (n_good_muons >=1 && n_good_electrons >= 1){
              if (n_good_electrons == 1 && n_good_muons == 1){
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
                if((((tag_electron1->pt()>=lep1ptThre_) && (tag_muon1->pt()>=lep2ptThre_))||((tag_muon1->pt()>=lep1ptThre_) && (tag_electron1->pt()>=lep2ptThre_))) && (tag_muon1->charge()*tag_electron1->charge()==-1) && (DiLepLV.M() > DiLepMassHigThre_ || DiLepLV.M() < DiLepMassLowThre_ )  && (DiLepLV.Pt()>DiLepPtThre_) && (MassT_l2>MassTThre_) && (MassT>MassT_l2Thre_) && (Save==1.) &&(Savejet==1)){
                  HHWWggTag tag_obj;
                  if(doHHWWggTagCutFlowAnalysis_){
                    if(tag_electron1->pt()>tag_muon1->pt()) Cut_Variables[18]=2.;//e mu
                    else Cut_Variables[18]=3.; //mu e
                    HHWWggTag tag_obj_(dipho, tag_electron1, tag_muon1, theMET, Cut_Variables, dipho_MVA);
                    tag_obj = tag_obj_; 
                    tag_obj.SetGoodJets(tagJets); 
                    tag_obj.SetAllJets(allJets);                     
                    tag_obj.setGenVtx_z(GenVtx_z);
                    tag_obj.setHggVtx_z(HggVtx_z);
                    tag_obj.setZeroVtx_z(ZeroVtx_z);             
                  }
                  else{
                    HHWWggTag tag_obj_(dipho);
                    tag_obj = tag_obj_;
                  }
                  tag_obj.setSystLabel(systLabel_);
                  tag_obj.setDiPhotonIndex( diphoIndex );
                  tag_obj.setMVA( -0.9 );
                  tag_obj.setCategoryNumber( catnum );
                  //-- Include Scale Factors 
                  DiphoCentralWeight = dipho->centralWeight();
                  prefireWeight = dipho->weight("prefireWeightCentral"); // if setting pre fire weight by hand 
                  DiphoCentralWeight = DiphoCentralWeight * prefireWeight; 
                  tag_obj.SetDiphoCentralWeight(DiphoCentralWeight);
                    
                  //-- Include Scale Factors 
                  // Set CentralWeight values for each SF to access in trees 
                  tag_obj = SetCentralUpDownWeights(tag_obj, goodElectrons, goodMuons, tagJets, dipho, doHHWWggDebug_);    

                  tag_obj.setGenMhh( genMhh );
                  tag_obj.setGenCosThetaStar_CS( genCosThetaStar_CS );                  
                  HHWWggtags->push_back( tag_obj );
                  FilledTag = 1;
                  if( ! event.isRealData() ) {
                    HHWWggtags->back().setTagTruth( edm::refToPtr( edm::Ref<vector<TagTruthBase> >( rTagTruth, 0 ) ) );
                  }
              }
              }
              } // if n_good_leptons >= 2 and other FL cat selections met
            } // if n_good_leptons >= 2
          } // FL selection

          // Untagged category
          // Only push tag into this category if catnum has not changed (not tagged by other categories)
          if(FilledTag == 0){
            if(doHHWWggTagCutFlowAnalysis_){
              if(doHHWWggDebug_) cout << "Filling untagged category..." << endl;
              catnum = 3;
              HHWWggTag tag_obj(dipho, allElectrons, goodElectrons, allMuons, goodMuons, theMET, allJets, tagJets, Cut_Variables, MuonVars, JetVars);
              tag_obj.setSystLabel(systLabel_);
              tag_obj.setDiPhotonIndex( diphoIndex );
              tag_obj.setCategoryNumber( catnum ); // Untagged category. Does not meet any selection criteria but want to save event
              tag_obj.setGenMhh( genMhh );
              tag_obj.setGenCosThetaStar_CS( genCosThetaStar_CS ); 
              tag_obj.setGenVtx_z(GenVtx_z);
              tag_obj.setHggVtx_z(HggVtx_z);
              tag_obj.setZeroVtx_z(ZeroVtx_z);                

              //-- Include Scale Factors 
              DiphoCentralWeight = dipho->centralWeight();
              prefireWeight = dipho->weight("prefireWeightCentral"); // if setting pre fire weight by hand 
              DiphoCentralWeight = DiphoCentralWeight * prefireWeight; 
              tag_obj.SetDiphoCentralWeight(DiphoCentralWeight);
                 
              //-- Include Scale Factors 
              // Set CentralWeight values for each SF to access in trees 
              tag_obj = SetCentralUpDownWeights(tag_obj, goodElectrons, goodMuons, tagJets, dipho, doHHWWggDebug_);   

              // if(doHHWWggDebug_){
              //   cout << "**************************************************" << endl; 
              //   cout << " Tag Object weights: " << endl; 
              //   PrintScaleFactorsObj(tag_obj);
              // }

              // Push back tag object 
              HHWWggtags->push_back( tag_obj );
              FilledTag = 1;  // actually need to configure this so that untagged isn't filled unless other preselected diphotons checked 
              if( ! event.isRealData() ) {
                HHWWggtags->back().setTagTruth( edm::refToPtr( edm::Ref<vector<TagTruthBase> >( rTagTruth, 0 ) ) );
              }
            } // doHHWWggTagCutFlowAnalysis_
          } // Untagged category: if FilledTag == 0
        } // Preselected Diphoton loop 
      event.put( std::move( HHWWggtags ) );
      event.put( std::move( truths ) );
    } // HHWWggTagProducer::produce

  } // namespace flashgg

  typedef flashgg::HHWWggTagProducer FlashggHHWWggTagProducer;
  DEFINE_FWK_MODULE( FlashggHHWWggTagProducer );
