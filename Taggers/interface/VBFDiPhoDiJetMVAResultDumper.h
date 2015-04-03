#ifndef flashgg_VBFDiPhoDiJetMVAResultDumpers_h
#define flashgg_VBFDiPhoDiJetMVAResultDumpers_h
#include "flashgg/DataFormats/interface/VBFDiPhoDiJetMVAResult.h"
#include "flashgg/Taggers/interface/CollectionDumper.h"
namespace flashgg
{
typedef CollectionDumper<std::vector<VBFDiPhoDiJetMVAResult> > VBFDiPhoDiJetMVAResultDumper;
typedef CollectionDumper<std::vector<VBFDiPhoDiJetMVAResult> ,
VBFDiPhoDiJetMVAResult,
CutBasedClassifier<VBFDiPhoDiJetMVAResult> > CutBasedVBFDiPhoDiJetMVAResultDumper;
}
#endif 
