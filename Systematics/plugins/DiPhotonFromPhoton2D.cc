#include "flashgg/Systematics/interface/DiPhotonFromPhotonBase.h"

namespace flashgg {

    typedef DiPhotonFromPhotonBase<std::pair<int, int> > DiPhotonFromPhoton2D;

}

DEFINE_EDM_PLUGIN( FlashggSystematicDiPhotonMethodsFactory2D,
                   flashgg::DiPhotonFromPhoton2D,
                   "FlashggDiPhotonFromPhoton2D" );

// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4
