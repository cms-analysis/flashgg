#include "flashgg/MicroAODFormats/interface/DiPhotonCandidate.h"
#include "flashgg/MicroAODFormats/interface/Photon.h"
#include "CommonTools/CandUtils/interface/AddFourMomenta.h"

using namespace flashgg;

DiPhotonCandidate::DiPhotonCandidate(){}

DiPhotonCandidate::~DiPhotonCandidate(){}

DiPhotonCandidate::DiPhotonCandidate(edm::Ptr<flashgg::Photon> photon1,edm::Ptr<flashgg::Photon> photon2, edm::Ptr<reco::Vertex> vertex) {
  addDaughter(*photon1);
  addDaughter(*photon2);
  vertex_ = vertex;

  // Adding momenta
  // Needs its own object - but why?
  // Copied from example
  AddFourMomenta addP4;
  addP4.set(*this);
}

const flashgg::Photon * DiPhotonCandidate::leadingPhoton() const {
  if (daughter(0)->pt() > daughter(1)->pt()) {
    return (const flashgg::Photon*) daughter(0);
  } else {
    return (const flashgg::Photon*) daughter(1);
  }
}

const flashgg::Photon * DiPhotonCandidate::subLeadingPhoton() const {
  if (daughter(0)->pt() > daughter(1)->pt()) {
    return (const flashgg::Photon*) daughter(1);
  } else {
    return (const flashgg::Photon*) daughter(0);
  }
}
