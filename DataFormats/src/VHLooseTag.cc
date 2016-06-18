#include "flashgg/DataFormats/interface/VHLooseTag.h"
#include <algorithm>

using namespace flashgg;

VHLooseTag::VHLooseTag() : DiPhotonTagBase::DiPhotonTagBase()
{}

VHLooseTag::~VHLooseTag()
{}


VHLooseTag::VHLooseTag( edm::Ptr<DiPhotonCandidate> diPho, edm::Ptr<DiPhotonMVAResult> mvares ) : DiPhotonTagBase::DiPhotonTagBase( diPho, *mvares ) {}
VHLooseTag::VHLooseTag( edm::Ptr<DiPhotonCandidate> dipho, DiPhotonMVAResult mvares ) : DiPhotonTagBase::DiPhotonTagBase( dipho, mvares ) {}

// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

