import FWCore.ParameterSet.Config as cms

flashggSmearPhoton = cms.EDProducer('FlashggPhotonSystematicProducer',
		PhotonTag = cms.untracked.InputTag('flashggPhotons'),
		SystMethods = cms.VPSet(cms.PSet( MethodName = cms.string("FlashggSimplePhotonSmear"),
                                                  Label = cms.string("exampleSystAlpha"),
                                                  Sigma = cms.double(3.0),
                                                  NSigmas = cms.vint32(-1,1)),
                                        cms.PSet( MethodName = cms.string("FlashggSimplePhotonSmear"),
                                                  Label = cms.string("exampleSystBravo"),
                                                  Sigma = cms.double(1.0),
                                                  NSigmas = cms.vint32(-2,-1,4,7))
                                        )
                                    )
