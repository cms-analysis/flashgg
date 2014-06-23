#include "DataFormats/Common/interface/Wrapper.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/Common/interface/AssociationMap.h"
#include "DataFormats/PatCandidates/interface/PackedCandidate.h"

namespace  { struct dictionary {
  edm::helpers::KeyVal<edm::RefProd<std::vector<reco::Vertex> >, edm::RefProd<std::vector<pat::PackedCandidate> > > dummy0;
  edm::AssociationMap<edm::OneToMany<std::vector<reco::Vertex>,std::vector<pat::PackedCandidate>,unsigned int> > dummy1;
  edm::Wrapper<edm::AssociationMap<edm::OneToMany<std::vector<reco::Vertex>,std::vector<pat::PackedCandidate>,unsigned int> > > dummy2;
};}
