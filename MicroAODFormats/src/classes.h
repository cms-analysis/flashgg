#include "flashgg/MicroAODFormats/interface/Photon.h"
#include "flashgg/MicroAODFormats/interface/DiPhotonCandidate.h"
#include "flashgg/MicroAODFormats/interface/Jet.h"
#include "flashgg/MicroAODFormats/interface/Electron.h"
#include "DataFormats/Common/interface/Wrapper.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/Common/interface/Ptr.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/JetReco/interface/PileupJetIdentifier.h"

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

  flashgg::MinimalPileupJetIdentifier                                               pujetid;
  std::pair<edm::Ptr<reco::Vertex>,flashgg::MinimalPileupJetIdentifier>                    pair_ptr_vtx_pujetid;
  std::map<edm::Ptr<reco::Vertex>,flashgg::MinimalPileupJetIdentifier>                    map_ptr_vtx_pujetid;

  flashgg::Jet                                                      fgg_jet;
  edm::Wrapper<flashgg::Jet>                                    wrp_fgg_jet;
  std::vector<flashgg::Jet>                                     vec_fgg_jet;
  edm::Ptr<flashgg::Jet>                                        ptr_fgg_jet;
  edm::PtrVector<flashgg::Jet>                                     ptr_vec_fgg_jet;
  edm::Ptr<pat::Muon>                                        ptr_fgg_muon;
  std::vector<pat::Muon>                                        vec_fgg_muon;
  edm::PtrVector<pat::Muon>                                     ptr_vec_fgg_muon;
  edm::Wrapper<std::vector<flashgg::Jet> >                  wrp_vec_fgg_jet;
  edm::PtrVector<pat::PackedCandidate>                          ptr_vec_pac;
  std::map<edm::Ptr<reco::Vertex>,edm::PtrVector<pat::PackedCandidate> >    map_ptr_vtx_ptr_vec_pac;
  std::pair<edm::Ptr<reco::Vertex>,edm::PtrVector<pat::PackedCandidate> >   pai_ptr_vtx_ptr_vec_pac;
  edm::Wrapper<std::map<edm::Ptr<reco::Vertex>,edm::PtrVector<pat::PackedCandidate> > >     wrp_map_ptr_vtx_ptr_vec_pac;
  edm::Wrapper<std::pair<edm::Ptr<reco::Vertex>,edm::PtrVector<pat::PackedCandidate> > >    wrp_pai_ptr_vtx_ptr_vec_pac;
  std::map<edm::Ptr<reco::Vertex>,float>                    map_ptr_vtx_flo;
  std::pair<edm::Ptr<reco::Vertex>,float>                   pai_ptr_vtx_flo;
  flashgg::Electron						    fgg_ele;
  edm::Ptr<flashgg::Electron> 					  ptr_fgg_ele;
  edm::PtrVector<flashgg::Electron>			      vec_ptr_fgg_ele;
  edm::Wrapper<flashgg::Electron>				  wrp_fgg_ele;
  std::vector<flashgg::Electron>				  vec_fgg_ele;
  edm::Wrapper<std::vector<flashgg::Electron> >               wrp_vec_fgg_ele;					
  std::vector<reco::Conversion>					  vec_rec_con;		
};}
