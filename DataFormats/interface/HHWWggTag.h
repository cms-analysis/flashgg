#ifndef flashgg_HHWWggTag
#define flashgg_HHWWggTag

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
#include "flashgg/DataFormats/interface/DiPhotonTagBase.h"

#include "flashgg/Taggers/interface/FunctionHelpers.h"

namespace flashgg {

  // HHWWggTag is a sub class or derived class of WeightedObject
  class HHWWggTag: public DiPhotonTagBase, public reco::LeafCandidate // inherit from tag producer
  {
  // access specifier
  public:
    //---ctors---
    // when constructor overloading, each must have different number or specific types of input variables
    HHWWggTag();

    //---dtor---
    ~HHWWggTag();

    //-- Utilities 
    void GetPhoAtt(edm::Ptr<DiPhotonCandidate> dipho);
    void GetObjects(edm::Ptr<DiPhotonCandidate> dipho);
    void GetObjects(edm::Ptr<DiPhotonCandidate> dipho, edm::Ptr<flashgg::Met>);
    void GetObjects(edm::Ptr<DiPhotonCandidate> dipho, edm::Ptr<flashgg::Electron> electron, edm::Ptr<flashgg::Met> MET, edm::Ptr<flashgg::Jet> jet1, edm::Ptr<flashgg::Jet> jet2);
    void GetObjects(edm::Ptr<DiPhotonCandidate> dipho, edm::Ptr<flashgg::Muon> muon, edm::Ptr<flashgg::Met> MET, edm::Ptr<flashgg::Jet> jet1, edm::Ptr<flashgg::Jet> jet2);
    void GetObjects(edm::Ptr<DiPhotonCandidate> dipho, edm::Ptr<flashgg::Met> MET, edm::Ptr<flashgg::Jet> jet1, edm::Ptr<flashgg::Jet> jet2, edm::Ptr<flashgg::Jet> jet3, edm::Ptr<flashgg::Jet> jet4);
    
    // Get Leptons and Jets
    std::vector<flashgg::Electron> GetElectrons(std::vector<edm::Ptr<flashgg::Electron>>);
    std::vector<flashgg::Muon> GetMuons(std::vector<edm::Ptr<flashgg::Muon>>);
    std::vector<flashgg::Jet> GetJets(std::vector<edm::Ptr<flashgg::Jet>>);

    //-- Fully Leptonic Leptons 
    void GetFLElectrons(edm::Ptr<flashgg::Electron> Ele1,edm::Ptr<flashgg::Electron> Ele2);
    void GetFLMuons(edm::Ptr<flashgg::Muon> muon1,edm::Ptr<flashgg::Muon> muon2);
    void GetDiffLeptons(edm::Ptr<flashgg::Electron> Ele,edm::Ptr<flashgg::Muon> muon);
    void GetMET(edm::Ptr<flashgg::Met> MET);

    //-- Final State Categories: HHWWggTag_X 

    //- Without cut flow analysis - Only need to save photons for CMS_hgg_mass in fggfinalfit 
    HHWWggTag(edm::Ptr<DiPhotonCandidate>);

    //- With Cutflow analysis - Save all final state objects 

    // HHWWggTag_0 - Semi Leptonic electron channel 
    HHWWggTag(edm::Ptr<DiPhotonCandidate>, edm::Ptr<flashgg::Electron>, std::vector<edm::Ptr<flashgg::Electron>>, std::vector<edm::Ptr<flashgg::Electron>>,
              std::vector<edm::Ptr<flashgg::Muon>>, edm::Ptr<flashgg::Met>, edm::Ptr<flashgg::Jet>, edm::Ptr<flashgg::Jet>,
              std::vector<edm::Ptr<flashgg::Jet>>, std::vector<edm::Ptr<flashgg::Jet>>,
              std::vector<double>, std::vector<double>, std::vector<double>);

    // HHWWggTag_1 - Semi Leptonic muon channel 
    HHWWggTag(edm::Ptr<DiPhotonCandidate>, std::vector<edm::Ptr<flashgg::Electron>>, edm::Ptr<flashgg::Muon>, std::vector<edm::Ptr<flashgg::Muon>>, std::vector<edm::Ptr<flashgg::Muon>>,
              edm::Ptr<flashgg::Met>, edm::Ptr<flashgg::Jet>, edm::Ptr<flashgg::Jet>,
              std::vector<edm::Ptr<flashgg::Jet>>, std::vector<edm::Ptr<flashgg::Jet>>,
              std::vector<double>, std::vector<double>, std::vector<double>);

    //-- 0 and 1 with just HH objects. Saving just in case 
    // HHWWggTag(edm::Ptr<DiPhotonCandidate>, edm::Ptr<flashgg::Electron>, edm::Ptr<flashgg::Met>, edm::Ptr<flashgg::Jet>, edm::Ptr<flashgg::Jet>); // HHWWggTag_0
    // HHWWggTag(edm::Ptr<DiPhotonCandidate>, edm::Ptr<flashgg::Muon>, edm::Ptr<flashgg::Met>, edm::Ptr<flashgg::Jet>, edm::Ptr<flashgg::Jet>); // HHWWggTag_1


    // HHWWggTag_2 - Fully Hadronic 
    HHWWggTag(edm::Ptr<DiPhotonCandidate>,
              edm::Ptr<flashgg::Met>,
              edm::Ptr<flashgg::Jet>, edm::Ptr<flashgg::Jet>,
              edm::Ptr<flashgg::Jet>, edm::Ptr<flashgg::Jet>,
              std::vector<edm::Ptr<flashgg::Jet>>, std::vector<edm::Ptr<flashgg::Jet>>,
              std::vector<double>, std::vector<double>
              );

    // HHWWggTag_3 - Fully Leptonic 
    HHWWggTag(edm::Ptr<flashgg::DiPhotonCandidate>, edm::Ptr<flashgg::Electron>, edm::Ptr<flashgg::Electron>, edm::Ptr<flashgg::Met>, std::vector<double>,double);
    HHWWggTag(edm::Ptr<flashgg::DiPhotonCandidate>, edm::Ptr<flashgg::Muon>, edm::Ptr<flashgg::Muon>, edm::Ptr<flashgg::Met>, std::vector<double>,double);
    HHWWggTag(edm::Ptr<flashgg::DiPhotonCandidate>, edm::Ptr<flashgg::Electron>, edm::Ptr<flashgg::Muon>, edm::Ptr<flashgg::Met>, std::vector<double>,double);    

    // HHWWggTag_4 - Untagged 
    HHWWggTag(edm::Ptr<DiPhotonCandidate>,
              std::vector<edm::Ptr<flashgg::Electron>>, std::vector<edm::Ptr<flashgg::Electron>>,
              std::vector<edm::Ptr<flashgg::Muon>>, std::vector<edm::Ptr<flashgg::Muon>>,
              edm::Ptr<flashgg::Met>,
              std::vector<edm::Ptr<flashgg::Jet>>, std::vector<edm::Ptr<flashgg::Jet>>,
              std::vector<double>, std::vector<double>, std::vector<double>
              );

    // Required this because HHWWggTag is derived from another class
    virtual HHWWggTag *clone() const override;

    //-- Utilities used in HHWWggCustomize, not HHWWggTag.cc 

    void setMVA(double x) { mva_ = x; }
    double MVA() const { return mva_; }
    void setEventNumber(double x) { eventNumber_ = x; }

    const flashgg::Photon* Leading_Photon() const { return Leading_Photon_; };
    const flashgg::Photon* Subleading_Photon() const { return Subleading_Photon_; };
    const float lp_Hgg_MVA() const {return lp_Hgg_MVA_;};
    const float slp_Hgg_MVA() const {return slp_Hgg_MVA_;};
    const float lp_pt() const {return lp_pt_;};
    const float slp_pt() const {return slp_pt_;};
    const flashgg::Electron Electron() const { return Electron_; };
    const flashgg::Muon Muon() const { return Muon_; };
    const flashgg::Jet Leading_Jet() const { return Leading_Jet_; };
    const flashgg::Jet Subleading_Jet() const { return Subleading_Jet_; };
    const flashgg::Jet Sub2leading_Jet() const { return Sub2leading_Jet_; };
    const flashgg::Jet Sub3leading_Jet() const { return Sub3leading_Jet_; };
    const flashgg::Met MET() const {return MET_; };
    const std::vector<double> Cut_Variables() const { return Cut_Variables_; };
    const std::vector<double> MuonVars() const { return MuonVars_; };
    const std::vector<double> JetVars() const { return JetVars_; };
    const std::vector<flashgg::Electron> allElectrons() const {return allElectrons_;}
    const std::vector<flashgg::Electron> goodElectrons() const {return goodElectrons_;}
    const std::vector<flashgg::Muon> allMuons() const {return allMuons_;}
    const std::vector<flashgg::Muon> goodMuons() const {return goodMuons_;}
    const std::vector<flashgg::Jet> allJets() const {return allJets_;}
    const std::vector<flashgg::Jet> goodJets() const {return goodJets_;}

    const LorentzVector & dijet() const { return dijet_; }
    const LorentzVector & dijet2() const { return dijet2_; }
    const LorentzVector & HWW() const { return HWW_; }
    const LorentzVector & HGG() const { return HGG_; }
    const LorentzVector & HH() const { return HH_; }

    const reco::Candidate::LorentzVector& Leading_lepton() const { return Leading_lepton_; };
    const reco::Candidate::LorentzVector& Subleading_lepton() const { return Subleading_lepton_; };
    const float dipho_MVA() const {return dipho_MVA_;};

  private:
    double mva_;
    long eventNumber_;
    const flashgg::Photon* Leading_Photon_;
    const flashgg::Photon* Subleading_Photon_;
    float lp_Hgg_MVA_;
    float slp_Hgg_MVA_;
    float lp_pt_;
    float slp_pt_;
    std::vector<double> Cut_Variables_;
    std::vector<double> MuonVars_;
    std::vector<double> JetVars_;
    flashgg::Electron Electron_;
    flashgg::Muon Muon_;
    flashgg::Jet Leading_Jet_;
    flashgg::Jet Subleading_Jet_;
    flashgg::Jet Sub2leading_Jet_;
    flashgg::Jet Sub3leading_Jet_;
    flashgg::Met MET_;

    std::vector<flashgg::Electron> allElectrons_;
    std::vector<flashgg::Electron> goodElectrons_;
    std::vector<flashgg::Muon> allMuons_;
    std::vector<flashgg::Muon> goodMuons_;
    std::vector<flashgg::Jet> allJets_;
    std::vector<flashgg::Jet> goodJets_;

    LorentzVector dijet_;
    LorentzVector dijet2_;
    LorentzVector HWW_;
    LorentzVector HGG_;
    LorentzVector HH_;
    reco::Candidate::LorentzVector Leading_lepton_;
    reco::Candidate::LorentzVector Subleading_lepton_;
    float dipho_MVA_;
  };

}

#endif
