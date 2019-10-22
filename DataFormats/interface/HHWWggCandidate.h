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
                    std::vector<double> Vertex_Variables, std::vector<double> Cut_Variables, double dipho_MVA, double lead_pho_Hgg_MVA, double sublead_pho_Hgg_MVA, double CMS_hgg_mass, double dZ);     
    
    //---dtor---
    ~HHWWggCandidate();

    //---utils---
    const std::vector<flashgg::DiPhotonCandidate> diphoVector() const { return diphoVector_; };
    const std::vector<flashgg::Electron> electronVector() const {return electronVector_;} 
    const std::vector<flashgg::Muon> muonVector() const {return muonVector_;}
    const std::vector<flashgg::Met> METVector() const {return METVector_;}
    const std::vector<reco::GenParticle> GenParticlesVector() const {return GenParticlesVector_;}
    const std::vector<flashgg::Jet> JetVector() const {return JetVector_;}
    // const std::vector<flashgg::Photon> Leading_Photon() const { return Leading_Photon_; };
    // const std::vector<flashgg::Photon> Subleading_Photon() const { return Subleading_Photon_; };
    const reco::Candidate::LorentzVector& MET_fourvec() const { return MET_fourvec_; };
    const reco::Candidate::LorentzVector& leading_dpho() const { return leading_dpho_; };
    const reco::Candidate::LorentzVector& leading_pho() const { return leading_pho_; };
    const reco::Candidate::LorentzVector& sub_leading_pho() const { return sub_leading_pho_; };
    const reco::Candidate::LorentzVector& leading_elec() const { return leading_elec_; };
    const reco::Candidate::LorentzVector& subleading_elec() const { return subleading_elec_; };
    const reco::Candidate::LorentzVector& leading_muon() const { return leading_muon_; };
    const reco::Candidate::LorentzVector& subleading_muon() const { return subleading_muon_; };
    //float getCosThetaStar_CS(float ebeam) const;
    //std::vector<float> CosThetaAngles() const;
    //float HelicityCosTheta( TLorentzVector Booster, TLorentzVector Boosted) const;
    const float theMETcorpt() const { return theMETcorpt_; };
    const reco::Candidate::LorentzVector& gen_leading_elec() const { return gen_leading_elec_; };
    const reco::Candidate::LorentzVector& gen_subleading_elec() const { return gen_subleading_elec_; };
    const reco::Candidate::LorentzVector& gen_leading_muon() const { return gen_leading_muon_; };
    const reco::Candidate::LorentzVector& gen_subleading_muon() const { return gen_subleading_muon_; };
    const std::vector<double> Vertex_Variables() const { return Vertex_Variables_; };
    const std::vector<double> Cut_Variables() const { return Cut_Variables_; };
    // const reco::Candidate::LorentzVector& leading_lepton() const { return leading_lepton_; }; 
    const float dipho_MVA() const {return dipho_MVA_;};
    const float lead_pho_Hgg_MVA() const {return lead_pho_Hgg_MVA_;};
    const float sublead_pho_Hgg_MVA() const {return sublead_pho_Hgg_MVA_;};
    const float lead_pho_EG_MVA() const {return lead_pho_EG_MVA_;};
    const float sublead_pho_EG_MVA() const {return sublead_pho_EG_MVA_;};
    const float lead_pho_passElectronVeto() const {return lead_pho_passElectronVeto_;};
    const float sublead_pho_passElectronVeto() const {return sublead_pho_passElectronVeto_;};
    const float lead_pho_hasPixelSeed() const {return lead_pho_hasPixelSeed_;};
    const float sublead_pho_hasPixelSeed() const {return sublead_pho_hasPixelSeed_;};
    const float CMS_hgg_mass() const {return CMS_hgg_mass_;};
    const float dZ() const {return dZ_;};
    const float LeadPhoInitEnergy() const {return LeadPhoInitEnergy_;};
    const float SubLeadPhoInitEnergy() const {return SubLeadPhoInitEnergy_;};

  private:

    std::vector<flashgg::DiPhotonCandidate> diphoVector_;
    std::vector<flashgg::Electron> electronVector_;
    std::vector<flashgg::Muon> muonVector_;
    std::vector<flashgg::Met> METVector_;
    std::vector<reco::GenParticle> GenParticlesVector_;
    std::vector<flashgg::Jet> JetVector_;
    // std::vector<flashgg::Photon> Leading_Photon_;
    // std::vector<flashgg::Photon> Subleading_Photon_;
    reco::Candidate::LorentzVector MET_fourvec_;
    reco::Candidate::LorentzVector leading_dpho_;
    reco::Candidate::LorentzVector leading_pho_;
    reco::Candidate::LorentzVector sub_leading_pho_;
    reco::Candidate::LorentzVector leading_elec_;
    reco::Candidate::LorentzVector subleading_elec_;
    reco::Candidate::LorentzVector leading_muon_;
    reco::Candidate::LorentzVector subleading_muon_;
    float theMETcorpt_;
    reco::Candidate::LorentzVector gen_leading_elec_;
    reco::Candidate::LorentzVector gen_subleading_elec_;
    reco::Candidate::LorentzVector gen_leading_muon_;
    reco::Candidate::LorentzVector gen_subleading_muon_;
    std::vector<double> Vertex_Variables_;
    std::vector<double> Cut_Variables_;
    // reco::Candidate::LorentzVector leading_lepton_;
    float dipho_MVA_;
    float lead_pho_Hgg_MVA_;
    float sublead_pho_Hgg_MVA_;
    float lead_pho_EG_MVA_;
    float sublead_pho_EG_MVA_;
    float lead_pho_passElectronVeto_;
    float sublead_pho_passElectronVeto_;
    float lead_pho_hasPixelSeed_;
    float sublead_pho_hasPixelSeed_;
    float CMS_hgg_mass_;
    float dZ_;
    float LeadPhoInitEnergy_;
    float SubLeadPhoInitEnergy_;
    
  };
  typedef std::vector<HHWWggCandidate> HHWWggCandidateCollection; // define new type: vector of HHWWggCandidates 

}

#endif
