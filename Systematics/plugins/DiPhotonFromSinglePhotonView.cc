#include "flashgg/Systematics/interface/DiPhotonFromSinglePhotonViewBase.h"

namespace flashgg {

    typedef DiPhotonFromSinglePhotonViewBase<int> DiPhotonFromSinglePhotonView;

}

DEFINE_EDM_PLUGIN( FlashggSystematicDiPhotonMethodsFactory,
                   flashgg::DiPhotonFromSinglePhotonView,
                   "FlashggDiPhotonFromSinglePhotonView" );

// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4
