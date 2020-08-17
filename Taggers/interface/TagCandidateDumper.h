#ifndef flashgg_TagCandidateDumper_h
#define flashgg_TagCandidateDumper_h

#include "flashgg/DataFormats/interface/TagCandidate.h"
#include "flashgg/Taggers/interface/CollectionDumper.h"

namespace flashgg {
    typedef CollectionDumper<std::vector<TagCandidate>, TagCandidate,
                             CutBasedClassifier<TagCandidate> > CutBasedTagCandidateDumper;
}

#endif

// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4
