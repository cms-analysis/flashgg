#include "DataFormats/Common/interface/Ptr.h"
#include "DataFormats/Common/interface/Wrapper.h"
#include "DataFormats/JetReco/interface/PileupJetIdentifier.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "flashgg/DataFormats/interface/DiPhotonCandidate.h"
#include "flashgg/DataFormats/interface/DiPhotonMVAResult.h"
#include "flashgg/DataFormats/interface/DiPhotonUntaggedCategory.h"
#include "flashgg/DataFormats/interface/Electron.h"
#include "flashgg/DataFormats/interface/GenPhotonExtra.h"
#include "flashgg/DataFormats/interface/Jet.h"
#include "flashgg/DataFormats/interface/Photon.h"
#include "flashgg/DataFormats/interface/SinglePhotonView.h"
#include "flashgg/DataFormats/interface/SingleVertexView.h" 
#include "flashgg/DataFormats/interface/TTHHadronicTag.h"
#include "flashgg/DataFormats/interface/TTHLeptonicTag.h"
#include "flashgg/DataFormats/interface/VBFDiPhoDiJetMVAResult.h"
#include "flashgg/DataFormats/interface/VBFMVAResult.h"
#include "flashgg/DataFormats/interface/VBFTag.h"
#include "flashgg/DataFormats/interface/VHHadronicTag.h"
#include "flashgg/DataFormats/interface/VHLooseTag.h"
#include "flashgg/DataFormats/interface/VHTightTag.h"
#include <vector>
#include <map>

namespace  { struct dictionary {
        flashgg::Photon                                                   fgg_pho;
        edm::Ptr<flashgg::Photon>                                     ptr_fgg_pho;
        edm::Wrapper<flashgg::Photon>                                 wrp_fgg_pho;
        std::vector<flashgg::Photon>                                  vec_fgg_pho;
        edm::Wrapper<std::vector<flashgg::Photon> >               wrp_vec_fgg_pho;
        flashgg::DiPhotonCandidate                                        fgg_dip;
        edm::Wrapper<flashgg::DiPhotonCandidate>                      wrp_fgg_dip;
        std::vector<flashgg::DiPhotonCandidate>                       vec_fgg_dip;
        edm::Wrapper<std::vector<flashgg::DiPhotonCandidate> >    wrp_vec_fgg_dip;
        edm::Ptr<flashgg::DiPhotonCandidate>                          ptr_fgg_dip;
        edm::Wrapper<edm::Ptr<flashgg::DiPhotonCandidate> >       wrp_ptr_fgg_dip;
        std::vector<edm::Ptr<flashgg::DiPhotonCandidate> >        vec_ptr_fgg_dip;
        edm::Wrapper<std::vector<edm::Ptr<flashgg::DiPhotonCandidate> > >   wrp_vec_ptr_fgg_dip;

        edm::Ptr<reco::Vertex>                                        ptr_rec_vtx;
        std::vector<edm::Ptr<reco::Vertex> >                      vec_ptr_rec_vtx;

        flashgg::SinglePhotonView                                      fgg_phoview;
        edm::Ptr<flashgg::SinglePhotonView>                        ptr_fgg_phoview;
        edm::Wrapper<flashgg::SinglePhotonView>                    wrp_fgg_phoview;
        std::vector<flashgg::SinglePhotonView>                     vec_fgg_phoview;
        edm::Wrapper<std::vector<flashgg::SinglePhotonView> >  wrp_vec_fgg_phoview;

        flashgg::SingleVertexView                                      fgg_vtxview;
        edm::Ptr<flashgg::SingleVertexView>                        ptr_fgg_vtxview;
        edm::Wrapper<flashgg::SingleVertexView>                    wrp_fgg_vtxview;
        std::vector<flashgg::SingleVertexView>                     vec_fgg_vtxview;
        edm::Wrapper<std::vector<flashgg::SingleVertexView> >  wrp_vec_fgg_vtxview;


        flashgg::MinimalPileupJetIdentifier                                               pujetid;
        std::pair<edm::Ptr<reco::Vertex>,flashgg::MinimalPileupJetIdentifier>                    pair_ptr_vtx_pujetid;
        std::map<edm::Ptr<reco::Vertex>,flashgg::MinimalPileupJetIdentifier>                    map_ptr_vtx_pujetid;

        flashgg::Jet                                                      fgg_jet;
        edm::Wrapper<flashgg::Jet>                                    wrp_fgg_jet;
        std::vector<flashgg::Jet>                                     vec_fgg_jet;
        edm::Ptr<flashgg::Jet>                                        ptr_fgg_jet;
        edm::Ptr<pat::Muon>                                        ptr_fgg_muon;
        std::vector<pat::Muon>                                        vec_fgg_muon;
        edm::Wrapper<std::vector<flashgg::Jet> >                  wrp_vec_fgg_jet;
        std::map<edm::Ptr<reco::Vertex>,float>                    map_ptr_vtx_flo;
        std::pair<edm::Ptr<reco::Vertex>,float>                   pai_ptr_vtx_flo;
        flashgg::Electron						    fgg_ele;
        edm::Ptr<flashgg::Electron> 					  ptr_fgg_ele;
        edm::Wrapper<flashgg::Electron>				  wrp_fgg_ele;
        std::vector<flashgg::Electron>				  vec_fgg_ele;
        edm::Wrapper<std::vector<flashgg::Electron> >               wrp_vec_fgg_ele;					
        std::vector<reco::Conversion>					  vec_rec_con;		


        edm::Ptr<pat::PackedGenParticle>                                     ptr_pat_pak_cand;
        flashgg::GenPhotonExtra                                                   fgg_pho_xtra;
        edm::Ptr<flashgg::GenPhotonExtra>                                     ptr_fgg_pho_xtra;
        edm::Wrapper<flashgg::GenPhotonExtra>                                 wrp_fgg_pho_xtra;
        std::vector<flashgg::GenPhotonExtra>                                  vec_fgg_pho_xtra;
        edm::Wrapper<std::vector<flashgg::GenPhotonExtra> >               wrp_vec_fgg_pho_xtra;

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

        flashgg::TTHLeptonicTag tthl;
        std::vector<flashgg::TTHLeptonicTag> vec_tthl;
        edm::Wrapper<std::vector<flashgg::TTHLeptonicTag> > wrp_vec_tthl;
        edm::Ptr<pat::Electron> ptr_elec;
        edm::Ptr<pat::Muon> ptr_muon;

        flashgg::TTHHadronicTag tthh;
        std::vector<flashgg::TTHHadronicTag> vec_tthh;
        edm::Wrapper<std::vector<flashgg::TTHHadronicTag> > wrp_vec_tthh;

        std::vector<edm::Ptr<flashgg::Jet> > vect_ptr_jet;

        edm::Ptr<flashgg::DiPhotonTagBase> Ptr_tagbase;
        edm::Wrapper<edm::Ptr<flashgg::DiPhotonTagBase> > wrp_ptr_tagbase;

        edm::OwnVector<flashgg::DiPhotonTagBase, edm::ClonePolicy<flashgg::DiPhotonTagBase> > ownvec_tagbase;
        edm::Wrapper<edm::OwnVector<flashgg::DiPhotonTagBase,edm::ClonePolicy<flashgg::DiPhotonTagBase> > > wrp_ownvec_tagbase;

        flashgg::VHLooseTag vhl;
        std::vector<flashgg::VHLooseTag> vec_vhl;
        edm::Wrapper<std::vector<flashgg::VHLooseTag> > wrp_vec_vhl;
        edm::Ptr<pat::Electron> ptr_elec_vhl;
        edm::Ptr<pat::Muon> ptr_muon_vhl;

        flashgg::VHTightTag vht;
        std::vector<flashgg::VHTightTag> vec_vht;
        edm::Wrapper<std::vector<flashgg::VHTightTag> > wrp_vec_vht;

        flashgg::VHHadronicTag vhhad;
        std::vector<flashgg::VHHadronicTag> vec_vhhad;
        edm::Wrapper<std::vector<flashgg::VHHadronicTag> > wrp_vec_vhhad;

        edm::Ptr<pat::Electron> ptr_elec_vht;
        edm::Ptr<pat::Muon> ptr_muon_vht;
        
				std::pair<edm::Ptr<reco::Vertex>,edm::Ptr<pat::PackedCandidate> >  pair_ptr_vx_ptr_pat_pc;
				std::vector<std::pair<edm::Ptr<reco::Vertex>,edm::Ptr<pat::PackedCandidate> > >  vec_pair_ptr_vx_ptr_pat_pc;
				edm::Wrapper<std::vector<std::pair<edm::Ptr<reco::Vertex>,edm::Ptr<pat::PackedCandidate> > > > wrp_vec_pair_ptr_vx_ptr_pat_pc;

        std::vector<edm::Ptr<pat::Muon> >        vec_ptr_pat_mu;
        edm::Wrapper<std::vector<edm::Ptr<pat::Muon> > >   wrp_vec_ptr_pat_mu;

        std::vector<edm::Ptr<flashgg::Electron> >        vec_ptr_pat_ele;
        edm::Wrapper<std::vector<edm::Ptr<flashgg::Electron> > >   wrp_vec_ptr_pat_ele;

        std::vector<edm::Ptr<pat::MET> >        vec_ptr_pat_met;
        edm::Wrapper<std::vector<edm::Ptr<pat::MET> > >   wrp_vec_ptr_pat_met;
};}
