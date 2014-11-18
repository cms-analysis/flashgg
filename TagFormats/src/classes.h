#include "flashgg/TagFormats/interface/DiPhotonMVAResult.h"
#include "flashgg/TagFormats/interface/VBFMVAResult.h"
#include "flashgg/TagFormats/interface/VBFDiPhoDiJetMVAResult.h"
#include "flashgg/TagFormats/interface/DiPhotonUntaggedCategory.h"
#include "flashgg/TagFormats/interface/VBFTag.h"
#include "flashgg/MicroAODFormats/interface/DiPhotonCandidate.h"
#include "flashgg/TagFormats/interface/TTHhadronicTag.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/Common/interface/Wrapper.h"
#include "DataFormats/Common/interface/Ptr.h"
#include "flashgg/TagFormats/interface/TTHleptonicTag.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "flashgg/TagFormats/interface/VHlooseTag.h"
#include "flashgg/TagFormats/interface/VHtightTag.h"
#include <map>
#include <vector>

namespace  { struct dictionary {
	flashgg::DiPhotonMVAResult res;
	//  edm::Wrapper<flashgg::DiPhotonMVAResult> wrp_res;
	//  std::pair<edm::Ptr<flashgg::DiPhotonCandidate>,flashgg::DiPhotonMVAResult> pair_res;
	//  std::map<edm::Ptr<flashgg::DiPhotonCandidate>,flashgg::DiPhotonMVAResult> map_res;
	//  edm::Wrapper<std::pair<edm::Ptr<flashgg::DiPhotonCandidate>,flashgg::DiPhotonMVAResult> > wrp_pair_res;
	//  edm::Wrapper<std::map<edm::Ptr<flashgg::DiPhotonCandidate>,flashgg::DiPhotonMVAResult> > wrp_map_res;
	std::vector<flashgg::DiPhotonMVAResult> vec_res;
	edm::Wrapper<std::vector<flashgg::DiPhotonMVAResult> > wrp_vec_res;

	flashgg::VBFMVAResult vbf_res;
	std::vector<flashgg::VBFMVAResult> vec_vbf_res;
	edm::Wrapper<std::vector<flashgg::VBFMVAResult> > wrp_vec_vbf_res;

	flashgg::VBFDiPhoDiJetMVAResult vbfDiPhoDiJet_res;
	std::vector<flashgg::VBFDiPhoDiJetMVAResult> vec_vbfDiPhoDiJet_res;
	edm::Wrapper<std::vector<flashgg::VBFDiPhoDiJetMVAResult> > wrp_vec_vbfDiPhoDiJet_res;

	flashgg::DiPhotonTagBase tagbase;
	std::vector<flashgg::DiPhotonTagBase> vec_tagbase;
	edm::Wrapper<std::vector<flashgg::DiPhotonTagBase> > wrp_vec_tagbase;

	flashgg::DiPhotonUntaggedCategory untaggedcat;
	std::vector<flashgg::DiPhotonUntaggedCategory> vec_untaggedcat;
	edm::Wrapper<std::vector<flashgg::DiPhotonUntaggedCategory> > wrp_vec_untaggedcat;

	flashgg::VBFTag vbf_cat;
	std::vector<flashgg::VBFTag> vec_vbf_cat;
	edm::Wrapper<std::vector<flashgg::VBFTag> > wrp_vec_vbf_cat;

  	flashgg::TTHleptonicTag tthl;
  	std::vector<flashgg::TTHleptonicTag> vec_tthl;
  	edm::Wrapper<std::vector<flashgg::TTHleptonicTag> > wrp_vec_tthl;
  	edm::Ptr<pat::Electron> ptr_elec;
  	edm::Ptr<pat::Muon> ptr_muon;
	edm::PtrVector<pat::Muon> vec_ptr_muon;
	edm::PtrVector<flashgg::Jet> vec_ptr_jet;

	flashgg::TTHhadronicTag tthh;
	std::vector<flashgg::TTHhadronicTag> vec_tthh;
	edm::Wrapper<std::vector<flashgg::TTHhadronicTag> > wrp_vec_tthh;

	std::vector<edm::Ptr<flashgg::Jet> > vect_ptr_jet;

	edm::Ptr<flashgg::DiPhotonTagBase> Ptr_tagbase;
	edm::Wrapper<edm::Ptr<flashgg::DiPhotonTagBase> > wrp_ptr_tagbase;
	
	edm::OwnVector<flashgg::DiPhotonTagBase, edm::ClonePolicy<flashgg::DiPhotonTagBase> > ownvec_tagbase;
	edm::Wrapper<edm::OwnVector<flashgg::DiPhotonTagBase,edm::ClonePolicy<flashgg::DiPhotonTagBase> > > wrp_ownvec_tagbase;

	flashgg::VHlooseTag vhl;
  	std::vector<flashgg::VHlooseTag> vec_vhl;
  	edm::Wrapper<std::vector<flashgg::VHlooseTag> > wrp_vec_vhl;
  	edm::Ptr<pat::Electron> ptr_elec_vhl;
  	edm::Ptr<pat::Muon> ptr_muon_vhl;
	edm::PtrVector<pat::Muon> vec_ptr_muon_vhl;
	edm::PtrVector<flashgg::Jet> vec_ptr_jet_vhl;
	edm::PtrVector<pat::MET> vec_ptr_met_vhl;

	flashgg::VHtightTag vht;
  	std::vector<flashgg::VHtightTag> vec_vht;
  	edm::Wrapper<std::vector<flashgg::VHtightTag> > wrp_vec_vht;
  	edm::Ptr<pat::Electron> ptr_elec_vht;
  	edm::Ptr<pat::Muon> ptr_muon_vht;
	edm::PtrVector<pat::Muon> vec_ptr_muon_vht;
	edm::PtrVector<flashgg::Jet> vec_ptr_jet_vht;
	edm::PtrVector<pat::MET> vec_ptr_met_vht;

};}
