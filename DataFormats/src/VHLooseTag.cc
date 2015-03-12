#include "flashgg/DataFormats/interface/VHLooseTag.h"
#include <algorithm>

using namespace flashgg;

VHLooseTag::VHLooseTag() : DiPhotonTagBase::DiPhotonTagBase()
{}

VHLooseTag::~VHLooseTag() 
{}


VHLooseTag::VHLooseTag(edm::Ptr<DiPhotonCandidate> diPho, edm::Ptr<DiPhotonMVAResult> mvares) : DiPhotonTagBase::DiPhotonTagBase(diPho,*mvares) {}
VHLooseTag::VHLooseTag(edm::Ptr<DiPhotonCandidate> dipho,DiPhotonMVAResult mvares) : DiPhotonTagBase::DiPhotonTagBase(dipho,mvares) {}

VHLooseTag* VHLooseTag::clone () const {
  VHLooseTag* result = new VHLooseTag(diPhoton(),diPhotonMVA());
  result->setDiPhotonIndex(diPhotonIndex());
  result->setJets(Jets_);
  result->setMuons(Muons_);
  result->setElectrons(Electrons_);
  result->setMET(MET_);

return result;
}
