#ifndef flashgg_VBFMVAResultDumpers_h
#define flashgg_VBFMVAResultDumpers_h
#include "flashgg/DataFormats/interface/VBFMVAResult.h"
#include "flashgg/Taggers/interface/CollectionDumper.h"
namespace flashgg {
    typedef CollectionDumper<std::vector<VBFMVAResult> > VBFMVAResultDumper;
    typedef CollectionDumper<std::vector<VBFMVAResult> ,
            VBFMVAResult,
            CutBasedClassifier<VBFMVAResult> > CutBasedVBFMVAResultDumper;
}
#endif
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

