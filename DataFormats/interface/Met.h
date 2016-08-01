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
        /*
        setCorPx(float mPx);
        setCorPy(float mPx);
        setCorPhi(float mPx);
        */
    private:
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

