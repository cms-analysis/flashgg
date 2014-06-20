#include "flashgg/MicroAODAlgos/interface/PhotonPreselectorBase.h"
#include "DataFormats/Common/interface/Handle.h"
#include "flashgg/MicroAODFormats/interface/Photon.h"
#include "DataFormats/Common/interface/PtrVector.h"

namespace flashgg {
  
  class NoPhotonPreselector : public PhotonPreselectorBase {
  public:
    NoPhotonPreselector(const edm::ParameterSet& conf) :
      PhotonPreselectorBase(conf) {}

    bool ispreselected(const edm::Ptr<pat::Photon>&, const edm::PtrVector<pat::PackedCandidate>&) const override;

  };

  bool NoPhotonPreselector::ispreselected(const edm::Ptr<pat::Photon>& g, const edm::PtrVector<pat::PackedCandidate>& pfcandidatePointers) const {
    return true;
  }

}

DEFINE_EDM_PLUGIN(FlashggPhotonPreselectorFactory,
		  flashgg::NoPhotonPreselector,
		  "FlashggNoPhotonPreselector");
