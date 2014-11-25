#include "flashgg/TagFormats/interface/VHlooseTag.h"
#include <algorithm>

using namespace flashgg;

VHlooseTag::VHlooseTag() : DiPhotonTagBase::DiPhotonTagBase()
{}

VHlooseTag::~VHlooseTag() 
{}


VHlooseTag::VHlooseTag(edm::Ptr<DiPhotonCandidate> diPho, edm::Ptr<DiPhotonMVAResult> mvares) : DiPhotonTagBase::DiPhotonTagBase(diPho,*mvares) {}
VHlooseTag::VHlooseTag(edm::Ptr<DiPhotonCandidate> dipho,DiPhotonMVAResult mvares) : DiPhotonTagBase::DiPhotonTagBase(dipho,mvares) {}

VHlooseTag* VHlooseTag::clone () const {
  VHlooseTag* result = new VHlooseTag(diPhoton(),diPhotonMVA());
  result->setDiPhotonIndex(getDiPhotonIndex());
  result->setJets(Jets_);
  result->setMuons(Muons_);
  result->setMET(MET_);

return result;
}
