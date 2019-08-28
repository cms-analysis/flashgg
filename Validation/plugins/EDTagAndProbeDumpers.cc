#include "FWCore/Framework/interface/MakerMacros.h"
#include "PhysicsTools/UtilAlgos/interface/EDAnalyzerWrapper.h"

#include "flashgg/Validation/interface/TagAndProbeDumper.h"

typedef edm::AnalyzerWrapper<flashgg::TagAndProbeDumper> TagAndProbeDumper;
typedef edm::AnalyzerWrapper<flashgg::CutBasedTagAndProbeDumper> CutBasedTagAndProbeDumper;

DEFINE_FWK_MODULE( TagAndProbeDumper );
DEFINE_FWK_MODULE( CutBasedTagAndProbeDumper );

// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4
