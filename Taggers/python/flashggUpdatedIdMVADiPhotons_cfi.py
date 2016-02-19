import FWCore.ParameterSet.Config as cms

flashggUpdatedIdMVADiPhotons = cms.EDProducer("FlashggDiPhotonWithUpdatedPhoIdMVAProducer",
                                              src=cms.InputTag("flashggDiPhotons"),
                                              rhoFixedGridCollection = cms.InputTag('fixedGridRhoAll'),
                                              photonIdMVAweightfile_EB = cms.FileInPath("flashgg/MicroAOD/data/MVAweights_76X_25ns_barrel.xml"),
                                              photonIdMVAweightfile_EE = cms.FileInPath("flashgg/MicroAOD/data/MVAweights_76X_25ns_endcap.xml"),
                                              Debug=cms.bool(False)
                                              )
