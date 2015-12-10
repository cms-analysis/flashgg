#include "flashgg/DataFormats/interface/Photon.h"
#include "flashgg/MicroAOD/interface/RandomizedObjectProducer.h"

namespace flashgg {

    typedef RandomizedObjectProducer<flashgg::Photon> RandomizedPhotonProducer;

}

typedef flashgg::RandomizedPhotonProducer FlashggRandomizedPhotonProducer;
DEFINE_FWK_MODULE( FlashggRandomizedPhotonProducer );

// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4
