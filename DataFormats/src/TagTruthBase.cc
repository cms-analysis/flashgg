#include "flashgg/DataFormats/interface/TagTruthBase.h"

using namespace flashgg;

TagTruthBase::TagTruthBase( const TagTruthBase &b )
{
    setGenPV( b.genPV() );
}

TagTruthBase *TagTruthBase::clone() const
{
    TagTruthBase *result = new TagTruthBase( *this );
    return result;
}
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4
