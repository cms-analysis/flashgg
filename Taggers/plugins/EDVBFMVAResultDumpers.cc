#include "FWCore/Framework/interface/MakerMacros.h"
#include "flashgg/Taggers/interface/VBFMVAResultDumper.h"
#include "PhysicsTools/UtilAlgos/interface/EDAnalyzerWrapper.h"
typedef edm::AnalyzerWrapper<flashgg::VBFMVAResultDumper> VBFMVAResultDumper;
typedef edm::AnalyzerWrapper<flashgg::CutBasedVBFMVAResultDumper> CutBasedVBFMVAResultDumper;
DEFINE_FWK_MODULE(VBFMVAResultDumper);
DEFINE_FWK_MODULE(CutBasedVBFMVAResultDumper);
