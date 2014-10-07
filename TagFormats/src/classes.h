#include "flashgg/TagFormats/interface/DiPhotonMVAResult.h"
#include "flashgg/TagFormats/interface/DiPhotonUntaggedCategory.h"
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
  flashgg::DiPhotonTagBase tagbase;
  std::vector<flashgg::DiPhotonTagBase> vec_tagbase;
  edm::Wrapper<std::vector<flashgg::DiPhotonTagBase> > wrp_vec_tagbase;
  flashgg::DiPhotonUntaggedCategory untaggedcat;
  std::vector<flashgg::DiPhotonUntaggedCategory> vec_untaggedcat;
  edm::Wrapper<std::vector<flashgg::DiPhotonUntaggedCategory> > wrp_vec_untaggedcat;
};}
