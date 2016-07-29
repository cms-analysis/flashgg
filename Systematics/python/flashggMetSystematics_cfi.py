import FWCore.ParameterSet.Config as cms



flashggMetSystematics = cms.EDProducer('FlashggMetSystematicProducer',
                                       src = cms.InputTag("slimmedMETs::FLASHggMicroAOD"),
                                       SystMethods2D = cms.VPSet(),
                                       SystMethods = cms.VPSet(cms.PSet( MethodName = cms.string("FlashggMetUncertainty"),
                                                                         Label = cms.string("metUncertainty"),
                                                                         NSigmas = cms.vint32(-1,1),
                                                                         OverallRange = cms.string("abs(eta)<2.5"),
                                                                         #BinList = binInfo,
                                                                         Debug = cms.untracked.bool(False),
                                                                         ApplyCentralValue = cms.bool(False)
                                                                         )
                                                               )
)
