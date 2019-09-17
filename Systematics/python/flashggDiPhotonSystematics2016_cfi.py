import FWCore.ParameterSet.Config as cms

# No default. Latest is only in the EGM tool
photonSmearBins = cms.PSet()
photonScaleUncertBins = cms.PSet()

mvaShiftBins = cms.PSet(
    variables = cms.vstring("abs(superCluster.eta)"),
    bins = cms.VPSet(
                     cms.PSet( lowBounds = cms.vdouble(0.000), upBounds = cms.vdouble(999.), values = cms.vdouble( 0.0 ), uncertainties = cms.vdouble( 0.00 ))
                     )
    )

# from Arnab via Martina 10/03/2017
preselBins = cms.PSet(
    variables = cms.vstring("abs(superCluster.eta)","full5x5_r9"),
    bins = cms.VPSet(
        cms.PSet( lowBounds = cms.vdouble( 0.0, 0.0 ) , upBounds = cms.vdouble( 1.5, 0.85 ) , values = cms.vdouble( 1.0057 ) , uncertainties = cms.vdouble( 0.0010 )  ) ,
        cms.PSet( lowBounds = cms.vdouble( 0.0, 0.85 ), upBounds = cms.vdouble( 1.5, 999. ) , values = cms.vdouble( 0.9988 ) , uncertainties = cms.vdouble( 0.0009 )  ) ,
        cms.PSet( lowBounds = cms.vdouble( 1.5, 0.0 ) , upBounds = cms.vdouble( 6.0, 0.9  ) , values = cms.vdouble( 0.9443 ) , uncertainties = cms.vdouble( 0.0072 )  ) ,
        cms.PSet( lowBounds = cms.vdouble( 1.5, 0.9 ) , upBounds = cms.vdouble( 6.0, 999. ) , values = cms.vdouble( 0.9947 ) , uncertainties = cms.vdouble( 0.0051 )  ) 
        )
    )

# JTao: slide 2 of https://indico.cern.ch/event/762183/contributions/3181242/attachments/1736403/2823204/201810_2016Legacy_e-veto_etc.pdf with 2016 legacy data samples, small changes on teh SFs wrt previous results with 2016 rereco data samples 
electronVetoBins = cms.PSet(
    variables = cms.vstring("abs(superCluster.eta)","full5x5_r9"),
    bins = cms.VPSet(
        cms.PSet( lowBounds = cms.vdouble( 0.0, 0.00 ) , upBounds = cms.vdouble( 1.5, 0.85 ) , values = cms.vdouble( 0.9939 ) , uncertainties = cms.vdouble( 0.0023 )  ) ,
        cms.PSet( lowBounds = cms.vdouble( 0.0, 0.85 ) , upBounds = cms.vdouble( 1.5, 999. ) , values = cms.vdouble( 0.9955 ) , uncertainties = cms.vdouble( 0.0006 )  ) ,
        cms.PSet( lowBounds = cms.vdouble( 1.5, 0.00 ) , upBounds = cms.vdouble( 6.0, 0.90 ) , values = cms.vdouble( 0.9709 ) , uncertainties = cms.vdouble( 0.0068 )  ) ,
        cms.PSet( lowBounds = cms.vdouble( 1.5, 0.90 ) , upBounds = cms.vdouble( 6.0, 999. ) , values = cms.vdouble( 0.9916 ) , uncertainties = cms.vdouble( 0.0017 )  ) 
        )
    )

FNUFBins = cms.PSet(
    variables = cms.vstring("abs(superCluster.eta)","full5x5_r9"),
    bins = cms.VPSet(
        cms.PSet( lowBounds = cms.vdouble( 0.0, 0.00 ) , upBounds = cms.vdouble( 1.5, 0.94 ) , values = cms.vdouble( 0. ) , uncertainties = cms.vdouble(  0.0007 )  ) ,
        cms.PSet( lowBounds = cms.vdouble( 0.0, 0.94 ) , upBounds = cms.vdouble( 1.5, 999. ) , values = cms.vdouble( 0. ) , uncertainties = cms.vdouble( -0.0007 )  ) ,
        cms.PSet( lowBounds = cms.vdouble( 1.5, 0.00 ) , upBounds = cms.vdouble( 6.0, 0.94 ) , values = cms.vdouble( 0. ) , uncertainties = cms.vdouble(  0.0007 )  ) ,
        cms.PSet( lowBounds = cms.vdouble( 1.5, 0.94 ) , upBounds = cms.vdouble( 6.0, 999. ) , values = cms.vdouble( 0. ) , uncertainties = cms.vdouble( -0.0007 )  )
        )
    )

# from Martina: https://indico.cern.ch/event/628676/contributions/2546615/attachments/1440085/2216643/20170405_martina_regrEchecksUpdate.pdf
showerShapeBins = cms.PSet(
    variables = cms.vstring("abs(superCluster.eta)","full5x5_r9"),
    bins = cms.VPSet(
        #EB low R9
        cms.PSet( lowBounds = cms.vdouble( 0.0, 0.00 ) , upBounds = cms.vdouble( 1.0, 0.94 ) , values = cms.vdouble( 0. ) , uncertainties = cms.vdouble( -0.0001 )  ) ,
        cms.PSet( lowBounds = cms.vdouble( 1.0, 0.00 ) , upBounds = cms.vdouble( 1.5, 0.94 ) , values = cms.vdouble( 0. ) , uncertainties = cms.vdouble(  0.0002 )  ) ,
        #EB high R9
        cms.PSet( lowBounds = cms.vdouble( 0.0, 0.94 ) , upBounds = cms.vdouble( 1.0, 999. ) , values = cms.vdouble( 0. ) , uncertainties = cms.vdouble( -0.0006 )  ) ,
        cms.PSet( lowBounds = cms.vdouble( 1.0, 0.94 ) , upBounds = cms.vdouble( 1.5, 999. ) , values = cms.vdouble( 0. ) , uncertainties = cms.vdouble( -0.0011 )  ) ,
        #EE low R9
        cms.PSet( lowBounds = cms.vdouble( 1.5, 0.00 ) , upBounds = cms.vdouble( 2.0, 0.94 ) , values = cms.vdouble( 0. ) , uncertainties = cms.vdouble( 0.0015 )  ) ,
        cms.PSet( lowBounds = cms.vdouble( 2.0, 0.00 ) , upBounds = cms.vdouble( 6.0, 0.94 ) , values = cms.vdouble( 0. ) , uncertainties = cms.vdouble( 0.0004 )  ) ,
        #EE high R9
        cms.PSet( lowBounds = cms.vdouble( 1.5, 0.94 ) , upBounds = cms.vdouble( 2.0, 999. ) , values = cms.vdouble( 0. ) , uncertainties = cms.vdouble( 0.0002 )  ) ,
        cms.PSet( lowBounds = cms.vdouble( 2.0, 0.94 ) , upBounds = cms.vdouble( 6.0, 999. ) , values = cms.vdouble( 0. ) , uncertainties = cms.vdouble( 0.0003 )  ) 
        )
    )


#for full 2016 rereco B-G + prompt H dataset.  Trigger scale factors for use without HLT applied in MC

leadTriggerScaleBins = cms.PSet(
    variables = cms.vstring("full5x5_r9","abs(superCluster.eta)","pt"),
    bins = cms.VPSet(
        cms.PSet(lowBounds = cms.vdouble(0.0,0.0,0.0), upBounds = cms.vdouble(0.54,1.5,33.3333), values = cms.vdouble(0.6810978055), uncertainties = cms.vdouble(0.00519262468286,0.00519262468286)),
        cms.PSet(lowBounds = cms.vdouble(0.0,0.0,33.3333), upBounds = cms.vdouble(0.54,1.5,35.0), values = cms.vdouble(0.762424051762), uncertainties = cms.vdouble(0.00606810658022,0.00606810658022)),
        cms.PSet(lowBounds = cms.vdouble(0.0,0.0,35.0), upBounds = cms.vdouble(0.54,1.5,40.0), values = cms.vdouble(0.778700113297), uncertainties = cms.vdouble(0.00311642304626,0.00311642304626)),
        cms.PSet(lowBounds = cms.vdouble(0.0,0.0,40.0), upBounds = cms.vdouble(0.54,1.5,45.0), values = cms.vdouble(0.795413970947), uncertainties = cms.vdouble(0.00303555330709,0.00303555330709)),
        cms.PSet(lowBounds = cms.vdouble(0.0,0.0,45.0), upBounds = cms.vdouble(0.54,1.5,50.0), values = cms.vdouble(0.808078825474), uncertainties = cms.vdouble(0.00422688722282,0.00422688722282)),
        cms.PSet(lowBounds = cms.vdouble(0.0,0.0,50.0), upBounds = cms.vdouble(0.54,1.5,60.0), values = cms.vdouble(0.819607198238), uncertainties = cms.vdouble(0.0074540959624,0.0074540959624)),
        cms.PSet(lowBounds = cms.vdouble(0.0,0.0,60.0), upBounds = cms.vdouble(0.54,1.5,70.0), values = cms.vdouble(0.851548194885), uncertainties = cms.vdouble(0.0158679720146,0.0158679720146)),
        cms.PSet(lowBounds = cms.vdouble(0.0,0.0,70.0), upBounds = cms.vdouble(0.54,1.5,90.0), values = cms.vdouble(0.868812680244), uncertainties = cms.vdouble(0.0302805485333,0.0302805485333)),
        cms.PSet(lowBounds = cms.vdouble(0.0,0.0,90.0), upBounds = cms.vdouble(0.54,1.5,99999999), values = cms.vdouble(0.84904062748), uncertainties = cms.vdouble(0.0736341456908,0.0736341456908)),
        cms.PSet(lowBounds = cms.vdouble(0.54,0.0,0.0), upBounds = cms.vdouble(0.85,1.5,33.3333), values = cms.vdouble(0.898902833462), uncertainties = cms.vdouble(0.00100959242125,0.00100959242125)),
        cms.PSet(lowBounds = cms.vdouble(0.54,0.0,33.3333), upBounds = cms.vdouble(0.85,1.5,35.0), values = cms.vdouble(0.958884656429), uncertainties = cms.vdouble(0.000808069907818,0.000808069907818)),
        cms.PSet(lowBounds = cms.vdouble(0.54,0.0,35.0), upBounds = cms.vdouble(0.85,1.5,40.0), values = cms.vdouble(0.965837776661), uncertainties = cms.vdouble(0.000305692172361,0.000305692172361)),
        cms.PSet(lowBounds = cms.vdouble(0.54,0.0,40.0), upBounds = cms.vdouble(0.85,1.5,45.0), values = cms.vdouble(0.970546543598), uncertainties = cms.vdouble(0.00026519245806,0.00026519245806)),
        cms.PSet(lowBounds = cms.vdouble(0.54,0.0,45.0), upBounds = cms.vdouble(0.85,1.5,50.0), values = cms.vdouble(0.973720788956), uncertainties = cms.vdouble(0.000322496232078,0.000322496232078)),
        cms.PSet(lowBounds = cms.vdouble(0.54,0.0,50.0), upBounds = cms.vdouble(0.85,1.5,60.0), values = cms.vdouble(0.976857423782), uncertainties = cms.vdouble(0.00049804973141,0.00049804973141)),
        cms.PSet(lowBounds = cms.vdouble(0.54,0.0,60.0), upBounds = cms.vdouble(0.85,1.5,70.0), values = cms.vdouble(0.980784952641), uncertainties = cms.vdouble(0.000875908397042,0.000875908397042)),
        cms.PSet(lowBounds = cms.vdouble(0.54,0.0,70.0), upBounds = cms.vdouble(0.85,1.5,90.0), values = cms.vdouble(0.985170722008), uncertainties = cms.vdouble(0.00134766564404,0.00134766564404)),
        cms.PSet(lowBounds = cms.vdouble(0.54,0.0,90.0), upBounds = cms.vdouble(0.85,1.5,99999999), values = cms.vdouble(0.989234626293), uncertainties = cms.vdouble(0.00161217085916,0.00161217085916)),
        cms.PSet(lowBounds = cms.vdouble(0.85,0.0,0.0), upBounds = cms.vdouble(999,1.5,33.3333), values = cms.vdouble(0.924440562725), uncertainties = cms.vdouble(0.000377342171653,0.000377342171653)),
        cms.PSet(lowBounds = cms.vdouble(0.85,0.0,33.3333), upBounds = cms.vdouble(999,1.5,35.0), values = cms.vdouble(0.971459209919), uncertainties = cms.vdouble(0.000281199348832,0.000281199348832)),
        cms.PSet(lowBounds = cms.vdouble(0.85,0.0,35.0), upBounds = cms.vdouble(999,1.5,40.0), values = cms.vdouble(0.979482769966), uncertainties = cms.vdouble(0.000115534010648,0.000115534010648)),
        cms.PSet(lowBounds = cms.vdouble(0.85,0.0,40.0), upBounds = cms.vdouble(999,1.5,45.0), values = cms.vdouble(0.982116281986), uncertainties = cms.vdouble(9.05590478798e-05,9.05590478798e-05)),
        cms.PSet(lowBounds = cms.vdouble(0.85,0.0,45.0), upBounds = cms.vdouble(999,1.5,50.0), values = cms.vdouble(0.98511326313), uncertainties = cms.vdouble(0.000105097340197,0.000105097340197)),
        cms.PSet(lowBounds = cms.vdouble(0.85,0.0,50.0), upBounds = cms.vdouble(999,1.5,60.0), values = cms.vdouble(0.984966993332), uncertainties = cms.vdouble(0.00111781327877,0.00111781327877)),
        cms.PSet(lowBounds = cms.vdouble(0.85,0.0,60.0), upBounds = cms.vdouble(999,1.5,70.0), values = cms.vdouble(0.986653208733), uncertainties = cms.vdouble(0.000295146360226,0.000295146360226)),
        cms.PSet(lowBounds = cms.vdouble(0.85,0.0,70.0), upBounds = cms.vdouble(999,1.5,90.0), values = cms.vdouble(0.98967808485), uncertainties = cms.vdouble(0.000409470102893,0.000409470102893)),
        cms.PSet(lowBounds = cms.vdouble(0.85,0.0,90.0), upBounds = cms.vdouble(999,1.5,99999999), values = cms.vdouble(0.991826117039), uncertainties = cms.vdouble(0.000323844893609,0.000323844893609)),

        cms.PSet(lowBounds = cms.vdouble(0.0,1.5,0.0), upBounds = cms.vdouble(0.84,3.0,33.3333), values = cms.vdouble(0.632706999779), uncertainties = cms.vdouble(0.00549277181579,0.00549277181579)),
        cms.PSet(lowBounds = cms.vdouble(0.0,1.5,33.3333), upBounds = cms.vdouble(0.84,3.0,35.0), values = cms.vdouble(0.705228030682), uncertainties = cms.vdouble(0.00671890259184,0.00671890259184)),
        cms.PSet(lowBounds = cms.vdouble(0.0,1.5,35.0), upBounds = cms.vdouble(0.84,3.0,40.0), values = cms.vdouble(0.729334890842), uncertainties = cms.vdouble(0.00316854098441,0.00316854098441)),
        cms.PSet(lowBounds = cms.vdouble(0.0,1.5,40.0), upBounds = cms.vdouble(0.84,3.0,45.0), values = cms.vdouble(0.752555906773), uncertainties = cms.vdouble(0.00315251887441,0.00315251887441)),
        cms.PSet(lowBounds = cms.vdouble(0.0,1.5,45.0), upBounds = cms.vdouble(0.84,3.0,50.0), values = cms.vdouble(0.770330071449), uncertainties = cms.vdouble(0.00465503941806,0.00465503941806)),
        cms.PSet(lowBounds = cms.vdouble(0.0,1.5,50.0), upBounds = cms.vdouble(0.84,3.0,60.0), values = cms.vdouble(0.786793231964), uncertainties = cms.vdouble(0.00682801354333,0.00682801354333)),
        cms.PSet(lowBounds = cms.vdouble(0.0,1.5,60.0), upBounds = cms.vdouble(0.84,3.0,70.0), values = cms.vdouble(0.816314458847), uncertainties = cms.vdouble(0.0133762782917,0.0133762782917)),
        cms.PSet(lowBounds = cms.vdouble(0.0,1.5,70.0), upBounds = cms.vdouble(0.84,3.0,90.0), values = cms.vdouble(0.857941806316), uncertainties = cms.vdouble(0.0154562893515,0.0154562893515)),
        cms.PSet(lowBounds = cms.vdouble(0.,1.5,90.0), upBounds = cms.vdouble(0.84,3.0,99999999), values = cms.vdouble(0.886136233807), uncertainties = cms.vdouble(0.0226681113534,0.0226681113534)),
        cms.PSet(lowBounds = cms.vdouble(0.84,1.5,0.0), upBounds = cms.vdouble(0.9,3.0,33.3333), values = cms.vdouble(0.872963190079), uncertainties = cms.vdouble(0.00255836970587,0.00255836970587)),
        cms.PSet(lowBounds = cms.vdouble(0.84,1.5,33.3333), upBounds = cms.vdouble(0.9,3.0,35.0), values = cms.vdouble(0.971260070801), uncertainties = cms.vdouble(0.00169839848133,0.00169839848133)),
        cms.PSet(lowBounds = cms.vdouble(0.84,1.5,35.0), upBounds = cms.vdouble(0.9,3.0,40.0), values = cms.vdouble(0.98152244091), uncertainties = cms.vdouble(0.000588619026864,0.000588619026864)),
        cms.PSet(lowBounds = cms.vdouble(0.84,1.5,40.0), upBounds = cms.vdouble(0.9,3.0,45.0), values = cms.vdouble(0.986895382404), uncertainties = cms.vdouble(0.000503406678232,0.000503406678232)),
        cms.PSet(lowBounds = cms.vdouble(0.84,1.5,45.0), upBounds = cms.vdouble(0.9,3.0,50.0), values = cms.vdouble(0.98783403635), uncertainties = cms.vdouble(0.000744728019948,0.000744728019948)),
        cms.PSet(lowBounds = cms.vdouble(0.84,1.5,50.0), upBounds = cms.vdouble(0.9,3.0,60.0), values = cms.vdouble(0.988175034523), uncertainties = cms.vdouble(0.00106780215648,0.00106780215648)),
        cms.PSet(lowBounds = cms.vdouble(0.84,1.5,60.0), upBounds = cms.vdouble(0.9,3.0,70.0), values = cms.vdouble(0.990262210369), uncertainties = cms.vdouble(0.0019675187127,0.0019675187127)),
        cms.PSet(lowBounds = cms.vdouble(0.84,1.5,70.0), upBounds = cms.vdouble(0.9,3.0,90.0), values = cms.vdouble(0.990519165993), uncertainties = cms.vdouble(0.00309011400271,0.00309011400271)),
        cms.PSet(lowBounds = cms.vdouble(0.84,1.5,90.0), upBounds = cms.vdouble(0.9,3.0,99999999), values = cms.vdouble(0.997375607491), uncertainties = cms.vdouble(0.00160821560062,0.00160821560062)),
        cms.PSet(lowBounds = cms.vdouble(0.9,1.5,0.0), upBounds = cms.vdouble(999,3.0,33.3333), values = cms.vdouble(0.827793955803), uncertainties = cms.vdouble(0.000860672672767,0.000860672672767)),
        cms.PSet(lowBounds = cms.vdouble(0.9,1.5,33.3333), upBounds = cms.vdouble(999,3.0,35.0), values = cms.vdouble(0.966853737831), uncertainties = cms.vdouble(0.00050528634985,0.00050528634985)),
        cms.PSet(lowBounds = cms.vdouble(0.9,1.5,35.0), upBounds = cms.vdouble(999,3.0,40.0), values = cms.vdouble(0.980059444904), uncertainties = cms.vdouble(0.000211483705081,0.000211483705081)),
        cms.PSet(lowBounds = cms.vdouble(0.9,1.5,40.0), upBounds = cms.vdouble(999,3.0,45.0), values = cms.vdouble(0.98436075449), uncertainties = cms.vdouble(0.000193397430742,0.000193397430742)),
        cms.PSet(lowBounds = cms.vdouble(0.9,1.5,45.0), upBounds = cms.vdouble(999,3.0,50.0), values = cms.vdouble(0.985809862614), uncertainties = cms.vdouble(0.000283829305427,0.000283829305427)),
        cms.PSet(lowBounds = cms.vdouble(0.9,1.5,50.0), upBounds = cms.vdouble(999,3.0,60.0), values = cms.vdouble(0.985918521881), uncertainties = cms.vdouble(0.0011583820968,0.0011583820968)),
        cms.PSet(lowBounds = cms.vdouble(0.9,1.5,60.0), upBounds = cms.vdouble(999,3.0,70.0), values = cms.vdouble(0.990703165531), uncertainties = cms.vdouble(0.000662821684507,0.000662821684507)),
        cms.PSet(lowBounds = cms.vdouble(0.9,1.5,70.0), upBounds = cms.vdouble(999,3.0,90.0), values = cms.vdouble(0.994281888008), uncertainties = cms.vdouble(0.000630687802869,0.000630687802869)),
        cms.PSet(lowBounds = cms.vdouble(0.9,1.5,90.0), upBounds = cms.vdouble(999,3.0,99999999), values = cms.vdouble(0.997075855732), uncertainties = cms.vdouble(0.000553451337113,0.000553451337113)),
        
        cms.PSet(lowBounds = cms.vdouble(0.0,3.,0), upBounds = cms.vdouble(999,999,999999),     values = cms.vdouble(1.), uncertainties = cms.vdouble(1.,1.))
        )
    )


subleadTriggerScaleBins = cms.PSet(
    variables = cms.vstring("full5x5_r9","abs(superCluster.eta)","pt"),
    bins = cms.VPSet(
        cms.PSet(lowBounds = cms.vdouble(0.0,0.0,0.0), upBounds = cms.vdouble(0.54,1.5,22.5), values = cms.vdouble(0.735150516033), uncertainties = cms.vdouble(0.0124989687013,0.0124989687013)),
        cms.PSet(lowBounds = cms.vdouble(0.0,0.0,22.5), upBounds = cms.vdouble(0.54,1.5,25.0), values = cms.vdouble(0.741944074631), uncertainties = cms.vdouble(0.0104878349021,0.0104878349021)),
        cms.PSet(lowBounds = cms.vdouble(0.0,0.0,25.0), upBounds = cms.vdouble(0.54,1.5,27.5), values = cms.vdouble(0.759739041328), uncertainties = cms.vdouble(0.0085182165305,0.0085182165305)),
        cms.PSet(lowBounds = cms.vdouble(0.0,0.0,27.5), upBounds = cms.vdouble(0.54,1.5,30.0), values = cms.vdouble(0.768184542656), uncertainties = cms.vdouble(0.00742963937731,0.00742963937731)),
        cms.PSet(lowBounds = cms.vdouble(0.0,0.0,30.0), upBounds = cms.vdouble(0.54,1.5,32.5), values = cms.vdouble(0.781085252762), uncertainties = cms.vdouble(0.00607655931058,0.00607655931058)),
        cms.PSet(lowBounds = cms.vdouble(0.0,0.0,32.5), upBounds = cms.vdouble(0.54,1.5,35.0), values = cms.vdouble(0.787775456905), uncertainties = cms.vdouble(0.00543688578024,0.00543688578024)),
        cms.PSet(lowBounds = cms.vdouble(0.0,0.0,35.0), upBounds = cms.vdouble(0.54,1.5,40.0), values = cms.vdouble(0.804114878178), uncertainties = cms.vdouble(0.00298680600149,0.00298680600149)),
        cms.PSet(lowBounds = cms.vdouble(0.0,0.0,40.0), upBounds = cms.vdouble(0.54,1.5,45.0), values = cms.vdouble(0.819701194763), uncertainties = cms.vdouble(0.00290229435272,0.00290229435272)),
        cms.PSet(lowBounds = cms.vdouble(0.0,0.0,45.0), upBounds = cms.vdouble(0.54,1.5,50.0), values = cms.vdouble(0.832450091839), uncertainties = cms.vdouble(0.00402395578062,0.00402395578062)),
        cms.PSet(lowBounds = cms.vdouble(0.0,0.0,50.0), upBounds = cms.vdouble(0.54,1.5,60.0), values = cms.vdouble(0.840809226036), uncertainties = cms.vdouble(0.00712945852573,0.00712945852573)),
        cms.PSet(lowBounds = cms.vdouble(0.0,0.0,60.0), upBounds = cms.vdouble(0.54,1.5,70.0), values = cms.vdouble(0.863569915295), uncertainties = cms.vdouble(0.0158928774505,0.0158928774505)),
        cms.PSet(lowBounds = cms.vdouble(0.0,0.0,70.0), upBounds = cms.vdouble(0.54,1.5,90.0), values = cms.vdouble(0.873844146729), uncertainties = cms.vdouble(0.0302822319315,0.0302822319315)),
        cms.PSet(lowBounds = cms.vdouble(0.0,0.0,90.0), upBounds = cms.vdouble(0.54,1.5,99999999), values = cms.vdouble(0.880566418171), uncertainties = cms.vdouble(0.0771217355214,0.0771217355214)),
        cms.PSet(lowBounds = cms.vdouble(0.54,0.0,0.0), upBounds = cms.vdouble(0.85,1.5,22.5), values = cms.vdouble(0.986665070057), uncertainties = cms.vdouble(0.00290137137345,0.00290137137345)),
        cms.PSet(lowBounds = cms.vdouble(0.54,0.0,22.5), upBounds = cms.vdouble(0.85,1.5,25.0), values = cms.vdouble(0.991021811962), uncertainties = cms.vdouble(0.000881010903373,0.000881010903373)),
        cms.PSet(lowBounds = cms.vdouble(0.54,0.0,25.0), upBounds = cms.vdouble(0.85,1.5,27.5), values = cms.vdouble(0.993968784809), uncertainties = cms.vdouble(0.000365351913748,0.000365351913748)),
        cms.PSet(lowBounds = cms.vdouble(0.54,0.0,27.5), upBounds = cms.vdouble(0.85,1.5,30.0), values = cms.vdouble(0.992568671703), uncertainties = cms.vdouble(0.00080863027601,0.00080863027601)),
        cms.PSet(lowBounds = cms.vdouble(0.54,0.0,30.0), upBounds = cms.vdouble(0.85,1.5,32.5), values = cms.vdouble(0.992429018021), uncertainties = cms.vdouble(0.000927701357281,0.000927701357281)),
        cms.PSet(lowBounds = cms.vdouble(0.54,0.0,32.5), upBounds = cms.vdouble(0.85,1.5,35.0), values = cms.vdouble(0.993385374546), uncertainties = cms.vdouble(0.000273290379079,0.000273290379079)),
        cms.PSet(lowBounds = cms.vdouble(0.54,0.0,35.0), upBounds = cms.vdouble(0.85,1.5,40.0), values = cms.vdouble(0.993640601635), uncertainties = cms.vdouble(0.000134285419182,0.000134285419182)),
        cms.PSet(lowBounds = cms.vdouble(0.54,0.0,40.0), upBounds = cms.vdouble(0.85,1.5,45.0), values = cms.vdouble(0.994162678719), uncertainties = cms.vdouble(0.000117730435885,0.000117730435885)),
        cms.PSet(lowBounds = cms.vdouble(0.54,0.0,45.0), upBounds = cms.vdouble(0.85,1.5,50.0), values = cms.vdouble(0.993752419949), uncertainties = cms.vdouble(0.000159345143113,0.000159345143113)),
        cms.PSet(lowBounds = cms.vdouble(0.54,0.0,50.0), upBounds = cms.vdouble(0.85,1.5,60.0), values = cms.vdouble(0.993596553802), uncertainties = cms.vdouble(0.000408447927548,0.000408447927548)),
        cms.PSet(lowBounds = cms.vdouble(0.54,0.0,60.0), upBounds = cms.vdouble(0.85,1.5,70.0), values = cms.vdouble(0.993239223957), uncertainties = cms.vdouble(0.000583930534644,0.000583930534644)),
        cms.PSet(lowBounds = cms.vdouble(0.54,0.0,70.0), upBounds = cms.vdouble(0.85,1.5,90.0), values = cms.vdouble(0.992464900017), uncertainties = cms.vdouble(0.00420925695041,0.00420925695041)),
        cms.PSet(lowBounds = cms.vdouble(0.54,0.0,90.0), upBounds = cms.vdouble(0.85,1.5,99999999), values = cms.vdouble(0.992560684681), uncertainties = cms.vdouble(0.001409051152,0.001409051152)),
        cms.PSet(lowBounds = cms.vdouble(0.85,0.0,0.0), upBounds = cms.vdouble(999,1.5,22.5), values = cms.vdouble(0.99289894104), uncertainties = cms.vdouble(0.000459394846531,0.000459394846531)),
        cms.PSet(lowBounds = cms.vdouble(0.85,0.0,22.5), upBounds = cms.vdouble(999,1.5,25.0), values = cms.vdouble(0.994199693203), uncertainties = cms.vdouble(0.000459394846531,0.000459394846531)),
        cms.PSet(lowBounds = cms.vdouble(0.85,0.0,25.0), upBounds = cms.vdouble(999,1.5,27.5), values = cms.vdouble(0.996753513813), uncertainties = cms.vdouble(0.00019635266302,0.00019635266302)),
        cms.PSet(lowBounds = cms.vdouble(0.85,0.0,27.5), upBounds = cms.vdouble(999,1.5,30.0), values = cms.vdouble(0.997437953949), uncertainties = cms.vdouble(0.000105276812567,0.000105276812567)),
        cms.PSet(lowBounds = cms.vdouble(0.85,0.0,30.0), upBounds = cms.vdouble(999,1.5,32.5), values = cms.vdouble(0.998195171356), uncertainties = cms.vdouble(0.000120250429564,0.000120250429564)),
        cms.PSet(lowBounds = cms.vdouble(0.85,0.0,32.5), upBounds = cms.vdouble(999,1.5,35.0), values = cms.vdouble(0.998485267162), uncertainties = cms.vdouble(5.53143156827e-05,5.53143156827e-05)),
        cms.PSet(lowBounds = cms.vdouble(0.85,0.0,35.0), upBounds = cms.vdouble(999,1.5,40.0), values = cms.vdouble(0.999016582966), uncertainties = cms.vdouble(2.49504923609e-05,2.49504923609e-05)),
        cms.PSet(lowBounds = cms.vdouble(0.85,0.0,40.0), upBounds = cms.vdouble(999,1.5,45.0), values = cms.vdouble(0.999399483204), uncertainties = cms.vdouble(1.67820877013e-05,1.67820877013e-05)),
        cms.PSet(lowBounds = cms.vdouble(0.85,0.0,45.0), upBounds = cms.vdouble(999,1.5,50.0), values = cms.vdouble(0.999610304832), uncertainties = cms.vdouble(1.71324623735e-05,1.71324623735e-05)),
        cms.PSet(lowBounds = cms.vdouble(0.85,0.0,50.0), upBounds = cms.vdouble(999,1.5,60.0), values = cms.vdouble(0.999711513519), uncertainties = cms.vdouble(2.13249403322e-05,2.13249403322e-05)),
        cms.PSet(lowBounds = cms.vdouble(0.85,0.0,60.0), upBounds = cms.vdouble(999,1.5,70.0), values = cms.vdouble(0.99971395731), uncertainties = cms.vdouble(4.15814070615e-05,4.15814070615e-05)),
        cms.PSet(lowBounds = cms.vdouble(0.85,0.0,70.0), upBounds = cms.vdouble(999,1.5,90.0), values = cms.vdouble(0.999810993671), uncertainties = cms.vdouble(4.80188315009e-05,4.80188315009e-05)),
        cms.PSet(lowBounds = cms.vdouble(0.85,0.0,90.0), upBounds = cms.vdouble(999,1.5,99999999), values = cms.vdouble(0.999862849712), uncertainties = cms.vdouble(4.13267813275e-05,4.13267813275e-05)),
        
        
        cms.PSet(lowBounds = cms.vdouble(0.0,1.5,0.0), upBounds = cms.vdouble(0.84,3.0,22.5), values = cms.vdouble(0.660493433475), uncertainties = cms.vdouble(0.0189502448237,0.0189502448237)),
        cms.PSet(lowBounds = cms.vdouble(0.0,1.5,22.5), upBounds = cms.vdouble(0.84,3.0,25.0), values = cms.vdouble(0.675408184528), uncertainties = cms.vdouble(0.0141567263708,0.0141567263708)),
        cms.PSet(lowBounds = cms.vdouble(0.0,1.5,25.0), upBounds = cms.vdouble(0.84,3.0,27.5), values = cms.vdouble(0.694644391537), uncertainties = cms.vdouble(0.0109544486394,0.0109544486394)),
        cms.PSet(lowBounds = cms.vdouble(0.0,1.5,27.5), upBounds = cms.vdouble(0.84,3.0,30.0), values = cms.vdouble(0.705103397369), uncertainties = cms.vdouble(0.00740787194485,0.00740787194485)),
        cms.PSet(lowBounds = cms.vdouble(0.0,1.5,30.0), upBounds = cms.vdouble(0.84,3.0,32.5), values = cms.vdouble(0.712502062321), uncertainties = cms.vdouble(0.00673718942098,0.00673718942098)),
        cms.PSet(lowBounds = cms.vdouble(0.0,1.5,32.5), upBounds = cms.vdouble(0.84,3.0,35.0), values = cms.vdouble(0.719795048237), uncertainties = cms.vdouble(0.00535038292038,0.00535038292038)),
        cms.PSet(lowBounds = cms.vdouble(0.0,1.5,35.0), upBounds = cms.vdouble(0.84,3.0,40.0), values = cms.vdouble(0.736499130726), uncertainties = cms.vdouble(0.00315590127157,0.00315590127157)),
        cms.PSet(lowBounds = cms.vdouble(0.0,1.5,40.0), upBounds = cms.vdouble(0.84,3.0,45.0), values = cms.vdouble(0.757362127304), uncertainties = cms.vdouble(0.00315107032637,0.00315107032637)),
        cms.PSet(lowBounds = cms.vdouble(0.0,1.5,45.0), upBounds = cms.vdouble(0.84,3.0,50.0), values = cms.vdouble(0.77392333746), uncertainties = cms.vdouble(0.00466395309879,0.00466395309879)),
        cms.PSet(lowBounds = cms.vdouble(0.0,1.5,50.0), upBounds = cms.vdouble(0.84,3.0,60.0), values = cms.vdouble(0.78893083334), uncertainties = cms.vdouble(0.00687048443179,0.00687048443179)),
        cms.PSet(lowBounds = cms.vdouble(0.0,1.5,60.0), upBounds = cms.vdouble(0.84,3.0,70.0), values = cms.vdouble(0.818382263184), uncertainties = cms.vdouble(0.0135113622786,0.0135113622786)),
        cms.PSet(lowBounds = cms.vdouble(0.0,1.5,70.0), upBounds = cms.vdouble(0.84,3.0,90.0), values = cms.vdouble(0.841334581375), uncertainties = cms.vdouble(0.0180361620001,0.0180361620001)),
        cms.PSet(lowBounds = cms.vdouble(0.0,1.5,90.0), upBounds = cms.vdouble(0.84,3.0,99999999), values = cms.vdouble(0.868076682091), uncertainties = cms.vdouble(0.0243523219982,0.0243523219982)),
        cms.PSet(lowBounds = cms.vdouble(0.84,1.5,0.0), upBounds = cms.vdouble(0.9,3.0,22.5), values = cms.vdouble(0.976537823677), uncertainties = cms.vdouble(0.00489157641841,0.00489157641841)),
        cms.PSet(lowBounds = cms.vdouble(0.84,1.5,22.5), upBounds = cms.vdouble(0.9,3.0,25.0), values = cms.vdouble(0.987386524677), uncertainties = cms.vdouble(0.00432063989045,0.00432063989045)),
        cms.PSet(lowBounds = cms.vdouble(0.84,1.5,25.0), upBounds = cms.vdouble(0.9,3.0,27.5), values = cms.vdouble(0.986037313938), uncertainties = cms.vdouble(0.00208121288909,0.00208121288909)),
        cms.PSet(lowBounds = cms.vdouble(0.84,1.5,27.5), upBounds = cms.vdouble(0.9,3.0,30.0), values = cms.vdouble(0.989064216614), uncertainties = cms.vdouble(0.00131255284358,0.00131255284358)),
        cms.PSet(lowBounds = cms.vdouble(0.84,1.5,30.0), upBounds = cms.vdouble(0.9,3.0,32.5), values = cms.vdouble(0.990115821362), uncertainties = cms.vdouble(0.000987952486947,0.000987952486947)),
        cms.PSet(lowBounds = cms.vdouble(0.84,1.5,32.5), upBounds = cms.vdouble(0.9,3.0,35.0), values = cms.vdouble(0.991321563721), uncertainties = cms.vdouble(0.00064998825176,0.00064998825176)),
        cms.PSet(lowBounds = cms.vdouble(0.84,1.5,35.0), upBounds = cms.vdouble(0.9,3.0,40.0), values = cms.vdouble(0.992424309254), uncertainties = cms.vdouble(0.000392538029826,0.000392538029826)),
        cms.PSet(lowBounds = cms.vdouble(0.84,1.5,40.0), upBounds = cms.vdouble(0.9,3.0,45.0), values = cms.vdouble(0.993506371975), uncertainties = cms.vdouble(0.000357896360151,0.000357896360151)),
        cms.PSet(lowBounds = cms.vdouble(0.84,1.5,45.0), upBounds = cms.vdouble(0.9,3.0,50.0), values = cms.vdouble(0.992881655693), uncertainties = cms.vdouble(0.000667513509479,0.000667513509479)),
        cms.PSet(lowBounds = cms.vdouble(0.84,1.5,50.0), upBounds = cms.vdouble(0.9,3.0,60.0), values = cms.vdouble(0.99330842495), uncertainties = cms.vdouble(0.000823890634755,0.000823890634755)),
        cms.PSet(lowBounds = cms.vdouble(0.84,1.5,60.0), upBounds = cms.vdouble(0.9,3.0,70.0), values = cms.vdouble(0.991472125053), uncertainties = cms.vdouble(0.00186896480055,0.00186896480055)),
        cms.PSet(lowBounds = cms.vdouble(0.84,1.5,70.0), upBounds = cms.vdouble(0.9,3.0,90.0), values = cms.vdouble(0.999999046326), uncertainties = cms.vdouble(0.000123650174015,0.000123650174015)),
        cms.PSet(lowBounds = cms.vdouble(0.84,1.5,90.0), upBounds = cms.vdouble(0.9,3.0,99999999), values = cms.vdouble(0.992282807827), uncertainties = cms.vdouble(0.00316821950182,0.00316821950182)),
        cms.PSet(lowBounds = cms.vdouble(0.9,1.5,0.0), upBounds = cms.vdouble(999,3.0,22.5), values = cms.vdouble(0.99313390255), uncertainties = cms.vdouble(0.000574329725221,0.000574329725221)),
        cms.PSet(lowBounds = cms.vdouble(0.9,1.5,22.5), upBounds = cms.vdouble(999,3.0,25.0), values = cms.vdouble(0.998826920986), uncertainties = cms.vdouble(0.000196940017181,0.000196940017181)),
        cms.PSet(lowBounds = cms.vdouble(0.9,1.5,25.0), upBounds = cms.vdouble(999,3.0,27.5), values = cms.vdouble(0.998663425446), uncertainties = cms.vdouble(0.000196940017181,0.000196940017181)),
        cms.PSet(lowBounds = cms.vdouble(0.9,1.5,27.5), upBounds = cms.vdouble(999,3.0,30.0), values = cms.vdouble(0.998753978656), uncertainties = cms.vdouble(0.000196940017181,0.000196940017181)),
        cms.PSet(lowBounds = cms.vdouble(0.9,1.5,30.0), upBounds = cms.vdouble(999,3.0,32.5), values = cms.vdouble(0.998868584633), uncertainties = cms.vdouble(9.19086602694e-05,9.19086602694e-05)),
        cms.PSet(lowBounds = cms.vdouble(0.9,1.5,32.5), upBounds = cms.vdouble(999,3.0,35.0), values = cms.vdouble(0.999282956123), uncertainties = cms.vdouble(6.61051993856e-05,6.61051993856e-05)),
        cms.PSet(lowBounds = cms.vdouble(0.9,1.5,35.0), upBounds = cms.vdouble(999,3.0,40.0), values = cms.vdouble(0.999356508255), uncertainties = cms.vdouble(4.28383970185e-05,4.28383970185e-05)),
        cms.PSet(lowBounds = cms.vdouble(0.9,1.5,40.0), upBounds = cms.vdouble(999,3.0,45.0), values = cms.vdouble(0.999553442001), uncertainties = cms.vdouble(3.74810811141e-05,3.74810811141e-05)),
        cms.PSet(lowBounds = cms.vdouble(0.9,1.5,45.0), upBounds = cms.vdouble(999,3.0,50.0), values = cms.vdouble(0.999633073807), uncertainties = cms.vdouble(4.60404995528e-05,4.60404995528e-05)),
        cms.PSet(lowBounds = cms.vdouble(0.9,1.5,50.0), upBounds = cms.vdouble(999,3.0,60.0), values = cms.vdouble(0.999295592308), uncertainties = cms.vdouble(8.84144904368e-05,8.84144904368e-05)),
        cms.PSet(lowBounds = cms.vdouble(0.9,1.5,60.0), upBounds = cms.vdouble(999,3.0,70.0), values = cms.vdouble(0.999351680279), uncertainties = cms.vdouble(0.00015872505313,0.00015872505313)),
        cms.PSet(lowBounds = cms.vdouble(0.9,1.5,70.0), upBounds = cms.vdouble(999,3.0,90.0), values = cms.vdouble(0.999379456043), uncertainties = cms.vdouble(0.000199756405112,0.000199756405112)),
        cms.PSet(lowBounds = cms.vdouble(0.9,1.5,90.0), upBounds = cms.vdouble(999,3.0,99999999), values = cms.vdouble(0.99986666441), uncertainties = cms.vdouble(0.000145852311423,0.000145852311423)),

        cms.PSet(lowBounds = cms.vdouble(0.0,3.,0), upBounds = cms.vdouble(999,999,999999),     values = cms.vdouble(1.), uncertainties = cms.vdouble(1.,1.))
        )
    )


# from Arnab via Martina 10/03/2016                                                                                                                               
looseMvaBins = cms.PSet(
    variables = cms.vstring("abs(superCluster.eta)","full5x5_r9"),
    bins = cms.VPSet(
        cms.PSet( lowBounds = cms.vdouble( 0.0, 0.0  ) , upBounds = cms.vdouble( 1.5, 0.85  ) , values = cms.vdouble( 0.9999 ) , uncertainties = cms.vdouble( 0.0001 )  ) ,
        cms.PSet( lowBounds = cms.vdouble( 0.0, 0.85 ) , upBounds = cms.vdouble( 1.5, 999.0 ) , values = cms.vdouble( 1.0003 ) , uncertainties = cms.vdouble( 0.0000 )  ) ,
        cms.PSet( lowBounds = cms.vdouble( 1.5, 0.0  ) , upBounds = cms.vdouble( 6.0, 0.9   ) , values = cms.vdouble( 1.0003 ) , uncertainties = cms.vdouble( 0.0000 )  ) ,
        cms.PSet( lowBounds = cms.vdouble( 1.5, 0.9  ) , upBounds = cms.vdouble( 6.0, 999.0 ) , values = cms.vdouble( 1.0004 ) , uncertainties = cms.vdouble( 0.0000 )  ) 
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
        cms.PSet(lowBounds = cms.vdouble(0), upBounds = cms.vdouble(5), values = cms.vdouble(1.00046,0.999332), uncertainties = cms.vdouble(0.00167952,0.00167952,0.00243437,0.00243437)),
        cms.PSet(lowBounds = cms.vdouble(5), upBounds = cms.vdouble(10), values = cms.vdouble(1.01403,0.978954), uncertainties = cms.vdouble(0.00132967,0.00132967,0.00199484,0.00199484)),
        cms.PSet(lowBounds = cms.vdouble(10), upBounds = cms.vdouble(15), values = cms.vdouble(1.0031,0.99458), uncertainties = cms.vdouble(0.00127284,0.00127284,0.00222552,0.00222552)),
        cms.PSet(lowBounds = cms.vdouble(15), upBounds = cms.vdouble(20), values = cms.vdouble(0.992237,1.01713), uncertainties = cms.vdouble(0.00122729,0.00122729,0.00270787,0.00270787)),
        cms.PSet(lowBounds = cms.vdouble(20), upBounds = cms.vdouble(30), values = cms.vdouble(0.990433,1.02985), uncertainties = cms.vdouble(0.000854325,0.000854325,0.00266531,0.00266531)),
        cms.PSet(lowBounds = cms.vdouble(30), upBounds = cms.vdouble(40), values = cms.vdouble(0.988515,1.05637), uncertainties = cms.vdouble(0.000847473,0.000847473,0.00415923,0.00415923)),
        cms.PSet(lowBounds = cms.vdouble(40), upBounds = cms.vdouble(50), values = cms.vdouble(0.988526,1.07976), uncertainties = cms.vdouble(0.000864982,0.000864982,0.00601261,0.00601261)),
        cms.PSet(lowBounds = cms.vdouble(50), upBounds = cms.vdouble(60), values = cms.vdouble(0.988509,1.11643), uncertainties = cms.vdouble(0.000909363,0.000909363,0.00921419,0.00921419)),
        cms.PSet(lowBounds = cms.vdouble(60), upBounds = cms.vdouble(80), values = cms.vdouble(0.989606,1.14786), uncertainties = cms.vdouble(0.000690743,0.000690743,0.00982573,0.00982573)),
        cms.PSet(lowBounds = cms.vdouble(80), upBounds = cms.vdouble(100), values = cms.vdouble(0.991492,1.16885), uncertainties = cms.vdouble(0.000759541,0.000759541,0.0150743,0.0150743)),
        cms.PSet(lowBounds = cms.vdouble(100), upBounds = cms.vdouble(140), values = cms.vdouble(0.997022,1.07771), uncertainties = cms.vdouble(0.00066297,0.00066297,0.0173001,0.0173001)),
        cms.PSet(lowBounds = cms.vdouble(140), upBounds = cms.vdouble(200), values = cms.vdouble(0.999255,1.02942), uncertainties = cms.vdouble(0.000738493,0.000738493,0.0291629,0.0291629)),
        cms.PSet(lowBounds = cms.vdouble(200), upBounds = cms.vdouble(400), values = cms.vdouble(1.00079,0.943138), uncertainties = cms.vdouble(0.000985164,0.000985164,0.0710487,0.0710487)),
        # maximum energy beyond 7000 because of wonky DiPhotons in data
        cms.PSet(lowBounds = cms.vdouble(400 ) , upBounds = cms.vdouble(999999999 ) , values = cms.vdouble(1,1              ) , uncertainties = cms.vdouble(0.,0.,0.,0.))       
        )
    )      
 
RVBinsNvtx = cms.PSet(
    variables = cms.vstring("nVert"),
     bins = cms.VPSet(
        cms.PSet(lowBounds = cms.vdouble(-0.5), upBounds = cms.vdouble(10.5), values = cms.vdouble(1.02898,0.828452), uncertainties = cms.vdouble(0.00155068,0.00155068,0.00918045,0.00918045)),
        cms.PSet(lowBounds = cms.vdouble(10.5), upBounds = cms.vdouble(12.5), values = cms.vdouble(1.00775,0.960156), uncertainties = cms.vdouble(0.0013211,0.0013211,0.00679271,0.00679271)),
        cms.PSet(lowBounds = cms.vdouble(12.5), upBounds = cms.vdouble(14.5), values = cms.vdouble(1.00406,0.980929), uncertainties = cms.vdouble(0.00113947,0.00113947,0.00535269,0.00535269)),
        cms.PSet(lowBounds = cms.vdouble(14.5), upBounds = cms.vdouble(16.5), values = cms.vdouble(1.00159,0.992869), uncertainties = cms.vdouble(0.00109956,0.00109956,0.00493888,0.00493888)),
        cms.PSet(lowBounds = cms.vdouble(16.5), upBounds = cms.vdouble(18.5), values = cms.vdouble(0.993201,1.02899), uncertainties = cms.vdouble(0.00112887,0.00112887,0.00481407,0.00481407)),
        cms.PSet(lowBounds = cms.vdouble(18.5), upBounds = cms.vdouble(20.5), values = cms.vdouble(0.991425,1.03468), uncertainties = cms.vdouble(0.0012414,0.0012414,0.00502105,0.00502105)),
        cms.PSet(lowBounds = cms.vdouble(20.5), upBounds = cms.vdouble(22.5), values = cms.vdouble(0.989716,1.03941), uncertainties = cms.vdouble(0.00142369,0.00142369,0.00545553,0.00545553)),
        cms.PSet(lowBounds = cms.vdouble(22.5), upBounds = cms.vdouble(25.5), values = cms.vdouble(0.98674,1.04837), uncertainties = cms.vdouble(0.00147513,0.00147513,0.00538112,0.00538112)),
        cms.PSet(lowBounds = cms.vdouble(25.5), upBounds = cms.vdouble(30.5), values = cms.vdouble(0.976922,1.07893), uncertainties = cms.vdouble(0.00188024,0.00188024,0.00643049,0.00643049)),
        cms.PSet(lowBounds = cms.vdouble(30.5), upBounds = cms.vdouble(100.5), values = cms.vdouble(0.959731,1.13018), uncertainties = cms.vdouble(0.00440431,0.00440431,0.0142389,0.0142389)),
        # just in case
        cms.PSet(lowBounds = cms.vdouble(100.5 ) , upBounds = cms.vdouble(999999999 ) , values = cms.vdouble(1,1              ) , uncertainties = cms.vdouble(0.,0.,0.,0.))       
        )
    )     




# Photon categoryscale [$\times 10^{-2}$]
# central EB (eta<0.8) low r9  0.035
# central EB (eta<0.8) high r9  0.033
# intermediate EB (0.8 < eta < 1.0) low r9  0.058
# intermediate EB (0.8 < eta < 1.0) high r9  0.12
# outer EB (eta>1.0) low r9  0.22
# outer EB (eta>1.0) high r9  0.34
# EE low r9  0.22
# EE high r9  0.34
#
# Copied from Run 1 values
# https://twiki.cern.ch/twiki/bin/viewauth/CMS/HggLegacySystematicUncertainties#Upstream_material_electron_to_ph
# NB these are comparable to other scale uncertainties
materialBinsRun1 = cms.PSet(
    variables = cms.vstring("abs(superCluster.eta)","full5x5_r9"),
    bins = cms.VPSet(
        cms.PSet( lowBounds = cms.vdouble( 0.0, 0.0  ), upBounds = cms.vdouble( 0.8, 0.94  ), values = cms.vdouble( 0. ), uncertainties = cms.vdouble( 0.00035 ) ),
        cms.PSet( lowBounds = cms.vdouble( 0.0, 0.94 ), upBounds = cms.vdouble( 0.8, 999.0 ), values = cms.vdouble( 0. ), uncertainties = cms.vdouble( 0.00033 ) ),
        cms.PSet( lowBounds = cms.vdouble( 0.8, 0.0  ), upBounds = cms.vdouble( 1.0, 0.94  ), values = cms.vdouble( 0. ), uncertainties = cms.vdouble( 0.00058 ) ),
        cms.PSet( lowBounds = cms.vdouble( 0.8, 0.94 ), upBounds = cms.vdouble( 1.0, 999.0 ), values = cms.vdouble( 0. ), uncertainties = cms.vdouble( 0.0012 ) ),
        cms.PSet( lowBounds = cms.vdouble( 1.0, 0.0  ), upBounds = cms.vdouble( 1.5, 0.94  ), values = cms.vdouble( 0. ), uncertainties = cms.vdouble( 0.0022 ) ),
        cms.PSet( lowBounds = cms.vdouble( 1.0, 0.94 ), upBounds = cms.vdouble( 1.5, 999.0 ), values = cms.vdouble( 0. ), uncertainties = cms.vdouble( 0.0034 ) ),
        cms.PSet( lowBounds = cms.vdouble( 1.5, 0.0  ), upBounds = cms.vdouble( 999., 0.94  ), values = cms.vdouble( 0. ), uncertainties = cms.vdouble( 0.0022 ) ),
        cms.PSet( lowBounds = cms.vdouble( 1.5, 0.94 ), upBounds = cms.vdouble( 999., 999.0 ), values = cms.vdouble( 0. ), uncertainties = cms.vdouble( 0.0034 ) ),

        )
    )

materialBinsICHEP = cms.PSet(
    variables = cms.vstring("abs(superCluster.eta)","full5x5_r9"),
    bins = cms.VPSet(
        cms.PSet( lowBounds = cms.vdouble( 0.0, 0.0 ), upBounds = cms.vdouble( 1.5, 0.94 ), values = cms.vdouble( 0. ) , uncertainties = cms.vdouble( 0.00070 ) ),
        cms.PSet( lowBounds = cms.vdouble( 0.0, 0.94 ), upBounds = cms.vdouble( 1.5, 999. ), values = cms.vdouble( 0. ), uncertainties = cms.vdouble( 0.00036 ) ),
        cms.PSet( lowBounds = cms.vdouble( 1.5, 0.0 ), upBounds = cms.vdouble( 6.0, 0.94  ), values = cms.vdouble( 0. ), uncertainties = cms.vdouble( 0.00089 ) ),
        cms.PSet( lowBounds = cms.vdouble( 1.5, 0.9 ), upBounds = cms.vdouble( 6.0, 999. ), values = cms.vdouble( 0. ), uncertainties = cms.vdouble( 0.00170 ) )
        )
    )

# Moriond17 studies from https://indico.cern.ch/event/628619/contributions/2552389/attachments/1442921/2222173/MaterialStudy_10042017_v2.pdf
#
# All numbers for 5% material uncertainty (x10^-3)
#
# Category1:CentralBarrelandlowr9 (|eta|<1.0andr9<0.94)     0.455
# Category2:CentralBarrelandhighr9(|eta|<1.0andr9>0.94)     0.233
# Category3:OuterBarrelandlowr9 (1.0<|eta|<1.5andr9<0.94)   2.089
# Category4:OuterBarrelandhighr9 (1.0<|eta|<1.5andr9>0.94)   N/A   (taken to be equal to cat3)
# Category5:Endcapandlowr9 (|eta|>1.5andr9<0.94)            1.090
# Category6:Endcapandhighr9 (|eta|>1.5andr9>0.94)           2.377

materialBinsMoriond17 = cms.PSet(
    variables = cms.vstring("abs(superCluster.eta)","full5x5_r9"),
    bins = cms.VPSet(
        cms.PSet( lowBounds = cms.vdouble( 0.0, 0.0  ), upBounds = cms.vdouble( 1.0, 0.94  ), values = cms.vdouble( 0. ),  uncertainties = cms.vdouble( 0.000455 ) ),
        cms.PSet( lowBounds = cms.vdouble( 0.0, 0.94 ), upBounds = cms.vdouble( 1.0, 999.0 ), values = cms.vdouble( 0. ),  uncertainties = cms.vdouble( 0.000233 ) ),
        cms.PSet( lowBounds = cms.vdouble( 1.0, 0.0  ), upBounds = cms.vdouble( 1.5, 0.94  ), values = cms.vdouble( 0. ),  uncertainties = cms.vdouble( 0.002089 ) ),
        cms.PSet( lowBounds = cms.vdouble( 1.0, 0.94 ), upBounds = cms.vdouble( 1.5, 999.0 ), values = cms.vdouble( 0. ),  uncertainties = cms.vdouble( 0.002089 ) ),
        cms.PSet( lowBounds = cms.vdouble( 1.5, 0.0  ), upBounds = cms.vdouble( 999., 0.94  ), values = cms.vdouble( 0. ), uncertainties = cms.vdouble( 0.001090 ) ),
        cms.PSet( lowBounds = cms.vdouble( 1.5, 0.94 ), upBounds = cms.vdouble( 999., 999.0 ), values = cms.vdouble( 0. ), uncertainties = cms.vdouble( 0.002377 ) ),

        )
    )

emptyBins = cms.PSet(
    variables = cms.vstring("1"),
    bins = cms.VPSet()
    )

emptySigma = cms.PSet(
    firstVar = cms.vint32(),
    secondVar = cms.vint32()
)

scalesAndSmearingsPrefix = cms.string("EgammaAnalysis/ElectronTools/data/ScalesSmearings/Legacy2016_07Aug2017_FineEtaR9_v3_ele_unc")
#scalesAndSmearingsPrefixForSigmaEOverE = scalesAndSmearingsPrefix
#cms.string("EgammaAnalysis/ElectronTools/data/ScalesSmearings/Legacy2016_07Aug2017_pho_unc")

MCScaleHighR9EB = cms.PSet( PhotonMethodName = cms.string("FlashggPhotonScale"),
          MethodName = cms.string("FlashggDiPhotonFromPhoton"),
          Label = cms.string("MCScaleHighR9EB"),
          NSigmas = cms.vint32(-1,1),
          OverallRange = cms.string("full5x5_r9>0.94&&abs(superCluster.eta)<1.5"),
          BinList = photonScaleUncertBins,
          ApplyCentralValue = cms.bool(False),
          Debug = cms.untracked.bool(False)
          )

MCScaleLowR9EB = cms.PSet( PhotonMethodName = cms.string("FlashggPhotonScale"),
          MethodName = cms.string("FlashggDiPhotonFromPhoton"),
          Label = cms.string("MCScaleLowR9EB"),
          NSigmas = cms.vint32(-1,1),
          OverallRange = cms.string("full5x5_r9<0.94&&abs(superCluster.eta)<1.5"),
          BinList = photonScaleUncertBins,
          ApplyCentralValue = cms.bool(False),
          Debug = cms.untracked.bool(False)
          )

MCScaleHighR9EE = cms.PSet( PhotonMethodName = cms.string("FlashggPhotonScale"),
          MethodName = cms.string("FlashggDiPhotonFromPhoton"),
          Label = cms.string("MCScaleHighR9EE"),
          NSigmas = cms.vint32(-1,1),
          OverallRange = cms.string("full5x5_r9>0.94&&abs(superCluster.eta)>=1.5"),
          BinList = photonScaleUncertBins,
          ApplyCentralValue = cms.bool(False),
          Debug = cms.untracked.bool(False)
          )

MCScaleLowR9EE = cms.PSet( PhotonMethodName = cms.string("FlashggPhotonScale"),
          MethodName = cms.string("FlashggDiPhotonFromPhoton"),
          Label = cms.string("MCScaleLowR9EE"),
          NSigmas = cms.vint32(-1,1),
          OverallRange = cms.string("full5x5_r9<0.94&&abs(superCluster.eta)>=1.5"),
          BinList = photonScaleUncertBins,
          ApplyCentralValue = cms.bool(False),
          Debug = cms.untracked.bool(False)
          )

MaterialCentralBarrel = cms.PSet( PhotonMethodName = cms.string("FlashggPhotonScale"),
          MethodName = cms.string("FlashggDiPhotonFromPhoton"),
          Label = cms.string("MaterialCentralBarrel"),
          NSigmas = cms.vint32(-1,1),
          OverallRange = cms.string("abs(superCluster.eta)<1.0"),
          BinList = materialBinsMoriond17,
          ApplyCentralValue = cms.bool(False),
          Debug = cms.untracked.bool(False)
          )

MaterialOuterBarrel = cms.PSet( PhotonMethodName = cms.string("FlashggPhotonScale"),
          MethodName = cms.string("FlashggDiPhotonFromPhoton"),
          Label = cms.string("MaterialOuterBarrel"),
          NSigmas = cms.vint32(-1,1),
          OverallRange = cms.string("abs(superCluster.eta)>=1.0&&abs(superCluster.eta)<1.5"),
          BinList = materialBinsMoriond17,
          ApplyCentralValue = cms.bool(False),
          Debug = cms.untracked.bool(False)
          )

MaterialForward = cms.PSet( PhotonMethodName = cms.string("FlashggPhotonScale"),
          MethodName = cms.string("FlashggDiPhotonFromPhoton"),
          Label = cms.string("MaterialForward"),
          NSigmas = cms.vint32(-1,1),
          OverallRange = cms.string("abs(superCluster.eta)>=1.5"),
          BinList = materialBinsMoriond17,
          ApplyCentralValue = cms.bool(False),
          Debug = cms.untracked.bool(False)
          )

FNUFEB = cms.PSet( PhotonMethodName = cms.string("FlashggPhotonScale"),
          MethodName = cms.string("FlashggDiPhotonFromPhoton"),
          Label = cms.string("FNUFEB"),
          NSigmas = cms.vint32(-1,1),
          OverallRange = cms.string("abs(superCluster.eta)<1.5"),
          BinList = FNUFBins,
          ApplyCentralValue = cms.bool(False),
          Debug = cms.untracked.bool(False)
          )

FNUFEE = cms.PSet( PhotonMethodName = cms.string("FlashggPhotonScale"),
          MethodName = cms.string("FlashggDiPhotonFromPhoton"),
          Label = cms.string("FNUFEE"),
          NSigmas = cms.vint32(-1,1),
          OverallRange = cms.string("abs(superCluster.eta)>=1.5"),
          BinList = FNUFBins,
          ApplyCentralValue = cms.bool(False),
          Debug = cms.untracked.bool(False)
          )

ShowerShapeHighR9EB = cms.PSet( PhotonMethodName = cms.string("FlashggPhotonScale"),
          MethodName = cms.string("FlashggDiPhotonFromPhoton"),
          Label = cms.string("ShowerShapeHighR9EB"),
          NSigmas = cms.vint32(-1,1),
          OverallRange = cms.string("full5x5_r9>0.94&&abs(superCluster.eta)<1.5"),
          BinList = showerShapeBins,
          ApplyCentralValue = cms.bool(False),
          Debug = cms.untracked.bool(False)
          )

ShowerShapeHighR9EE = cms.PSet( PhotonMethodName = cms.string("FlashggPhotonScale"),
          MethodName = cms.string("FlashggDiPhotonFromPhoton"),
          Label = cms.string("ShowerShapeHighR9EE"),
          NSigmas = cms.vint32(-1,1),
          OverallRange = cms.string("full5x5_r9>0.94&&abs(superCluster.eta)>=1.5"),
          BinList = showerShapeBins,
          ApplyCentralValue = cms.bool(False),
          Debug = cms.untracked.bool(False)
          )

ShowerShapeLowR9EB = cms.PSet( PhotonMethodName = cms.string("FlashggPhotonScale"),
          MethodName = cms.string("FlashggDiPhotonFromPhoton"),
          Label = cms.string("ShowerShapeLowR9EB"),
          NSigmas = cms.vint32(-1,1),
          OverallRange = cms.string("full5x5_r9<=0.94&&abs(superCluster.eta)<1.5"),
          BinList = showerShapeBins,
          ApplyCentralValue = cms.bool(False),
          Debug = cms.untracked.bool(False)
          )

ShowerShapeLowR9EE = cms.PSet( PhotonMethodName = cms.string("FlashggPhotonScale"),
          MethodName = cms.string("FlashggDiPhotonFromPhoton"),
          Label = cms.string("ShowerShapeLowR9EE"),
          NSigmas = cms.vint32(-1,1),
          OverallRange = cms.string("full5x5_r9<=0.94&&abs(superCluster.eta)>=1.5"),
          BinList = showerShapeBins,
          ApplyCentralValue = cms.bool(False),
          Debug = cms.untracked.bool(False)
          )


MCSmearHighR9EE = cms.PSet( PhotonMethodName = cms.string("FlashggPhotonSmearConstant"),
          MethodName = cms.string("FlashggDiPhotonFromPhoton"),
          Label = cms.string("MCSmearHighR9EE"),
          NSigmas = cms.vint32(-1,1),
          OverallRange = cms.string("full5x5_r9>0.94&&abs(superCluster.eta)>=1.5"),
          BinList = photonSmearBins,
          # has to match the labels embedded in the photon object as
          # defined e.g. in flashgg/MicroAOD/python/flashggRandomizedPerPhotonDiPhotonProducer_cff.py
          #           or in flashgg/MicroAOD/python/flashggRandomizedPhotonProducer_cff.py (if at MicroAOD prod.)
          RandomLabel = cms.string("rnd_g_E"),
          Debug = cms.untracked.bool(False),
          ExaggerateShiftUp = cms.bool(False),
          ApplyCentralValue = cms.bool(True)
          )

MCSmearLowR9EE = cms.PSet( PhotonMethodName = cms.string("FlashggPhotonSmearConstant"),
          MethodName = cms.string("FlashggDiPhotonFromPhoton"),
          Label = cms.string("MCSmearLowR9EE"),
          NSigmas = cms.vint32(-1,1),
          OverallRange = cms.string("full5x5_r9<0.94&&abs(superCluster.eta)>=1.5"),
          BinList = photonSmearBins,
          # has to match the labels embedded in the photon object as
          # defined e.g. in flashgg/MicroAOD/python/flashggRandomizedPerPhotonDiPhotonProducer_cff.py
          #           or in flashgg/MicroAOD/python/flashggRandomizedPhotonProducer_cff.py (if at MicroAOD prod.)
          RandomLabel = cms.string("rnd_g_E"),
          Debug = cms.untracked.bool(False),
          ExaggerateShiftUp = cms.bool(False),
          ApplyCentralValue = cms.bool(True)
          )

MCSmearHighR9EB = cms.PSet( PhotonMethodName = cms.string("FlashggPhotonSmearConstant"),
          MethodName = cms.string("FlashggDiPhotonFromPhoton"),
          Label = cms.string("MCSmearHighR9EB"),
          NSigmas = cms.vint32(-1,1),
          OverallRange = cms.string("full5x5_r9>0.94&&abs(superCluster.eta)<1.5"),
          BinList = photonSmearBins,
          # has to match the labels embedded in the photon object as
          # defined e.g. in flashgg/MicroAOD/python/flashggRandomizedPerPhotonDiPhotonProducer_cff.py
          #           or in flashgg/MicroAOD/python/flashggRandomizedPhotonProducer_cff.py (if at MicroAOD prod.)
          RandomLabel = cms.string("rnd_g_E"),
          Debug = cms.untracked.bool(False),
          ExaggerateShiftUp = cms.bool(False),
          ApplyCentralValue = cms.bool(True)
          )

MCSmearLowR9EB = cms.PSet( PhotonMethodName = cms.string("FlashggPhotonSmearConstant"),
          MethodName = cms.string("FlashggDiPhotonFromPhoton"),
          Label = cms.string("MCSmearLowR9EB"),
          NSigmas = cms.vint32(-1,1),
          OverallRange = cms.string("full5x5_r9<=0.94&&abs(superCluster.eta)<1.5"),
          BinList = photonSmearBins,
          # has to match the labels embedded in the photon object as
          # defined e.g. in flashgg/MicroAOD/python/flashggRandomizedPerPhotonDiPhotonProducer_cff.py
          #           or in flashgg/MicroAOD/python/flashggRandomizedPhotonProducer_cff.py (if at MicroAOD prod.)
          RandomLabel = cms.string("rnd_g_E"),
          Debug = cms.untracked.bool(False),
          ExaggerateShiftUp = cms.bool(False),
          ApplyCentralValue = cms.bool(True)
          )

MvaShift = cms.PSet( PhotonMethodName = cms.string("FlashggPhotonMvaTransform"),
          MethodName = cms.string("FlashggDiPhotonFromPhoton"),
          Label = cms.string("MvaShift"),
          NSigmas = cms.vint32(-1,1),
          OverallRange = cms.string("1"),
          CorrectionFile = cms.FileInPath("flashgg/Systematics/data/SystematicsIDMVA_LegRunII_v1_2016.root"),
          BinList = mvaShiftBins,
          Debug = cms.untracked.bool(False),
          ApplyCentralValue = cms.bool(False)
          )

PreselSF = cms.PSet( PhotonMethodName = cms.string("FlashggPhotonWeight"),
          MethodName = cms.string("FlashggDiPhotonFromPhoton"),
          Label = cms.string("PreselSF"),
          NSigmas = cms.vint32(-1,1),
          OverallRange = cms.string("1"),
          BinList = preselBins,
          Debug = cms.untracked.bool(False),
          ApplyCentralValue = cms.bool(True)
          )

electronVetoSF = cms.PSet( PhotonMethodName = cms.string("FlashggPhotonWeight"),
          MethodName = cms.string("FlashggDiPhotonFromPhoton"),
          Label = cms.string("electronVetoSF"),
          NSigmas = cms.vint32(-1,1),
          OverallRange = cms.string("1"),
          BinList = electronVetoBins,
          Debug = cms.untracked.bool(False),
          ApplyCentralValue = cms.bool(True)
          )

TriggerWeight = cms.PSet( PhotonMethodName = cms.string("FlashggPhotonWeight"),
          MethodName = cms.string("FlashggDiPhotonFromPhoton"),
          Label = cms.string("TriggerWeight"),
          NSigmas = cms.vint32(-1,1),
          OverallRange = cms.string("pt<99999"),
          BinList = leadTriggerScaleBins,
          BinList2 = subleadTriggerScaleBins,
          Debug = cms.untracked.bool(False),
          ApplyCentralValue = cms.bool(True)
          )

LooseMvaSF = cms.PSet( PhotonMethodName = cms.string("FlashggPhotonWeight"),
          MethodName = cms.string("FlashggDiPhotonFromPhoton"),
          Label = cms.string("LooseMvaSF"),
          NSigmas = cms.vint32(-1,1),
          OverallRange = cms.string("1"),
          BinList = looseMvaBins,
          Debug = cms.untracked.bool(False),
          ApplyCentralValue = cms.bool(True)
          )

SigmaEOverEShift = cms.PSet( PhotonMethodName = cms.string("FlashggPhotonSigEOverEShift"),
          MethodName = cms.string("FlashggDiPhotonFromPhoton"),
          Label = cms.string("SigmaEOverEShift"),
          NSigmas = cms.vint32(-1,1),
          OverallRange = cms.string("1"),
          BinList = sigmaEOverEShiftBins,
          Debug = cms.untracked.bool(False),
          ApplyCentralValue = cms.bool(False)
          )

SigmaEOverESmearing = cms.PSet( PhotonMethodName = cms.string("FlashggPhotonSigEoverESmearing"),
          MethodName = cms.string("FlashggDiPhotonFromPhoton"),
          Label = cms.string("SigmaEOverESmearing"),
          NSigmas = cms.vint32(),
          OverallRange = cms.string("1"),
          BinList = photonSmearBins,
          Debug = cms.untracked.bool(False),
          ApplyCentralValue = cms.bool(True)
          )

SigmaEOverESmearing_EGM = cms.PSet( PhotonMethodName = cms.string("FlashggPhotonSigEoverESmearingEGMTool"),
          MethodName = cms.string("FlashggDiPhotonFromPhoton"),
          Label = cms.string("SigmaEOverESmearing"),
          CorrectionFile = scalesAndSmearingsPrefix,
          NSigmas = cms.vint32(),
          OverallRange = cms.string("1"),
          BinList = emptyBins,
          Debug = cms.untracked.bool(False),
          ExaggerateShiftUp = cms.bool(False),
          ApplyCentralValue = cms.bool(True)
          )

FracRVWeight = cms.PSet( MethodName = cms.string("FlashggDiPhotonWeightFromFracRV"),
          Label = cms.string("FracRVWeight"),
          NSigmas = cms.vint32(-1,1),
          OverallRange = cms.string("pt<99999"),
          BinList = RVBins,
          Debug = cms.untracked.bool(False),
          ApplyCentralValue = cms.bool(True)
          )

FracRVNvtxWeight = cms.PSet( MethodName = cms.string("FlashggDiPhotonWeightFromFracRV"),
          Label = cms.string("FracRVNvtxWeight"),
          NSigmas = cms.vint32(-1,1),
          OverallRange = cms.string("nVert<99999"),
          BinList = RVBinsNvtx,
          Debug = cms.untracked.bool(False),
          ApplyCentralValue = cms.bool(True)
          )

MCSmearHighR9EE_EGM = cms.PSet( PhotonMethodName = cms.string("FlashggPhotonSmearStochasticEGMTool"),
         MethodName = cms.string("FlashggDiPhotonFromPhoton2D"),
         Label = cms.string("MCSmearHighR9EE"),
         FirstParameterName = cms.string("Rho"),
         SecondParameterName = cms.string("Phi"),
         CorrectionFile = scalesAndSmearingsPrefix,
         NSigmas = cms.PSet( firstVar = cms.vint32(1,-1,0,0),
                            secondVar = cms.vint32(0,0,1,-1)),
         OverallRange = cms.string("full5x5_r9>0.94&&abs(superCluster.eta)>=1.5"),
         BinList = emptyBins,
         # has to match the labels embedded in the photon object as
         # defined e.g. in flashgg/MicroAOD/python/flashggRandomizedPerPhotonDiPhotonProducer_cff.py
         #           or in flashgg/MicroAOD/python/flashggRandomizedPhotonProducer_cff.py (if at MicroAOD prod.)
         RandomLabel = cms.string("rnd_g_E"),
         Debug = cms.untracked.bool(False),
         ExaggerateShiftUp = cms.bool(False),
         ApplyCentralValue = cms.bool(True)
         )

MCSmearLowR9EE_EGM = cms.PSet( PhotonMethodName = cms.string("FlashggPhotonSmearStochasticEGMTool"),
         MethodName = cms.string("FlashggDiPhotonFromPhoton2D"),
         Label = cms.string("MCSmearLowR9EE"),
         FirstParameterName = cms.string("Rho"),
         SecondParameterName = cms.string("Phi"),
         CorrectionFile = scalesAndSmearingsPrefix,
         NSigmas = cms.PSet( firstVar = cms.vint32(1,-1,0,0),
                            secondVar = cms.vint32(0,0,1,-1)),
         OverallRange = cms.string("full5x5_r9<=0.94&&abs(superCluster.eta)>=1.5"),
         BinList = emptyBins,
         # has to match the labels embedded in the photon object as
         # defined e.g. in flashgg/MicroAOD/python/flashggRandomizedPerPhotonDiPhotonProducer_cff.py
         #           or in flashgg/MicroAOD/python/flashggRandomizedPhotonProducer_cff.py (if at MicroAOD prod.)
         RandomLabel = cms.string("rnd_g_E"),
         Debug = cms.untracked.bool(False),
         ExaggerateShiftUp = cms.bool(False),
         ApplyCentralValue = cms.bool(True)
         )

MCSmearHighR9EB_EGM = cms.PSet( PhotonMethodName = cms.string("FlashggPhotonSmearStochasticEGMTool"),
         MethodName = cms.string("FlashggDiPhotonFromPhoton2D"),
         Label = cms.string("MCSmearHighR9EB"),
         FirstParameterName = cms.string("Rho"),
         SecondParameterName = cms.string("Phi"),
         CorrectionFile = scalesAndSmearingsPrefix,
         NSigmas = cms.PSet( firstVar = cms.vint32(1,-1,0,0),
                             secondVar = cms.vint32(0,0,1,-1)),
         OverallRange = cms.string("full5x5_r9>0.94&&abs(superCluster.eta)<1.5"),
         BinList = emptyBins,
         # has to match the labels embedded in the photon object as
         # defined e.g. in flashgg/MicroAOD/python/flashggRandomizedPerPhotonDiPhotonProducer_cff.py
         #           or in flashgg/MicroAOD/python/flashggRandomizedPhotonProducer_cff.py (if at MicroAOD prod.)
         RandomLabel = cms.string("rnd_g_E"),
         Debug = cms.untracked.bool(False),
         ExaggerateShiftUp = cms.bool(False),
         ApplyCentralValue = cms.bool(True)
         )

MCSmearLowR9EB_EGM = cms.PSet( PhotonMethodName = cms.string("FlashggPhotonSmearStochasticEGMTool"),
         MethodName = cms.string("FlashggDiPhotonFromPhoton2D"),
         Label = cms.string("MCSmearLowR9EB"),
         FirstParameterName = cms.string("Rho"),
         SecondParameterName = cms.string("Phi"),
         CorrectionFile = scalesAndSmearingsPrefix,
         NSigmas = cms.PSet( firstVar = cms.vint32(1,-1,0,0),
                            secondVar = cms.vint32(0,0,1,-1)),
         OverallRange = cms.string("full5x5_r9<=0.94&&abs(superCluster.eta)<1.5"),
         BinList = emptyBins,
         # has to match the labels embedded in the photon object as
         # defined e.g. in flashgg/MicroAOD/python/flashggRandomizedPerPhotonDiPhotonProducer_cff.py
         #           or in flashgg/MicroAOD/python/flashggRandomizedPhotonProducer_cff.py (if at MicroAOD prod.)
         RandomLabel = cms.string("rnd_g_E"),
         Debug = cms.untracked.bool(False),
         ExaggerateShiftUp = cms.bool(False),
         ApplyCentralValue = cms.bool(True)
         )

MCScaleHighR9EB_EGM = cms.PSet( PhotonMethodName = cms.string("FlashggPhotonScaleEGMTool"),
         MethodName = cms.string("FlashggDiPhotonFromPhoton"),
         Label = cms.string("MCScaleHighR9EB"),
         NSigmas = cms.vint32(-1,1),
         OverallRange = cms.string("full5x5_r9>0.94&&abs(superCluster.eta)<1.5"),
         BinList = emptyBins,
         CorrectionFile = scalesAndSmearingsPrefix,
         ApplyCentralValue = cms.bool(False),
         UncertaintyBitMask = cms.string("011"),#cms.string("110"),
         ExaggerateShiftUp = cms.bool(False),
         Debug = cms.untracked.bool(False)
         )

MCScaleLowR9EB_EGM = cms.PSet( PhotonMethodName = cms.string("FlashggPhotonScaleEGMTool"),
         MethodName = cms.string("FlashggDiPhotonFromPhoton"),
         Label = cms.string("MCScaleLowR9EB"),
         NSigmas = cms.vint32(-1,1),
         OverallRange = cms.string("full5x5_r9<0.94&&abs(superCluster.eta)<1.5"),
         BinList = emptyBins,
         CorrectionFile = scalesAndSmearingsPrefix,
         ApplyCentralValue = cms.bool(False),
         UncertaintyBitMask = cms.string("011"),#cms.string("110"),
         ExaggerateShiftUp = cms.bool(False),
         Debug = cms.untracked.bool(False)
         )

MCScaleHighR9EE_EGM = cms.PSet( PhotonMethodName = cms.string("FlashggPhotonScaleEGMTool"),
         MethodName = cms.string("FlashggDiPhotonFromPhoton"),
         Label = cms.string("MCScaleHighR9EE"),
         NSigmas = cms.vint32(-1,1),
         OverallRange = cms.string("full5x5_r9>0.94&&abs(superCluster.eta)>=1.5"),
         BinList = emptyBins,
         CorrectionFile = scalesAndSmearingsPrefix,
         ApplyCentralValue = cms.bool(False),
         UncertaintyBitMask = cms.string("011"),#cms.string("110"),
         ExaggerateShiftUp = cms.bool(False),
         Debug = cms.untracked.bool(False)
         )

MCScaleLowR9EE_EGM = cms.PSet( PhotonMethodName = cms.string("FlashggPhotonScaleEGMTool"),
         MethodName = cms.string("FlashggDiPhotonFromPhoton"),
         Label = cms.string("MCScaleLowR9EE"),
         NSigmas = cms.vint32(-1,1),
         OverallRange = cms.string("full5x5_r9<0.94&&abs(superCluster.eta)>=1.5"),
         BinList = emptyBins,
         CorrectionFile = scalesAndSmearingsPrefix,
         ApplyCentralValue = cms.bool(False),
         UncertaintyBitMask = cms.string("011"),#cms.string("110"),
         ExaggerateShiftUp = cms.bool(False),
         Debug = cms.untracked.bool(False)
         )

MCScaleGain6EB_EGM = cms.PSet( PhotonMethodName = cms.string("FlashggPhotonScaleEGMTool"),
         MethodName = cms.string("FlashggDiPhotonFromPhoton"),
         Label = cms.string("MCScaleGain6EB"),
         NSigmas = cms.vint32(-1,1),
         OverallRange = cms.string("hasSwitchToGain6&&abs(superCluster.eta)<1.5"),
         BinList = emptyBins,
         CorrectionFile = scalesAndSmearingsPrefix,
         ApplyCentralValue = cms.bool(False),
         ExaggerateShiftUp = cms.bool(False),
         UncertaintyBitMask = cms.string("100"),#cms.string("001"), # this should be a bit mask, don't know how to make it in python now
         Debug = cms.untracked.bool(False)
         )

MCScaleGain1EB_EGM = cms.PSet( PhotonMethodName = cms.string("FlashggPhotonScaleEGMTool"),
         MethodName = cms.string("FlashggDiPhotonFromPhoton"),
         Label = cms.string("MCScaleGain1EB"),
         NSigmas = cms.vint32(-1,1),
         OverallRange = cms.string("hasSwitchToGain1&&abs(superCluster.eta)<1.5"),
         BinList = emptyBins,
         CorrectionFile = scalesAndSmearingsPrefix,
         ApplyCentralValue = cms.bool(False),
         ExaggerateShiftUp = cms.bool(False),
         UncertaintyBitMask = cms.string("100"),#cms.string("001"), # this should be a bit mask, don't know how to make it in python now
         Debug = cms.untracked.bool(False)
         )

