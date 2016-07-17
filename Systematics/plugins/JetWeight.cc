#include "flashgg/Systematics/interface/ObjectWeight.h"

namespace flashgg {

    typedef ObjectWeight<flashgg::Jet, int> JetWeight;
}

DEFINE_EDM_PLUGIN( FlashggSystematicJetMethodsFactory,
                   flashgg::JetWeight,
                   "FlashggJetWeight" );

// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4
