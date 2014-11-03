#include "flashgg/TagFormats/interface/DiPhotonUntaggedCategory.h"

using namespace flashgg;

DiPhotonUntaggedCategory::DiPhotonUntaggedCategory() : DiPhotonTagBase::DiPhotonTagBase() {}

DiPhotonUntaggedCategory::DiPhotonUntaggedCategory(edm::Ptr<DiPhotonCandidate> dipho,edm::Ptr<DiPhotonMVAResult> mvares) :  DiPhotonTagBase::DiPhotonTagBase(dipho,mvares) {
  std::cout << " In DiPhotonUntaggedCategory::DiPhotonUntaggedCategory and my numberOfDaughters() is " << numberOfDaughters() << std::endl;
  const flashgg::DiPhotonCandidate* test = dynamic_cast<const flashgg::DiPhotonCandidate*> (daughter(0));
  std::cout << " test = " << test << std::endl;
  if (test != NULL) std::cout << " test->getSumPt() = " << test->getSumPt() << std::endl;
}

//DiPhotonUntaggedCategory::DiPhotonUntaggedCategory(edm::Ptr<DiPhotonCandidate> dipho,edm::Ptr<DiPhotonMVAResult> mvares, int catnum) :  DiPhotonTagBase::DiPhotonTagBase(dipho,mvares) {category_number_ = catnum;}
