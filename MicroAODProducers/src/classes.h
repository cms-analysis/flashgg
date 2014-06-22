#include "DataFormats/Common/interface/Wrapper.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/Common/interface/AssociationMap.h"
#include "DataFormats/PatCandidates/interface/PackedCandidate.h"

namespace  { struct dictionary {
  edm::AssociationMap<edm::OneToMany<std::vector<reco::Vertex>,std::vector<pat::PackedCandidate>,unsigned int> > dummy0;
  edm::Wrapper<edm::AssociationMap<edm::OneToMany<std::vector<reco::Vertex>,std::vector<pat::PackedCandidate>,unsigned int> > > dummy1;
};}
