#include "flashgg/MicroAODFormats/interface/Photon.h"
#include "flashgg/MicroAODFormats/interface/DiPhotonCandidate.h"
#include "DataFormats/Common/interface/Wrapper.h"
#include "DataFormats/VertexReco/interface/Vertex.h"

namespace  { struct dictionary {
  std::vector<flashgg::Photon> dummy0;
  edm::Wrapper<std::vector<flashgg::Photon> > dummy1;
  std::vector<flashgg::DiPhotonCandidate> dummy2;
  edm::Wrapper<std::vector<flashgg::DiPhotonCandidate> > dummy3;
  edm::Ptr<reco::Vertex> dummy4;
};}
