#include "flashgg/TagFormats/interface/TTHhadronicTag.h"
#include "flashgg/MicroAODFormats/interface/Jet.h"

using namespace flashgg;

TTHhadronicTag::TTHhadronicTag() : DiPhotonTagBase::DiPhotonTagBase()
{}

TTHhadronicTag::~TTHhadronicTag() 
{}

TTHhadronicTag::TTHhadronicTag(std::vector<edm::Ptr<flashgg::Jet> > theJetVec , std::vector<edm::Ptr<flashgg::Jet> > theBJetVec ){ 

theJetVec_ = theJetVec;

theBJetVec_ = theBJetVec;

}



