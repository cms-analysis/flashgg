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
	theJetVec_ = std::vector<edm::Ptr<flashgg::Jet> >(theJetVec);
	theBJetVec_ = std::vector<edm::Ptr<flashgg::Jet> >(theBJetVec);
}

TTHhadronicTag* TTHhadronicTag::clone () const {
	TTHhadronicTag* result = new TTHhadronicTag(diPhoton(),diPhotonMVA(),theJetVec_,theBJetVec_);
	result->setDiPhotonIndex(getDiPhotonIndex());
	return result;
}





