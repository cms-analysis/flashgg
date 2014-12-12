#include "flashgg/TagFormats/interface/VHhadronicTag.h"

using namespace flashgg;

VHhadronicTag::VHhadronicTag() : DiPhotonTagBase::DiPhotonTagBase()
{}

VHhadronicTag::~VHhadronicTag() 
{}


VHhadronicTag::VHhadronicTag(edm::Ptr<DiPhotonCandidate> diPho, edm::Ptr<DiPhotonMVAResult> mvares) : DiPhotonTagBase::DiPhotonTagBase(diPho,mvares) {}

void VHhadronicTag::setJets( edm::Ptr<flashgg::Jet> ljet, edm::Ptr<flashgg::Jet> sjet ) 
{
  leadingJet_    = ljet;
  subleadingJet_ = sjet;
}
