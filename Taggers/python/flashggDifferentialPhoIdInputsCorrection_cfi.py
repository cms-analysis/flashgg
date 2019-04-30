import FWCore.ParameterSet.Config as cms

from RecoEgamma.EgammaTools.regressionModifier_cfi import regressionModifier 

flashggDifferentialPhoIdInputsCorrection = cms.EDProducer("FlashggDifferentialPhoIdInputsCorrector",
                                                          diphotonSrc              = cms.InputTag("flashggDiPhotons"),
                                                          rhoFixedGridCollection   = cms.InputTag('fixedGridRhoAll'),
                                                          
                                                          photonIdMVAweightfile_EB = cms.FileInPath("flashgg/MicroAOD/data/MVAweights_80X_barrel_ICHEPvtx.xml"),
                                                          photonIdMVAweightfile_EE = cms.FileInPath("flashgg/MicroAOD/data/MVAweights_80X_endcap_ICHEPvtx.xml"),
                                                          

                                                          ## Shower shape correction (5x5)
                                                          reRunRegression = cms.bool(False),
                                                          regressionConfig = regressionModifier.clone(manualBunchSpacing = cms.int32(25),              
                                                                                                      rhoCollection = cms.InputTag("fixedGridRhoFastjetAll"),
                                                                                                      vertexCollection = cms.InputTag("offlineSlimmedPrimaryVertices"),
                                                                                                      autoDetectBunchSpacing = cms.bool(False)
                                                                                                  )
)
