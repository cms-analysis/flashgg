#include "flashgg/MicroAODFormats/interface/DiPhotonCandidate.h"
#include "flashgg/TagFormats/interface/VBFTag.h"
#include "CommonTools/CandUtils/interface/AddFourMomenta.h"

using namespace flashgg;

VBFTag::VBFTag(){}

VBFTag::~VBFTag(){}

VBFTag::VBFTag(edm::Ptr<flashgg::DiPhotonCandidate> diPho,edm::Ptr<DiPhotonMVAResult> mvaRes,edm::Ptr<VBFDiPhoDiJetMVAResult> vbfDiPhoDiJet_mvaRes) : 
  VBFTag::VBFTag(diPho,*mvaRes,*vbfDiPhoDiJet_mvaRes) {}

VBFTag::VBFTag(edm::Ptr<DiPhotonCandidate> dipho,DiPhotonMVAResult mvares,VBFDiPhoDiJetMVAResult vbfDiPhoDiJet_mvaRes) :
  DiPhotonTagBase::DiPhotonTagBase(dipho,mvares) {}

const VBFDiPhoDiJetMVAResult VBFTag::VBFDiPhoDiJetMVA() const {
  return vbfDiPhoDiJet_mva_result_;
	}
const VBFMVAResult VBFTag::VBFMVA() const {
  return vbfDiPhoDiJet_mva_result_.vbfMvaResult;
	}

const Jet VBFTag::leadingJet() const{
	return vbfDiPhoDiJet_mva_result_.vbfMvaResult.leadJet;
}
		const Jet VBFTag::subLeadingJet() const{
return vbfDiPhoDiJet_mva_result_.vbfMvaResult.subleadJet;
}

VBFTag* VBFTag::clone () const {
  VBFTag* result = new VBFTag(diPhoton(),diPhotonMVA(),vbfDiPhoDiJet_mva_result_);
  result->setCategoryNumber(getCategoryNumber());
  result->setDiPhotonIndex(getDiPhotonIndex());
  return result;
}


