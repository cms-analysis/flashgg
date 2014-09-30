#ifndef FLASHgg_Photon_h
#define FLASHgg_Photon_h

#include "DataFormats/PatCandidates/interface/Photon.h"

namespace flashgg {

  class Photon : public pat::Photon {

  public:
    Photon();
    Photon(const pat::Photon &);
    ~Photon();
    virtual Photon * clone() const { return new Photon(*this); }
    
    void setSipip(float val) {sipip=val;};
    void setSieip(float val) {sieip=val;};
    void setE2nd(float val) {e2nd=val;};
    void setE2x5right(float val) {e2x5right=val;};
    void setE2x5left(float val) {e2x5left=val;};
    void setE2x5top(float val) {e2x5top=val;};
    void setE2x5bottom(float val) {e2x5bottom=val;};
    void setE2x5max(float val) {e2x5max=val;};
    void setEright(float val) {eright=val;};
    void setEleft(float val) {eleft=val;};
    void setEtop(float val) {etop=val;};
    void setEbottom(float val) {ebottom=val;};
    void setE1x3(float val) {e1x3= val;};
    void setS4(float val) {S4_=val;};  
    void setpfPhoIso04(float val) {pfPhoIso04_=val;};   
    void setpfPhoIso03(float val) {pfPhoIso03_=val;};   
    void setpfChgIso04( std::map<edm::Ptr<reco::Vertex>,float> valmap ) {  pfChgIso04_ = valmap; };  // concept: pass the pre-computed map when calling this in the producer
    void setpfChgIso03( std::map<edm::Ptr<reco::Vertex>,float> valmap ) {  pfChgIso03_ = valmap; };  // concept: pass the pre-computed map when calling this in the producer
    void setpfChgIso02( std::map<edm::Ptr<reco::Vertex>,float> valmap ) {  pfChgIso02_ = valmap; };  // concept: pass the pre-computed map when calling this in the producer
    void setpfChgIsoWrtWorstVtx04(float val) {pfChgIsoWrtWorstVtx04_=val;};
    void setpfChgIsoWrtWorstVtx03(float val) {pfChgIsoWrtWorstVtx03_=val;};
    void setpfChgIsoWrtChosenVtx02(float val) {pfChgIsoWrtChosenVtx02_=val;};
    void setESEffSigmaRR(float val) {ESEffSigmaRR_=val;};  
    void setPhoIdMvaD( std::map<edm::Ptr<reco::Vertex>,float> valmap ) {  phoIdMvaD_ = valmap; };   // concept: pass the pre-computed map when calling this in the producer
    void setEnergyAtStep(std::string key,float val);
    void updateEnergy(std::string key,float val);
    void setSigEOverE(float val) { sigEOverE_ = val; };

    float const getSipip() const {return sipip;};
    float const getSieip() const {return sieip;};
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
    float const getS4() const {return S4_;};   
    float const getpfPhoIso04() const {return pfPhoIso04_;};   
    float const getpfPhoIso03() const {return pfPhoIso03_;};   
    std::map<edm::Ptr<reco::Vertex>,float> const getpfChgIso03() const {return pfChgIso03_;}; 
    std::map<edm::Ptr<reco::Vertex>,float> const getpfChgIso02() const {return pfChgIso02_;}; 
    float const getpfChgIso03WrtVtx( const edm::Ptr<reco::Vertex>& vtx ) const {return pfChgIso03_.at(vtx);};
    float const getpfChgIso02WrtVtx( const edm::Ptr<reco::Vertex>& vtx ) const {return pfChgIso02_.at(vtx);};
    float const getpfChgIsoWrtWorstVtx04() const {return pfChgIsoWrtWorstVtx04_;};
    float const getpfChgIsoWrtWorstVtx03() const {return pfChgIsoWrtWorstVtx03_;};
    float const getpfChgIsoWrtChosenVtx02() const {return pfChgIsoWrtChosenVtx02_;};
    float const getESEffSigmaRR() const {return ESEffSigmaRR_;};

    bool hasEnergyAtStep(std::string key) const;
    float const getEnergyAtStep(std::string key) const;
    float const getSigEOverE() const { return sigEOverE_; };
    
    std::map<edm::Ptr<reco::Vertex>,float> const getPhoIdMvaD() const {return phoIdMvaD_;};
    float const getPhoIdMvaDWrtVtx( const edm::Ptr<reco::Vertex>& vtx ) const {return phoIdMvaD_.at(vtx);};       

  private:
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
    float S4_;   
    float pfPhoIso04_;
    float pfPhoIso03_;
    float pfChgIsoWrtWorstVtx04_;
    float pfChgIsoWrtWorstVtx03_;
    float pfChgIsoWrtChosenVtx02_;
    float ESEffSigmaRR_;
    float sigEOverE_;
    std::map<edm::Ptr<reco::Vertex>,float> pfChgIso04_; 
    std::map<edm::Ptr<reco::Vertex>,float> pfChgIso03_; 
    std::map<edm::Ptr<reco::Vertex>,float> pfChgIso02_; 
    std::map<edm::Ptr<reco::Vertex>,float> phoIdMvaD_;
     };
}

#endif
