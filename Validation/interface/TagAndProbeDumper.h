#ifndef flashgg_TagAndProbeDumper_h
#define flashgg_TagAndProbeDumper_h

#include "flashgg/DataFormats/interface/TagAndProbeCandidate.h"

#include "flashgg/Taggers/interface/CollectionDumper.h"

namespace flashgg
{
    typedef CollectionDumper<std::vector<TagAndProbeCandidate> > TagAndProbeDumper;
    typedef CollectionDumper<std::vector<TagAndProbeCandidate>,
            TagAndProbeCandidate,
            CutBasedClassifier<TagAndProbeCandidate> > CutBasedTagAndProbeDumper;
}

#endif

// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4
