#ifndef FLASHgg_VHEtTag_h
#define FLASHgg_VHEtTag_h


#include "flashgg/DataFormats/interface/DiPhotonTagBase.h"

namespace flashgg {

    class VHEtTag: public DiPhotonTagBase
    {
    public:
        VHEtTag();
        ~VHEtTag();

        VHEtTag( edm::Ptr<DiPhotonCandidate>, edm::Ptr<DiPhotonMVAResult> );
        VHEtTag( edm::Ptr<DiPhotonCandidate>, DiPhotonMVAResult);
        VHEtTag *clone() const;
        float met(); //change to flashgg::met eventually

        //private:
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

