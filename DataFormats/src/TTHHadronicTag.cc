#include "flashgg/DataFormats/interface/TTHHadronicTag.h"
#include "flashgg/DataFormats/interface/Jet.h"

using namespace flashgg;

TTHHadronicTag::TTHHadronicTag() : DiPhotonTagBase::DiPhotonTagBase() {}

TTHHadronicTag::~TTHHadronicTag() {}

TTHHadronicTag::TTHHadronicTag(edm::Ptr<flashgg::DiPhotonCandidate> diPho,edm::Ptr<DiPhotonMVAResult> mvaRes,
			       std::vector<edm::Ptr<flashgg::Jet> > theJetVec , std::vector<edm::Ptr<flashgg::Jet> > theBJetVec) :
	TTHHadronicTag::TTHHadronicTag(diPho,*mvaRes,theJetVec,theBJetVec) {}

TTHHadronicTag::TTHHadronicTag(edm::Ptr<DiPhotonCandidate> dipho,DiPhotonMVAResult mvares,
			       std::vector<edm::Ptr<flashgg::Jet> > theJetVec , std::vector<edm::Ptr<flashgg::Jet> > theBJetVec) :
	DiPhotonTagBase::DiPhotonTagBase(dipho,mvares) {
	theJetVec_ = std::vector<edm::Ptr<flashgg::Jet> >(theJetVec);
	theBJetVec_ = std::vector<edm::Ptr<flashgg::Jet> >(theBJetVec);
}

TTHHadronicTag* TTHHadronicTag::clone () const {
	TTHHadronicTag* result = new TTHHadronicTag(diPhoton(),diPhotonMVA(),theJetVec_,theBJetVec_);
	result->setDiPhotonIndex(diPhotonIndex());
	return result;
}





