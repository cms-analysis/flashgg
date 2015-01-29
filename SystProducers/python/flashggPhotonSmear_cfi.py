import FWCore.ParameterSet.Config as cms

flashggSmearPhoton = cms.EDProducer('FlashggPhotonSystematicProducer',
		PhotonTag = cms.untracked.InputTag('flashggPhotons'),
		SystMethodName=cms.string("FlashggSimplePhotonSmear")
		)
