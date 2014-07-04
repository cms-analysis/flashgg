#include "flashgg/MicroAODFormats/interface/Photon.h"
#include "flashgg/MicroAODFormats/interface/DiPhotonCandidate.h"
#include "flashgg/MicroAODFormats/interface/Jet.h"
#include "DataFormats/Common/interface/Wrapper.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include <vector>
namespace  { struct dictionary {
  std::vector<flashgg::Photon> dummy0;
  const std::vector<flashgg::Photon>::const_iterator dummy1;
  edm::Wrapper<std::vector<flashgg::Photon> > dummy2;
  std::vector<flashgg::DiPhotonCandidate> dummy3;
  const std::vector<flashgg::DiPhotonCandidate>::const_iterator dummy4;
  edm::Wrapper<std::vector<flashgg::DiPhotonCandidate> > dummy5;
  edm::Ptr<reco::Vertex> dummy6;
  std::vector<flashgg::Jet> dummy7;
  edm::Wrapper<std::vector<flashgg::Jet> > dummy8;
  edm::PtrVector<pat::PackedCandidate> dummy9;
  std::map<edm::Ptr<reco::Vertex>,edm::PtrVector<pat::PackedCandidate> > dummy10;
  edm::Wrapper<std::map<edm::Ptr<reco::Vertex>,edm::PtrVector<pat::PackedCandidate> > > dummy11;
};}
