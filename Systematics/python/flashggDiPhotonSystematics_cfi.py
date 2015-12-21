import FWCore.ParameterSet.Config as cms

scaleBins = cms.PSet(
    variables = cms.vstring("abs(superCluster.eta)","r9"),
    bins = cms.VPSet(
            # scale for prompt 2015, photons, values OK, errors as in Run1 (since Run2 stat. only)
            # see Fasanella et al., ECAL DPG 17/12/2015, https://cern.ch/go/Tmm9
            # w.r.t. the numbers in slide 28, the scale is computed as provided_scale - 1.
            cms.PSet( lowBounds = cms.vdouble(0.000 , -999.000 ) , upBounds = cms.vdouble(1.000 , 0.940   ) , values = cms.vdouble( 0.00394  ) , uncertainties = cms.vdouble( 0.00050 )  ) ,
            cms.PSet( lowBounds = cms.vdouble(0.000 , 0.940    ) , upBounds = cms.vdouble(1.000 , 999.000 ) , values = cms.vdouble( 0.00032  ) , uncertainties = cms.vdouble( 0.00050 )  ) ,
            cms.PSet( lowBounds = cms.vdouble(1.000 , -999.000 ) , upBounds = cms.vdouble(1.500 , 0.940   ) , values = cms.vdouble( 0.00229  ) , uncertainties = cms.vdouble( 0.00120 )  ) ,
            cms.PSet( lowBounds = cms.vdouble(1.000 , 0.940    ) , upBounds = cms.vdouble(1.500 , 999.000 ) , values = cms.vdouble( -0.00754 ) , uncertainties = cms.vdouble( 0.00060 )  ) ,
            cms.PSet( lowBounds = cms.vdouble(1.500 , -999.000 ) , upBounds = cms.vdouble(2.000 , 0.940   ) , values = cms.vdouble( 0.01243  ) , uncertainties = cms.vdouble( 0.00200 )  ) ,
            cms.PSet( lowBounds = cms.vdouble(1.500 , 0.940    ) , upBounds = cms.vdouble(2.000 , 999.000 ) , values = cms.vdouble( 0.00340  ) , uncertainties = cms.vdouble( 0.00300 )  ) ,
            cms.PSet( lowBounds = cms.vdouble(2.000 , -999.000 ) , upBounds = cms.vdouble(3.000 , 0.940   ) , values = cms.vdouble( 0.02042  ) , uncertainties = cms.vdouble( 0.00200 )  ) ,
            cms.PSet( lowBounds = cms.vdouble(2.000 , 0.940    ) , upBounds = cms.vdouble(3.000 , 999.000 ) , values = cms.vdouble( 0.01445  ) , uncertainties = cms.vdouble( 0.00300 )  ) ,
                    ))


smearBins = cms.PSet(
    variables = cms.vstring("abs(superCluster.eta)","r9"),
    bins = cms.VPSet(
            # smearings for prompt 2015, photons, values OK, errors as statistical of Run2 (since Run1 different parametrization)
            # see Fasanella et al., ECAL DPG 03/12/2015, https://cern.ch/go/Tmm9
            cms.PSet( lowBounds = cms.vdouble(0.000,-999.000 ) , upBounds = cms.vdouble(1.000,0.940   ) , values = cms.vdouble( 0.0144 ) , uncertainties = cms.vdouble( 0.00024652 )  ) ,
            cms.PSet( lowBounds = cms.vdouble(0.000,0.940    ) , upBounds = cms.vdouble(1.000,999.000 ) , values = cms.vdouble( 0.0141 ) , uncertainties = cms.vdouble( 0.00018319 )  ) ,
            cms.PSet( lowBounds = cms.vdouble(1.000,-999.000 ) , upBounds = cms.vdouble(1.500,0.940   ) , values = cms.vdouble( 0.0212 ) , uncertainties = cms.vdouble( 0.00032435 )  ) ,
            cms.PSet( lowBounds = cms.vdouble(1.000,0.940    ) , upBounds = cms.vdouble(1.500,999.000 ) , values = cms.vdouble( 0.0168 ) , uncertainties = cms.vdouble( 0.00098551 )  ) ,
            cms.PSet( lowBounds = cms.vdouble(1.500,-999.000 ) , upBounds = cms.vdouble(2.000,0.940   ) , values = cms.vdouble( 0.0290 ) , uncertainties = cms.vdouble( 0.00045816 )  ) ,
            cms.PSet( lowBounds = cms.vdouble(1.500,0.940    ) , upBounds = cms.vdouble(2.000,999.000 ) , values = cms.vdouble( 0.0276 ) , uncertainties = cms.vdouble( 0.00076386 )  ) ,
            cms.PSet( lowBounds = cms.vdouble(2.000,-999.000 ) , upBounds = cms.vdouble(3.000,0.940   ) , values = cms.vdouble( 0.0324 ) , uncertainties = cms.vdouble( 0.00061517 )  ) ,
            cms.PSet( lowBounds = cms.vdouble(2.000,0.940    ) , upBounds = cms.vdouble(3.000,999.000 ) , values = cms.vdouble( 0.0314 ) , uncertainties = cms.vdouble( 0.00042066 )  ) ,
                    ))

mvaShiftBins = cms.PSet(
    variables = cms.vstring("abs(superCluster.eta)"),
    bins = cms.VPSet(
                     cms.PSet( lowBounds = cms.vdouble(0.000), upBounds = cms.vdouble(999.), values = cms.vdouble( 0.0 ), uncertainties = cms.vdouble( 0.03 ))
                     )
    )

# slide 7 of https://indico.cern.ch/event/367883/contribution/2/attachments/1194817/1736347/egamma_26_11_2015.pdf
preselBins = cms.PSet(
    variables = cms.vstring("abs(superCluster.eta)","r9"),
    bins = cms.VPSet(
        cms.PSet( lowBounds = cms.vdouble( 0.0, 0.0 ) , upBounds = cms.vdouble( 1.5, 0.9  ) , values = cms.vdouble( 0.9968 ) , uncertainties = cms.vdouble( 0.0227 )  ) ,
        cms.PSet( lowBounds = cms.vdouble( 0.0, 0.9 ) , upBounds = cms.vdouble( 1.5, 999. ) , values = cms.vdouble(0.9978  ) , uncertainties = cms.vdouble( 0.0029 )  ) ,
        cms.PSet( lowBounds = cms.vdouble( 1.5, 0.0 ) , upBounds = cms.vdouble( 2.5, 0.9  ) , values = cms.vdouble( 1.0115 ) , uncertainties = cms.vdouble( 0.0297 )  ) ,
        cms.PSet( lowBounds = cms.vdouble( 1.5, 0.9 ) , upBounds = cms.vdouble( 2.5, 999. ) , values = cms.vdouble(0.9963  ) , uncertainties = cms.vdouble( 0.0044 )  ) 
        )
    )

# Slide 9 https://indico.cern.ch/event/464689/session/12/contribution/65/attachments/1204381/1754436/flashgg_14_12_2015.pdf
# N.B. for lose MVA cut SF the R9 boundary is at 0.94
looseMvaBins = cms.PSet(
    variables = cms.vstring("abs(superCluster.eta)","r9"),
    bins = cms.VPSet(
        cms.PSet( lowBounds = cms.vdouble( 0.0, 0.0  ) , upBounds = cms.vdouble( 1.5, 0.94  ) , values = cms.vdouble( 1.0012 ) , uncertainties = cms.vdouble( 0.0016 )  ) ,
        cms.PSet( lowBounds = cms.vdouble( 0.0, 0.94 ) , upBounds = cms.vdouble( 1.5, 999.0 ) , values = cms.vdouble( 0.9999 ) , uncertainties = cms.vdouble( 0.0005 )  ) ,
        cms.PSet( lowBounds = cms.vdouble( 1.5, 0.0  ) , upBounds = cms.vdouble( 2.5, 0.94  ) , values = cms.vdouble( 1.0018 ) , uncertainties = cms.vdouble( 0.0019 )  ) ,
        cms.PSet( lowBounds = cms.vdouble( 1.5, 0.94 ) , upBounds = cms.vdouble( 2.5, 999.0 ) , values = cms.vdouble( 0.9997 ) , uncertainties = cms.vdouble( 0.0008 )  ) 
        )
    )



# RELATIVE shift of sigmaE/E --> 0.05 corresponds to a shift of 5%
sigmaEOverEShiftBins = cms.PSet(
    variables = cms.vstring("abs(superCluster.eta)"),
    bins = cms.VPSet(
                     cms.PSet( lowBounds = cms.vdouble(0.000), upBounds = cms.vdouble(999.), values = cms.vdouble( 0.0 ), uncertainties = cms.vdouble( 0.05 ))
                     )
    )


RVBins = cms.PSet(
                variables = cms.vstring("pt"),
                bins = cms.VPSet(
        cms.PSet(lowBounds = cms.vdouble(0   ) , upBounds = cms.vdouble(10        ) , values = cms.vdouble(1.04969,0.925092 ) , uncertainties = cms.vdouble(0.00369751,0.00369751,0.00557573,0.00557573 )  ) ,
        cms.PSet(lowBounds = cms.vdouble(10  ) , upBounds = cms.vdouble(20        ) , values = cms.vdouble(1.03627,0.915031 ) , uncertainties = cms.vdouble(0.00284216,0.00284216,0.00665951,0.00665951 )  ) ,
        cms.PSet(lowBounds = cms.vdouble(20  ) , upBounds = cms.vdouble(30        ) , values = cms.vdouble(1.00917,0.962609 ) , uncertainties = cms.vdouble(0.00237848,0.00237848,0.00969786,0.00969786 )  ) ,
        cms.PSet(lowBounds = cms.vdouble(30  ) , upBounds = cms.vdouble(40        ) , values = cms.vdouble(0.999164,1.00591 ) , uncertainties = cms.vdouble(0.00217114,0.00217114,0.0153518,0.0153518   )  ) ,
        cms.PSet(lowBounds = cms.vdouble(40  ) , upBounds = cms.vdouble(50        ) , values = cms.vdouble(1.0002,0.997897  ) , uncertainties = cms.vdouble(0.00202796,0.00202796,0.0208318,0.0208318   )  ) ,
        cms.PSet(lowBounds = cms.vdouble(50  ) , upBounds = cms.vdouble(70        ) , values = cms.vdouble(0.998695,1.02067 ) , uncertainties = cms.vdouble(0.00137969,0.00137969,0.021848,0.021848     )  ) ,
        cms.PSet(lowBounds = cms.vdouble(70  ) , upBounds = cms.vdouble(110       ) , values = cms.vdouble(1.00354,0.899309 ) , uncertainties = cms.vdouble(0.00105337,0.00105337,0.0299365,0.0299365   )  ) ,
        cms.PSet(lowBounds = cms.vdouble(110 ) , upBounds = cms.vdouble(170       ) , values = cms.vdouble(1.00369,0.786148 ) , uncertainties = cms.vdouble(0.00105199,0.00105199,0.0609889,0.0609889   )  ) ,
        cms.PSet(lowBounds = cms.vdouble(170 ) , upBounds = cms.vdouble(400       ) , values = cms.vdouble(1,1              ) , uncertainties = cms.vdouble(0.00383577,0.00383577,0.47521,0.47521       )  ) ,
        # maximum energy beyond 7000 because of wonky DiPhotons in data
        cms.PSet(lowBounds = cms.vdouble(400 ) , upBounds = cms.vdouble(999999999 ) , values = cms.vdouble(1,1              ) , uncertainties = cms.vdouble(0.,0.,0.,0.                                 )  ) 
        )
                )

flashggDiPhotonSystematics = cms.EDProducer('FlashggDiPhotonSystematicProducer',
		src = cms.InputTag("flashggPreselectedDiPhotons"),
                SystMethods2D = cms.VPSet(),
                # the number of syst methods matches the number of nuisance parameters
                # assumed for a given systematic uncertainty and is NOT required
                # to match 1-to-1 the number of bins above.
		SystMethods = cms.VPSet(
                                        cms.PSet( PhotonMethodName = cms.string("FlashggPhotonScale"),
                                                  MethodName = cms.string("FlashggDiPhotonFromPhoton"),
                                                  Label = cms.string("MCScaleHighR9EB"),
                                                  NSigmas = cms.vint32(-1,1),
                                                  OverallRange = cms.string("r9>0.94&&abs(superCluster.eta)<1.5"),
                                                  BinList = scaleBins,
                                                  NoCentralShift = cms.bool(True),
                                                  Debug = cms.untracked.bool(False)
                                                  ),
                                        cms.PSet( PhotonMethodName = cms.string("FlashggPhotonScale"),
                                                  MethodName = cms.string("FlashggDiPhotonFromPhoton"),
                                                  Label = cms.string("MCScaleLowR9EB"),
                                                  NSigmas = cms.vint32(-1,1),
                                                  OverallRange = cms.string("r9<0.94&&abs(superCluster.eta)<1.5"),
                                                  BinList = scaleBins,
                                                  NoCentralShift = cms.bool(True),
                                                  Debug = cms.untracked.bool(False)
                                                  ),
                                        cms.PSet( PhotonMethodName = cms.string("FlashggPhotonScale"),
                                                  MethodName = cms.string("FlashggDiPhotonFromPhoton"),
                                                  Label = cms.string("MCScaleHighR9EE"),
                                                  NSigmas = cms.vint32(-1,1),
                                                  OverallRange = cms.string("r9>0.94&&abs(superCluster.eta)>=1.5"),
                                                  BinList = scaleBins,
                                                  NoCentralShift = cms.bool(True),
                                                  Debug = cms.untracked.bool(False)
                                                  ),
                                        cms.PSet( PhotonMethodName = cms.string("FlashggPhotonScale"),
                                                  MethodName = cms.string("FlashggDiPhotonFromPhoton"),
                                                  Label = cms.string("MCScaleLowR9EE"),
                                                  NSigmas = cms.vint32(-1,1),
                                                  OverallRange = cms.string("r9<0.94&&abs(superCluster.eta)>=1.5"),
                                                  BinList = scaleBins,
                                                  NoCentralShift = cms.bool(True),
                                                  Debug = cms.untracked.bool(False)
                                                  ),
                                        cms.PSet( PhotonMethodName = cms.string("FlashggPhotonSmearConstant"),
                                                  MethodName = cms.string("FlashggDiPhotonFromPhoton"),
                                                  Label = cms.string("MCSmearHighR9EE"),
                                                  NSigmas = cms.vint32(-1,1),
                                                  OverallRange = cms.string("r9>0.94&&abs(superCluster.eta)>=1.5"),
                                                  BinList = smearBins,
                                                  # has to match the labels embedded in the photon object as
                                                  # defined e.g. in flashgg/MicroAOD/python/flashggRandomizedPerPhotonDiPhotonProducer_cff.py
                                                  #           or in flashgg/MicroAOD/python/flashggRandomizedPhotonProducer_cff.py (if at MicroAOD prod.)
                                                  RandomLabel = cms.string("smearE"),
                                                  Debug = cms.untracked.bool(False),
                                                  ExaggerateShiftUp = cms.untracked.bool(False),
                                                  ),
                                        cms.PSet( PhotonMethodName = cms.string("FlashggPhotonSmearConstant"),
                                                  MethodName = cms.string("FlashggDiPhotonFromPhoton"),
                                                  Label = cms.string("MCSmearLowR9EE"),
                                                  NSigmas = cms.vint32(-1,1),
                                                  OverallRange = cms.string("r9<0.94&&abs(superCluster.eta)>=1.5"),
                                                  BinList = smearBins,
                                                  # has to match the labels embedded in the photon object as
                                                  # defined e.g. in flashgg/MicroAOD/python/flashggRandomizedPerPhotonDiPhotonProducer_cff.py
                                                  #           or in flashgg/MicroAOD/python/flashggRandomizedPhotonProducer_cff.py (if at MicroAOD prod.)
                                                  RandomLabel = cms.string("smearE"),
                                                  Debug = cms.untracked.bool(False),
                                                  ExaggerateShiftUp = cms.untracked.bool(False),
                                                  ),
                                        cms.PSet( PhotonMethodName = cms.string("FlashggPhotonSmearConstant"),
                                                  MethodName = cms.string("FlashggDiPhotonFromPhoton"),
                                                  Label = cms.string("MCSmearHighR9EB"),
                                                  NSigmas = cms.vint32(-1,1),
                                                  OverallRange = cms.string("r9>0.94&&abs(superCluster.eta)<1.5"),
                                                  BinList = smearBins,
                                                  # has to match the labels embedded in the photon object as
                                                  # defined e.g. in flashgg/MicroAOD/python/flashggRandomizedPerPhotonDiPhotonProducer_cff.py
                                                  #           or in flashgg/MicroAOD/python/flashggRandomizedPhotonProducer_cff.py (if at MicroAOD prod.)
                                                  RandomLabel = cms.string("smearE"),
                                                  Debug = cms.untracked.bool(False),
                                                  ExaggerateShiftUp = cms.untracked.bool(False),
                                                  ),
                                        cms.PSet( PhotonMethodName = cms.string("FlashggPhotonSmearConstant"),
                                                  MethodName = cms.string("FlashggDiPhotonFromPhoton"),
                                                  Label = cms.string("MCSmearLowR9EB"),
                                                  NSigmas = cms.vint32(-1,1),
                                                  OverallRange = cms.string("r9<=0.94&&abs(superCluster.eta)<1.5"),
                                                  BinList = smearBins,
                                                  # has to match the labels embedded in the photon object as
                                                  # defined e.g. in flashgg/MicroAOD/python/flashggRandomizedPerPhotonDiPhotonProducer_cff.py
                                                  #           or in flashgg/MicroAOD/python/flashggRandomizedPhotonProducer_cff.py (if at MicroAOD prod.)
                                                  RandomLabel = cms.string("smearE"),
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
                                                  Label = cms.string("PreselSF"),
                                                  NSigmas = cms.vint32(-1,1),
                                                  OverallRange = cms.string("1"),
                                                  BinList = preselBins,
                                                  Debug = cms.untracked.bool(False)
                                                  ),
                                        cms.PSet( PhotonMethodName = cms.string("FlashggPhotonWeight"),
                                                  MethodName = cms.string("FlashggDiPhotonFromPhoton"),
                                                  Label = cms.string("LooseMvaSF"),
                                                  NSigmas = cms.vint32(-1,1),
                                                  OverallRange = cms.string("1"),
                                                  BinList = looseMvaBins,
                                                  Debug = cms.untracked.bool(False)
                                                  ),
                                        cms.PSet( PhotonMethodName = cms.string("FlashggPhotonSigEOverEShift"),
                                                  MethodName = cms.string("FlashggDiPhotonFromPhoton"),
                                                  Label = cms.string("SigmaEOverEShift"),
                                                  NSigmas = cms.vint32(-1,1),
                                                  OverallRange = cms.string("1"),
                                                  BinList = sigmaEOverEShiftBins,
                                                  Debug = cms.untracked.bool(False)
                                                  ),
                                        cms.PSet( MethodName = cms.string("FlashggDiPhotonWeightFromFracRV"),
                                                  Label = cms.string("FracRVWeight"),
                                                  NSigmas = cms.vint32(-1,1),
                                                  OverallRange = cms.string("1"),
                                                  BinList = RVBins,
                                                  Debug = cms.untracked.bool(False)
                                                  )
                                        )
                                            )
