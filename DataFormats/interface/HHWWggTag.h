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
    // Vertex
    // void SetVtxVals(double, double, double);

    // Other 
    void GetPhoAtt(edm::Ptr<DiPhotonCandidate> dipho);
    void GetObjects(edm::Ptr<DiPhotonCandidate> dipho);
    void GetObjects(edm::Ptr<DiPhotonCandidate> dipho, edm::Ptr<flashgg::Met>);
    void GetObjects(edm::Ptr<DiPhotonCandidate> dipho, edm::Ptr<flashgg::Electron> electron, edm::Ptr<flashgg::Met> MET, edm::Ptr<flashgg::Jet> jet1, edm::Ptr<flashgg::Jet> jet2);
    void GetObjects(edm::Ptr<DiPhotonCandidate> dipho, edm::Ptr<flashgg::Muon> muon, edm::Ptr<flashgg::Met> MET, edm::Ptr<flashgg::Jet> jet1, edm::Ptr<flashgg::Jet> jet2);
    void GetObjects(edm::Ptr<DiPhotonCandidate> dipho, edm::Ptr<flashgg::Met> MET, edm::Ptr<flashgg::Jet> jet1, edm::Ptr<flashgg::Jet> jet2, edm::Ptr<flashgg::Jet> jet3, edm::Ptr<flashgg::Jet> jet4);
    
    // Get Leptons and Jets
    std::vector<flashgg::Electron> GetElectrons(std::vector<edm::Ptr<flashgg::Electron>>);
    void SetAllElectrons(std::vector<edm::Ptr<flashgg::Electron>>);
    void SetGoodElectrons(std::vector<edm::Ptr<flashgg::Electron>>);
    void SetAllMuons(std::vector<edm::Ptr<flashgg::Muon>>);
    void SetGoodMuons(std::vector<edm::Ptr<flashgg::Muon>>);

    std::vector<flashgg::Muon> GetMuons(std::vector<edm::Ptr<flashgg::Muon>>);
    std::vector<flashgg::Jet> GetJets(std::vector<edm::Ptr<flashgg::Jet>>);
    // void setZeroVertex(edm::Ptr<reco::Vertex>);
    void SetGoodJets(std::vector<edm::Ptr<flashgg::Jet>>);
    void SetAllJets(std::vector<edm::Ptr<flashgg::Jet>>);
    void SetDiPhoMVA(double);
    void SetDiPhoPt(double);
    void SetDiphoCentralWeight(double);

    // Jet Vars 
    void SetJetVars(std::vector<double>); 
    void SetJetIDs(std::vector<std::vector<double>>);

    //-- Gen Particles
    void SetGenObjs(std::vector<edm::Ptr<reco::GenParticle> >, std::vector<edm::Ptr<reco::GenParticle> >, std::vector<edm::Ptr<reco::GenParticle> >,
                    std::vector<edm::Ptr<reco::GenParticle> >, std::vector<edm::Ptr<reco::GenParticle> >, std::vector<edm::Ptr<reco::GenParticle> >);

    //-- PDF Info 
    // void SavePDFInfo();

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

    //-- SL systematic tree. Good objects only 
    HHWWggTag(edm::Ptr<DiPhotonCandidate>,
                          std::vector<edm::Ptr<flashgg::Electron>>,
                          std::vector<edm::Ptr<flashgg::Muon>>,
                          edm::Ptr<flashgg::Met>,
                          std::vector<edm::Ptr<flashgg::Jet>>,
                          std::vector<double>); 


    //-- FH systematic tree. Good objects only 
    HHWWggTag(edm::Ptr<DiPhotonCandidate> dipho,
                  edm::Ptr<flashgg::Met>,
                  edm::Ptr<flashgg::Jet>, edm::Ptr<flashgg::Jet>,
                  edm::Ptr<flashgg::Jet>, edm::Ptr<flashgg::Jet>,
                  std::vector<edm::Ptr<flashgg::Jet>>, 
                  std::vector<double>);


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
    // const edm::Ptr<reco::Vertex> ZeroVertex() const {return ZeroVertex_;}
    const float DiphoCentralWeight() const {return DiphoCentralWeight_;}
// const std::vector<edm::Ptr<reco::Vertex>> vtx

    const LorentzVector & dijet() const { return dijet_; }
    const LorentzVector & dijet2() const { return dijet2_; }
    const LorentzVector & HWW() const { return HWW_; }
    const LorentzVector & HGG() const { return HGG_; }
    const LorentzVector & HH() const { return HH_; }

    const reco::Candidate::LorentzVector& Leading_lepton() const { return Leading_lepton_; };
    const reco::Candidate::LorentzVector& Subleading_lepton() const { return Subleading_lepton_; };
    const float dipho_MVA() const {return dipho_MVA_;};
    const float dipho_pt() const {return dipho_pt_;};

    void setGenMhh(double x) { genMhh_ = x; }
    double genMhh() const { return genMhh_; }
    void setGenCosThetaStar_CS(double x) { genCosThetaStar_CS_ = x; }
    double getGenCosThetaStar_CS() const { return genCosThetaStar_CS_; }

    double GenVtx_z() const { return GenVtx_z_; }
    void setGenVtx_z(double x) { GenVtx_z_ = x; }
    double HggVtx_z() const { return HggVtx_z_; }
    void setHggVtx_z(double x) { HggVtx_z_ = x; }
    double ZeroVtx_z() const {return ZeroVtx_z_; }
    void setZeroVtx_z(double x) { ZeroVtx_z_ = x; }

    // Jet PUID Booleans 
    const std::vector<std::vector<double>> goodJets_passJetPUID() const {return goodJets_passJetPUID_;}

    // Gen particles
    const std::vector<reco::Candidate::LorentzVector > genHiggsBosons() const {return genHiggsBosons_;}
    const std::vector<reco::Candidate::LorentzVector > genWBosons() const {return genWBosons_;}
    const std::vector<reco::Candidate::LorentzVector > genPhotons() const {return genPhotons_;}
    const std::vector<reco::Candidate::LorentzVector > genQuarks() const {return genQuarks_;}
    const std::vector<reco::Candidate::LorentzVector > genLeptons() const {return genLeptons_;}
    const std::vector<reco::Candidate::LorentzVector > genNeutrinos() const {return genNeutrinos_;}
    const std::vector<double> genQuarksPdgIds() const {return genQuarksPdgIds_;}
    const std::vector<double> genLeptonsPdgIds() const {return genLeptonsPdgIds_;}
    const std::vector<double> genNeutrinosPdgIds() const {return genNeutrinosPdgIds_;}   

    // PDF Info 
     

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
    std::vector<double> MuonVars_ = {-99}; // Give initial value to avoid void vectors when obtaining final state variables  
    std::vector<double> JetVars_ = {-99}; // Give initial value to avoid void vectors when obtaining final state variables 
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
    // edm::Ptr<reco::Vertex> ZeroVertex_;

    LorentzVector dijet_;
    LorentzVector dijet2_;
    LorentzVector HWW_;
    LorentzVector HGG_;
    LorentzVector HH_;
    reco::Candidate::LorentzVector Leading_lepton_;
    reco::Candidate::LorentzVector Subleading_lepton_;
    float dipho_MVA_ = -999;
    float dipho_pt_ = -999; 
    double genMhh_;
    double genCosThetaStar_CS_;

    // Vertex Variables
    double GenVtx_z_; 
    double HggVtx_z_; 
    double ZeroVtx_z_; 

    // Jet PUID booleans  
    vector<vector<double>> goodJets_passJetPUID_; 

    // Gen particles
    std::vector<reco::Candidate::LorentzVector> genHiggsBosons_;
    std::vector<reco::Candidate::LorentzVector> genWBosons_;
    std::vector<reco::Candidate::LorentzVector> genPhotons_;
    std::vector<reco::Candidate::LorentzVector> genQuarks_;
    std::vector<reco::Candidate::LorentzVector> genLeptons_;
    std::vector<reco::Candidate::LorentzVector> genNeutrinos_;
    std::vector<double> genQuarksPdgIds_; 
    std::vector<double> genLeptonsPdgIds_;
    std::vector<double> genNeutrinosPdgIds_;       

    double DiphoCentralWeight_;
  };

}

#endif
