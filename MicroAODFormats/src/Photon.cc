#include "flashgg/MicroAODFormats/interface/Photon.h"
#include "FWCore/Utilities/interface/Exception.h"

using namespace flashgg;

Photon::Photon() {
  sipip_ = 0.;
  sieip_ = 0.;
  zernike20_ = 0.;
  zernike42_ = 0.;
  e2nd_ = 0.;
  e2x5right_ = 0.;
  e2x5left_ = 0.;
  e2x5top_ = 0.;
  e2x5bottom_ = 0.;
  e2x5max_ = 0.;
  eright_ = 0.;
  eleft_ = 0.;
  etop_ = 0.;
  ebottom_ = 0.;
  e1x3_ = 0.;
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


float const Photon::findVertexFloat( const edm::Ptr<reco::Vertex>& vtx, const std::map<edm::Ptr<reco::Vertex>,float> & mp, bool lazy) const 
{
	lazy = lazy && (vtx.id() == edm::ProductID(0,0));
	for(std::map<edm::Ptr<reco::Vertex>,float>::const_iterator it=mp.begin(); it!=mp.end(); ++it) {
		if( (lazy && it->first.key() == vtx.key()) || it->first == vtx  ) { 
			return  it->second;
		}
	}
	
	throw cms::Exception("Missing Data") << "could not find value for vertex " << vtx.key() << " " << vtx.id() << " lazy search: "<< lazy <<  "\n";;
	
	return 0.;
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
