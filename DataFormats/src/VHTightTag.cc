#include "flashgg/DataFormats/interface/VHTightTag.h"
#include <algorithm>

using namespace flashgg;

VHTightTag::VHTightTag() : DiPhotonTagBase::DiPhotonTagBase()
{}

VHTightTag::~VHTightTag() 
{}


VHTightTag::VHTightTag(edm::Ptr<DiPhotonCandidate> diPho, edm::Ptr<DiPhotonMVAResult> mvares) : DiPhotonTagBase::DiPhotonTagBase(diPho,*mvares) {}
VHTightTag::VHTightTag(edm::Ptr<DiPhotonCandidate> dipho,DiPhotonMVAResult mvares) : DiPhotonTagBase::DiPhotonTagBase(dipho,mvares) {}

VHTightTag* VHTightTag::clone () const {
  VHTightTag* result = new VHTightTag(diPhoton(),diPhotonMVA());
  result->setDiPhotonIndex(diPhotonIndex());
  result->setJets(Jets_);
  result->setMuons(Muons_);
  result->setElectrons(Electrons_);
  result->setMET(MET_);

return result;
}


