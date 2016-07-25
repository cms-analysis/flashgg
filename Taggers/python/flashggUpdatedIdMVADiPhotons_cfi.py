import FWCore.ParameterSet.Config as cms

flashggUpdatedIdMVADiPhotons = cms.EDProducer("FlashggDiPhotonWithUpdatedPhoIdMVAProducer",
                                              src                      = cms.InputTag("flashggDiPhotons"),
                                              rhoFixedGridCollection   = cms.InputTag('fixedGridRhoAll'),
                                              #photonIdMVAweightfile_EB = cms.FileInPath("flashgg/MicroAOD/data/MVAweights_76X_25ns_r9shift_barrel.xml"),
                                              #photonIdMVAweightfile_EB = cms.FileInPath("flashgg/MicroAOD/data/MVAweights_76X_25ns_barrel.xml"),
                                              #photonIdMVAweightfile_EB = cms.FileInPath("flashgg/MicroAOD/data/MVAweights_76X_25ns_r9s4EtaWshift_barrel.xml"),
                                              #photonIdMVAweightfile_EE = cms.FileInPath("flashgg/MicroAOD/data/MVAweights_76X_25ns_endcap.xml"),
#                                              photonIdMVAweightfile_EB = cms.FileInPath("flashgg/MicroAOD/data/MVAweights_80X_25ns_barrel_MoriondVtx.xml"),
#                                              photonIdMVAweightfile_EE = cms.FileInPath("flashgg/MicroAOD/data/MVAweights_80X_25ns_endcap_MoriondVtx.xml"),
                                              photonIdMVAweightfile_EB = cms.FileInPath("flashgg/MicroAOD/data/MVAweights_80X_barrel_ICHEPvtx.xml"),
                                              photonIdMVAweightfile_EE = cms.FileInPath("flashgg/MicroAOD/data/MVAweights_80X_endcap_ICHEPvtx.xml"),
                                              # commenting out this parameter will disable all corrections performed by this module
                                              correctionFile           = cms.FileInPath("flashgg/MicroAOD/data/transformation_80X_v4.root"),
                                              Debug                    = cms.bool(False)
                                              )
