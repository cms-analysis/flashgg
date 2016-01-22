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
            cms.PSet( lowBounds = cms.vdouble(2.000 , -999.000 ) , upBounds = cms.vdouble(6.000 , 0.940   ) , values = cms.vdouble( 0.02042  ) , uncertainties = cms.vdouble( 0.00200 )  ) ,
            cms.PSet( lowBounds = cms.vdouble(2.000 , 0.940    ) , upBounds = cms.vdouble(6.000 , 999.000 ) , values = cms.vdouble( 0.01445  ) , uncertainties = cms.vdouble( 0.00300 )  ) ,
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
            cms.PSet( lowBounds = cms.vdouble(2.000,-999.000 ) , upBounds = cms.vdouble(6.000,0.940   ) , values = cms.vdouble( 0.0324 ) , uncertainties = cms.vdouble( 0.00061517 )  ) ,
            cms.PSet( lowBounds = cms.vdouble(2.000,0.940    ) , upBounds = cms.vdouble(6.000,999.000 ) , values = cms.vdouble( 0.0314 ) , uncertainties = cms.vdouble( 0.00042066 )  ) ,
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
        cms.PSet( lowBounds = cms.vdouble( 1.5, 0.0 ) , upBounds = cms.vdouble( 6.0, 0.9  ) , values = cms.vdouble( 1.0115 ) , uncertainties = cms.vdouble( 0.0297 )  ) ,
        cms.PSet( lowBounds = cms.vdouble( 1.5, 0.9 ) , upBounds = cms.vdouble( 6.0, 999. ) , values = cms.vdouble(0.9963  ) , uncertainties = cms.vdouble( 0.0044 )  ) 
        )
    )
#for 74x MC uses trigger efficiency not just scale factor since the trigger isn't in the MC
leadTriggerScaleBins = cms.PSet(
    variables = cms.vstring("r9","abs(superCluster.eta)","pt"),
    bins = cms.VPSet(
        
        #cat 0
        cms.PSet(lowBounds = cms.vdouble(0.94,0,0), upBounds = cms.vdouble(999,1.5,30), values = cms.vdouble(0), uncertainties = cms.vdouble(0,0)),
        cms.PSet(lowBounds = cms.vdouble(0.94,0,30), upBounds = cms.vdouble(999,1.5,33.33), values = cms.vdouble(1.04579), uncertainties = cms.vdouble(0.00290162,0.00278382)),
        cms.PSet(lowBounds = cms.vdouble(0.94,0,33.33), upBounds = cms.vdouble(999,1.5,35), values = cms.vdouble(1.00438), uncertainties = cms.vdouble(0.00112295,0.00437988)),
        cms.PSet(lowBounds = cms.vdouble(0.94,0,35), upBounds = cms.vdouble(999,1.5,40), values = cms.vdouble(1.00298), uncertainties = cms.vdouble(0.000422958,0.000386402)),
        cms.PSet(lowBounds = cms.vdouble(0.94,0,40), upBounds = cms.vdouble(999,1.5,45), values = cms.vdouble(1.00161), uncertainties = cms.vdouble(0.000257923,0.000257923)),
        cms.PSet(lowBounds = cms.vdouble(0.94,0,45), upBounds = cms.vdouble(999,1.5,50), values = cms.vdouble(1.00126), uncertainties = cms.vdouble(0.000302996,0.000261448)),
        cms.PSet(lowBounds = cms.vdouble(0.94,0,50), upBounds = cms.vdouble(999,1.5,60), values = cms.vdouble(1.00032), uncertainties = cms.vdouble(0.000250161,0.000321562)),
        cms.PSet(lowBounds = cms.vdouble(0.94,0,60), upBounds = cms.vdouble(999,1.5,70), values = cms.vdouble(1), uncertainties = cms.vdouble(0.00071693,0)),
        cms.PSet(lowBounds = cms.vdouble(0.94,0,70), upBounds = cms.vdouble(999,1.5,90), values = cms.vdouble(1), uncertainties = cms.vdouble(0.00145938,0)),
        cms.PSet(lowBounds = cms.vdouble(0.94,0,90), upBounds = cms.vdouble(999,1.5,120), values = cms.vdouble(1), uncertainties = cms.vdouble(0.0032519,0)),
        cms.PSet(lowBounds = cms.vdouble(0.94,0,120), upBounds = cms.vdouble(999,1.5,150), values = cms.vdouble(1), uncertainties = cms.vdouble(0.00797462,0)),
        cms.PSet(lowBounds = cms.vdouble(0.94,0,150), upBounds = cms.vdouble(999,1.5,10000), values = cms.vdouble(1), uncertainties = cms.vdouble(0.00930901,0)),
        
        #cat1
        cms.PSet(lowBounds = cms.vdouble(0,0,0), upBounds = cms.vdouble(0.94,1.5,30), values = cms.vdouble(0), uncertainties = cms.vdouble(0,0)),
        cms.PSet(lowBounds = cms.vdouble(0,0,30), upBounds = cms.vdouble(0.94,1.5,33.33), values = cms.vdouble(1.58157), uncertainties = cms.vdouble(0.0323024,0.0266563)),
        cms.PSet(lowBounds = cms.vdouble(0,0,33.33), upBounds = cms.vdouble(0.94,1.5,35), values = cms.vdouble(1.08074), uncertainties = cms.vdouble(0.00346232,0.00337425)),
        cms.PSet(lowBounds = cms.vdouble(0,0,35), upBounds = cms.vdouble(0.94,1.5,40), values = cms.vdouble(1.00994), uncertainties = cms.vdouble(0.000735422,0.000712876)),
        cms.PSet(lowBounds = cms.vdouble(0,0,40), upBounds = cms.vdouble(0.94,1.5,45), values = cms.vdouble(1.00442), uncertainties = cms.vdouble(0.000388044,0.000388044)),
        cms.PSet(lowBounds = cms.vdouble(0,0,45), upBounds = cms.vdouble(0.94,1.5,50), values = cms.vdouble(1.00306), uncertainties = cms.vdouble(0.000443947,0.000404669)),
        cms.PSet(lowBounds = cms.vdouble(0,0,50), upBounds = cms.vdouble(0.94,1.5,60), values = cms.vdouble(1.00289), uncertainties = cms.vdouble(0.000554998,0.000554998)),
        cms.PSet(lowBounds = cms.vdouble(0,0,60), upBounds = cms.vdouble(0.94,1.5,70), values = cms.vdouble(1.0037), uncertainties = cms.vdouble(0.00129343,0.00104829)),
        cms.PSet(lowBounds = cms.vdouble(0,0,70), upBounds = cms.vdouble(0.94,1.5,90), values = cms.vdouble(1.00251), uncertainties = cms.vdouble(0.00144813,0.00144813)),
        cms.PSet(lowBounds = cms.vdouble(0,0,90), upBounds = cms.vdouble(0.94,1.5,120), values = cms.vdouble(1), uncertainties = cms.vdouble(0.00334379,0)),
        cms.PSet(lowBounds = cms.vdouble(0,0,120), upBounds = cms.vdouble(0.94,1.5,150), values = cms.vdouble(1), uncertainties = cms.vdouble(0.0100245,0)),
        cms.PSet(lowBounds = cms.vdouble(0,0,150), upBounds = cms.vdouble(0.94,1.5,10000), values = cms.vdouble(1), uncertainties = cms.vdouble(0.0141262,0)),
        
        #cat2
        cms.PSet(lowBounds = cms.vdouble(0.94,1.5,0), upBounds = cms.vdouble(999,6,30), values = cms.vdouble(0), uncertainties = cms.vdouble(0,0)),
        cms.PSet(lowBounds = cms.vdouble(0.94,1.5,30), upBounds = cms.vdouble(999,6,33.33), values = cms.vdouble(1.00509), uncertainties = cms.vdouble(0.00261961,0.00509262)),
        cms.PSet(lowBounds = cms.vdouble(0.94,1.5,33.33), upBounds = cms.vdouble(999,6,35), values = cms.vdouble(1.00135), uncertainties = cms.vdouble(0,0.000516502)),
        cms.PSet(lowBounds = cms.vdouble(0.94,1.5,35), upBounds = cms.vdouble(999,6,40), values = cms.vdouble(1.00146), uncertainties = cms.vdouble(0.000750924,0.000558063)),
        cms.PSet(lowBounds = cms.vdouble(0.94,1.5,40), upBounds = cms.vdouble(999,6,45), values = cms.vdouble(1.00347), uncertainties = cms.vdouble(0.00141393,0.00141393)),
        cms.PSet(lowBounds = cms.vdouble(0.94,1.5,45), upBounds = cms.vdouble(999,6,50), values = cms.vdouble(1.0047), uncertainties = cms.vdouble(0.00241938,0.00179839)),
        cms.PSet(lowBounds = cms.vdouble(0.94,1.5,50), upBounds = cms.vdouble(999,6,60), values = cms.vdouble(1), uncertainties = cms.vdouble(0.00730364,0)),
        cms.PSet(lowBounds = cms.vdouble(0.94,1.5,60), upBounds = cms.vdouble(999,6,70), values = cms.vdouble(1), uncertainties = cms.vdouble(0.0114313,0)),
        cms.PSet(lowBounds = cms.vdouble(0.94,1.5,70), upBounds = cms.vdouble(999,6,90), values = cms.vdouble(1), uncertainties = cms.vdouble(0.0239662,0)),
        cms.PSet(lowBounds = cms.vdouble(0.94,1.5,90), upBounds = cms.vdouble(999,6,120), values = cms.vdouble(1), uncertainties = cms.vdouble(0.0521317,0)),
        cms.PSet(lowBounds = cms.vdouble(0.94,1.5,120), upBounds = cms.vdouble(999,6,10000), values = cms.vdouble(1), uncertainties = cms.vdouble(0.0806517,0)),
        
        #cat3
        cms.PSet(lowBounds = cms.vdouble(0,1.5,0), upBounds = cms.vdouble(0.94,6,30), values = cms.vdouble(0), uncertainties = cms.vdouble(0,0)),
        cms.PSet(lowBounds = cms.vdouble(0,1.5,30), upBounds = cms.vdouble(0.94,6,33.33), values = cms.vdouble(1.26181), uncertainties = cms.vdouble(0.0114529,0.0112355)),
        cms.PSet(lowBounds = cms.vdouble(0,1.5,33.33), upBounds = cms.vdouble(0.94,6,35), values = cms.vdouble(1.03664), uncertainties = cms.vdouble(0.00556093,0.00508205)),
        cms.PSet(lowBounds = cms.vdouble(0,1.5,35), upBounds = cms.vdouble(0.94,6,40), values = cms.vdouble(1.02482), uncertainties = cms.vdouble(0.00241796,0.00228534)),
        cms.PSet(lowBounds = cms.vdouble(0,1.5,40), upBounds = cms.vdouble(0.94,6,45), values = cms.vdouble(1.02614), uncertainties = cms.vdouble(0.00264867,0.00248845)),
        cms.PSet(lowBounds = cms.vdouble(0,1.5,45), upBounds = cms.vdouble(0.94,6,50), values = cms.vdouble(1.01822), uncertainties = cms.vdouble(0.00327081,0.00292662)),
        cms.PSet(lowBounds = cms.vdouble(0,1.5,50), upBounds = cms.vdouble(0.94,6,60), values = cms.vdouble(1.01306), uncertainties = cms.vdouble(0.00365737,0.00308069)),
        cms.PSet(lowBounds = cms.vdouble(0,1.5,60), upBounds = cms.vdouble(0.94,6,70), values = cms.vdouble(1.02967), uncertainties = cms.vdouble(0.0116915,0.00926521)),
        cms.PSet(lowBounds = cms.vdouble(0,1.5,70), upBounds = cms.vdouble(0.94,6,90), values = cms.vdouble(1.01579), uncertainties = cms.vdouble(0,0.00742504)),
        cms.PSet(lowBounds = cms.vdouble(0,1.5,90), upBounds = cms.vdouble(0.94,6,120), values = cms.vdouble(1.01296), uncertainties = cms.vdouble(0.0128499,0.0128499)),
        cms.PSet(lowBounds = cms.vdouble(0,1.5,120), upBounds = cms.vdouble(0.94,6,150), values = cms.vdouble(1), uncertainties = cms.vdouble(0.0808998,0)),
        cms.PSet(lowBounds = cms.vdouble(0,1.5,150), upBounds = cms.vdouble(0.94,6,10000), values = cms.vdouble(1), uncertainties = cms.vdouble(0.113917,0)),
        )
    )


subleadTriggerScaleBins = cms.PSet(
    variables = cms.vstring("r9","abs(superCluster.eta)","pt"),
    bins = cms.VPSet(
        #cat0
        cms.PSet(lowBounds = cms.vdouble(0.94,0,0), upBounds = cms.vdouble(999,1.5,30), values = cms.vdouble(0), uncertainties = cms.vdouble(0,0)),
        cms.PSet(lowBounds = cms.vdouble(0.94,0,20), upBounds = cms.vdouble(999,1.5,22.5), values = cms.vdouble(1.01844), uncertainties = cms.vdouble(0.00589303,0.00482249)),
        cms.PSet(lowBounds = cms.vdouble(0.94,0,22.5), upBounds = cms.vdouble(999,1.5,25), values = cms.vdouble(1.02051), uncertainties = cms.vdouble(0.00451484,0.00428102)),
        cms.PSet(lowBounds = cms.vdouble(0.94,0,25), upBounds = cms.vdouble(999,1.5,27.5), values = cms.vdouble(1.01067), uncertainties = cms.vdouble(0,0.00216708)),
        cms.PSet(lowBounds = cms.vdouble(0.94,0,27.5), upBounds = cms.vdouble(999,1.5,30), values = cms.vdouble(1.00687), uncertainties = cms.vdouble(0.00168729,0.00150298)),
        cms.PSet(lowBounds = cms.vdouble(0.94,0,30), upBounds = cms.vdouble(999,1.5,33.33), values = cms.vdouble(1.00633), uncertainties = cms.vdouble(0.00109777,0.000984921)),
        cms.PSet(lowBounds = cms.vdouble(0.94,0,33.33), upBounds = cms.vdouble(999,1.5,35), values = cms.vdouble(1.00433), uncertainties = cms.vdouble(0.00113761,0.000969282)),
        cms.PSet(lowBounds = cms.vdouble(0.94,0,35), upBounds = cms.vdouble(999,1.5,40), values = cms.vdouble(1.00307), uncertainties = cms.vdouble(0.000436662,0.000398903)),
        cms.PSet(lowBounds = cms.vdouble(0.94,0,40), upBounds = cms.vdouble(999,1.5,45), values = cms.vdouble(1.00156), uncertainties = cms.vdouble(0.000269855,0.00155738)),
        cms.PSet(lowBounds = cms.vdouble(0.94,0,45), upBounds = cms.vdouble(999,1.5,50), values = cms.vdouble(1.00128), uncertainties = cms.vdouble(0.000308592,0.000266496)),
        cms.PSet(lowBounds = cms.vdouble(0.94,0,50), upBounds = cms.vdouble(999,1.5,60), values = cms.vdouble(1.00035), uncertainties = cms.vdouble(0.000238305,0.000164471)),
        cms.PSet(lowBounds = cms.vdouble(0.94,0,60), upBounds = cms.vdouble(999,1.5,70), values = cms.vdouble(1), uncertainties = cms.vdouble(0.00078206,0)),
        cms.PSet(lowBounds = cms.vdouble(0.94,0,70), upBounds = cms.vdouble(999,1.5,90), values = cms.vdouble(1), uncertainties = cms.vdouble(0.00155038,0)),
        cms.PSet(lowBounds = cms.vdouble(0.94,0,90), upBounds = cms.vdouble(999,1.5,120), values = cms.vdouble(1), uncertainties = cms.vdouble(0.0033556,0)),
        cms.PSet(lowBounds = cms.vdouble(0.94,0,120), upBounds = cms.vdouble(999,1.5,150), values = cms.vdouble(1), uncertainties = cms.vdouble(0.00819353,0)),
        cms.PSet(lowBounds = cms.vdouble(0.94,0,150), upBounds = cms.vdouble(999,1.5,10000), values = cms.vdouble(1), uncertainties = cms.vdouble(0.00938149,0)),
        #cat1
        cms.PSet(lowBounds = cms.vdouble(0,0,0), upBounds = cms.vdouble(0.94,1.5,30), values = cms.vdouble(0), uncertainties = cms.vdouble(0,0)),
        cms.PSet(lowBounds = cms.vdouble(0,0,20), upBounds = cms.vdouble(0.94,1.5,22.5), values = cms.vdouble(1.14498), uncertainties = cms.vdouble(0.0124551,0.0120691)),
        cms.PSet(lowBounds = cms.vdouble(0,0,22.5), upBounds = cms.vdouble(0.94,1.5,25), values = cms.vdouble(1.04092), uncertainties = cms.vdouble(0.00485141,0.00463763)),
        cms.PSet(lowBounds = cms.vdouble(0,0,25), upBounds = cms.vdouble(0.94,1.5,27.5), values = cms.vdouble(1.01759), uncertainties = cms.vdouble(0.00298201,0.00368042)),
        cms.PSet(lowBounds = cms.vdouble(0,0,27.5), upBounds = cms.vdouble(0.94,1.5,30), values = cms.vdouble(1.01408), uncertainties = cms.vdouble(0.00204538,0.00204538)),
        cms.PSet(lowBounds = cms.vdouble(0,0,30), upBounds = cms.vdouble(0.94,1.5,33.33), values = cms.vdouble(1.0112), uncertainties = cms.vdouble(0.00107592,0.00101043)),
        cms.PSet(lowBounds = cms.vdouble(0,0,33.33), upBounds = cms.vdouble(0.94,1.5,35), values = cms.vdouble(1.00709), uncertainties = cms.vdouble(0.00108035,0.000981565)),
        cms.PSet(lowBounds = cms.vdouble(0,0,35), upBounds = cms.vdouble(0.94,1.5,40), values = cms.vdouble(1.00647), uncertainties = cms.vdouble(0.000514991,0.000489291)),
        cms.PSet(lowBounds = cms.vdouble(0,0,40), upBounds = cms.vdouble(0.94,1.5,45), values = cms.vdouble(1.00531), uncertainties = cms.vdouble(0.000427973,0.000427973)),
        cms.PSet(lowBounds = cms.vdouble(0,0,45), upBounds = cms.vdouble(0.94,1.5,50), values = cms.vdouble(1.00392), uncertainties = cms.vdouble(0.000505294,0.000465629)),
        cms.PSet(lowBounds = cms.vdouble(0,0,50), upBounds = cms.vdouble(0.94,1.5,60), values = cms.vdouble(1.0047), uncertainties = cms.vdouble(0.000762064,0.000687884)),
        cms.PSet(lowBounds = cms.vdouble(0,0,60), upBounds = cms.vdouble(0.94,1.5,70), values = cms.vdouble(1.00577), uncertainties = cms.vdouble(0.00174581,0.00145329)),
        cms.PSet(lowBounds = cms.vdouble(0,0,70), upBounds = cms.vdouble(0.94,1.5,90), values = cms.vdouble(1.00271), uncertainties = cms.vdouble(0.00187464,0.00127644)),
        cms.PSet(lowBounds = cms.vdouble(0,0,90), upBounds = cms.vdouble(0.94,1.5,120), values = cms.vdouble(1), uncertainties = cms.vdouble(0.00346301,0)),
        cms.PSet(lowBounds = cms.vdouble(0,0,120), upBounds = cms.vdouble(0.94,1.5,150), values = cms.vdouble(1), uncertainties = cms.vdouble(0.0100028,0)),
        cms.PSet(lowBounds = cms.vdouble(0,0,150), upBounds = cms.vdouble(0.94,1.5,10000), values = cms.vdouble(1), uncertainties = cms.vdouble(0.0154601,0)),
        #cat2
        cms.PSet(lowBounds = cms.vdouble(0.94,1.5,0), upBounds = cms.vdouble(999,6,30), values = cms.vdouble(0), uncertainties = cms.vdouble(0,0)),
        cms.PSet(lowBounds = cms.vdouble(0.94,1.5,20), upBounds = cms.vdouble(999,6,22.5), values = cms.vdouble(1), uncertainties = cms.vdouble(0.00181133,0.00181133)),
        cms.PSet(lowBounds = cms.vdouble(0.94,1.5,22.5), upBounds = cms.vdouble(999,6,25), values = cms.vdouble(1), uncertainties = cms.vdouble(0.0087868,6.95923e-08)),
        cms.PSet(lowBounds = cms.vdouble(0.94,1.5,25), upBounds = cms.vdouble(999,6,27.5), values = cms.vdouble(1), uncertainties = cms.vdouble(0.00280262,0)),
        cms.PSet(lowBounds = cms.vdouble(0.94,1.5,27.5), upBounds = cms.vdouble(999,6,30), values = cms.vdouble(1), uncertainties = cms.vdouble(0.00116971,0)),
        cms.PSet(lowBounds = cms.vdouble(0.94,1.5,30), upBounds = cms.vdouble(999,6,33.33), values = cms.vdouble(1), uncertainties = cms.vdouble(0.00107293,0)),
        cms.PSet(lowBounds = cms.vdouble(0.94,1.5,33.33), upBounds = cms.vdouble(999,6,35), values = cms.vdouble(1), uncertainties = cms.vdouble(0.00190493,0)),
        cms.PSet(lowBounds = cms.vdouble(0.94,1.5,35), upBounds = cms.vdouble(999,6,40), values = cms.vdouble(1.00055), uncertainties = cms.vdouble(0.000489846,0.000554099)),
        cms.PSet(lowBounds = cms.vdouble(0.94,1.5,40), upBounds = cms.vdouble(999,6,45), values = cms.vdouble(1.0003), uncertainties = cms.vdouble(0.000298933,0.000298933)),
        cms.PSet(lowBounds = cms.vdouble(0.94,1.5,45), upBounds = cms.vdouble(999,6,50), values = cms.vdouble(1), uncertainties = cms.vdouble(0.00114621,0)),
        cms.PSet(lowBounds = cms.vdouble(0.94,1.5,50), upBounds = cms.vdouble(999,6,60), values = cms.vdouble(1), uncertainties = cms.vdouble(0.00188188,0)),
        cms.PSet(lowBounds = cms.vdouble(0.94,1.5,60), upBounds = cms.vdouble(999,6,70), values = cms.vdouble(1), uncertainties = cms.vdouble(0.00829012,0)),
        cms.PSet(lowBounds = cms.vdouble(0.94,1.5,70), upBounds = cms.vdouble(999,6,90), values = cms.vdouble(1), uncertainties = cms.vdouble(0.0108213,0)),
        cms.PSet(lowBounds = cms.vdouble(0.94,1.5,90), upBounds = cms.vdouble(999,6,120), values = cms.vdouble(1), uncertainties = cms.vdouble(0.0243501,0)),
        cms.PSet(lowBounds = cms.vdouble(0.94,1.5,120), upBounds = cms.vdouble(999,6,150), values = cms.vdouble(1), uncertainties = cms.vdouble(0.0521317,0)),
        cms.PSet(lowBounds = cms.vdouble(0.94,1.5,150), upBounds = cms.vdouble(999,6,10000), values = cms.vdouble(1), uncertainties = cms.vdouble(0.0843269,0)),
        #cat3
        cms.PSet(lowBounds = cms.vdouble(0,1.5,0), upBounds = cms.vdouble(0.94,6,30), values = cms.vdouble(0), uncertainties = cms.vdouble(0,0)),
        cms.PSet(lowBounds = cms.vdouble(0,1.5,20), upBounds = cms.vdouble(0.94,6,22.5), values = cms.vdouble(1.06701), uncertainties = cms.vdouble(0.0208198,0.067009)),
        cms.PSet(lowBounds = cms.vdouble(0,1.5,22.5), upBounds = cms.vdouble(0.94,6,25), values = cms.vdouble(1.04158), uncertainties = cms.vdouble(0.00944754,0.0415832)),
        cms.PSet(lowBounds = cms.vdouble(0,1.5,25), upBounds = cms.vdouble(0.94,6,27.5), values = cms.vdouble(1.0382), uncertainties = cms.vdouble(0.0123807,0.038198)),
        cms.PSet(lowBounds = cms.vdouble(0,1.5,27.5), upBounds = cms.vdouble(0.94,6,30), values = cms.vdouble(1.03282), uncertainties = cms.vdouble(0.00826408,0.00635126)),
        cms.PSet(lowBounds = cms.vdouble(0,1.5,30), upBounds = cms.vdouble(0.94,6,33.33), values = cms.vdouble(1.02176), uncertainties = cms.vdouble(0.00348958,0.00357021)),
        cms.PSet(lowBounds = cms.vdouble(0,1.5,33.33), upBounds = cms.vdouble(0.94,6,35), values = cms.vdouble(1.02931), uncertainties = cms.vdouble(0.0051215,0.00459718)),
        cms.PSet(lowBounds = cms.vdouble(0,1.5,35), upBounds = cms.vdouble(0.94,6,40), values = cms.vdouble(1.02327), uncertainties = cms.vdouble(0.002339,0.00219548)),
        cms.PSet(lowBounds = cms.vdouble(0,1.5,40), upBounds = cms.vdouble(0.94,6,45), values = cms.vdouble(1.02589), uncertainties = cms.vdouble(0.00266628,0.00249925)),
        cms.PSet(lowBounds = cms.vdouble(0,1.5,45), upBounds = cms.vdouble(0.94,6,50), values = cms.vdouble(1.01956), uncertainties = cms.vdouble(0.00327475,0.00327475)),
        cms.PSet(lowBounds = cms.vdouble(0,1.5,50), upBounds = cms.vdouble(0.94,6,60), values = cms.vdouble(1.01009), uncertainties = cms.vdouble(0,0.00298873)),
        cms.PSet(lowBounds = cms.vdouble(0,1.5,60), upBounds = cms.vdouble(0.94,6,70), values = cms.vdouble(1.0289), uncertainties = cms.vdouble(0.0205071,0.0135909)),
        cms.PSet(lowBounds = cms.vdouble(0,1.5,70), upBounds = cms.vdouble(0.94,6,90), values = cms.vdouble(1.01204), uncertainties = cms.vdouble(0.00845749,0.00845749)),
        cms.PSet(lowBounds = cms.vdouble(0,1.5,90), upBounds = cms.vdouble(0.94,6,120), values = cms.vdouble(1.0135), uncertainties = cms.vdouble(0.0180314,0.00941194)),
        cms.PSet(lowBounds = cms.vdouble(0,1.5,120), upBounds = cms.vdouble(0.94,6,150), values = cms.vdouble(1), uncertainties = cms.vdouble(0.0848572,0)),
        cms.PSet(lowBounds = cms.vdouble(0,1.5,150), upBounds = cms.vdouble(0.94,6,10000), values = cms.vdouble(1), uncertainties = cms.vdouble(0.121587,0)),
        )
    )



# Slide 9 https://indico.cern.ch/event/464689/session/12/contribution/65/attachments/1204381/1754436/flashgg_14_12_2015.pdf
# N.B. for lose MVA cut SF the R9 boundary is at 0.94
looseMvaBins = cms.PSet(
    variables = cms.vstring("abs(superCluster.eta)","r9"),
    bins = cms.VPSet(
        cms.PSet( lowBounds = cms.vdouble( 0.0, 0.0  ) , upBounds = cms.vdouble( 1.5, 0.94  ) , values = cms.vdouble( 1.0012 ) , uncertainties = cms.vdouble( 0.0016 )  ) ,
        cms.PSet( lowBounds = cms.vdouble( 0.0, 0.94 ) , upBounds = cms.vdouble( 1.5, 999.0 ) , values = cms.vdouble( 0.9999 ) , uncertainties = cms.vdouble( 0.0005 )  ) ,
        cms.PSet( lowBounds = cms.vdouble( 1.5, 0.0  ) , upBounds = cms.vdouble( 6.0, 0.94  ) , values = cms.vdouble( 1.0018 ) , uncertainties = cms.vdouble( 0.0019 )  ) ,
        cms.PSet( lowBounds = cms.vdouble( 1.5, 0.94 ) , upBounds = cms.vdouble( 6.0, 999.0 ) , values = cms.vdouble( 0.9997 ) , uncertainties = cms.vdouble( 0.0008 )  ) 
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
		src = cms.InputTag("flashggDiPhotons"),
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
                                                  RandomLabel = cms.string("rnd_g_E"),
                                                  Debug = cms.untracked.bool(False),
                                                  ExaggerateShiftUp = cms.bool(False),
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
                                                  RandomLabel = cms.string("rnd_g_E"),
                                                  Debug = cms.untracked.bool(False),
                                                  ExaggerateShiftUp = cms.bool(False),
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
                                                  RandomLabel = cms.string("rnd_g_E"),
                                                  Debug = cms.untracked.bool(False),
                                                  ExaggerateShiftUp = cms.bool(False),
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
                                                  RandomLabel = cms.string("rnd_g_E"),
                                                  Debug = cms.untracked.bool(False),
                                                  ExaggerateShiftUp = cms.bool(False),
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
                                                  Label = cms.string("TriggerWeight"),
                                                  NSigmas = cms.vint32(-1,1),
                                                  OverallRange = cms.string("1"),
                                                  BinList = leadTriggerScaleBins,
                                                  BinList2 = subleadTriggerScaleBins,
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
