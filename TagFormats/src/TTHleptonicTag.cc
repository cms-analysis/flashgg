#include "flashgg/TagFormats/interface/TTHleptonicTag.h"
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

    //  std::copy(Jets_.begin(),Jets_.end(),result->Jets_.begin());
    //  std::copy(BJets_.begin(),BJets_.end(),result->BJets_.begin());
    //  std::copy(Muons_.begin(),Muons_.end(),result->Muons_.begin());


  return result;
}
