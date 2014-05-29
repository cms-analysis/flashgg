#include "flashgg/MicroAODFormats/interface/DiPhotonCandidate.h"
#include "flashgg/MicroAODFormats/interface/Photon.h"
#include "CommonTools/CandUtils/interface/AddFourMomenta.h"

//using namespace edm;
//using namespace std;
//using namespace reco;
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

const flashgg::Photon & DiPhotonCandidate::getPhoton(size_type i) const {
  return static_cast<const flashgg::Photon&>(*daughter(i));
}

