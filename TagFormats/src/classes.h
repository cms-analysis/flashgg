#include "flashgg/TagFormats/interface/DiPhotonMVAResult.h"
#include "flashgg/MicroAODFormats/interface/DiPhotonCandidate.h"
#include "DataFormats/Common/interface/Wrapper.h"
#include "DataFormats/Common/interface/Ptr.h"
#include <map>

namespace  { struct dictionary {
  flashgg::DiPhotonMVAResult res;
  std::pair<edm::Ptr<flashgg::DiPhotonCandidate>,flashgg::DiPhotonMVAResult> pair_res;
  std::map<edm::Ptr<flashgg::DiPhotonCandidate>,flashgg::DiPhotonMVAResult> map_res;
  edm::Wrapper<std::pair<edm::Ptr<flashgg::DiPhotonCandidate>,flashgg::DiPhotonMVAResult> > wrp_pair_res;
  edm::Wrapper<std::map<edm::Ptr<flashgg::DiPhotonCandidate>,flashgg::DiPhotonMVAResult> > wrp_map_res;
};}
