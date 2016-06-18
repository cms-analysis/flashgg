#ifndef flashgg_VBFDiPhoDiJetMVAResultDumpers_h
#define flashgg_VBFDiPhoDiJetMVAResultDumpers_h
#include "flashgg/DataFormats/interface/VBFDiPhoDiJetMVAResult.h"
#include "flashgg/Taggers/interface/CollectionDumper.h"
namespace flashgg {
    typedef CollectionDumper<std::vector<VBFDiPhoDiJetMVAResult> > VBFDiPhoDiJetMVAResultDumper;
    typedef CollectionDumper<std::vector<VBFDiPhoDiJetMVAResult> ,
            VBFDiPhoDiJetMVAResult,
            CutBasedClassifier<VBFDiPhoDiJetMVAResult> > CutBasedVBFDiPhoDiJetMVAResultDumper;
}
#endif
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

