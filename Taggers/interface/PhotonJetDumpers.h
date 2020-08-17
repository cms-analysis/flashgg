#ifndef flashgg_PhotonJetDumpers_h
#define flashgg_PhotonJetDumpers_h

#include "flashgg/DataFormats/interface/PhotonJetCandidate.h"

#include "flashgg/Taggers/interface/CollectionDumper.h"


namespace flashgg {

    typedef CollectionDumper<std::vector<PhotonJetCandidate> > PhotonJetDumper;
    typedef CollectionDumper<std::vector<PhotonJetCandidate>,
            PhotonJetCandidate,
            CutBasedClassifier<PhotonJetCandidate> > CutBasedPhotonJetDumper;

}

#endif

// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4
