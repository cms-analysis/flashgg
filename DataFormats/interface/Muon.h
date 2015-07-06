#ifndef FLASHgg_Muon_h
#define FLASHgg_Muon_h

#include "DataFormats/PatCandidates/interface/Muon.h"
#include "flashgg/DataFormats/interface/DiPhotonCandidate.h"

namespace flashgg {

    class Muon : public pat::Muon
    {

    public:
        Muon();
        Muon( const pat::Muon & );
        ~Muon();

        void setWeight( double val ) { weight_ = val; }
        double weight() const { return weight_; }

    private:
        float  weight_;

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

