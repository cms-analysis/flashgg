#ifndef FLASHgg_DiPhotonCandidate_h
#define FLASHgg_DiPhotonCandidate_h

#include "DataFormats/Candidate/interface/CompositeCandidate.h"
#include "flashgg/MicroAODFormats/interface/Photon.h"
#include "DataFormats/VertexReco/interface/Vertex.h"


namespace flashgg {
  class DiPhotonCandidate : public reco::CompositeCandidate {
  public:
    DiPhotonCandidate();
    DiPhotonCandidate(edm::Ptr<flashgg::Photon>,edm::Ptr<flashgg::Photon>,edm::Ptr<reco::Vertex>);
    DiPhotonCandidate(const flashgg::Photon&, const flashgg::Photon &,edm::Ptr<reco::Vertex>);
    ~DiPhotonCandidate();

    const edm::Ptr<reco::Vertex> getVertex() const { return vertex_; }
    const flashgg::Photon* leadingPhoton() const;
    const flashgg::Photon* subLeadingPhoton() const;

    void setLogSumPt2(float val) { logsumpt2_ = val; }
    void setPtBal(float val) { ptbal_ = val; }
    void setPtAsym(float val) { ptasym_ = val; }
    void setNConv(float val) { nConv_ = val; }
    void setPullConv(float val) { pull_conv_ = val; }
    void setNVert(float val) { nVert_ =val; }
    void setMVA0(float val) { MVA0_ = val; }
    void setMVA1(float val) { MVA1_ = val; }
    void setMVA2(float val) { MVA2_ = val; }
    void setDZ1(float val) { dZ1_ = val; }
    void setDZ2(float val) { dZ2_ = val; }
    void setVtxProbMVA(float val) { vtxprobmva_ = val; }
    void setVertex_index(int val) { vertex_index_ = val; }
    
    float getLogSumPt2() const { return logsumpt2_; }
    float getPtBal() const { return ptbal_; }
    float getPtAsym() const { return ptasym_; }
    float getNConv() const { return nConv_; }
    float getPullConv() const { return pull_conv_; }
    float getNVert() const { return nVert_; }
    float getMVA0() const { return MVA0_; }
    float getMVA1() const { return MVA1_; }
    float getMVA2() const { return MVA2_; }
    float getDZ1() const { return dZ1_; }
    float getDZ2() const { return dZ2_; }
    float getVtxProbMVA() const { return vtxprobmva_; }
		float getSumPt() const {
		return (this->leadingPhoton()->pt() + this->subLeadingPhoton()->pt()); 
		}
    int vertex_index() const { return vertex_index_; }


  private:
    edm::Ptr<reco::Vertex> vertex_;
    int vertex_index_;

    float logsumpt2_;
    float ptbal_;
    float ptasym_;
    float nConv_;
    float pull_conv_;
    float nVert_;
    float MVA0_;
    float MVA1_;
    float MVA2_;
    float dZ1_;
    float dZ2_;
    float vtxprobmva_;
  };


}


#endif
