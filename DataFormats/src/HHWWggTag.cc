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

// void HHWWggTag::GetObjects(edm::Ptr<DiPhotonCandidate> dipho)
// {
//   // Save as dumper objects 
//   Leading_Photon_ = dipho->leadingPhoton();
//   Subleading_Photon_ = dipho->subLeadingPhoton();

//   lp_Hgg_MVA_ = Leading_Photon_->phoIdMvaDWrtVtx( dipho->vtx() ); 
//   slp_Hgg_MVA_ = Subleading_Photon_->phoIdMvaDWrtVtx( dipho->vtx() ); 
// }

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

//-- With no cut flow analysis and b tag variables 

//- 2 jets, electron 
HHWWggTag::HHWWggTag(edm::Ptr<DiPhotonCandidate> dipho, edm::Ptr<flashgg::Electron> electron, edm::Ptr<flashgg::Met> MET,
                      edm::Ptr<flashgg::Jet> jet1, edm::Ptr<flashgg::Jet> jet2)
{
  dipho_ = dipho;
  GetObjects(dipho);
}

//- two jets, muon 
HHWWggTag::HHWWggTag(edm::Ptr<DiPhotonCandidate> dipho, edm::Ptr<flashgg::Muon> muon, edm::Ptr<flashgg::Met> MET,
                      edm::Ptr<flashgg::Jet> jet1, edm::Ptr<flashgg::Jet> jet2)
{
  dipho_ = dipho;
  GetObjects(dipho);
}

//-- With cut flow analysis and b tag variables 

//- Two jets, one electron 
HHWWggTag::HHWWggTag(edm::Ptr<DiPhotonCandidate> dipho, edm::Ptr<flashgg::Electron> electron, edm::Ptr<flashgg::Met> MET,
                      edm::Ptr<flashgg::Jet> jet1, edm::Ptr<flashgg::Jet> jet2, std::vector<double> Cut_Variables) : Cut_Variables_(Cut_Variables)
{
  dipho_ = dipho;
  GetObjects(dipho, electron, MET, jet1, jet2);
}

//- Two jets, one muon 
HHWWggTag::HHWWggTag(edm::Ptr<DiPhotonCandidate> dipho, edm::Ptr<flashgg::Muon> muon, edm::Ptr<flashgg::Met> MET,
                      edm::Ptr<flashgg::Jet> jet1, edm::Ptr<flashgg::Jet> jet2, std::vector<double> Cut_Variables) : Cut_Variables_(Cut_Variables)
{
  dipho_ = dipho;
  GetObjects(dipho, muon, MET, jet1, jet2);
}

//-- Untagged
HHWWggTag::HHWWggTag(edm::Ptr<DiPhotonCandidate> dipho, edm::Ptr<flashgg::Met> MET, std::vector<double> Cut_Variables) : Cut_Variables_(Cut_Variables)
{
  dipho_ = dipho;
  GetObjects(dipho, MET);
}

// You need this because HHWWggTag is derived from another class 
HHWWggTag *HHWWggTag::clone() const
{
    HHWWggTag *result = new HHWWggTag( *this );
    return result;
}


