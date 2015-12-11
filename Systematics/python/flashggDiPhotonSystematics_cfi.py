import FWCore.ParameterSet.Config as cms

scaleBins = cms.PSet(
    variables = cms.vstring("abs(superCluster.eta)","r9"),
    bins = cms.VPSet(
            # scale for prompt 2015, values OK, errors as in Run1 (since Run2 stat. only)
            # see Fasanella et al., ECAL DPG 03/12/2015, https://cern.ch/go/zFH8
            # w.r.t. the numbers in slide 3, the scale is computed as 1. / scale(MC) - 1.
            cms.PSet( lowBounds = cms.vdouble(0.000 , 0.940)    , upBounds = cms.vdouble(1.000 , 999.000) , values = cms.vdouble( 0.00118139404497 )  , uncertainties = cms.vdouble( 0.00050 ) ) ,
            cms.PSet( lowBounds = cms.vdouble(0.000 , -999.000) , upBounds = cms.vdouble(1.000 , 0.940)   , values = cms.vdouble( 0.00458088885317 )  , uncertainties = cms.vdouble( 0.00050 ) ) ,
            cms.PSet( lowBounds = cms.vdouble(1.000 , 0.940)    , upBounds = cms.vdouble(1.500 , 999.000) , values = cms.vdouble( -0.00645802285147 ) , uncertainties = cms.vdouble( 0.00060 ) ) ,
            cms.PSet( lowBounds = cms.vdouble(1.000 , -999.000) , upBounds = cms.vdouble(1.500 , 0.940)   , values = cms.vdouble( 0.00339146314543 )  , uncertainties = cms.vdouble( 0.00120 ) ) ,
            cms.PSet( lowBounds = cms.vdouble(1.500 , -999.000) , upBounds = cms.vdouble(2.000 , 0.940)   , values = cms.vdouble( 0.0138594588018 )   , uncertainties = cms.vdouble( 0.00200 ) ) ,
            cms.PSet( lowBounds = cms.vdouble(1.500 , 0.940)    , upBounds = cms.vdouble(2.000 , 999.000) , values = cms.vdouble( 0.00466162996303 )  , uncertainties = cms.vdouble( 0.00300 ) ) ,
            cms.PSet( lowBounds = cms.vdouble(2.000 , -999.000) , upBounds = cms.vdouble(3.000 , 0.940)   , values = cms.vdouble( 0.021878416906 )    , uncertainties = cms.vdouble( 0.00200 ) ) ,
            cms.PSet( lowBounds = cms.vdouble(2.000 , 0.940)    , upBounds = cms.vdouble(3.000 , 999.000) , values = cms.vdouble( 0.014538334331 )    , uncertainties = cms.vdouble( 0.00300 ) ) ,
                    ))


smearBins = cms.PSet(
    variables = cms.vstring("abs(superCluster.eta)","r9"),
    bins = cms.VPSet(
            # smearings for prompt 2015, values OK, errors as statistical of Run2 (since Run1 different parametrization)
            # see Fasanella et al., ECAL DPG 03/12/2015, https://cern.ch/go/zFH8
            cms.PSet( lowBounds = cms.vdouble(0.000 , -999.000) , upBounds = cms.vdouble(1.000 , 0.940)   , values = cms.vdouble( 0.013654 ) , uncertainties = cms.vdouble( 0.00024652 ) ) ,
            cms.PSet( lowBounds = cms.vdouble(0.000 , 0.940)    , upBounds = cms.vdouble(1.000 , 999.000) , values = cms.vdouble( 0.014142 ) , uncertainties = cms.vdouble( 0.00018319 ) ) ,
            cms.PSet( lowBounds = cms.vdouble(1.000 , -999.000) , upBounds = cms.vdouble(1.500 , 0.940)   , values = cms.vdouble( 0.020859 ) , uncertainties = cms.vdouble( 0.00032435 ) ) ,
            cms.PSet( lowBounds = cms.vdouble(1.000 , 0.940)    , upBounds = cms.vdouble(1.500 , 999.000) , values = cms.vdouble( 0.017120 ) , uncertainties = cms.vdouble( 0.00098551 ) ) ,
            cms.PSet( lowBounds = cms.vdouble(1.500 , -999.000) , upBounds = cms.vdouble(2.000 , 0.940)   , values = cms.vdouble( 0.028083 ) , uncertainties = cms.vdouble( 0.00045816 ) ) ,
            cms.PSet( lowBounds = cms.vdouble(1.500 , 0.940)    , upBounds = cms.vdouble(2.000 , 999.000) , values = cms.vdouble( 0.027289 ) , uncertainties = cms.vdouble( 0.00076386 ) ) ,
            cms.PSet( lowBounds = cms.vdouble(2.000 , -999.000) , upBounds = cms.vdouble(3.000 , 0.940)   , values = cms.vdouble( 0.031793 ) , uncertainties = cms.vdouble( 0.00061517 ) ) ,
            cms.PSet( lowBounds = cms.vdouble(2.000 , 0.940)    , upBounds = cms.vdouble(3.000 , 999.000) , values = cms.vdouble( 0.030831 ) , uncertainties = cms.vdouble( 0.00042066 ) ) ,
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
        cms.PSet( lowBounds = cms.vdouble( 0.0 , 0.0 ) , upBounds = cms.vdouble( 1.5 , 0.9 )  , values = cms.vdouble( 0.9968 ) , uncertainties = cms.vdouble( 0.0227 ) ) ,
        cms.PSet( lowBounds = cms.vdouble( 0.0 , 0.9 ) , upBounds = cms.vdouble( 1.5 , 999. ) , values = cms.vdouble( 0.9978 ) , uncertainties = cms.vdouble( 0.0029 ) ) ,
        cms.PSet( lowBounds = cms.vdouble( 1.5 , 0.0 ) , upBounds = cms.vdouble( 2.5 , 0.9)   , values = cms.vdouble( 1.0115 ) , uncertainties = cms.vdouble( 0.0297 ) ) ,
        cms.PSet( lowBounds = cms.vdouble( 1.5 , 0.9 ) , upBounds = cms.vdouble( 2.5 , 999. ) , values = cms.vdouble( 0.9963 ) , uncertainties = cms.vdouble( 0.0044 ) )
        )
    )

# Slide 11, https://indico.cern.ch/event/367883/contribution/2/attachments/1194817/1736347/egamma_26_11_2015.pdf
looseMvaBins = cms.PSet(
    variables = cms.vstring("abs(superCluster.eta)","r9"),
    bins = cms.VPSet(
        cms.PSet( lowBounds = cms.vdouble( 0.0 , 0.0 ) , upBounds = cms.vdouble( 1.5 , 0.9 )   , values = cms.vdouble( 1.0001 ) , uncertainties = cms.vdouble( 0.0022 ) ) ,
        cms.PSet( lowBounds = cms.vdouble( 0.0 , 0.9 ) , upBounds = cms.vdouble( 1.5 , 999.0 ) , values = cms.vdouble( 1.000 )  , uncertainties = cms.vdouble( 0.0001 ) ) ,
        cms.PSet( lowBounds = cms.vdouble( 1.5 , 0.0 ) , upBounds = cms.vdouble( 2.5 , 0.9 )   , values = cms.vdouble( 0.9851 ) , uncertainties = cms.vdouble( 0.0016 ) ) ,
        cms.PSet( lowBounds = cms.vdouble( 1.5 , 0.9 ) , upBounds = cms.vdouble( 2.5 , 999.0 ) , values = cms.vdouble( 1.0001 ) , uncertainties = cms.vdouble( 0.0009 ) )
        )
    )



# RELATIVE shift of sigmaE/E --> 0.05 corresponds to a shift of 5%
sigmaEOverEShiftBins = cms.PSet(
    variables = cms.vstring("abs(superCluster.eta)"),
    bins = cms.VPSet(
         cms.PSet( lowBounds = cms.vdouble(0.000), upBounds = cms.vdouble(999.), values = cms.vdouble( 0.0 ), uncertainties = cms.vdouble( 0.05 ))
                     )
    )


flashggDiPhotonSystematics = cms.EDProducer('FlashggDiPhotonSystematicProducer',
		src = cms.InputTag("flashggFinalEGamma","finalDiPhotons"),
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
                                                  )
                                        )
)
