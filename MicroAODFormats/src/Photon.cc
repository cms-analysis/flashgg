#include "flashgg/MicroAODFormats/interface/Photon.h"

using namespace flashgg;

Photon::Photon() {}
Photon::Photon(const pat::Photon& aPhoton ) : pat::Photon(aPhoton) {}
Photon::~Photon() {}

void setChargedPFIso02(const edm::PtrVector<pat::PackedCandidate>& pfcandidatePointers, const edm::Ptr<reco::Vertex>& vertex){
  return;
}
void setChargedPFIso02(const edm::PtrVector<pat::PackedCandidate>& pfcandidatePointers, const edm::PtrVector<reco::Vertex>& vertex){

  return;
}

