#ifndef FLASHgg_SingleVertexView_h
#define FLASHgg_SingleVertexView_h

#include "flashgg/MicroAODFormats/interface/DiPhotonCandidate.h"
#include "DataFormats/Common/interface/Ptr.h"

#include <string>

namespace flashgg {
	
  class SingleVertexView  {

  public:
	  typedef edm::Ptr<DiPhotonCandidate> dipho_ptr_type;
	  typedef reco::Vertex cand_type;
	  
	  SingleVertexView() : isClosestToGen_(false), dZtoGen_(0.), vtx_(0), dipho_(0) {}
	  SingleVertexView(dipho_ptr_type dipho, int ivtx) : isClosestToGen_(false), dZtoGen_(0.), edmdipho_(dipho), ivtx_(ivtx), vtx_(0), dipho_(0) {}
	  SingleVertexView(const DiPhotonCandidate * dipho, int ivtx) : isClosestToGen_(false), dZtoGen_(0.), ivtx_(ivtx), vtx_(0), dipho_(dipho) {}
	  
	  const cand_type& pos() const { getVertexMaybe(); return *vtx_; }
	  
	  const cand_type* operator->() const { getVertexMaybe(); return vtx_; }
	  
          float logSumPt2() const { getVertexMaybe(); return dipho_->getLogSumPt2(ivtx_); } 
	  float ptBal() const { getVertexMaybe(); return dipho_->getPtBal(ivtx_);     }
	  float ptAsym() const { getVertexMaybe(); return dipho_->getPtAsym(ivtx_);    }
	  float nConv() const { getVertexMaybe(); return dipho_->getNConv(ivtx_);     }
	  float pullConv() const { getVertexMaybe(); return dipho_->getPullConv(ivtx_);  }
	  float mva() const { getVertexMaybe(); return dipho_->getMVA(ivtx_);       }
	  
	  bool isClosestToGen() const { return isClosestToGen_; }
	  float dZtoGen() const { return dZtoGen_; }
	  
	  void setDzToGen(float x) { dZtoGen_=x; };
	  void setIsClosestToGen(bool x=true) { isClosestToGen_=x; };
	  
  private:
	  void getVertexMaybe() const;
	  bool isClosestToGen_;
	  float dZtoGen_;
	  
	  dipho_ptr_type edmdipho_;
	  int ivtx_;
	  mutable const cand_type* vtx_;
	  mutable const DiPhotonCandidate * dipho_;

  };
}

#endif
