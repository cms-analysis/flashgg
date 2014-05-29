#include "flashgg/MicroAODFormats/interface/Photon.h"

using namespace flashgg;

Photon::Photon() : pat::Photon() {
  testVariable_ = 0;
}

Photon::Photon(const pat::Photon& aPhoton ) : pat::Photon(aPhoton) {
  testVariable_ = 0;
}

Photon::~Photon() {}
