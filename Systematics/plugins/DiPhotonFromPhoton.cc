#include "flashgg/Systematics/interface/DiPhotonFromPhotonBase.h"

namespace flashgg {

    typedef DiPhotonFromPhotonBase<int> DiPhotonFromPhoton;

}

DEFINE_EDM_PLUGIN( FlashggSystematicDiPhotonMethodsFactory,
                   flashgg::DiPhotonFromPhoton,
                   "FlashggDiPhotonFromPhoton" );

// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4
