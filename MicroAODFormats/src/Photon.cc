#include "flashgg/MicroAODFormats/interface/Photon.h"

using namespace flashgg;

Photon::Photon() {}
Photon::Photon(const pat::Photon& aPhoton ) : pat::Photon(aPhoton) {}
Photon::~Photon() {}

// Very simple functions now, but we want to be smarter about them later
void Photon::setEnergyAtStep(std::string key, float val) {
  addUserFloat(key,val);
}
float const Photon::getEnergyAtStep(std::string key) const {
  return userFloat(key);
}
bool Photon::hasEnergyAtStep(std::string key) const {
  return hasUserData(key);
}

void Photon::updateEnergy(std::string key, float val) {

  // Current energy saved when updated, unless we're still at the initial step
  if (!hasEnergyAtStep("initial")) {
    setEnergyAtStep("initial",energy());
  }

  // Update new energy
  setEnergyAtStep(key,val);
  
  // Update 4-vector
  setP4((val/energy())*p4());
}
