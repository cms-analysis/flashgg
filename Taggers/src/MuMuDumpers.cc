#include "flashgg/Taggers/interface/MuMuDumpers.h"

#include "flashgg/Taggers/interface/PluggableAnalyzer.h"

//-----------J. Tao from IHEP-Beijing--------------

namespace flashgg {
    // typedef AnalyzerFactoryImpl<DiPhotonDumper,"DiPhotonDumper"> DiPhotonDumperFactory;
    namespace fwlite {
        PLUGGABLE_ANALYZER( DiMuonDumper );
        PLUGGABLE_ANALYZER( CutBasedDiMuonDumper );
        PLUGGABLE_ANALYZER( MuMuGammaDumper );
        PLUGGABLE_ANALYZER( CutBasedMuMuGammaDumper );
    }
}

// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4
