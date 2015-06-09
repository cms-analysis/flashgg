import FWCore.ParameterSet.Config as cms

flashggSmearElectronEff = cms.EDProducer('FlashggElectronEffSystematicProducer',
					InputTag = cms.untracked.InputTag('flashggElectrons'),
					SystMethods = cms.VPSet(cms.PSet( MethodName = cms.string("FlashggElectronEffSmear"),
									  Label = cms.string("exampleSystAlphaElectron"),
									  Sigma = cms.double(3.0),
									  NSigmas = cms.vint32(-1,1)),
						                cms.PSet( MethodName = cms.string("FlashggElectronEffSmear"),
									  Label = cms.string("exampleSystBravo"),
									  Sigma = cms.double(1.0),
									  NSigmas = cms.vint32(-2,-1,4,7))
								)
							)


flashggSmearMuonEff = cms.EDProducer('FlashggMuonEffSystematicProducer',
					InputTag = cms.untracked.InputTag('slimmedMuons'),
					SystMethods = cms.VPSet(cms.PSet( MethodName = cms.string("FlashggMuonEffSmear"),
									  Label = cms.string("exampleSystAlphaMuon"),
									  Sigma = cms.double(3.0),
									  NSigmas = cms.vint32(-1,1)),
						                cms.PSet( MethodName = cms.string("FlashggMuonEffSmear"),
									  Label = cms.string("exampleSystBravo"),
									  Sigma = cms.double(1.0),
									  NSigmas = cms.vint32(-2,-1,4,7))
								)
							)	
