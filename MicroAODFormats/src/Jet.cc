#include "flashgg/MicroAODFormats/interface/Jet.h"

using namespace flashgg;

Jet::Jet() : pat::Jet() {
  PuJetId_.clear();
}

Jet::Jet(const pat::Jet& aJet ) : pat::Jet(aJet) {
}

Jet::~Jet() {}

float Jet::getPuJetId(const edm::Ptr<reco::Vertex> vtx) const {
  if (PuJetId_.count(vtx)) {
    return PuJetId_.at(vtx);
  }
  return -1.9;
}

float Jet::getPuJetId(const edm::Ptr<DiPhotonCandidate> dipho) const {
  return getPuJetId(dipho->getVertex());
}

void Jet::setPuJetId(const edm::Ptr<reco::Vertex> vtx, float val) {
  if (PuJetId_.count(vtx)) {
    PuJetId_[vtx] = val;
  } else {
    PuJetId_.insert(std::make_pair(vtx,val));
  }
}
