#include "flashgg/MicroAODFormats/interface/Photon.h"

using namespace flashgg;

Photon::Photon() {
  sipip = 0.;
  sieip = 0.;
  zernike20 = 0.;
  zernike42 = 0.;
  e2nd = 0.;
  e2x5right = 0.;
  e2x5left = 0.;
  e2x5top = 0.;
  e2x5bottom = 0.;
  e2x5max = 0.;
  eright = 0.;
  eleft = 0.;
  etop = 0.;
  ebottom = 0.;
  e1x3 = 0.;
  S4_ = 0.;
  pfPhoIso04_ = 0.;
  pfPhoIso03_ = 0.;
  pfChgIsoWrtWorstVtx04_ = 0.;
  pfChgIsoWrtWorstVtx03_ = 0.;
  pfChgIsoWrtChosenVtx02_ = 0.;
  ESEffSigmaRR_ = 0.;
  sigEOverE_ = 0.;
  pfChgIso03_.clear();
  pfChgIso02_.clear();
  phoIdMvaD_.clear();
}
Photon::Photon(const pat::Photon& aPhoton ) : pat::Photon(aPhoton) {}
Photon::~Photon() {}

Photon* Photon::clone() const { return new Photon(*this); }

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
