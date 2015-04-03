#include "FWCore/Framework/interface/MakerMacros.h"
#include "flashgg/Taggers/interface/DiPhotonMVAResultDumper.h"
#include "PhysicsTools/UtilAlgos/interface/EDAnalyzerWrapper.h"

typedef edm::AnalyzerWrapper<flashgg::DiPhotonMVAResultDumper> DiPhotonMVAResultDumper;
typedef edm::AnalyzerWrapper<flashgg::CutBasedDiPhotonMVAResultDumper> CutBasedDiPhotonMVAResultDumper;

DEFINE_FWK_MODULE( DiPhotonMVAResultDumper );
DEFINE_FWK_MODULE( CutBasedDiPhotonMVAResultDumper );
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

