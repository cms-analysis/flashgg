#include "EgammaAnalysis/TnPTreeProducer/plugins/SelectorByValueMap.h"
#include "flashgg/DataFormats/interface/Photon.h"

typedef SelectorByValueMap<flashgg::Photon, float> FlashggPhotonSelectorByDoubleValueMap;
DEFINE_FWK_MODULE(FlashggPhotonSelectorByDoubleValueMap);

typedef SelectorByValueMap<flashgg::Photon, bool> FlashggPhotonSelectorByValueMap;
DEFINE_FWK_MODULE(FlashggPhotonSelectorByValueMap);
