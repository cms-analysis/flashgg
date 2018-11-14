#include "FWCore/Framework/interface/MakerMacros.h"
#include "PhysicsTools/UtilAlgos/interface/EDAnalyzerWrapper.h"

#include "flashgg/Taggers/interface/H4GCandidateDumper.h"

typedef edm::AnalyzerWrapper<flashgg::H4GCandidateDumper> H4GCandidateDumper;
typedef edm::AnalyzerWrapper<flashgg::CutBasedH4GCandidateDumper> CutBasedH4GCandidateDumper;

DEFINE_FWK_MODULE( H4GCandidateDumper );
DEFINE_FWK_MODULE( CutBasedH4GCandidateDumper );

// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4
