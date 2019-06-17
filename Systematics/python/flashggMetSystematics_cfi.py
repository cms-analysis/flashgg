import FWCore.ParameterSet.Config as cms


metJecSmear = cms.PSet( MethodName = cms.string("FlashggMetJecSmear"),
                     Label = cms.string("metJecUncertainty"),
                     NSigmas = cms.vint32(-1,1),
                     OverallRange = cms.string("abs(eta)<2.5"),
                     #BinList = binInfo,
                     Debug = cms.untracked.bool(False),
                     ApplyCentralValue = cms.bool(False)
                     )

metJerSmear = cms.PSet( MethodName = cms.string("FlashggMetJerSmear"),
                     Label = cms.string("metJerUncertainty"),
                     NSigmas = cms.vint32(-1,1),
                     OverallRange = cms.string("abs(eta)<2.5"),
                     #BinList = binInfo,
                     Debug = cms.untracked.bool(False),
                     ApplyCentralValue = cms.bool(False)
                     )
metPhoSmear = cms.PSet( MethodName = cms.string("FlashggMetPhoSmear"),
                     Label = cms.string("metPhoUncertainty"),
                     NSigmas = cms.vint32(-1,1),
                     OverallRange = cms.string("abs(eta)<2.5"),
                     #BinList = binInfo,
                     Debug = cms.untracked.bool(False),
                     ApplyCentralValue = cms.bool(False)
                     )
metUncSmear = cms.PSet( MethodName = cms.string("FlashggMetUncSmear"),
                     Label = cms.string("metUncUncertainty"),
                     NSigmas = cms.vint32(-1,1),
                     OverallRange = cms.string("abs(eta)<2.5"),
                     #BinList = binInfo,
                     Debug = cms.untracked.bool(False),
                     ApplyCentralValue = cms.bool(False)
                     )

flashggMetSystematics = cms.EDProducer('FlashggMetSmearSystematicProducer',
                                          src = cms.InputTag("flashggMets"),
                                          SystMethods2D = cms.VPSet(),
                                          SystMethods = cms.VPSet(metJecSmear,
                                                                  metJerSmear,
                                                                  metPhoSmear,
                                                                  metUncSmear)
                                          )


