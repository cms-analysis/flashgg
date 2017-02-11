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

# from Arnab via Martina 25/07/2016
preselBins = cms.PSet(
    variables = cms.vstring("abs(superCluster.eta)","full5x5_r9"),
    bins = cms.VPSet(
        cms.PSet( lowBounds = cms.vdouble( 0.0, 0.0 ) , upBounds = cms.vdouble( 1.5, 0.85 ) , values = cms.vdouble( 0.9996 ) , uncertainties = cms.vdouble( 0.0140 )  ) ,
        cms.PSet( lowBounds = cms.vdouble( 0.0, 0.85 ), upBounds = cms.vdouble( 1.5, 999. ) , values = cms.vdouble( 1.008 ) , uncertainties = cms.vdouble( 0.0203 )  ) ,
        cms.PSet( lowBounds = cms.vdouble( 1.5, 0.0 ) , upBounds = cms.vdouble( 6.0, 0.9  ) , values = cms.vdouble( 0.9938 ) , uncertainties = cms.vdouble( 0.0175 )  ) ,
        cms.PSet( lowBounds = cms.vdouble( 1.5, 0.9 ) , upBounds = cms.vdouble( 6.0, 999. ) , values = cms.vdouble( 0.9969 ) , uncertainties = cms.vdouble( 0.0233 )  ) 
        )
    )

# slide 7 of https://indico.cern.ch/event/535800/contributions/2213640/attachments/1295950/1969805/201607ZmmgWith2016data_Update12p9perfb.pdf from the update with 12.9/fb and shower shape correction file v4
electronVetoBins = cms.PSet(
    variables = cms.vstring("abs(superCluster.eta)","full5x5_r9"),
    bins = cms.VPSet(
        cms.PSet( lowBounds = cms.vdouble( 0.0, 0.00 ) , upBounds = cms.vdouble( 1.5, 0.85 ) , values = cms.vdouble( 0.9924 ) , uncertainties = cms.vdouble( 0.0045 )  ) ,
        cms.PSet( lowBounds = cms.vdouble( 0.0, 0.85 ) , upBounds = cms.vdouble( 1.5, 999. ) , values = cms.vdouble( 0.9960 ) , uncertainties = cms.vdouble( 0.0010 )  ) ,
        cms.PSet( lowBounds = cms.vdouble( 1.5, 0.00 ) , upBounds = cms.vdouble( 6.0, 0.90 ) , values = cms.vdouble( 0.9862 ) , uncertainties = cms.vdouble( 0.0137 )  ) ,
        cms.PSet( lowBounds = cms.vdouble( 1.5, 0.90 ) , upBounds = cms.vdouble( 6.0, 999. ) , values = cms.vdouble( 0.9999 ) , uncertainties = cms.vdouble( 0.0029 )  ) 
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

showerShapeBins = cms.PSet(
    variables = cms.vstring("abs(superCluster.eta)","full5x5_r9"),
    bins = cms.VPSet(
        cms.PSet( lowBounds = cms.vdouble( 0.0, 0.00 ) , upBounds = cms.vdouble( 1.5, 0.94 ) , values = cms.vdouble( 0. ) , uncertainties = cms.vdouble( 0.00023 )  ) ,
        cms.PSet( lowBounds = cms.vdouble( 0.0, 0.94 ) , upBounds = cms.vdouble( 1.5, 999. ) , values = cms.vdouble( 0. ) , uncertainties = cms.vdouble( 0.00024 )  ) ,
        cms.PSet( lowBounds = cms.vdouble( 1.5, 0.00 ) , upBounds = cms.vdouble( 6.0, 0.94 ) , values = cms.vdouble( 0. ) , uncertainties = cms.vdouble( 0.00064 )  ) ,
        cms.PSet( lowBounds = cms.vdouble( 1.5, 0.94 ) , upBounds = cms.vdouble( 6.0, 999. ) , values = cms.vdouble( 0. ) , uncertainties = cms.vdouble( 0.00057 )  )
        )
    )


#for full 2016 rereco B-G + prompt H dataset.  Trigger scale factors for use without HLT applied in MC

leadTriggerScaleBins = cms.PSet(
    variables = cms.vstring("full5x5_r9","abs(superCluster.eta)","pt"),
    bins = cms.VPSet(
        cms.PSet(lowBounds = cms.vdouble(0.0,0,0), upBounds = cms.vdouble(0.54,1.5,33.3333),    values = cms.vdouble(0.7368125), uncertainties = cms.vdouble(0.005934485,0.005934485)),
        cms.PSet(lowBounds = cms.vdouble(0.0,0,33.3333), upBounds = cms.vdouble(0.54,1.5,35),   values = cms.vdouble(0.803485),  uncertainties = cms.vdouble(0.006875321,0.006875321)),
        cms.PSet(lowBounds = cms.vdouble(0.0,0,35), upBounds = cms.vdouble(0.54,1.5,40),        values = cms.vdouble(0.8262914), uncertainties = cms.vdouble(0.003487855,0.003487855)),
        cms.PSet(lowBounds = cms.vdouble(0.0,0,40), upBounds = cms.vdouble(0.54,1.5,45),        values = cms.vdouble(0.8450183), uncertainties = cms.vdouble(0.003428032,0.003428032)),
        cms.PSet(lowBounds = cms.vdouble(0.0,0,45), upBounds = cms.vdouble(0.54,1.5,50),        values = cms.vdouble(0.8568889), uncertainties = cms.vdouble(0.004831102,0.004831102)),
        cms.PSet(lowBounds = cms.vdouble(0.0,0,50), upBounds = cms.vdouble(0.54,1.5,60),        values = cms.vdouble(0.8734105), uncertainties = cms.vdouble(0.008495216,0.008495216)),
        cms.PSet(lowBounds = cms.vdouble(0.0,0,60), upBounds = cms.vdouble(0.54,1.5,70),        values = cms.vdouble(0.9219323), uncertainties = cms.vdouble(0.01547847,0.01547847)),
        cms.PSet(lowBounds = cms.vdouble(0.0,0,70), upBounds = cms.vdouble(0.54,1.5,90),        values = cms.vdouble(.938305),   uncertainties = cms.vdouble(0.0316759,0.03167594)),
        cms.PSet(lowBounds = cms.vdouble(0.0,0,90), upBounds = cms.vdouble(0.54,1.5,9999999),   values = cms.vdouble(.88683),    uncertainties = cms.vdouble(0.0927345,0.0927345)),
        cms.PSet(lowBounds = cms.vdouble(0.54,0,0), upBounds = cms.vdouble(0.85,1.5,33.3333),   values = cms.vdouble(0.8975189), uncertainties = cms.vdouble(0.001102289,0.001102289)),
        cms.PSet(lowBounds = cms.vdouble(0.54,0,33.3333), upBounds = cms.vdouble(0.85,1.5,35),  values = cms.vdouble(0.9572551), uncertainties = cms.vdouble(0.0007517759,0.0007517759)),
        cms.PSet(lowBounds = cms.vdouble(0.54,0,35), upBounds = cms.vdouble(0.85,1.5,40),       values = cms.vdouble(0.9649172), uncertainties = cms.vdouble(0.0003397426,0.0003397426)),
        cms.PSet(lowBounds = cms.vdouble(0.54,0,40), upBounds = cms.vdouble(0.85,1.5,45),       values = cms.vdouble(0.9699754), uncertainties = cms.vdouble(0.0002911724,0.0002911724)),
        cms.PSet(lowBounds = cms.vdouble(0.54,0,45), upBounds = cms.vdouble(0.85,1.5,50),       values = cms.vdouble(0.973146),  uncertainties = cms.vdouble(0.0003644946,0.0003644946)),
        cms.PSet(lowBounds = cms.vdouble(0.54,0,50), upBounds = cms.vdouble(0.85,1.5,60),       values = cms.vdouble(0.9762565), uncertainties = cms.vdouble(0.0005647071,0.0005647071)),
        cms.PSet(lowBounds = cms.vdouble(0.54,0,60), upBounds = cms.vdouble(0.85,1.5,70),       values = cms.vdouble(0.9806802), uncertainties = cms.vdouble(0.001140569,0.001140569)),
        cms.PSet(lowBounds = cms.vdouble(0.54,0,70), upBounds = cms.vdouble(0.85,1.5,90),       values = cms.vdouble(0.9841905), uncertainties = cms.vdouble(0.001753392,0.001753392)),
        cms.PSet(lowBounds = cms.vdouble(0.54,0,90), upBounds = cms.vdouble(0.85,1.5,9999999),  values = cms.vdouble(.990979),   uncertainties = cms.vdouble(0.00177158,0.00177158)),
        cms.PSet(lowBounds = cms.vdouble(0.85,0,0), upBounds = cms.vdouble(999,1.5,33.3333),    values = cms.vdouble(0.9337798), uncertainties = cms.vdouble(0.0004109319,0.0004109319)),
        cms.PSet(lowBounds = cms.vdouble(0.85,0,33.3333), upBounds = cms.vdouble(999,1.5,35),   values = cms.vdouble(0.9737664), uncertainties = cms.vdouble(0.0003030143,0.0003030143)),
        cms.PSet(lowBounds = cms.vdouble(0.85,0,35), upBounds = cms.vdouble(999,1.5,40),        values = cms.vdouble(0.9795737), uncertainties = cms.vdouble(1.610485e-05,1.610485e-05)),
        cms.PSet(lowBounds = cms.vdouble(0.85,0,40), upBounds = cms.vdouble(999,1.5,45),        values = cms.vdouble(0.9820987), uncertainties = cms.vdouble(8.764671e-05,8.764671e-05)),
        cms.PSet(lowBounds = cms.vdouble(0.85,0,45), upBounds = cms.vdouble(999,1.5,50),        values = cms.vdouble(0.984919),  uncertainties = cms.vdouble(0.0001013935,0.0001013935)),
        cms.PSet(lowBounds = cms.vdouble(0.85,0,50), upBounds = cms.vdouble(999,1.5,60),        values = cms.vdouble(0.9870015), uncertainties = cms.vdouble(0.0001260565,0.0001260565)),
        cms.PSet(lowBounds = cms.vdouble(0.85,0,60), upBounds = cms.vdouble(999,1.5,70),        values = cms.vdouble(0.9866868), uncertainties = cms.vdouble(0.0003095091,0.0003095091)),
        cms.PSet(lowBounds = cms.vdouble(0.85,0,70), upBounds = cms.vdouble(999,1.5,90),        values = cms.vdouble(0.9893069), uncertainties = cms.vdouble(0.0004031963,0.0004031963)),
        cms.PSet(lowBounds = cms.vdouble(0.85,0,90), upBounds = cms.vdouble(999,1.5,9999999),   values = cms.vdouble(0.9912323), uncertainties = cms.vdouble(0.001165088,0.001165088)),
        
        cms.PSet(lowBounds = cms.vdouble(0.0,1.5,0), upBounds = cms.vdouble(0.84,3.0,33.3333),  values = cms.vdouble(0.6483833), uncertainties = cms.vdouble(0.005726951,0.005726951)),
        cms.PSet(lowBounds = cms.vdouble(0.0,1.5,33.3333), upBounds = cms.vdouble(0.84,3.0,35), values = cms.vdouble(0.7293547), uncertainties = cms.vdouble(0.00721854,0.00721854)),
        cms.PSet(lowBounds = cms.vdouble(0.0,1.5,35), upBounds = cms.vdouble(0.84,3.0,40),      values = cms.vdouble(0.7499575), uncertainties = cms.vdouble(0.003258722,0.003258722)),
        cms.PSet(lowBounds = cms.vdouble(0.0,1.5,40), upBounds = cms.vdouble(0.84,3.0,45),      values = cms.vdouble(0.7711238), uncertainties = cms.vdouble(0.003219017,0.003219017)),
        cms.PSet(lowBounds = cms.vdouble(0.0,1.5,45), upBounds = cms.vdouble(0.84,3.0,50),      values = cms.vdouble(0.7854179), uncertainties = cms.vdouble(0.00474363,0.00474363)),
        cms.PSet(lowBounds = cms.vdouble(0.0,1.5,50), upBounds = cms.vdouble(0.84,3.0,60),      values = cms.vdouble(0.8005168), uncertainties = cms.vdouble(0.006979738,0.006979738)),
        cms.PSet(lowBounds = cms.vdouble(0.0,1.5,60), upBounds = cms.vdouble(0.84,3.0,70),      values = cms.vdouble(0.8310958), uncertainties = cms.vdouble(0.01336354,0.01336354)),
        cms.PSet(lowBounds = cms.vdouble(0.0,1.5,70), upBounds = cms.vdouble(0.84,3.0,90),      values = cms.vdouble(0.8414366), uncertainties = cms.vdouble(0.02033466,0.02033466)),
        cms.PSet(lowBounds = cms.vdouble(0.0,1.5,90), upBounds = cms.vdouble(0.84,3.0,9999999), values = cms.vdouble(0.8811042), uncertainties = cms.vdouble(0.02275205,0.02275205)),
        cms.PSet(lowBounds = cms.vdouble(0.84,1.5,0), upBounds = cms.vdouble(0.9,3.0,33.3333),  values = cms.vdouble(0.8775965),   uncertainties = cms.vdouble(0.002743641,0.002743641)),
        cms.PSet(lowBounds = cms.vdouble(0.84,1.5,33.3333), upBounds = cms.vdouble(0.9,3.0,35), values = cms.vdouble(0.9721891), uncertainties = cms.vdouble(0.001415367,0.001415367)),
        cms.PSet(lowBounds = cms.vdouble(0.84,1.5,35), upBounds = cms.vdouble(0.9,3.0,40),      values = cms.vdouble(0.9822822), uncertainties = cms.vdouble(0.0005890333,0.0005890333)),
        cms.PSet(lowBounds = cms.vdouble(0.84,1.5,40), upBounds = cms.vdouble(0.9,3.0,45),      values = cms.vdouble(0.9874611), uncertainties = cms.vdouble(0.0005002402,0.0005002402)),
        cms.PSet(lowBounds = cms.vdouble(0.84,1.5,45), upBounds = cms.vdouble(0.9,3.0,50),      values = cms.vdouble(0.9881354), uncertainties = cms.vdouble(0.000747335,0.000747335)),
        cms.PSet(lowBounds = cms.vdouble(0.84,1.5,50), upBounds = cms.vdouble(0.9,3.0,60),      values = cms.vdouble(0.9883946), uncertainties = cms.vdouble(0.001083091,0.001083091)),
        cms.PSet(lowBounds = cms.vdouble(0.84,1.5,60), upBounds = cms.vdouble(0.9,3.0,70),      values = cms.vdouble(0.9898933), uncertainties = cms.vdouble(0.00200695,0.00200695)),
        cms.PSet(lowBounds = cms.vdouble(0.84,1.5,70), upBounds = cms.vdouble(0.9,3.0,90),      values = cms.vdouble(0.9907125), uncertainties = cms.vdouble(0.002246845,0.002246845)),
        cms.PSet(lowBounds = cms.vdouble(0.84,1.5,90), upBounds = cms.vdouble(0.9,3.0,9999999), values = cms.vdouble(0.9970199), uncertainties = cms.vdouble(0.001640693,0.001640693)),
        cms.PSet(lowBounds = cms.vdouble(0.9,1.5,0), upBounds = cms.vdouble(999,3.0,33.3333),   values = cms.vdouble(0.8339034), uncertainties = cms.vdouble(0.001055176,0.001055176)),
        cms.PSet(lowBounds = cms.vdouble(0.9,1.5,33.3333), upBounds = cms.vdouble(999,3.0,35),  values = cms.vdouble(0.9728958), uncertainties = cms.vdouble(0.0005636366,0.0005636366)),
        cms.PSet(lowBounds = cms.vdouble(0.9,1.5,35), upBounds = cms.vdouble(999,3.0,40),       values = cms.vdouble(0.9850414), uncertainties = cms.vdouble(0.0002257617,0.0002257617)),
        cms.PSet(lowBounds = cms.vdouble(0.9,1.5,40), upBounds = cms.vdouble(999,3.0,45),       values = cms.vdouble(0.9882804), uncertainties = cms.vdouble(0.0002009588,0.0002009588)),
        cms.PSet(lowBounds = cms.vdouble(0.9,1.5,45), upBounds = cms.vdouble(999,3.0,50),       values = cms.vdouble(0.9889721), uncertainties = cms.vdouble(0.0002744104,0.0002744104)),
        cms.PSet(lowBounds = cms.vdouble(0.9,1.5,50), upBounds = cms.vdouble(999,3.0,60),       values = cms.vdouble(0.9899117), uncertainties = cms.vdouble(0.0004237505,0.0004237505)),
        cms.PSet(lowBounds = cms.vdouble(0.9,1.5,60), upBounds = cms.vdouble(999,3.0,70),       values = cms.vdouble(0.9920495), uncertainties = cms.vdouble(0.0007189953,0.0007189953)),
        cms.PSet(lowBounds = cms.vdouble(0.9,1.5,70), upBounds = cms.vdouble(999,3.0,90),       values = cms.vdouble(0.9951106), uncertainties = cms.vdouble(0.0006737891,0.0006737891)),
        cms.PSet(lowBounds = cms.vdouble(0.9,1.5,90), upBounds = cms.vdouble(999,3.0,9999999),  values = cms.vdouble(0.9971578), uncertainties = cms.vdouble(0.000547562,0.000547562)),
        
        cms.PSet(lowBounds = cms.vdouble(0.0,3.,0), upBounds = cms.vdouble(999,999,999999),     values = cms.vdouble(1.), uncertainties = cms.vdouble(1.,1.))
        )
    )


subleadTriggerScaleBins = cms.PSet(
    variables = cms.vstring("full5x5_r9","abs(superCluster.eta)","pt"),
    bins = cms.VPSet(
        cms.PSet(lowBounds = cms.vdouble(0.0,0,0), upBounds = cms.vdouble(0.54,1.5,22.5),      values = cms.vdouble(0.7866918), uncertainties = cms.vdouble(0.01254866,0.01254866)),
        cms.PSet(lowBounds = cms.vdouble(0.0,0,22.5), upBounds = cms.vdouble(0.54,1.5,25),     values = cms.vdouble(0.8080705), uncertainties = cms.vdouble(0.01071845,0.01071845)),
        cms.PSet(lowBounds = cms.vdouble(0.0,0,25), upBounds = cms.vdouble(0.54,1.5,27.5),     values = cms.vdouble(0.8154247), uncertainties = cms.vdouble(0.007835814,0.007835814)),
        cms.PSet(lowBounds = cms.vdouble(0.0,0,27.5), upBounds = cms.vdouble(0.54,1.5,30),     values = cms.vdouble(0.8159582), uncertainties = cms.vdouble(0.006884084,0.006884084)),
        cms.PSet(lowBounds = cms.vdouble(0.0,0,30), upBounds = cms.vdouble(0.54,1.5,33.3333),  values = cms.vdouble(0.8359094), uncertainties = cms.vdouble(0.006731299,0.006731299)),
        cms.PSet(lowBounds = cms.vdouble(0.0,0,33.3333), upBounds = cms.vdouble(0.54,1.5,35),  values = cms.vdouble(0.8407688), uncertainties = cms.vdouble(0.005897487,0.005897487)),
        cms.PSet(lowBounds = cms.vdouble(0.0,0,35), upBounds = cms.vdouble(0.54,1.5,40),       values = cms.vdouble(0.8546336), uncertainties = cms.vdouble(0.003256139,0.003256139)),
        cms.PSet(lowBounds = cms.vdouble(0.0,0,40), upBounds = cms.vdouble(0.54,1.5,45),       values = cms.vdouble(0.8739202), uncertainties = cms.vdouble(0.003154279,0.003154279)),
        cms.PSet(lowBounds = cms.vdouble(0.0,0,45), upBounds = cms.vdouble(0.54,1.5,50),       values = cms.vdouble(0.8850074), uncertainties = cms.vdouble(0.004417045,0.004417045)),
        cms.PSet(lowBounds = cms.vdouble(0.0,0,50), upBounds = cms.vdouble(0.54,1.5,60),       values = cms.vdouble(0.8970586), uncertainties = cms.vdouble(0.0077702,0.0077702)),
        cms.PSet(lowBounds = cms.vdouble(0.0,0,60), upBounds = cms.vdouble(0.54,1.5,70),       values = cms.vdouble(0.9400832), uncertainties = cms.vdouble(0.01393298,0.01393298)),
        cms.PSet(lowBounds = cms.vdouble(0.0,0,70), upBounds = cms.vdouble(0.54,1.5,90),       values = cms.vdouble(0.9058264), uncertainties = cms.vdouble(0.03185569,0.03185569)),
        cms.PSet(lowBounds = cms.vdouble(0.0,0,90), upBounds = cms.vdouble(0.54,1.5,9999999),  values = cms.vdouble(0.895312),  uncertainties = cms.vdouble(0.07102735,0.07102735)),
        cms.PSet(lowBounds = cms.vdouble(0.54,0,0), upBounds = cms.vdouble(0.85,1.5,22.5),     values = cms.vdouble(0.9892057), uncertainties = cms.vdouble(0.0007812397,0.0007812397)),
        cms.PSet(lowBounds = cms.vdouble(0.54,0,22.5), upBounds = cms.vdouble(0.85,1.5,25),    values = cms.vdouble(0.9898468), uncertainties = cms.vdouble(0.0006199468,0.0006199468)),
        cms.PSet(lowBounds = cms.vdouble(0.54,0,25), upBounds = cms.vdouble(0.85,1.5,27.5),    values = cms.vdouble(0.9892397), uncertainties = cms.vdouble(0.0005056075,0.0005056075)),
        cms.PSet(lowBounds = cms.vdouble(0.54,0,27.5), upBounds = cms.vdouble(0.85,1.5,30),    values = cms.vdouble(0.9930074), uncertainties = cms.vdouble(0.0009899177,0.0009899177)),
        cms.PSet(lowBounds = cms.vdouble(0.54,0,30), upBounds = cms.vdouble(0.85,1.5,33.3333), values = cms.vdouble(0.9929972), uncertainties = cms.vdouble(0.0005588681,0.0005588681)),
        cms.PSet(lowBounds = cms.vdouble(0.54,0,33.3333), upBounds = cms.vdouble(0.85,1.5,35), values = cms.vdouble(0.9937046), uncertainties = cms.vdouble(0.0003179381,0.0003179381)),
        cms.PSet(lowBounds = cms.vdouble(0.54,0,35), upBounds = cms.vdouble(0.85,1.5,40),      values = cms.vdouble(0.9938148), uncertainties = cms.vdouble(0.0001453314,0.0001453314)),
        cms.PSet(lowBounds = cms.vdouble(0.54,0,40), upBounds = cms.vdouble(0.85,1.5,45),      values = cms.vdouble(0.9942803), uncertainties = cms.vdouble(0.0001290686,0.0001290686)),
        cms.PSet(lowBounds = cms.vdouble(0.54,0,45), upBounds = cms.vdouble(0.85,1.5,50),      values = cms.vdouble(0.993707),  uncertainties = cms.vdouble(0.0001788478,0.0001788478)),
        cms.PSet(lowBounds = cms.vdouble(0.54,0,50), upBounds = cms.vdouble(0.85,1.5,60),      values = cms.vdouble(0.9931642), uncertainties = cms.vdouble(0.0003150063,0.0003150063)),
        cms.PSet(lowBounds = cms.vdouble(0.54,0,60), upBounds = cms.vdouble(0.85,1.5,70),      values = cms.vdouble(0.9933501), uncertainties = cms.vdouble(0.0006756582,0.0006756582)),
        cms.PSet(lowBounds = cms.vdouble(0.54,0,70), upBounds = cms.vdouble(0.85,1.5,90),      values = cms.vdouble(0.9920016), uncertainties = cms.vdouble(0.001237299,0.001237299)),
        cms.PSet(lowBounds = cms.vdouble(0.54,0,90), upBounds = cms.vdouble(0.85,1.5,9999999), values = cms.vdouble(0.9912997), uncertainties = cms.vdouble(0.002155913,0.002155913)),
        cms.PSet(lowBounds = cms.vdouble(0.85,0,0), upBounds = cms.vdouble(999,1.5,22.5),      values = cms.vdouble(0.9926198), uncertainties = cms.vdouble(0.0004631939,0.0004631939)),
        cms.PSet(lowBounds = cms.vdouble(0.85,0,22.5), upBounds = cms.vdouble(999,1.5,25),     values = cms.vdouble(0.9942708), uncertainties = cms.vdouble(4.431477e-11,4.431477e-11)),
        cms.PSet(lowBounds = cms.vdouble(0.85,0,25), upBounds = cms.vdouble(999,1.5,27.5),     values = cms.vdouble(0.9965277), uncertainties = cms.vdouble(0.0002057712,0.0002057712)),
        cms.PSet(lowBounds = cms.vdouble(0.85,0,27.5), upBounds = cms.vdouble(999,1.5,30),     values = cms.vdouble(0.9974731), uncertainties = cms.vdouble(0.0001043457,0.0001043457)),
        cms.PSet(lowBounds = cms.vdouble(0.85,0,30), upBounds = cms.vdouble(999,1.5,33.3333),  values = cms.vdouble(0.9973601), uncertainties = cms.vdouble(1.674949e-11,1.674949e-11)),
        cms.PSet(lowBounds = cms.vdouble(0.85,0,33.3333), upBounds = cms.vdouble(999,1.5,35),  values = cms.vdouble(0.9981954), uncertainties = cms.vdouble(8.116824e-05,8.116824e-05)),
        cms.PSet(lowBounds = cms.vdouble(0.85,0,35), upBounds = cms.vdouble(999,1.5,40),       values = cms.vdouble(0.9986982), uncertainties = cms.vdouble(2.775563e-05,2.775563e-05)),
        cms.PSet(lowBounds = cms.vdouble(0.85,0,40), upBounds = cms.vdouble(999,1.5,45),       values = cms.vdouble(0.9991035), uncertainties = cms.vdouble(1.997999e-05,1.997999e-05)),
        cms.PSet(lowBounds = cms.vdouble(0.85,0,45), upBounds = cms.vdouble(999,1.5,50),       values = cms.vdouble(0.9992715), uncertainties = cms.vdouble(1.997999e-05,1.997999e-05)),
        cms.PSet(lowBounds = cms.vdouble(0.85,0,50), upBounds = cms.vdouble(999,1.5,60),       values = cms.vdouble(0.9993898), uncertainties = cms.vdouble(2.894345e-05,2.894345e-05)),
        cms.PSet(lowBounds = cms.vdouble(0.85,0,60), upBounds = cms.vdouble(999,1.5,70),       values = cms.vdouble(0.9993549), uncertainties = cms.vdouble(6.249402e-05,6.249402e-05)),
        cms.PSet(lowBounds = cms.vdouble(0.85,0,70), upBounds = cms.vdouble(999,1.5,90),       values = cms.vdouble(0.9994068), uncertainties = cms.vdouble(7.931303e-05,7.931303e-05)),
        cms.PSet(lowBounds = cms.vdouble(0.85,0,90), upBounds = cms.vdouble(999,1.5,9999999),  values = cms.vdouble(0.9996992), uncertainties = cms.vdouble(6.98866e-05,6.98866e-05)),
        
        cms.PSet(lowBounds = cms.vdouble(0.0,1.5,0), upBounds = cms.vdouble(0.84,3.0,22.5),     values = cms.vdouble(0.7051322), uncertainties = cms.vdouble(0.01975682,0.01975682)),
        cms.PSet(lowBounds = cms.vdouble(0.0,1.5,22.5), upBounds = cms.vdouble(0.84,3.0,25),    values = cms.vdouble(0.7179037), uncertainties = cms.vdouble(0.01366772,0.01366772)),
        cms.PSet(lowBounds = cms.vdouble(0.0,1.5,25), upBounds = cms.vdouble(0.84,3.0,27.5),    values = cms.vdouble(0.7183913), uncertainties = cms.vdouble(0.02655889,0.02655889)),
        cms.PSet(lowBounds = cms.vdouble(0.0,1.5,27.5), upBounds = cms.vdouble(0.84,3.0,30),    values = cms.vdouble(0.7321315), uncertainties = cms.vdouble(0.008956205,0.008956205)),
        cms.PSet(lowBounds = cms.vdouble(0.0,1.5,30), upBounds = cms.vdouble(0.84,3.0,33.3333), values = cms.vdouble(0.7336519), uncertainties = cms.vdouble(0.0162308,0.0162308)),
        cms.PSet(lowBounds = cms.vdouble(0.0,1.5,33.3333), upBounds = cms.vdouble(0.84,3.0,35), values = cms.vdouble(0.7417315), uncertainties = cms.vdouble(0.005347382,0.005347382)),
        cms.PSet(lowBounds = cms.vdouble(0.0,1.5,35), upBounds = cms.vdouble(0.84,3.0,40),      values = cms.vdouble(0.7570794), uncertainties = cms.vdouble(0.003242113,0.003242113)),
        cms.PSet(lowBounds = cms.vdouble(0.0,1.5,40), upBounds = cms.vdouble(0.84,3.0,45),      values = cms.vdouble(0.7759593), uncertainties = cms.vdouble(0.003214742,0.003214742)),
        cms.PSet(lowBounds = cms.vdouble(0.0,1.5,45), upBounds = cms.vdouble(0.84,3.0,50),      values = cms.vdouble(0.7889931), uncertainties = cms.vdouble(0.004746488,0.004746488)),
        cms.PSet(lowBounds = cms.vdouble(0.0,1.5,50), upBounds = cms.vdouble(0.84,3.0,60),      values = cms.vdouble(0.8034303), uncertainties = cms.vdouble(0.006989213,0.006989213)),
        cms.PSet(lowBounds = cms.vdouble(0.0,1.5,60), upBounds = cms.vdouble(0.84,3.0,70),      values = cms.vdouble(0.8332614), uncertainties = cms.vdouble(0.01388053,0.01388053)),
        cms.PSet(lowBounds = cms.vdouble(0.0,1.5,70), upBounds = cms.vdouble(0.84,3.0,90),      values = cms.vdouble(0.8417318), uncertainties = cms.vdouble(0.01816052,0.01816052)),
        cms.PSet(lowBounds = cms.vdouble(0.0,1.5,90), upBounds = cms.vdouble(0.84,3.0,9999999),  values = cms.vdouble(0.8741621), uncertainties = cms.vdouble(0.02323361,0.02323361)),
        cms.PSet(lowBounds = cms.vdouble(0.84,1.5,0), upBounds = cms.vdouble(0.9,3.0,22.5),     values = cms.vdouble(0.9826243), uncertainties = cms.vdouble(0.003703207,0.003703207)),
        cms.PSet(lowBounds = cms.vdouble(0.84,1.5,22.5), upBounds = cms.vdouble(0.9,3.0,25),    values = cms.vdouble(0.9887179), uncertainties = cms.vdouble(0.002550003,0.002550003)),
        cms.PSet(lowBounds = cms.vdouble(0.84,1.5,25), upBounds = cms.vdouble(0.9,3.0,27.5),    values = cms.vdouble(0.9863665), uncertainties = cms.vdouble(0.001956897,0.001956897)),
        cms.PSet(lowBounds = cms.vdouble(0.84,1.5,27.5), upBounds = cms.vdouble(0.9,3.0,30),    values = cms.vdouble(0.990492),  uncertainties = cms.vdouble(0.00121942,0.00121942)),
        cms.PSet(lowBounds = cms.vdouble(0.84,1.5,30), upBounds = cms.vdouble(0.9,3.0,33.3333), values = cms.vdouble(0.9912649), uncertainties = cms.vdouble(0.0007792677,0.0007792677)),
        cms.PSet(lowBounds = cms.vdouble(0.84,1.5,33.3333), upBounds = cms.vdouble(0.9,3.0,35), values = cms.vdouble(0.9918969), uncertainties = cms.vdouble(0.0006458993,0.0006458993)),
        cms.PSet(lowBounds = cms.vdouble(0.84,1.5,35), upBounds = cms.vdouble(0.9,3.0,40),      values = cms.vdouble(0.9927475), uncertainties = cms.vdouble(0.0003805226,0.0003805226)),
        cms.PSet(lowBounds = cms.vdouble(0.84,1.5,40), upBounds = cms.vdouble(0.9,3.0,45),      values = cms.vdouble(0.9938826), uncertainties = cms.vdouble(0.0003528221,0.0003528221)),
        cms.PSet(lowBounds = cms.vdouble(0.84,1.5,45), upBounds = cms.vdouble(0.9,3.0,50),      values = cms.vdouble(0.9931839), uncertainties = cms.vdouble(0.0005223983,0.0005223983)),
        cms.PSet(lowBounds = cms.vdouble(0.84,1.5,50), upBounds = cms.vdouble(0.9,3.0,60),      values = cms.vdouble(0.993775),  uncertainties = cms.vdouble(0.0007327843,0.0007327843)),
        cms.PSet(lowBounds = cms.vdouble(0.84,1.5,60), upBounds = cms.vdouble(0.9,3.0,70),      values = cms.vdouble(0.9904481), uncertainties = cms.vdouble(0.001922431,0.001922431)),
        cms.PSet(lowBounds = cms.vdouble(0.84,1.5,70), upBounds = cms.vdouble(0.9,3.0,90),      values = cms.vdouble(0.9920911), uncertainties = cms.vdouble(0.002106618,0.002106618)),
        cms.PSet(lowBounds = cms.vdouble(0.84,1.5,90), upBounds = cms.vdouble(0.9,3.0,9999999), values = cms.vdouble(0.9940322), uncertainties = cms.vdouble(0.00281186,0.00281186)),
        cms.PSet(lowBounds = cms.vdouble(0.9,1.5,0), upBounds = cms.vdouble(999,3.0,22.5),      values = cms.vdouble(0.9947124), uncertainties = cms.vdouble(1.695764e-10,1.695764e-10)),
        cms.PSet(lowBounds = cms.vdouble(0.9,1.5,22.5), upBounds = cms.vdouble(999,3.0,25),     values = cms.vdouble(0.9985281), uncertainties = cms.vdouble(7.403056e-11,7.403056e-11)),
        cms.PSet(lowBounds = cms.vdouble(0.9,1.5,25), upBounds = cms.vdouble(999,3.0,27.5),     values = cms.vdouble(0.9989061), uncertainties = cms.vdouble(5.184875e-11,5.184875e-11)),
        cms.PSet(lowBounds = cms.vdouble(0.9,1.5,27.5), upBounds = cms.vdouble(999,3.0,30),     values = cms.vdouble(0.9989711), uncertainties = cms.vdouble(0.0001578865,0.0001578865)),
        cms.PSet(lowBounds = cms.vdouble(0.9,1.5,30), upBounds = cms.vdouble(999,3.0,33.3333),  values = cms.vdouble(0.9988723), uncertainties = cms.vdouble(0.0001126245,0.0001126245)),
        cms.PSet(lowBounds = cms.vdouble(0.9,1.5,33.3333), upBounds = cms.vdouble(999,3.0,35),  values = cms.vdouble(0.9992916), uncertainties = cms.vdouble(8.178196e-05,8.178196e-05)),
        cms.PSet(lowBounds = cms.vdouble(0.9,1.5,35), upBounds = cms.vdouble(999,3.0,40),       values = cms.vdouble(0.999307),  uncertainties = cms.vdouble(5.401457e-05,5.401457e-05)),
        cms.PSet(lowBounds = cms.vdouble(0.9,1.5,40), upBounds = cms.vdouble(999,3.0,45),       values = cms.vdouble(0.9994712), uncertainties = cms.vdouble(4.527759e-05,4.527759e-05)),
        cms.PSet(lowBounds = cms.vdouble(0.9,1.5,45), upBounds = cms.vdouble(999,3.0,50),       values = cms.vdouble(0.9996495), uncertainties = cms.vdouble(6.032325e-05,6.032325e-05)),
        cms.PSet(lowBounds = cms.vdouble(0.9,1.5,50), upBounds = cms.vdouble(999,3.0,60),       values = cms.vdouble(0.9993799), uncertainties = cms.vdouble(0.0001145189,0.0001145189)),
        cms.PSet(lowBounds = cms.vdouble(0.9,1.5,60), upBounds = cms.vdouble(999,3.0,70),       values = cms.vdouble(0.9992085), uncertainties = cms.vdouble(0.0004140147,0.0004140147)),
        cms.PSet(lowBounds = cms.vdouble(0.9,1.5,70), upBounds = cms.vdouble(999,3.0,90),       values = cms.vdouble(0.9994234), uncertainties = cms.vdouble(0.00021993,0.00021993)),
        cms.PSet(lowBounds = cms.vdouble(0.9,1.5,90), upBounds = cms.vdouble(999,3.0,9999999),  values = cms.vdouble(0.9995666), uncertainties = cms.vdouble(0.0002642505,0.0002642505)),
        
        cms.PSet(lowBounds = cms.vdouble(0.0,3.,0), upBounds = cms.vdouble(999,999,999999),     values = cms.vdouble(1.), uncertainties = cms.vdouble(1.,1.))
        )
    )


# from Arnab via Martina 24/07/2016                                                                                                                               
looseMvaBins = cms.PSet(
    variables = cms.vstring("abs(superCluster.eta)","full5x5_r9"),
    bins = cms.VPSet(
        cms.PSet( lowBounds = cms.vdouble( 0.0, 0.0  ) , upBounds = cms.vdouble( 1.5, 0.85  ) , values = cms.vdouble( 0.9932 ) , uncertainties = cms.vdouble( 0.0008 )  ) ,
        cms.PSet( lowBounds = cms.vdouble( 0.0, 0.85 ) , upBounds = cms.vdouble( 1.5, 999.0 ) , values = cms.vdouble( 0.9992 ) , uncertainties = cms.vdouble( 0.0008 )  ) ,
        cms.PSet( lowBounds = cms.vdouble( 1.5, 0.0  ) , upBounds = cms.vdouble( 6.0, 0.9   ) , values = cms.vdouble( 0.9873 ) , uncertainties = cms.vdouble( 0.0033 )  ) ,
        cms.PSet( lowBounds = cms.vdouble( 1.5, 0.9  ) , upBounds = cms.vdouble( 6.0, 999.0 ) , values = cms.vdouble( 0.9992 ) , uncertainties = cms.vdouble( 0.0011 )  ) 
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

emptyBins = cms.PSet(
    variables = cms.vstring("1"),
    bins = cms.VPSet()
    )

emptySigma = cms.PSet(
    firstVar = cms.vint32(),
    secondVar = cms.vint32()
)

#scalesAndSmearingsPrefix = cms.string("EgammaAnalysis/ElectronTools/data/80X_ichepV2_2016_pho")
#scalesAndSmearingsPrefixForSigmaEOverE = cms.string("EgammaAnalysis/ElectronTools/data/Golden22June")
scalesAndSmearingsPrefix = cms.string("EgammaAnalysis/ElectronTools/data/Winter_2016_reReco_v1_ele")
scalesAndSmearingsPrefixForSigmaEOverE = cms.string("EgammaAnalysis/ElectronTools/data/Winter_2016_reReco_v1_ele")

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

MaterialCentral = cms.PSet( PhotonMethodName = cms.string("FlashggPhotonScale"),
          MethodName = cms.string("FlashggDiPhotonFromPhoton"),
          Label = cms.string("MaterialCentral"),
          NSigmas = cms.vint32(-1,1),
          OverallRange = cms.string("abs(superCluster.eta)<1.5"),
          BinList = materialBinsICHEP,
          ApplyCentralValue = cms.bool(False),
          Debug = cms.untracked.bool(False)
          )

MaterialForward = cms.PSet( PhotonMethodName = cms.string("FlashggPhotonScale"),
          MethodName = cms.string("FlashggDiPhotonFromPhoton"),
          Label = cms.string("MaterialForward"),
          NSigmas = cms.vint32(-1,1),
          OverallRange = cms.string("abs(superCluster.eta)>=1.5"),
          BinList = materialBinsICHEP,
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
