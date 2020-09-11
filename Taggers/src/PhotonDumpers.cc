#include "flashgg/Taggers/interface/PhotonDumpers.h"

#include "flashgg/Taggers/interface/PluggableAnalyzer.h"

namespace flashgg {
    // typedef AnalyzerFactoryImpl<DiPhotonDumper,"DiPhotonDumper"> DiPhotonDumperFactory;
    namespace fwlite {
        PLUGGABLE_ANALYZER( GenDiPhotonDumper );
        PLUGGABLE_ANALYZER( CutBasedGenDiPhotonDumper );
        PLUGGABLE_ANALYZER( PhotonDumper );
        PLUGGABLE_ANALYZER( CutBasedPhotonDumper );
        PLUGGABLE_ANALYZER( DiPhotonDumper );
        PLUGGABLE_ANALYZER( CutBasedDiPhotonDumper );
        PLUGGABLE_ANALYZER( CutBasedSinglePhotonViewDumper );
        PLUGGABLE_ANALYZER( CutBasedSingleVertexViewDumper );
        PLUGGABLE_ANALYZER( DiPhotonTagDumper );
        PLUGGABLE_ANALYZER( StageOneDiPhotonTagDumper );
    }
}

// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

