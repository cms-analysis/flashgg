#include "flashgg/Taggers/interface/VBFMVAResultDumper.h"
#include "flashgg/Taggers/interface/PluggableAnalyzer.h"
namespace flashgg {
	namespace fwlite {
		PLUGGABLE_ANALYZER(VBFMVAResultDumper);
		PLUGGABLE_ANALYZER(CutBasedVBFMVAResultDumper);
	}
}
