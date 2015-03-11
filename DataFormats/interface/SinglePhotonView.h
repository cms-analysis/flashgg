#ifndef FLASHgg_SinglePhotonView_h
#define FLASHgg_SinglePhotonView_h

#include "flashgg/DataFormats/interface/Photon.h"
#include "flashgg/DataFormats/interface/DiPhotonCandidate.h"
#include "DataFormats/Common/interface/Ptr.h"

#include <string>

namespace flashgg {
	
  class SinglePhotonView  {

  public:
	  typedef edm::Ptr<DiPhotonCandidate> dipho_ptr_type;
	  typedef Photon cand_type;
	  
	  SinglePhotonView() : pho_(0), dipho_(0) {}
	  SinglePhotonView(dipho_ptr_type dipho, int daughter) : edmdipho_(dipho), daughter_(daughter), pho_(0), dipho_(0) {}
	  SinglePhotonView(const DiPhotonCandidate * dipho, int daughter) : daughter_(daughter), pho_(0), dipho_(dipho) {}
	  
	  const cand_type& photon() const { getDaughterMaybe(); return *pho_; }
	  
	  const cand_type* operator->() const { getDaughterMaybe(); return pho_; }
	  
	  float pfChIso02WrtChosenVtx() const { return photon().getpfChgIso02WrtVtx(dipho_->getVertex()); }
	  float pfChIso03WrtChosenVtx() const { return photon().getpfChgIso03WrtVtx(dipho_->getVertex()); }
	  float pfChIso04WrtChosenVtx() const { return photon().getpfChgIso04WrtVtx(dipho_->getVertex()); }
	  					       
	  float phoIdMvaWrtChosenVtx() const { return photon().getPhoIdMvaDWrtVtx(dipho_->getVertex()); }
	  
  private:
	  void getDaughterMaybe() const;
	  
	  dipho_ptr_type edmdipho_;
	  int daughter_;
	  mutable const Photon* pho_;
	  mutable const DiPhotonCandidate * dipho_;

  };
}

#endif
