import FWCore.ParameterSet.Config as cms

flashggElectrons = cms.EDProducer('FlashggElectronProducer',
		verbose = cms.untracked.bool(False),
		electronTag = cms.untracked.InputTag('slimmedElectrons'),
		vertexTag = cms.untracked.InputTag('offlineSlimmedPrimaryVertices'),
		convTag   = cms.untracked.InputTag('reducedEgamma','reducedConversions'),
		#beamSpotTag = cms.untracked.InputTag(''),
		reducedEBRecHitCollection = cms.InputTag('reducedEcalRecHitsEB'),
		reducedEERecHitCollection = cms.InputTag('reducedEcalRecHitsEE'),
		method = cms.string("BDT"),
		rhoFixedGridCollection = cms.InputTag('fixedGridRhoAll'),
		mvaWeightFile = cms.vstring(
			"flashgg/MicroAODProducers/data/Electrons_BDTG_NonTrigV0_Cat1.weights.xml",
			"flashgg/MicroAODProducers/data/Electrons_BDTG_NonTrigV0_Cat2.weights.xml",
			"flashgg/MicroAODProducers/data/Electrons_BDTG_NonTrigV0_Cat3.weights.xml",
			"flashgg/MicroAODProducers/data/Electrons_BDTG_NonTrigV0_Cat4.weights.xml",
			"flashgg/MicroAODProducers/data/Electrons_BDTG_NonTrigV0_Cat5.weights.xml",
			"flashgg/MicroAODProducers/data/Electrons_BDTG_NonTrigV0_Cat6.weights.xml",
			),
		Trig = cms.bool(False),
		NoIP = cms.bool(False),
		)
