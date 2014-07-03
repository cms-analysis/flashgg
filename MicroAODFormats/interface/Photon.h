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
    void  setS4(float val) {S4_=val;};    // MM 
    void  setpfPhoIso03(float val) {pfPhoIso03_=val;};    // MM
    void  setpfChIso03(float val) {pfChIso03_=val;};      // MM
    void  setpfChIso03worst(float val) {pfChIso03worst_=val;};    // MM
    void  setRho(float val) {rho_=val;};    // MM
    void  setESEffSigmaRR_(float val) {ESEffSigmaRR_=val;};   // MM

    void  setPhoIdMvaD_(float val) {phoIdMvaD_=val;};   // MM


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

    float const getS4() const {return S4_;};   // MM
    float const getpfPhoIso03() const {return pfPhoIso03_;};   // why 0.3? do we want to keep this value?
    float const getpfChIso03() const {return pfChIso03_;};
    float const getpfChIso03worst() const {return pfChIso03worst_;};
    float const getRho() const {return rho_;};
    float const getESEffSigmaRR() const {return ESEffSigmaRR_;};

    float const getPhoIdMvaD() const {return phoIdMvaD_;};    // MM

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
    float S4_;    // MM
    float pfPhoIso03_;
    float pfChIso03_;
    float pfChIso03worst_;
    float rho_;
    float ESEffSigmaRR_;
    float phoIdMvaD_;

    
    };
}

#endif
