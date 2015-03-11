#include "flashgg/DataFormats/interface/TTHleptonicTag.h"
#include <algorithm>

using namespace flashgg;

TTHleptonicTag::TTHleptonicTag() : DiPhotonTagBase::DiPhotonTagBase()
{}

TTHleptonicTag::~TTHleptonicTag() 
{}

// N.B. Other attributes are set using methods in header file
TTHleptonicTag::TTHleptonicTag(edm::Ptr<DiPhotonCandidate> diPho, edm::Ptr<DiPhotonMVAResult> mvares) : TTHleptonicTag::TTHleptonicTag(diPho,*mvares) {}
TTHleptonicTag::TTHleptonicTag(edm::Ptr<DiPhotonCandidate> dipho,DiPhotonMVAResult mvares) : DiPhotonTagBase::DiPhotonTagBase(dipho,mvares) {}


TTHleptonicTag* TTHleptonicTag::clone () const {
  TTHleptonicTag* result = new TTHleptonicTag(diPhoton(),diPhotonMVA());
  result->setDiPhotonIndex(getDiPhotonIndex());
  result->setJets(Jets_);
  result->setBJets(BJets_);
  result->setMuons(Muons_);
  result->setElectrons(Electrons_);

  return result;
}
