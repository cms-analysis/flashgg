#include "flashgg/DataFormats/interface/DiPhotonTagBase.h"

namespace flashgg {

    class DiPhotonUntaggedCategory: public DiPhotonTagBase
    {
    public:
        DiPhotonUntaggedCategory();
        ~DiPhotonUntaggedCategory();

        DiPhotonUntaggedCategory( edm::Ptr<DiPhotonCandidate>, DiPhotonMVAResult );
        DiPhotonUntaggedCategory( edm::Ptr<DiPhotonCandidate>, edm::Ptr<DiPhotonMVAResult> );
        virtual DiPhotonUntaggedCategory *clone() const;


    private:



    };
}
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

