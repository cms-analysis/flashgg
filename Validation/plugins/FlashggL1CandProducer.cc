#include "PhysicsTools/TagAndProbe/plugins/MiniAODL1CandProducer.h"
#include "flashgg/DataFormats/interface/Photon.h"

typedef MiniAODL1CandProducer<flashgg::Photon> FlashggPhotonL1CandProducer;
DEFINE_FWK_MODULE(FlashggPhotonL1CandProducer);
