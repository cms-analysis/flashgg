import FWCore.ParameterSet.Config as cms

binInfo = cms.PSet(
		variables = cms.vstring("pt"),
		bins = cms.VPSet(
			cms.PSet(lowBounds = cms.vdouble(1.0), upBounds = cms.vdouble(25.0), values = cms.vdouble(0.8), uncertainties = cms.vdouble(.05,0.3)),
			cms.PSet(lowBounds = cms.vdouble(25.0), upBounds = cms.vdouble(100.0), values = cms.vdouble(0.9), uncertainties = cms.vdouble(.05,0.3)),
			cms.PSet(lowBounds = cms.vdouble(100.0), upBounds = cms.vdouble(300.0), values = cms.vdouble(0.95), uncertainties = cms.vdouble(.05,0.3))
			)
		)	

flashggMuonSystematics = cms.EDProducer('FlashggMuonEffSystematicProducer',
					src = cms.InputTag("flashggSelectedMuons"),
					SystMethods2D = cms.VPSet(),
					SystMethods = cms.VPSet(cms.PSet( MethodName = cms.string("FlashggMuonEffScale"),
									  Label = cms.string("MuonWeight"),
									  NSigmas = cms.vint32(-1,1),
									  OverallRange = cms.string("abs(eta)<1.5"),
									  BinList = binInfo,
									  Debug = cms.untracked.bool(False)
                                                                          )
								)
                                        )

