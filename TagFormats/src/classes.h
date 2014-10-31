#include "flashgg/TagFormats/interface/DiPhotonMVAResult.h"
#include "flashgg/TagFormats/interface/VBFMVAResult.h"
#include "flashgg/TagFormats/interface/VBFDiPhoDiJetMVAResult.h"
#include "flashgg/TagFormats/interface/DiPhotonUntaggedCategory.h"
#include "flashgg/TagFormats/interface/VBFTag.h"
#include "flashgg/MicroAODFormats/interface/DiPhotonCandidate.h"
#include "DataFormats/Common/interface/Wrapper.h"
#include "DataFormats/Common/interface/Ptr.h"
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

	std::vector<std::vector<flashgg::DiPhotonTagBase> >  vec_tags;
	edm::Wrapper<std::vector<std::vector<flashgg::DiPhotonTagBase> > > wrp_vec_tags;

	std::vector<std::vector<flashgg::DiPhotonUntaggedCategory> >  un_vec_tags;
	edm::Wrapper<std::vector<std::vector<flashgg::DiPhotonUntaggedCategory> > > wrp_un_vec_tags;

	edm::View<flashgg::DiPhotonTagBase>  test;
	std::vector<edm::View<flashgg::DiPhotonTagBase> > vec_test;
	edm::Wrapper<std::vector<edm::View<flashgg::DiPhotonTagBase> > > wrp_vec_test;

	edm::Ptr<flashgg::DiPhotonTagBase> ptr_tagbase; 
	edm::Wrapper<edm::Ptr<flashgg::DiPhotonTagBase> > wrp_ptr_tagbase;

	edm::PtrVector<flashgg::DiPhotonTagBase> ptrvec_tagbase; 
	edm::Wrapper<edm::PtrVector<flashgg::DiPhotonTagBase> > wrp_ptrvec_tagbase;


};}
