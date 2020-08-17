#include "flashgg/MicroAOD/interface/PhotonMCUtils.h"

using namespace flashgg;
void PhotonMCUtils::determineMatchType( flashgg::Photon &fg, std::vector<int> promptMothers, flashgg::Photon::mcMatch_t defaultType )
{
    if( ! fg.hasMatchedGenPhoton() ) { return; }
    auto gen = fg.matchedGenPhoton();
    auto matchType = determineMatchType( *gen, promptMothers, defaultType );
    fg.setGenMatchType( matchType );
}

// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

