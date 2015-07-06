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
