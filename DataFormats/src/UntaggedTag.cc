#include "flashgg/DataFormats/interface/UntaggedTag.h"

using namespace flashgg;

UntaggedTag::UntaggedTag() : DiPhotonTagBase::DiPhotonTagBase() {}

UntaggedTag::~UntaggedTag() {}

UntaggedTag::UntaggedTag( edm::Ptr<flashgg::DiPhotonCandidate> diPho, edm::Ptr<DiPhotonMVAResult> mvaRes )
    : UntaggedTag::UntaggedTag( diPho, *mvaRes ) {}

UntaggedTag::UntaggedTag( edm::Ptr<DiPhotonCandidate> dipho, DiPhotonMVAResult mvares ) :  DiPhotonTagBase::DiPhotonTagBase( dipho,
            mvares ) {}

UntaggedTag *UntaggedTag::clone() const
{
    UntaggedTag *result = new UntaggedTag( *this );
    return result;
}
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

