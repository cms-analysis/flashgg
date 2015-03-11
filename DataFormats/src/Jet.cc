#include "flashgg/MicroAODFormats/interface/Jet.h"

using namespace flashgg;

Jet::Jet() : pat::Jet() {
  puJetId_.clear();
}

Jet::Jet(const pat::Jet& aJet ) : pat::Jet(aJet) {
}

Jet::~Jet() {}

void Jet::setPuJetId(const edm::Ptr<reco::Vertex> vtx, const PileupJetIdentifier & id) {
  MinimalPileupJetIdentifier min_id;
  min_id.RMS = id.RMS();
  min_id.betaStar = id.betaStar();
  min_id.idFlag = id.idFlag();
  puJetId_.insert(std::make_pair(vtx,min_id));
}

bool Jet::hasPuJetId(const edm::Ptr<reco::Vertex> vtx) const {
  return (puJetId_.count(vtx) > 0);
}

bool Jet::passesPuJetId(const edm::Ptr<reco::Vertex> vtx, PileupJetIdentifier::Id level) const {
  assert (hasPuJetId(vtx));
  return PileupJetIdentifier::passJetId(puJetId_.at(vtx).idFlag,level);
}

float Jet::RMS(const edm::Ptr<reco::Vertex> vtx) const {
  assert (hasPuJetId(vtx));
  return puJetId_.at(vtx).RMS;
}

float Jet::betaStar(const edm::Ptr<reco::Vertex> vtx) const {
  assert (hasPuJetId(vtx));
  return puJetId_.at(vtx).betaStar;
}

bool Jet::passesPuJetId(const edm::Ptr<DiPhotonCandidate> dipho, PileupJetIdentifier::Id level) const {
  return passesPuJetId(dipho->getVertex());
}

float Jet::RMS(const edm::Ptr<DiPhotonCandidate> dipho) const {
  return RMS(dipho->getVertex());
}

float Jet::betaStar(const edm::Ptr<DiPhotonCandidate> dipho) const {
  return betaStar(dipho->getVertex());
}

