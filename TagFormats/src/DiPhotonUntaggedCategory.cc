#include "flashgg/TagFormats/interface/DiPhotonUntaggedCategory.h"

using namespace flashgg;

DiPhotonUntaggedCategory::DiPhotonUntaggedCategory() : DiPhotonTagBase::DiPhotonTagBase() {}

DiPhotonUntaggedCategory::~DiPhotonUntaggedCategory() {}

DiPhotonUntaggedCategory::DiPhotonUntaggedCategory(edm::Ptr<flashgg::DiPhotonCandidate> diPho,edm::Ptr<DiPhotonMVAResult> mvaRes) 
	: DiPhotonUntaggedCategory::DiPhotonUntaggedCategory(diPho,*mvaRes) {}

DiPhotonUntaggedCategory::DiPhotonUntaggedCategory(edm::Ptr<DiPhotonCandidate> dipho,DiPhotonMVAResult mvares) :  DiPhotonTagBase::DiPhotonTagBase(dipho,mvares) {}

DiPhotonUntaggedCategory* DiPhotonUntaggedCategory::clone () const {
  DiPhotonUntaggedCategory* result = new DiPhotonUntaggedCategory(diPhoton(),diPhotonMVA());
  result->setCategoryNumber(getCategoryNumber());
  result->setDiPhotonIndex(getDiPhotonIndex());
  return result;
}
