import FWCore.ParameterSet.Config as cms






metSmear = cms.PSet( MethodName = cms.string("FlashggMetSmear"),
                     Label = cms.string("metUncertainty"),
                     NSigmas = cms.vint32(-1,1),
                     OverallRange = cms.string("abs(eta)<2.5"),
                     #BinList = binInfo,
                     Debug = cms.untracked.bool(False),
                     ApplyCentralValue = cms.bool(False)
                     )




flashggMetSystematics = cms.EDProducer('FlashggMetSmearSystematicProducer',
                                       #src = cms.InputTag("slimmedMETs::FLASHggMicroAOD"),
                                       src = cms.InputTag("flashggMets::FLASHggMicroAOD"),
                                       SystMethods2D = cms.VPSet(),
                                       SystMethods = cms.VPSet(metSmear)
                                       )

