#include "flashgg/Taggers/interface/MuMuDumpers.h"

#include "flashgg/Taggers/interface/PluggableAnalyzer.h"

//-----------J. Tao from IHEP-Beijing--------------

namespace flashgg {
	// typedef AnalyzerFactoryImpl<DiPhotonDumper,"DiPhotonDumper"> DiPhotonDumperFactory;
	namespace fwlite {
		PLUGGABLE_ANALYZER(DiMuonDumper);
                PLUGGABLE_ANALYZER(CutBasedDiMuonDumper);
		PLUGGABLE_ANALYZER(MuMuGammaDumper);
                PLUGGABLE_ANALYZER(CutBasedMuMuGammaDumper);
	}
}
