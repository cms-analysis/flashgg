#include "flashgg/TagFormats/interface/VHhadronicTag.h"

using namespace flashgg;

VHhadronicTag::VHhadronicTag() : DiPhotonTagBase::DiPhotonTagBase()
{}

VHhadronicTag::~VHhadronicTag() 
{}


VHhadronicTag::VHhadronicTag(edm::Ptr<DiPhotonCandidate> diPho, DiPhotonMVAResult mvares) : DiPhotonTagBase::DiPhotonTagBase(diPho,mvares) {}

VHhadronicTag::VHhadronicTag(edm::Ptr<DiPhotonCandidate> diPho, edm::Ptr<DiPhotonMVAResult> mvares) : VHhadronicTag::VHhadronicTag(diPho,*mvares) {}


void VHhadronicTag::setJets( edm::Ptr<flashgg::Jet> ljet, edm::Ptr<flashgg::Jet> sjet ) 
{
  leadingJet_    = ljet;
  subleadingJet_ = sjet;
}


VHhadronicTag* VHhadronicTag::clone () const {
  VHhadronicTag* result = new VHhadronicTag(diPhoton(),diPhotonMVA());
  result->setCategoryNumber(getCategoryNumber());
  result->setDiPhotonIndex(getDiPhotonIndex());
  result->setJets(leadingJet_, subleadingJet_);
  return result;
}

