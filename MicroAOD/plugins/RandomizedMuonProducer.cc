#include "flashgg/DataFormats/interface/Muon.h"
#include "flashgg/MicroAOD/interface/RandomizedObjectProducer.h"

namespace flashgg {

    typedef RandomizedObjectProducer<flashgg::Muon> RandomizedMuonProducer;

}

typedef flashgg::RandomizedMuonProducer FlashggRandomizedMuonProducer;
DEFINE_FWK_MODULE( FlashggRandomizedMuonProducer );

// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4
