#include "flashgg/DataFormats/interface/Met.h"
//#include "DataFormats/PatCandidates/interface/MET.h"
#include "flashgg/Systematics/interface/BaseSystMethod.h"
#include "flashgg/Systematics/interface/ObjectSystematicProducer.h"

namespace flashgg {

    typedef ObjectSystematicProducer<flashgg::Met, int, std::vector> MetSmearSystematicProducer;

}

typedef flashgg::MetSmearSystematicProducer FlashggMetSmearSystematicProducer;
DEFINE_FWK_MODULE( FlashggMetSmearSystematicProducer );


// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4
