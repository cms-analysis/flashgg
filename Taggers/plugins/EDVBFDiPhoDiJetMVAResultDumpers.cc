#include "FWCore/Framework/interface/MakerMacros.h"
#include "flashgg/Taggers/interface/VBFDiPhoDiJetMVAResultDumper.h"
#include "PhysicsTools/UtilAlgos/interface/EDAnalyzerWrapper.h"
typedef edm::AnalyzerWrapper<flashgg::VBFDiPhoDiJetMVAResultDumper> VBFDiPhoDiJetMVAResultDumper;
typedef edm::AnalyzerWrapper<flashgg::CutBasedVBFDiPhoDiJetMVAResultDumper> CutBasedVBFDiPhoDiJetMVAResultDumper;
DEFINE_FWK_MODULE(VBFDiPhoDiJetMVAResultDumper);
DEFINE_FWK_MODULE(CutBasedVBFDiPhoDiJetMVAResultDumper);
