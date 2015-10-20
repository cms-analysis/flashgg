import FWCore.ParameterSet.Config as cms

from PhysicsTools.SelectorUtils.tools.vid_id_tools import *

process = cms.Process("FLASHggMicroAOD")

# turn on VID producer, indicate data format  to be
# DataFormat.AOD or DataFormat.MiniAOD, as appropriate 

dataFormat = DataFormat.MiniAOD

switchOnVIDElectronIdProducer(process, dataFormat)

# define which IDs we want to produce
my_id_modules = ['RecoEgamma.ElectronIdentification.Identification.mvaElectronID_Spring15_25ns_nonTrig_V1_cff']

#add them to the VID producer
for idmod in my_id_modules:
    setupAllVIDIdsInModule(process,idmod,setupVIDElectronSelection)

flashggElectrons = cms.EDProducer('FlashggElectronProducer',
		verbose = cms.untracked.bool(False),
		electronTag = cms.InputTag('slimmedElectrons'),
		vertexTag = cms.InputTag('offlineSlimmedPrimaryVertices'),
		convTag   = cms.InputTag('reducedEgamma','reducedConversions'),
		beamSpotTag = cms.InputTag( "offlineBeamSpot" ),
		reducedEBRecHitCollection = cms.InputTag('reducedEcalRecHitsEB'),
		reducedEERecHitCollection = cms.InputTag('reducedEcalRecHitsEE'),
		rhoFixedGridCollection = cms.InputTag('fixedGridRhoAll'),
                mvaValuesMap = cms.InputTag("electronMVAValueMapProducer:ElectronMVAEstimatorRun2Spring15NonTrig25nsV1Values"),
		)
