#include "FWCore/Framework/interface/MakerMacros.h"
#include "PhysicsTools/UtilAlgos/interface/EDAnalyzerWrapper.h"

#include "flashgg/Taggers/interface/TagCandidateDumper.h"

typedef edm::AnalyzerWrapper<flashgg::CutBasedTagCandidateDumper> CutBasedTagCandidateDumper;

DEFINE_FWK_MODULE( CutBasedTagCandidateDumper );

// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4
