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
    variables = cms.vstring("abs(superCluster.eta)","r9"),
    bins = cms.VPSet(
        cms.PSet( lowBounds = cms.vdouble( 0.0, 0.0 ) , upBounds = cms.vdouble( 1.5, 0.85 ) , values = cms.vdouble( 0.9996 ) , uncertainties = cms.vdouble( 0.0140 )  ) ,
        cms.PSet( lowBounds = cms.vdouble( 0.0, 0.85 ), upBounds = cms.vdouble( 1.5, 999. ) , values = cms.vdouble( 1.008 ) , uncertainties = cms.vdouble( 0.0203 )  ) ,
        cms.PSet( lowBounds = cms.vdouble( 1.5, 0.0 ) , upBounds = cms.vdouble( 6.0, 0.9  ) , values = cms.vdouble( 0.9938 ) , uncertainties = cms.vdouble( 0.0175 )  ) ,
        cms.PSet( lowBounds = cms.vdouble( 1.5, 0.9 ) , upBounds = cms.vdouble( 6.0, 999. ) , values = cms.vdouble( 0.9969 ) , uncertainties = cms.vdouble( 0.0233 )  ) 
        )
    )

# slide 7 of https://indico.cern.ch/event/535800/contributions/2213640/attachments/1295950/1969805/201607ZmmgWith2016data_Update12p9perfb.pdf from the update with 12.9/fb and shower shape correction file v4
electronVetoBins = cms.PSet(
    variables = cms.vstring("abs(superCluster.eta)","r9"),
    bins = cms.VPSet(
        cms.PSet( lowBounds = cms.vdouble( 0.0, 0.00 ) , upBounds = cms.vdouble( 1.5, 0.85 ) , values = cms.vdouble( 0.9924 ) , uncertainties = cms.vdouble( 0.0045 )  ) ,
        cms.PSet( lowBounds = cms.vdouble( 0.0, 0.85 ) , upBounds = cms.vdouble( 1.5, 999. ) , values = cms.vdouble( 0.9960 ) , uncertainties = cms.vdouble( 0.0010 )  ) ,
        cms.PSet( lowBounds = cms.vdouble( 1.5, 0.00 ) , upBounds = cms.vdouble( 6.0, 0.90 ) , values = cms.vdouble( 0.9862 ) , uncertainties = cms.vdouble( 0.0137 )  ) ,
        cms.PSet( lowBounds = cms.vdouble( 1.5, 0.90 ) , upBounds = cms.vdouble( 6.0, 999. ) , values = cms.vdouble( 0.9999 ) , uncertainties = cms.vdouble( 0.0029 )  ) 
        )
    )

FNUFBins = cms.PSet(
    variables = cms.vstring("abs(superCluster.eta)","r9"),
    bins = cms.VPSet(
        cms.PSet( lowBounds = cms.vdouble( 0.0, 0.00 ) , upBounds = cms.vdouble( 1.5, 0.94 ) , values = cms.vdouble( 0. ) , uncertainties = cms.vdouble(  0.0007 )  ) ,
        cms.PSet( lowBounds = cms.vdouble( 0.0, 0.94 ) , upBounds = cms.vdouble( 1.5, 999. ) , values = cms.vdouble( 0. ) , uncertainties = cms.vdouble( -0.0007 )  ) ,
        cms.PSet( lowBounds = cms.vdouble( 1.5, 0.00 ) , upBounds = cms.vdouble( 6.0, 0.94 ) , values = cms.vdouble( 0. ) , uncertainties = cms.vdouble(  0.0007 )  ) ,
        cms.PSet( lowBounds = cms.vdouble( 1.5, 0.94 ) , upBounds = cms.vdouble( 6.0, 999. ) , values = cms.vdouble( 0. ) , uncertainties = cms.vdouble( -0.0007 )  )
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
        cms.PSet(lowBounds = cms.vdouble(0.0,0,0), upBounds = cms.vdouble(0.55,1.5,33.3333), values = cms.vdouble(0.775114), uncertainties = cms.vdouble(0.00600855,0.00600855)),
        cms.PSet(lowBounds = cms.vdouble(0.0,0,33.3333), upBounds = cms.vdouble(0.55,1.5,35), values = cms.vdouble(0.88532), uncertainties = cms.vdouble(0.00612793,0.00612793)),
        cms.PSet(lowBounds = cms.vdouble(0.0,0,35), upBounds = cms.vdouble(0.55,1.5,40), values = cms.vdouble(0.892203), uncertainties = cms.vdouble(0.00322536,0.00322536)),
        cms.PSet(lowBounds = cms.vdouble(0.0,0,40), upBounds = cms.vdouble(0.55,1.5,45), values = cms.vdouble(0.904842), uncertainties = cms.vdouble(0.00307669,0.00307669)),
        cms.PSet(lowBounds = cms.vdouble(0.0,0,45), upBounds = cms.vdouble(0.55,1.5,50), values = cms.vdouble(0.914859), uncertainties = cms.vdouble(0.00409752,0.00409752)),
        cms.PSet(lowBounds = cms.vdouble(0.0,0,50), upBounds = cms.vdouble(0.55,1.5,60), values = cms.vdouble(0.924156), uncertainties = cms.vdouble(0.00579445,0.00579445)),
        cms.PSet(lowBounds = cms.vdouble(0.0,0,60), upBounds = cms.vdouble(0.55,1.5,70), values = cms.vdouble(0.942922), uncertainties = cms.vdouble(0.0123039,0.0123039)),
        cms.PSet(lowBounds = cms.vdouble(0.0,0,70), upBounds = cms.vdouble(0.55,1.5,90), values = cms.vdouble(0.927536), uncertainties = cms.vdouble(0.0258482,0.0258482)),
        cms.PSet(lowBounds = cms.vdouble(0.0,0,90), upBounds = cms.vdouble(0.55,1.5,9999999), values = cms.vdouble(0.925926), uncertainties = cms.vdouble(0.0688613,0.0688613)),
        cms.PSet(lowBounds = cms.vdouble(0.55,0,0), upBounds = cms.vdouble(0.85,1.5,33.3333), values = cms.vdouble(0.878942), uncertainties = cms.vdouble(0.00108507,0.00108507)),
        cms.PSet(lowBounds = cms.vdouble(0.55,0,33.3333), upBounds = cms.vdouble(0.85,1.5,35), values = cms.vdouble(0.966379), uncertainties = cms.vdouble(0.000754105,0.000754105)),
        cms.PSet(lowBounds = cms.vdouble(0.55,0,35), upBounds = cms.vdouble(0.85,1.5,40), values = cms.vdouble(0.972522), uncertainties = cms.vdouble(0.000343941,0.000343941)),
        cms.PSet(lowBounds = cms.vdouble(0.55,0,40), upBounds = cms.vdouble(0.85,1.5,45), values = cms.vdouble(0.976961), uncertainties = cms.vdouble(0.000285283,0.000285283)),
        cms.PSet(lowBounds = cms.vdouble(0.55,0,45), upBounds = cms.vdouble(0.85,1.5,50), values = cms.vdouble(0.979303), uncertainties = cms.vdouble(0.000340352,0.000340352)),
        cms.PSet(lowBounds = cms.vdouble(0.55,0,50), upBounds = cms.vdouble(0.85,1.5,60), values = cms.vdouble(0.982115), uncertainties = cms.vdouble(0.00041993,0.00041993)),
        cms.PSet(lowBounds = cms.vdouble(0.55,0,60), upBounds = cms.vdouble(0.85,1.5,70), values = cms.vdouble(0.984479), uncertainties = cms.vdouble(0.000812822,0.000812822)),
        cms.PSet(lowBounds = cms.vdouble(0.55,0,70), upBounds = cms.vdouble(0.85,1.5,90), values = cms.vdouble(0.986807), uncertainties = cms.vdouble(0.00114881,0.00114881)),
        cms.PSet(lowBounds = cms.vdouble(0.55,0,90), upBounds = cms.vdouble(0.85,1.5,9999999), values = cms.vdouble(0.988418), uncertainties = cms.vdouble(0.00175583,0.00175583)),
        cms.PSet(lowBounds = cms.vdouble(0.85,0,0), upBounds = cms.vdouble(999,1.5,33.3333), values = cms.vdouble(0.91112), uncertainties = cms.vdouble(0.000468659,0.000468659)),
        cms.PSet(lowBounds = cms.vdouble(0.85,0,33.3333), upBounds = cms.vdouble(999,1.5,35), values = cms.vdouble(0.976805), uncertainties = cms.vdouble(0.000305267,0.000305267)),
        cms.PSet(lowBounds = cms.vdouble(0.85,0,35), upBounds = cms.vdouble(999,1.5,40), values = cms.vdouble(0.983143), uncertainties = cms.vdouble(0.000127084,0.000127084)),
        cms.PSet(lowBounds = cms.vdouble(0.85,0,40), upBounds = cms.vdouble(999,1.5,45), values = cms.vdouble(0.985977), uncertainties = cms.vdouble(9.92575e-05,9.92575e-05)),
        cms.PSet(lowBounds = cms.vdouble(0.85,0,45), upBounds = cms.vdouble(999,1.5,50), values = cms.vdouble(0.988068), uncertainties = cms.vdouble(0.000110239,0.000110239)),
        cms.PSet(lowBounds = cms.vdouble(0.85,0,50), upBounds = cms.vdouble(999,1.5,60), values = cms.vdouble(0.989167), uncertainties = cms.vdouble(0.000131794,0.000131794)),
        cms.PSet(lowBounds = cms.vdouble(0.85,0,60), upBounds = cms.vdouble(999,1.5,70), values = cms.vdouble(0.98897), uncertainties = cms.vdouble(0.000252297,0.000252297)),
        cms.PSet(lowBounds = cms.vdouble(0.85,0,70), upBounds = cms.vdouble(999,1.5,90), values = cms.vdouble(0.990893), uncertainties = cms.vdouble(0.000317489,0.000317489)),
        cms.PSet(lowBounds = cms.vdouble(0.85,0,90), upBounds = cms.vdouble(999,1.5,9999999), values = cms.vdouble(0.990964), uncertainties = cms.vdouble(0.000397677,0.000397677)),
        
        cms.PSet(lowBounds = cms.vdouble(0.0,1.5,0), upBounds = cms.vdouble(0.85,3.0,33.3333), values = cms.vdouble(0.679018), uncertainties = cms.vdouble(0.00591026,0.00591026)),
        cms.PSet(lowBounds = cms.vdouble(0.0,1.5,33.3333), upBounds = cms.vdouble(0.85,3.0,35), values = cms.vdouble(0.81687), uncertainties = cms.vdouble(0.00620355,0.00620355)),
        cms.PSet(lowBounds = cms.vdouble(0.0,1.5,35), upBounds = cms.vdouble(0.85,3.0,40), values = cms.vdouble(0.83901), uncertainties = cms.vdouble(0.00302377,0.00302377)),
        cms.PSet(lowBounds = cms.vdouble(0.0,1.5,40), upBounds = cms.vdouble(0.85,3.0,45), values = cms.vdouble(0.853319), uncertainties = cms.vdouble(0.00284136,0.00284136)),
        cms.PSet(lowBounds = cms.vdouble(0.0,1.5,45), upBounds = cms.vdouble(0.85,3.0,50), values = cms.vdouble(0.861865), uncertainties = cms.vdouble(0.00364834,0.00364834)),
        cms.PSet(lowBounds = cms.vdouble(0.0,1.5,50), upBounds = cms.vdouble(0.85,3.0,60), values = cms.vdouble(0.873447), uncertainties = cms.vdouble(0.00454297,0.00454297)),
        cms.PSet(lowBounds = cms.vdouble(0.0,1.5,60), upBounds = cms.vdouble(0.85,3.0,70), values = cms.vdouble(0.887509), uncertainties = cms.vdouble(0.00869211,0.00869211)),
        cms.PSet(lowBounds = cms.vdouble(0.0,1.5,70), upBounds = cms.vdouble(0.85,3.0,90), values = cms.vdouble(0.898403), uncertainties = cms.vdouble(0.0123003,0.0123003)),
        cms.PSet(lowBounds = cms.vdouble(0.0,1.5,90), upBounds = cms.vdouble(0.85,3.0,9999999), values = cms.vdouble(0.908284), uncertainties = cms.vdouble(0.0172694,0.0172694)),
        cms.PSet(lowBounds = cms.vdouble(0.85,1.5,0), upBounds = cms.vdouble(0.9,3.0,33.3333), values = cms.vdouble(0.786549), uncertainties = cms.vdouble(0.0037656,0.0037656)),
        cms.PSet(lowBounds = cms.vdouble(0.85,1.5,33.3333), upBounds = cms.vdouble(0.9,3.0,35), values = cms.vdouble(0.966016), uncertainties = cms.vdouble(0.0021447,0.0021447)),
        cms.PSet(lowBounds = cms.vdouble(0.85,1.5,35), upBounds = cms.vdouble(0.9,3.0,40), values = cms.vdouble(0.985321), uncertainties = cms.vdouble(0.000729027,0.000729027)),
        cms.PSet(lowBounds = cms.vdouble(0.85,1.5,40), upBounds = cms.vdouble(0.9,3.0,45), values = cms.vdouble(0.989327), uncertainties = cms.vdouble(0.000603978,0.000603978)),
        cms.PSet(lowBounds = cms.vdouble(0.85,1.5,45), upBounds = cms.vdouble(0.9,3.0,50), values = cms.vdouble(0.98988), uncertainties = cms.vdouble(0.000774284,0.000774284)),
        cms.PSet(lowBounds = cms.vdouble(0.85,1.5,50), upBounds = cms.vdouble(0.9,3.0,60), values = cms.vdouble(0.991248), uncertainties = cms.vdouble(0.000920685,0.000920685)),
        cms.PSet(lowBounds = cms.vdouble(0.85,1.5,60), upBounds = cms.vdouble(0.9,3.0,70), values = cms.vdouble(0.991908), uncertainties = cms.vdouble(0.00182805,0.00182805)),
        cms.PSet(lowBounds = cms.vdouble(0.85,1.5,70), upBounds = cms.vdouble(0.9,3.0,90), values = cms.vdouble(0.994417), uncertainties = cms.vdouble(0.00219212,0.00219212)),
        cms.PSet(lowBounds = cms.vdouble(0.85,1.5,90), upBounds = cms.vdouble(0.9,3.0,9999999), values = cms.vdouble(0.996711), uncertainties = cms.vdouble(0.00250067,0.00250067)),
        cms.PSet(lowBounds = cms.vdouble(0.9,1.5,0), upBounds = cms.vdouble(999,3.0,33.3333), values = cms.vdouble(0.712497), uncertainties = cms.vdouble(0.00124044,0.00124044)),
        cms.PSet(lowBounds = cms.vdouble(0.9,1.5,33.3333), upBounds = cms.vdouble(999,3.0,35), values = cms.vdouble(0.952991), uncertainties = cms.vdouble(0.000751837,0.000751837)),
        cms.PSet(lowBounds = cms.vdouble(0.9,1.5,35), upBounds = cms.vdouble(999,3.0,40), values = cms.vdouble(0.981648), uncertainties = cms.vdouble(0.000254865,0.000254865)),
        cms.PSet(lowBounds = cms.vdouble(0.9,1.5,40), upBounds = cms.vdouble(999,3.0,45), values = cms.vdouble(0.987275), uncertainties = cms.vdouble(0.000210871,0.000210871)),
        cms.PSet(lowBounds = cms.vdouble(0.9,1.5,45), upBounds = cms.vdouble(999,3.0,50), values = cms.vdouble(0.988334), uncertainties = cms.vdouble(0.000265264,0.000265264)),
        cms.PSet(lowBounds = cms.vdouble(0.9,1.5,50), upBounds = cms.vdouble(999,3.0,60), values = cms.vdouble(0.989925), uncertainties = cms.vdouble(0.000315163,0.000315163)),
        cms.PSet(lowBounds = cms.vdouble(0.9,1.5,60), upBounds = cms.vdouble(999,3.0,70), values = cms.vdouble(0.992184), uncertainties = cms.vdouble(0.00055368,0.00055368)),
        cms.PSet(lowBounds = cms.vdouble(0.9,1.5,70), upBounds = cms.vdouble(999,3.0,90), values = cms.vdouble(0.993519), uncertainties = cms.vdouble(0.000678988,0.000678988)),
        cms.PSet(lowBounds = cms.vdouble(0.9,1.5,90), upBounds = cms.vdouble(999,3.0,9999999), values = cms.vdouble(0.995242), uncertainties = cms.vdouble(0.000723744,0.000723744)),
        
        cms.PSet(lowBounds = cms.vdouble(0.0,3.,0), upBounds = cms.vdouble(999,999,999999), values = cms.vdouble(1.), uncertainties = cms.vdouble(1.,1.))
        )
    )


subleadTriggerScaleBins = cms.PSet(
    variables = cms.vstring("r9","abs(superCluster.eta)","pt"),
    bins = cms.VPSet(
        cms.PSet(lowBounds = cms.vdouble(0.0,0,0), upBounds = cms.vdouble(0.55,1.5,22.5), values = cms.vdouble(0.856223), uncertainties = cms.vdouble(0.00979298,0.00979298)),
        cms.PSet(lowBounds = cms.vdouble(0.0,0,22.5), upBounds = cms.vdouble(0.55,1.5,25), values = cms.vdouble(0.879531), uncertainties = cms.vdouble(0.00782373,0.00782373)),
        cms.PSet(lowBounds = cms.vdouble(0.0,0,25), upBounds = cms.vdouble(0.55,1.5,27.5), values = cms.vdouble(0.889777), uncertainties = cms.vdouble(0.00666975,0.00666975)),
        cms.PSet(lowBounds = cms.vdouble(0.0,0,27.5), upBounds = cms.vdouble(0.55,1.5,30), values = cms.vdouble(0.8999), uncertainties = cms.vdouble(0.00568016,0.00568016)),
        cms.PSet(lowBounds = cms.vdouble(0.0,0,30), upBounds = cms.vdouble(0.55,1.5,33.3333), values = cms.vdouble(0.910071), uncertainties = cms.vdouble(0.00420541,0.00420541)),
        cms.PSet(lowBounds = cms.vdouble(0.0,0,33.3333), upBounds = cms.vdouble(0.55,1.5,35), values = cms.vdouble(0.916902), uncertainties = cms.vdouble(0.00539697,0.00539697)),
        cms.PSet(lowBounds = cms.vdouble(0.0,0,35), upBounds = cms.vdouble(0.55,1.5,40), values = cms.vdouble(0.919239), uncertainties = cms.vdouble(0.00287004,0.00287004)),
        cms.PSet(lowBounds = cms.vdouble(0.0,0,40), upBounds = cms.vdouble(0.55,1.5,45), values = cms.vdouble(0.930018), uncertainties = cms.vdouble(0.00270958,0.00270958)),
        cms.PSet(lowBounds = cms.vdouble(0.0,0,45), upBounds = cms.vdouble(0.55,1.5,50), values = cms.vdouble(0.941432), uncertainties = cms.vdouble(0.00350092,0.00350092)),
        cms.PSet(lowBounds = cms.vdouble(0.0,0,50), upBounds = cms.vdouble(0.55,1.5,60), values = cms.vdouble(0.945578), uncertainties = cms.vdouble(0.00508646,0.00508646)),
        cms.PSet(lowBounds = cms.vdouble(0.0,0,60), upBounds = cms.vdouble(0.55,1.5,70), values = cms.vdouble(0.954545), uncertainties = cms.vdouble(0.0114646,0.0114646)),
        cms.PSet(lowBounds = cms.vdouble(0.0,0,70), upBounds = cms.vdouble(0.55,1.5,90), values = cms.vdouble(0.938462), uncertainties = cms.vdouble(0.025101,0.025101)),
        cms.PSet(lowBounds = cms.vdouble(0.0,0,90), upBounds = cms.vdouble(0.55,1.5,9999999), values = cms.vdouble(0.923077), uncertainties = cms.vdouble(0.071344,0.071344)),
        cms.PSet(lowBounds = cms.vdouble(0.55,0,0), upBounds = cms.vdouble(0.85,1.5,22.5), values = cms.vdouble(0.976199), uncertainties = cms.vdouble(0.00124463,0.00124463)),
        cms.PSet(lowBounds = cms.vdouble(0.55,0,22.5), upBounds = cms.vdouble(0.85,1.5,25), values = cms.vdouble(0.986182), uncertainties = cms.vdouble(0.000783178,0.000783178)),
        cms.PSet(lowBounds = cms.vdouble(0.55,0,25), upBounds = cms.vdouble(0.85,1.5,27.5), values = cms.vdouble(0.989449), uncertainties = cms.vdouble(0.000569396,0.000569396)),
        cms.PSet(lowBounds = cms.vdouble(0.55,0,27.5), upBounds = cms.vdouble(0.85,1.5,30), values = cms.vdouble(0.992121), uncertainties = cms.vdouble(0.000416989,0.000416989)),
        cms.PSet(lowBounds = cms.vdouble(0.55,0,30), upBounds = cms.vdouble(0.85,1.5,33.3333), values = cms.vdouble(0.994007), uncertainties = cms.vdouble(0.000265174,0.000265174)),
        cms.PSet(lowBounds = cms.vdouble(0.55,0,33.3333), upBounds = cms.vdouble(0.85,1.5,35), values = cms.vdouble(0.995227), uncertainties = cms.vdouble(0.000298071,0.000298071)),
        cms.PSet(lowBounds = cms.vdouble(0.55,0,35), upBounds = cms.vdouble(0.85,1.5,40), values = cms.vdouble(0.995363), uncertainties = cms.vdouble(0.00014614,0.00014614)),
        cms.PSet(lowBounds = cms.vdouble(0.55,0,40), upBounds = cms.vdouble(0.85,1.5,45), values = cms.vdouble(0.995721), uncertainties = cms.vdouble(0.000126645,0.000126645)),
        cms.PSet(lowBounds = cms.vdouble(0.55,0,45), upBounds = cms.vdouble(0.85,1.5,50), values = cms.vdouble(0.995166), uncertainties = cms.vdouble(0.000169403,0.000169403)),
        cms.PSet(lowBounds = cms.vdouble(0.55,0,50), upBounds = cms.vdouble(0.85,1.5,60), values = cms.vdouble(0.99457), uncertainties = cms.vdouble(0.000238974,0.000238974)),
        cms.PSet(lowBounds = cms.vdouble(0.55,0,60), upBounds = cms.vdouble(0.85,1.5,70), values = cms.vdouble(0.994133), uncertainties = cms.vdouble(0.000524288,0.000524288)),
        cms.PSet(lowBounds = cms.vdouble(0.55,0,70), upBounds = cms.vdouble(0.85,1.5,90), values = cms.vdouble(0.992971), uncertainties = cms.vdouble(0.000879743,0.000879743)),
        cms.PSet(lowBounds = cms.vdouble(0.55,0,90), upBounds = cms.vdouble(0.85,1.5,9999999), values = cms.vdouble(0.991319), uncertainties = cms.vdouble(0.00157224,0.00157224)),
        cms.PSet(lowBounds = cms.vdouble(0.85,0,0), upBounds = cms.vdouble(999,1.5,22.5), values = cms.vdouble(0.983968), uncertainties = cms.vdouble(0.00049477,0.00049477)),
        cms.PSet(lowBounds = cms.vdouble(0.85,0,22.5), upBounds = cms.vdouble(999,1.5,25), values = cms.vdouble(0.990318), uncertainties = cms.vdouble(0.000326358,0.000326358)),
        cms.PSet(lowBounds = cms.vdouble(0.85,0,25), upBounds = cms.vdouble(999,1.5,27.5), values = cms.vdouble(0.99455), uncertainties = cms.vdouble(0.000207224,0.000207224)),
        cms.PSet(lowBounds = cms.vdouble(0.85,0,27.5), upBounds = cms.vdouble(999,1.5,30), values = cms.vdouble(0.996122), uncertainties = cms.vdouble(0.000147981,0.000147981)),
        cms.PSet(lowBounds = cms.vdouble(0.85,0,30), upBounds = cms.vdouble(999,1.5,33.3333), values = cms.vdouble(0.997719), uncertainties = cms.vdouble(8.10655e-05,8.10655e-05)),
        cms.PSet(lowBounds = cms.vdouble(0.85,0,33.3333), upBounds = cms.vdouble(999,1.5,35), values = cms.vdouble(0.998429), uncertainties = cms.vdouble(8.31577e-05,8.31577e-05)),
        cms.PSet(lowBounds = cms.vdouble(0.85,0,35), upBounds = cms.vdouble(999,1.5,40), values = cms.vdouble(0.998859), uncertainties = cms.vdouble(3.41767e-05,3.41767e-05)),
        cms.PSet(lowBounds = cms.vdouble(0.85,0,40), upBounds = cms.vdouble(999,1.5,45), values = cms.vdouble(0.999347), uncertainties = cms.vdouble(2.21294e-05,2.21294e-05)),
        cms.PSet(lowBounds = cms.vdouble(0.85,0,45), upBounds = cms.vdouble(999,1.5,50), values = cms.vdouble(0.99953), uncertainties = cms.vdouble(2.27352e-05,2.27352e-05)),
        cms.PSet(lowBounds = cms.vdouble(0.85,0,50), upBounds = cms.vdouble(999,1.5,60), values = cms.vdouble(0.999667), uncertainties = cms.vdouble(2.43059e-05,2.43059e-05)),
        cms.PSet(lowBounds = cms.vdouble(0.85,0,60), upBounds = cms.vdouble(999,1.5,70), values = cms.vdouble(0.999679), uncertainties = cms.vdouble(4.70166e-05,4.70166e-05)),
        cms.PSet(lowBounds = cms.vdouble(0.85,0,70), upBounds = cms.vdouble(999,1.5,90), values = cms.vdouble(0.999707), uncertainties = cms.vdouble(6.36726e-05,6.36726e-05)),
        cms.PSet(lowBounds = cms.vdouble(0.85,0,90), upBounds = cms.vdouble(999,1.5,9999999), values = cms.vdouble(0.999774), uncertainties = cms.vdouble(7.212e-05,7.212e-05)),
        
        cms.PSet(lowBounds = cms.vdouble(0.0,1.5,0), upBounds = cms.vdouble(0.85,3.0,22.5), values = cms.vdouble(0.786797), uncertainties = cms.vdouble(0.0140851,0.0140851)),
        cms.PSet(lowBounds = cms.vdouble(0.0,1.5,22.5), upBounds = cms.vdouble(0.85,3.0,25), values = cms.vdouble(0.802244), uncertainties = cms.vdouble(0.0109551,0.0109551)),
        cms.PSet(lowBounds = cms.vdouble(0.0,1.5,25), upBounds = cms.vdouble(0.85,3.0,27.5), values = cms.vdouble(0.81586), uncertainties = cms.vdouble(0.00847315,0.00847315)),
        cms.PSet(lowBounds = cms.vdouble(0.0,1.5,27.5), upBounds = cms.vdouble(0.85,3.0,30), values = cms.vdouble(0.827889), uncertainties = cms.vdouble(0.00675266,0.00675266)),
        cms.PSet(lowBounds = cms.vdouble(0.0,1.5,30), upBounds = cms.vdouble(0.85,3.0,33.3333), values = cms.vdouble(0.831283), uncertainties = cms.vdouble(0.00484,0.00484)),
        cms.PSet(lowBounds = cms.vdouble(0.0,1.5,33.3333), upBounds = cms.vdouble(0.85,3.0,35), values = cms.vdouble(0.835983), uncertainties = cms.vdouble(0.00604094,0.00604094)),
        cms.PSet(lowBounds = cms.vdouble(0.0,1.5,35), upBounds = cms.vdouble(0.85,3.0,40), values = cms.vdouble(0.846593), uncertainties = cms.vdouble(0.00300429,0.00300429)),
        cms.PSet(lowBounds = cms.vdouble(0.0,1.5,40), upBounds = cms.vdouble(0.85,3.0,45), values = cms.vdouble(0.857106), uncertainties = cms.vdouble(0.00284788,0.00284788)),
        cms.PSet(lowBounds = cms.vdouble(0.0,1.5,45), upBounds = cms.vdouble(0.85,3.0,50), values = cms.vdouble(0.864572), uncertainties = cms.vdouble(0.0036748,0.0036748)),
        cms.PSet(lowBounds = cms.vdouble(0.0,1.5,50), upBounds = cms.vdouble(0.85,3.0,60), values = cms.vdouble(0.875115), uncertainties = cms.vdouble(0.00460394,0.00460394)),
        cms.PSet(lowBounds = cms.vdouble(0.0,1.5,60), upBounds = cms.vdouble(0.85,3.0,70), values = cms.vdouble(0.88824), uncertainties = cms.vdouble(0.00892828,0.00892828)),
        cms.PSet(lowBounds = cms.vdouble(0.0,1.5,70), upBounds = cms.vdouble(0.85,3.0,90), values = cms.vdouble(0.896024), uncertainties = cms.vdouble(0.0127668,0.0127668)),
        cms.PSet(lowBounds = cms.vdouble(0.0,1.5,90), upBounds = cms.vdouble(0.85,3.0,9999999), values = cms.vdouble(0.901538), uncertainties = cms.vdouble(0.018158,0.018158)),
        cms.PSet(lowBounds = cms.vdouble(0.85,1.5,0), upBounds = cms.vdouble(0.9,3.0,22.5), values = cms.vdouble(0.972328), uncertainties = cms.vdouble(0.00411822,0.00411822)),
        cms.PSet(lowBounds = cms.vdouble(0.85,1.5,22.5), upBounds = cms.vdouble(0.9,3.0,25), values = cms.vdouble(0.989023), uncertainties = cms.vdouble(0.00219214,0.00219214)),
        cms.PSet(lowBounds = cms.vdouble(0.85,1.5,25), upBounds = cms.vdouble(0.9,3.0,27.5), values = cms.vdouble(0.990974), uncertainties = cms.vdouble(0.00158747,0.00158747)),
        cms.PSet(lowBounds = cms.vdouble(0.85,1.5,27.5), upBounds = cms.vdouble(0.9,3.0,30), values = cms.vdouble(0.991866), uncertainties = cms.vdouble(0.00123518,0.00123518)),
        cms.PSet(lowBounds = cms.vdouble(0.85,1.5,30), upBounds = cms.vdouble(0.9,3.0,33.3333), values = cms.vdouble(0.994503), uncertainties = cms.vdouble(0.000726915,0.000726915)),
        cms.PSet(lowBounds = cms.vdouble(0.85,1.5,33.3333), upBounds = cms.vdouble(0.9,3.0,35), values = cms.vdouble(0.994357), uncertainties = cms.vdouble(0.000941941,0.000941941)),
        cms.PSet(lowBounds = cms.vdouble(0.85,1.5,35), upBounds = cms.vdouble(0.9,3.0,40), values = cms.vdouble(0.994772), uncertainties = cms.vdouble(0.000450467,0.000450467)),
        cms.PSet(lowBounds = cms.vdouble(0.85,1.5,40), upBounds = cms.vdouble(0.9,3.0,45), values = cms.vdouble(0.99502), uncertainties = cms.vdouble(0.000424732,0.000424732)),
        cms.PSet(lowBounds = cms.vdouble(0.85,1.5,45), upBounds = cms.vdouble(0.9,3.0,50), values = cms.vdouble(0.995574), uncertainties = cms.vdouble(0.000531705,0.000531705)),
        cms.PSet(lowBounds = cms.vdouble(0.85,1.5,50), upBounds = cms.vdouble(0.9,3.0,60), values = cms.vdouble(0.995177), uncertainties = cms.vdouble(0.000711163,0.000711163)),
        cms.PSet(lowBounds = cms.vdouble(0.85,1.5,60), upBounds = cms.vdouble(0.9,3.0,70), values = cms.vdouble(0.993546), uncertainties = cms.vdouble(0.00171057,0.00171057)),
        cms.PSet(lowBounds = cms.vdouble(0.85,1.5,70), upBounds = cms.vdouble(0.9,3.0,90), values = cms.vdouble(0.995398), uncertainties = cms.vdouble(0.00209281,0.00209281)),
        cms.PSet(lowBounds = cms.vdouble(0.85,1.5,90), upBounds = cms.vdouble(0.9,3.0,9999999), values = cms.vdouble(0.993151), uncertainties = cms.vdouble(0.00340703,0.00340703)),
        cms.PSet(lowBounds = cms.vdouble(0.9,1.5,0), upBounds = cms.vdouble(999,3.0,22.5), values = cms.vdouble(0.980932), uncertainties = cms.vdouble(0.000838197,0.000838197)),
        cms.PSet(lowBounds = cms.vdouble(0.9,1.5,22.5), upBounds = cms.vdouble(999,3.0,25), values = cms.vdouble(0.995857), uncertainties = cms.vdouble(0.000342926,0.000342926)),
        cms.PSet(lowBounds = cms.vdouble(0.9,1.5,25), upBounds = cms.vdouble(999,3.0,27.5), values = cms.vdouble(0.997887), uncertainties = cms.vdouble(0.000208258,0.000208258)),
        cms.PSet(lowBounds = cms.vdouble(0.9,1.5,27.5), upBounds = cms.vdouble(999,3.0,30), values = cms.vdouble(0.998436), uncertainties = cms.vdouble(0.00015416,0.00015416)),
        cms.PSet(lowBounds = cms.vdouble(0.9,1.5,30), upBounds = cms.vdouble(999,3.0,33.3333), values = cms.vdouble(0.998723), uncertainties = cms.vdouble(0.000103196,0.000103196)),
        cms.PSet(lowBounds = cms.vdouble(0.9,1.5,33.3333), upBounds = cms.vdouble(999,3.0,35), values = cms.vdouble(0.999228), uncertainties = cms.vdouble(0.000105837,0.000105837)),
        cms.PSet(lowBounds = cms.vdouble(0.9,1.5,35), upBounds = cms.vdouble(999,3.0,40), values = cms.vdouble(0.999385), uncertainties = cms.vdouble(4.92599e-05,4.92599e-05)),
        cms.PSet(lowBounds = cms.vdouble(0.9,1.5,40), upBounds = cms.vdouble(999,3.0,45), values = cms.vdouble(0.999617), uncertainties = cms.vdouble(3.88079e-05,3.88079e-05)),
        cms.PSet(lowBounds = cms.vdouble(0.9,1.5,45), upBounds = cms.vdouble(999,3.0,50), values = cms.vdouble(0.999592), uncertainties = cms.vdouble(5.32127e-05,5.32127e-05)),
        cms.PSet(lowBounds = cms.vdouble(0.9,1.5,50), upBounds = cms.vdouble(999,3.0,60), values = cms.vdouble(0.999622), uncertainties = cms.vdouble(6.67433e-05,6.67433e-05)),
        cms.PSet(lowBounds = cms.vdouble(0.9,1.5,60), upBounds = cms.vdouble(999,3.0,70), values = cms.vdouble(0.999571), uncertainties = cms.vdouble(0.000150493,0.000150493)),
        cms.PSet(lowBounds = cms.vdouble(0.9,1.5,70), upBounds = cms.vdouble(999,3.0,90), values = cms.vdouble(0.999525), uncertainties = cms.vdouble(0.000216609,0.000216609)),
        cms.PSet(lowBounds = cms.vdouble(0.9,1.5,90), upBounds = cms.vdouble(999,3.0,9999999), values = cms.vdouble(0.999603), uncertainties = cms.vdouble(0.000253108,0.000253108)),
        
        cms.PSet(lowBounds = cms.vdouble(0.0,3.,0), upBounds = cms.vdouble(999,999,999999), values = cms.vdouble(1.), uncertainties = cms.vdouble(1.,1.))
        )
    )


# from Arnab via Martina 24/07/2016                                                                                                                               
looseMvaBins = cms.PSet(
    variables = cms.vstring("abs(superCluster.eta)","r9"),
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

materialBinsICHEP = cms.PSet(
    variables = cms.vstring("abs(superCluster.eta)","r9"),
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

scalesAndSmearingsPrefix = cms.string("EgammaAnalysis/ElectronTools/data/80X_ichepV2_2016_pho")
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
