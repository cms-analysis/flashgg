#ifndef flashgg_H4GCandidate
#define flashgg_H4GCandidate

#include "TLorentzVector.h"
#include "DataFormats/Math/interface/LorentzVector.h"

#include "flashgg/DataFormats/interface/DiPhotonCandidate.h"
#include "flashgg/DataFormats/interface/Photon.h"
#include "flashgg/DataFormats/interface/Jet.h"
#include "DataFormats/Candidate/interface/LeafCandidate.h"
#include "DataFormats/Math/interface/deltaR.h"

#include "flashgg/Taggers/interface/FunctionHelpers.h"

namespace flashgg {

  class H4GCandidate : public WeightedObject
  {
  public:
    //---ctors---
    H4GCandidate() ;
    H4GCandidate( flashgg::Photon pho1, flashgg::Photon pho2, flashgg::Photon pho3, flashgg::Photon pho4, edm::Ptr<reco::Vertex> vertex);
    H4GCandidate( flashgg::Photon pho1, flashgg::Photon pho2, flashgg::Photon pho3, edm::Ptr<reco::Vertex> vertex);
    H4GCandidate( flashgg::Photon pho1, flashgg::Photon pho2, edm::Ptr<reco::Vertex> vertex);

    //---dtor---
    ~H4GCandidate();

    //---utils---
    // const int& h4g_npho() const { return n_photons; };
    const bool is4Photons() const { return n_photons==4; };
    const bool is3Photons() const { return n_photons==3; };
    const bool is2Photons() const { return n_photons==2; };
    const flashgg::Photon& h4gPho1() const { return pho1_; };
    const flashgg::Photon& h4gPho2() const { return pho2_; };
    const flashgg::Photon& h4gPho3() const { return pho3_; };
    const flashgg::Photon& h4gPho4() const { return pho4_; };
    const reco::Candidate::LorentzVector& h4gPho12() const { return pho12_; };
    const reco::Candidate::LorentzVector& h4gPho13() const { return pho13_; };
    const reco::Candidate::LorentzVector& h4gPho14() const { return pho14_; };
    const reco::Candidate::LorentzVector& h4gPho23() const { return pho23_; };
    const reco::Candidate::LorentzVector& h4gPho24() const { return pho24_; };
    const reco::Candidate::LorentzVector& h4gPho34() const { return pho34_; };
    const float& h4gPho1_r9() const { return pho1_r9_; };
    const float& h4gPho2_r9() const { return pho2_r9_; };
    const float& h4gPho3_r9() const { return pho3_r9_; };
    const float& h4gPho4_r9() const { return pho4_r9_; };
    const float& h4gPho1_full5x5_r9() const { return pho1_full5x5_r9_; };
    const float& h4gPho2_full5x5_r9() const { return pho2_full5x5_r9_; };
    const float& h4gPho3_full5x5_r9() const { return pho3_full5x5_r9_; };
    const float& h4gPho4_full5x5_r9() const { return pho4_full5x5_r9_; };
    const float& h4gPho1_EGMVA() const { return pho1_EGMVA_; };
    const float& h4gPho2_EGMVA() const { return pho2_EGMVA_; };
    const float& h4gPho3_EGMVA() const { return pho3_EGMVA_; };
    const float& h4gPho4_EGMVA() const { return pho4_EGMVA_; };
    const float& h4gPho1_MVA() const { return pho1_MVA_; };
    const float& h4gPho2_MVA() const { return pho2_MVA_; };
    const float& h4gPho3_MVA() const { return pho3_MVA_; };
    const float& h4gPho4_MVA() const { return pho4_MVA_; };
    const reco::Candidate::LorentzVector& h4gDiPho1() const { return dp1_; };
    const reco::Candidate::LorentzVector& h4gDiPho2() const { return dp2_; };
    const reco::Candidate::LorentzVector& h4gDiPho1_Pho1() const { return dp1_pho1_; };
    const reco::Candidate::LorentzVector& h4gDiPho1_Pho2() const { return dp1_pho2_; };
    const reco::Candidate::LorentzVector& h4gDiPho2_Pho1() const { return dp2_pho1_; };
    const reco::Candidate::LorentzVector& h4gDiPho2_Pho2() const { return dp2_pho2_; };
    const int& h4gDiPho1_iPho1() const { return dp1_ipho1_; };
    const int& h4gDiPho1_iPho2() const { return dp1_ipho2_; };
    const int& h4gDiPho2_iPho1() const { return dp2_ipho1_; };
    const int& h4gDiPho2_iPho2() const { return dp2_ipho2_; };
    const reco::Candidate::LorentzVector& h4gFourVect() const { return tp_; };
    float getCosThetaStar_CS(float ebeam) const;
    std::vector<float> CosThetaAngles() const;
    float HelicityCosTheta( TLorentzVector Booster, TLorentzVector Boosted) const;


  private:
    int n_photons;
    flashgg::Photon            pho1_;
    flashgg::Photon            pho2_;
    flashgg::Photon            pho3_;
    flashgg::Photon            pho4_;
    std::vector<flashgg::Photon> phoVec_;
    edm::Ptr<reco::Vertex>               vertex_;
    reco::Candidate::LorentzVector pho12_;
    reco::Candidate::LorentzVector pho13_;
    reco::Candidate::LorentzVector pho14_;
    reco::Candidate::LorentzVector pho23_;
    reco::Candidate::LorentzVector pho24_;
    reco::Candidate::LorentzVector pho34_;
    float pho1_r9_;
    float pho2_r9_;
    float pho3_r9_;
    float pho4_r9_;
    float pho1_full5x5_r9_;
    float pho2_full5x5_r9_;
    float pho3_full5x5_r9_;
    float pho4_full5x5_r9_;
    float pho1_EGMVA_;
    float pho2_EGMVA_;
    float pho3_EGMVA_;
    float pho4_EGMVA_;
    float pho1_MVA_;
    float pho2_MVA_;
    float pho3_MVA_;
    float pho4_MVA_;
    reco::Candidate::LorentzVector dp1_;
    reco::Candidate::LorentzVector dp2_;
    reco::Candidate::LorentzVector dp1_pho1_;
    reco::Candidate::LorentzVector dp1_pho2_;
    reco::Candidate::LorentzVector dp2_pho1_;
    reco::Candidate::LorentzVector dp2_pho2_;
    int dp1_ipho1_;
    int dp1_ipho2_;
    int dp2_ipho1_;
    int dp2_ipho2_;
    reco::Candidate::LorentzVector tp_;

  };
  typedef std::vector<H4GCandidate> H4GCandidateCollection;


}

#endif
