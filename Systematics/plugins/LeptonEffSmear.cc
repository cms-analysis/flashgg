#include "flashgg/Systematics/interface/LeptonEffSmearNew.h"

namespace flashgg {

		typedef LeptonEffSmearNew<flashgg::Electron, int> ElectronEffSmear;
		typedef LeptonEffSmearNew<pat::Muon, int> MuonEffSmear;
}
	
DEFINE_EDM_PLUGIN(FlashggSystematicElectronMethodsFactory,
		flashgg::ElectronEffSmear,
		"FlashggElectronEffSmear" );

DEFINE_EDM_PLUGIN(FlashggSystematicMuonMethodsFactory,
		flashgg::MuonEffSmear,
		"FlashggMuonEffSmear" );
