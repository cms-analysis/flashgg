#ifndef FLASHgg_VertexCandidateMap_h
#define FLASHgg_VertexCandidateMap_h

#include "DataFormats/Common/interface/Ptr.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/PatCandidates/interface/PackedCandidate.h"

namespace flashgg {
  typedef std::map<edm::Ptr<reco::Vertex>,edm::PtrVector<pat::PackedCandidate> > VertexCandidateMap;
}

#endif
