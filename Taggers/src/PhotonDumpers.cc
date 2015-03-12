#include "flashgg/Taggers/interface/PhotonDumpers.h"

#include "flashgg/Taggers/interface/PluggableAnalyzer.h"

namespace flashgg {
	// typedef AnalyzerFactoryImpl<DiPhotonDumper,"DiPhotonDumper"> DiPhotonDumperFactory;
	namespace fwlite {
		PLUGGABLE_ANALYZER(PhotonDumper);
		PLUGGABLE_ANALYZER(CutBasedPhotonDumper);
		PLUGGABLE_ANALYZER(DiPhotonDumper);
		PLUGGABLE_ANALYZER(CutBasedDiPhotonDumper);
		PLUGGABLE_ANALYZER(CutBasedSinglePhotonViewDumper);
		PLUGGABLE_ANALYZER(CutBasedSingleVertexViewDumper);
		PLUGGABLE_ANALYZER(DiPhotonTagDumper);
	}
}
