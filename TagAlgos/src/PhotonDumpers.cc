#include "flashgg/TagAlgos/interface/PhotonDumpers.h"

#include "flashgg/TagAlgos/interface/PluggableAnalyzer.h"

namespace flashgg {
	// typedef AnalyzerFactoryImpl<DiPhotonDumper,"DiPhotonDumper"> DiPhotonDumperFactory;
	namespace fwlite {
		PLUGGABLE_ANALYZER(PhotonDumper);
		PLUGGABLE_ANALYZER(CutBasedPhotonDumper);
		PLUGGABLE_ANALYZER(DiPhotonDumper);
		PLUGGABLE_ANALYZER(CutBasedDiPhotonDumper);
		PLUGGABLE_ANALYZER(DiPhotonTagDumper);
	}
}
