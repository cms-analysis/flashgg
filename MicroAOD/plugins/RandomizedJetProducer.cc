#include "flashgg/DataFormats/interface/Jet.h"
#include "flashgg/MicroAOD/interface/RandomizedObjectProducer.h"

namespace flashgg {

    typedef RandomizedObjectProducer<flashgg::Jet> RandomizedJetProducer;

}

typedef flashgg::RandomizedJetProducer FlashggRandomizedJetProducer;
DEFINE_FWK_MODULE( FlashggRandomizedJetProducer );

// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

