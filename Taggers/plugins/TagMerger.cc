#include "FWCore/Framework/interface/MakerMacros.h"
#include "CommonTools/UtilAlgos/interface/Merger.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "flashgg/DataFormats/interface/DiPhotonTagBase.h"

typedef Merger<edm::OwnVector<flashgg::DiPhotonTagBase> > TagMerger;

DEFINE_FWK_MODULE( TagMerger );
