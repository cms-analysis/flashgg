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
        virtual TagTruthBase *clone() const;

        // Ordering is needed for technical reasons (OwnVector) but will never be used
        bool operator <( const TagTruthBase &b ) const { return ( genPV().z() < b.genPV().z() ); };

    private:
        Point genPV_;
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

