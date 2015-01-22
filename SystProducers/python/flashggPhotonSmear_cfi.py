import FWCore.ParameterSet.Config as cms

#flashggSmearPhoton = cms.EDProducer('FlashggPhotonSystematicProducer',
#		PhotonTag = cms.untracked.InputTag('flashggPhotons'),
#		SystMethodNames = cms.VPSet(cms.PSet( MethodName = cms.string("FlashggSimplePhotonSmear"),
#							var = cms.double(28.5)),
#			                    cms.PSet( MethodName = cms.string("FlashggShiftPhotonId"),
#							var = cms.double(29.5))	
#			)
#		)
flashggSmearPhoton = cms.EDProducer('FlashggPhotonSystematicProducer',
		PhotonTag = cms.untracked.InputTag('flashggPhotons'),
		SystMethodNames = cms.VPSet(cms.PSet( MethodName = cms.string("FlashggShiftPhotonId"),
							var = cms.double(28.5)),
			                    cms.PSet( MethodName = cms.string("FlashggSimplePhotonSmear"),
							var = cms.double(29.5))	
			)
		)
