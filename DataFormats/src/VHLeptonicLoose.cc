#include "flashgg/DataFormats/interface/VHLeptonicLooseTag.h"
#include <algorithm>

using namespace flashgg;

VHLeptonicLooseTag::VHLeptonicLooseTag() : DiPhotonTagBase::DiPhotonTagBase()
{}

VHLeptonicLooseTag::~VHLeptonicLooseTag()
{}


VHLeptonicLooseTag::VHLeptonicLooseTag( edm::Ptr<DiPhotonCandidate> diPho, edm::Ptr<DiPhotonMVAResult> mvares ) : DiPhotonTagBase::DiPhotonTagBase( diPho, *mvares ) {}
VHLeptonicLooseTag::VHLeptonicLooseTag( edm::Ptr<DiPhotonCandidate> dipho, DiPhotonMVAResult mvares ) : DiPhotonTagBase::DiPhotonTagBase( dipho, mvares ) {}


// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

