#ifndef flashgg_H4GCandidateDumper_h
#define flashgg_H4GCandidateDumper_h

#include "flashgg/DataFormats/interface/H4GCandidate.h"

#include "flashgg/Taggers/interface/CollectionDumper.h"

namespace flashgg
{
  typedef CollectionDumper<std::vector<H4GCandidate> > H4GCandidateDumper;
  typedef CollectionDumper<std::vector<H4GCandidate>,
    H4GCandidate,
    CutBasedClassifier<H4GCandidate> > CutBasedH4GCandidateDumper;
}

#endif

// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4
