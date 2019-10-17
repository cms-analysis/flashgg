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
JetVector_ (),
phoP4Corrected_ (),
MET_fourvec_ (),
leading_dpho_ (),
leading_pho_ (),
sub_leading_pho_ (),
leading_elec_(),
subleading_elec_(),
leading_muon_(),
subleading_muon_(),
theMETcorpt_(),
gen_leading_elec_ (),
gen_subleading_elec_ (),
gen_leading_muon_ (),
gen_subleading_muon_ (), 
Vertex_Variables_ (),
Cut_Variables_ (),
dipho_MVA_ (),
lead_pho_Hgg_MVA_ (),
sublead_pho_Hgg_MVA_ (),
lead_pho_EG_MVA_ (),
sublead_pho_EG_MVA_ (),
lead_pho_passElectronVeto_ (),
sublead_pho_passElectronVeto_ (),
lead_pho_hasPixelSeed_ (),
sublead_pho_hasPixelSeed_ (),
CMS_hgg_mass_ (),
dZ_ ()
// Need absence of comma on last variable 

{}

  HHWWggCandidate::~HHWWggCandidate() {}
  
  HHWWggCandidate::HHWWggCandidate( std::vector<flashgg::DiPhotonCandidate> diphoVector, std::vector<flashgg::Electron> electronVector, 
                                    std::vector<flashgg::Muon> muonVector, std::vector<flashgg::Met> METVector, std::vector<reco::GenParticle> GenParticlesVector,
                                    std::vector<flashgg::Jet> JetVector, std::vector<double> Vertex_Variables, std::vector<double> Cut_Variables, double dipho_MVA, double lead_pho_Hgg_MVA, double sublead_pho_Hgg_MVA, double CMS_hgg_mass, double dZ):
  diphoVector_(diphoVector), electronVector_(electronVector), muonVector_(muonVector), METVector_(METVector), GenParticlesVector_(GenParticlesVector), 
  JetVector_(JetVector), Vertex_Variables_(Vertex_Variables), Cut_Variables_(Cut_Variables), dipho_MVA_(dipho_MVA), lead_pho_Hgg_MVA_(lead_pho_Hgg_MVA), sublead_pho_Hgg_MVA_(sublead_pho_Hgg_MVA), CMS_hgg_mass_(CMS_hgg_mass), dZ_ (dZ)

  {


    // GEN Object operations below

    // /////////////////////////////////////////////////////////////////////

    // // cout << "Cut_Variables_[11] = " << Cut_Variables_[11] << endl;
    
    // //-- GEN and Jets 

    // //std::vector<int> quark_pdgids = {1,2,3,4,5}; // Include b quarks
    // std::vector<int> quark_pdgids = {1,2,3,4}; // Don't look for b quarks 
    // vector<reco::GenParticle> quarkVector; // quarks
    // vector<reco::GenParticle> gen_electronVector; // gen electrons
    // vector<reco::GenParticle> gen_muonVector; // gen muons 
    // vector<reco::GenParticle> gen_phoVector; // gen muons 

    // //cout << "GenParticlesVector_.size() = " << GenParticlesVector_.size() << endl; 

    // // Look through gen particles vector 
    // for (unsigned int i = 0; i < GenParticlesVector_.size(); i++){

    //   reco::GenParticle gen_ = GenParticlesVector_[i];  
    //   // Save gen H->gg pt  
    //   if (abs(gen_.pdgId()) == 22){
    //     gen_phoVector.push_back( gen_ );
    //     auto hgg = gen_.mother(0);
    //     auto hgg_p4 = hgg->p4();
    //     hgg_p4_ = hgg_p4;
        
    //     // cout << "higgs pt = " << hgg_p4.pt() << endl;
    //   }

    //   // Create quark vector (gen) 
    //   for (unsigned int i = 0; i < quark_pdgids.size(); i++){
    //     int val = quark_pdgids[i];

    //     // all gen_ quark objects in genparticles vector came from W bosons, as dictated by HHWWggCandidateProducer.cc 
    //     // Should break into W+ quarks and W- quarks so you know if two quarks came from the same W and can accurately call a pair a match 
    //     if (abs(gen_.pdgId()) == val ){
    //         //cout << "quark mother = " << gen_->mother(0)->pdgId() << endl;
    //         //cout << "quark mother = " << gen_.mother(0).pdgId() << endl;
    //         // auto m = gen_.mother(0);
    //         //int mid = m->pdgId();
    //         //cout << "motherID = " << m->pdgId() << endl;
    //         //quarkVector.push_back( make_pair( gen_,mid ) );
    //         quarkVector.push_back( gen_ );
    //     } 
    //   }
    // }

    // //-- Jet/Quark Matching

    // // The options for number of gen quarks from H->WW is -> qqqq (4), ->qqlnu (2), and ->lnulnu (0)
    // // 4,2 or 0 Quarks. 
    // // 2, 1 or 0 diquark matches (from H->WW)
    // // 
    // // Number of jets can be many. 
    // // Max number of dijet matched from H->WW: 2 (b/c 2, 1 or 0)

    // // Now that we have gen quark vector, we use it to match to jets 
    // unsigned int JVSize = JetVector_.size(), QVSize = quarkVector.size();

    

    // float qone_matches__ = 0;
    // float qtwo_matches__ = 0;
    // float merged_qs__ = 0;
    // float dr_1__ = 0;
    // float dr_2__ = 0;
    // if (QVSize == 2){
    //    reco::GenParticle q1 = quarkVector[0]; 
    //    auto q1_p4 = q1.p4();
    //    reco::GenParticle q2 = quarkVector[1]; 
    //    auto q2_p4 = q2.p4();   
    //    auto qq = q1_p4 + q2_p4;   
    //    auto m_qq = qq.mass();
    //   //  cout << "qq invmass = " << qq.mass() << endl; 
    //    m_qq_ = m_qq;
    //   //  m_qq
    //   //if (JVSize == 1){
    //   for (unsigned int jet_i = 0; jet_i < JVSize; jet_i ++){
    //     auto j0_ = JetVector_[jet_i].p4();
    //     dr_1__ = deltaR(j0_.eta(), j0_.phi(), q1_p4.eta(), q1_p4.phi());
    //     dr_2__ = deltaR(j0_.eta(), j0_.phi(), q2_p4.eta(), q2_p4.phi());
    //     dr_1_ = dr_1__;
    //     dr_2_ = dr_2__;
    //     // cout << "dr_2 = " << dr_2 << endl;
    //     if ( (0 < dr_1__) && (dr_1__ < 0.1) ) qone_matches__ += 1.0;
    //     if ( (0 < dr_2__) && (dr_2__ < 0.1) ) qtwo_matches__ += 1.0;
    //     qone_matches_ = qone_matches__;
    //     qtwo_matches_ = qtwo_matches__;
    //     if ( (qone_matches__ == 1.0) && (qtwo_matches__ == 1.0) ){
    //       merged_qs__ += 1.0;
    //     }
    //   }
    //   //}
    // }

    // merged_qs_ = merged_qs__;

    // // cout << "qq invmass = " << qq.mass() << endl; 

    // // for (unsigned int i = 0; i < QVSize; i++){
    // //    reco::GenParticle q = quarkVector[i]; 
    // //    auto q4 = q.p4();
    // //   //  cout << "quark " << i << " pt = " << q4.pt() << endl;
    // // }

    // //bool match1 = false, match2 = false;
    // bool is_pair = 0;
    // // if (diphoVector_.size() >= 1) {
    // //   test = 1;
    // //   }
    // // else {
    // //   test = 0;
    // //   }
    // double dr = 0.; //, dpt = 0.;
    // int mii = 0; 
    // int qim = 0, qjm = 0; // i and j quark mothers 
    // //float inv_mass = 0;

    // std::vector<flashgg::Jet> jVecCopy = JetVector_; // make new copy with all jets in it so you can remove a jet after it's matched to a quark 

    // //cout << "QVSize = " << QVSize << endl;
    // //cout << "JVSize = " << JVSize << endl;

    // // Before using gen info, can already form dijet objects based on pT
    // // Remember, JetVector_ is ordered by pT
    // // Save object of leading/subleading jets

    // if (JVSize >= 2){
    //   auto j0 = JetVector_[0].p4();
    //   auto j1 = JetVector_[1].p4();
    //   auto lsl_dij = j0 + j1;
    //   lsl_dij_ = lsl_dij;
    // }
    // // If there are more quarks than jets, there will be quarks without matching jets 
    // // If there are extra jets, we don't care. 

    // // for some reason there are 4 quarks coming from the 2 quark signal file. For now want to skip these events. 
    // if (QVSize == 2){

    // if ( (QVSize < JVSize) && (JVSize > 0) && (QVSize > 0) ){

    //   //cout << "Hello, in quark/jet matching loop" << endl;

    //   //vector<reco::GenParticle> qVecCopy = quarkVector;
    //   // Loop 'i' quarks 
    //   for (unsigned int i = 0; i < QVSize - 1; i++){ // last quark cannot be ith quark because then there will be no j quark 

    //     //cout << "Looking at quark " << i << endl;

    //     //flashgg::Jet ijet = JetVector_[i];
    //     reco::GenParticle quarki = quarkVector[i]; //quark i 
    //     auto qim0 = quarki.mother(0);
    //     qim = qim0->pdgId();
    //     //cout << "qim = " << qim << endl;

    //     auto qi = quarki.p4(); // quark i four vector 

    //     // Loop all jets to find smallest dr 
    //     // if a jet matches the quark, remove the jet from the temporary vector 
    //     double mdr = 100.; // minimum dr 
    //     // Loop 
    //     //double dr = 0.,; 
    //     // vector<reco::GenParticle> qVecCopy = quarkVector; // make new copy with all quarks in
    //     for (unsigned int ii = 0; ii < jVecCopy.size(); ii++){
    //       dr = 0.;
    //       //cout << "jVecCopy[ii].eta() = " << jVecCopy[ii].eta() << endl;
    //       //cout << "jVecCopy[ii].p4().eta() = " << jVecCopy[ii].p4().eta() << endl;

    //       dr = sqrt( (jVecCopy[ii].eta()-qi.eta())*(jVecCopy[ii].eta()-qi.eta()) + (jVecCopy[ii].phi()-qi.phi())*(jVecCopy[ii].phi()-qi.phi()) );
    //       //cout << "dr = " << dr << endl;
    //       if (dr < mdr) {
    //         mdr = dr;
    //         mii = ii; // index of minimum dr jet 
    //       }

    //     }

    //     // First jet matched 
    //     // Save whatever you need before deleting the matching quark from qVecCopy
    //     //cout << "Matched quark mother = " << qVecCopy[mii].mother(0)->pdgId() << endl;
    //     //cout << "Matched quark/jet dr = " << mdr << endl;
    //     //cout << "Matched jet index = " << mii << endl;
    //     auto jet1 = jVecCopy[mii].p4();
    //       // for (unsigned int iii = 0; iii < jVecCopy.size(); iii++){
    //       //   cout << "jVecCopy[" << iii << "] = " << jVecCopy[iii] << endl;
    //       // }
    //     jVecCopy.erase(jVecCopy.begin() + mii); // remove matched jet from cloned vector 

    //       // for (unsigned int iii = 0; iii < jVecCopy.size(); iii++){
    //       //   cout << "jVecCopy[" << iii << "] = " << jVecCopy[iii] << endl;
    //       // }
    //     // use same cloned vector to match remaining jets to second quark

    //     // loop 'j' quarks. The ones remaining after 'i'
    //     //for (unsigned int j = i; j < JVSize; j++){
    //     for (unsigned int j = i + 1; j < QVSize; j++){
    //       reco::GenParticle quarkj = quarkVector[j]; //quark j
    //       auto qjm0 = quarkj.mother(0);
    //       qjm = qjm0->pdgId();
    //       //cout << "qjm = " << qjm << endl;
    //       auto qj = quarkj.p4(); // quark j four vector 
    //       mdr = 100.;
    //       mii = 0;
    //       for (unsigned int jjj = 0; jjj < jVecCopy.size(); jjj++){ //jVecCopy size should be different 
    //         dr = 0.;
    //         dr = sqrt( (jVecCopy[jjj].eta()-qj.eta())*(jVecCopy[jjj].eta()-qj.eta()) + (jVecCopy[jjj].phi()-qj.phi())*(jVecCopy[jjj].phi()-qj.phi()) );
    //         //cout << "dr = " << dr << endl;
    //         if (dr < mdr) {
    //           mdr = dr;
    //           mii = jjj; // index of minimum dr quark 
    //         }

    //       }
    //       // Save whatever you need before deleting the matching quark 
    //       //cout << "Matched quark/jet dr = " << mdr << endl;
    //       //cout << "Matched jet index = " << mii << endl;
    //       auto jet2 = jVecCopy[mii].p4();

    //       // for (unsigned int iii = 0; iii < jVecCopy.size(); iii++){
    //       //   cout << "jVecCopy[" << iii << "] = " << jVecCopy[iii] << endl;
    //       // }

    //       jVecCopy.erase(jVecCopy.begin() + mii); // remove matched jet from cloned vector 

    //       // for (unsigned int iii = 0; iii < jVecCopy.size(); iii++){
    //       //   cout << "jVecCopy[" << iii << "] = " << jVecCopy[iii] << endl;
    //       // }

    //       // After each second (or 'j') quark is matched, you have a dijet pair (i jet and j jet)
    //       // which are matched to quarks based on dr.
    //       // The mothers of the quarks tell you if the quarks come from the same W or not.

    //       // If quarks have same mother particle type *** This may need to be adjusted when there is pileup or backgrounds
    //       // because in that case, one quark may come from a signal W and the other from a background/pileup W 
    //       if (qim == qjm) is_pair = 1; // only have this info when checking quarks 
    //       else is_pair = 0;
        
    //       // Might need to add more matching and non matching jet objects when there are 4 quarks 
    //       // This is because only one of each object is saved per event 

    //       if (is_pair){
    //       // matching dijet object 
    //       auto mdij = jet1 + jet2; // does 'auto' use more memory than giving the proper type? Might want to change this to speed things up in the future. 
    //       mdij_ = mdij;

    //       // matching diquark object 
    //       auto mdiq = qi + qj;
    //       mdiq_ = mdiq;
    //       }

    //       if (!is_pair){
    //       // non-matching diquark object 
    //       auto nmdiq = qi + qj;
    //       nmdiq_ = nmdiq;
          
    //       // non-matching dijet object 
    //       auto nmdij = jet1 + jet2;
    //       nmdij_ = nmdij;
    //       }

    //     }

    //   }

    // } // Loop performed if there are more jets than quarks 

    // }

    // //else cout << "There are more quarks than jets, and/or no jets at all. Skipping." << endl;
    // else {};

    // //cout << "Hello, outside of matching loop" << endl;

    // if (gen_phoVector.size() == 2){
    //   float gen_pho_pt_0 = 0., gen_pho_pt_1 = 0.;
    //   reco::GenParticle gen_pho_0 = gen_phoVector[0];
    //   reco::GenParticle gen_pho_1  = gen_phoVector[1];
    //   auto gen_pho_0_4vec = gen_pho_0.p4();
    //   auto gen_pho_1_4vec = gen_pho_1.p4();
    //   gen_pho_pt_0 = gen_pho_0_4vec.pt();
    //   gen_pho_pt_1 = gen_pho_1_4vec.pt();
    //   if (gen_pho_pt_0 > gen_pho_pt_1){
    //     l_genpho_ = gen_pho_0_4vec;
    //     sl_genpho_ = gen_pho_1_4vec;
    //   }

    //   else if (gen_pho_pt_1 > gen_pho_pt_0){
    //     l_genpho_ = gen_pho_1_4vec;
    //     sl_genpho_ = gen_pho_0_4vec; 
    //   }

    //   else{
    //     cout << "gen photon pt's are equal\n"; 
    //     l_genpho_ = gen_pho_0_4vec;
    //     sl_genpho_ = gen_pho_1_4vec;
    //   }

    // }

    // // Gen photons 
    // // float l_gen_pho_pt = -99;
    // // for (unsigned int i = 0; i < gen_electronVector.size(); i++ )
    // // {
    // //   reco::GenParticle current_gen_elec = gen_electronVector[i];
    // //   auto current_gen_elec_4vec = current_gen_elec.p4();
    // //   auto current_gen_elec_pt = current_gen_elec.pt();

    // //   // If current gen electron pt is greater than maximum so far, make it the leading pt gen electron 
    // //   if (current_gen_elec_pt > l_gen_elec_pt){ 
    // //     l_gen_pho_pt = current_gen_elec_pt; 
    // //     gen_leading_elec_ = current_gen_elec_4vec;

    // //   }
    // // }  

    // // Get leading and subleading GEN leptons 

    // // Get gen electrons

    // // Get leading pt gen_electron 
    // float l_gen_elec_pt = -99;
    // for (unsigned int i = 0; i < gen_electronVector.size(); i++ )
    // {
    //   reco::GenParticle current_gen_elec = gen_electronVector[i];
    //   auto current_gen_elec_4vec = current_gen_elec.p4();
    //   auto current_gen_elec_pt = current_gen_elec.pt();

    //   // If current gen electron pt is greater than maximum so far, make it the leading pt gen electron 
    //   if (current_gen_elec_pt > l_gen_elec_pt){ 
    //     l_gen_elec_pt = current_gen_elec_pt; 
    //     gen_leading_elec_ = current_gen_elec_4vec;

    //   }
    // }     

    // //if (l_gen_elec_pt == -99) leading_gen_elec_pt_ = -99;

    // // Get subleading pt gen_electron 
    // float sl_gen_elec_pt = -99;
    // for (unsigned int i = 0; i < gen_electronVector.size(); i++ )
    // {
    //   reco::GenParticle current_gen_elec = gen_electronVector[i];
    //   auto current_gen_elec_4vec = current_gen_elec.p4();
    //   auto current_gen_elec_pt = current_gen_elec.pt();

    //   // If current gen electron pt is greater than max subleading gen pt, and isn't the leading pt gen electron, make it the subleading gen electron 
    //   if ( (current_gen_elec_pt > sl_gen_elec_pt) && (current_gen_elec_pt != l_gen_elec_pt) ){ 
    //     sl_gen_elec_pt = current_gen_elec_pt; 
    //     gen_subleading_elec_ = current_gen_elec_4vec;

    //   }
    // } 

    // //if (sl_gen_elec_pt == -99) subleading_gen_elec_pt_ = -99;

    // // Get gen muons 

    // //cout << "gen_muonVector.size() = " << gen_muonVector.size() << endl;
    // // Get leading pt gen_muon
    // float l_gen_muon_pt = -99;
    // for (unsigned int i = 0; i < gen_muonVector.size(); i++ )
    // {
    //   reco::GenParticle current_gen_muon = gen_muonVector[i];
    //   auto current_gen_muon_4vec = current_gen_muon.p4();
    //   auto current_gen_muon_pt = current_gen_muon.pt();

    //   // If current gen muon pt is greater than maximum so far, make it the leading pt gen muon
    //   if (current_gen_muon_pt > l_gen_muon_pt){ 
    //     //l_gen_muon_pt = current_gen_muon_pt; 
    //     //leading_gen_muon_pt_ = l_gen_muon_pt;
    //     l_gen_muon_pt = current_gen_muon_pt; 
    //     gen_leading_muon_ = current_gen_muon_4vec;

    //   }
    // }     

    // //if (l_gen_muon_pt == -99) leading_gen_muon_pt_ = -99;

    // // Get subleading pt gen_muon
    // float sl_gen_muon_pt = -99;
    // for (unsigned int i = 0; i < gen_muonVector.size(); i++ )
    // {
    //   reco::GenParticle current_gen_muon = gen_muonVector[i];
    //   auto current_gen_muon_pt = current_gen_muon.pt();
    //   auto current_gen_muon_4vec = current_gen_muon.p4();

    //   // If current gen muon pt is greater than max subleading gen pt, and isn't the leading pt gen muon, make it the subleading gen muon
    //   if ( (current_gen_muon_pt > sl_gen_muon_pt) && (current_gen_muon_pt != l_gen_muon_pt) ){ 
    //     //sl_gen_muon_pt = current_gen_muon_pt; 
    //     //subleading_gen_muon_pt_ = sl_gen_muon_pt;
    //     sl_gen_muon_pt = current_gen_muon_pt; 
    //     gen_subleading_muon_ = current_gen_muon_4vec;

    //   }
    // } 


///////////////////////////////////////////////////////////////////////////////////////

// ^^^ GEN object operations above 

    //-- RECO 


    // Jets

    // unsigned int JVSize = JetVector_.size();
    // if (JVSize>0){
    //   for (unsigned int i = 0; i < JVSize; i++){
    //     flashgg::Jet ijet = JetVector_[i];
    //     cout << "ijet.puJetIdMVA() = " << ijet.puJetIdMVA() << endl;
    //   }
    // }

    //-- Diphotons 
    // unsigned int ndpho = diphoVector_.size(); // number of diphotons 
    // double tmp_dp_pt = 0, max_dp_pt = -99; // temporary diphoton pt 
    //bool test = 0;

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

      // Get EG MVA scores, passelectronveto, haspixelseed 
      lead_pho_EG_MVA_ = leading_photon.userFloat("PhotonMVAEstimatorRunIIFall17v1p1Values"); // v1p1 = 1.1 ? Better than 1.0 ?
      sublead_pho_EG_MVA_ = subleading_photon.userFloat("PhotonMVAEstimatorRunIIFall17v1p1Values");
      lead_pho_passElectronVeto_ = leading_photon.passElectronVeto();
      sublead_pho_passElectronVeto_ = subleading_photon.passElectronVeto();
      lead_pho_hasPixelSeed_ = leading_photon.hasPixelSeed();
      sublead_pho_hasPixelSeed_= subleading_photon.hasPixelSeed();

      auto l_pho_ = leading_photon.p4();
      auto sl_pho_ = subleading_photon.p4();
      leading_pho_ = l_pho_; 
      sub_leading_pho_ = sl_pho_;

      CMS_hgg_mass_ = dipho.mass();

      //----------------------------------------------------------------------------------------------------------------------------

      // For fggfinalfit, adding selections here because don't know how to filter workspace after dumper has run
      // Should turn this off if not creating a workspace 

      // bool pass_selections = 0;
      // bool lead_pass_TightPhoID = 0, sublead_pass_TightPhoID = 0; 

      // double n_good_electrons = electronVector_.size();
      // double n_good_muons = muonVector_.size();
      // double n_good_leptons = n_good_electrons + n_good_muons;
      // double n_good_jets = JetVector_.size();
      // double leading_pho_eta = l_pho_.eta(), sub_leading_pho_eta = sl_pho_.eta();

      // if (n_good_leptons == 1){
      //   if (n_good_jets >= 2 ){
      //     // leading photon 
      //     // EB 
      //     if (( abs(leading_pho_eta) > 0) && ( abs(leading_pho_eta) < 1.4442)){
      //       if (lead_pho_EG_MVA_ > 0.42) lead_pass_TightPhoID = 1; 
      //     }

      //     // EE 
      //     else if (( abs(leading_pho_eta) > 1.566) && ( abs(leading_pho_eta) < 2.5)){
      //       if (lead_pho_EG_MVA_ > 0.14) lead_pass_TightPhoID = 1;
      //     }

      //     // SubLeading Photon
      //     // EB 
      //     if (( abs(sub_leading_pho_eta) > 0) && ( abs(sub_leading_pho_eta) < 1.4442)){
      //       if (sublead_pho_EG_MVA_ > 0.42) sublead_pass_TightPhoID = 1; 
      //     }

      //     // EE 
      //     else if (( abs(sub_leading_pho_eta) > 1.566) && ( abs(sub_leading_pho_eta) < 2.5)){
      //       if (sublead_pho_EG_MVA_ > 0.14) sublead_pass_TightPhoID = 1;
      //     }

      //     if (lead_pass_TightPhoID && sublead_pass_TightPhoID){
            
      //       if  ((dipho.mass() > 118) && (dipho.mass() < 132)){
      //         pass_selections = 1;
      //       }


      //     }

      //   }
      // }

      // if (pass_selections){
      //   CMS_hgg_mass_ = dipho.mass();
      // }
      // else {
      //   CMS_hgg_mass_ = -99; 
      //   dZ_ = -999;
      // }
      //----------------------------------------------------------------------------------------------------------------------------

      //-- Photon Object Checks

      // cout << "supercluster eta: " << leading_photon.superCluster()->eta() << endl;

      // // Check etas 
      // auto l_pho_eta = l_pho_.eta();
      // auto sl_pho_eta = sl_pho_.eta();

      // // Check pts 
      // auto l_pho_pt = l_pho_.pt();
      // auto sl_pho_pt = sl_pho_.pt();

      // // cout << "supercluster eta: " << leading_photon.superCluster()->eta() << endl;
      // auto l_pho_SC_eta = leading_photon.superCluster()->eta();
      // auto sl_pho_SC_eta = subleading_photon.superCluster()->eta();

      // cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
      // cout << "photon EG ID 1:" << leading_photon.userFloat("PhotonMVAEstimatorRunIIFall17v1Values") << endl;
      // cout << "Conversionsafeelectronveto:" << leading_photon.passElectronVeto() << endl;
      // cout << "hasPixelSeed :" << leading_photon.hasPixelSeed() << endl;
      // // cout << "photon EG ID 2:" << leading_photon.userFloat("PhotonMVAEstimatorRunIIFall17v1p1Values") << endl;
      // cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;

// Requested UserFloat PhotonMVAEstimatorRunIIFall17v2Values is not available! Possible UserFloats are:
// EGMPhotonMVA PhotonMVAEstimatorRun2Spring15NonTrig25nsV2p1Values PhotonMVAEstimatorRun2Spring15NonTrig50nsV2p1Values PhotonMVAEstimatorRun2Spring16NonTrigV1Values PhotonMVAEstimatorRunIIFall17v1Values PhotonMVAEstimatorRunIIFall17v1p1Values ecalEnergyErrPostCorr ecalEnergyErrPreCorr ecalEnergyPostCorr ecalEnergyPreCorr energyScaleDown energyScaleGainDown energyScaleGainUp energyScaleStatDown energyScaleStatUp energyScaleSystDown energyScaleSystUp energyScaleUp energyScaleValue energySigmaDown energySigmaPhiDown energySigmaPhiUp energySigmaRhoDown energySigmaRhoUp energySigmaUp energySigmaValue energySmearNrSigma genIso phoChargedIsolation phoNeutralHadronIsolation phoPhotonIsolation phoWorstChargedIsolation rnd_g_E


      // if (l_pho_eta != l_pho_SC_eta){
      //   cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
      //   cout << "Leading Photon eta and SC eta don't match:" << endl;
      //   cout << "Leading Photon eta: " << l_pho_eta << endl;
      //   cout << "Leading Photon SC eta:" << l_pho_SC_eta << endl;
      //   cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
      // }

      // if (sl_pho_eta != sl_pho_SC_eta){
      //   cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
      //   cout << "Subleading Photon eta and SC eta don't match:" << endl;
      //   cout << "Subleading Photon eta: " << sl_pho_eta << endl;
      //   cout << "Subleading Photon SC eta:" << sl_pho_SC_eta << endl;
      //   cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
      // }

      // if (l_pho_pt <= 35.0 ){
      //   cout << "***************************************************************************************" << endl;
      //   cout << "leading photon pt: " << l_pho_pt << endl;
      //   cout << "***************************************************************************************" << endl;
      // }

      // if ( sl_pho_pt <= 25.0 ){
      //   cout << "***************************************************************************************" << endl;
      //   cout << "subleading photon pt: " << sl_pho_pt << endl;
      //   cout << "***************************************************************************************" << endl;
      // }      

      // if ( ( (abs(l_pho_eta) > 1.4442) && (abs(l_pho_eta) < 1.566) ) ||  (abs(l_pho_eta) > 2.5)   ){
      //   cout << "***************************************************************************************" << endl;
      //   cout << "leading photon eta: " << l_pho_eta << endl;
      //   cout << "***************************************************************************************" << endl;
      // }

      // if ( ( (abs(sl_pho_eta) > 1.4442) && (abs(sl_pho_eta) < 1.566) ) || (abs(sl_pho_eta) > 2.5)  ){
      //   cout << "***************************************************************************************" << endl;
      //   cout << "subleading photon eta: " << sl_pho_eta << endl;
      //   cout << "***************************************************************************************" << endl;
      // }

    //-- End Photon Object Checks

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

    // float SL_mT_WW = 0;
    // float SL_mT_W = 0;
    // float FL_mT_WW = 0;

    // // WW transverse mass 
    // if (METVector_.size() == 1)
    // {

    //   // SL 
      
    //   // MET 
    //   flashgg::Met met__ = METVector_[0];
    //   auto met_ = met__.p4();
    //   MET_fourvec_ = met_;

    //   // Two Jets
    //   if (JVSize >= 2){
    //     auto j0 = JetVector_[0].p4();
    //     auto j1 = JetVector_[1].p4();
    //     auto dij = j0 + j1;
    //     float dij_m = dij.M();
    //     // cout << "dijet mass = " << dij_m << endl;

    //     // if (abs(dij_m - 80.379) < 5.){

    //       reco::Candidate::LorentzVector leading_lepton_; 
    //       if ( (electronVector_.size() > 0) && (muonVector_.size() == 0) ){
    //         flashgg::Electron l_elec = electronVector_[0];
    //         auto l_elec_4vec = l_elec.p4();
    //         auto leading_elec = l_elec_4vec;
    //         leading_elec_ = leading_elec;
    //         leading_lepton_ = leading_elec_;
    //         auto WW = met_ + leading_lepton_ + j0 + j1;
    //         auto W = met_ + leading_lepton_;
    //         SL_mT_WW = WW.Mt();
    //         SL_mT_WW_ = SL_mT_WW;  
    //         SL_mT_W = W.Mt();
    //         SL_mT_W_ = SL_mT_W;
            
    //       }

    //       else if ( (muonVector_.size() > 0) && (electronVector_.size() == 0) ){
    //         flashgg::Muon l_muon = muonVector_[0];
    //         auto l_muon_4vec = l_muon.p4();
    //         auto leading_muon = l_muon_4vec;
    //         leading_muon_ = leading_muon;
    //         leading_lepton_ = leading_muon_;
    //         auto WW = met_ + leading_lepton_ + j0 + j1;
    //         auto W = met_ + leading_lepton_;
    //         SL_mT_WW = WW.Mt();
    //         SL_mT_WW_ = SL_mT_WW;  
    //         SL_mT_W = W.Mt();
    //         SL_mT_W_ = SL_mT_W;
    //       }

    //       else if ( (muonVector_.size() > 0) && (electronVector_.size() > 0) ){
    //         flashgg::Electron l_elec = electronVector_[0];
    //         auto l_elec_4vec = l_elec.p4();
    //         auto leading_elec = l_elec_4vec;
    //         leading_elec_ = leading_elec;
    //         flashgg::Muon l_muon = muonVector_[0];
    //         auto l_muon_4vec = l_muon.p4();
    //         auto leading_muon = l_muon_4vec;
    //         leading_muon_ = leading_muon;
    //         elec_pt = l_elec_4vec.pt();
    //         muon_pt = l_muon_4vec.pt();
    //         if (elec_pt > muon_pt){
    //           leading_lepton_ = leading_elec_;
    //           auto WW = met_ + leading_lepton_ + j0 + j1;
    //           auto W = met_ + leading_lepton_;
    //         SL_mT_WW = WW.Mt();
    //         SL_mT_WW_ = SL_mT_WW;  
    //         SL_mT_W = W.Mt();
    //         SL_mT_W_ = SL_mT_W;
    //         }
    //         else if (muon_pt > elec_pt){
    //           leading_lepton_ = leading_muon_;
    //           auto WW = met_ + leading_lepton_ + j0 + j1;
    //           auto W = met_ + leading_lepton_;
    //         SL_mT_WW = WW.Mt();
    //         SL_mT_WW_ = SL_mT_WW;  
    //         SL_mT_W = W.Mt();
    //         SL_mT_W_ = SL_mT_W;
    //         }

    //       }

    //     // }
      
    //   }



    //   // FL 

    //   // // Two Jets
    //   // if (JVSize >= 2){
    //   //   auto j0 = JetVector_[0].p4();
    //   //   auto j1 = JetVector_[1].p4();
    //   //   auto dij = j0 + j1;
    //   //   float dij_m = dij.M();
    //   //   cout << "dijet mass = " << dij_m << endl;

    //     // if (abs(dij_m - 80.379) < 5.){

    //       reco::Candidate::LorentzVector leading_lepton_; 
    //       reco::Candidate::LorentzVector subleading_lepton_; 
    //       if ( (electronVector_.size() >= 2) && (muonVector_.size() == 0) ){
    //         flashgg::Electron l_elec = electronVector_[0];
    //         flashgg::Electron sl_elec = electronVector_[1];
    //         auto l_elec_4vec = l_elec.p4();
    //         auto sl_elec_4vec = sl_elec.p4();
    //         auto leading_elec = l_elec_4vec;
    //         auto subleading_elec = sl_elec_4vec;
    //         leading_elec_ = leading_elec;
    //         subleading_elec_ = subleading_elec;
    //         leading_lepton_ = leading_elec_;
    //         subleading_lepton_ = subleading_elec_;
    //         auto WW = met_ + leading_lepton_ + subleading_lepton_;
    //         FL_mT_WW = WW.Mt();
    //         FL_mT_WW_ = FL_mT_WW;  
    //       }

    //       else if ( (muonVector_.size() >= 2) && (electronVector_.size() == 0) ){
    //         flashgg::Muon l_muon = muonVector_[0];
    //         flashgg::Muon sl_muon = muonVector_[1];
    //         auto l_muon_4vec = l_muon.p4();
    //         auto sl_muon_4vec = sl_muon.p4();
    //         auto leading_muon = l_muon_4vec;
    //         auto subleading_muon = sl_muon_4vec;
    //         leading_muon_ = leading_muon;
    //         subleading_muon_ = subleading_muon;
    //         leading_lepton_ = leading_muon_;
    //         subleading_lepton_ = subleading_muon_;
    //         auto WW = met_ + leading_lepton_ + subleading_lepton_;
    //         FL_mT_WW = WW.Mt();
    //         FL_mT_WW_ = FL_mT_WW;  
    //       }

    //       else if ( (muonVector_.size() > 0) && (electronVector_.size() > 0) ){
    //         // flashgg::Electron l_elec = electronVector_[0];
    //         // auto l_elec_4vec = l_elec.p4();
    //         // auto leading_elec = l_elec_4vec;
    //         // leading_elec_ = leading_elec;
    //         // flashgg::Muon l_muon = muonVector_[0];
    //         // auto l_muon_4vec = l_muon.p4();
    //         // auto leading_muon = l_muon_4vec;
    //         // leading_muon_ = leading_muon;
    //         // elec_pt = l_elec_4vec.pt();
    //         // muon_pt = l_muon_4vec.pt();
    //         // if (elec_pt > muon_pt){
    //         //   leading_lepton_ = leading_elec_;
    //         //   auto WW = met_ + leading_lepton_ + j0 + j1;
    //         //   SL_mT_WW = WW.Mt();
    //         //   SL_mT_WW_ = SL_mT_WW;  
    //         // }
    //         // else if (muon_pt > elec_pt){
    //         //   leading_lepton_ = leading_muon_;
    //         //   auto WW = met_ + leading_lepton_ + j0 + j1;
    //         //   SL_mT_WW = WW.Mt();
    //         //   SL_mT_WW_ = SL_mT_WW;  
    //         // }

    //       }

    //     // }
      
    //   // }



    //   // cout << "SL_mT_WW = " << SL_mT_WW << endl;
    //   // auto WW = met_ + leading_lepton_;
    //   // SL_mT_WW = W.Mt();
    //   // SL_mT_WW_ = SL_mT_WW;

    // }     

  } //end 