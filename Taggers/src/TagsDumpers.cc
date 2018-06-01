#include "flashgg/Taggers/interface/TagsDumpers.h"

#include "flashgg/Taggers/interface/PluggableAnalyzer.h"

namespace flashgg {
    namespace fwlite {
        PLUGGABLE_ANALYZER( CutBasedTTHHadronicTagDumper );
        PLUGGABLE_ANALYZER( CutBasedTTHLeptonicTagDumper );
        PLUGGABLE_ANALYZER( CutBasedTTHDiLeptonTagDumper );
        PLUGGABLE_ANALYZER( CutBasedVHLooseTagDumper );
        PLUGGABLE_ANALYZER( CutBasedVHTightTagDumper );
        PLUGGABLE_ANALYZER( CutBasedVHEtTagDumper );
        PLUGGABLE_ANALYZER( CutBasedVHHadronicTagDumper );
        PLUGGABLE_ANALYZER( CutBasedVBFTagDumper );
        PLUGGABLE_ANALYZER( CutBasedZPlusJetTagDumper );
        PLUGGABLE_ANALYZER( CutBasedWHLeptonicTagDumper );
        PLUGGABLE_ANALYZER( CutBasedVHLeptonicLooseTagDumper );
        PLUGGABLE_ANALYZER( CutBasedZHLeptonicTagDumper );
    }
}

// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4
