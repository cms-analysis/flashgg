import FWCore.ParameterSet.Config as cms

scaleBins = cms.VPSet(cms.PSet( Range = cms.string("pt>10.&&pt<15."), Scale = cms.double(0.5) ),
                      cms.PSet( Range = cms.string("pt>15.&&pt<20."), Scale = cms.double(1.0) ),
                      cms.PSet( Range = cms.string("pt>20."), Scale = cms.double(2.0) )
                      )

flashggSmearPhoton = cms.EDProducer('FlashggPhotonSystematicProducer',
		PhotonTag = cms.untracked.InputTag('flashggPhotons'),
		SystMethods = cms.VPSet(cms.PSet( MethodName = cms.string("FlashggSimplePhotonSmear"),
                                                  Label = cms.string("exampleSystAlpha"),
                                                  Sigma = cms.double(3.0),
                                                  NSigmas = cms.vint32(-1,1)),
                                        cms.PSet( MethodName = cms.string("FlashggSimplePhotonSmear"),
                                                  Label = cms.string("exampleSystBravo"),
                                                  Sigma = cms.double(1.0),
                                                  NSigmas = cms.vint32(-2,-1,4,7)),
                                        cms.PSet( MethodName = cms.string("FlashggPhotonScaleString"),
                                                  Label = cms.string("exampleSystCharlieBarrel"),
                                                  NSigmas = cms.vint32(-1,1),
                                                  OverallRange = cms.string("pt > 10.&&abs(eta)<1.479"),
                                                  Bins = scaleBins,
                                                  Debug = cms.untracked.bool(True)
                                                  ),
                                        cms.PSet( MethodName = cms.string("FlashggPhotonScaleString"),
                                                  Label = cms.string("exampleSystCharlieEndcap"),
                                                  NSigmas = cms.vint32(-1,1),
                                                  OverallRange = cms.string("pt > 10.&&abs(eta)>1.653"),
                                                  Bins = scaleBins,
                                                  Debug = cms.untracked.bool(True)
                                                  )
                                        )
                                    )
