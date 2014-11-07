#include "flashgg/TagFormats/interface/TTHhadronicTag.h"
#include "flashgg/MicroAODFormats/interface/Jet.h"

using namespace flashgg;

TTHhadronicTag::TTHhadronicTag() : DiPhotonTagBase::DiPhotonTagBase()
{}

TTHhadronicTag::~TTHhadronicTag() 
{}

TTHhadronicTag::TTHhadronicTag(edm::Ptr<flashgg::Jet> theBjet){

theBjet_ = theBjet;
 
}

