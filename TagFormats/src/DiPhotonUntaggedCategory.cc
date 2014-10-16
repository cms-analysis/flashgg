#include "flashgg/TagFormats/interface/DiPhotonUntaggedCategory.h"

using namespace flashgg;

DiPhotonUntaggedCategory::DiPhotonUntaggedCategory() : DiPhotonTagBase::DiPhotonTagBase() {}
//DiPhotonUntaggedCategory::~DiPhotonUntaggedCategory() : DiPhotonTagBase::~DiPhotonTagBase() {}

DiPhotonUntaggedCategory::DiPhotonUntaggedCategory(const DiPhotonUntaggedCategory& x) : DiPhotonTagBase::DiPhotonTagBase(x) {
  category_number_ = x.category_number_;  
}

DiPhotonUntaggedCategory::DiPhotonUntaggedCategory(edm::Ptr<DiPhotonCandidate> dipho,edm::Ptr<DiPhotonMVAResult> mvares) :  DiPhotonTagBase::DiPhotonTagBase(dipho,mvares) { dipho_mva_res_ = *mvares;}
DiPhotonUntaggedCategory::DiPhotonUntaggedCategory(edm::Ptr<DiPhotonCandidate> dipho,edm::Ptr<DiPhotonMVAResult> mvares, int catnum) :  DiPhotonTagBase::DiPhotonTagBase(dipho,mvares) {category_number_ = catnum;}
