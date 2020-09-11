import importlib
import FWCore.ParameterSet.Config as cms

flashggDiPhotonSystematics = cms.EDProducer('FlashggDiPhotonSystematicProducer',
                src = cms.InputTag("flashggDifferentialPhoIdInputsCorrection"),
                SystMethods2D = cms.VPSet(),
                SystMethods = cms.VPSet()
)

def setupDiPhotonSystematics( process, options ):
   print'[setupDiPhotonSystematics] - Choosing diphoton systematics'
   process.load("flashgg.Systematics."+options.metaConditions["flashggDiPhotonSystematics"])
   sysmodule = importlib.import_module("flashgg.Systematics."+options.metaConditions["flashggDiPhotonSystematics"])
   flashggDiPhotonSystematics.SystMethods.append(sysmodule.MCScaleHighR9EB)
   flashggDiPhotonSystematics.SystMethods.append(sysmodule.MCScaleLowR9EB)
   flashggDiPhotonSystematics.SystMethods.append(sysmodule.MCScaleHighR9EE)
   flashggDiPhotonSystematics.SystMethods.append(sysmodule.MCScaleLowR9EE)
   flashggDiPhotonSystematics.SystMethods.append(sysmodule.MCScaleGain6EB_EGM)
   flashggDiPhotonSystematics.SystMethods.append(sysmodule.MCScaleGain1EB_EGM)
   flashggDiPhotonSystematics.SystMethods.append(sysmodule.MaterialCentralBarrel)
   flashggDiPhotonSystematics.SystMethods.append(sysmodule.MaterialOuterBarrel)
   flashggDiPhotonSystematics.SystMethods.append(sysmodule.MaterialForward)
   flashggDiPhotonSystematics.SystMethods.append(sysmodule.ShowerShapeHighR9EB)
   flashggDiPhotonSystematics.SystMethods.append(sysmodule.ShowerShapeHighR9EE)
   flashggDiPhotonSystematics.SystMethods.append(sysmodule.ShowerShapeLowR9EB)
   flashggDiPhotonSystematics.SystMethods.append(sysmodule.ShowerShapeLowR9EE)
   flashggDiPhotonSystematics.SystMethods.append(sysmodule.FNUFEB)
   flashggDiPhotonSystematics.SystMethods.append(sysmodule.FNUFEE)
   flashggDiPhotonSystematics.SystMethods.append(sysmodule.MCSmearHighR9EE)
   flashggDiPhotonSystematics.SystMethods.append(sysmodule.MCSmearLowR9EE)
   flashggDiPhotonSystematics.SystMethods.append(sysmodule.MCSmearHighR9EB)
   flashggDiPhotonSystematics.SystMethods.append(sysmodule.MCSmearLowR9EB)
   flashggDiPhotonSystematics.SystMethods.append(sysmodule.MvaShift)
   flashggDiPhotonSystematics.SystMethods.append(sysmodule.PreselSF)
   flashggDiPhotonSystematics.SystMethods.append(sysmodule.electronVetoSF)
   flashggDiPhotonSystematics.SystMethods.append(sysmodule.TriggerWeight)
   flashggDiPhotonSystematics.SystMethods.append(sysmodule.LooseMvaSF)
   flashggDiPhotonSystematics.SystMethods.append(sysmodule.SigmaEOverEShift)
   flashggDiPhotonSystematics.SystMethods.append(sysmodule.SigmaEOverESmearing)

   if (options.processId.count('ggh') or options.processId.count('GluGluHToGG')) and not options.processId.count('gghh'):
      flashggDiPhotonSystematics.SystMethods.append(sysmodule.FracRVWeight)
   #flashggDiPhotonSystematics.SystMethods.append(sysmodule.FracRVNvtxWeight)

   if options.ignoreNegR9:
      for syst_method in flashggDiPhotonSystematics.SystMethods:
         if hasattr(syst_method, "PhotonMethodName"):
            syst_method.OverallRange = syst_method.OverallRange._value + " && full5x5_r9>0."
