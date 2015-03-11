// system include files
#include <memory>
#include <cmath>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/Common/interface/Ptr.h"
#include "DataFormats/Common/interface/PtrVector.h"

#include "DataFormats/Math/interface/deltaR.h"
#include "FWCore/Common/interface/TriggerNames.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "DataFormats/PatCandidates/interface/TriggerObjectStandAlone.h"
#include "DataFormats/PatCandidates/interface/PackedTriggerPrescales.h"
#include "HLTrigger/HLTcore/interface/HLTConfigProvider.h"

#include "flashgg/DataFormats/interface/DiPhotonCandidate.h" 
#include "flashgg/DataFormats/interface/Electron.h"

#include "flashgg/DataFormats/interface/DiPhotonMVAResult.h"

#include <DataFormats/Math/interface/deltaR.h>

#include <set>
#include "TFile.h"
#include "TH1F.h"

class HLTEfficiency : public edm::EDAnalyzer {
public:
  explicit HLTEfficiency(const edm::ParameterSet&);
  ~HLTEfficiency();
  void init(const edm::TriggerResults &result, const edm::TriggerNames & triggerNames);
  //bool hltEvent(edm::Handle<edm::TriggerResults> triggerBits);
  bool onlineOfflineMatching(const edm::TriggerNames & triggerNames, edm::Handle<pat::TriggerObjectStandAloneCollection> triggerObjects, math::XYZTLorentzVector p4, std::string filterLabel, float dRmin);

  //void endJob(const edm::LuminosityBlock& lumiSeg, const edm::EventSetup& c);
  bool L1Matching(edm::Handle<edm::View<l1extra::L1EmParticle>> l1H, math::XYZTLorentzVector cand, float ptThreshold); 
  std::vector<math::XYZTLorentzVector> hltP4(const edm::TriggerNames & triggerNames, edm::Handle<pat::TriggerObjectStandAloneCollection> triggerObjects, std::vector<std::string> filterLabels);

private:
  virtual void analyze(const edm::Event&, const edm::EventSetup&) override;

  std::string outputFileName_;
  float diphoMVACut_;
  std::vector<std::string> tagFilterName_;
  std::vector<std::string> probeFilterName_;
  std::vector<std::string> filterName_;

  edm::EDGetTokenT<edm::View<flashgg::DiPhotonCandidate>> diphotons_;
  edm::EDGetTokenT<edm::View<flashgg::DiPhotonMVAResult>> mvaResultToken_;
  edm::EDGetTokenT<edm::View<flashgg::Electron>> eles_;
  edm::EDGetTokenT<edm::View<l1extra::L1EmParticle>> l1iso_;
  edm::EDGetTokenT<edm::View<l1extra::L1EmParticle>> l1noniso_;
  edm::EDGetTokenT<edm::TriggerResults> triggerBits_;
  edm::EDGetTokenT<pat::TriggerObjectStandAloneCollection> triggerObjects_;
  edm::EDGetTokenT<pat::PackedTriggerPrescales> triggerPrescales_;
  
  edm::ParameterSetID triggerNamesID_;
  std::map<std::string, unsigned int> trigger_indices;
  std::map<std::string, unsigned int> prescales;
  std::map<std::string, unsigned int> prescale_counter;

  TH1F* TAG_L1_eta;
  TH1F* PROBE_L1_15_eta;
  TH1F* PROBE_L1_10_eta;
  TH1F* TAG_L1_pt;
  TH1F* PROBE_L1_15_pt;
  TH1F* PROBE_L1_10_pt;
  TH1F* PROBE_L1_35_eta;
  TH1F* PROBE_L1_35_pt;
  TH1F* Zpeak;

  TH1F* PROBE_HLT_OR_eta_twosuite;
  TH1F* PROBE_HLT_OR_pt_twosuite;
  TH1F* PROBE_HLT_Iso_eta_twosuite;
  TH1F* PROBE_HLT_Iso_pt_twosuite;
  TH1F* PROBE_HLT_R9_eta_twosuite;
  TH1F* PROBE_HLT_R9_pt_twosuite;
  TH1F* TAG_HLT_eta_twosuite;
  TH1F* TAG_HLT_pt_twosuite;  

  TH1F* PROBE_HLT_OR_eta_twosuite_high;
  TH1F* PROBE_HLT_OR_pt_twosuite_high;
  TH1F* PROBE_HLT_Iso_eta_twosuite_high;
  TH1F* PROBE_HLT_Iso_pt_twosuite_high;
  TH1F* PROBE_HLT_R9_eta_twosuite_high;
  TH1F* PROBE_HLT_R9_pt_twosuite_high;
  TH1F* TAG_HLT_eta_twosuite_high;
  TH1F* TAG_HLT_pt_twosuite_high;  

  TH1F* PROBE_HLT_OR_eta_seeded;
  TH1F* PROBE_HLT_OR_pt_seeded;
  TH1F* PROBE_HLT_Iso_eta_seeded;
  TH1F* PROBE_HLT_Iso_pt_seeded;
  TH1F* PROBE_HLT_R9_eta_seeded;
  TH1F* PROBE_HLT_R9_pt_seeded;
  TH1F* TAG_HLT_eta_seeded;
  TH1F* TAG_HLT_pt_seeded;

  TH1F* PROBE_HLT_OR_eta_unseeded;
  TH1F* PROBE_HLT_OR_pt_unseeded;
  TH1F* PROBE_HLT_Iso_eta_unseeded;
  TH1F* PROBE_HLT_Iso_pt_unseeded;
  TH1F* PROBE_HLT_R9_eta_unseeded;
  TH1F* PROBE_HLT_R9_pt_unseeded;
  TH1F* TAG_HLT_eta_unseeded;
  TH1F* TAG_HLT_pt_unseeded;
};

HLTEfficiency::~HLTEfficiency() {
  
  TFile* file = new TFile(outputFileName_.c_str(), "recreate");
  Zpeak->Write();
  TAG_L1_eta->Write();
  PROBE_L1_15_eta->Write();
  PROBE_L1_10_eta->Write();
  TAG_L1_pt->Write();
  PROBE_L1_15_pt->Write();
  PROBE_L1_10_pt->Write();
  PROBE_L1_35_eta->Write();
  PROBE_L1_35_pt->Write();
  PROBE_HLT_OR_eta_twosuite->Write();
  PROBE_HLT_OR_pt_twosuite->Write();
  PROBE_HLT_Iso_eta_twosuite->Write();
  PROBE_HLT_Iso_pt_twosuite->Write();
  PROBE_HLT_R9_eta_twosuite->Write();
  PROBE_HLT_R9_pt_twosuite->Write();
  TAG_HLT_eta_twosuite->Write();
  TAG_HLT_pt_twosuite->Write();

  PROBE_HLT_OR_eta_twosuite_high->Write();
  PROBE_HLT_OR_pt_twosuite_high->Write();
  PROBE_HLT_Iso_eta_twosuite_high->Write();
  PROBE_HLT_Iso_pt_twosuite_high->Write();
  PROBE_HLT_R9_eta_twosuite_high->Write();
  PROBE_HLT_R9_pt_twosuite_high->Write();
  TAG_HLT_eta_twosuite_high->Write();
  TAG_HLT_pt_twosuite_high->Write();

  PROBE_HLT_OR_eta_seeded->Write();
  PROBE_HLT_OR_pt_seeded->Write();
  PROBE_HLT_Iso_eta_seeded->Write();
  PROBE_HLT_Iso_pt_seeded->Write();
  PROBE_HLT_R9_eta_seeded->Write();
  PROBE_HLT_R9_pt_seeded->Write();
  TAG_HLT_eta_seeded->Write();
  TAG_HLT_pt_seeded->Write();

  PROBE_HLT_OR_eta_unseeded->Write();
  PROBE_HLT_OR_pt_unseeded->Write();
  PROBE_HLT_Iso_eta_unseeded->Write();
  PROBE_HLT_Iso_pt_unseeded->Write();
  PROBE_HLT_R9_eta_unseeded->Write();
  PROBE_HLT_R9_pt_unseeded->Write();
  TAG_HLT_eta_unseeded->Write();
  TAG_HLT_pt_unseeded->Write();
  file->Close();
}

HLTEfficiency::HLTEfficiency(const edm::ParameterSet& iConfig):
  outputFileName_(iConfig.getParameter<std::string>("outputFileName")),
  diphoMVACut_(iConfig.getParameter<double>("diphoMVACut")),
  tagFilterName_(iConfig.getParameter<std::vector<std::string> >("tagFilterName")),
  probeFilterName_(iConfig.getParameter<std::vector<std::string> >("probeFilterName")),
  filterName_(iConfig.getParameter<std::vector<std::string>>("filterName")),
  diphotons_(consumes<edm::View<flashgg::DiPhotonCandidate>>(iConfig.getParameter<edm::InputTag>("diphotons"))),
  mvaResultToken_(consumes<edm::View<flashgg::DiPhotonMVAResult>>(iConfig.getParameter<edm::InputTag>("diphoMVAResult"))),
  eles_(consumes<edm::View<flashgg::Electron>>(iConfig.getParameter<edm::InputTag>("electrons"))),
  l1iso_(consumes<edm::View<l1extra::L1EmParticle>>(iConfig.getParameter<edm::InputTag>("l1Iso"))),
  l1noniso_(consumes<edm::View<l1extra::L1EmParticle>>(iConfig.getParameter<edm::InputTag>("l1NonIso"))),
  triggerBits_(consumes<edm::TriggerResults>(iConfig.getParameter<edm::InputTag>("bits"))),
  triggerObjects_(consumes<pat::TriggerObjectStandAloneCollection>(iConfig.getParameter<edm::InputTag>("objects"))),
  triggerPrescales_(consumes<pat::PackedTriggerPrescales>(iConfig.getParameter<edm::InputTag>("prescales"))) {
  
  if (tagFilterName_.size() != probeFilterName_.size()) {
    std::cout << "Need to specify the same numbers of tag and probe filters." << std::endl;
    abort();
  }

  Zpeak = new TH1F("Zpeak", "", 120, 60, 120);
  TAG_L1_eta = new TH1F("TAG_L1_eta","",45,-3,3);
  PROBE_L1_15_eta = new TH1F("PROBE_L1_15_eta","",45,-3,3);
  PROBE_L1_10_eta = new TH1F("PROBE_L1_10_eta","",45,-3,3);
  TAG_L1_pt = new TH1F("TAG_L1_pt","",60,10,70);
  PROBE_L1_15_pt = new TH1F("PROBE_L1_15_pt","",60,10,70);
  PROBE_L1_10_pt = new TH1F("PROBE_L1_10_pt","",60,10,70);
  PROBE_L1_35_eta = new TH1F("PROBE_L1_35_eta","",45,-3,3);
  PROBE_L1_35_pt = new TH1F("PROBE_L1_35_pt","",60,10,70);
  PROBE_HLT_OR_eta_twosuite = new TH1F("PROBE_HLT_OR_eta_twosuite","",45,-3,3);
  PROBE_HLT_OR_pt_twosuite = new TH1F("PROBE_HLT_OR_pt_twosuite","",60,10,70);
  PROBE_HLT_Iso_eta_twosuite = new TH1F("PROBE_HLT_ISO_eta_twosuite","",45,-3,3);
  PROBE_HLT_Iso_pt_twosuite = new TH1F("PROBE_HLT_ISO_pt_twosuite","",60,10,70);
  PROBE_HLT_R9_eta_twosuite = new TH1F("PROBE_HLT_R9_eta_twosuite","",45,-3,3);
  PROBE_HLT_R9_pt_twosuite = new TH1F("PROBE_HLT_R9_pt_twosuite","",60,10,70);
  TAG_HLT_eta_twosuite = new TH1F("TAG_HLT_eta_twosuite","",45,-3,3);
  TAG_HLT_pt_twosuite = new TH1F("TAG_HLT_pt_twosuite","",60,10,70);
  PROBE_HLT_OR_eta_twosuite_high = new TH1F("PROBE_HLT_OR_eta_twosuite_high","",45,-3,3);
  PROBE_HLT_OR_pt_twosuite_high = new TH1F("PROBE_HLT_OR_pt_twosuite_high","",60,10,70);
  PROBE_HLT_Iso_eta_twosuite_high = new TH1F("PROBE_HLT_ISO_eta_twosuite_high","",45,-3,3);
  PROBE_HLT_Iso_pt_twosuite_high = new TH1F("PROBE_HLT_ISO_pt_twosuite_high","",60,10,70);
  PROBE_HLT_R9_eta_twosuite_high = new TH1F("PROBE_HLT_R9_eta_twosuite_high","",45,-3,3);
  PROBE_HLT_R9_pt_twosuite_high = new TH1F("PROBE_HLT_R9_pt_twosuite_high","",60,10,70);
  TAG_HLT_eta_twosuite_high = new TH1F("TAG_HLT_eta_twosuite_high","",45,-3,3);
  TAG_HLT_pt_twosuite_high = new TH1F("TAG_HLT_pt_twosuite_high","",60,10,70);

  PROBE_HLT_OR_eta_unseeded = new TH1F("PROBE_HLT_OR_eta_unseeded","",45,-3,3);
  PROBE_HLT_OR_pt_unseeded = new TH1F("PROBE_HLT_OR_pt_unseeded","",60,10,70);
  PROBE_HLT_Iso_eta_unseeded = new TH1F("PROBE_HLT_ISO_eta_unseeded","",45,-3,3);
  PROBE_HLT_Iso_pt_unseeded = new TH1F("PROBE_HLT_ISO_pt_unseeded","",60,10,70);
  PROBE_HLT_R9_eta_unseeded = new TH1F("PROBE_HLT_R9_eta_unseeded","",45,-3,3);
  PROBE_HLT_R9_pt_unseeded = new TH1F("PROBE_HLT_R9_pt_unseeded","",60,10,70);
  TAG_HLT_eta_unseeded = new TH1F("TAG_HLT_eta_unseeded","",45,-3,3);
  TAG_HLT_pt_unseeded = new TH1F("TAG_HLT_pt_unseeded","",60,10,70);

  PROBE_HLT_OR_eta_seeded = new TH1F("PROBE_HLT_OR_eta_seeded","",45,-3,3);
  PROBE_HLT_OR_pt_seeded = new TH1F("PROBE_HLT_OR_pt_seeded","",60,10,70);
  PROBE_HLT_Iso_eta_seeded = new TH1F("PROBE_HLT_ISO_eta_seeded","",45,-3,3);
  PROBE_HLT_Iso_pt_seeded = new TH1F("PROBE_HLT_ISO_pt_seeded","",60,10,70);
  PROBE_HLT_R9_eta_seeded = new TH1F("PROBE_HLT_R9_eta_seeded","",45,-3,3);
  PROBE_HLT_R9_pt_seeded = new TH1F("PROBE_HLT_R9_pt_seeded","",60,10,70);
  TAG_HLT_eta_seeded = new TH1F("TAG_HLT_eta_seeded","",45,-3,3);
  TAG_HLT_pt_seeded = new TH1F("TAG_HLT_pt_seeded","",60,10,70);
}

void HLTEfficiency::init(const edm::TriggerResults &result, const edm::TriggerNames & triggerNames) {
  
  trigger_indices.clear();
  for (unsigned int i=0; i<triggerNames.triggerNames().size(); i++) {    
    std::string trimmedName = HLTConfigProvider::removeVersion(triggerNames.triggerName(i));
    //std::cout << trimmedName << std::endl;
    trigger_indices[trimmedName] = triggerNames.triggerIndex(triggerNames.triggerName(i));    
  }
}

//bool HLTEfficiency::hltEvent(edm::Handle<edm::TriggerResults> triggerBits) {
//
//  for (std::map<std::string, unsigned int>::const_iterator cit = trigger_indices.begin(); cit != trigger_indices.end(); cit++) {
//    if (triggerBits->accept(cit->second)) {
//      std::vector<std::string>::const_iterator it = find(tpTriggerName_.begin(), tpTriggerName_.end(), cit->first);
//      if (it != tpTriggerName_.end())
//	return true;
//    }
//  }
//    
//  return false;    
//}

bool HLTEfficiency::L1Matching(edm::Handle<edm::View<l1extra::L1EmParticle>> l1H, math::XYZTLorentzVector cand, float ptThreshold) {  
  
  const edm::PtrVector<l1extra::L1EmParticle>& l1Pointers = l1H->ptrVector();
  //for (size_t i=0; i<l1Pointers.size(); i++) {
  
  for (edm::Ptr<l1extra::L1EmParticle> l1Ptr : l1Pointers) {
    
    float dR = deltaR(l1Ptr->p4(), cand);
    if (dR < 0.2 and l1Ptr->et() > ptThreshold)
      return true;
  }

  return false;
}

std::vector<math::XYZTLorentzVector> HLTEfficiency::hltP4(const edm::TriggerNames & triggerNames, edm::Handle<pat::TriggerObjectStandAloneCollection> triggerObjects, std::vector<std::string> filterLabels) {
  
  std::vector< math::XYZTLorentzVector> triggerCandidates;
  
  for (pat::TriggerObjectStandAlone obj : *triggerObjects) { 
    obj.unpackPathNames(triggerNames); 
    for (std::string filter : filterLabels) {
      if (obj.hasFilterLabel(filter)) 
	triggerCandidates.push_back(obj.p4());
    }
  }

  return triggerCandidates;
}

bool HLTEfficiency::onlineOfflineMatching(const edm::TriggerNames & triggerNames, edm::Handle<pat::TriggerObjectStandAloneCollection> triggerObjects, math::XYZTLorentzVector p4, std::string filterLabel, float dRmin=0.15) {

  for (pat::TriggerObjectStandAlone obj : *triggerObjects) { 
    obj.unpackPathNames(triggerNames); 
    if (obj.hasFilterLabel(filterLabel)) {
      //for (unsigned h = 0; h < obj.filterLabels().size(); ++h) std::cout << " " << obj.filterLabels()[h];  
      float dR = deltaR(p4, obj.p4());
      if (dR < dRmin)
	return true;
    }
  }

  return false;
}

void HLTEfficiency::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup) {

  edm::Handle<edm::TriggerResults> triggerBits;
  edm::Handle<pat::TriggerObjectStandAloneCollection> triggerObjects;
  edm::Handle<pat::PackedTriggerPrescales> triggerPrescales;
  edm::Handle<edm::View<flashgg::DiPhotonCandidate>> diphotons;
  edm::Handle<edm::View<flashgg::DiPhotonMVAResult> > mvaResults;
  edm::Handle<edm::View<flashgg::Electron>> eles;

  iEvent.getByToken(triggerBits_, triggerBits);
  iEvent.getByToken(triggerObjects_, triggerObjects);
  iEvent.getByToken(triggerPrescales_, triggerPrescales);
  iEvent.getByToken(diphotons_, diphotons);
  iEvent.getByToken(mvaResultToken_,mvaResults);
  iEvent.getByToken(eles_, eles);
  if (eles.failedToGet())
    return;

  if (!triggerBits.isValid()) {
    LogDebug("") << "TriggerResults product not found - returning result=false!";
    return;
  }

  // Apply event selection
  const edm::TriggerNames & triggerNames = iEvent.triggerNames(*triggerBits);
  if (triggerNamesID_ != triggerNames.parameterSetID()) {
    triggerNamesID_ = triggerNames.parameterSetID();
    init(*triggerBits, triggerNames);
  } 
  
  // require passing Ele T&P path
  //if (!hltEvent(triggerBits)) 
  //  return;

  //// FIXME SERVONO ????
  //// get p4 of tag and probe T&P trigger
  //std::vector< math::XYZTLorentzVector> triggerTags = hltP4(triggerNames, triggerObjects, tagFilterName_);
  //
  ////at least one offline electron matching tag trigger Ele 
  //std::vector<int> TAGind;
  //const edm::PtrVector<flashgg::Electron>& elePointers = eles->ptrVector();
  //for (size_t i=0; i<elePointers.size(); i++) {
  //
  //  edm::Ptr<flashgg::Electron> elePtr = elePointers[i];
  //
  //  for (unsigned int j=0; j<triggerTags.size(); j++){
  //    if (deltaR(triggerTags[i], elePtr->p4()));
  //    TAGind.push_back(i);
  //  }
  //}	
  //if (TAGind.size() == 0) 
  //  return;
  //tot_matchedEle++;

  // loop over diphoton pairs
  // check one matches to Ele and another matched to SC
  const edm::PtrVector<flashgg::DiPhotonMVAResult>& mvaResultPointers = mvaResults->ptrVector(); 
  const edm::PtrVector<flashgg::DiPhotonCandidate>& diphotonPointers = diphotons->ptrVector();
  assert(diphotonPointers.size() == mvaResultPointers.size()); // We are relying on corresponding sets - update this to give an error/exception

  int diphotonIndex =-1;
  bool isInverted = false;
  for (size_t i=0; i<diphotonPointers.size(); i++) {

    edm::Ptr<flashgg::DiPhotonCandidate> diphoPtr = diphotonPointers[i];
    edm::Ptr<flashgg::DiPhotonMVAResult> mvares = mvaResultPointers[i];

    if (mvares->mvaValue() < diphoMVACut_)
      continue;
    // FIXME add check same size filter collections
    for (size_t f=0; f<tagFilterName_.size(); f++) {
      bool leadMatchedToTag = onlineOfflineMatching(triggerNames, triggerObjects, diphoPtr->leadingPhoton()->p4(), tagFilterName_[f]);
      bool leadMatchedToProbe = onlineOfflineMatching(triggerNames, triggerObjects, diphoPtr->leadingPhoton()->p4(), probeFilterName_[f]);
      bool subLeadMatchedToTag = onlineOfflineMatching(triggerNames, triggerObjects, diphoPtr->subLeadingPhoton()->p4(), tagFilterName_[f]);
      bool subLeadMatchedToProbe = onlineOfflineMatching(triggerNames, triggerObjects, diphoPtr->subLeadingPhoton()->p4(), probeFilterName_[f]);
      
      if (leadMatchedToTag and subLeadMatchedToProbe) {
	diphotonIndex = i;
	break;
      }
      if (leadMatchedToProbe and subLeadMatchedToTag) {	
	diphotonIndex = i;
	isInverted = true;
	break;
      }
    }
  }
  
  if (diphotonIndex == -1)
    return;

  edm::Ptr<flashgg::DiPhotonCandidate> theDiPhoton = diphotonPointers[diphotonIndex];
  const flashgg::Photon* tag = theDiPhoton->leadingPhoton();
  const flashgg::Photon* probe = theDiPhoton->subLeadingPhoton();

  if (isInverted) {
    tag = theDiPhoton->subLeadingPhoton();
    probe = theDiPhoton->leadingPhoton();
  }

  if (theDiPhoton->mass() < 70 or theDiPhoton->mass() > 110)
    return;

  Zpeak->Fill(theDiPhoton->mass());
  float probe_weight = 1;
//  if (pho_r9[PROBE] < 0.8) probe_weight = weights_low[GetR9Bin_low(pho_r9[PROBE])];
//  else if (pho_r9[PROBE] < 1.2) probe_weight = weights_high[GetR9Bin_high(pho_r9[PROBE])];
//  if (probe_weight <=0) continue;

  edm::Handle<edm::View<l1extra::L1EmParticle>> l1iso;
  edm::Handle<edm::View<l1extra::L1EmParticle>> l1noniso;

  iEvent.getByToken(l1iso_, l1iso);
  iEvent.getByToken(l1noniso_, l1noniso);  
  
  //TAG is matched to L1 iso 20 object, is it absolute requirement ????
  if (not L1Matching(l1iso, tag->p4(), 20.))
    return;
  
  //TAG is matched to HLT seeded leg
  if (!onlineOfflineMatching(triggerNames, triggerObjects, tag->p4(), filterName_[0]) and
      !onlineOfflineMatching(triggerNames, triggerObjects, tag->p4(), filterName_[1]))
    return;
  
  //float probe_nvtx = vtx_std_n;
  TAG_L1_eta->Fill(probe->eta(), probe_weight); 
  TAG_L1_pt->Fill(probe->pt(), probe_weight); 
  //TAG_L1_nvtx->Fill(probe_nvtx, probe_weight);
  
  bool flag_L1_probe10=false;
  bool flag_L1_probe15=false;
  bool flag_L1_probe35=false;
  //=========L1===========
  //see if there's another L1 object > 22 or 15 ??? Controllare il Seed di L1
  if (L1Matching(l1iso, probe->p4(), 25)) {
    PROBE_L1_35_eta->Fill(probe->eta(), probe_weight); 
    PROBE_L1_35_pt->Fill(probe->pt(), probe_weight);
    //PROBE_L1_13_nvtx->Fill(probe_nvtx, probe_weight);
    flag_L1_probe35 = true;
  } 

  if (!flag_L1_probe35 and L1Matching(l1noniso, probe->p4(), 25)) {
    PROBE_L1_35_eta->Fill(probe->eta(), probe_weight); 
    PROBE_L1_35_pt->Fill(probe->pt(), probe_weight); 
    //PROBE_L1_13_nvtx->Fill(probe_nvtx, probe_weight);
    flag_L1_probe35 = true;
  }
  
  if (L1Matching(l1iso, probe->p4(), 15)) {
    PROBE_L1_15_eta->Fill(probe->eta(), probe_weight); 
    PROBE_L1_15_pt->Fill(probe->pt(), probe_weight);
    //PROBE_L1_13_nvtx->Fill(probe_nvtx, probe_weight);
    flag_L1_probe15 = true;
  } 
  
  if (!flag_L1_probe15 and L1Matching(l1noniso, probe->p4(), 15)) {
    PROBE_L1_15_eta->Fill(probe->eta(), probe_weight); 
    PROBE_L1_15_pt->Fill(probe->pt(), probe_weight); 
    //PROBE_L1_13_nvtx->Fill(probe_nvtx, probe_weight);
    flag_L1_probe15 = true;
  } 
  
  if (L1Matching(l1iso, probe->p4(), 10)) {
    PROBE_L1_10_eta->Fill(probe->eta(), probe_weight); 
    PROBE_L1_10_pt->Fill(probe->pt(), probe_weight);
    //PROBE_L1_13_nvtx->Fill(probe_nvtx, probe_weight);
    flag_L1_probe10 = true;
  } 
  
  if (!flag_L1_probe10 and L1Matching(l1noniso, probe->p4(), 10)) {
    PROBE_L1_10_eta->Fill(probe->eta(), probe_weight); 
    PROBE_L1_10_pt->Fill(probe->pt(), probe_weight); 
    //PROBE_L1_13_nvtx->Fill(probe_nvtx, probe_weight);
    flag_L1_probe10 = true;
  } 

  //=======HLT===========
  //PART 0: 42/22
  if (!flag_L1_probe10) 
  {
    TAG_HLT_eta_twosuite->Fill(probe->eta(), probe_weight); 
    TAG_HLT_pt_twosuite->Fill(probe->pt(), probe_weight); 
    //TAG_HLT_nvtx_twosuite->Fill(probe_nvtx, probe_weight);

    bool isoflag1 = 0; bool r9flag1 = 0;
    if (onlineOfflineMatching(triggerNames, triggerObjects, probe->p4(), filterName_[2])) {
      r9flag1 = 1; 
      PROBE_HLT_R9_eta_twosuite->Fill(probe->eta(), probe_weight); 
      PROBE_HLT_R9_pt_twosuite->Fill(probe->pt(), probe_weight); 
      //PROBE_HLT_R9_nvtx_twosuite->Fill(probe_nvtx, probe_weight);
    }
    if (onlineOfflineMatching(triggerNames, triggerObjects, probe->p4(), filterName_[3])) {
      isoflag1 = 1; 
      PROBE_HLT_Iso_eta_twosuite->Fill(probe->eta(), probe_weight); 
      PROBE_HLT_Iso_pt_twosuite->Fill(probe->pt(), probe_weight); 
      //PROBE_HLT_Iso_nvtx_twosuite->Fill(probe_nvtx, probe_weight);
    }
    
    if (!r9flag1) {
      if (onlineOfflineMatching(triggerNames, triggerObjects, probe->p4(), filterName_[1])) {
	r9flag1=1; 
	PROBE_HLT_R9_eta_twosuite->Fill(probe->eta(), probe_weight); 
	PROBE_HLT_R9_pt_twosuite->Fill(probe->pt(), probe_weight); 
	//PROBE_HLT_R9_nvtx_twosuite->Fill(probe_nvtx, probe_weight);
      }
    }

    if (!isoflag1) {
      if (onlineOfflineMatching(triggerNames, triggerObjects, probe->p4(), filterName_[0])) {
	isoflag1 = 1; 
	PROBE_HLT_Iso_eta_twosuite->Fill(probe->eta(), probe_weight); 
	PROBE_HLT_Iso_pt_twosuite->Fill(probe->pt(), probe_weight); 
	//PROBE_HLT_Iso_nvtx_twosuite->Fill(probe_nvtx, probe_weight);
      }
    }

    if (isoflag1 and r9flag1) {
      PROBE_HLT_OR_eta_twosuite->Fill(probe->eta(), probe_weight); 
      PROBE_HLT_OR_pt_twosuite->Fill(probe->pt(), probe_weight);
      //PROBE_HLT_OR_nvtx_twosuite->Fill(probe_nvtx, probe_weight); 
    }
  }
  
  //PART I: PROBE leg passed L1 EG 7, see if it matches to L1 non-seeded/seeded HLT object
  if (!flag_L1_probe10) 
    return;

  TAG_HLT_eta_unseeded->Fill(probe->eta(), probe_weight); 
  TAG_HLT_pt_unseeded->Fill(probe->pt(), probe_weight);
  //  TAG_HLT_nvtx_unseeded->Fill(probe_nvtx, probe_weight);

  bool isoflag = 0; bool r9flag = 0;
  if (onlineOfflineMatching(triggerNames, triggerObjects, probe->p4(), filterName_[2])) {
    r9flag = 1;
    PROBE_HLT_R9_eta_unseeded->Fill(probe->eta(), probe_weight); 
    PROBE_HLT_R9_pt_unseeded->Fill(probe->pt(), probe_weight); 
    //PROBE_HLT_R9_nvtx_unseeded->Fill(probe_nvtx, probe_weight);
  }

  if (onlineOfflineMatching(triggerNames, triggerObjects, probe->p4(), filterName_[3])) {
    isoflag = 1; 
    PROBE_HLT_Iso_eta_unseeded->Fill(probe->eta(), probe_weight); 
    PROBE_HLT_Iso_pt_unseeded->Fill(probe->pt(), probe_weight);
    //PROBE_HLT_Iso_nvtx_unseeded->Fill(probe_nvtx, probe_weight); 
  }
  
  if (!r9flag) {
    if (onlineOfflineMatching(triggerNames, triggerObjects, probe->p4(), filterName_[1])) {
      r9flag=1; 
      PROBE_HLT_R9_eta_unseeded->Fill(probe->eta(), probe_weight); 
      PROBE_HLT_R9_pt_unseeded->Fill(probe->pt(), probe_weight); 
      //PROBE_HLT_R9_nvtx_unseeded->Fill(probe_nvtx, probe_weight);
    }
  }
   
  if (!isoflag) {
    if (onlineOfflineMatching(triggerNames, triggerObjects, probe->p4(), filterName_[0])) {
      isoflag = 1; 
      PROBE_HLT_Iso_eta_unseeded->Fill(probe->eta(), probe_weight); 
      PROBE_HLT_Iso_pt_unseeded->Fill(probe->pt(), probe_weight); 
      //PROBE_HLT_Iso_nvtx_unseeded->Fill(probe_nvtx, probe_weight);
    }
  }
   
  if (isoflag or r9flag) {
    PROBE_HLT_OR_eta_unseeded->Fill(probe->eta(), probe_weight); 
    PROBE_HLT_OR_pt_unseeded->Fill(probe->pt(), probe_weight); 
    //PROBE_HLT_OR_nvtx_unseeded->Fill(probe_nvtx, probe_weight);
  }

  if (!flag_L1_probe15) 
    return;

  TAG_HLT_eta_seeded->Fill(probe->eta(), probe_weight); 
  TAG_HLT_pt_seeded->Fill(probe->pt(), probe_weight);
  //TAG_HLT_nvtx_seeded->Fill(probe_nvtx, probe_weight);

  bool tempflag = 0;
  if (onlineOfflineMatching(triggerNames, triggerObjects, probe->p4(), filterName_[1])) {
    tempflag=1; 
    PROBE_HLT_R9_eta_seeded->Fill(probe->eta(),probe_weight); 
    PROBE_HLT_R9_pt_seeded->Fill(probe->pt(),probe_weight); 
    //PROBE_HLT_R9_nvtx_seeded->Fill(probe_nvtx,probe_weight);
  }

  if (onlineOfflineMatching(triggerNames, triggerObjects, probe->p4(), filterName_[0])) {
    tempflag = 1; 
    PROBE_HLT_Iso_eta_seeded->Fill(probe->eta(),probe_weight); 
    PROBE_HLT_Iso_pt_seeded->Fill(probe->pt(),probe_weight); 
    //PROBE_HLT_Iso_nvtx_seeded->Fill(probe_nvtx,probe_weight);
  }

  if (tempflag) {
    PROBE_HLT_OR_eta_seeded->Fill(probe->eta(),probe_weight); 
    PROBE_HLT_OR_pt_seeded->Fill(probe->pt(),probe_weight); 
    //PROBE_HLT_OR_nvtx_seeded->Fill(probe_nvtx,probe_weight);
  }

  // 42 given 22
  if (!flag_L1_probe35) 
    return;

  TAG_HLT_eta_twosuite_high->Fill(probe->eta(), probe_weight); 
  TAG_HLT_pt_twosuite_high->Fill(probe->pt(), probe_weight);
  //TAG_HLT_nvtx_twosuite_high->Fill(probe_nvtx, probe_weight);

  bool tempflag1 = 0;
  if (onlineOfflineMatching(triggerNames, triggerObjects, probe->p4(), filterName_[1])) {
    tempflag1=1;
    PROBE_HLT_R9_eta_twosuite_high->Fill(probe->eta(), probe_weight);
    PROBE_HLT_R9_pt_twosuite_high->Fill(probe->pt(), probe_weight); 
    //PROBE_HLT_R9_nvtx_twosuite_high->Fill(probe_nvtx,probe_weight);
  }

  if (onlineOfflineMatching(triggerNames, triggerObjects, probe->p4(), filterName_[0])) {
    tempflag1 = 1;
    PROBE_HLT_Iso_eta_twosuite_high->Fill(probe->eta(), probe_weight); 
    PROBE_HLT_Iso_pt_twosuite_high->Fill(probe->pt(), probe_weight); 
    //PROBE_HLT_Iso_nvtx_twosuite_high->Fill(probe_nvtx,probe_weight);
  }

  if (tempflag1) {
    PROBE_HLT_OR_eta_twosuite_high->Fill(probe->eta(), probe_weight);
    PROBE_HLT_OR_pt_twosuite_high->Fill(probe->pt(), probe_weight);
    //PROBE_HLT_OR_nvtx_twosuite_high->Fill(probe_nvtx,probe_weight);
  }
}

DEFINE_FWK_MODULE(HLTEfficiency);
