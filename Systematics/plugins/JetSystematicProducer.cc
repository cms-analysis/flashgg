#include "flashgg/DataFormats/interface/Jet.h"
#include "flashgg/Systematics/interface/BaseSystMethod.h"
#include "flashgg/Systematics/interface/ObjectSystematicProducer.h"

namespace flashgg {

    typedef ObjectSystematicProducer<Jet, int, std::vector> JetSystematicProducer;

}

typedef flashgg::JetSystematicProducer FlashggJetSystematicProducer;
DEFINE_FWK_MODULE( FlashggJetSystematicProducer );

// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4
