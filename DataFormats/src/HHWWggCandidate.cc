// include path /cvmfs/cms.cern.ch/slc7_amd64_gcc700/cms/cmssw/CMSSW_10_5_0/src/
#include "TLorentzVector.h"
#include "DataFormats/Math/interface/LorentzVector.h"
#include "flashgg/DataFormats/interface/Photon.h"
#include "flashgg/DataFormats/interface/Electron.h"
#include "flashgg/DataFormats/interface/Muon.h"
#include "flashgg/DataFormats/interface/DiPhotonCandidate.h"
#include "flashgg/DataFormats/interface/HHWWggCandidate.h"
#include "flashgg/DataFormats/interface/Met.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "flashgg/DataFormats/interface/Jet.h"
#include "DataFormats/Math/interface/deltaR.h"

using namespace flashgg; // makes flashgg sub members visible 
HHWWggCandidate::HHWWggCandidate():
diphoVector_ (),
electronVector_ (),
muonVector_ (),
METVector_ (),
GenParticlesVector_ (),
GenElectrons_ (),
GenMuons_ (),
GenPhotons_ (),
GenQuarks_ (),
GenNeutrinos_ (),
JetVector_ (),
Leading_Photon_ (),
Subleading_Photon_ (),
MET_fourvec_ (),
leading_dpho_ (),
leading_elec_(),
subleading_elec_(),
leading_muon_(),
subleading_muon_(),
theMETcorpt_(),
Vertex_Variables_ (),
Cut_Variables_ (),
dipho_MVA_ (),
CMS_hgg_mass_ (),
dZ_ (),
lp_Hgg_MVA_ (),
slp_Hgg_MVA_ ()
// Need absence of comma on last variable 

{}

  HHWWggCandidate::~HHWWggCandidate() {}
  
  HHWWggCandidate::HHWWggCandidate( std::vector<flashgg::DiPhotonCandidate> diphoVector, std::vector<flashgg::Electron> electronVector, 
                                    std::vector<flashgg::Muon> muonVector, std::vector<flashgg::Met> METVector, std::vector<reco::GenParticle> GenParticlesVector,
                                    std::vector<flashgg::Jet> JetVector, std::vector<double> Vertex_Variables, std::vector<double> Cut_Variables, double dipho_MVA, double CMS_hgg_mass, double dZ):
  diphoVector_(diphoVector), electronVector_(electronVector), muonVector_(muonVector), METVector_(METVector), GenParticlesVector_(GenParticlesVector), 
  JetVector_(JetVector), Vertex_Variables_(Vertex_Variables), Cut_Variables_(Cut_Variables), dipho_MVA_(dipho_MVA), CMS_hgg_mass_(CMS_hgg_mass), dZ_ (dZ)

  {

/*
    // GEN Object operations below
    
    std::vector<int> quark_pdgids = {1,2,3,4}; // Don't look for b quarks 

    // // Look through gen particles vector 
    for (unsigned int i = 0; i < GenParticlesVector_.size(); i++){
      reco::GenParticle gen_ = GenParticlesVector_[i];  

      // Electrons
      if (abs(gen_.pdgId()) == 11){
        auto gen_p4 = gen_.p4();
        GenElectrons_.push_back(gen_p4);
      }
      
      // Muons 
      if (abs(gen_.pdgId()) == 13){
        auto gen_p4 = gen_.p4();
        GenMuons_.push_back(gen_p4);
      }

      // Photons 
      if (abs(gen_.pdgId()) == 22){
        auto gen_p4 = gen_.p4();
        GenPhotons_.push_back(gen_p4);
      }

      // Quarks 
      for (unsigned int i = 0; i < quark_pdgids.size(); i++){
        int val = quark_pdgids[i];

        // all gen_ quark objects in genparticles vector came from W bosons, as dictated by HHWWggCandidateProducer.cc 
        // Should break into W+ quarks and W- quarks so you know if two quarks came from the same W and can accurately call a pair a match 
        if (abs(gen_.pdgId()) == val ){
            auto gen_p4 = gen_.p4();
            GenQuarks_.push_back( gen_p4 );
        } 
      }  

      // Neutrinos 
      if ( (abs(gen_.pdgId()) == 12) || (abs(gen_.pdgId()) == 14) ){
        auto gen_p4 = gen_.p4();
        GenNeutrinos_.push_back(gen_p4);
      }

      // Save gen H->gg pt  
      // if (abs(gen_.pdgId()) == 22){
      //   gen_phoVector.push_back( gen_ );
      //   auto hgg = gen_.mother(0);
      //   auto hgg_p4 = hgg->p4();
      //   hgg_p4_ = hgg_p4;
        
      //   // cout << "higgs pt = " << hgg_p4.pt() << endl;
      // }

      // // Create quark vector (gen) 
      // for (unsigned int i = 0; i < quark_pdgids.size(); i++){
      //   int val = quark_pdgids[i];

      //   // all gen_ quark objects in genparticles vector came from W bosons, as dictated by HHWWggCandidateProducer.cc 
      //   // Should break into W+ quarks and W- quarks so you know if two quarks came from the same W and can accurately call a pair a match 
      //   if (abs(gen_.pdgId()) == val ){
      //       //cout << "quark mother = " << gen_->mother(0)->pdgId() << endl;
      //       //cout << "quark mother = " << gen_.mother(0).pdgId() << endl;
      //       // auto m = gen_.mother(0);
      //       //int mid = m->pdgId();
      //       //cout << "motherID = " << m->pdgId() << endl;
      //       //quarkVector.push_back( make_pair( gen_,mid ) );
      //       quarkVector.push_back( gen_ );
      //   } 
      // }

    }

    // if (GenElectrons_.size() > 1){
    //   cout << "[WARNING] - GenElectrons_.size() > 1" << endl; 
    // }

    // In GEN vectors, if second element has higher pt, swap 
    float genPhopT1 = GenPhotons_[0].pt(); 
    float genPhopT2 = GenPhotons_[1].pt(); 
    if (genPhopT2 > genPhopT1){
        reco::Candidate::LorentzVector gen_pho_p4_1 = GenPhotons_[0];
        reco::Candidate::LorentzVector gen_pho_p4_2 = GenPhotons_[1];
        GenPhotons_[0] = gen_pho_p4_2;
        GenPhotons_[1] = gen_pho_p4_1;
    } 

    float genQuarkpT1 = GenQuarks_[0].pt();
    float genQuarkpT2 = GenQuarks_[1].pt();

    if (genQuarkpT2 > genQuarkpT1) {
        reco::Candidate::LorentzVector gen_quark_p4_1 = GenQuarks_[0];
        reco::Candidate::LorentzVector gen_quark_p4_2 = GenQuarks_[1];
        GenQuarks_[0] = gen_quark_p4_2;
        GenQuarks_[1] = gen_quark_p4_1;      
    }

*/

    // GEN operations above 

    // First diphoton has highest pt 
    if (diphoVector_.size() > 0){
      flashgg::DiPhotonCandidate dipho_ = diphoVector_[0];
      // if (!dipho_zero_vtx){
      //   cout << "Diphoton vertex is not zero. Need to recompute" << endl; 
      // }
      dipho_.makePhotonsPersistent();
      auto dipho = dipho_.p4();
      leading_dpho_ = dipho;

      // Get photons 
      flashgg::Photon leading_photon = dipho_.getLeadingPhoton();
      flashgg::Photon subleading_photon = dipho_.getSubLeadingPhoton();

      // Save as dumper objects 
      Leading_Photon_ = leading_photon;
      Subleading_Photon_ = subleading_photon;

      // float lead_pho_Hgg_MVA = leading_photon.phoIdMvaDWrtVtx( dipho_.vtx() ); // Should be photon hgg MVA wrt to 0th vertex
      // float sublead_pho_Hgg_MVA = subleading_photon.phoIdMvaDWrtVtx( dipho_.vtx() ); // Should be Photon hgg MVA wrt to 0th vertex

      lp_Hgg_MVA_ = leading_photon.phoIdMvaDWrtVtx( dipho_.vtx() ); // Should be photon hgg MVA wrt to 0th vertex
      slp_Hgg_MVA_ = subleading_photon.phoIdMvaDWrtVtx( dipho_.vtx() ); // Should be Photon hgg MVA wrt to 0th vertex

      // Get EG MVA scores, passelectronveto, haspixelseed 
      // lead_pho_EG_MVA_ = leading_photon.userFloat("PhotonMVAEstimatorRunIIFall17v1p1Values"); // v1p1 = 1.1. Not sure what this is, EG group recommends v2 for 2017 EOY 
      // sublead_pho_EG_MVA_ = subleading_photon.userFloat("PhotonMVAEstimatorRunIIFall17v1p1Values");
      // lead_pho_passElectronVeto_ = leading_photon.passElectronVeto();
      // sublead_pho_passElectronVeto_ = subleading_photon.passElectronVeto();
      // lead_pho_hasPixelSeed_ = leading_photon.hasPixelSeed();
      // sublead_pho_hasPixelSeed_= subleading_photon.hasPixelSeed();

      // cout << "l_pho_SC_R9 = " << leading_photon.old_r9();
      // cout << "l_pho_SC_5x5 = " << leading_photon.full5x5_r9();

      // l_pho_SC_eta_ = leading_photon.superCluster()->eta();
      // sl_pho_SC_eta_ = subleading_photon.superCluster()->eta();

      CMS_hgg_mass_ = dipho.mass();

      //----------------------------------------------------------------------------------------------------------------------------

      // For fggfinalfit, adding selections here because don't know how to filter workspace after dumper has run
      // Should turn this off if not creating a workspace 

      bool pass_selections = 0;
      bool lead_pass_TightPhoID = 0, sublead_pass_TightPhoID = 0; 

      double n_good_electrons = electronVector_.size();
      double n_good_muons = muonVector_.size();
      double n_good_leptons = n_good_electrons + n_good_muons;
      double n_good_jets = JetVector_.size();
      double leading_pho_eta = leading_photon.p4().eta(), sub_leading_pho_eta = subleading_photon.p4().eta();

      // Using January 2016 Photon MVA ID training working points for bbgg 

      if (n_good_leptons == 1){
        if ( n_good_jets >= 2 ){

          // leading photon 
          // EB 
          if (( abs(leading_pho_eta) > 0) && ( abs(leading_pho_eta) < 1.4442)){
            // if (lead_pho_EG_MVA_ > 0.42) lead_pass_TightPhoID = 1; 
            if (lp_Hgg_MVA_ > 0.07) lead_pass_TightPhoID = 1; 
          }

          // EE 
          else if (( abs(leading_pho_eta) > 1.566) && ( abs(leading_pho_eta) < 2.5)){
            // if (lead_pho_EG_MVA_ > 0.14) lead_pass_TightPhoID = 1;
            if (lp_Hgg_MVA_ > -0.03) lead_pass_TightPhoID = 1;
          }

          // SubLeading Photon
          // EB 
          if (( abs(sub_leading_pho_eta) > 0) && ( abs(sub_leading_pho_eta) < 1.4442)){
            // if (sublead_pho_EG_MVA_ > 0.42) sublead_pass_TightPhoID = 1; 
            if (slp_Hgg_MVA_ > 0.07) sublead_pass_TightPhoID = 1; 
          }

          // EE 
          else if (( abs(sub_leading_pho_eta) > 1.566) && ( abs(sub_leading_pho_eta) < 2.5)){
            // if (sublead_pho_EG_MVA_ > 0.14) sublead_pass_TightPhoID = 1;
            if (slp_Hgg_MVA_ > -0.03) sublead_pass_TightPhoID = 1;
          }

          if (lead_pass_TightPhoID && sublead_pass_TightPhoID){
            
            // mass window for fitting background
            if  ((dipho.mass() > 100.) && (dipho.mass() < 180.)){
              pass_selections = 1;
            }
          }

        }
      }

      // // Only n good leptons & jets selections 
      // if (n_good_leptons == 1){
      //   if (n_good_jets >= 2){                                                                                                                                                                                                                                                                                                                                                                                                                                                           
      //   // if ((n_good_jets == 1) || (n_good_jets == 2) ){
      //     pass_selections = 1;
      //   }
      //   // }
      // }

      if (pass_selections){
        CMS_hgg_mass_ = dipho.mass();
      }
      else {    
        CMS_hgg_mass_ = -99; 
        dZ_ = -999;
      }
      //----------------------------------------------------------------------------------------------------------------------------

    // Requested UserFloat PhotonMVAEstimatorRunIIFall17v2Values is not available! Possible UserFloats are:
    // EGMPhotonMVA PhotonMVAEstimatorRun2Spring15NonTrig25nsV2p1Values PhotonMVAEstimatorRun2Spring15NonTrig50nsV2p1Values PhotonMVAEstimatorRun2Spring16NonTrigV1Values PhotonMVAEstimatorRunIIFall17v1Values PhotonMVAEstimatorRunIIFall17v1p1Values ecalEnergyErrPostCorr ecalEnergyErrPreCorr ecalEnergyPostCorr ecalEnergyPreCorr energyScaleDown energyScaleGainDown energyScaleGainUp energyScaleStatDown energyScaleStatUp energyScaleSystDown energyScaleSystUp energyScaleUp energyScaleValue energySigmaDown energySigmaPhiDown energySigmaPhiUp energySigmaRhoDown energySigmaRhoUp energySigmaUp energySigmaValue energySmearNrSigma genIso phoChargedIsolation phoNeutralHadronIsolation phoPhotonIsolation phoWorstChargedIsolation rnd_g_E

    }

    // MET 
    if (METVector_.size() == 1)
    {
      flashgg::Met met__ = METVector_[0];
      auto met_ = met__.p4();
      MET_fourvec_ = met_;

      theMETcorpt_ = met__.getCorPt();

      //auto W = met_ + elec1;
      //W1_TM_ = W.Mt();

    } 

    // float elec_pt = -99, muon_pt = -99;
    // reco::Candidate::LorentzVector leading_lepton_; 

    // Leading/subleading electrons
    if (electronVector_.size() > 0){
      flashgg::Electron l_elec = electronVector_[0];
      auto l_elec_4vec = l_elec.p4();
      auto leading_elec = l_elec_4vec;
      leading_elec_ = leading_elec;
      // if (muonVector_.size() == 0){
      //   leading_lepton_ = leading_elec;
      // }
    }
    
    if (electronVector_.size() > 1){    
      flashgg::Electron sl_elec = electronVector_[1];
      auto sl_elec_4vec = sl_elec.p4();
      auto subleading_elec = sl_elec_4vec;
      subleading_elec_ = subleading_elec;    
    }
    // Leading/subleading Muons 
    if (muonVector_.size() > 0){
      flashgg::Muon l_muon = muonVector_[0];
      auto l_muon_4vec = l_muon.p4();
      auto leading_muon = l_muon_4vec;
      leading_muon_ = leading_muon;
      // if (electronVector_.size() == 0){
      //   leading_lepton_ = leading_muon;
      // }
    }

    if (muonVector_.size() > 1){
      flashgg::Muon sl_muon = muonVector_[1];
      auto sl_muon_4vec = sl_muon.p4();
      auto subleading_muon = sl_muon_4vec;
      subleading_muon_ = subleading_muon;   
    }

  } //end 