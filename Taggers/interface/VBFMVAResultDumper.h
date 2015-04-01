#ifndef flashgg_VBFMVAResultDumpers_h
#define flashgg_VBFMVAResultDumpers_h
#include "flashgg/DataFormats/interface/VBFMVAResult.h"
#include "flashgg/Taggers/interface/CollectionDumper.h"
namespace flashgg
{
typedef CollectionDumper<std::vector<VBFMVAResult> > VBFMVAResultDumper;
typedef CollectionDumper<std::vector<VBFMVAResult> ,
VBFMVAResult,
CutBasedClassifier<VBFMVAResult> > CutBasedVBFMVAResultDumper;
}
#endif 
