#include "flashgg/DataFormats/interface/SigmaMpTTag.h"

using namespace flashgg;

SigmaMpTTag::SigmaMpTTag() : DiPhotonTagBase::DiPhotonTagBase() {}

SigmaMpTTag::~SigmaMpTTag() {}

SigmaMpTTag::SigmaMpTTag( edm::Ptr<flashgg::DiPhotonCandidate> diPho, edm::Ptr<DiPhotonMVAResult> mvaRes )
    : SigmaMpTTag::SigmaMpTTag( diPho, *mvaRes ) {}

SigmaMpTTag::SigmaMpTTag( edm::Ptr<DiPhotonCandidate> dipho, DiPhotonMVAResult mvares ) :  DiPhotonTagBase::DiPhotonTagBase( dipho,
            mvares ) {}

SigmaMpTTag *SigmaMpTTag::clone() const
{
    SigmaMpTTag *result = new SigmaMpTTag( *this );
    return result;
}
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

