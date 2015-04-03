#include "FWCore/Framework/interface/MakerMacros.h"
#include "flashgg/Taggers/interface/VBFDiPhoDiJetMVAResultDumper.h"
#include "PhysicsTools/UtilAlgos/interface/EDAnalyzerWrapper.h"
typedef edm::AnalyzerWrapper<flashgg::VBFDiPhoDiJetMVAResultDumper> VBFDiPhoDiJetMVAResultDumper;
typedef edm::AnalyzerWrapper<flashgg::CutBasedVBFDiPhoDiJetMVAResultDumper> CutBasedVBFDiPhoDiJetMVAResultDumper;
DEFINE_FWK_MODULE( VBFDiPhoDiJetMVAResultDumper );
DEFINE_FWK_MODULE( CutBasedVBFDiPhoDiJetMVAResultDumper );
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

