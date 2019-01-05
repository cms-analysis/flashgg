import FWCore.ParameterSet.Config as cms

flashggDiPhotonSystematics = cms.EDProducer('FlashggDiPhotonSystematicProducer',
		src = cms.InputTag("flashggUpdatedIdMVADiPhotons"),
                SystMethods2D = cms.VPSet(),
                SystMethods = cms.VPSet()
)

def SetupDiPhotonSystematics( process ):
 import flashgg.Systematics.settings as settings
 year = settings.year
 if year == "2016":
   process.load("flashgg.Systematics.flashggDiPhotonSystematics2016_cfi")
   import flashgg.Systematics.flashggDiPhotonSystematics2016_cfi as diphotons2016
   flashggDiPhotonSystematics.SystMethods.append(diphotons2016.MCScaleHighR9EB)
   flashggDiPhotonSystematics.SystMethods.append(diphotons2016.MCScaleLowR9EB)
   flashggDiPhotonSystematics.SystMethods.append(diphotons2016.MCScaleHighR9EE)
   flashggDiPhotonSystematics.SystMethods.append(diphotons2016.MCScaleLowR9EE)
   flashggDiPhotonSystematics.SystMethods.append(diphotons2016.MCScaleGain6EB_EGM)
   flashggDiPhotonSystematics.SystMethods.append(diphotons2016.MCScaleGain1EB_EGM)
   flashggDiPhotonSystematics.SystMethods.append(diphotons2016.MaterialCentralBarrel)
   flashggDiPhotonSystematics.SystMethods.append(diphotons2016.MaterialOuterBarrel)
   flashggDiPhotonSystematics.SystMethods.append(diphotons2016.MaterialForward)
   flashggDiPhotonSystematics.SystMethods.append(diphotons2016.ShowerShapeHighR9EB)
   flashggDiPhotonSystematics.SystMethods.append(diphotons2016.ShowerShapeHighR9EE)
   flashggDiPhotonSystematics.SystMethods.append(diphotons2016.ShowerShapeLowR9EB)
   flashggDiPhotonSystematics.SystMethods.append(diphotons2016.ShowerShapeLowR9EE)
   flashggDiPhotonSystematics.SystMethods.append(diphotons2016.FNUFEB)
   flashggDiPhotonSystematics.SystMethods.append(diphotons2016.FNUFEE)
   flashggDiPhotonSystematics.SystMethods.append(diphotons2016.MCSmearHighR9EE)
   flashggDiPhotonSystematics.SystMethods.append(diphotons2016.MCSmearLowR9EE)
   flashggDiPhotonSystematics.SystMethods.append(diphotons2016.MCSmearHighR9EB)
   flashggDiPhotonSystematics.SystMethods.append(diphotons2016.MCSmearLowR9EB)
   flashggDiPhotonSystematics.SystMethods.append(diphotons2016.MvaShift)
   flashggDiPhotonSystematics.SystMethods.append(diphotons2016.PreselSF)
   flashggDiPhotonSystematics.SystMethods.append(diphotons2016.electronVetoSF)
   flashggDiPhotonSystematics.SystMethods.append(diphotons2016.TriggerWeight)
   flashggDiPhotonSystematics.SystMethods.append(diphotons2016.LooseMvaSF)
   flashggDiPhotonSystematics.SystMethods.append(diphotons2016.SigmaEOverEShift)
   flashggDiPhotonSystematics.SystMethods.append(diphotons2016.SigmaEOverESmearing)
   flashggDiPhotonSystematics.SystMethods.append(diphotons2016.FracRVWeight)
   flashggDiPhotonSystematics.SystMethods.append(diphotons2016.FracRVNvtxWeight)
 elif year == "2017":
   process.load("flashgg.Systematics.flashggDiPhotonSystematics2017_cfi")
   import flashgg.Systematics.flashggDiPhotonSystematics2017_cfi as diphotons2017
   flashggDiPhotonSystematics.SystMethods.append(diphotons2017.MCScaleHighR9EB)
   flashggDiPhotonSystematics.SystMethods.append(diphotons2017.MCScaleLowR9EB)
   flashggDiPhotonSystematics.SystMethods.append(diphotons2017.MCScaleHighR9EE)
   flashggDiPhotonSystematics.SystMethods.append(diphotons2017.MCScaleLowR9EE)
   flashggDiPhotonSystematics.SystMethods.append(diphotons2017.MCScaleGain6EB_EGM)
   flashggDiPhotonSystematics.SystMethods.append(diphotons2017.MCScaleGain1EB_EGM)
   flashggDiPhotonSystematics.SystMethods.append(diphotons2017.MaterialCentralBarrel)
   flashggDiPhotonSystematics.SystMethods.append(diphotons2017.MaterialOuterBarrel)
   flashggDiPhotonSystematics.SystMethods.append(diphotons2017.MaterialForward)
   flashggDiPhotonSystematics.SystMethods.append(diphotons2017.ShowerShapeHighR9EB)
   flashggDiPhotonSystematics.SystMethods.append(diphotons2017.ShowerShapeHighR9EE)
   flashggDiPhotonSystematics.SystMethods.append(diphotons2017.ShowerShapeLowR9EB)
   flashggDiPhotonSystematics.SystMethods.append(diphotons2017.ShowerShapeLowR9EE)
   flashggDiPhotonSystematics.SystMethods.append(diphotons2017.FNUFEB)
   flashggDiPhotonSystematics.SystMethods.append(diphotons2017.FNUFEE)
   flashggDiPhotonSystematics.SystMethods.append(diphotons2017.MCSmearHighR9EE)
   flashggDiPhotonSystematics.SystMethods.append(diphotons2017.MCSmearLowR9EE)
   flashggDiPhotonSystematics.SystMethods.append(diphotons2017.MCSmearHighR9EB)
   flashggDiPhotonSystematics.SystMethods.append(diphotons2017.MCSmearLowR9EB)
   flashggDiPhotonSystematics.SystMethods.append(diphotons2017.MvaShift)
   flashggDiPhotonSystematics.SystMethods.append(diphotons2017.PreselSF)
   flashggDiPhotonSystematics.SystMethods.append(diphotons2017.electronVetoSF)
   flashggDiPhotonSystematics.SystMethods.append(diphotons2017.TriggerWeight)
   flashggDiPhotonSystematics.SystMethods.append(diphotons2017.LooseMvaSF)
   flashggDiPhotonSystematics.SystMethods.append(diphotons2017.SigmaEOverEShift)
   flashggDiPhotonSystematics.SystMethods.append(diphotons2017.SigmaEOverESmearing)
   flashggDiPhotonSystematics.SystMethods.append(diphotons2017.FracRVWeight)
   flashggDiPhotonSystematics.SystMethods.append(diphotons2017.FracRVNvtxWeight)


