import importlib

import FWCore.ParameterSet.Config as cms

flashggDiPhotonSystematics = cms.EDProducer('FlashggDiPhotonSystematicProducer',
                src = cms.InputTag("flashggDifferentialPhoIdInputsCorrection"),
                SystMethods2D = cms.VPSet(),
                SystMethods = cms.VPSet()
)

def setupDiPhotonSystematics( process, options ):
   # print'*******************************************************************************************************'
   process.load("flashgg.Systematics."+options.metaConditions["flashggDiPhotonSystematics"])
   sysmodule = importlib.import_module("flashgg.Systematics."+options.metaConditions["flashggDiPhotonSystematics"])
   flashggDiPhotonSystematics.SystMethods.append(sysmodule.MCScaleHighR9EB_EGM)
   flashggDiPhotonSystematics.SystMethods.append(sysmodule.MCScaleLowR9EB_EGM)
   flashggDiPhotonSystematics.SystMethods.append(sysmodule.MCScaleHighR9EE_EGM)
   flashggDiPhotonSystematics.SystMethods.append(sysmodule.MCScaleLowR9EE_EGM)
   # flashggDiPhotonSystematics.SystMethods.append(sysmodule.MCScaleGain6EB_EGM)
   # flashggDiPhotonSystematics.SystMethods.append(sysmodule.MCScaleGain1EB_EGM)
   # flashggDiPhotonSystematics.SystMethods.append(sysmodule.MaterialCentralBarrel)
   # flashggDiPhotonSystematics.SystMethods.append(sysmodule.MaterialOuterBarrel)
   # # flashggDiPhotonSystematics.SystMethods.append(sysmodule.MaterialForward)
   # flashggDiPhotonSystematics.SystMethods.append(sysmodule.ShowerShapeHighR9EB)
   # flashggDiPhotonSystematics.SystMethods.append(sysmodule.ShowerShapeHighR9EE)
   # flashggDiPhotonSystematics.SystMethods.append(sysmodule.ShowerShapeLowR9EB)
   # # flashggDiPhotonSystematics.SystMethods.append(sysmodule.ShowerShapeLowR9EE)
   # flashggDiPhotonSystematics.SystMethods.append(sysmodule.FNUFEB)
   # flashggDiPhotonSystematics.SystMethods.append(sysmodule.FNUFEE)
   flashggDiPhotonSystematics.SystMethods.append(sysmodule.MCSmearHighR9EE_EGM)
   flashggDiPhotonSystematics.SystMethods.append(sysmodule.MCSmearLowR9EE_EGM)
   flashggDiPhotonSystematics.SystMethods.append(sysmodule.MCSmearHighR9EB_EGM)
   flashggDiPhotonSystematics.SystMethods.append(sysmodule.MCSmearLowR9EB_EGM)
   # flashggDiPhotonSystematics.SystMethods.append(sysmodule.MvaShift)
   # flashggDiPhotonSystematics.SystMethods.append(sysmodule.PreselSF)
   # flashggDiPhotonSystematics.SystMethods.append(sysmodule.electronVetoSF)
   # flashggDiPhotonSystematics.SystMethods.append(sysmodule.TriggerWeight)
   # flashggDiPhotonSystematics.SystMethods.append(sysmodule.LooseMvaSF)
   # flashggDiPhotonSystematics.SystMethods.append(sysmodule.SigmaEOverEShift)
   # flashggDiPhotonSystematics.SystMethods.append(sysmodule.SigmaEOverESmearing)
   # flashggDiPhotonSystematics.SystMethods.append(sysmodule.FracRVWeight)
   # flashggDiPhotonSystematics.SystMethods.append(sysmodule.FracRVNvtxWeight)
