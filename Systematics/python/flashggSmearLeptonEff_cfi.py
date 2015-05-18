import FWCore.ParameterSet.Config as cms

flashggSmearLeptonEff = cms.EDProducer('FlashggLeptonEffSystematicProducer',
					LeptonTag = cms.untracked.InputTag('flashggElectron'),
					SystMethods = cms.VPSet(cms.PSet( MethodName = cms.string("FlashggLeptonEffSmear"),
									  Label = cms.string("exampleSystAlpha"),
									  Sigma = cms.double(3.0),
									  NSigmas = cms.vint32(-1,1)),
						                cms.PSet( MethodName = cms.string("FlashggLeptonEffSmear"),
									  Label = cms.string("exampleSystBravo"),
									  Sigma = cms.double(1.0),
									  NSigmas = cms.vint32(-2,-1,4,7))
								)
							)	
