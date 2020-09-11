import FWCore.ParameterSet.Config as cms

from RecoEgamma.EgammaTools.regressionModifier_cfi import regressionModifier 

import flashgg.Systematics.settings as settings

flashggUpdatedIdMVADiPhotons = cms.EDProducer("FlashggDiPhotonWithUpdatedPhoIdMVAProducer",
                                              src                      = cms.InputTag("flashggDiPhotons"),
                                              rhoFixedGridCollection   = cms.InputTag('fixedGridRhoAll'),

                                              photonIdMVAweightfile_EB = cms.FileInPath("flashgg/MicroAOD/data/MVAweights_80X_barrel_ICHEPvtx.xml"),
                                              photonIdMVAweightfile_EE = cms.FileInPath("flashgg/MicroAOD/data/MVAweights_80X_endcap_ICHEPvtx.xml"),

                                              useNewPhoId = cms.bool(True),
                                              is2017 = cms.bool(False), ## Turn this to "False" for 2016 analysis

                                              ## For 2016 Legacy ReReco
                                              effAreasConfigFile = cms.FileInPath("RecoEgamma/PhotonIdentification/data/Spring16/effAreaPhotons_cone03_pfPhotons_90percentBased.txt"),
                                              phoIsoPtScalingCoeff = cms.vdouble(0.0053,0.0034),
                                              phoIsoCutoff = cms.double(2.5),
                                              photonIdMVAweightfile_EB_new = cms.FileInPath("flashgg/MicroAOD/data/HggPhoId_barrel_Moriond2017_wRhoRew.weights.xml"),
                                              photonIdMVAweightfile_EE_new = cms.FileInPath("flashgg/MicroAOD/data/HggPhoId_endcap_Moriond2017_wRhoRew.weights.xml"),

                                              ## For 2017 ReReco
                                             # effAreasConfigFile = cms.FileInPath("RecoEgamma/PhotonIdentification/data/Fall17/effAreaPhotons_cone03_pfPhotons_90percentBased_TrueVtx.txt"),  #for 2017 it is not used at all
                                              photonIdMVAweightfile_EB_2017 = cms.FileInPath("flashgg/MicroAOD/data/HggPhoId_94X_barrel_BDT_v2.weights.xml"),
                                              photonIdMVAweightfile_EE_2017 = cms.FileInPath("flashgg/MicroAOD/data/HggPhoId_94X_endcap_BDT_v2.weights.xml"),

                                              ## Shower shape correction (5x5)
                                              do5x5correction          = cms.bool(False), ## Turn this off to remove 5x5 shower shape corrections
                                              # correctionFile           = cms.FileInPath("flashgg/MicroAOD/data/transformation5x5_Legacy2016_v1.root"), ## for Legacy2016 
                                              correctionFile           = cms.FileInPath("flashgg/MicroAOD/data/transformation_Moriond17_AfterPreApr_v1.root"),  ## for Rereco2017

                                              # To apply correction for non5x5 r9, sieie, sipip, sieip set this variable True. Default value False.

                                              doNon5x5transformation   = cms.bool(True), ## Turn this off to remove non5x5 corrections
                                              non5x5correctionFile     = cms.FileInPath("flashgg/MicroAOD/data/transformation_Moriond17_non5x5_v2.root"),

                                              Debug                    = cms.bool(False),
                                              
                                              reRunRegression = cms.bool(False),
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
                                              
                                              doIsoCorrection = cms.bool(False), ## Turned off for Rereco2017. Turn this on for Legacy2016.
                                              #isoCorrectionFile = cms.FileInPath("flashgg/Taggers/data/pho_iso_corrections_hybrid_Legacy2016_v1.root") ## for Legacy2016
                                              
                                              # For Updated photon MVA study 
                                              SuperClusterTag                 = cms.InputTag("particleFlowSuperClusterECAL"),

                                              )
