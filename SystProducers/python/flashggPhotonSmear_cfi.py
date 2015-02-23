import FWCore.ParameterSet.Config as cms

flashggSmearPhoton = cms.EDProducer('FlashggPhotonSystematicProducer',
		PhotonTag = cms.untracked.InputTag('flashggPhotons'),
		SystMethodNames = cms.VPSet(cms.PSet( MethodName = cms.string("FlashggSimplePhotonSmear"),
                                                      Sigma = cms.double(3.0),
						      NSigmas = cms.untracked.vint32(-2,-1,3,7))
			)
		)
