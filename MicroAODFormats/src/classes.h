#include "flashgg/MicroAODFormats/interface/Photon.h"
#include "flashgg/MicroAODFormats/interface/DiPhotonCandidate.h"
#include "flashgg/MicroAODFormats/interface/Jet.h"
#include "DataFormats/Common/interface/Wrapper.h"
#include "DataFormats/VertexReco/interface/Vertex.h"

namespace  { struct dictionary {
  std::vector<flashgg::Photon> dummy0;
  edm::Wrapper<std::vector<flashgg::Photon> > dummy1;
  std::vector<flashgg::DiPhotonCandidate> dummy2;
  edm::Wrapper<std::vector<flashgg::DiPhotonCandidate> > dummy3;
  edm::Ptr<reco::Vertex> dummy4;
  std::vector<flashgg::Jet> dummy5;
  edm::Wrapper<std::vector<flashgg::Jet> > dummy6;
  std::map<edm::Ptr<reco::Vertex>,float> dummy7;
};}
