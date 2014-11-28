#ifndef FLASHgg_Photon_h
#define FLASHgg_Photon_h

#include "DataFormats/PatCandidates/interface/Photon.h"

namespace flashgg {

  class Photon : public pat::Photon {

  public:
    Photon();
    Photon(const pat::Photon &);
    ~Photon();
    virtual Photon * clone() const;

    
    // someone had the insane idea of shadowing these methods in the pat::Photon....
    float egChargedHadronIso() const {return  reco::Photon::chargedHadronIso();}
    float egNeutralHadronIso() const {return  reco::Photon::neutralHadronIso();}
    float egPhotonIso() const {return  reco::Photon::photonIso();}
        
    void setSipip(float val) {sipip_=val;};
    void setSieip(float val) {sieip_=val;};
    void setE2nd(float val) {e2nd_=val;};
    void setE2x5right(float val) {e2x5right_=val;};
    void setE2x5left(float val) {e2x5left_=val;};
    void setE2x5top(float val) {e2x5top_=val;};
    void setE2x5bottom(float val) {e2x5bottom_=val;};
    void setE2x5max(float val) {e2x5max_=val;};
    void setEright(float val) {eright_=val;};
    void setEleft(float val) {eleft_=val;};
    void setEtop(float val) {etop_=val;};
    void setEbottom(float val) {ebottom_=val;};
    void setE1x3(float val) {e1x3_=val;};
    void setS4(float val) {S4_=val;};  
    void setpfPhoIso04(float val) {pfPhoIso04_=val;};   
    void setpfPhoIso03(float val) {pfPhoIso03_=val;};   
    void setpfNeutIso04(float val) {pfNeutIso04_=val;};   
    void setpfNeutIso03(float val) {pfNeutIso03_=val;};   
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

    float const getSipip() const {return sipip_;};
    float const getSieip() const {return sieip_;};
    float const getE2nd() const {return e2nd_;};
    float const getE2x5right() const {return e2x5right_;};
    float const getE2x5left() const {return e2x5left_;};
    float const getE2x5top() const {return e2x5top_;};
    float const getE2x5bottom() const {return e2x5bottom_;};
    float const getE2x5max() const {return e2x5max_;};
    float const getEright() const {return eright_;};
    float const getEleft() const {return eleft_;};
    float const getEtop() const {return etop_;};
    float const getEbottom() const {return ebottom_;};
    float const getE1x3() const {return e1x3_;};
    float const getS4() const {return S4_;};   
    float const getpfPhoIso04() const {return pfPhoIso04_;};   
    float const getpfPhoIso03() const {return pfPhoIso03_;};   
    std::map<edm::Ptr<reco::Vertex>,float> const getpfChgIso04() const {return pfChgIso04_;};
    std::map<edm::Ptr<reco::Vertex>,float> const getpfChgIso03() const {return pfChgIso03_;};
    std::map<edm::Ptr<reco::Vertex>,float> const getpfChgIso02() const {return pfChgIso02_;};
    float const getpfChgIso04WrtVtx( const edm::Ptr<reco::Vertex>& vtx, bool lazy=false ) const { return findVertexFloat(vtx,pfChgIso04_,lazy); }; // if lazy flag is true only compare key (needed since fwlite does not fill provenance info)
    float const getpfChgIso03WrtVtx( const edm::Ptr<reco::Vertex>& vtx, bool lazy=false ) const { return findVertexFloat(vtx,pfChgIso03_,lazy); }; // if lazy flag is true only compare key (needed since fwlite does not fill provenance info)
    float const getpfChgIso02WrtVtx( const edm::Ptr<reco::Vertex>& vtx, bool lazy=false ) const { return findVertexFloat(vtx,pfChgIso02_,lazy); }; // if lazy flag is true only compare key (needed since fwlite does not fill provenance info)
    float const getpfChgIsoWrtWorstVtx04() const {return pfChgIsoWrtWorstVtx04_;};
    float const getpfChgIsoWrtWorstVtx03() const {return pfChgIsoWrtWorstVtx03_;};
    float const getpfChgIsoWrtChosenVtx02() const {return pfChgIsoWrtChosenVtx02_;};
    float const getESEffSigmaRR() const {return ESEffSigmaRR_;};

    bool hasEnergyAtStep(std::string key) const;
    float const getEnergyAtStep(std::string key) const;
    float const getSigEOverE() const { return sigEOverE_; };
    
    std::map<edm::Ptr<reco::Vertex>,float> const getPhoIdMvaD() const {return phoIdMvaD_;};
    float const getPhoIdMvaDWrtVtx( const edm::Ptr<reco::Vertex>& vtx, bool lazy=false ) const { return findVertexFloat(vtx,phoIdMvaD_,lazy); }; // if lazy flag is true only compare key (needed since fwlite does not fill provenance info)

  private:
    float const findVertexFloat(const edm::Ptr<reco::Vertex>& vtx, const std::map<edm::Ptr<reco::Vertex>,float> & mp, bool lazy) const;
    
    float sipip_;
    float sieip_;
    float zernike20_; 
    float zernike42_;
    float e2nd_;
    float e2x5right_;
    float e2x5left_;
    float e2x5top_;
    float e2x5bottom_;
    float e2x5max_;
    float eright_;
    float eleft_;
    float etop_;
    float ebottom_;
    float e1x3_;
    float S4_;   
    float pfPhoIso04_;
    float pfPhoIso03_;
    float pfNeutIso04_;
    float pfNeutIso03_;
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
