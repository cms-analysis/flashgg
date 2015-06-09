#include "flashgg/Systematics/interface/LeptonEffSmear.h"

namespace flashgg {

		typedef LeptonEffSmear<flashgg::Electron, int> ElectronEffSmear;
		typedef LeptonEffSmear<pat::Muon, int> MuonEffSmear;

}
	
DEFINE_EDM_PLUGIN(FlashggSystematicElectronMethodsFactory,
		flashgg::ElectronEffSmear,
		"FlashggElectronEffSmear" );

DEFINE_EDM_PLUGIN(FlashggSystematicMuonMethodsFactory,
		flashgg::MuonEffSmear,
		"FlashggMuonEffSmear" );
