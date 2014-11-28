#include "flashgg/MicroAODFormats/interface/DiPhotonCandidate.h"
#include "flashgg/TagFormats/interface/DiPhotonTagBase.h"
#include "CommonTools/CandUtils/interface/AddFourMomenta.h"

using namespace flashgg;

DiPhotonTagBase::DiPhotonTagBase(){
  category_number_ = -1;
}

DiPhotonTagBase::DiPhotonTagBase(edm::Ptr<flashgg::DiPhotonCandidate> diPho,edm::Ptr<DiPhotonMVAResult> mvaRes) : DiPhotonTagBase::DiPhotonTagBase(diPho,*mvaRes) {}

DiPhotonTagBase::DiPhotonTagBase(edm::Ptr<flashgg::DiPhotonCandidate> diPho,DiPhotonMVAResult mvaRes) {
  mva_result_ = mvaRes;
  category_number_ = -1;
  dipho_ = diPho;
}

DiPhotonTagBase* DiPhotonTagBase::clone () const {
  DiPhotonTagBase* result = new DiPhotonTagBase(dipho_,mva_result_);
  result->setCategoryNumber(category_number_);
  result->setDiPhotonIndex(diPhotonIndex_);
  return result;
}

bool DiPhotonTagBase::operator <(const DiPhotonTagBase & b) const {
  // For choosing which of two tags OF THE SAME TYPE are of higher priority
  // Comparison of different tags not currently supported - is it ever needed?
  // Overloading may be appropriate if different tags have different priorities

  if (getCategoryNumber() == b.getCategoryNumber()) {
    return (getSumPt() < b.getSumPt()); 
  } else {
    return (getCategoryNumber() < b.getCategoryNumber());
  }
}
