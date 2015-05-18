#include "flashgg/DataFormats/interface/Electron.h"
#include "flashgg/Systematics/interface/BaseSystMethods.h"
#include "flashgg/Systematics/interface/ObjectSystematicProducer.h"

namespace flashgg {

    typedef ObjectSystematicProducer<flashgg::Electron, int> LeptonEffSystematicProducer;

}

typedef flashgg::LeptonEffSystematicProducer FlashggLeptonEffSystematicProducer;
DEFINE_FWK_MODULE( FlashggLeptonEffSystematicProducer );

// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4
