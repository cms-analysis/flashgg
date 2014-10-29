#include "flashgg/TagFormats/interface/TTHleptonicTag.h"

using namespace flashgg;

TTHleptonicTag::TTHleptonicTag() : DiPhotonTagBase::DiPhotonTagBase()
{}

TTHleptonicTag::~TTHleptonicTag() 
{}


TTHleptonicTag::TTHleptonicTag(edm::Ptr<DiPhotonCandidate> diPho){

theDiPhotonCandidate_ = diPho;

}
