import FWCore.ParameterSet.Config as cms

binInfo = cms.PSet(
		variables = cms.vstring("pt"),
		bins = cms.VPSet(
			cms.PSet(lowBounds = cms.vdouble(1.0), upBounds = cms.vdouble(25.0), values = cms.vdouble(0.8), uncertainties = cms.vdouble(.05,0.3)),
			cms.PSet(lowBounds = cms.vdouble(25.0), upBounds = cms.vdouble(100.0), values = cms.vdouble(0.9), uncertainties = cms.vdouble(.05,0.3)),
			cms.PSet(lowBounds = cms.vdouble(100.0), upBounds = cms.vdouble(300.0), values = cms.vdouble(.95), uncertainties = cms.vdouble(.05,0.3))
			)
		)	


flashggElectronSystematics = cms.EDProducer('FlashggElectronEffSystematicProducer',
                                            src = cms.InputTag("flashggFinalEGamma","finalElectrons"),
                                            SystMethods2D = cms.VPSet(),
                                            SystMethods = cms.VPSet(cms.PSet( MethodName = cms.string("FlashggElectronEffScale"),
                                                                              Label = cms.string("ElectronWeight"),
                                                                              NSigmas = cms.vint32(-1,1),
                                                                              OverallRange = cms.string("abs(eta)<1.5"),
                                                                              BinList = binInfo,
                                                                              Debug = cms.untracked.bool(False)
                                                                              )	
                                                                    )
                                            )
