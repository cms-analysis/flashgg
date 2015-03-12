#include "flashgg/MicroAOD/interface/PhotonMCUtils.h"

using namespace flashgg;
void PhotonMCUtils::determineMatchType(flashgg::Photon & fg, flashgg::Photon::mcMatch_t defaultType) 
{
	if( ! fg.hasMatchedGenPhoton() ) { return; }
	auto gen = fg.matchedGenPhoton();
	auto matchType = determineMatchType(*gen,defaultType);
	fg.setGenMatchType(matchType);
}

