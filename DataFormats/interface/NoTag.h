#ifndef flashgg_NoTag
#define flashgg_NoTag

#include "flashgg/DataFormats/interface/DiPhotonTagBase.h"

namespace flashgg {

    class NoTag: public DiPhotonTagBase
    {
    public:
        NoTag();
        ~NoTag();

        virtual NoTag *clone() const override;
        DiPhotonTagBase::tag_t tagEnum() const override {return DiPhotonTagBase::kUndefined; }

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

