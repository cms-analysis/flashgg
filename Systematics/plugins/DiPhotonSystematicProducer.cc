#include "flashgg/DataFormats/interface/DiPhotonCandidate.h"
#include "flashgg/Systematics/interface/BaseSystMethods.h"
#include "flashgg/Systematics/interface/ObjectSystematicProducer.h"

namespace flashgg {

    typedef ObjectSystematicProducer<DiPhotonCandidate, int> DiPhotonSystematicProducer;

}

typedef flashgg::DiPhotonSystematicProducer FlashggDiPhotonSystematicProducer;
DEFINE_FWK_MODULE( FlashggDiPhotonSystematicProducer );

// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4
