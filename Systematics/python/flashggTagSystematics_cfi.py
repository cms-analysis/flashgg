import FWCore.ParameterSet.Config as cms

fakeBins = cms.PSet(
    variables = cms.vstring("abs(eta)"),
    bins = cms.VPSet(
                     cms.PSet( lowBounds = cms.vdouble(0.0), upBounds = cms.vdouble(0.8),
                               values = cms.vdouble( 1. ), uncertainties = cms.vdouble( 0. ) ),
                    ))

flashggTagSystematics = cms.EDProducer('FlashggTagSystematicProducer',
                                       src = cms.InputTag("flashggTagMerger"),
                                       SystMethods2D = cms.VPSet(),
                                       SystMethods = cms.VPSet(cms.PSet( MethodName = cms.string("TagWeightFromFracRV"),
                                                                          Label = cms.string("FracRVWeight"),
                                                                          NSigmas = cms.vint32(-1,1),
                                                                          OverallRange = cms.string("1"),
                                                                          BinList = fakeBins,
                                                                          Debug = cms.untracked.bool(True)
                                                                         )
                                                               )
                                       )

