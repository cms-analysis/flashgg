#include "flashgg/TagFormats/interface/TTHhadronicTag.h"
#include "flashgg/MicroAODFormats/interface/Jet.h"

using namespace flashgg;

TTHhadronicTag::TTHhadronicTag() : DiPhotonTagBase::DiPhotonTagBase() {}

TTHhadronicTag::~TTHhadronicTag() {}

TTHhadronicTag::TTHhadronicTag(edm::Ptr<flashgg::DiPhotonCandidate> diPho,edm::Ptr<DiPhotonMVAResult> mvaRes,
			       std::vector<edm::Ptr<flashgg::Jet> > theJetVec , std::vector<edm::Ptr<flashgg::Jet> > theBJetVec) :
	TTHhadronicTag::TTHhadronicTag(diPho,*mvaRes,theJetVec,theBJetVec) {}

TTHhadronicTag::TTHhadronicTag(edm::Ptr<DiPhotonCandidate> dipho,DiPhotonMVAResult mvares,
                               std::vector<edm::Ptr<flashgg::Jet> > theJetVec , std::vector<edm::Ptr<flashgg::Jet> > theBJetVec) :
	DiPhotonTagBase::DiPhotonTagBase(dipho,mvares) {
  std::cout << "TTHhadronicTag constructor: " << (diPhoton()->getSumPt()) << " " << diPhotonMVA().result << std::endl;
	theJetVec_ = std::vector<edm::Ptr<flashgg::Jet> >(theJetVec);
	theBJetVec_ = std::vector<edm::Ptr<flashgg::Jet> >(theBJetVec);
}

TTHhadronicTag* TTHhadronicTag::clone () const {
  std::cout << " Cloning TTHhadronicTag" << std::endl;
	TTHhadronicTag* result = new TTHhadronicTag(diPhoton(),diPhotonMVA(),theJetVec_,theBJetVec_);
	std::cout << (diPhoton()->getSumPt()) << " " << (result->diPhoton()->getSumPt()) << std::endl;
	std::cout << diPhotonMVA().result << " " << result->diPhotonMVA().result << std::endl;
	std::cout << theJetVec_.size() << " " << result->theJetVec_.size() << std::endl;
	std::cout << theBJetVec_.size() << " " << result->theBJetVec_.size() << std::endl;
	result->setDiPhotonIndex(getDiPhotonIndex());
	std::cout << getDiPhotonIndex() << " " << result->getDiPhotonIndex() << std::endl;
	return result;
}





