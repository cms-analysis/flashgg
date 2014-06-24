#ifndef FLASHgg_Photon_h
#define FLASHgg_Photon_h

#include "DataFormats/PatCandidates/interface/Photon.h"

namespace flashgg {

  class Photon : public pat::Photon {

  public:
    Photon();
    Photon(const pat::Photon &);
    ~Photon();
    
    void  setSipip(float val) {sipip=val;};
    void  setSieip(float val) {sieip=val;};
    void  setZernike20(float val) {zernike20=val;};
    void  setZernike42(float val) {zernike42=val;};
    void  setE2nd(float val) {e2nd=val;};
    void  setE2x5right(float val) {e2x5right=val;};
    void  setE2x5left(float val) {e2x5left=val;};
    void  setE2x5top(float val) {e2x5top=val;};
    void  setE2x5bottom(float val) {e2x5bottom=val;};
    void  setE2x5max(float val) {e2x5max=val;};
    void  setEright(float val) {eright=val;};
    void  setEleft(float val) {eleft=val;};
    void  setEtop(float val) {etop=val;};
    void  setEbottom(float val) {ebottom=val;};
    void  setE1x3(float val) {e1x3= val;};
    void  setChargedPFIso02(const edm::PtrVector<pat::PackedCandidate>&, const edm::Ptr<reco::Vertex>&);
    void  setChargedPFIso02(const edm::PtrVector<pat::PackedCandidate>&, const edm::PtrVector<reco::Vertex>&);

    float const getSipip() const {return sipip;};
    float const getSieip() const {return sieip;};
    float const getZernike20() const {return zernike20;};
    float const getZernike42() const {return zernike42;};
    float const getE2nd() const {return e2nd;};
    float const getE2x5right() const {return e2x5right;};
    float const getE2x5left() const {return e2x5left;};
    float const getE2x5top() const {return e2x5top;};
    float const getE2x5bottom() const {return e2x5bottom;};
    float const getE2x5max() const {return e2x5max;};
    float const getEright() const {return eright;};
    float const getEleft() const {return eleft;};
    float const getEtop() const {return etop;};
    float const getEbottom() const {return ebottom;};
    float const getE1x3() const {return e1x3;};
    float const getChargedPFIso02(edm::Ptr<reco::Vertex> vtx) const {return ChargedPFIso02_.at(vtx);};

  private:
    unsigned int testVariable_;
    float sipip;
    float sieip;
    float zernike20; 
    float zernike42;
    float e2nd;
    float e2x5right;
    float e2x5left;
    float e2x5top;
    float e2x5bottom;
    float e2x5max;
    float eright;
    float eleft;
    float etop;
    float ebottom;
    float e1x3;
    std::map<edm::Ptr<reco::Vertex>, float> ChargedPFIso02_; // change to unordered_map later.  getting c++11 errors in 7_0_4


    };
}

#endif
