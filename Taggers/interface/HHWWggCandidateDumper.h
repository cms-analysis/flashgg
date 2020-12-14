#ifndef flashgg_HHWWggCandidateDumper_h
#define flashgg_HHWWggCandidateDumper_h

#include "flashgg/DataFormats/interface/HHWWggCandidate.h"

#include "flashgg/Taggers/interface/CollectionDumper.h"

namespace flashgg
{
  typedef CollectionDumper<std::vector<HHWWggCandidate> > HHWWggCandidateDumper;
  typedef CollectionDumper<std::vector<HHWWggCandidate>,
    HHWWggCandidate,
    CutBasedClassifier<HHWWggCandidate> > CutBasedHHWWggCandidateDumper;
}

#endif

// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4
