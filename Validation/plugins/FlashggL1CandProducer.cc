#include "EgammaAnalysis/TnPTreeProducer/plugins/MiniAODL1CandProducer.h"
//#include "EgammaAnalysis/TnPTreeProducer/plugins/MiniAODL1Stage2CandProducer.h"
 

#include "flashgg/DataFormats/interface/Photon.h"

typedef MiniAODL1CandProducer<flashgg::Photon> FlashggPhotonL1CandProducer;
DEFINE_FWK_MODULE(FlashggPhotonL1CandProducer);

//typedef MiniAODL1Stage2CandProducer<flashgg::Photon> FlashggPhotonL1Stage2CandProducer;
//DEFINE_FWK_MODULE(FlashggPhotonL1Stage2CandProducer);
