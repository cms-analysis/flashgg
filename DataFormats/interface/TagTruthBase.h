#ifndef FLASHgg_TagTruthBase_h
#define FLASHgg_TagTruthBase_h

#include "DataFormats/Math/interface/Point3D.h"

namespace flashgg {

    class TagTruthBase
    {
    public:

        typedef math::XYZPoint Point;

        TagTruthBase() {}
        TagTruthBase( const TagTruthBase &b );
        virtual ~TagTruthBase() {}

        Point genPV() const { return genPV_; }
        void setGenPV( const Point genpv ) { genPV_ = genpv; }
        int HTXSstage0cat() const { return stage0cat_; }
        int HTXSstage1cat() const { return stage1cat_; }
        int HTXSnjets() const { return njets_; }
        float HTXSpTH() const { return pTH_; }
        float HTXSpTV() const { return pTV_; }
        void setHTXSInfo( int stage0cat, int stage1cat, int njets, float pTH, float pTV );
        void copyBaseInfo( const TagTruthBase &b );
        virtual TagTruthBase *clone() const;

        // Ordering is needed for technical reasons (OwnVector) but will never be used
        bool operator <( const TagTruthBase &b ) const { return ( genPV().z() < b.genPV().z() ); };

    private:
        Point genPV_;
        int stage0cat_;
        int stage1cat_;
        int njets_;
        float pTH_;
        float pTV_;
    };
}

#endif
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

