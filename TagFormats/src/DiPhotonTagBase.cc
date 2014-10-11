#include "flashgg/MicroAODFormats/interface/DiPhotonCandidate.h"
#include "flashgg/TagFormats/interface/DiPhotonTagBase.h"
#include "CommonTools/CandUtils/interface/AddFourMomenta.h"

using namespace flashgg;

DiPhotonTagBase::DiPhotonTagBase(){}

DiPhotonTagBase::~DiPhotonTagBase(){}

DiPhotonTagBase::DiPhotonTagBase(edm::Ptr<flashgg::DiPhotonCandidate> diPho,edm::Ptr<DiPhotonMVAResult> mvaRes) {
  addDaughter(*diPho);
  AddFourMomenta addP4;
  addP4.set(*this);
  mva_result_ = *mvaRes; // copies 
}
const DiPhotonCandidate* DiPhotonTagBase::diPhoton() const {
  return dynamic_cast<const flashgg::DiPhotonCandidate*> (daughter(0));
}
const DiPhotonMVAResult DiPhotonTagBase::diPhotonMVA() const {
  return mva_result_;
}
