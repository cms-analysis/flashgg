#ifndef DiPhotonMVAResultDumper_h
#define DiPhotonMVAResultDumper_h

#include "flashgg/DataFormats/interface/DiPhotonMVAResult.h"
#include "flashgg/Taggers/interface/CollectionDumper.h"

namespace flashgg {
    typedef CollectionDumper<std::vector<DiPhotonMVAResult>> DiPhotonMVAResultDumper;
    typedef CollectionDumper<std::vector<DiPhotonMVAResult>, DiPhotonMVAResult, CutBasedClassifier<DiPhotonMVAResult>> CutBasedDiPhotonMVAResultDumper;
}

#endif
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

