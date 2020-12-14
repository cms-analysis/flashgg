#include "DataFormats/Common/interface/Ptr.h"
#include "DataFormats/Common/interface/Wrapper.h"
#include "DataFormats/JetReco/interface/PileupJetIdentifier.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "flashgg/DataFormats/interface/GenDiPhoton.h"
#include "flashgg/DataFormats/interface/DiPhotonCandidate.h"
#include "flashgg/DataFormats/interface/DiPhotonMVAResult.h"
#include "flashgg/DataFormats/interface/NoTag.h"
#include "flashgg/DataFormats/interface/UntaggedTag.h"
#include "flashgg/DataFormats/interface/SigmaMpTTag.h"
#include "flashgg/DataFormats/interface/Electron.h"
#include "flashgg/DataFormats/interface/Muon.h"
#include "flashgg/DataFormats/interface/GenPhotonExtra.h"
#include "flashgg/DataFormats/interface/GenLeptonExtra.h"
#include "flashgg/DataFormats/interface/GenJetExtra.h"
#include "flashgg/DataFormats/interface/Jet.h"
#include "flashgg/DataFormats/interface/Met.h"
#include "flashgg/DataFormats/interface/Photon.h"
#include "flashgg/DataFormats/interface/SinglePhotonView.h"
#include "flashgg/DataFormats/interface/SingleVertexView.h"
#include "flashgg/DataFormats/interface/TTHHadronicTag.h"
#include "flashgg/DataFormats/interface/TTHLeptonicTag.h"
#include "flashgg/DataFormats/interface/THQLeptonicTag.h"
#include "flashgg/DataFormats/interface/THQLeptonicTagTruth.h"
#include "flashgg/DataFormats/interface/TTHDiLeptonTag.h"
#include "flashgg/DataFormats/interface/VBFDiPhoDiJetMVAResult.h"
#include "flashgg/DataFormats/interface/VBFMVAResult.h"
#include "flashgg/DataFormats/interface/VBFTag.h"
#include "flashgg/DataFormats/interface/VHhadMVAResult.h"
#include "flashgg/DataFormats/interface/VHHadronicTag.h"
#include "flashgg/DataFormats/interface/VHLooseTag.h"
#include "flashgg/DataFormats/interface/VHTightTag.h"
#include "flashgg/DataFormats/interface/WHLeptonicTag.h"
#include "flashgg/DataFormats/interface/ZHLeptonicTag.h"
#include "flashgg/DataFormats/interface/VHLeptonicLooseTag.h"
#include "flashgg/DataFormats/interface/VHMetTag.h"
#include "flashgg/DataFormats/interface/VHEtTag.h"
#include "flashgg/DataFormats/interface/DiMuonCandidate.h"   //JTao
#include "flashgg/DataFormats/interface/MuMuGammaCandidate.h"   //JTao
#include "flashgg/DataFormats/interface/PhotonJetCandidate.h" //Martina
#include "flashgg/DataFormats/interface/TagTruthBase.h"
#include "flashgg/DataFormats/interface/VBFTagTruth.h"
#include "flashgg/DataFormats/interface/VHTagTruth.h" //mplaner
#include "flashgg/DataFormats/interface/WeightedObject.h"
#include "flashgg/DataFormats/interface/PDFWeightObject.h"
#include "flashgg/DataFormats/interface/ZPlusJetTag.h"
#include "flashgg/DataFormats/interface/TagCandidate.h"
#include "flashgg/DataFormats/interface/TagAndProbeCandidate.h" //spigazzi
#include "flashgg/DataFormats/interface/DoubleHTag.h"
#include "flashgg/DataFormats/interface/HHWWggTag.h"
#include "flashgg/DataFormats/interface/HHWWggCandidate.h"
#include "flashgg/DataFormats/interface/VBFDoubleHTag.h"
#include <vector>
#include <map>

namespace  {
    struct dictionary {
        flashgg::WeightedObject                                             fgg_obj;
        
        flashgg::PDFWeightObject                                             fgg_pobj;
        edm::Ptr<flashgg::PDFWeightObject>                                ptr_fgg_pobj;
        edm::Wrapper<flashgg::PDFWeightObject>                            wrp_fgg_pobj;
        std::vector<flashgg::PDFWeightObject>                             vec_fgg_pobj;
        edm::Wrapper<std::vector<flashgg::PDFWeightObject> >               wrp_vec_fgg_pobj;


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

        edm::Ref<std::vector<flashgg::Photon>,flashgg::Photon,edm::refhelper::FindUsingAdvance<std::vector<flashgg::Photon>,flashgg::Photon> > ref_fgg_pho;
        std::vector<edm::Ref<std::vector<flashgg::Photon>,flashgg::Photon,edm::refhelper::FindUsingAdvance<std::vector<flashgg::Photon>,flashgg::Photon> > > vref_fgg_pho;
        edm::RefProd<std::vector<flashgg::Photon> >              rfp_fgg_pho; 
        edm::RefVector<std::vector<flashgg::Photon>,flashgg::Photon,edm::refhelper::FindUsingAdvance<std::vector<flashgg::Photon>,flashgg::Photon> >    refv_fgg_pho; 
        edm::Wrapper<edm::RefVector<std::vector<flashgg::Photon>,flashgg::Photon,edm::refhelper::FindUsingAdvance<std::vector<flashgg::Photon>,flashgg::Photon> > >  wrp_rv_fgg_pho;


        flashgg::GenDiPhoton                                        fgg_gdip;
        edm::Wrapper<flashgg::GenDiPhoton>                      wrp_fgg_gdip;
        std::vector<flashgg::GenDiPhoton>                       vec_fgg_gdip;
        edm::Wrapper<std::vector<flashgg::GenDiPhoton> >    wrp_vec_fgg_gdip;
        edm::Ptr<flashgg::GenDiPhoton>                          ptr_fgg_gdip;
        edm::Wrapper<edm::Ptr<flashgg::GenDiPhoton> >       wrp_ptr_fgg_gdip;
        std::vector<edm::Ptr<flashgg::GenDiPhoton> >        vec_ptr_fgg_gdip;
        edm::Wrapper<std::vector<edm::Ptr<flashgg::GenDiPhoton> > >   wrp_vec_ptr_fgg_gdip;

        //-----------J. Tao from IHEP-Beijing--------------
        flashgg::DiMuonCandidate                                        fgg_dim;
        edm::Wrapper<flashgg::DiMuonCandidate>                      wrp_fgg_dim;
        std::vector<flashgg::DiMuonCandidate>                       vec_fgg_dim;
        edm::Wrapper<std::vector<flashgg::DiMuonCandidate> >    wrp_vec_fgg_dim;
        edm::Ptr<flashgg::DiMuonCandidate>                          ptr_fgg_dim;
        edm::Wrapper<edm::Ptr<flashgg::DiMuonCandidate> >       wrp_ptr_fgg_dim;
        std::vector<edm::Ptr<flashgg::DiMuonCandidate> >        vec_ptr_fgg_dim;
        edm::Wrapper<std::vector<edm::Ptr<flashgg::DiMuonCandidate> > >   wrp_vec_ptr_fgg_dim;
        flashgg::MuMuGammaCandidate                                        fgg_mmg;
        edm::Wrapper<flashgg::MuMuGammaCandidate>                      wrp_fgg_mmg;
        std::vector<flashgg::MuMuGammaCandidate>                       vec_fgg_mmg;
        edm::Wrapper<std::vector<flashgg::MuMuGammaCandidate> >    wrp_vec_fgg_mmg;
        edm::Ptr<flashgg::MuMuGammaCandidate>                          ptr_fgg_mmg;
        edm::Wrapper<edm::Ptr<flashgg::MuMuGammaCandidate> >       wrp_ptr_fgg_mmg;
        std::vector<edm::Ptr<flashgg::MuMuGammaCandidate> >        vec_ptr_fgg_mmg;
        edm::Wrapper<std::vector<edm::Ptr<flashgg::MuMuGammaCandidate> > >   wrp_vec_ptr_fgg_mmg;
        //-----------J. Tao from IHEP-Beijing--------------

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
        std::pair<edm::Ptr<reco::Vertex>, flashgg::MinimalPileupJetIdentifier>                    pair_ptr_vtx_pujetid;
        std::map<edm::Ptr<reco::Vertex>, flashgg::MinimalPileupJetIdentifier>                    map_ptr_vtx_pujetid;

        flashgg::Jet                                                       fgg_jet;
        edm::Wrapper<flashgg::Jet>                                     wrp_fgg_jet;
        std::vector<flashgg::Jet>                                      vec_fgg_jet;
        edm::Ptr<flashgg::Jet>                                         ptr_fgg_jet;
        std::vector<std::vector<flashgg::Jet> >                    vec_vec_fgg_jet;
        edm::Wrapper<std::vector<flashgg::Jet> >                   wrp_vec_fgg_jet;
        edm::Wrapper<std::vector<std::vector<flashgg::Jet> > > wrp_vec_vec_fgg_jet;
        std::vector<pat::Muon>                                        vec_fgg_muon;
        flashgg::Muon						                                fgg_mu;
        edm::Ptr<flashgg::Muon> 					                    ptr_fgg_mu;
        edm::Wrapper<flashgg::Muon>				                        wrp_fgg_mu;
        std::vector<flashgg::Muon>				                        vec_fgg_mu;
        edm::Wrapper<std::vector<flashgg::Muon> >                   wrp_vec_fgg_mu;

        std::map<edm::Ptr<reco::Vertex>, float>                    map_ptr_vtx_flo;
        std::pair<edm::Ptr<reco::Vertex>, float>                   pai_ptr_vtx_flo;
        std::map<std::string, std::map<edm::Ptr<reco::Vertex>, float> >  map_str_ptr_vtx_flo;
        std::pair<std::string, std::map<edm::Ptr<reco::Vertex>, float> >  pai_str_ptr_vtx_flo;
        flashgg::Electron						    fgg_ele;
        edm::Ptr<flashgg::Electron> 					  ptr_fgg_ele;
        edm::Wrapper<flashgg::Electron>				  wrp_fgg_ele;
        std::vector<flashgg::Electron>				  vec_fgg_ele;
        edm::Wrapper<std::vector<flashgg::Electron> >               wrp_vec_fgg_ele;
        std::vector<reco::Conversion>					  vec_rec_con;

        flashgg::GenPhotonExtra                                                   fgg_pho_xtra;
        edm::Ptr<flashgg::GenPhotonExtra>                                     ptr_fgg_pho_xtra;
        edm::Wrapper<flashgg::GenPhotonExtra>                                 wrp_fgg_pho_xtra;
        std::vector<flashgg::GenPhotonExtra>                                  vec_fgg_pho_xtra;
        edm::Wrapper<std::vector<flashgg::GenPhotonExtra> >               wrp_vec_fgg_pho_xtra;

        flashgg::GenLeptonExtra                                                   fgg_lep_xtra;
        edm::Ptr<flashgg::GenLeptonExtra>                                     ptr_fgg_lep_xtra;
        std::vector<flashgg::GenLeptonExtra>                                  vec_fgg_lep_xtra;
        edm::Wrapper<flashgg::GenLeptonExtra>                                 wrp_fgg_lep_xtra;
        edm::Wrapper<std::vector<flashgg::GenLeptonExtra> >               wrp_vec_fgg_lep_xtra;

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

        flashgg::VHhadMVAResult vhHad_res;
        std::vector<flashgg::VHhadMVAResult> vec_vhHad_res;
        edm::Wrapper<std::vector<flashgg::VHhadMVAResult> > wrp_vec_vhHad_res;

        flashgg::ZPlusJetTag zpj_res;
        std::vector<flashgg::ZPlusJetTag> vec_zpj_res;
        edm::Wrapper<std::vector<flashgg::ZPlusJetTag> > wrp_vec_zpj_res;

        flashgg::VBFDiPhoDiJetMVAResult vbfDiPhoDiJet_res;
        std::vector<flashgg::VBFDiPhoDiJetMVAResult> vec_vbfDiPhoDiJet_res;
        edm::Wrapper<std::vector<flashgg::VBFDiPhoDiJetMVAResult> > wrp_vec_vbfDiPhoDiJet_res;

        flashgg::DiPhotonTagBase tagbase;
        std::vector<flashgg::DiPhotonTagBase> vec_tagbase;
        edm::Wrapper<std::vector<flashgg::DiPhotonTagBase> > wrp_vec_tagbase;

        flashgg::NoTag nocat;
        std::vector<flashgg::NoTag> vec_nocat;
        edm::Wrapper<std::vector<flashgg::NoTag> > wrp_vec_nocat;

        flashgg::UntaggedTag untaggedcat;
        std::vector<flashgg::UntaggedTag> vec_untaggedcat;
        edm::Wrapper<std::vector<flashgg::UntaggedTag> > wrp_vec_untaggedcat;

        flashgg::SigmaMpTTag sigmamptcat;
        std::vector<flashgg::SigmaMpTTag> vec_sigmamptcat;
        edm::Wrapper<std::vector<flashgg::SigmaMpTTag> > wrp_vec_sigmamptcat;

        flashgg::VBFTag vbf_cat;
        std::vector<flashgg::VBFTag> vec_vbf_cat;
        edm::Wrapper<std::vector<flashgg::VBFTag> > wrp_vec_vbf_cat;

        flashgg::TTHLeptonicTag tthl;
        std::vector<flashgg::TTHLeptonicTag> vec_tthl;
        edm::Wrapper<std::vector<flashgg::TTHLeptonicTag> > wrp_vec_tthl;

        flashgg::THQLeptonicTag thql;
        std::vector<flashgg::THQLeptonicTag> vec_thql;
        edm::Wrapper<std::vector<flashgg::THQLeptonicTag> > wrp_vec_thql;

        flashgg::THQLeptonicTagTruth thqlt;
        std::vector<flashgg::THQLeptonicTagTruth> vec_thqlt;
        edm::Wrapper<std::vector<flashgg::THQLeptonicTagTruth> > wrp_vec_thqlt;

        flashgg::DoubleHTag hh;
        std::vector<flashgg::DoubleHTag> vec_hh;
        edm::Wrapper<std::vector<flashgg::DoubleHTag> > wrp_vec_hh;
        //VBF Tag
        flashgg::VBFDoubleHTag vbfhh;
        std::vector<flashgg::VBFDoubleHTag> vec_vbfhh;
        edm::Wrapper<std::vector<flashgg::VBFDoubleHTag> > wrp_vec_vbfhh;

        flashgg::TTHDiLeptonTag tthd;
        std::vector<flashgg::TTHDiLeptonTag> vec_tthd;
        edm::Wrapper<std::vector<flashgg::TTHDiLeptonTag> > wrp_vec_tthd;

        edm::Ptr<reco::GenParticle> ptr_genp;

        flashgg::TTHHadronicTag tthh;
        std::vector<flashgg::TTHHadronicTag> vec_tthh;
        edm::Wrapper<std::vector<flashgg::TTHHadronicTag> > wrp_vec_tthh;

        std::vector<edm::Ptr<flashgg::Jet> > vect_ptr_jet;

        edm::Ptr<flashgg::DiPhotonTagBase> Ptr_tagbase;
        edm::Wrapper<edm::Ptr<flashgg::DiPhotonTagBase> > wrp_ptr_tagbase;

        flashgg::TagTruthBase truthbase;
        std::vector<flashgg::TagTruthBase> vec_truthbase;
        edm::Wrapper<std::vector<flashgg::TagTruthBase> > wrp_vec_truthbase;
        edm::Ptr<flashgg::TagTruthBase> ptr_truthbase;

        edm::OwnVector<flashgg::DiPhotonTagBase, edm::ClonePolicy<flashgg::DiPhotonTagBase> > ownvec_tagbase;
        edm::Wrapper<edm::OwnVector<flashgg::DiPhotonTagBase, edm::ClonePolicy<flashgg::DiPhotonTagBase> > > wrp_ownvec_tagbase;

        edm::OwnVector<flashgg::TagTruthBase, edm::ClonePolicy<flashgg::TagTruthBase> > ownvec_truthbase;
        edm::Wrapper<edm::OwnVector<flashgg::TagTruthBase, edm::ClonePolicy<flashgg::TagTruthBase> > > wrp_ownvec_truthbase;

        std::vector<flashgg::DiPhotonTagBase*> vec_ptr_tagbase;
        std::vector<flashgg::TagTruthBase*> vec_ptr_truthbase;

        flashgg::VBFTagTruth vbftt;
        std::vector<flashgg::VBFTagTruth> vec_vbftt;
        edm::Wrapper<std::vector<flashgg::VBFTagTruth> > wrp_vec_vbftt;

        flashgg::VHTagTruth vhtt;
        std::vector<flashgg::VHTagTruth> vec_vhtt;
        edm::Wrapper<std::vector<flashgg::VHTagTruth> > wrp_vec_vhtt;

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

        flashgg::VHMetTag vhmet;
        std::vector<flashgg::VHMetTag> vec_vhmet;
        edm::Wrapper<std::vector<flashgg::VHMetTag> > wrp_vec_vhmet;

        flashgg::ZHLeptonicTag zht;
        std::vector<flashgg::ZHLeptonicTag> vec_zht;
        edm::Wrapper<std::vector<flashgg::ZHLeptonicTag> > wrp_vec_zht;
        
        flashgg::VHLeptonicLooseTag vhll;
        std::vector<flashgg::VHLeptonicLooseTag> vec_vhll;
        edm::Wrapper<std::vector<flashgg::VHLeptonicLooseTag> > wrp_vec_vhll;

        flashgg::WHLeptonicTag whl;
        std::vector<flashgg::WHLeptonicTag> vec_whl;
        edm::Wrapper<std::vector<flashgg::WHLeptonicTag> > wrp_vec_whl;
        edm::Ptr<pat::Electron> ptr_elec_whl;
        edm::Ptr<pat::Muon> ptr_muon_whl;
        
        flashgg::VHEtTag vhet;
        std::vector<flashgg::VHEtTag> vec_vhet;
        edm::Wrapper<std::vector<flashgg::VHEtTag> > wrp_vec_vhet;

        edm::Ptr<pat::Electron> ptr_elec_vht;
        edm::Ptr<pat::Muon> ptr_muon_vht;

        std::pair<edm::Ptr<reco::Vertex>, edm::Ptr<pat::PackedCandidate> >  pair_ptr_vx_ptr_pat_pc;
        std::vector<std::pair<edm::Ptr<reco::Vertex>, edm::Ptr<pat::PackedCandidate> > >  vec_pair_ptr_vx_ptr_pat_pc;
        edm::Wrapper<std::vector<std::pair<edm::Ptr<reco::Vertex>, edm::Ptr<pat::PackedCandidate> > > > wrp_vec_pair_ptr_vx_ptr_pat_pc;

        std::vector<edm::Ptr<pat::Muon> >        vec_ptr_pat_mu;
        edm::Wrapper<std::vector<edm::Ptr<pat::Muon> > >   wrp_vec_ptr_pat_mu;

        std::vector<edm::Ptr<flashgg::Electron> >        vec_ptr_pat_ele;
        edm::Wrapper<std::vector<edm::Ptr<flashgg::Electron> > >   wrp_vec_ptr_pat_ele;

        std::vector<edm::Ptr<flashgg::Muon> >        vec_ptr_fgg_mu;
        edm::Wrapper<std::vector<edm::Ptr<flashgg::Muon> > >   wrp_vec_ptr_fgg_mu;

        std::vector<edm::Ptr<pat::MET> >        vec_ptr_pat_met;
        edm::Wrapper<std::vector<edm::Ptr<pat::MET> > >   wrp_vec_ptr_pat_met;

        // -- Martina
        flashgg::PhotonJetCandidate                                       fgj_dip;
        edm::Wrapper<flashgg::PhotonJetCandidate>                      wrp_fgj_dip;
        std::vector<flashgg::PhotonJetCandidate>                       vec_fgj_dip;
        edm::Wrapper<std::vector<flashgg::PhotonJetCandidate> >    wrp_vec_fgj_dip;
        edm::Ptr<flashgg::PhotonJetCandidate>                          ptr_fgj_dip;
        edm::Wrapper<edm::Ptr<flashgg::PhotonJetCandidate> >       wrp_ptr_fgj_dip;
        std::vector<edm::Ptr<flashgg::PhotonJetCandidate> >        vec_ptr_fgj_dip;
        edm::Wrapper<std::vector<edm::Ptr<flashgg::PhotonJetCandidate> > >   wrp_vec_ptr_fgj_dip;

        flashgg::TagCandidate                                        tags;
        edm::Wrapper<flashgg::TagCandidate>                      wrp_tags;
        std::vector<flashgg::TagCandidate>                       vec_tags;
        edm::Wrapper<std::vector<flashgg::TagCandidate> >    wrp_vec_tags;

        //---TagAndProbe candidate
        flashgg::TagAndProbeCandidate                                          fgg_tnp;
        edm::Wrapper<flashgg::TagAndProbeCandidate>                            wrp_fgg_tnp;
        std::vector<flashgg::TagAndProbeCandidate>                             vec_fgg_tnp;
        edm::Wrapper<std::vector<flashgg::TagAndProbeCandidate> >              wrp_vec_fgg_tnp;
        edm::Ptr<flashgg::TagAndProbeCandidate>                                ptr_fgg_tnp;
        edm::Wrapper<edm::Ptr<flashgg::TagAndProbeCandidate> >                 wrp_ptr_fgg_tnp;
        std::vector<edm::Ptr<flashgg::TagAndProbeCandidate> >                  vec_ptr_fgg_tnp;
        edm::Wrapper<std::vector<edm::Ptr<flashgg::TagAndProbeCandidate> > >   wrp_vec_ptr_fgg_tnp;

        //---HHWWggCandidate
        flashgg::HHWWggCandidate HHWWgg_c;
        std::vector<flashgg::HHWWggCandidate> vec_HHWWgg_c;
        edm::Wrapper<std::vector<flashgg::HHWWggCandidate> > wrp_vec_HHWWgg_c;

        //---HHWWggTag
        flashgg::HHWWggTag HHWWgg;
        std::vector<flashgg::HHWWggTag> vec_HHWWgg;
        edm::Wrapper<std::vector<flashgg::HHWWggTag> > wrp_vec_HHWWgg;

    };
}
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

