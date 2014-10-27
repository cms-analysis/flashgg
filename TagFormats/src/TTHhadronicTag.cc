#include "flashgg/TagFormats/interface/TTHhadronicTag.h"

using namespace flashgg;

TTHhadronicTag::TTHhadronicTag() : DiPhotonTagBase::DiPhotonTagBase()
{}

TTHhadronicTag::~TTHhadronicTag() 
{}


TTHhadronicTag::TTHhadronicTag(edm::Ptr<flashgg::Jet> theBJet){

theBJet_ = theBJet;

}

