import FWCore.ParameterSet.Config as cms

from RecoEgamma.EgammaTools.regressionModifier_cfi import regressionModifier 

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

                                              #correctionFile           = cms.FileInPath("flashgg/MicroAOD/data/transformation_80X_v4.root"),
                                              # correctionFile           = cms.FileInPath("flashgg/MicroAOD/data/transformation_Moriond17_v2.root"),
                                              correctionFile           = cms.FileInPath("flashgg/MicroAOD/data/transformation_Moriond17_AfterPreApr_v1.root"),

                                              # To apply correction for non5x5 r9, sieie, sipip, sieip set this variable True. Default value False.

                                              doNon5x5transformation   = cms.bool(True),
                                              non5x5correctionFile     = cms.FileInPath("flashgg/MicroAOD/data/transformation_Moriond17_non5x5_v2.root"),

                                              Debug                    = cms.bool(False),
                                              
                                              reRunRegression = cms.bool(True),
                                              reRunRegressionOnData = cms.bool(False),
                                              keepInitialEnergy = cms.bool(True),
                                              regressionConfig = regressionModifier.clone(manualBunchSpacing = cms.int32(25),              
                                                   rhoCollection = cms.InputTag("fixedGridRhoFastjetAll"),
                                                   vertexCollection = cms.InputTag("offlineSlimmedPrimaryVertices"),
                                                   autoDetectBunchSpacing = cms.bool(False),                                                                                          
                                                                                          ),
                                              ## regressionConfig = cms.PSet(
                                              ##      ## ugly copy/paste from RecoEgamma/EgammaTools/python/regressionModifier_cfi.py
                                              ##      ## include it rather...
                                              ##      modifierName    = cms.string('EGExtraInfoModifierFromDB'),  
                                              ##      autoDetectBunchSpacing = cms.bool(False),
                                              ##      applyExtraHighEnergyProtection = cms.bool(True),
                                              ##      bunchSpacingTag = cms.InputTag("bunchSpacingProducer"),
                                              ##      manualBunchSpacing = cms.int32(25),              
                                              ##      rhoCollection = cms.InputTag("fixedGridRhoFastjetAll"),
                                              ##      vertexCollection = cms.InputTag("offlineSlimmedPrimaryVertices"),
                                              ##      electron_config = cms.PSet( # EB, EE
                                              ##                                  regressionKey_25ns  = cms.vstring('gedelectron_EBCorrection_25ns', 'gedelectron_EECorrection_25ns'),
                                              ##                                  uncertaintyKey_25ns = cms.vstring('gedelectron_EBUncertainty_25ns', 'gedelectron_EEUncertainty_25ns'),
                                              ##                                  combinationKey_25ns   = cms.string('gedelectron_p4combination_25ns'),                                                   
                                              ##                                  regressionKey_50ns  = cms.vstring('gedelectron_EBCorrection_50ns', 'gedelectron_EECorrection_50ns'),
                                              ##                                  uncertaintyKey_50ns = cms.vstring('gedelectron_EBUncertainty_50ns', 'gedelectron_EEUncertainty_50ns'),
                                              ##                                  combinationKey_50ns   = cms.string('gedelectron_p4combination_50ns'),
                                              ##      ),
                                              ##            
                                              ##      photon_config   = cms.PSet( # EB, EE
                                              ##                                  regressionKey_25ns  = cms.vstring('gedphoton_EBCorrection_25ns', 'gedphoton_EECorrection_25ns'),
                                              ##                                  uncertaintyKey_25ns = cms.vstring('gedphoton_EBUncertainty_25ns', 'gedphoton_EEUncertainty_25ns'),
                                              ##                                  regressionKey_50ns  = cms.vstring('gedphoton_EBCorrection_50ns', 'gedphoton_EECorrection_50ns'),
                                              ##                                  uncertaintyKey_50ns = cms.vstring('gedphoton_EBUncertainty_50ns', 'gedphoton_EEUncertainty_50ns'),
                                              ##      ),
                                              ## ),
                                              
                                              doIsoCorrection = cms.bool(True),
                                              isoCorrectionFile = cms.FileInPath("flashgg/Taggers/data/pho_iso_corrections_hybrid_moriond17_v3.root")
                                              )
