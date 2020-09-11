#ifndef FLASHgg_TagTruthBase_h
#define FLASHgg_TagTruthBase_h

#include "DataFormats/Math/interface/Point3D.h"
#include "flashgg/DataFormats/interface/WeightedObject.h"
#include <vector>
#include <map>

namespace flashgg {

    class TagTruthBase : public WeightedObject
    {
    public:

        typedef math::XYZPoint Point;

        TagTruthBase();
        TagTruthBase( const TagTruthBase &b );
        virtual ~TagTruthBase() {}

        Point genPV() const { return genPV_; }
        void setGenPV( const Point genpv ) { genPV_ = genpv; }
        int HTXSstage0bin() const { return stage0bin_; }
        int HTXSstage1bin() const { return stage1bin_; }
        int HTXSstage1p1bin() const { return stage1p1bin_; }
        int HTXSstage1p1binFine() const { return stage1p1binFine_; }
        int HTXSstage1p2bin() const { return stage1p2bin_; }
        int HTXSstage1p2binFine() const { return stage1p2binFine_; }
        int HTXSstage0orderedBin() const;
        int HTXSstage1orderedBin() const;
        int HTXSstage1p1orderedBin() const;
        int HTXSstage1p1orderedBinFine() const;
        int HTXSstage1p2orderedBin() const;
        int HTXSstage1p2orderedBinFine() const;
        int HTXSnjets() const { return njets_; }
        float HTXSpTH() const { return pTH_; }
        float HTXSpTV() const { return pTV_; }
        void setHTXSInfo( int stage0bin, int stage1bin, int stage1p1bin, int stage1p1binFine, int stage1p2bin, int stage1p2binFine, int njets, float pTH, float pTV );
        void setGluonFusionWeights( int , float , int );
        void copyBaseInfo( const TagTruthBase &b );
        virtual TagTruthBase *clone() const;

        // Ordering is needed for technical reasons (OwnVector) but will never be used
        bool operator <( const TagTruthBase &b ) const { return ( genPV().z() < b.genPV().z() ); };

    private:
        Point genPV_;
        int stage0bin_;
        int stage1bin_;
        int stage1p1bin_;
        int stage1p1binFine_;
        int stage1p2bin_;
        int stage1p2binFine_;
        std::map<int,int> stage0map_;
        std::map<int,int> stage1map_;
        std::map<int,int> stage1p1map_;
        std::map<int,int> stage1p1mapFine_;
        std::map<int,int> stage1p2map_;
        std::map<int,int> stage1p2mapFine_;
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

