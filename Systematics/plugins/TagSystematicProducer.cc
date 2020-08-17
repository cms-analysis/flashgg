#include "flashgg/DataFormats/interface/DiPhotonTagBase.h"
#include "flashgg/Systematics/interface/BaseSystMethod.h"
#include "flashgg/Systematics/interface/ObjectSystematicProducer.h"

namespace flashgg {

    typedef ObjectSystematicProducer<flashgg::DiPhotonTagBase, int, edm::OwnVector> TagSystematicProducer;

}

typedef flashgg::TagSystematicProducer FlashggTagSystematicProducer;
DEFINE_FWK_MODULE( FlashggTagSystematicProducer );

// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4
