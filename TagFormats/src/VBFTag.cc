#include "flashgg/MicroAODFormats/interface/DiPhotonCandidate.h"
#include "flashgg/TagFormats/interface/VBFTag.h"
#include "CommonTools/CandUtils/interface/AddFourMomenta.h"

using namespace flashgg;

VBFTag::VBFTag(){}

//VBFTag::VBFTag() : DiPhotonTagBase::DiPhotonTagBase() {}
//VBFTag::~VBFTag(){}

VBFTag::VBFTag(const VBFTag& x) : DiPhotonTagBase::DiPhotonTagBase(x) {
  category_number_ = x.category_number_;  
}

VBFTag::VBFTag(edm::Ptr<DiPhotonCandidate> diPho,edm::Ptr<VBFDiPhoDiJetMVAResult> vbfDiPhoDiJet_mvaRes) {
  addDaughter(*diPho);
  AddFourMomenta addP4;
  addP4.set(*this);
  vbfDiPhoDiJet_mva_result_ = *vbfDiPhoDiJet_mvaRes; // copies 
}

//VBFTag::VBFTag(edm::Ptr<DiPhotonCandidate> dipho,edm::Ptr<DiPhotonMVAResult> mvares) :  DiPhotonTagBase::DiPhotonTagBase(dipho,mvares) {}

//VBFTag::VBFTag(edm::Ptr<DiPhotonCandidate> dipho,edm::Ptr<DiPhotonMVAResult> mvares, int catnum) :  DiPhotonTagBase::DiPhotonTagBase(dipho,mvares) {category_number_ = catnum;}



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


