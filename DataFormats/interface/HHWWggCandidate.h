#ifndef flashgg_HHWWggCandidate
#define flashgg_HHWWggCandidate

// https://root.cern.ch/doc/v608/TLorentzVector_8h_source.html
#include "TLorentzVector.h"
#include "DataFormats/Math/interface/LorentzVector.h"

#include "flashgg/DataFormats/interface/DiPhotonCandidate.h"
#include "flashgg/DataFormats/interface/Photon.h"
#include "flashgg/DataFormats/interface/SinglePhotonView.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "flashgg/DataFormats/interface/Jet.h"
#include "DataFormats/Candidate/interface/LeafCandidate.h"
#include "DataFormats/Math/interface/deltaR.h"
#include "flashgg/DataFormats/interface/Electron.h"
#include "flashgg/DataFormats/interface/Muon.h"
#include "flashgg/DataFormats/interface/Met.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"

#include "flashgg/Taggers/interface/FunctionHelpers.h"

namespace flashgg {

  // HHWWggCandidate is a sub class or derived class of WeightedObject 
  class HHWWggCandidate : public WeightedObject
  {
  // access specifier 
  public:
    //---ctors--- 
    // when constructor overloading, each must have different number or specific types of input variables 
    HHWWggCandidate() ;
    HHWWggCandidate(std::vector<flashgg::DiPhotonCandidate> diphoVector,
                    std::vector<flashgg::Electron> electronVector, std::vector<flashgg::Muon> muonVector, std::vector<flashgg::Met> METVector,
                    std::vector<reco::GenParticle> GenParticlesVector, std::vector<flashgg::Jet> JetVector,
                    std::vector<double> Cut_Variables, double dipho_MVA, double lead_pho_MVA, double sublead_pho_MVA);     
    
    //---dtor---
    ~HHWWggCandidate();

    //---utils---
    const std::vector<flashgg::DiPhotonCandidate> diphoVector() const { return diphoVector_; };
    const std::vector<flashgg::Electron> electronVector() const {return electronVector_;} 
    const std::vector<flashgg::Muon> muonVector() const {return muonVector_;}
    const std::vector<flashgg::Met> METVector() const {return METVector_;}
    const std::vector<reco::GenParticle> GenParticlesVector() const {return GenParticlesVector_;}
    const std::vector<flashgg::Jet> JetVector() const {return JetVector_;}
    const std::vector<flashgg::Photon> phoP4Corrected() const { return phoP4Corrected_; };
    const reco::Candidate::LorentzVector& MET_fourvec() const { return MET_fourvec_; };
    const reco::Candidate::LorentzVector& leading_dpho() const { return leading_dpho_; };
    const reco::Candidate::LorentzVector& leading_pho() const { return leading_pho_; };
    const reco::Candidate::LorentzVector& sub_leading_pho() const { return sub_leading_pho_; };
    const reco::Candidate::LorentzVector& leading_elec() const { return leading_elec_; };
    const reco::Candidate::LorentzVector& subleading_elec() const { return subleading_elec_; };
    const reco::Candidate::LorentzVector& leading_muon() const { return leading_muon_; };
    const reco::Candidate::LorentzVector& subleading_muon() const { return subleading_muon_; };
    const reco::Candidate::LorentzVector& muon1() const { return muon1_; };
    const reco::Candidate::LorentzVector& HHWWggDiPho1() const { return dp1_; };
    const reco::Candidate::LorentzVector& HHWWggDiPho2() const { return dp2_; };
    const reco::Candidate::LorentzVector& HHWWggFourVect() const { return tp_; };     
    const reco::Candidate::LorentzVector& MatchingDiJet() const { return mdij_; }; 
    const reco::Candidate::LorentzVector& NonMatchingDiJet() const { return nmdij_; }; 
    const reco::Candidate::LorentzVector& MatchingDiQuark() const { return mdiq_; }; 
    const reco::Candidate::LorentzVector& NonMatchingDiQuark() const { return nmdiq_; }; 
    //float getCosThetaStar_CS(float ebeam) const;
    //std::vector<float> CosThetaAngles() const;
    //float HelicityCosTheta( TLorentzVector Booster, TLorentzVector Boosted) const;
    const float theMETcorpt() const { return theMETcorpt_; };
    const float W1_TM() const { return W1_TM_; };
    const reco::Candidate::LorentzVector& gen_leading_elec() const { return gen_leading_elec_; };
    const reco::Candidate::LorentzVector& gen_subleading_elec() const { return gen_subleading_elec_; };
    const reco::Candidate::LorentzVector& gen_leading_muon() const { return gen_leading_muon_; };
    const reco::Candidate::LorentzVector& gen_subleading_muon() const { return gen_subleading_muon_; };
    bool test() const { return test_; };
    bool SLW_tag() const { return SLW_tag_; }; 
    bool Pass_PS() const { return Pass_PS_; }; 
    const std::vector<double> Cut_Variables() const { return Cut_Variables_; };
    const reco::Candidate::LorentzVector& lsl_dij() const { return lsl_dij_; };
    const float m_qq() const {return m_qq_;};
    const float dr_1() const {return dr_1_;};
    const float dr_2() const {return dr_2_;};
    const float merged_qs() const {return merged_qs_;};
    const float qone_matches() const {return qone_matches_;};
    const float qtwo_matches() const {return qtwo_matches_;};
    const reco::Candidate::LorentzVector& hgg_p4() const { return hgg_p4_; }; 
    const reco::Candidate::LorentzVector& l_genpho() const { return l_genpho_; }; 
    const reco::Candidate::LorentzVector& sl_genpho() const { return sl_genpho_; }; 
    // const reco::Candidate::LorentzVector& leading_lepton() const { return leading_lepton_; }; 
    const float SL_mT_WW() const { return SL_mT_WW_; };
    const float FL_mT_WW() const { return FL_mT_WW_; };
    const float FH_m_WW() const { return FH_m_WW_; };   
    const float SL_mT_W() const { return SL_mT_W_; };   
    const float dipho_MVA() const {return dipho_MVA_;};
    const float lead_pho_MVA() const {return lead_pho_MVA_;};
    const float sublead_pho_MVA() const {return sublead_pho_MVA_;};
  private:

    std::vector<flashgg::DiPhotonCandidate> diphoVector_;
    std::vector<flashgg::Electron> electronVector_;
    std::vector<flashgg::Muon> muonVector_;
    std::vector<flashgg::Met> METVector_;
    std::vector<reco::GenParticle> GenParticlesVector_;
    std::vector<flashgg::Jet> JetVector_;
    std::vector<flashgg::Photon> phoP4Corrected_;
    reco::Candidate::LorentzVector MET_fourvec_;
    reco::Candidate::LorentzVector leading_dpho_;
    reco::Candidate::LorentzVector leading_pho_;
    reco::Candidate::LorentzVector sub_leading_pho_;
    reco::Candidate::LorentzVector leading_elec_;
    reco::Candidate::LorentzVector subleading_elec_;
    reco::Candidate::LorentzVector leading_muon_;
    reco::Candidate::LorentzVector subleading_muon_;
    reco::Candidate::LorentzVector muon1_;
    reco::Candidate::LorentzVector dp1_;
    reco::Candidate::LorentzVector dp2_;
    reco::Candidate::LorentzVector tp_;
    reco::Candidate::LorentzVector mdij_;
    reco::Candidate::LorentzVector nmdij_;
    reco::Candidate::LorentzVector mdiq_;
    reco::Candidate::LorentzVector nmdiq_;
    float theMETcorpt_;
    float W1_TM_;
    reco::Candidate::LorentzVector gen_leading_elec_;
    reco::Candidate::LorentzVector gen_subleading_elec_;
    reco::Candidate::LorentzVector gen_leading_muon_;
    reco::Candidate::LorentzVector gen_subleading_muon_;
    bool test_;
    bool SLW_tag_;
    bool Pass_PS_;
    std::vector<double> Cut_Variables_;
    reco::Candidate::LorentzVector lsl_dij_;
    float m_qq_;
    float dr_1_;
    float dr_2_;
    float merged_qs_;
    float qone_matches_;
    float qtwo_matches_;
    reco::Candidate::LorentzVector hgg_p4_;
    reco::Candidate::LorentzVector l_genpho_;
    reco::Candidate::LorentzVector sl_genpho_;
    // reco::Candidate::LorentzVector leading_lepton_;
    float SL_mT_WW_;
    float FL_mT_WW_;
    float FH_m_WW_;
    float SL_mT_W_;
    float dipho_MVA_;
    float lead_pho_MVA_;
    float sublead_pho_MVA_;
    
  };
  typedef std::vector<HHWWggCandidate> HHWWggCandidateCollection; // define new type: vector of HHWWggCandidates 

}

#endif
