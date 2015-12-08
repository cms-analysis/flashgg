#include "flashgg/Systematics/interface/ObjectWeight.h"

namespace flashgg {

    typedef ObjectWeight<flashgg::Photon, int> PhotonWeight;
}

DEFINE_EDM_PLUGIN( FlashggSystematicPhotonMethodsFactory,
                   flashgg::PhotonWeight,
                   "FlashggPhotonWeight" );

// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4
