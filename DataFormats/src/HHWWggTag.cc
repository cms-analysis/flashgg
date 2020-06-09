// include path /cvmfs/cms.cern.ch/slc7_amd64_gcc700/cms/cmssw/CMSSW_10_5_0/src/
#include "TLorentzVector.h"
#include "DataFormats/Math/interface/LorentzVector.h"
#include "flashgg/DataFormats/interface/Photon.h"
#include "flashgg/DataFormats/interface/Electron.h"
#include "flashgg/DataFormats/interface/Muon.h"
#include "flashgg/DataFormats/interface/DiPhotonCandidate.h"
#include "flashgg/DataFormats/interface/HHWWggTag.h"
#include "flashgg/DataFormats/interface/Met.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "flashgg/DataFormats/interface/Jet.h"
#include "DataFormats/Math/interface/deltaR.h"

using namespace flashgg; // makes flashgg sub members visible 
// HHWWggTag::HHWWggTag() : DiPhotonTagBase::DiPhotonTagBase(), mva_(-2.), JetVector_ (), Cut_Variables_ ()
HHWWggTag::HHWWggTag() : DiPhotonTagBase::DiPhotonTagBase(), mva_(-2.), Cut_Variables_ ()
{

}

HHWWggTag::~HHWWggTag() {}

void HHWWggTag::GetObjects(edm::Ptr<DiPhotonCandidate> dipho)
{
  // Save as dumper objects 
  Leading_Photon_ = dipho->leadingPhoton();
  Subleading_Photon_ = dipho->subLeadingPhoton();

  lp_Hgg_MVA_ = Leading_Photon_->phoIdMvaDWrtVtx( dipho->vtx() ); 
  slp_Hgg_MVA_ = Subleading_Photon_->phoIdMvaDWrtVtx( dipho->vtx() ); 
}

void HHWWggTag::GetObjects(edm::Ptr<DiPhotonCandidate> dipho, edm::Ptr<flashgg::Met> MET)
{
  // Save as dumper objects 
  Leading_Photon_ = dipho->leadingPhoton();
  Subleading_Photon_ = dipho->subLeadingPhoton();
  MET_ = *MET;

  lp_Hgg_MVA_ = Leading_Photon_->phoIdMvaDWrtVtx( dipho->vtx() ); 
  slp_Hgg_MVA_ = Subleading_Photon_->phoIdMvaDWrtVtx( dipho->vtx() ); 
}

void HHWWggTag::GetObjects(edm::Ptr<DiPhotonCandidate> dipho, edm::Ptr<flashgg::Electron> electron, edm::Ptr<flashgg::Met> MET, edm::Ptr<flashgg::Jet> jet1, edm::Ptr<flashgg::Jet> jet2)
{
  // Save as dumper objects 
  Leading_Photon_ = dipho->leadingPhoton();
  Subleading_Photon_ = dipho->subLeadingPhoton();
  Electron_ = *electron;
  MET_ = *MET;
  Leading_Jet_ = *jet1;
  Subleading_Jet_ = *jet2;

  lp_Hgg_MVA_ = Leading_Photon_->phoIdMvaDWrtVtx( dipho->vtx() ); 
  slp_Hgg_MVA_ = Subleading_Photon_->phoIdMvaDWrtVtx( dipho->vtx() ); 
}

void HHWWggTag::GetObjects(edm::Ptr<DiPhotonCandidate> dipho, edm::Ptr<flashgg::Muon> muon, edm::Ptr<flashgg::Met> MET, edm::Ptr<flashgg::Jet> jet1, edm::Ptr<flashgg::Jet> jet2)
{
  // Save as dumper objects 
  Leading_Photon_ = dipho->leadingPhoton();
  Subleading_Photon_ = dipho->subLeadingPhoton();
  Muon_ = *muon;
  MET_ = *MET;
  Leading_Jet_ = *jet1;
  Subleading_Jet_ = *jet2;  

  lp_Hgg_MVA_ = Leading_Photon_->phoIdMvaDWrtVtx( dipho->vtx() ); 
  slp_Hgg_MVA_ = Subleading_Photon_->phoIdMvaDWrtVtx( dipho->vtx() ); 
}

std::vector<flashgg::Electron> HHWWggTag::GetElectrons(std::vector<edm::Ptr<flashgg::Electron>> electrons)
{
  std::vector<flashgg::Electron> savedElectrons;
  for (unsigned int i = 0; i < electrons.size(); i++){
    auto electron = electrons[i]; 
    flashgg::Electron * thisElectronPointer = const_cast<flashgg::Electron *>(electron.get());
    savedElectrons.push_back(*thisElectronPointer);
  }

  return savedElectrons;
}

std::vector<flashgg::Muon> HHWWggTag::GetMuons(std::vector<edm::Ptr<flashgg::Muon>> muons)
{
  std::vector<flashgg::Muon> savedMuons;
  for (unsigned int i = 0; i < muons.size(); i++){
    auto muon = muons[i]; 
    flashgg::Muon * thisMuonPointer = const_cast<flashgg::Muon *>(muon.get());
    savedMuons.push_back(*thisMuonPointer);
  }

  return savedMuons;  
}

std::vector<flashgg::Jet> HHWWggTag::GetJets(std::vector<edm::Ptr<flashgg::Jet>> jets)
{
  std::vector<flashgg::Jet> savedJets;
  for (unsigned int i = 0; i < jets.size(); i++){
    auto jet = jets[i]; 
    flashgg::Jet * thisJetPointer = const_cast<flashgg::Jet *>(jet.get());
    savedJets.push_back(*thisJetPointer);
  }

  return savedJets;    
}

// HHWWggTag_0 
HHWWggTag::HHWWggTag(edm::Ptr<DiPhotonCandidate> dipho, edm::Ptr<flashgg::Electron> electron, edm::Ptr<flashgg::Met> MET,
                      edm::Ptr<flashgg::Jet> jet1, edm::Ptr<flashgg::Jet> jet2)
{
  dipho_ = dipho;
  GetObjects(dipho);
}

// HHWWggTag_1
HHWWggTag::HHWWggTag(edm::Ptr<DiPhotonCandidate> dipho, edm::Ptr<flashgg::Muon> muon, edm::Ptr<flashgg::Met> MET,
                      edm::Ptr<flashgg::Jet> jet1, edm::Ptr<flashgg::Jet> jet2)
{
  dipho_ = dipho;
  GetObjects(dipho);
}

// HHWWggTag_0 with cutFlowAnalysis
HHWWggTag::HHWWggTag(edm::Ptr<DiPhotonCandidate> dipho, edm::Ptr<flashgg::Electron> electron, std::vector<edm::Ptr<flashgg::Electron>> allElectrons, std::vector<edm::Ptr<flashgg::Electron>> goodElectrons,
                     std::vector<edm::Ptr<flashgg::Muon>> allMuons,
                     edm::Ptr<flashgg::Met> MET, edm::Ptr<flashgg::Jet> jet1, edm::Ptr<flashgg::Jet> jet2, 
                     std::vector<edm::Ptr<flashgg::Jet>> allJets, std::vector<edm::Ptr<flashgg::Jet>> goodJets, std::vector<double> Cut_Variables) : Cut_Variables_(Cut_Variables)
{
  dipho_ = dipho;
  GetObjects(dipho, electron, MET, jet1, jet2);
  // GetObjects(dipho, electron, allElectrons, goodElectrons, MET, jet1, jet2);
  allElectrons_ = GetElectrons(allElectrons);
  goodElectrons_ = GetElectrons(goodElectrons);
  allMuons_ = GetMuons(allMuons);
  // goodMuons_ = GetMuons(goodMuons);
  allJets_ = GetJets(allJets);
  goodJets_ = GetJets(goodJets);    
}

// HHWWggTag_1 with cutFlowAnalysis
HHWWggTag::HHWWggTag(edm::Ptr<DiPhotonCandidate> dipho, std::vector<edm::Ptr<flashgg::Electron>> allElectrons, edm::Ptr<flashgg::Muon> muon, std::vector<edm::Ptr<flashgg::Muon>> allMuons, std::vector<edm::Ptr<flashgg::Muon>> goodMuons,
              edm::Ptr<flashgg::Met> MET, edm::Ptr<flashgg::Jet> jet1, edm::Ptr<flashgg::Jet> jet2, 
              std::vector<edm::Ptr<flashgg::Jet>> allJets, std::vector<edm::Ptr<flashgg::Jet>> goodJets, std::vector<double> Cut_Variables) : Cut_Variables_(Cut_Variables)
{
  dipho_ = dipho;
  GetObjects(dipho, muon, MET, jet1, jet2);
  allElectrons_ = GetElectrons(allElectrons);
  // goodElectrons_ = GetElectrons(goodElectrons);
  allMuons_ = GetMuons(allMuons);
  goodMuons_ = GetMuons(goodMuons);
  allJets_ = GetJets(allJets);
  goodJets_ = GetJets(goodJets);    
}

//-- Untagged with cutFlowAnalysis 
HHWWggTag::HHWWggTag(edm::Ptr<DiPhotonCandidate> dipho, 
                     std::vector<edm::Ptr<flashgg::Electron>> allElectrons, std::vector<edm::Ptr<flashgg::Electron>> goodElectrons,
                     std::vector<edm::Ptr<flashgg::Muon>> allMuons, std::vector<edm::Ptr<flashgg::Muon>> goodMuons,
                     edm::Ptr<flashgg::Met> MET,
                     std::vector<edm::Ptr<flashgg::Jet>> allJets, std::vector<edm::Ptr<flashgg::Jet>> goodJets,
                     std::vector<double> Cut_Variables) : Cut_Variables_(Cut_Variables)
{
  dipho_ = dipho;
  GetObjects(dipho, MET);
  allElectrons_ = GetElectrons(allElectrons);
  goodElectrons_ = GetElectrons(goodElectrons);
  allMuons_ = GetMuons(allMuons);
  goodMuons_ = GetMuons(goodMuons);
  allJets_ = GetJets(allJets);
  goodJets_ = GetJets(goodJets);    
}

// You need this because HHWWggTag is derived from another class 
HHWWggTag *HHWWggTag::clone() const
{
    HHWWggTag *result = new HHWWggTag( *this );
    return result;
}


