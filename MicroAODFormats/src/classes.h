#include "flashgg/MicroAODFormats/interface/Photon.h"
#include "flashgg/MicroAODFormats/interface/DiPhotonCandidate.h"
#include "flashgg/MicroAODFormats/interface/Jet.h"
#include "DataFormats/Common/interface/Wrapper.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/Common/interface/Ptr.h"
#include <vector>
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
  edm::Ptr<reco::Vertex>                                        ptr_rec_vtx;
  flashgg::Jet                                                      fgg_jet;
  edm::Wrapper<flashgg::Jet>                                    wrp_fgg_jet;
  std::vector<flashgg::Jet>                                     vec_fgg_jet;
  edm::Wrapper<std::vector<flashgg::Jet> >                  wrp_vec_fgg_jet;
  edm::PtrVector<pat::PackedCandidate>                          ptr_vec_pac;
  std::map<edm::Ptr<reco::Vertex>,edm::PtrVector<pat::PackedCandidate> >    map_ptr_vtx_ptr_vec_pac;
  std::pair<edm::Ptr<reco::Vertex>,edm::PtrVector<pat::PackedCandidate> >   pai_ptr_vtx_ptr_vec_pac;
  edm::Wrapper<std::map<edm::Ptr<reco::Vertex>,edm::PtrVector<pat::PackedCandidate> > >     wrp_map_ptr_vtx_ptr_vec_pac;
  edm::Wrapper<std::pair<edm::Ptr<reco::Vertex>,edm::PtrVector<pat::PackedCandidate> > >    wrp_pai_ptr_vtx_ptr_vec_pac;
  std::map<edm::Ptr<reco::Vertex>,float>                    map_ptr_vtx_flo;
  std::pair<edm::Ptr<reco::Vertex>,float>                   pai_ptr_vtx_flo;
};}
