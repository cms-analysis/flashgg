
#include "flashgg/Taggers/interface/bRegressionDumper.h"

#include "flashgg/Taggers/interface/PluggableAnalyzer.h"

namespace flashgg {
    // typedef AnalyzerFactoryImpl<DiPhotonDumper,"DiPhotonDumper"> DiPhotonDumperFactory;
    namespace fwlite {
        PLUGGABLE_ANALYZER( bRegressionDumper );
        PLUGGABLE_ANALYZER( CutBasedbRegressionDumper );
    }
}

// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

