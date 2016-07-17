#ifndef flashgg_SigmaMpTTag
#define flashgg_SigmaMpTTag

#include "flashgg/DataFormats/interface/DiPhotonTagBase.h"

namespace flashgg {

    class SigmaMpTTag: public DiPhotonTagBase
    {
    public:
        SigmaMpTTag();
        ~SigmaMpTTag();

        SigmaMpTTag( edm::Ptr<DiPhotonCandidate>, DiPhotonMVAResult );
        SigmaMpTTag( edm::Ptr<DiPhotonCandidate>, edm::Ptr<DiPhotonMVAResult> );
        virtual SigmaMpTTag *clone() const override;
        DiPhotonTagBase::tag_t tagEnum() const override {return DiPhotonTagBase::kUntagged; }

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

