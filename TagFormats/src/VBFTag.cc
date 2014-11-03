#include "flashgg/MicroAODFormats/interface/DiPhotonCandidate.h"
#include "flashgg/TagFormats/interface/VBFTag.h"
#include "CommonTools/CandUtils/interface/AddFourMomenta.h"

using namespace flashgg;

VBFTag::VBFTag(){}

//VBFTag::VBFTag() : DiPhotonTagBase::DiPhotonTagBase() {}
//VBFTag::~VBFTag(){}

//VBFTag::VBFTag(const VBFTag& x) : DiPhotonTagBase::DiPhotonTagBase(x) {
 // category_number_ = x.category_number_; 
//}

VBFTag::VBFTag(edm::Ptr<DiPhotonCandidate> dipho,edm::Ptr<DiPhotonMVAResult> mvares,edm::Ptr<VBFDiPhoDiJetMVAResult> vbfDiPhoDiJet_mvaRes) :
  DiPhotonTagBase::DiPhotonTagBase(dipho,mvares) {
  std::cout << " In VBFTag::VBFTag, and my numberOfDaughters() is " << numberOfDaughters() << std::endl;
  const flashgg::DiPhotonCandidate* test = dynamic_cast<const flashgg::DiPhotonCandidate*> (daughter(0));
  std::cout << " test = " << test << std::endl;
  if (test != NULL) std::cout << " test->getSumPt() = " << test->getSumPt() << std::endl;
  vbfDiPhoDiJet_mva_result_ = *vbfDiPhoDiJet_mvaRes; // copies 
}


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


