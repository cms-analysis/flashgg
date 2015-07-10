#include "FWCore/Framework/interface/MakerMacros.h"
#include "CommonTools/UtilAlgos/interface/Merger.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "flashgg/DataFormats/interface/DiPhotonTagBase.h"

typedef Merger<edm::OwnVector<flashgg::DiPhotonTagBase> > TagMerger;

DEFINE_FWK_MODULE( TagMerger );

// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4
