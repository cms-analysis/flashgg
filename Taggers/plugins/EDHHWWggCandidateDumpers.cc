#include "FWCore/Framework/interface/MakerMacros.h"
#include "PhysicsTools/UtilAlgos/interface/EDAnalyzerWrapper.h"

#include "flashgg/Taggers/interface/HHWWggCandidateDumper.h"

typedef edm::AnalyzerWrapper<flashgg::HHWWggCandidateDumper> HHWWggCandidateDumper;
typedef edm::AnalyzerWrapper<flashgg::CutBasedHHWWggCandidateDumper> CutBasedHHWWggCandidateDumper;

DEFINE_FWK_MODULE( HHWWggCandidateDumper );
DEFINE_FWK_MODULE( CutBasedHHWWggCandidateDumper );

// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4
