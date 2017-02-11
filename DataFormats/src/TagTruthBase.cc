#include "flashgg/DataFormats/interface/TagTruthBase.h"

using namespace flashgg;

TagTruthBase::TagTruthBase( const TagTruthBase &b )
{
    copyBaseInfo( b );
}

TagTruthBase *TagTruthBase::clone() const
{
    TagTruthBase *result = new TagTruthBase( *this );
    return result;
}

void TagTruthBase::setHTXSInfo( int stage0cat, int stage1cat, int njets, float pTH, float pTV ) {
    stage0cat_ = stage0cat;
    stage1cat_ = stage1cat;
    njets_ = njets;
    pTH_ = pTH;
    pTV_ = pTV;
}

void TagTruthBase::copyBaseInfo( const TagTruthBase &b ) {
    setGenPV( b.genPV() );
    setHTXSInfo( b.HTXSstage0cat(), b.HTXSstage1cat(), b.HTXSnjets(), b.HTXSpTH(), b.HTXSpTV() );
}


// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4
