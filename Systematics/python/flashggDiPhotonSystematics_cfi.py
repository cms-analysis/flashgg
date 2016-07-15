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

# AN version 5
preselBins = cms.PSet(
    variables = cms.vstring("abs(superCluster.eta)","r9"),
    bins = cms.VPSet(
        cms.PSet( lowBounds = cms.vdouble( 0.0, 0.0 ) , upBounds = cms.vdouble( 1.5, 0.85 ) , values = cms.vdouble( 1.032 ) , uncertainties = cms.vdouble( 0.008 )  ) ,
        cms.PSet( lowBounds = cms.vdouble( 0.0, 0.85 ), upBounds = cms.vdouble( 1.5, 999. ) , values = cms.vdouble( 1.004 ) , uncertainties = cms.vdouble( 0.003 )  ) ,
        cms.PSet( lowBounds = cms.vdouble( 1.5, 0.0 ) , upBounds = cms.vdouble( 6.0, 0.9  ) , values = cms.vdouble( 1.006 ) , uncertainties = cms.vdouble( 0.032 )  ) ,
        cms.PSet( lowBounds = cms.vdouble( 1.5, 0.9 ) , upBounds = cms.vdouble( 6.0, 999. ) , values = cms.vdouble( 1.004 ) , uncertainties = cms.vdouble( 0.022 )  ) 
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
        cms.PSet(lowBounds = cms.vdouble(0.0,0,0), upBounds = cms.vdouble(0.55,1.5,33.3333), values = cms.vdouble(0.7635), uncertainties = cms.vdouble(0.163829,0.163829)),
        cms.PSet(lowBounds = cms.vdouble(0.0,0,33.3333), upBounds = cms.vdouble(0.55,1.5,35), values = cms.vdouble(0.879028), uncertainties = cms.vdouble(0.149742,0.149742)),
        cms.PSet(lowBounds = cms.vdouble(0.0,0,35), upBounds = cms.vdouble(0.55,1.5,40), values = cms.vdouble(0.889278), uncertainties = cms.vdouble(0.0749281,0.0749281)),
        cms.PSet(lowBounds = cms.vdouble(0.0,0,40), upBounds = cms.vdouble(0.55,1.5,45), values = cms.vdouble(0.902332), uncertainties = cms.vdouble(0.0703223,0.0703223)),
        cms.PSet(lowBounds = cms.vdouble(0.0,0,45), upBounds = cms.vdouble(0.55,1.5,50), values = cms.vdouble(0.918481), uncertainties = cms.vdouble(0.0939176,0.0939176)),
        cms.PSet(lowBounds = cms.vdouble(0.0,0,50), upBounds = cms.vdouble(0.55,1.5,60), values = cms.vdouble(0.91794), uncertainties = cms.vdouble(0.141839,0.141839)),
        cms.PSet(lowBounds = cms.vdouble(0.0,0,60), upBounds = cms.vdouble(0.55,1.5,70), values = cms.vdouble(0.924875), uncertainties = cms.vdouble(0.128788,0.128788)),
        cms.PSet(lowBounds = cms.vdouble(0.0,0,70), upBounds = cms.vdouble(0.55,1.5,90), values = cms.vdouble(0.924359), uncertainties = cms.vdouble(0.129019,0.129019)),
        cms.PSet(lowBounds = cms.vdouble(0.0,0,90), upBounds = cms.vdouble(0.55,1.5,999999), values = cms.vdouble(0.95384), uncertainties = cms.vdouble(0.801063,0.801063)),
        cms.PSet(lowBounds = cms.vdouble(0.55,0,0), upBounds = cms.vdouble(0.85,1.5,33.3333), values = cms.vdouble(0.860961), uncertainties = cms.vdouble(0.034386,0.034386)),
        cms.PSet(lowBounds = cms.vdouble(0.55,0,33.3333), upBounds = cms.vdouble(0.85,1.5,35), values = cms.vdouble(0.960358), uncertainties = cms.vdouble(0.0201443,0.0201443)),
        cms.PSet(lowBounds = cms.vdouble(0.55,0,35), upBounds = cms.vdouble(0.85,1.5,40), values = cms.vdouble(0.967632), uncertainties = cms.vdouble(0.00890293,0.00890293)),
        cms.PSet(lowBounds = cms.vdouble(0.55,0,40), upBounds = cms.vdouble(0.85,1.5,45), values = cms.vdouble(0.973117), uncertainties = cms.vdouble(0.00711533,0.00711533)),
        cms.PSet(lowBounds = cms.vdouble(0.55,0,45), upBounds = cms.vdouble(0.85,1.5,50), values = cms.vdouble(0.975686), uncertainties = cms.vdouble(0.00859094,0.00859094)),
        cms.PSet(lowBounds = cms.vdouble(0.55,0,50), upBounds = cms.vdouble(0.85,1.5,60), values = cms.vdouble(0.977974), uncertainties = cms.vdouble(0.0117695,0.0117695)),
        cms.PSet(lowBounds = cms.vdouble(0.55,0,60), upBounds = cms.vdouble(0.85,1.5,70), values = cms.vdouble(0.986717), uncertainties = cms.vdouble(0.0431203,0.0431203)),
        cms.PSet(lowBounds = cms.vdouble(0.55,0,70), upBounds = cms.vdouble(0.85,1.5,90), values = cms.vdouble(0.982285), uncertainties = cms.vdouble(0.0874219,0.0874219)),
        cms.PSet(lowBounds = cms.vdouble(0.55,0,90), upBounds = cms.vdouble(0.85,1.5,999999), values = cms.vdouble(0.988917), uncertainties = cms.vdouble(0.0920197,0.0920197)),
        cms.PSet(lowBounds = cms.vdouble(0.85,0,0), upBounds = cms.vdouble(999,1.5,33.3333), values = cms.vdouble(0.912302), uncertainties = cms.vdouble(0.0145261,0.0145261)),
        cms.PSet(lowBounds = cms.vdouble(0.85,0,33.3333), upBounds = cms.vdouble(999,1.5,35), values = cms.vdouble(0.971861), uncertainties = cms.vdouble(0.00988774,0.00988774)),
        cms.PSet(lowBounds = cms.vdouble(0.85,0,35), upBounds = cms.vdouble(999,1.5,40), values = cms.vdouble(0.979709), uncertainties = cms.vdouble(0.0037688,0.0037688)),
        cms.PSet(lowBounds = cms.vdouble(0.85,0,40), upBounds = cms.vdouble(999,1.5,45), values = cms.vdouble(0.98346), uncertainties = cms.vdouble(0.00257309,0.00257309)),
        cms.PSet(lowBounds = cms.vdouble(0.85,0,45), upBounds = cms.vdouble(999,1.5,50), values = cms.vdouble(0.985757), uncertainties = cms.vdouble(0.00304894,0.00304894)),
        cms.PSet(lowBounds = cms.vdouble(0.85,0,50), upBounds = cms.vdouble(999,1.5,60), values = cms.vdouble(0.987282), uncertainties = cms.vdouble(0.00469971,0.00469971)),
        cms.PSet(lowBounds = cms.vdouble(0.85,0,60), upBounds = cms.vdouble(999,1.5,70), values = cms.vdouble(0.979734), uncertainties = cms.vdouble(0.0161771,0.0161771)),
        cms.PSet(lowBounds = cms.vdouble(0.85,0,70), upBounds = cms.vdouble(999,1.5,90), values = cms.vdouble(0.989034), uncertainties = cms.vdouble(0.0173812,0.0173812)),
        cms.PSet(lowBounds = cms.vdouble(0.85,0,90), upBounds = cms.vdouble(999,1.5,999999), values = cms.vdouble(0.988829), uncertainties = cms.vdouble(0.0130439,0.0130439)),
        
        cms.PSet(lowBounds = cms.vdouble(0.0,1.5,0), upBounds = cms.vdouble(0.85,3.0,33.3333), values = cms.vdouble(0.662814), uncertainties = cms.vdouble(0.157045,0.157045)),
        cms.PSet(lowBounds = cms.vdouble(0.0,1.5,33.3333), upBounds = cms.vdouble(0.85,3.0,35), values = cms.vdouble(0.81578), uncertainties = cms.vdouble(0.084438,0.084438)),
        cms.PSet(lowBounds = cms.vdouble(0.0,1.5,35), upBounds = cms.vdouble(0.85,3.0,40), values = cms.vdouble(0.83025), uncertainties = cms.vdouble(0.0735095,0.0735095)),
        cms.PSet(lowBounds = cms.vdouble(0.0,1.5,40), upBounds = cms.vdouble(0.85,3.0,45), values = cms.vdouble(0.845993), uncertainties = cms.vdouble(0.0675948,0.0675948)),
        cms.PSet(lowBounds = cms.vdouble(0.0,1.5,45), upBounds = cms.vdouble(0.85,3.0,50), values = cms.vdouble(0.850527), uncertainties = cms.vdouble(0.119681,0.119681)),
        cms.PSet(lowBounds = cms.vdouble(0.0,1.5,50), upBounds = cms.vdouble(0.85,3.0,60), values = cms.vdouble(0.861851), uncertainties = cms.vdouble(0.123405,0.123405)),
        cms.PSet(lowBounds = cms.vdouble(0.0,1.5,60), upBounds = cms.vdouble(0.85,3.0,70), values = cms.vdouble(0.890836), uncertainties = cms.vdouble(0.223341,0.223341)),
        cms.PSet(lowBounds = cms.vdouble(0.0,1.5,70), upBounds = cms.vdouble(0.85,3.0,90), values = cms.vdouble(0.896951), uncertainties = cms.vdouble(0.140135,0.140135)),
        cms.PSet(lowBounds = cms.vdouble(0.0,1.5,90), upBounds = cms.vdouble(0.85,3.0,999999), values = cms.vdouble(0.900974), uncertainties = cms.vdouble(0.138633,0.138633)),
        cms.PSet(lowBounds = cms.vdouble(0.85,1.5,0), upBounds = cms.vdouble(0.9,3.0,33.3333), values = cms.vdouble(0.767776), uncertainties = cms.vdouble(0.100585,0.100585)),
        cms.PSet(lowBounds = cms.vdouble(0.85,1.5,33.3333), upBounds = cms.vdouble(0.9,3.0,35), values = cms.vdouble(0.961381), uncertainties = cms.vdouble(0.100773,0.100773)),
        cms.PSet(lowBounds = cms.vdouble(0.85,1.5,35), upBounds = cms.vdouble(0.9,3.0,40), values = cms.vdouble(0.984018), uncertainties = cms.vdouble(0.0202268,0.0202268)),
        cms.PSet(lowBounds = cms.vdouble(0.85,1.5,40), upBounds = cms.vdouble(0.9,3.0,45), values = cms.vdouble(0.987936), uncertainties = cms.vdouble(0.0164115,0.0164115)),
        cms.PSet(lowBounds = cms.vdouble(0.85,1.5,45), upBounds = cms.vdouble(0.9,3.0,50), values = cms.vdouble(0.989514), uncertainties = cms.vdouble(0.0373995,0.0373995)),
        cms.PSet(lowBounds = cms.vdouble(0.85,1.5,50), upBounds = cms.vdouble(0.9,3.0,60), values = cms.vdouble(0.991144), uncertainties = cms.vdouble(0.0249723,0.0249723)),
        cms.PSet(lowBounds = cms.vdouble(0.85,1.5,60), upBounds = cms.vdouble(0.9,3.0,70), values = cms.vdouble(0.990017), uncertainties = cms.vdouble(0.091276,0.091276)),
        cms.PSet(lowBounds = cms.vdouble(0.85,1.5,70), upBounds = cms.vdouble(0.9,3.0,90), values = cms.vdouble(0.9934), uncertainties = cms.vdouble(0.117221,0.117221)),
        cms.PSet(lowBounds = cms.vdouble(0.85,1.5,90), upBounds = cms.vdouble(0.9,3.0,999999), values = cms.vdouble(0.995246), uncertainties = cms.vdouble(0.0877127,0.0877127)),
        cms.PSet(lowBounds = cms.vdouble(0.9,1.5,0), upBounds = cms.vdouble(999,3.0,33.3333), values = cms.vdouble(0.683636), uncertainties = cms.vdouble(0.0409331,0.0409331)),
        cms.PSet(lowBounds = cms.vdouble(0.9,1.5,33.3333), upBounds = cms.vdouble(999,3.0,35), values = cms.vdouble(0.97684), uncertainties = cms.vdouble(0.00835194,0.00835194)),
        cms.PSet(lowBounds = cms.vdouble(0.9,1.5,35), upBounds = cms.vdouble(999,3.0,40), values = cms.vdouble(0.980213), uncertainties = cms.vdouble(0.0067896,0.0067896)),
        cms.PSet(lowBounds = cms.vdouble(0.9,1.5,40), upBounds = cms.vdouble(999,3.0,45), values = cms.vdouble(0.986465), uncertainties = cms.vdouble(0.00642962,0.00642962)),
        cms.PSet(lowBounds = cms.vdouble(0.9,1.5,45), upBounds = cms.vdouble(999,3.0,50), values = cms.vdouble(0.987902), uncertainties = cms.vdouble(0.00737469,0.00737469)),
        cms.PSet(lowBounds = cms.vdouble(0.9,1.5,50), upBounds = cms.vdouble(999,3.0,60), values = cms.vdouble(0.988291), uncertainties = cms.vdouble(0.0107741,0.0107741)),
        cms.PSet(lowBounds = cms.vdouble(0.9,1.5,60), upBounds = cms.vdouble(999,3.0,70), values = cms.vdouble(0.991139), uncertainties = cms.vdouble(0.0387218,0.0387218)),
        cms.PSet(lowBounds = cms.vdouble(0.9,1.5,70), upBounds = cms.vdouble(999,3.0,90), values = cms.vdouble(0.99175), uncertainties = cms.vdouble(0.0663639,0.0663639)),
        cms.PSet(lowBounds = cms.vdouble(0.9,1.5,90), upBounds = cms.vdouble(999,3.0,999999), values = cms.vdouble(0.99343), uncertainties = cms.vdouble(0.0734221,0.0734221)),
        cms.PSet(lowBounds = cms.vdouble(0.0,3.,0), upBounds = cms.vdouble(999,999,999999), values = cms.vdouble(1.), uncertainties = cms.vdouble(1.,1.))
        )
    )


subleadTriggerScaleBins = cms.PSet(
    variables = cms.vstring("r9","abs(superCluster.eta)","pt"),
    bins = cms.VPSet(
        cms.PSet(lowBounds = cms.vdouble(0.0,0,0), upBounds = cms.vdouble(0.55,1.5,22.5), values = cms.vdouble(0.854435), uncertainties = cms.vdouble(0.153914,0.153914)),
        cms.PSet(lowBounds = cms.vdouble(0.0,0,22.5), upBounds = cms.vdouble(0.55,1.5,25), values = cms.vdouble(0.890449), uncertainties = cms.vdouble(0.169063,0.169063)),
        cms.PSet(lowBounds = cms.vdouble(0.0,0,25), upBounds = cms.vdouble(0.55,1.5,27.5), values = cms.vdouble(0.896599), uncertainties = cms.vdouble(0.153691,0.153691)),
        cms.PSet(lowBounds = cms.vdouble(0.0,0,27.5), upBounds = cms.vdouble(0.55,1.5,30), values = cms.vdouble(0.901832), uncertainties = cms.vdouble(0.138307,0.138307)),
        cms.PSet(lowBounds = cms.vdouble(0.0,0,30), upBounds = cms.vdouble(0.55,1.5,33.3333), values = cms.vdouble(0.907066), uncertainties = cms.vdouble(0.136279,0.136279)),
        cms.PSet(lowBounds = cms.vdouble(0.0,0,33.3333), upBounds = cms.vdouble(0.55,1.5,35), values = cms.vdouble(0.913635), uncertainties = cms.vdouble(0.131835,0.131835)),
        cms.PSet(lowBounds = cms.vdouble(0.0,0,35), upBounds = cms.vdouble(0.55,1.5,40), values = cms.vdouble(0.91794), uncertainties = cms.vdouble(0.0660042,0.0660042)),
        cms.PSet(lowBounds = cms.vdouble(0.0,0,40), upBounds = cms.vdouble(0.55,1.5,45), values = cms.vdouble(0.931323), uncertainties = cms.vdouble(0.0604456,0.0604456)),
        cms.PSet(lowBounds = cms.vdouble(0.0,0,45), upBounds = cms.vdouble(0.55,1.5,50), values = cms.vdouble(0.944398), uncertainties = cms.vdouble(0.0806266,0.0806266)),
        cms.PSet(lowBounds = cms.vdouble(0.0,0,50), upBounds = cms.vdouble(0.55,1.5,60), values = cms.vdouble(0.948354), uncertainties = cms.vdouble(0.117212,0.117212)),
        cms.PSet(lowBounds = cms.vdouble(0.0,0,60), upBounds = cms.vdouble(0.55,1.5,70), values = cms.vdouble(0.948826), uncertainties = cms.vdouble(0.116956,0.116956)),
        cms.PSet(lowBounds = cms.vdouble(0.0,0,70), upBounds = cms.vdouble(0.55,1.5,90), values = cms.vdouble(0.925785), uncertainties = cms.vdouble(0.957026,0.957026)),
        cms.PSet(lowBounds = cms.vdouble(0.0,0,90), upBounds = cms.vdouble(0.55,1.5,999999), values = cms.vdouble(0.95384), uncertainties = cms.vdouble(0.801063,0.801063)),
        
        cms.PSet(lowBounds = cms.vdouble(0.55,0,0), upBounds = cms.vdouble(0.85,1.5,22.5), values = cms.vdouble(0.989816), uncertainties = cms.vdouble(0.0420725,0.0420725)),
        cms.PSet(lowBounds = cms.vdouble(0.55,0,22.5), upBounds = cms.vdouble(0.85,1.5,25), values = cms.vdouble(0.991947), uncertainties = cms.vdouble(0.0169083,0.0169083)),
        cms.PSet(lowBounds = cms.vdouble(0.55,0,25), upBounds = cms.vdouble(0.85,1.5,27.5), values = cms.vdouble(0.993767), uncertainties = cms.vdouble(0.0227635,0.0227635)),
        cms.PSet(lowBounds = cms.vdouble(0.55,0,27.5), upBounds = cms.vdouble(0.85,1.5,30), values = cms.vdouble(0.990037), uncertainties = cms.vdouble(0.0191881,0.0191881)),
        cms.PSet(lowBounds = cms.vdouble(0.55,0,30), upBounds = cms.vdouble(0.85,1.5,33.3333), values = cms.vdouble(0.993207), uncertainties = cms.vdouble(0.0107986,0.0107986)),
        cms.PSet(lowBounds = cms.vdouble(0.55,0,33.3333), upBounds = cms.vdouble(0.85,1.5,35), values = cms.vdouble(0.994697), uncertainties = cms.vdouble(0.0135679,0.0135679)),
        cms.PSet(lowBounds = cms.vdouble(0.55,0,35), upBounds = cms.vdouble(0.85,1.5,40), values = cms.vdouble(0.995293), uncertainties = cms.vdouble(0.00485554,0.00485554)),
        cms.PSet(lowBounds = cms.vdouble(0.55,0,40), upBounds = cms.vdouble(0.85,1.5,45), values = cms.vdouble(0.99569), uncertainties = cms.vdouble(0.00310689,0.00310689)),
        cms.PSet(lowBounds = cms.vdouble(0.55,0,45), upBounds = cms.vdouble(0.85,1.5,50), values = cms.vdouble(0.995187), uncertainties = cms.vdouble(0.00404808,0.00404808)),
        cms.PSet(lowBounds = cms.vdouble(0.55,0,50), upBounds = cms.vdouble(0.85,1.5,60), values = cms.vdouble(0.993708), uncertainties = cms.vdouble(0.00770686,0.00770686)),
        cms.PSet(lowBounds = cms.vdouble(0.55,0,60), upBounds = cms.vdouble(0.85,1.5,70), values = cms.vdouble(0.99462), uncertainties = cms.vdouble(0.0460525,0.0460525)),
        cms.PSet(lowBounds = cms.vdouble(0.55,0,70), upBounds = cms.vdouble(0.85,1.5,90), values = cms.vdouble(0.992479), uncertainties = cms.vdouble(0.0675497,0.0675497)),
        cms.PSet(lowBounds = cms.vdouble(0.55,0,90), upBounds = cms.vdouble(0.85,1.5,999999), values = cms.vdouble(0.992725), uncertainties = cms.vdouble(0.0894363,0.0894363)),
        
        cms.PSet(lowBounds = cms.vdouble(0.85,0,0), upBounds = cms.vdouble(999,1.5,22.5), values = cms.vdouble(0.97841), uncertainties = cms.vdouble(0.0244727,0.0244727)),
        cms.PSet(lowBounds = cms.vdouble(0.85,0,22.5), upBounds = cms.vdouble(999,1.5,25), values = cms.vdouble(0.994065), uncertainties = cms.vdouble(0.0122321,0.0122321)),
        cms.PSet(lowBounds = cms.vdouble(0.85,0,25), upBounds = cms.vdouble(999,1.5,27.5), values = cms.vdouble(0.994368), uncertainties = cms.vdouble(0.00932334,0.00932334)),
        cms.PSet(lowBounds = cms.vdouble(0.85,0,27.5), upBounds = cms.vdouble(999,1.5,30), values = cms.vdouble(0.996238), uncertainties = cms.vdouble(0.00645919,0.00645919)),
        cms.PSet(lowBounds = cms.vdouble(0.85,0,30), upBounds = cms.vdouble(999,1.5,33.3333), values = cms.vdouble(0.997118), uncertainties = cms.vdouble(0.00325806,0.00325806)),
        cms.PSet(lowBounds = cms.vdouble(0.85,0,33.3333), upBounds = cms.vdouble(999,1.5,35), values = cms.vdouble(0.998677), uncertainties = cms.vdouble(0.0020928,0.0020928)),
        cms.PSet(lowBounds = cms.vdouble(0.85,0,35), upBounds = cms.vdouble(999,1.5,40), values = cms.vdouble(0.998683), uncertainties = cms.vdouble(0.000893517,0.000893517)),
        cms.PSet(lowBounds = cms.vdouble(0.85,0,40), upBounds = cms.vdouble(999,1.5,45), values = cms.vdouble(0.999182), uncertainties = cms.vdouble(0.00061764,0.00061764)),
        cms.PSet(lowBounds = cms.vdouble(0.85,0,45), upBounds = cms.vdouble(999,1.5,50), values = cms.vdouble(0.999475), uncertainties = cms.vdouble(0.00061281,0.00061281)),
        cms.PSet(lowBounds = cms.vdouble(0.85,0,50), upBounds = cms.vdouble(999,1.5,60), values = cms.vdouble(0.999496), uncertainties = cms.vdouble(0.000835649,0.000835649)),
        cms.PSet(lowBounds = cms.vdouble(0.85,0,60), upBounds = cms.vdouble(999,1.5,70), values = cms.vdouble(0.999434), uncertainties = cms.vdouble(0.0072497,0.0072497)),
        cms.PSet(lowBounds = cms.vdouble(0.85,0,70), upBounds = cms.vdouble(999,1.5,90), values = cms.vdouble(0.999772), uncertainties = cms.vdouble(0.00851415,0.00851415)),
        cms.PSet(lowBounds = cms.vdouble(0.85,0,90), upBounds = cms.vdouble(999,1.5,999999), values = cms.vdouble(1), uncertainties = cms.vdouble(0.0118445,0.0118445)),
        
        cms.PSet(lowBounds = cms.vdouble(0.0,1.5,0), upBounds = cms.vdouble(0.85,3.0,22.5), values = cms.vdouble(0.787275), uncertainties = cms.vdouble(0.278766,0.278766)),
        cms.PSet(lowBounds = cms.vdouble(0.0,1.5,22.5), upBounds = cms.vdouble(0.85,3.0,25), values = cms.vdouble(0.777265), uncertainties = cms.vdouble(0.171962,0.171962)),
        cms.PSet(lowBounds = cms.vdouble(0.0,1.5,25), upBounds = cms.vdouble(0.85,3.0,27.5), values = cms.vdouble(0.79815), uncertainties = cms.vdouble(0.203152,0.203152)),
        cms.PSet(lowBounds = cms.vdouble(0.0,1.5,27.5), upBounds = cms.vdouble(0.85,3.0,30), values = cms.vdouble(0.811727), uncertainties = cms.vdouble(0.171971,0.171971)),
        cms.PSet(lowBounds = cms.vdouble(0.0,1.5,30), upBounds = cms.vdouble(0.85,3.0,33.3333), values = cms.vdouble(0.816492), uncertainties = cms.vdouble(0.128134,0.128134)),
        cms.PSet(lowBounds = cms.vdouble(0.0,1.5,33.3333), upBounds = cms.vdouble(0.85,3.0,35), values = cms.vdouble(0.829126), uncertainties = cms.vdouble(0.146704,0.146704)),
        cms.PSet(lowBounds = cms.vdouble(0.0,1.5,35), upBounds = cms.vdouble(0.85,3.0,40), values = cms.vdouble(0.839178), uncertainties = cms.vdouble(0.072172,0.072172)),
        cms.PSet(lowBounds = cms.vdouble(0.0,1.5,40), upBounds = cms.vdouble(0.85,3.0,45), values = cms.vdouble(0.850255), uncertainties = cms.vdouble(0.0671414,0.0671414)),
        cms.PSet(lowBounds = cms.vdouble(0.0,1.5,45), upBounds = cms.vdouble(0.85,3.0,50), values = cms.vdouble(0.854513), uncertainties = cms.vdouble(0.0891853,0.0891853)),
        cms.PSet(lowBounds = cms.vdouble(0.0,1.5,50), upBounds = cms.vdouble(0.85,3.0,60), values = cms.vdouble(0.878577), uncertainties = cms.vdouble(0.146525,0.146525)),
        cms.PSet(lowBounds = cms.vdouble(0.0,1.5,60), upBounds = cms.vdouble(0.85,3.0,70), values = cms.vdouble(0.893625), uncertainties = cms.vdouble(0.221853,0.221853)),
        cms.PSet(lowBounds = cms.vdouble(0.0,1.5,70), upBounds = cms.vdouble(0.85,3.0,90), values = cms.vdouble(0.894782), uncertainties = cms.vdouble(0.532653,0.532653)),
        cms.PSet(lowBounds = cms.vdouble(0.0,1.5,90), upBounds = cms.vdouble(0.85,3.0,999999), values = cms.vdouble(0.888978), uncertainties = cms.vdouble(0.142999,0.142999)),
        
        cms.PSet(lowBounds = cms.vdouble(0.85,1.5,0), upBounds = cms.vdouble(0.9,3.0,22.5), values = cms.vdouble(0.968423), uncertainties = cms.vdouble(0.105457,0.105457)),
        cms.PSet(lowBounds = cms.vdouble(0.85,1.5,22.5), upBounds = cms.vdouble(0.9,3.0,25), values = cms.vdouble(0.993974), uncertainties = cms.vdouble(0.0885836,0.0885836)),
        cms.PSet(lowBounds = cms.vdouble(0.85,1.5,25), upBounds = cms.vdouble(0.9,3.0,27.5), values = cms.vdouble(0.990256), uncertainties = cms.vdouble(0.0911141,0.0911141)),
        cms.PSet(lowBounds = cms.vdouble(0.85,1.5,27.5), upBounds = cms.vdouble(0.9,3.0,30), values = cms.vdouble(0.98956), uncertainties = cms.vdouble(0.0915851,0.0915851)),
        cms.PSet(lowBounds = cms.vdouble(0.85,1.5,30), upBounds = cms.vdouble(0.9,3.0,33.3333), values = cms.vdouble(0.993943), uncertainties = cms.vdouble(0.0557785,0.0557785)),
        cms.PSet(lowBounds = cms.vdouble(0.85,1.5,33.3333), upBounds = cms.vdouble(0.9,3.0,35), values = cms.vdouble(0.994551), uncertainties = cms.vdouble(0.0793104,0.0793104)),
        cms.PSet(lowBounds = cms.vdouble(0.85,1.5,35), upBounds = cms.vdouble(0.9,3.0,40), values = cms.vdouble(0.995491), uncertainties = cms.vdouble(0.0223824,0.0223824)),
        cms.PSet(lowBounds = cms.vdouble(0.85,1.5,40), upBounds = cms.vdouble(0.9,3.0,45), values = cms.vdouble(0.995686), uncertainties = cms.vdouble(0.0201981,0.0201981)),
        cms.PSet(lowBounds = cms.vdouble(0.85,1.5,45), upBounds = cms.vdouble(0.9,3.0,50), values = cms.vdouble(0.994845), uncertainties = cms.vdouble(0.0340122,0.0340122)),
        cms.PSet(lowBounds = cms.vdouble(0.85,1.5,50), upBounds = cms.vdouble(0.9,3.0,60), values = cms.vdouble(0.994532), uncertainties = cms.vdouble(0.0193471,0.0193471)),
        cms.PSet(lowBounds = cms.vdouble(0.85,1.5,60), upBounds = cms.vdouble(0.9,3.0,70), values = cms.vdouble(0.991368), uncertainties = cms.vdouble(0.0765343,0.0765343)),
        cms.PSet(lowBounds = cms.vdouble(0.85,1.5,70), upBounds = cms.vdouble(0.9,3.0,90), values = cms.vdouble(0.994232), uncertainties = cms.vdouble(0.0884069,0.0884069)),
        cms.PSet(lowBounds = cms.vdouble(0.85,1.5,90), upBounds = cms.vdouble(0.9,3.0,999999), values = cms.vdouble(0.994101), uncertainties = cms.vdouble(0.188704,0.188704)),
        
        cms.PSet(lowBounds = cms.vdouble(0.9,1.5,0), upBounds = cms.vdouble(999,3.0,22.5), values = cms.vdouble(0.969587), uncertainties = cms.vdouble(0.039105,0.039105)),
        cms.PSet(lowBounds = cms.vdouble(0.9,1.5,22.5), upBounds = cms.vdouble(999,3.0,25), values = cms.vdouble(0.996676), uncertainties = cms.vdouble(0.0214523,0.0214523)),
        cms.PSet(lowBounds = cms.vdouble(0.9,1.5,25), upBounds = cms.vdouble(999,3.0,27.5), values = cms.vdouble(0.998897), uncertainties = cms.vdouble(0.0149529,0.0149529)),
        cms.PSet(lowBounds = cms.vdouble(0.9,1.5,27.5), upBounds = cms.vdouble(999,3.0,30), values = cms.vdouble(0.998201), uncertainties = cms.vdouble(0.011616,0.011616)),
        cms.PSet(lowBounds = cms.vdouble(0.9,1.5,30), upBounds = cms.vdouble(999,3.0,33.3333), values = cms.vdouble(0.999194), uncertainties = cms.vdouble(0.00238635,0.00238635)),
        cms.PSet(lowBounds = cms.vdouble(0.9,1.5,33.3333), upBounds = cms.vdouble(999,3.0,35), values = cms.vdouble(0.999276), uncertainties = cms.vdouble(0.00854162,0.00854162)),
        cms.PSet(lowBounds = cms.vdouble(0.9,1.5,35), upBounds = cms.vdouble(999,3.0,40), values = cms.vdouble(0.999156), uncertainties = cms.vdouble(0.00272319,0.00272319)),
        cms.PSet(lowBounds = cms.vdouble(0.9,1.5,40), upBounds = cms.vdouble(999,3.0,45), values = cms.vdouble(0.999515), uncertainties = cms.vdouble(0.00233563,0.00233563)),
        cms.PSet(lowBounds = cms.vdouble(0.9,1.5,45), upBounds = cms.vdouble(999,3.0,50), values = cms.vdouble(0.999397), uncertainties = cms.vdouble(0.00407974,0.00407974)),
        cms.PSet(lowBounds = cms.vdouble(0.9,1.5,50), upBounds = cms.vdouble(999,3.0,60), values = cms.vdouble(0.999573), uncertainties = cms.vdouble(0.00239142,0.00239142)),
        cms.PSet(lowBounds = cms.vdouble(0.9,1.5,60), upBounds = cms.vdouble(999,3.0,70), values = cms.vdouble(0.999471), uncertainties = cms.vdouble(0.0330566,0.0330566)),
        cms.PSet(lowBounds = cms.vdouble(0.9,1.5,70), upBounds = cms.vdouble(999,3.0,90), values = cms.vdouble(0.999194), uncertainties = cms.vdouble(0.0489851,0.0489851)),
        cms.PSet(lowBounds = cms.vdouble(0.9,1.5,90), upBounds = cms.vdouble(999,3.0,999999), values = cms.vdouble(0.999), uncertainties = cms.vdouble(0.0699623,0.0699623)),
        cms.PSet(lowBounds = cms.vdouble(0.0,3.,0), upBounds = cms.vdouble(999,999,999999), values = cms.vdouble(1.), uncertainties = cms.vdouble(1.,1.))
        )
    )


# AN version 5
# N.B. boundary changed accordingly to the preselection
looseMvaBins = cms.PSet(
    variables = cms.vstring("abs(superCluster.eta)","r9"),
    bins = cms.VPSet(
        cms.PSet( lowBounds = cms.vdouble( 0.0, 0.0  ) , upBounds = cms.vdouble( 1.5, 0.85  ) , values = cms.vdouble( 0.9996 ) , uncertainties = cms.vdouble( 0.0008 )  ) ,
        cms.PSet( lowBounds = cms.vdouble( 0.0, 0.85 ) , upBounds = cms.vdouble( 1.5, 999.0 ) , values = cms.vdouble( 0.9992 ) , uncertainties = cms.vdouble( 0.0005 )  ) ,
        cms.PSet( lowBounds = cms.vdouble( 1.5, 0.0  ) , upBounds = cms.vdouble( 6.0, 0.9   ) , values = cms.vdouble( 1.0014 ) , uncertainties = cms.vdouble( 0.0016 )  ) ,
        cms.PSet( lowBounds = cms.vdouble( 1.5, 0.9  ) , upBounds = cms.vdouble( 6.0, 999.0 ) , values = cms.vdouble( 0.9998 ) , uncertainties = cms.vdouble( 0.0019 )  ) 
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
        cms.PSet(lowBounds = cms.vdouble(0), upBounds = cms.vdouble(5), values = cms.vdouble(1.01448,0.981165), uncertainties = cms.vdouble(0.00436957,0.00436957,0.00568231,0.00568231)),
        cms.PSet(lowBounds = cms.vdouble(5), upBounds = cms.vdouble(10), values = cms.vdouble(1.02477,0.964937), uncertainties = cms.vdouble(0.00340337,0.00340337,0.00481751,0.00481751)),
        cms.PSet(lowBounds = cms.vdouble(10), upBounds = cms.vdouble(15), values = cms.vdouble(1.01705,0.97076), uncertainties = cms.vdouble(0.00320492,0.00320492,0.00549499,0.00549499)),
        cms.PSet(lowBounds = cms.vdouble(15), upBounds = cms.vdouble(20), values = cms.vdouble(1.00447,0.990251), uncertainties = cms.vdouble(0.00301649,0.00301649,0.00657761,0.00657761)),
        cms.PSet(lowBounds = cms.vdouble(20), upBounds = cms.vdouble(30), values = cms.vdouble(0.989095,1.03485), uncertainties = cms.vdouble(0.00203486,0.00203486,0.00650198,0.00650198)),
        cms.PSet(lowBounds = cms.vdouble(30), upBounds = cms.vdouble(40), values = cms.vdouble(0.990194,1.05189), uncertainties = cms.vdouble(0.00202018,0.00202018,0.0106899,0.0106899)),
        cms.PSet(lowBounds = cms.vdouble(40), upBounds = cms.vdouble(50), values = cms.vdouble(0.993103,1.05375), uncertainties = cms.vdouble(0.00211958,0.00211958,0.0165186,0.0165186)),
        cms.PSet(lowBounds = cms.vdouble(50), upBounds = cms.vdouble(60), values = cms.vdouble(0.997169,1.03212), uncertainties = cms.vdouble(0.00226204,0.00226204,0.0256642,0.0256642)),
        cms.PSet(lowBounds = cms.vdouble(60), upBounds = cms.vdouble(80), values = cms.vdouble(0.989989,1.16613), uncertainties = cms.vdouble(0.00170693,0.00170693,0.0283254,0.0283254)),
        cms.PSet(lowBounds = cms.vdouble(80), upBounds = cms.vdouble(100), values = cms.vdouble(0.994285,1.12413), uncertainties = cms.vdouble(0.00195568,0.00195568,0.0424751,0.0424751)),
        cms.PSet(lowBounds = cms.vdouble(100), upBounds = cms.vdouble(140), values = cms.vdouble(0.997186,1.09614), uncertainties = cms.vdouble(0.00166747,0.00166747,0.0569571,0.0569571)),
        cms.PSet(lowBounds = cms.vdouble(140), upBounds = cms.vdouble(200), values = cms.vdouble(0.998048,1.0862), uncertainties = cms.vdouble(0.00196353,0.00196353,0.0866963,0.0866963)),
        cms.PSet(lowBounds = cms.vdouble(200), upBounds = cms.vdouble(400), values = cms.vdouble(1,1), uncertainties = cms.vdouble(0.00276887,0.00276887,0.255427,0.255427)),
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
