#include "flashgg/DataFormats/interface/Electron.h"
#include "flashgg/MicroAOD/interface/RandomizedObjectProducer.h"

namespace flashgg {

    typedef RandomizedObjectProducer<flashgg::Electron> RandomizedElectronProducer;

}

typedef flashgg::RandomizedElectronProducer FlashggRandomizedElectronProducer;
DEFINE_FWK_MODULE( FlashggRandomizedElectronProducer );

// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4
