#include "flashgg/DataFormats/interface/NoTag.h"

using namespace flashgg;

NoTag::NoTag() : DiPhotonTagBase::DiPhotonTagBase() {}

NoTag::~NoTag() {}

NoTag *NoTag::clone() const
{
    NoTag *result = new NoTag(*this);
    return result;
}
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

