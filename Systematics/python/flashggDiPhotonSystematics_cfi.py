import FWCore.ParameterSet.Config as cms

scaleBins = cms.PSet(
    variables = cms.vstring("abs(superCluster.eta)","r9"),
    bins = cms.VPSet(
                     cms.PSet( lowBounds = cms.vdouble(0.000,0.940), upBounds = cms.vdouble(1.000,999.000),
                              values = cms.vdouble( 0.00000 ), uncertainties = cms.vdouble( 0.00050 ) ),
                     cms.PSet( lowBounds = cms.vdouble(0.000,-999.000), upBounds = cms.vdouble(1.000,0.940),
                              values = cms.vdouble( 0.00000 ), uncertainties = cms.vdouble( 0.00050 ) ),
                     cms.PSet( lowBounds = cms.vdouble(1.000,0.940), upBounds = cms.vdouble(1.500,999.000),
                              values = cms.vdouble( 0.00000 ), uncertainties = cms.vdouble( 0.00060 ) ),
                     cms.PSet( lowBounds = cms.vdouble(1.000,-999.000), upBounds = cms.vdouble(1.500,0.940),
                              values = cms.vdouble( 0.00000 ), uncertainties = cms.vdouble( 0.00120 ) ),
                     cms.PSet( lowBounds = cms.vdouble(1.500,-999.000), upBounds = cms.vdouble(2.000,0.940),
                              values = cms.vdouble( 0.00000 ), uncertainties = cms.vdouble( 0.00200 ) ),
                     cms.PSet( lowBounds = cms.vdouble(1.500,0.940), upBounds = cms.vdouble(2.000,999.000),
                              values = cms.vdouble( 0.00000 ), uncertainties = cms.vdouble( 0.00300 ) ),
                     cms.PSet( lowBounds = cms.vdouble(2.000,-999.000), upBounds = cms.vdouble(3.000,0.940),
                              values = cms.vdouble( 0.00000 ), uncertainties = cms.vdouble( 0.00200 ) ),
                     cms.PSet( lowBounds = cms.vdouble(2.000,0.940), upBounds = cms.vdouble(3.000,999.000),
                              values = cms.vdouble( 0.00000 ), uncertainties = cms.vdouble( 0.00300 ) ),
                    ))


smearBins = cms.PSet(
    variables = cms.vstring("abs(superCluster.eta)","r9"),
    bins = cms.VPSet(
                     cms.PSet( lowBounds = cms.vdouble(0.000,-999.000), upBounds = cms.vdouble(1.000,0.940),
                              values = cms.vdouble( 0.00770, 0.00000, 6.73000 ), uncertainties = cms.vdouble( 0.00063, 0.16000 ) ),
                     cms.PSet( lowBounds = cms.vdouble(0.000,0.940), upBounds = cms.vdouble(1.000,999.000),
                              values = cms.vdouble( 0.00740, 0.00000, 6.60000 ), uncertainties = cms.vdouble( 0.00065, 0.16000 ) ),
                     cms.PSet( lowBounds = cms.vdouble(1.000,-999.000), upBounds = cms.vdouble(1.500,0.940),
                              values = cms.vdouble( 0.01260, 0.00000, 6.73000 ), uncertainties = cms.vdouble( 0.00103, 0.07000 ) ),
                     cms.PSet( lowBounds = cms.vdouble(1.000,0.940), upBounds = cms.vdouble(1.500,999.000),
                              values = cms.vdouble( 0.01120, 0.00000, 6.52000 ), uncertainties = cms.vdouble( 0.00132, 0.22000 ) ),
                     cms.PSet( lowBounds = cms.vdouble(1.500,-999.000), upBounds = cms.vdouble(2.000,0.940),
                              values = cms.vdouble( 0.01980 ), uncertainties = cms.vdouble( 0.00303 ) ),
                     cms.PSet( lowBounds = cms.vdouble(1.500,0.940), upBounds = cms.vdouble(2.000,999.000),
                              values = cms.vdouble( 0.01630 ), uncertainties = cms.vdouble( 0.00122 ) ),
                     cms.PSet( lowBounds = cms.vdouble(2.000,-999.000), upBounds = cms.vdouble(3.000,0.940),
                              values = cms.vdouble( 0.01920 ), uncertainties = cms.vdouble( 0.00092 ) ),
                     cms.PSet( lowBounds = cms.vdouble(2.000,0.940), upBounds = cms.vdouble(3.000,999.000),
                              values = cms.vdouble( 0.01860 ), uncertainties = cms.vdouble( 0.00078 ) ),
                    ))

mvaShiftBins = cms.PSet(
    variables = cms.vstring("abs(superCluster.eta)"),
    bins = cms.VPSet(
                     cms.PSet( lowBounds = cms.vdouble(0.000), upBounds = cms.vdouble(999.),
                               values = cms.vdouble( 0.0 ), uncertainties = cms.vdouble( 0.03 ))
                     )
    )

# slide 7 of https://indico.cern.ch/event/367883/contribution/2/attachments/1194817/1736347/egamma_26_11_2015.pdf
preselBins = cms.PSet(
    variables = cms.vstring("abs(superCluster.eta)","r9"),
    bins = cms.VPSet(
        cms.PSet( lowBounds = cms.vdouble( 0.0, 0.0 ), upBounds = cms.vdouble( 1.5, 0.9 ),
                  values = cms.vdouble( 0.9968 ), uncertainties = cms.vdouble( 0.0227 ) ),
        cms.PSet( lowBounds = cms.vdouble( 0.0, 0.9 ), upBounds = cms.vdouble( 1.5, 1.0 ),
                  values = cms.vdouble(0.9978 ), uncertainties= cms.vdouble( 0.0029 ) ),
        cms.PSet( lowBounds = cms.vdouble( 1.5, 0.0 ), upBounds = cms.vdouble( 2.5, 0.9),
                  values = cms.vdouble( 1.0115 ), uncertainties= cms.vdouble( 0.0297 ) ),
        cms.PSet( lowBounds = cms.vdouble( 1.5, 0.9 ), upBounds = cms.vdouble( 2.5, 1.0 ),
                  values = cms.vdouble(0.9963 ), uncertainties= cms.vdouble( 0.0044 ) )
        )
    )

# Slide 11, https://indico.cern.ch/event/367883/contribution/2/attachments/1194817/1736347/egamma_26_11_2015.pdf
looseMvaBins = cms.PSet(
    variables = cms.vstring("abs(superCluster.eta)","r9"),
    bins = cms.VPSet(
        cms.PSet( lowBounds = cms.vdouble( 0.0, 0.0 ), upBounds = cms.vdouble( 1.5, 0.9 ),
                  values = cms.vdouble( 1.0001 ), uncertainties = cms.vdouble( 0.0022 ) ),
        cms.PSet( lowBounds = cms.vdouble( 0.0, 0.9 ), upBounds = cms.vdouble( 1.5, 1.0 ),
                  values = cms.vdouble( 1.000 ), uncertainties= cms.vdouble( 0.0001 ) ),
        cms.PSet( lowBounds = cms.vdouble( 1.5, 0.0 ), upBounds = cms.vdouble( 2.5, 0.9),
                  values = cms.vdouble( 0.9851 ), uncertainties= cms.vdouble( 0.0016 ) ),
        cms.PSet( lowBounds = cms.vdouble( 1.5, 0.9 ), upBounds = cms.vdouble( 2.5, 1.0 ),
                  values = cms.vdouble( 1.0001 ), uncertainties= cms.vdouble( 0.0009 ) )
        )
    )




sigmaEOverEShiftBins = cms.PSet(
    variables = cms.vstring("abs(superCluster.eta)"),
    bins = cms.VPSet(
                     cms.PSet( lowBounds = cms.vdouble(0.000), upBounds = cms.vdouble(999.),
                               values = cms.vdouble( 0.0 ), uncertainties = cms.vdouble( 0.05 ))
                     )
    )


flashggDiPhotonSystematics = cms.EDProducer('FlashggDiPhotonSystematicProducer',
		src = cms.InputTag("flashggFinalEGamma","finalDiPhotons"),
                SystMethods2D = cms.VPSet(
                                          cms.PSet( PhotonMethodName = cms.string("FlashggPhotonSmearStochastic"),
                                                    MethodName = cms.string("FlashggDiPhotonFromPhoton2D"),
                                                    Label = cms.string("MCSmearHighR9EB"),
                                                    OverallRange = cms.string("r9>0.94&&abs(superCluster.eta)<1.5"),
                                                    NSigmas = cms.PSet( firstVar = cms.vint32(1,-1,0,0),
                                                                        secondVar = cms.vint32(0,0,1,-1)),
                                                    BinList = smearBins,
                                                    Debug = cms.untracked.bool(False),
                                                    ExaggerateShiftUp = cms.untracked.bool(False),
                                                    ),
                                          cms.PSet( PhotonMethodName = cms.string("FlashggPhotonSmearStochastic"),
                                                    MethodName = cms.string("FlashggDiPhotonFromPhoton2D"),
                                                    Label = cms.string("MCSmearLowR9EB"),
                                                    OverallRange = cms.string("r9<0.94&&abs(superCluster.eta)<1.5"),
                                                    NSigmas = cms.PSet( firstVar = cms.vint32(1,-1,0,0),
                                                                        secondVar = cms.vint32(0,0,1,-1)),
                                                    BinList = smearBins,
                                                    Debug = cms.untracked.bool(False),
                                                    ExaggerateShiftUp = cms.untracked.bool(False),
                                                    )
                                          ),
		SystMethods = cms.VPSet(
                                        cms.PSet( PhotonMethodName = cms.string("FlashggPhotonScale"),
                                                  MethodName = cms.string("FlashggDiPhotonFromPhoton"),
                                                  Label = cms.string("MCScaleHighR9EB"),
                                                  NSigmas = cms.vint32(-1,1),
                                                  OverallRange = cms.string("r9>0.94&&abs(superCluster.eta)<1.5"),
                                                  BinList = scaleBins,
                                                  Debug = cms.untracked.bool(False)
                                                  ),
                                        cms.PSet( PhotonMethodName = cms.string("FlashggPhotonScale"),
                                                  MethodName = cms.string("FlashggDiPhotonFromPhoton"),
                                                  Label = cms.string("MCScaleLowR9EB"),
                                                  NSigmas = cms.vint32(-1,1),
                                                  OverallRange = cms.string("r9<0.94&&abs(superCluster.eta)<1.5"),
                                                  BinList = scaleBins,
                                                  Debug = cms.untracked.bool(False)
                                                  ),
                                        cms.PSet( PhotonMethodName = cms.string("FlashggPhotonScale"),
                                                  MethodName = cms.string("FlashggDiPhotonFromPhoton"),
                                                  Label = cms.string("MCScaleHighR9EE"),
                                                  NSigmas = cms.vint32(-1,1),
                                                  OverallRange = cms.string("r9>0.94&&abs(superCluster.eta)>=1.5"),
                                                  BinList = scaleBins,
                                                  Debug = cms.untracked.bool(False)
                                                  ),
                                        cms.PSet( PhotonMethodName = cms.string("FlashggPhotonScale"),
                                                  MethodName = cms.string("FlashggDiPhotonFromPhoton"),
                                                  Label = cms.string("MCScaleLowR9EE"),
                                                  NSigmas = cms.vint32(-1,1),
                                                  OverallRange = cms.string("r9<0.94&&abs(superCluster.eta)>=1.5"),
                                                  BinList = scaleBins,
                                                  Debug = cms.untracked.bool(False)
                                                  ),
                                        cms.PSet( PhotonMethodName = cms.string("FlashggPhotonSmearConstant"),
                                                  MethodName = cms.string("FlashggDiPhotonFromPhoton"),
                                                  Label = cms.string("MCSmearHighR9EE"),
                                                  NSigmas = cms.vint32(-1,1),
                                                  OverallRange = cms.string("r9>0.94&&abs(superCluster.eta)>=1.5"),
                                                  BinList = smearBins,
                                                  Debug = cms.untracked.bool(False),
                                                  ExaggerateShiftUp = cms.untracked.bool(False),
                                                  ),
                                        cms.PSet( PhotonMethodName = cms.string("FlashggPhotonSmearConstant"),
                                                  MethodName = cms.string("FlashggDiPhotonFromPhoton"),
                                                  Label = cms.string("MCSmearLowR9EE"),
                                                  NSigmas = cms.vint32(-1,1),
                                                  OverallRange = cms.string("r9<0.94&&abs(superCluster.eta)>=1.5"),
                                                  BinList = smearBins,
                                                  Debug = cms.untracked.bool(False),
                                                  ExaggerateShiftUp = cms.untracked.bool(False),
                                                  ),
                                        cms.PSet( PhotonMethodName = cms.string("FlashggPhotonMvaShift"),
                                                  MethodName = cms.string("FlashggDiPhotonFromPhoton"),
                                                  Label = cms.string("MvaShift"),
                                                  NSigmas = cms.vint32(-1,1),
                                                  OverallRange = cms.string("1"),
                                                  BinList = mvaShiftBins,
                                                  Debug = cms.untracked.bool(False)
                                                  ),
                                        cms.PSet( PhotonMethodName = cms.string("FlashggPhotonWeight"),
                                                  MethodName = cms.string("FlashggDiPhotonFromPhoton"),
                                                  Label = cms.string("PreselSFLowR9EE"),
                                                  NSigmas = cms.vint32(-1,1),
                                                  OverallRange = cms.string("r9<0.9&&abs(superCluster.eta)>=1.5"),
                                                  BinList = preselBins,
                                                  Debug = cms.untracked.bool(False)
                                                  ),
                                        cms.PSet( PhotonMethodName = cms.string("FlashggPhotonWeight"),
                                                  MethodName = cms.string("FlashggDiPhotonFromPhoton"),
                                                  Label = cms.string("PreselSFHighR9EE"),
                                                  NSigmas = cms.vint32(-1,1),
                                                  OverallRange = cms.string("r9>=0.9&&abs(superCluster.eta)>=1.5"),
                                                  BinList = preselBins,
                                                  Debug = cms.untracked.bool(False)
                                                  ),
                                        cms.PSet( PhotonMethodName = cms.string("FlashggPhotonWeight"),
                                                  MethodName = cms.string("FlashggDiPhotonFromPhoton"),
                                                  Label = cms.string("PreselSFLowR9EB"),
                                                  NSigmas = cms.vint32(-1,1),
                                                  OverallRange = cms.string("r9<0.9&&abs(superCluster.eta)<1.5"),
                                                  BinList = preselBins,
                                                  Debug = cms.untracked.bool(False)
                                                  ),
                                        cms.PSet( PhotonMethodName = cms.string("FlashggPhotonWeight"),
                                                  MethodName = cms.string("FlashggDiPhotonFromPhoton"),
                                                  Label = cms.string("PreselSFHighR9EB"),
                                                  NSigmas = cms.vint32(-1,1),
                                                  OverallRange = cms.string("r9>=0.9&&abs(superCluster.eta)<1.5"),
                                                  BinList = preselBins,
                                                  Debug = cms.untracked.bool(False)
                                                  ),
                                        cms.PSet( PhotonMethodName = cms.string("FlashggPhotonWeight"),
                                                  MethodName = cms.string("FlashggDiPhotonFromPhoton"),
                                                  Label = cms.string("LooseMvaSFLowR9EE"),
                                                  NSigmas = cms.vint32(-1,1),
                                                  OverallRange = cms.string("r9<0.9&&abs(superCluster.eta)>=1.5"),
                                                  BinList = looseMvaBins,
                                                  Debug = cms.untracked.bool(False)
                                                  ),
                                        cms.PSet( PhotonMethodName = cms.string("FlashggPhotonWeight"),
                                                  MethodName = cms.string("FlashggDiPhotonFromPhoton"),
                                                  Label = cms.string("LooseMvaSFHighR9EE"),
                                                  NSigmas = cms.vint32(-1,1),
                                                  OverallRange = cms.string("r9>=0.9&&abs(superCluster.eta)>=1.5"),
                                                  BinList = looseMvaBins,
                                                  Debug = cms.untracked.bool(False)
                                                  ),
                                        cms.PSet( PhotonMethodName = cms.string("FlashggPhotonWeight"),
                                                  MethodName = cms.string("FlashggDiPhotonFromPhoton"),
                                                  Label = cms.string("LooseMvaSFLowR9EB"),
                                                  NSigmas = cms.vint32(-1,1),
                                                  OverallRange = cms.string("r9<0.9&&abs(superCluster.eta)<1.5"),
                                                  BinList = looseMvaBins,
                                                  Debug = cms.untracked.bool(False)
                                                  ),
                                        cms.PSet( PhotonMethodName = cms.string("FlashggPhotonWeight"),
                                                  MethodName = cms.string("FlashggDiPhotonFromPhoton"),
                                                  Label = cms.string("LooseMvaSFHighR9EB"),
                                                  NSigmas = cms.vint32(-1,1),
                                                  OverallRange = cms.string("r9>=0.9&&abs(superCluster.eta)<1.5"),
                                                  BinList = looseMvaBins,
                                                  Debug = cms.untracked.bool(False)
                                                  ),
                                        cms.PSet( PhotonMethodName = cms.string("FlashggPhotonSigEOverEShift"),
                                                  MethodName = cms.string("FlashggDiPhotonFromPhoton"),
                                                  Label = cms.string("SigmaEOverEShift"),
                                                  NSigmas = cms.vint32(-1,1),
                                                  OverallRange = cms.string("1"),
                                                  BinList = sigmaEOverEShiftBins,
                                                  Debug = cms.untracked.bool(True)
                                                  )

                                        )
                                     )

