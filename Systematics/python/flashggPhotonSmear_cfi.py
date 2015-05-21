import FWCore.ParameterSet.Config as cms

scaleBins = cms.VPSet(
          cms.PSet( Range = cms.string("abs(eta)>0.000&&abs(eta)<1.000&&r9>0.94&&r9<999.00"), Shift = cms.double(0.00000), Uncertainty = cms.double(0.00050) ),
          cms.PSet( Range = cms.string("abs(eta)>0.000&&abs(eta)<1.000&&r9>-999.00&&r9<0.94"), Shift = cms.double(0.00000), Uncertainty = cms.double(0.00050) ),
          cms.PSet( Range = cms.string("abs(eta)>1.000&&abs(eta)<1.500&&r9>0.94&&r9<999.00"), Shift = cms.double(0.00000), Uncertainty = cms.double(0.00060) ),
          cms.PSet( Range = cms.string("abs(eta)>1.000&&abs(eta)<1.500&&r9>-999.00&&r9<0.94"), Shift = cms.double(0.00000), Uncertainty = cms.double(0.00120) ),
          cms.PSet( Range = cms.string("abs(eta)>1.500&&abs(eta)<2.000&&r9>-999.00&&r9<0.94"), Shift = cms.double(0.00000), Uncertainty = cms.double(0.00200) ),
          cms.PSet( Range = cms.string("abs(eta)>1.500&&abs(eta)<2.000&&r9>0.94&&r9<999.00"), Shift = cms.double(0.00000), Uncertainty = cms.double(0.00300) ),
          cms.PSet( Range = cms.string("abs(eta)>2.000&&abs(eta)<3.000&&r9>-999.00&&r9<0.94"), Shift = cms.double(0.00000), Uncertainty = cms.double(0.00200) ),
          cms.PSet( Range = cms.string("abs(eta)>2.000&&abs(eta)<3.000&&r9>0.94&&r9<999.00"), Shift = cms.double(0.00000), Uncertainty = cms.double(0.00300) )
                      )

flashggSmearDiPhoton = cms.EDProducer('FlashggDiPhotonSystematicProducer',
		src = cms.InputTag('flashggDiPhotons'),
		SystMethods = cms.VPSet(
                                        cms.PSet( PhotonMethodName = cms.string("FlashggPhotonScaleString"),
                                                  MethodName = cms.string("FlashggDiPhotonFromPhoton"),
                                                  Label = cms.string("MCScaleHighR9EB"),
                                                  NSigmas = cms.vint32(-1,1),
                                                  OverallRange = cms.string("r9>0.94&&abs(eta)<1.5"),
                                                  Bins = scaleBins,
                                                  Debug = cms.untracked.bool(True)
                                                  ),
                                        cms.PSet( PhotonMethodName = cms.string("FlashggPhotonScaleString"),
                                                  MethodName = cms.string("FlashggDiPhotonFromPhoton"),
                                                  Label = cms.string("MCScaleHighR9EE"),
                                                  NSigmas = cms.vint32(-1,1),
                                                  OverallRange = cms.string("r9>0.94&&abs(eta)>1.5"),
                                                  Bins = scaleBins,
                                                  Debug = cms.untracked.bool(True)
                                                  ),
                                        cms.PSet( PhotonMethodName = cms.string("FlashggPhotonScaleString"),
                                                  MethodName = cms.string("FlashggDiPhotonFromPhoton"),
                                                  Label = cms.string("MCScaleLowR9EB"),
                                                  NSigmas = cms.vint32(-1,1),
                                                  OverallRange = cms.string("r9<=0.94&&abs(eta)<1.5"),
                                                  Bins = scaleBins,
                                                  Debug = cms.untracked.bool(True)
                                                  ),
                                        cms.PSet( PhotonMethodName = cms.string("FlashggPhotonScaleString"),
                                                  MethodName = cms.string("FlashggDiPhotonFromPhoton"),
                                                  Label = cms.string("MCScaleLowR9EE"),
                                                  NSigmas = cms.vint32(-1,1),
                                                  OverallRange = cms.string("r9<0.94&&abs(eta)>1.5"),
                                                  Bins = scaleBins,
                                                  Debug = cms.untracked.bool(True)
                                                  )
                                        )
                                    )
