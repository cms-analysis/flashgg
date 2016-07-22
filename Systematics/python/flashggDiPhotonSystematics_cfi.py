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

# from Arnab 15/07/2016
preselBins = cms.PSet(
    variables = cms.vstring("abs(superCluster.eta)","r9"),
    bins = cms.VPSet(
        cms.PSet( lowBounds = cms.vdouble( 0.0, 0.0 ) , upBounds = cms.vdouble( 1.5, 0.85 ) , values = cms.vdouble( 0.9960 ) , uncertainties = cms.vdouble( 0.0017 )  ) ,
        cms.PSet( lowBounds = cms.vdouble( 0.0, 0.85 ), upBounds = cms.vdouble( 1.5, 999. ) , values = cms.vdouble( 1.0052 ) , uncertainties = cms.vdouble( 0.0009 )  ) ,
        cms.PSet( lowBounds = cms.vdouble( 1.5, 0.0 ) , upBounds = cms.vdouble( 6.0, 0.9  ) , values = cms.vdouble( 0.9974 ) , uncertainties = cms.vdouble( 0.0019 )  ) ,
        cms.PSet( lowBounds = cms.vdouble( 1.5, 0.9 ) , upBounds = cms.vdouble( 6.0, 999. ) , values = cms.vdouble( 1.0028 ) , uncertainties = cms.vdouble( 0.0013 )  ) 
        )
    )

# slide 13 of https://indico.cern.ch/event/535800/contributions/2213640/attachments/1295950/1956455/201607ZmmgWith2016data_Update6p26perfb.pdf from the update with 6.26/fb
electronVetoBins = cms.PSet(
    variables = cms.vstring("abs(superCluster.eta)","r9"),
    bins = cms.VPSet(
        cms.PSet( lowBounds = cms.vdouble( 0.0, 0.00 ) , upBounds = cms.vdouble( 1.5, 0.85 ) , values = cms.vdouble( 0.9910 ) , uncertainties = cms.vdouble( 0.0049 )  ) ,
        cms.PSet( lowBounds = cms.vdouble( 0.0, 0.85 ) , upBounds = cms.vdouble( 1.5, 999. ) , values = cms.vdouble( 0.9961 ) , uncertainties = cms.vdouble( 0.0012 )  ) ,
        cms.PSet( lowBounds = cms.vdouble( 1.5, 0.00 ) , upBounds = cms.vdouble( 6.0, 0.90 ) , values = cms.vdouble( 0.9939 ) , uncertainties = cms.vdouble( 0.0171 )  ) ,
        cms.PSet( lowBounds = cms.vdouble( 1.5, 0.90 ) , upBounds = cms.vdouble( 6.0, 999. ) , values = cms.vdouble( 0.9994 ) , uncertainties = cms.vdouble( 0.0033 )  ) 
        )
    )

FNUFBins = cms.PSet(
    variables = cms.vstring("abs(superCluster.eta)","r9"),
    bins = cms.VPSet(
        cms.PSet( lowBounds = cms.vdouble( 0.0, 0.00 ) , upBounds = cms.vdouble( 1.5, 0.94 ) , values = cms.vdouble( 0. ) , uncertainties = cms.vdouble(  0.0005 )  ) ,
        cms.PSet( lowBounds = cms.vdouble( 0.0, 0.94 ) , upBounds = cms.vdouble( 1.5, 999. ) , values = cms.vdouble( 0. ) , uncertainties = cms.vdouble( -0.0005 )  ) ,
        cms.PSet( lowBounds = cms.vdouble( 1.5, 0.00 ) , upBounds = cms.vdouble( 6.0, 0.94 ) , values = cms.vdouble( 0. ) , uncertainties = cms.vdouble(  0.0005 )  ) ,
        cms.PSet( lowBounds = cms.vdouble( 1.5, 0.94 ) , upBounds = cms.vdouble( 6.0, 999. ) , values = cms.vdouble( 0. ) , uncertainties = cms.vdouble( -0.0005 )  )
        )
    )

showerShapeBins = cms.PSet(
    variables = cms.vstring("abs(superCluster.eta)","r9"),
    bins = cms.VPSet(
        cms.PSet( lowBounds = cms.vdouble( 0.0, 0.00 ) , upBounds = cms.vdouble( 1.5, 0.94 ) , values = cms.vdouble( 0. ) , uncertainties = cms.vdouble( 0.00023 )  ) ,
        cms.PSet( lowBounds = cms.vdouble( 0.0, 0.94 ) , upBounds = cms.vdouble( 1.5, 999. ) , values = cms.vdouble( 0. ) , uncertainties = cms.vdouble( 0.00024 )  ) ,
        cms.PSet( lowBounds = cms.vdouble( 1.5, 0.00 ) , upBounds = cms.vdouble( 6.0, 0.94 ) , values = cms.vdouble( 0. ) , uncertainties = cms.vdouble( 0.00064 )  ) ,
        cms.PSet( lowBounds = cms.vdouble( 1.5, 0.94 ) , upBounds = cms.vdouble( 6.0, 999. ) , values = cms.vdouble( 0. ) , uncertainties = cms.vdouble( 0.00057 )  )
        )
    )


#for 74x MC uses trigger efficiency not just scale factor since the trigger isn't in the MC

leadTriggerScaleBins = cms.PSet(
    variables = cms.vstring("r9","abs(superCluster.eta)","pt"),
    bins = cms.VPSet(
        cms.PSet(lowBounds = cms.vdouble(0.0,0,0), upBounds = cms.vdouble(0.55,1.5,33.3333), values = cms.vdouble(0.726986), uncertainties = cms.vdouble(0.00601384,0.00601384)),
        cms.PSet(lowBounds = cms.vdouble(0.0,0,33.3333), upBounds = cms.vdouble(0.55,1.5,35), values = cms.vdouble(0.842279), uncertainties = cms.vdouble(0.00649634,0.00649634)),
        cms.PSet(lowBounds = cms.vdouble(0.0,0,35), upBounds = cms.vdouble(0.55,1.5,40), values = cms.vdouble(0.850328), uncertainties = cms.vdouble(0.00323035,0.00323035)),
        cms.PSet(lowBounds = cms.vdouble(0.0,0,40), upBounds = cms.vdouble(0.55,1.5,45), values = cms.vdouble(0.864256), uncertainties = cms.vdouble(0.00299286,0.00299286)),
        cms.PSet(lowBounds = cms.vdouble(0.0,0,45), upBounds = cms.vdouble(0.55,1.5,50), values = cms.vdouble(0.880986), uncertainties = cms.vdouble(0.00392306,0.00392306)),
        cms.PSet(lowBounds = cms.vdouble(0.0,0,50), upBounds = cms.vdouble(0.55,1.5,60), values = cms.vdouble(0.884351), uncertainties = cms.vdouble(0.00612909,0.00612909)),
        cms.PSet(lowBounds = cms.vdouble(0.0,0,60), upBounds = cms.vdouble(0.55,1.5,70), values = cms.vdouble(0.895262), uncertainties = cms.vdouble(0.0166232,0.0166232)),
        cms.PSet(lowBounds = cms.vdouble(0.0,0,70), upBounds = cms.vdouble(0.55,1.5,90), values = cms.vdouble(0.919118), uncertainties = cms.vdouble(0.0272038,0.0272038)),
        cms.PSet(lowBounds = cms.vdouble(0.0,0,90), upBounds = cms.vdouble(0.55,1.5,9999999), values = cms.vdouble(0.90625), uncertainties = cms.vdouble(0.067007,0.067007)),
        cms.PSet(lowBounds = cms.vdouble(0.55,0,0), upBounds = cms.vdouble(0.85,1.5,33.3333), values = cms.vdouble(0.860014), uncertainties = cms.vdouble(0.00140032,0.00140032)),
        cms.PSet(lowBounds = cms.vdouble(0.55,0,33.3333), upBounds = cms.vdouble(0.85,1.5,35), values = cms.vdouble(0.960374), uncertainties = cms.vdouble(0.000976708,0.000976708)),
        cms.PSet(lowBounds = cms.vdouble(0.55,0,35), upBounds = cms.vdouble(0.85,1.5,40), values = cms.vdouble(0.96763), uncertainties = cms.vdouble(0.00043067,0.00043067)),
        cms.PSet(lowBounds = cms.vdouble(0.55,0,40), upBounds = cms.vdouble(0.85,1.5,45), values = cms.vdouble(0.973196), uncertainties = cms.vdouble(0.000343886,0.000343886)),
        cms.PSet(lowBounds = cms.vdouble(0.55,0,45), upBounds = cms.vdouble(0.85,1.5,50), values = cms.vdouble(0.975702), uncertainties = cms.vdouble(0.000416642,0.000416642)),
        cms.PSet(lowBounds = cms.vdouble(0.55,0,50), upBounds = cms.vdouble(0.85,1.5,60), values = cms.vdouble(0.977872), uncertainties = cms.vdouble(0.000577909,0.000577909)),
        cms.PSet(lowBounds = cms.vdouble(0.55,0,60), upBounds = cms.vdouble(0.85,1.5,70), values = cms.vdouble(0.980816), uncertainties = cms.vdouble(0.001329,0.001329)),
        cms.PSet(lowBounds = cms.vdouble(0.55,0,70), upBounds = cms.vdouble(0.85,1.5,90), values = cms.vdouble(0.985045), uncertainties = cms.vdouble(0.00166751,0.00166751)),
        cms.PSet(lowBounds = cms.vdouble(0.55,0,90), upBounds = cms.vdouble(0.85,1.5,9999999), values = cms.vdouble(0.988918), uncertainties = cms.vdouble(0.00221308,0.00221308)),
        cms.PSet(lowBounds = cms.vdouble(0.85,0,0), upBounds = cms.vdouble(999,1.5,33.3333), values = cms.vdouble(0.886498), uncertainties = cms.vdouble(0.000995863,0.000995863)),
        cms.PSet(lowBounds = cms.vdouble(0.85,0,33.3333), upBounds = cms.vdouble(999,1.5,35), values = cms.vdouble(0.969207), uncertainties = cms.vdouble(0.000662469,0.000662469)),
        cms.PSet(lowBounds = cms.vdouble(0.85,0,35), upBounds = cms.vdouble(999,1.5,40), values = cms.vdouble(0.97839), uncertainties = cms.vdouble(0.000261818,0.000261818)),
        cms.PSet(lowBounds = cms.vdouble(0.85,0,40), upBounds = cms.vdouble(999,1.5,45), values = cms.vdouble(0.982035), uncertainties = cms.vdouble(0.000198525,0.000198525)),
        cms.PSet(lowBounds = cms.vdouble(0.85,0,45), upBounds = cms.vdouble(999,1.5,50), values = cms.vdouble(0.984078), uncertainties = cms.vdouble(0.000226408,0.000226408)),
        cms.PSet(lowBounds = cms.vdouble(0.85,0,50), upBounds = cms.vdouble(999,1.5,60), values = cms.vdouble(0.984446), uncertainties = cms.vdouble(0.000306773,0.000306773)),
        cms.PSet(lowBounds = cms.vdouble(0.85,0,60), upBounds = cms.vdouble(999,1.5,70), values = cms.vdouble(0.983138), uncertainties = cms.vdouble(0.000713391,0.000713391)),
        cms.PSet(lowBounds = cms.vdouble(0.85,0,70), upBounds = cms.vdouble(999,1.5,90), values = cms.vdouble(0.987782), uncertainties = cms.vdouble(0.000779483,0.000779483)),
        cms.PSet(lowBounds = cms.vdouble(0.85,0,90), upBounds = cms.vdouble(999,1.5,9999999), values = cms.vdouble(0.987671), uncertainties = cms.vdouble(0.000873464,0.000873464)),
        
        cms.PSet(lowBounds = cms.vdouble(0.0,1.5,0), upBounds = cms.vdouble(0.85,3.0,33.3333), values = cms.vdouble(0.624174), uncertainties = cms.vdouble(0.00678721,0.00678721)),
        cms.PSet(lowBounds = cms.vdouble(0.0,1.5,33.3333), upBounds = cms.vdouble(0.85,3.0,35), values = cms.vdouble(0.777935), uncertainties = cms.vdouble(0.00716134,0.00716134)),
        cms.PSet(lowBounds = cms.vdouble(0.0,1.5,35), upBounds = cms.vdouble(0.85,3.0,40), values = cms.vdouble(0.808946), uncertainties = cms.vdouble(0.00339668,0.00339668)),
        cms.PSet(lowBounds = cms.vdouble(0.0,1.5,40), upBounds = cms.vdouble(0.85,3.0,45), values = cms.vdouble(0.82709), uncertainties = cms.vdouble(0.00316103,0.00316103)),
        cms.PSet(lowBounds = cms.vdouble(0.0,1.5,45), upBounds = cms.vdouble(0.85,3.0,50), values = cms.vdouble(0.833188), uncertainties = cms.vdouble(0.00424665,0.00424665)),
        cms.PSet(lowBounds = cms.vdouble(0.0,1.5,50), upBounds = cms.vdouble(0.85,3.0,60), values = cms.vdouble(0.849137), uncertainties = cms.vdouble(0.00581788,0.00581788)),
        cms.PSet(lowBounds = cms.vdouble(0.0,1.5,60), upBounds = cms.vdouble(0.85,3.0,70), values = cms.vdouble(0.877707), uncertainties = cms.vdouble(0.0123943,0.0123943)),
        cms.PSet(lowBounds = cms.vdouble(0.0,1.5,70), upBounds = cms.vdouble(0.85,3.0,90), values = cms.vdouble(0.874725), uncertainties = cms.vdouble(0.0166999,0.0166999)),
        cms.PSet(lowBounds = cms.vdouble(0.0,1.5,90), upBounds = cms.vdouble(0.85,3.0,9999999), values = cms.vdouble(0.901235), uncertainties = cms.vdouble(0.0213025,0.0213025)),
        cms.PSet(lowBounds = cms.vdouble(0.85,1.5,0), upBounds = cms.vdouble(0.9,3.0,33.3333), values = cms.vdouble(0.731227), uncertainties = cms.vdouble(0.00473286,0.00473286)),
        cms.PSet(lowBounds = cms.vdouble(0.85,1.5,33.3333), upBounds = cms.vdouble(0.9,3.0,35), values = cms.vdouble(0.951694), uncertainties = cms.vdouble(0.00292958,0.00292958)),
        cms.PSet(lowBounds = cms.vdouble(0.85,1.5,35), upBounds = cms.vdouble(0.9,3.0,40), values = cms.vdouble(0.982342), uncertainties = cms.vdouble(0.000883194,0.000883194)),
        cms.PSet(lowBounds = cms.vdouble(0.85,1.5,40), upBounds = cms.vdouble(0.9,3.0,45), values = cms.vdouble(0.986406), uncertainties = cms.vdouble(0.000737782,0.000737782)),
        cms.PSet(lowBounds = cms.vdouble(0.85,1.5,45), upBounds = cms.vdouble(0.9,3.0,50), values = cms.vdouble(0.988268), uncertainties = cms.vdouble(0.000937303,0.000937303)),
        cms.PSet(lowBounds = cms.vdouble(0.85,1.5,50), upBounds = cms.vdouble(0.9,3.0,60), values = cms.vdouble(0.988268), uncertainties = cms.vdouble(0.00135063,0.00135063)),
        cms.PSet(lowBounds = cms.vdouble(0.85,1.5,60), upBounds = cms.vdouble(0.9,3.0,70), values = cms.vdouble(0.989789), uncertainties = cms.vdouble(0.0029909,0.0029909)),
        cms.PSet(lowBounds = cms.vdouble(0.85,1.5,70), upBounds = cms.vdouble(0.9,3.0,90), values = cms.vdouble(0.993471), uncertainties = cms.vdouble(0.00324819,0.00324819)),
        cms.PSet(lowBounds = cms.vdouble(0.85,1.5,90), upBounds = cms.vdouble(0.9,3.0,9999999), values = cms.vdouble(0.994801), uncertainties = cms.vdouble(0.0039475,0.0039475)),
        cms.PSet(lowBounds = cms.vdouble(0.9,1.5,0), upBounds = cms.vdouble(999,3.0,33.3333), values = cms.vdouble(0.645111), uncertainties = cms.vdouble(0.00242429,0.00242429)),
        cms.PSet(lowBounds = cms.vdouble(0.9,1.5,33.3333), upBounds = cms.vdouble(999,3.0,35), values = cms.vdouble(0.93541), uncertainties = cms.vdouble(0.00155803,0.00155803)),
        cms.PSet(lowBounds = cms.vdouble(0.9,1.5,35), upBounds = cms.vdouble(999,3.0,40), values = cms.vdouble(0.980371), uncertainties = cms.vdouble(0.000447182,0.000447182)),
        cms.PSet(lowBounds = cms.vdouble(0.9,1.5,40), upBounds = cms.vdouble(999,3.0,45), values = cms.vdouble(0.986907), uncertainties = cms.vdouble(0.000352489,0.000352489)),
        cms.PSet(lowBounds = cms.vdouble(0.9,1.5,45), upBounds = cms.vdouble(999,3.0,50), values = cms.vdouble(0.988021), uncertainties = cms.vdouble(0.000451752,0.000451752)),
        cms.PSet(lowBounds = cms.vdouble(0.9,1.5,50), upBounds = cms.vdouble(999,3.0,60), values = cms.vdouble(0.988861), uncertainties = cms.vdouble(0.00061417,0.00061417)),
        cms.PSet(lowBounds = cms.vdouble(0.9,1.5,60), upBounds = cms.vdouble(999,3.0,70), values = cms.vdouble(0.992071), uncertainties = cms.vdouble(0.00116734,0.00116734)),
        cms.PSet(lowBounds = cms.vdouble(0.9,1.5,70), upBounds = cms.vdouble(999,3.0,90), values = cms.vdouble(0.994656), uncertainties = cms.vdouble(0.00120893,0.00120893)),
        cms.PSet(lowBounds = cms.vdouble(0.9,1.5,90), upBounds = cms.vdouble(999,3.0,9999999), values = cms.vdouble(0.993131), uncertainties = cms.vdouble(0.00155273,0.00155273)),
        cms.PSet(lowBounds = cms.vdouble(0.0,3.,0), upBounds = cms.vdouble(999,999,999999), values = cms.vdouble(1.), uncertainties = cms.vdouble(1.,1.))
        )
    )


subleadTriggerScaleBins = cms.PSet(
    variables = cms.vstring("r9","abs(superCluster.eta)","pt"),
    bins = cms.VPSet(
        
        cms.PSet(lowBounds = cms.vdouble(0.0,0,0), upBounds = cms.vdouble(0.55,1.5,22.5), values = cms.vdouble(0.818911), uncertainties = cms.vdouble(0.00955535,0.00955535)),
        cms.PSet(lowBounds = cms.vdouble(0.0,0,22.5), upBounds = cms.vdouble(0.55,1.5,25), values = cms.vdouble(0.838894), uncertainties = cms.vdouble(0.00796446,0.00796446)),
        cms.PSet(lowBounds = cms.vdouble(0.0,0,25), upBounds = cms.vdouble(0.55,1.5,27.5), values = cms.vdouble(0.848657), uncertainties = cms.vdouble(0.00719885,0.00719885)),
        cms.PSet(lowBounds = cms.vdouble(0.0,0,27.5), upBounds = cms.vdouble(0.55,1.5,30), values = cms.vdouble(0.855251), uncertainties = cms.vdouble(0.006363,0.006363)),
        cms.PSet(lowBounds = cms.vdouble(0.0,0,30), upBounds = cms.vdouble(0.55,1.5,33.3333), values = cms.vdouble(0.86449), uncertainties = cms.vdouble(0.00464777,0.00464777)),
        cms.PSet(lowBounds = cms.vdouble(0.0,0,33.3333), upBounds = cms.vdouble(0.55,1.5,35), values = cms.vdouble(0.870404), uncertainties = cms.vdouble(0.00601391,0.00601391)),
        cms.PSet(lowBounds = cms.vdouble(0.0,0,35), upBounds = cms.vdouble(0.55,1.5,40), values = cms.vdouble(0.873688), uncertainties = cms.vdouble(0.0030182,0.0030182)),
        cms.PSet(lowBounds = cms.vdouble(0.0,0,40), upBounds = cms.vdouble(0.55,1.5,45), values = cms.vdouble(0.888051), uncertainties = cms.vdouble(0.0027635,0.0027635)),
        cms.PSet(lowBounds = cms.vdouble(0.0,0,45), upBounds = cms.vdouble(0.55,1.5,50), values = cms.vdouble(0.901702), uncertainties = cms.vdouble(0.00362072,0.00362072)),
        cms.PSet(lowBounds = cms.vdouble(0.0,0,50), upBounds = cms.vdouble(0.55,1.5,60), values = cms.vdouble(0.906745), uncertainties = cms.vdouble(0.00559685,0.00559685)),
        cms.PSet(lowBounds = cms.vdouble(0.0,0,60), upBounds = cms.vdouble(0.55,1.5,70), values = cms.vdouble(0.915), uncertainties = cms.vdouble(0.0152772,0.0152772)),
        cms.PSet(lowBounds = cms.vdouble(0.0,0,70), upBounds = cms.vdouble(0.55,1.5,90), values = cms.vdouble(0.926471), uncertainties = cms.vdouble(0.0262131,0.0262131)),
        cms.PSet(lowBounds = cms.vdouble(0.0,0,90), upBounds = cms.vdouble(0.55,1.5,9999999), values = cms.vdouble(0.9375), uncertainties = cms.vdouble(0.0586506,0.0586506)),
        cms.PSet(lowBounds = cms.vdouble(0.55,0,0), upBounds = cms.vdouble(0.85,1.5,22.5), values = cms.vdouble(0.976751), uncertainties = cms.vdouble(0.00129344,0.00129344)),
        cms.PSet(lowBounds = cms.vdouble(0.55,0,22.5), upBounds = cms.vdouble(0.85,1.5,25), values = cms.vdouble(0.985276), uncertainties = cms.vdouble(0.000890231,0.000890231)),
        cms.PSet(lowBounds = cms.vdouble(0.55,0,25), upBounds = cms.vdouble(0.85,1.5,27.5), values = cms.vdouble(0.986385), uncertainties = cms.vdouble(0.00074319,0.00074319)),
        cms.PSet(lowBounds = cms.vdouble(0.55,0,27.5), upBounds = cms.vdouble(0.85,1.5,30), values = cms.vdouble(0.99059), uncertainties = cms.vdouble(0.000538723,0.000538723)),
        cms.PSet(lowBounds = cms.vdouble(0.55,0,30), upBounds = cms.vdouble(0.85,1.5,33.3333), values = cms.vdouble(0.993015), uncertainties = cms.vdouble(0.000342906,0.000342906)),
        cms.PSet(lowBounds = cms.vdouble(0.55,0,33.3333), upBounds = cms.vdouble(0.85,1.5,35), values = cms.vdouble(0.994536), uncertainties = cms.vdouble(0.000377616,0.000377616)),
        cms.PSet(lowBounds = cms.vdouble(0.55,0,35), upBounds = cms.vdouble(0.85,1.5,40), values = cms.vdouble(0.995255), uncertainties = cms.vdouble(0.000169365,0.000169365)),
        cms.PSet(lowBounds = cms.vdouble(0.55,0,40), upBounds = cms.vdouble(0.85,1.5,45), values = cms.vdouble(0.995633), uncertainties = cms.vdouble(0.000142003,0.000142003)),
        cms.PSet(lowBounds = cms.vdouble(0.55,0,45), upBounds = cms.vdouble(0.85,1.5,50), values = cms.vdouble(0.99514), uncertainties = cms.vdouble(0.000190547,0.000190547)),
        cms.PSet(lowBounds = cms.vdouble(0.55,0,50), upBounds = cms.vdouble(0.85,1.5,60), values = cms.vdouble(0.994476), uncertainties = cms.vdouble(0.000295527,0.000295527)),
        cms.PSet(lowBounds = cms.vdouble(0.55,0,60), upBounds = cms.vdouble(0.85,1.5,70), values = cms.vdouble(0.993642), uncertainties = cms.vdouble(0.000790334,0.000790334)),
        cms.PSet(lowBounds = cms.vdouble(0.55,0,70), upBounds = cms.vdouble(0.85,1.5,90), values = cms.vdouble(0.992077), uncertainties = cms.vdouble(0.00124376,0.00124376)),
        cms.PSet(lowBounds = cms.vdouble(0.55,0,90), upBounds = cms.vdouble(0.85,1.5,9999999), values = cms.vdouble(0.992953), uncertainties = cms.vdouble(0.0018121,0.0018121)),
        cms.PSet(lowBounds = cms.vdouble(0.85,0,0), upBounds = cms.vdouble(999,1.5,22.5), values = cms.vdouble(0.980424), uncertainties = cms.vdouble(0.000946161,0.000946161)),
        cms.PSet(lowBounds = cms.vdouble(0.85,0,22.5), upBounds = cms.vdouble(999,1.5,25), values = cms.vdouble(0.987592), uncertainties = cms.vdouble(0.000657883,0.000657883)),
        cms.PSet(lowBounds = cms.vdouble(0.85,0,25), upBounds = cms.vdouble(999,1.5,27.5), values = cms.vdouble(0.992521), uncertainties = cms.vdouble(0.00044671,0.00044671)),
        cms.PSet(lowBounds = cms.vdouble(0.85,0,27.5), upBounds = cms.vdouble(999,1.5,30), values = cms.vdouble(0.994337), uncertainties = cms.vdouble(0.000335638,0.000335638)),
        cms.PSet(lowBounds = cms.vdouble(0.85,0,30), upBounds = cms.vdouble(999,1.5,33.3333), values = cms.vdouble(0.996452), uncertainties = cms.vdouble(0.000191078,0.000191078)),
        cms.PSet(lowBounds = cms.vdouble(0.85,0,33.3333), upBounds = cms.vdouble(999,1.5,35), values = cms.vdouble(0.997842), uncertainties = cms.vdouble(0.000183685,0.000183685)),
        cms.PSet(lowBounds = cms.vdouble(0.85,0,35), upBounds = cms.vdouble(999,1.5,40), values = cms.vdouble(0.998464), uncertainties = cms.vdouble(7.18406e-05,7.18406e-05)),
        cms.PSet(lowBounds = cms.vdouble(0.85,0,40), upBounds = cms.vdouble(999,1.5,45), values = cms.vdouble(0.999023), uncertainties = cms.vdouble(4.76118e-05,4.76118e-05)),
        cms.PSet(lowBounds = cms.vdouble(0.85,0,45), upBounds = cms.vdouble(999,1.5,50), values = cms.vdouble(0.999345), uncertainties = cms.vdouble(4.76336e-05,4.76336e-05)),
        cms.PSet(lowBounds = cms.vdouble(0.85,0,50), upBounds = cms.vdouble(999,1.5,60), values = cms.vdouble(0.999337), uncertainties = cms.vdouble(6.62963e-05,6.62963e-05)),
        cms.PSet(lowBounds = cms.vdouble(0.85,0,60), upBounds = cms.vdouble(999,1.5,70), values = cms.vdouble(0.999241), uncertainties = cms.vdouble(0.000164655,0.000164655)),
        cms.PSet(lowBounds = cms.vdouble(0.85,0,70), upBounds = cms.vdouble(999,1.5,90), values = cms.vdouble(0.999343), uncertainties = cms.vdouble(0.00020097,0.00020097)),
        cms.PSet(lowBounds = cms.vdouble(0.85,0,90), upBounds = cms.vdouble(999,1.5,9999999), values = cms.vdouble(0.99942), uncertainties = cms.vdouble(0.000214904,0.000214904)),
        
        cms.PSet(lowBounds = cms.vdouble(0.0,1.5,0), upBounds = cms.vdouble(0.85,3.0,22.5), values = cms.vdouble(0.752036), uncertainties = cms.vdouble(0.0135112,0.0135112)),
        cms.PSet(lowBounds = cms.vdouble(0.0,1.5,22.5), upBounds = cms.vdouble(0.85,3.0,25), values = cms.vdouble(0.754183), uncertainties = cms.vdouble(0.0113026,0.0113026)),
        cms.PSet(lowBounds = cms.vdouble(0.0,1.5,25), upBounds = cms.vdouble(0.85,3.0,27.5), values = cms.vdouble(0.774454), uncertainties = cms.vdouble(0.00939415,0.00939415)),
        cms.PSet(lowBounds = cms.vdouble(0.0,1.5,27.5), upBounds = cms.vdouble(0.85,3.0,30), values = cms.vdouble(0.78478), uncertainties = cms.vdouble(0.00784043,0.00784043)),
        cms.PSet(lowBounds = cms.vdouble(0.0,1.5,30), upBounds = cms.vdouble(0.85,3.0,33.3333), values = cms.vdouble(0.795558), uncertainties = cms.vdouble(0.00568224,0.00568224)),
        cms.PSet(lowBounds = cms.vdouble(0.0,1.5,33.3333), upBounds = cms.vdouble(0.85,3.0,35), values = cms.vdouble(0.806809), uncertainties = cms.vdouble(0.00682827,0.00682827)),
        cms.PSet(lowBounds = cms.vdouble(0.0,1.5,35), upBounds = cms.vdouble(0.85,3.0,40), values = cms.vdouble(0.818228), uncertainties = cms.vdouble(0.00334391,0.00334391)),
        cms.PSet(lowBounds = cms.vdouble(0.0,1.5,40), upBounds = cms.vdouble(0.85,3.0,45), values = cms.vdouble(0.831468), uncertainties = cms.vdouble(0.00314421,0.00314421)),
        cms.PSet(lowBounds = cms.vdouble(0.0,1.5,45), upBounds = cms.vdouble(0.85,3.0,50), values = cms.vdouble(0.838005), uncertainties = cms.vdouble(0.00422638,0.00422638)),
        cms.PSet(lowBounds = cms.vdouble(0.0,1.5,50), upBounds = cms.vdouble(0.85,3.0,60), values = cms.vdouble(0.852049), uncertainties = cms.vdouble(0.00580433,0.00580433)),
        cms.PSet(lowBounds = cms.vdouble(0.0,1.5,60), upBounds = cms.vdouble(0.85,3.0,70), values = cms.vdouble(0.878866), uncertainties = cms.vdouble(0.0124214,0.0124214)),
        cms.PSet(lowBounds = cms.vdouble(0.0,1.5,70), upBounds = cms.vdouble(0.85,3.0,90), values = cms.vdouble(0.871965), uncertainties = cms.vdouble(0.016885,0.016885)),
        cms.PSet(lowBounds = cms.vdouble(0.0,1.5,90), upBounds = cms.vdouble(0.85,3.0,9999999), values = cms.vdouble(0.888889), uncertainties = cms.vdouble(0.0223226,0.0223226)),
        cms.PSet(lowBounds = cms.vdouble(0.85,1.5,0), upBounds = cms.vdouble(0.9,3.0,22.5), values = cms.vdouble(0.964054), uncertainties = cms.vdouble(0.00443559,0.00443559)),
        cms.PSet(lowBounds = cms.vdouble(0.85,1.5,22.5), upBounds = cms.vdouble(0.9,3.0,25), values = cms.vdouble(0.993039), uncertainties = cms.vdouble(0.00184439,0.00184439)),
        cms.PSet(lowBounds = cms.vdouble(0.85,1.5,25), upBounds = cms.vdouble(0.9,3.0,27.5), values = cms.vdouble(0.990476), uncertainties = cms.vdouble(0.00177831,0.00177831)),
        cms.PSet(lowBounds = cms.vdouble(0.85,1.5,27.5), upBounds = cms.vdouble(0.9,3.0,30), values = cms.vdouble(0.989557), uncertainties = cms.vdouble(0.00158399,0.00158399)),
        cms.PSet(lowBounds = cms.vdouble(0.85,1.5,30), upBounds = cms.vdouble(0.9,3.0,33.3333), values = cms.vdouble(0.994032), uncertainties = cms.vdouble(0.000870729,0.000870729)),
        cms.PSet(lowBounds = cms.vdouble(0.85,1.5,33.3333), upBounds = cms.vdouble(0.9,3.0,35), values = cms.vdouble(0.994762), uncertainties = cms.vdouble(0.00104905,0.00104905)),
        cms.PSet(lowBounds = cms.vdouble(0.85,1.5,35), upBounds = cms.vdouble(0.9,3.0,40), values = cms.vdouble(0.995616), uncertainties = cms.vdouble(0.000455051,0.000455051)),
        cms.PSet(lowBounds = cms.vdouble(0.85,1.5,40), upBounds = cms.vdouble(0.9,3.0,45), values = cms.vdouble(0.995538), uncertainties = cms.vdouble(0.000435225,0.000435225)),
        cms.PSet(lowBounds = cms.vdouble(0.85,1.5,45), upBounds = cms.vdouble(0.9,3.0,50), values = cms.vdouble(0.994855), uncertainties = cms.vdouble(0.000639919,0.000639919)),
        cms.PSet(lowBounds = cms.vdouble(0.85,1.5,50), upBounds = cms.vdouble(0.9,3.0,60), values = cms.vdouble(0.994907), uncertainties = cms.vdouble(0.000923962,0.000923962)),
        cms.PSet(lowBounds = cms.vdouble(0.85,1.5,60), upBounds = cms.vdouble(0.9,3.0,70), values = cms.vdouble(0.99169), uncertainties = cms.vdouble(0.00276347,0.00276347)),
        cms.PSet(lowBounds = cms.vdouble(0.85,1.5,70), upBounds = cms.vdouble(0.9,3.0,90), values = cms.vdouble(0.994493), uncertainties = cms.vdouble(0.00305641,0.00305641)),
        cms.PSet(lowBounds = cms.vdouble(0.85,1.5,90), upBounds = cms.vdouble(0.9,3.0,9999999), values = cms.vdouble(0.993043), uncertainties = cms.vdouble(0.00441637,0.00441637)),
        cms.PSet(lowBounds = cms.vdouble(0.9,1.5,0), upBounds = cms.vdouble(999,3.0,22.5), values = cms.vdouble(0.964424), uncertainties = cms.vdouble(0.0019852,0.0019852)),
        cms.PSet(lowBounds = cms.vdouble(0.9,1.5,22.5), upBounds = cms.vdouble(999,3.0,25), values = cms.vdouble(0.994833), uncertainties = cms.vdouble(0.000688953,0.000688953)),
        cms.PSet(lowBounds = cms.vdouble(0.9,1.5,25), upBounds = cms.vdouble(999,3.0,27.5), values = cms.vdouble(0.996479), uncertainties = cms.vdouble(0.000499726,0.000499726)),
        cms.PSet(lowBounds = cms.vdouble(0.9,1.5,27.5), upBounds = cms.vdouble(999,3.0,30), values = cms.vdouble(0.997557), uncertainties = cms.vdouble(0.000364297,0.000364297)),
        cms.PSet(lowBounds = cms.vdouble(0.9,1.5,30), upBounds = cms.vdouble(999,3.0,33.3333), values = cms.vdouble(0.998083), uncertainties = cms.vdouble(0.00023374,0.00023374)),
        cms.PSet(lowBounds = cms.vdouble(0.9,1.5,33.3333), upBounds = cms.vdouble(999,3.0,35), values = cms.vdouble(0.998832), uncertainties = cms.vdouble(0.000234398,0.000234398)),
        cms.PSet(lowBounds = cms.vdouble(0.9,1.5,35), upBounds = cms.vdouble(999,3.0,40), values = cms.vdouble(0.998948), uncertainties = cms.vdouble(0.000108853,0.000108853)),
        cms.PSet(lowBounds = cms.vdouble(0.9,1.5,40), upBounds = cms.vdouble(999,3.0,45), values = cms.vdouble(0.999381), uncertainties = cms.vdouble(8.13489e-05,8.13489e-05)),
        cms.PSet(lowBounds = cms.vdouble(0.9,1.5,45), upBounds = cms.vdouble(999,3.0,50), values = cms.vdouble(0.999283), uncertainties = cms.vdouble(0.000118433,0.000118433)),
        cms.PSet(lowBounds = cms.vdouble(0.9,1.5,50), upBounds = cms.vdouble(999,3.0,60), values = cms.vdouble(0.999479), uncertainties = cms.vdouble(0.000147922,0.000147922)),
        cms.PSet(lowBounds = cms.vdouble(0.9,1.5,60), upBounds = cms.vdouble(999,3.0,70), values = cms.vdouble(0.99909), uncertainties = cms.vdouble(0.000454104,0.000454104)),
        cms.PSet(lowBounds = cms.vdouble(0.9,1.5,70), upBounds = cms.vdouble(999,3.0,90), values = cms.vdouble(0.998653), uncertainties = cms.vdouble(0.000671823,0.000671823)),
        cms.PSet(lowBounds = cms.vdouble(0.9,1.5,90), upBounds = cms.vdouble(999,3.0,9999999), values = cms.vdouble(0.998557), uncertainties = cms.vdouble(0.000802797,0.000802797)),
        cms.PSet(lowBounds = cms.vdouble(0.0,3.,0), upBounds = cms.vdouble(999,999,999999), values = cms.vdouble(1.), uncertainties = cms.vdouble(1.,1.))
        )
    )


# AN version 5
# N.B. boundary changed accordingly to the preselection
looseMvaBins = cms.PSet(
    variables = cms.vstring("abs(superCluster.eta)","r9"),
    bins = cms.VPSet(
        cms.PSet( lowBounds = cms.vdouble( 0.0, 0.0  ) , upBounds = cms.vdouble( 1.5, 0.85  ) , values = cms.vdouble( 0.9925 ) , uncertainties = cms.vdouble( 0.0017 )  ) ,
        cms.PSet( lowBounds = cms.vdouble( 0.0, 0.85 ) , upBounds = cms.vdouble( 1.5, 999.0 ) , values = cms.vdouble( 0.9989 ) , uncertainties = cms.vdouble( 0.0007 )  ) ,
        cms.PSet( lowBounds = cms.vdouble( 1.5, 0.0  ) , upBounds = cms.vdouble( 6.0, 0.9   ) , values = cms.vdouble( 0.9884 ) , uncertainties = cms.vdouble( 0.0028 )  ) ,
        cms.PSet( lowBounds = cms.vdouble( 1.5, 0.9  ) , upBounds = cms.vdouble( 6.0, 999.0 ) , values = cms.vdouble( 0.9991 ) , uncertainties = cms.vdouble( 0.0010 )  ) 
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
        cms.PSet(lowBounds = cms.vdouble(0), upBounds = cms.vdouble(5), values = cms.vdouble(0.994404,1.0096), uncertainties = cms.vdouble(0.00376503,0.00376503,0.00646123,0.00646123)),
        cms.PSet(lowBounds = cms.vdouble(5), upBounds = cms.vdouble(10), values = cms.vdouble(0.997896,1.00378), uncertainties = cms.vdouble(0.00292413,0.00292413,0.00524691,0.00524691)),
        cms.PSet(lowBounds = cms.vdouble(10), upBounds = cms.vdouble(15), values = cms.vdouble(0.99532,1.00982), uncertainties = cms.vdouble(0.00275737,0.00275737,0.00578493,0.00578493)),
        cms.PSet(lowBounds = cms.vdouble(15), upBounds = cms.vdouble(20), values = cms.vdouble(0.983516,1.04232), uncertainties = cms.vdouble(0.00258585,0.00258585,0.00663929,0.00663929)),
        cms.PSet(lowBounds = cms.vdouble(20), upBounds = cms.vdouble(30), values = cms.vdouble(0.976449,1.08694), uncertainties = cms.vdouble(0.00174309,0.00174309,0.00643469,0.00643469)),
        cms.PSet(lowBounds = cms.vdouble(30), upBounds = cms.vdouble(40), values = cms.vdouble(0.979645,1.11879), uncertainties = cms.vdouble(0.00173336,0.00173336,0.0101156,0.0101156)),
        cms.PSet(lowBounds = cms.vdouble(40), upBounds = cms.vdouble(50), values = cms.vdouble(0.985952,1.12079), uncertainties = cms.vdouble(0.00182411,0.00182411,0.0156836,0.0156836)),
        cms.PSet(lowBounds = cms.vdouble(50), upBounds = cms.vdouble(60), values = cms.vdouble(0.989872,1.11928), uncertainties = cms.vdouble(0.00196615,0.00196615,0.0231568,0.0231568)),
        cms.PSet(lowBounds = cms.vdouble(60), upBounds = cms.vdouble(80), values = cms.vdouble(0.98607,1.23569), uncertainties = cms.vdouble(0.00146497,0.00146497,0.0247859,0.0247859)),
        cms.PSet(lowBounds = cms.vdouble(80), upBounds = cms.vdouble(100), values = cms.vdouble(0.99251,1.17482), uncertainties = cms.vdouble(0.00169638,0.00169638,0.0395923,0.0395923)),
        cms.PSet(lowBounds = cms.vdouble(100), upBounds = cms.vdouble(140), values = cms.vdouble(0.996121,1.12922), uncertainties = cms.vdouble(0.00144462,0.00144462,0.0481288,0.0481288)),
        cms.PSet(lowBounds = cms.vdouble(140), upBounds = cms.vdouble(200), values = cms.vdouble(0.999642,1.0155), uncertainties = cms.vdouble(0.00171889,0.00171889,0.0745073,0.0745073)),
        cms.PSet(lowBounds = cms.vdouble(200), upBounds = cms.vdouble(400), values = cms.vdouble(1,1), uncertainties = cms.vdouble(0.00175567,0.00175567,0.150616,0.150616)),
        # maximum energy beyond 7000 because of wonky DiPhotons in data
        cms.PSet(lowBounds = cms.vdouble(400 ) , upBounds = cms.vdouble(999999999 ) , values = cms.vdouble(1,1              ) , uncertainties = cms.vdouble(0.,0.,0.,0.))        
        
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
    variables = cms.vstring("abs(superCluster.eta)","r9"),
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

emptyBins = cms.PSet(
    variables = cms.vstring("1"),
    bins = cms.VPSet()
    )

emptySigma = cms.PSet(
    firstVar = cms.vint32(),
    secondVar = cms.vint32()
)

scalesAndSmearingsPrefix = cms.string("EgammaAnalysis/ElectronTools/data/80X_DCS05July_plus_Golden22")
scalesAndSmearingsPrefixForSigmaEOverE = cms.string("EgammaAnalysis/ElectronTools/data/Golden22June")

MCScaleHighR9EB = cms.PSet( PhotonMethodName = cms.string("FlashggPhotonScale"),
          MethodName = cms.string("FlashggDiPhotonFromPhoton"),
          Label = cms.string("MCScaleHighR9EB"),
          NSigmas = cms.vint32(-1,1),
          OverallRange = cms.string("r9>0.94&&abs(superCluster.eta)<1.5"),
          BinList = photonScaleUncertBins,
          ApplyCentralValue = cms.bool(False),
          Debug = cms.untracked.bool(False)
          )

MCScaleLowR9EB = cms.PSet( PhotonMethodName = cms.string("FlashggPhotonScale"),
          MethodName = cms.string("FlashggDiPhotonFromPhoton"),
          Label = cms.string("MCScaleLowR9EB"),
          NSigmas = cms.vint32(-1,1),
          OverallRange = cms.string("r9<0.94&&abs(superCluster.eta)<1.5"),
          BinList = photonScaleUncertBins,
          ApplyCentralValue = cms.bool(False),
          Debug = cms.untracked.bool(False)
          )

MCScaleHighR9EE = cms.PSet( PhotonMethodName = cms.string("FlashggPhotonScale"),
          MethodName = cms.string("FlashggDiPhotonFromPhoton"),
          Label = cms.string("MCScaleHighR9EE"),
          NSigmas = cms.vint32(-1,1),
          OverallRange = cms.string("r9>0.94&&abs(superCluster.eta)>=1.5"),
          BinList = photonScaleUncertBins,
          ApplyCentralValue = cms.bool(False),
          Debug = cms.untracked.bool(False)
          )

MCScaleLowR9EE = cms.PSet( PhotonMethodName = cms.string("FlashggPhotonScale"),
          MethodName = cms.string("FlashggDiPhotonFromPhoton"),
          Label = cms.string("MCScaleLowR9EE"),
          NSigmas = cms.vint32(-1,1),
          OverallRange = cms.string("r9<0.94&&abs(superCluster.eta)>=1.5"),
          BinList = photonScaleUncertBins,
          ApplyCentralValue = cms.bool(False),
          Debug = cms.untracked.bool(False)
          )

MaterialCentral = cms.PSet( PhotonMethodName = cms.string("FlashggPhotonScale"),
          MethodName = cms.string("FlashggDiPhotonFromPhoton"),
          Label = cms.string("MaterialCentral"),
          NSigmas = cms.vint32(-1,1),
          OverallRange = cms.string("abs(superCluster.eta)<1.0"),
          BinList = materialBinsRun1,
          ApplyCentralValue = cms.bool(False),
          Debug = cms.untracked.bool(False)
          )

MaterialForward = cms.PSet( PhotonMethodName = cms.string("FlashggPhotonScale"),
          MethodName = cms.string("FlashggDiPhotonFromPhoton"),
          Label = cms.string("MaterialForward"),
          NSigmas = cms.vint32(-1,1),
          OverallRange = cms.string("abs(superCluster.eta)>=1.0"),
          BinList = materialBinsRun1,
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
          OverallRange = cms.string("r9>0.94&&abs(superCluster.eta)<1.5"),
          BinList = showerShapeBins,
          ApplyCentralValue = cms.bool(False),
          Debug = cms.untracked.bool(False)
          )

ShowerShapeHighR9EE = cms.PSet( PhotonMethodName = cms.string("FlashggPhotonScale"),
          MethodName = cms.string("FlashggDiPhotonFromPhoton"),
          Label = cms.string("ShowerShapeHighR9EE"),
          NSigmas = cms.vint32(-1,1),
          OverallRange = cms.string("r9>0.94&&abs(superCluster.eta)>=1.5"),
          BinList = showerShapeBins,
          ApplyCentralValue = cms.bool(False),
          Debug = cms.untracked.bool(False)
          )

ShowerShapeLowR9EB = cms.PSet( PhotonMethodName = cms.string("FlashggPhotonScale"),
          MethodName = cms.string("FlashggDiPhotonFromPhoton"),
          Label = cms.string("ShowerShapeLowR9EB"),
          NSigmas = cms.vint32(-1,1),
          OverallRange = cms.string("r9<=0.94&&abs(superCluster.eta)<1.5"),
          BinList = showerShapeBins,
          ApplyCentralValue = cms.bool(False),
          Debug = cms.untracked.bool(False)
          )

ShowerShapeLowR9EE = cms.PSet( PhotonMethodName = cms.string("FlashggPhotonScale"),
          MethodName = cms.string("FlashggDiPhotonFromPhoton"),
          Label = cms.string("ShowerShapeLowR9EE"),
          NSigmas = cms.vint32(-1,1),
          OverallRange = cms.string("r9<=0.94&&abs(superCluster.eta)>=1.5"),
          BinList = showerShapeBins,
          ApplyCentralValue = cms.bool(False),
          Debug = cms.untracked.bool(False)
          )


MCSmearHighR9EE = cms.PSet( PhotonMethodName = cms.string("FlashggPhotonSmearConstant"),
          MethodName = cms.string("FlashggDiPhotonFromPhoton"),
          Label = cms.string("MCSmearHighR9EE"),
          NSigmas = cms.vint32(-1,1),
          OverallRange = cms.string("r9>0.94&&abs(superCluster.eta)>=1.5"),
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
          OverallRange = cms.string("r9<0.94&&abs(superCluster.eta)>=1.5"),
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
          OverallRange = cms.string("r9>0.94&&abs(superCluster.eta)<1.5"),
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
          OverallRange = cms.string("r9<=0.94&&abs(superCluster.eta)<1.5"),
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
          CorrectionFile = cms.FileInPath("flashgg/MicroAOD/data/transformationIDMVA_v2.root"),
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
          MethodName = cms.string("FlashggDiPhotonFromPhoton2D"),
          Label = cms.string("SigmaEOverESmearing"),
          FirstParameterName = cms.string("Rho"),
          SecondParameterName = cms.string("Phi"),
          CorrectionFile = scalesAndSmearingsPrefixForSigmaEOverE,
          NSigmas = cms.PSet( firstVar = cms.vint32(),
                            secondVar = cms.vint32()),
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

MCSmearHighR9EE_EGM = cms.PSet( PhotonMethodName = cms.string("FlashggPhotonSmearStochasticEGMTool"),
         MethodName = cms.string("FlashggDiPhotonFromPhoton2D"),
         Label = cms.string("MCSmearHighR9EE"),
         FirstParameterName = cms.string("Rho"),
         SecondParameterName = cms.string("Phi"),
         CorrectionFile = scalesAndSmearingsPrefix,
         NSigmas = cms.PSet( firstVar = cms.vint32(1,-1,0,0),
                            secondVar = cms.vint32(0,0,1,-1)),
         OverallRange = cms.string("r9>0.94&&abs(superCluster.eta)>=1.5"),
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
         OverallRange = cms.string("r9<=0.94&&abs(superCluster.eta)>=1.5"),
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
         OverallRange = cms.string("r9>0.94&&abs(superCluster.eta)<1.5"),
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
         OverallRange = cms.string("r9<=0.94&&abs(superCluster.eta)<1.5"),
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
         OverallRange = cms.string("r9>0.94&&abs(superCluster.eta)<1.5"),
         BinList = emptyBins,
         CorrectionFile = scalesAndSmearingsPrefix,
         ApplyCentralValue = cms.bool(False),
         ExaggerateShiftUp = cms.bool(False),
         Debug = cms.untracked.bool(False)
         )

MCScaleLowR9EB_EGM = cms.PSet( PhotonMethodName = cms.string("FlashggPhotonScaleEGMTool"),
         MethodName = cms.string("FlashggDiPhotonFromPhoton"),
         Label = cms.string("MCScaleLowR9EB"),
         NSigmas = cms.vint32(-1,1),
         OverallRange = cms.string("r9<0.94&&abs(superCluster.eta)<1.5"),
         BinList = emptyBins,
         CorrectionFile = scalesAndSmearingsPrefix,
         ApplyCentralValue = cms.bool(False),
         ExaggerateShiftUp = cms.bool(False),
         Debug = cms.untracked.bool(False)
         )

MCScaleHighR9EE_EGM = cms.PSet( PhotonMethodName = cms.string("FlashggPhotonScaleEGMTool"),
         MethodName = cms.string("FlashggDiPhotonFromPhoton"),
         Label = cms.string("MCScaleHighR9EE"),
         NSigmas = cms.vint32(-1,1),
         OverallRange = cms.string("r9>0.94&&abs(superCluster.eta)>=1.5"),
         BinList = emptyBins,
         CorrectionFile = scalesAndSmearingsPrefix,
         ApplyCentralValue = cms.bool(False),
         ExaggerateShiftUp = cms.bool(False),
         Debug = cms.untracked.bool(False)
         )

MCScaleLowR9EE_EGM = cms.PSet( PhotonMethodName = cms.string("FlashggPhotonScaleEGMTool"),
         MethodName = cms.string("FlashggDiPhotonFromPhoton"),
         Label = cms.string("MCScaleLowR9EE"),
         NSigmas = cms.vint32(-1,1),
         OverallRange = cms.string("r9<0.94&&abs(superCluster.eta)>=1.5"),
         BinList = emptyBins,
         CorrectionFile = scalesAndSmearingsPrefix,
         ApplyCentralValue = cms.bool(False),
         ExaggerateShiftUp = cms.bool(False),
         Debug = cms.untracked.bool(False)
         )


flashggDiPhotonSystematics = cms.EDProducer('FlashggDiPhotonSystematicProducer',
		src = cms.InputTag("flashggUpdatedIdMVADiPhotons"),
                SystMethods2D = cms.VPSet(),
                # the number of syst methods matches the number of nuisance parameters
                # assumed for a given systematic uncertainty and is NOT required
                # to match 1-to-1 the number of bins above.
                SystMethods = cms.VPSet(
                    MCScaleHighR9EB,
                    MCScaleLowR9EB,
                    MCScaleHighR9EE,
                    MCScaleLowR9EE,
                    MaterialCentral,
                    MaterialForward,
                    ShowerShapeHighR9EB,
                    ShowerShapeHighR9EE,
                    ShowerShapeLowR9EB,
                    ShowerShapeLowR9EE,
                    FNUFEB,
                    FNUFEE,
                    MCSmearHighR9EE,
                    MCSmearLowR9EE,
                    MCSmearHighR9EB,
                    MCSmearLowR9EB,
                    MvaShift,
                    PreselSF,
                    electronVetoSF,
                    TriggerWeight,
                    LooseMvaSF,
                    SigmaEOverEShift,
                    SigmaEOverESmearing,
                    FracRVWeight
                )
)
