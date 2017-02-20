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

                                              useNewPhoId = cms.bool(True),

                                              #photonIdMVAweightfile_EB_new = cms.FileInPath("flashgg/MicroAOD/data/MVAweights_80X_barrel_ICHEPvtx_r9s4EtaWshift_wLowMass.xml"),
                                              #photonIdMVAweightfile_EE_new = cms.FileInPath("flashgg/MicroAOD/data/MVAweights_80X_endcap_ICHEPvtx_r9s4EtaWshift_wLowMass.xml"),

                                              effAreasConfigFile = cms.FileInPath("RecoEgamma/PhotonIdentification/data/Spring16/effAreaPhotons_cone03_pfPhotons_90percentBased.txt"),
                                              phoIsoPtScalingCoeff = cms.vdouble(0.0053,0.0034),
                                              phoIsoCutoff = cms.double(2.5),

                                              photonIdMVAweightfile_EB_new = cms.FileInPath("flashgg/MicroAOD/data/HggPhoId_barrel_Moriond2017_wRhoRew.weights.xml"),
                                              photonIdMVAweightfile_EE_new = cms.FileInPath("flashgg/MicroAOD/data/HggPhoId_endcap_Moriond2017_wRhoRew.weights.xml"),

                                              # commenting out this parameter will disable all corrections performed by this module
                                              correctionFile           = cms.FileInPath("flashgg/MicroAOD/data/transformation_Moriond17_AfterPreApr_v1.root"),
                                              # To apply correction for non5x5 r9, sieie, sipip, sieip set this variable True. Default value False.
                                              doNon5x5transformation   = cms.bool(False),
                                              non5x5correctionFile     = cms.FileInPath("flashgg/MicroAOD/data/transformation_Moriond17_non5x5_v1.root"),

                                              Debug                    = cms.bool(False)
                                              )
