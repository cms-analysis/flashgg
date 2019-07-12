
#include "flashgg/Validation/interface/TagAndProbeDumper.h"

#include "flashgg/Taggers/interface/PluggableAnalyzer.h"

namespace flashgg {
    // typedef AnalyzerFactoryImpl<DiPhotonDumper,"DiPhotonDumper"> DiPhotonDumperFactory;
    namespace fwlite {
        PLUGGABLE_ANALYZER( TagAndProbeDumper );
        PLUGGABLE_ANALYZER( CutBasedTagAndProbeDumper );
    }
}

// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

