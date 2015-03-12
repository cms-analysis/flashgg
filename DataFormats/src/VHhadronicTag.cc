#include "flashgg/DataFormats/interface/VHHadronicTag.h"

using namespace flashgg;

VHHadronicTag::VHHadronicTag() : DiPhotonTagBase::DiPhotonTagBase()
{}

VHHadronicTag::~VHHadronicTag() 
{}


VHHadronicTag::VHHadronicTag(edm::Ptr<DiPhotonCandidate> diPho, DiPhotonMVAResult mvares) : DiPhotonTagBase::DiPhotonTagBase(diPho,mvares) {}

VHHadronicTag::VHHadronicTag(edm::Ptr<DiPhotonCandidate> diPho, edm::Ptr<DiPhotonMVAResult> mvares) : VHHadronicTag::VHHadronicTag(diPho,*mvares) {}


void VHHadronicTag::setJets( edm::Ptr<flashgg::Jet> ljet, edm::Ptr<flashgg::Jet> sjet ) 
{
  leadingJet_    = ljet;
  subleadingJet_ = sjet;
}


VHHadronicTag* VHHadronicTag::clone () const {
  VHHadronicTag* result = new VHHadronicTag(diPhoton(),diPhotonMVA());
  result->setCategoryNumber(categoryNumber());
  result->setDiPhotonIndex(diPhotonIndex());
  result->setJets(leadingJet_, subleadingJet_);
  return result;
}

