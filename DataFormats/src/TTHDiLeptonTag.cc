#include "flashgg/DataFormats/interface/TTHDiLeptonTag.h"
#include <algorithm>

using namespace flashgg;

TTHDiLeptonTag::TTHDiLeptonTag() : DiPhotonTagBase::DiPhotonTagBase()
{}

TTHDiLeptonTag::~TTHDiLeptonTag()
{}

// N.B. Other attributes are set using methods in header file
TTHDiLeptonTag::TTHDiLeptonTag( edm::Ptr<DiPhotonCandidate> diPho, edm::Ptr<DiPhotonMVAResult> mvares ) : TTHDiLeptonTag::TTHDiLeptonTag( diPho, *mvares ) {}
TTHDiLeptonTag::TTHDiLeptonTag( edm::Ptr<DiPhotonCandidate> dipho, DiPhotonMVAResult mvares ) : DiPhotonTagBase::DiPhotonTagBase( dipho, mvares ) {}

// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

