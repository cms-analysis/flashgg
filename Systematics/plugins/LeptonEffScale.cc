#include "flashgg/Systematics/interface/ObjectEffScale.h"

namespace flashgg {

    typedef ObjectEffScale<flashgg::Electron, int> ElectronEffScale;
    typedef ObjectEffScale<flashgg::Muon, int> MuonEffScale;
}

DEFINE_EDM_PLUGIN( FlashggSystematicElectronMethodsFactory,
                   flashgg::ElectronEffScale,
                   "FlashggElectronEffScale" );

DEFINE_EDM_PLUGIN( FlashggSystematicMuonMethodsFactory,
                   flashgg::MuonEffScale,
                   "FlashggMuonEffScale" );

// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4
