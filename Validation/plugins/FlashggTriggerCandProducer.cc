#include "PhysicsTools/TagAndProbe/plugins/MiniAODTriggerCandProducer.h"
#include "flashgg/DataFormats/interface/Photon.h"

#include <DataFormats/Math/interface/deltaR.h>

template <>
bool MiniAODTriggerCandProducer<flashgg::Photon, pat::TriggerObjectStandAlone>::onlineOfflineMatching(edm::Ref<std::vector<flashgg::Photon> > ref, 
												      const std::vector<pat::TriggerObjectStandAlone>* triggerObjects, 
												      std::string filterLabel, float dRmin) {
  
  for (pat::TriggerObjectStandAlone obj : *triggerObjects) { 
    //obj.unpackPathNames(triggerNames); 
    if (obj.hasFilterLabel(filterLabel)) {
      float dR = deltaR(ref->superCluster()->position(), obj.p4());
      if (dR < dRmin)
	return true;
    }
  }

  return false;
}

typedef MiniAODTriggerCandProducer<flashgg::Photon, pat::TriggerObjectStandAlone> FlashggPhotonTriggerCandProducer;
DEFINE_FWK_MODULE(FlashggPhotonTriggerCandProducer);

