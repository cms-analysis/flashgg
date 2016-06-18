#include "flashgg/DataFormats/interface/Photon.h"
#include "flashgg/Systematics/interface/BaseSystMethod.h"
#include "flashgg/Systematics/interface/ObjectSystematicProducer.h"

namespace flashgg {

    typedef ObjectSystematicProducer<flashgg::Photon, int, std::vector> PhotonSystematicProducer;

}

typedef flashgg::PhotonSystematicProducer FlashggPhotonSystematicProducer;
DEFINE_FWK_MODULE( FlashggPhotonSystematicProducer );

// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4
