#ifndef FLASHgg_Met_h
#define FLASHgg_Met_h

#include "DataFormats/PatCandidates/interface/MET.h"
#include "flashgg/DataFormats/interface/DiPhotonCandidate.h"
#include "flashgg/DataFormats/interface/WeightedObject.h"

namespace flashgg {

    class Met : public pat::MET, public WeightedObject
    {

    public:
        Met();
        Met( const pat::MET & );
        ~Met();

        Met *clone() const { return ( new Met( *this ) ); }

        void setCorPx(float mPx);
        void setCorPy(float mPy);
        void setCorPt(float mPt);
        void setCorPhi(float mPhi);
        void setPassEcalBadCalibFilter(bool mPass);
        float getCorPx() const;
        float getCorPy() const;
        float getCorPhi() const;
        float getCorPt() const;
        bool getPassEcalBadCalibFilter() const;

    private:
        float corpx;
        float corpy;
        float corphi;
        float corpt;
        bool passEcalBadCalibFilter;
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

