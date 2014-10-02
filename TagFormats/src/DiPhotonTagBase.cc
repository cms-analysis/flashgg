#include "flashgg/MicroAODFormats/interface/DiPhotonCandidate.h"
#include "flashgg/TagFormats/interface/DiPhotonTagBase.h"
#include "CommonTools/CandUtils/interface/AddFourMomenta.h"

using namespace flashgg;

DiPhotonTagBase::DiPhotonTagBase(){}

DiPhotonTagBase::~DiPhotonTagBase(){}

DiPhotonTagBase::DiPhotonTagBase(edm::Ptr<flashgg::DiPhotonCandidate> diPho,edm::Ptr<flashgg::DiPhotonMVAResult> mvaRes) {
  addDaughter(*diPho);
  AddFourMomenta addP4;
  addP4.set(*this);
  mva_result_ = mvaRes;
}
const flashgg::DiPhotonCandidate* DiPhotonTagBase::diPhoton() const {
  return dynamic_cast<const flashgg::DiPhotonCandidate*> (daughter(0));
}
const edm::Ptr<flashgg::DiPhotonMVAResult> DiPhotonTagBase::diPhotonMVA() const {
  return mva_result_;
}
