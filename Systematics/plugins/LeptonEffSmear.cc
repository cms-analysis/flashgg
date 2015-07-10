#include "flashgg/Systematics/interface/LeptonEffSmear.h"

namespace flashgg {

    typedef LeptonEffSmearNew<flashgg::Electron, int> ElectronEffSmear;
    typedef LeptonEffSmearNew<flashgg::Muon, int> MuonEffSmear;
}

DEFINE_EDM_PLUGIN( FlashggSystematicElectronMethodsFactory,
                   flashgg::ElectronEffSmear,
                   "FlashggElectronEffSmear" );

DEFINE_EDM_PLUGIN( FlashggSystematicMuonMethodsFactory,
                   flashgg::MuonEffSmear,
                   "FlashggMuonEffSmear" );

// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4
