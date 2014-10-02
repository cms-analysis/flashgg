#ifndef FLASHgg_DiPhotonTagBase_h
#define FLASHgg_DiPhotonTagBase_h

#include "flashgg/MicroAODFormats/interface/DiPhotonCandidate.h"
#include "flashgg/TagFormats/interface/DiPhotonMVAResult.h"

namespace flashgg {

  class DiPhotonTagBase : public reco::CompositeCandidate {
  public:
    DiPhotonTagBase();
    ~DiPhotonTagBase();
    DiPhotonTagBase(edm::Ptr<flashgg::DiPhotonCandidate>,edm::Ptr<flashgg::DiPhotonMVAResult>);
    const flashgg::DiPhotonCandidate* diPhoton() const;
    const edm::Ptr<flashgg::DiPhotonMVAResult> diPhotonMVA() const;
  private:
    edm::Ptr<flashgg::DiPhotonMVAResult> mva_result_;
  };

}

#endif
