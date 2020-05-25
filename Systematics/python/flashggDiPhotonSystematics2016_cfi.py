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

# JTao: slide 40 of https://indico.cern.ch/event/850506/contributions/3606914/attachments/1927946/3192290/201910_Zmmg_RhoBugFixed.pdf with 2016 legacy data and MC samples, with new shower-shape and isolation corrections applied and photon PT > 20 GeV 
electronVetoBins = cms.PSet(
    variables = cms.vstring("abs(superCluster.eta)","full5x5_r9"),
    bins = cms.VPSet(
        cms.PSet( lowBounds = cms.vdouble( 0.0, 0.00 ) , upBounds = cms.vdouble( 1.5, 0.85 ) , values = cms.vdouble( 0.9973 ) , uncertainties = cms.vdouble( 0.0024 )  ) ,
        cms.PSet( lowBounds = cms.vdouble( 0.0, 0.85 ) , upBounds = cms.vdouble( 1.5, 999. ) , values = cms.vdouble( 0.9963 ) , uncertainties = cms.vdouble( 0.0006 )  ) ,
        cms.PSet( lowBounds = cms.vdouble( 1.5, 0.00 ) , upBounds = cms.vdouble( 6.0, 0.90 ) , values = cms.vdouble( 0.9700 ) , uncertainties = cms.vdouble( 0.0075 )  ) ,
        cms.PSet( lowBounds = cms.vdouble( 1.5, 0.90 ) , upBounds = cms.vdouble( 6.0, 999. ) , values = cms.vdouble( 0.9909 ) , uncertainties = cms.vdouble( 0.0018 )  ) 
        )
    )


# Based on the updated derivation by N. Schroeder for HIG-19-004. Re-evaluated in coarse Eta-R9 bins
FNUFBins = cms.PSet(
    variables = cms.vstring("abs(superCluster.eta)","full5x5_r9"),
    bins = cms.VPSet(
        cms.PSet( lowBounds = cms.vdouble( 0.0, 0.00 ) , upBounds = cms.vdouble( 1.5, 0.94 ) , values = cms.vdouble( 0. ) , uncertainties = cms.vdouble(  0.00044 )  ) ,
        cms.PSet( lowBounds = cms.vdouble( 0.0, 0.94 ) , upBounds = cms.vdouble( 1.5, 999. ) , values = cms.vdouble( 0. ) , uncertainties = cms.vdouble(  0.00156 )  ) ,
        cms.PSet( lowBounds = cms.vdouble( 1.5, 0.00 ) , upBounds = cms.vdouble( 6.0, 0.94 ) , values = cms.vdouble( 0. ) , uncertainties = cms.vdouble(  0.00003 )  ) ,
        cms.PSet( lowBounds = cms.vdouble( 1.5, 0.94 ) , upBounds = cms.vdouble( 6.0, 999. ) , values = cms.vdouble( 0. ) , uncertainties = cms.vdouble(  0.00165 )  )
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


#for full 2016 Legacy dataset.  Trigger scale factors for use without HLT applied in MC

leadTriggerScaleBins = cms.PSet(
    variables = cms.vstring("full5x5_r9","abs(superCluster.eta)","pt"),
    bins = cms.VPSet(

        #seeded leg, R9 turn-on (0.50-0.54), cat2, barrel
        cms.PSet(lowBounds = cms.vdouble(0.0,0.0,0.0), upBounds = cms.vdouble(0.54,1.5,33.3333),  values = cms.vdouble(0.59825812412), uncertainties = cms.vdouble(0.00402725234934, 0.00402725234934)),
        cms.PSet(lowBounds = cms.vdouble(0.0,0.0,33.3333), upBounds = cms.vdouble(0.54,1.5,35.0), values = cms.vdouble(0.653756261564), uncertainties = cms.vdouble(0.0099055319024, 0.0099055319024)),
        cms.PSet(lowBounds = cms.vdouble(0.0,0.0,35.0), upBounds = cms.vdouble(0.54,1.5,40.0),    values = cms.vdouble(0.683431664312), uncertainties = cms.vdouble(0.00554805412963, 0.00554805412963)),
        cms.PSet(lowBounds = cms.vdouble(0.0,0.0,40.0), upBounds = cms.vdouble(0.54,1.5,45.0),    values = cms.vdouble(0.710022589532), uncertainties = cms.vdouble(0.00532044177457, 0.00532044177457)),
        cms.PSet(lowBounds = cms.vdouble(0.0,0.0,45.0), upBounds = cms.vdouble(0.54,1.5,50.0),    values = cms.vdouble(0.730570707514), uncertainties = cms.vdouble(0.00371231458923, 0.00371231458923)),
        cms.PSet(lowBounds = cms.vdouble(0.0,0.0,50.0), upBounds = cms.vdouble(0.54,1.5,60.0),    values = cms.vdouble(0.751603284508), uncertainties = cms.vdouble(0.0096076672755, 0.0096076672755)),
        cms.PSet(lowBounds = cms.vdouble(0.0,0.0,60.0), upBounds = cms.vdouble(0.54,1.5,70.0),    values = cms.vdouble(0.788168324479), uncertainties = cms.vdouble(0.0224762460948, 0.0224762460948)),
        cms.PSet(lowBounds = cms.vdouble(0.0,0.0,70.0), upBounds = cms.vdouble(0.54,1.5,90.0),    values = cms.vdouble(0.827941857065), uncertainties = cms.vdouble(0.0252561959982, 0.0252561959982)),
        cms.PSet(lowBounds = cms.vdouble(0.0,0.0,90.0), upBounds = cms.vdouble(0.54,1.5,99999999),values = cms.vdouble(0.783051501808), uncertainties = cms.vdouble(0.0585125120425, 0.0585125120425)),
        
        #seeded leg, R9 plateau (0.54-0.85), cat2, barrel
        cms.PSet(lowBounds = cms.vdouble(0.54,0.0,0.0), upBounds = cms.vdouble(0.85,1.5,33.3333),  values = cms.vdouble(0.895998413691), uncertainties = cms.vdouble(0.001, 0.001)),
        cms.PSet(lowBounds = cms.vdouble(0.54,0.0,33.3333), upBounds = cms.vdouble(0.85,1.5,35.0), values = cms.vdouble(0.953053304133), uncertainties = cms.vdouble(0.001, 0.001)),
        cms.PSet(lowBounds = cms.vdouble(0.54,0.0,35.0), upBounds = cms.vdouble(0.85,1.5,40.0),    values = cms.vdouble(0.963043242302), uncertainties = cms.vdouble(0.001, 0.001)),
        cms.PSet(lowBounds = cms.vdouble(0.54,0.0,40.0), upBounds = cms.vdouble(0.85,1.5,45.0),    values = cms.vdouble(0.968461390346), uncertainties = cms.vdouble(0.001, 0.001)),
        cms.PSet(lowBounds = cms.vdouble(0.54,0.0,45.0), upBounds = cms.vdouble(0.85,1.5,50.0),    values = cms.vdouble(0.972040191424), uncertainties = cms.vdouble(0.001, 0.001)),
        cms.PSet(lowBounds = cms.vdouble(0.54,0.0,50.0), upBounds = cms.vdouble(0.85,1.5,60.0),    values = cms.vdouble(0.974762665083), uncertainties = cms.vdouble(0.001, 0.001)),
        cms.PSet(lowBounds = cms.vdouble(0.54,0.0,60.0), upBounds = cms.vdouble(0.85,1.5,70.0),    values = cms.vdouble(0.980444525098), uncertainties = cms.vdouble(0.00109371653612, 0.00109371653612)),
        cms.PSet(lowBounds = cms.vdouble(0.54,0.0,70.0), upBounds = cms.vdouble(0.85,1.5,90.0),    values = cms.vdouble(0.985215654176), uncertainties = cms.vdouble(0.001, 0.001)),
        cms.PSet(lowBounds = cms.vdouble(0.54,0.0,90.0), upBounds = cms.vdouble(0.85,1.5,99999999),values = cms.vdouble(0.990441809012), uncertainties = cms.vdouble(0.00116568181032, 0.00116568181032)),

        #seeded leg, cat0 (R9>0.85), barrel
        cms.PSet(lowBounds = cms.vdouble(0.85,0.0,0.0), upBounds = cms.vdouble(999,1.5,33.3333),  values = cms.vdouble(0.913564877899), uncertainties = cms.vdouble(0.001, 0.001)),
        cms.PSet(lowBounds = cms.vdouble(0.85,0.0,33.3333), upBounds = cms.vdouble(999,1.5,35.0), values = cms.vdouble(0.970947331321), uncertainties = cms.vdouble(0.001, 0.001)),
        cms.PSet(lowBounds = cms.vdouble(0.85,0.0,35.0), upBounds = cms.vdouble(999,1.5,40.0),    values = cms.vdouble(0.976527980646), uncertainties = cms.vdouble(0.001, 0.001)),
        cms.PSet(lowBounds = cms.vdouble(0.85,0.0,40.0), upBounds = cms.vdouble(999,1.5,45.0),    values = cms.vdouble(0.981419518931), uncertainties = cms.vdouble(0.001, 0.001)),
        cms.PSet(lowBounds = cms.vdouble(0.85,0.0,45.0), upBounds = cms.vdouble(999,1.5,50.0),    values = cms.vdouble(0.985854458442), uncertainties = cms.vdouble(0.001, 0.001)),
        cms.PSet(lowBounds = cms.vdouble(0.85,0.0,50.0), upBounds = cms.vdouble(999,1.5,60.0),    values = cms.vdouble(0.986833495137), uncertainties = cms.vdouble(0.001, 0.001)),
        cms.PSet(lowBounds = cms.vdouble(0.85,0.0,60.0), upBounds = cms.vdouble(999,1.5,70.0),    values = cms.vdouble(0.985750212762), uncertainties = cms.vdouble(0.001, 0.001)),
        cms.PSet(lowBounds = cms.vdouble(0.85,0.0,70.0), upBounds = cms.vdouble(999,1.5,90.0),    values = cms.vdouble(0.989860818686), uncertainties = cms.vdouble(0.001, 0.001)),
        cms.PSet(lowBounds = cms.vdouble(0.85,0.0,90.0), upBounds = cms.vdouble(999,1.5,99999999),values = cms.vdouble(0.992375919183), uncertainties = cms.vdouble(0.001, 0.001)),
       
        #seeded leg, R9 turn-on bin (0.8-0.84), cat3, endcaps
        cms.PSet(lowBounds = cms.vdouble(0.0,1.5,0.0), upBounds = cms.vdouble(0.84,3.0,33.3333),   values = cms.vdouble(0.420726348915), uncertainties = cms.vdouble(0.00329881518128, 0.00329881518128)),
        cms.PSet(lowBounds = cms.vdouble(0.0,1.5,33.3333), upBounds = cms.vdouble(0.84,3.0,35.0),  values = cms.vdouble(0.502663676545), uncertainties = cms.vdouble(0.00845707569877, 0.00845707569877)),
        cms.PSet(lowBounds = cms.vdouble(0.0,1.5,35.0), upBounds = cms.vdouble(0.84,3.0,40.0),     values = cms.vdouble(0.540375613026), uncertainties = cms.vdouble(0.00805181819008, 0.00805181819008)),
        cms.PSet(lowBounds = cms.vdouble(0.0,1.5,40.0), upBounds = cms.vdouble(0.84,3.0,45.0),     values = cms.vdouble(0.583997232374), uncertainties = cms.vdouble(0.00236124137414, 0.00236124137414)),
        cms.PSet(lowBounds = cms.vdouble(0.0,1.5,45.0), upBounds = cms.vdouble(0.84,3.0,50.0),     values = cms.vdouble(0.634241018525), uncertainties = cms.vdouble(0.00478050171836, 0.00478050171836)),
        cms.PSet(lowBounds = cms.vdouble(0.0,1.5,50.0), upBounds = cms.vdouble(0.84,3.0,60.0),     values = cms.vdouble(0.681745539531), uncertainties = cms.vdouble(0.0108745570348, 0.0108745570348)),
        cms.PSet(lowBounds = cms.vdouble(0.0,1.5,60.0), upBounds = cms.vdouble(0.84,3.0,70.0),     values = cms.vdouble(0.691025346542), uncertainties = cms.vdouble(0.0152948033429, 0.0152948033429)),
        cms.PSet(lowBounds = cms.vdouble(0.0,1.5,70.0), upBounds = cms.vdouble(0.84,3.0,90.0),     values = cms.vdouble(0.693154551833), uncertainties = cms.vdouble(0.0155980381218, 0.0155980381218)),
        cms.PSet(lowBounds = cms.vdouble(0.0,1.5,90.0), upBounds = cms.vdouble(0.84,3.0,99999999), values = cms.vdouble(0.781237964132), uncertainties = cms.vdouble(0.0189850019066, 0.0189850019066)),

                
        #seeded leg, R9 plateau bin (0.84-0.9), cat3, endcaps
        cms.PSet(lowBounds = cms.vdouble(0.84,1.5,0.0), upBounds = cms.vdouble(0.9,3.0,33.3333),   values = cms.vdouble(0.855415306414), uncertainties = cms.vdouble(0.00164562364632, 0.00164562364632)),
        cms.PSet(lowBounds = cms.vdouble(0.84,1.5,33.3333), upBounds = cms.vdouble(0.9,3.0,35.0),  values = cms.vdouble(0.941868329313), uncertainties = cms.vdouble(0.00136563428604, 0.00136563428604)),
        cms.PSet(lowBounds = cms.vdouble(0.84,1.5,35.0), upBounds = cms.vdouble(0.9,3.0,40.0),     values = cms.vdouble(0.9702138178), uncertainties = cms.vdouble(0.001, 0.001)),
        cms.PSet(lowBounds = cms.vdouble(0.84,1.5,40.0), upBounds = cms.vdouble(0.9,3.0,45.0),     values = cms.vdouble(0.980593509643), uncertainties = cms.vdouble(0.001, 0.001)),
        cms.PSet(lowBounds = cms.vdouble(0.84,1.5,45.0), upBounds = cms.vdouble(0.9,3.0,50.0),     values = cms.vdouble(0.982254272023), uncertainties = cms.vdouble(0.001, 0.001)),
        cms.PSet(lowBounds = cms.vdouble(0.84,1.5,50.0), upBounds = cms.vdouble(0.9,3.0,60.0),     values = cms.vdouble(0.980124144279), uncertainties = cms.vdouble(0.00169012504277, 0.00169012504277)),
        cms.PSet(lowBounds = cms.vdouble(0.84,1.5,60.0), upBounds = cms.vdouble(0.9,3.0,70.0),     values = cms.vdouble(0.991947425994), uncertainties = cms.vdouble(0.00182442812301, 0.00182442812301)),
        cms.PSet(lowBounds = cms.vdouble(0.84,1.5,70.0), upBounds = cms.vdouble(0.9,3.0,90.0),     values = cms.vdouble(0.993083773686), uncertainties = cms.vdouble(0.0037048730698, 0.0037048730698)),
        cms.PSet(lowBounds = cms.vdouble(0.84,1.5,90.0), upBounds = cms.vdouble(0.9,3.0,99999999), values = cms.vdouble(0.996949860244), uncertainties = cms.vdouble(0.00118896445727, 0.00118896445727)),
        
        
        #seeded leg, cat1 (R9>0.90), endcaps
        cms.PSet(lowBounds = cms.vdouble(0.9,1.5,0.0), upBounds = cms.vdouble(999,3.0,33.3333),   values = cms.vdouble(0.820965272234), uncertainties = cms.vdouble(0.00204918413177, 0.00204918413177)),
        cms.PSet(lowBounds = cms.vdouble(0.9,1.5,33.3333), upBounds = cms.vdouble(999,3.0,35.0),  values = cms.vdouble(0.957238991239), uncertainties = cms.vdouble(0.001, 0.001)),
        cms.PSet(lowBounds = cms.vdouble(0.9,1.5,35.0), upBounds = cms.vdouble(999,3.0,40.0),     values = cms.vdouble(0.978884202189), uncertainties = cms.vdouble(0.001, 0.001)),
        cms.PSet(lowBounds = cms.vdouble(0.9,1.5,40.0), upBounds = cms.vdouble(999,3.0,45.0),     values = cms.vdouble(0.983109457831), uncertainties = cms.vdouble(0.001, 0.001)),
        cms.PSet(lowBounds = cms.vdouble(0.9,1.5,45.0), upBounds = cms.vdouble(999,3.0,50.0),     values = cms.vdouble(0.983013877197), uncertainties = cms.vdouble(0.001, 0.001)),
        cms.PSet(lowBounds = cms.vdouble(0.9,1.5,50.0), upBounds = cms.vdouble(999,3.0,60.0),     values = cms.vdouble(0.981541934213), uncertainties = cms.vdouble(0.00117705799271, 0.00117705799271)),
        cms.PSet(lowBounds = cms.vdouble(0.9,1.5,60.0), upBounds = cms.vdouble(999,3.0,70.0),     values = cms.vdouble(0.990797373585), uncertainties = cms.vdouble(0.00140759611697, 0.00140759611697)),
        cms.PSet(lowBounds = cms.vdouble(0.9,1.5,70.0), upBounds = cms.vdouble(999,3.0,90.0),     values = cms.vdouble(0.993913709497), uncertainties = cms.vdouble(0.00103928573661, 0.00103928573661)),
        cms.PSet(lowBounds = cms.vdouble(0.9,1.5,90.0), upBounds = cms.vdouble(999,3.0,99999999), values = cms.vdouble(0.995338862975), uncertainties = cms.vdouble(0.001, 0.001)),
        
        
        cms.PSet(lowBounds = cms.vdouble(0.0,3.,0), upBounds = cms.vdouble(999,999,999999),     values = cms.vdouble(1.), uncertainties = cms.vdouble(1.,1.))
       )
   )


subleadTriggerScaleBins = cms.PSet(
    variables = cms.vstring("full5x5_r9","abs(superCluster.eta)","pt"),
    bins = cms.VPSet(

        #unseeded leg, R9 turn-on (0.50-0.54), cat2, barrel
        cms.PSet(lowBounds = cms.vdouble(0.0,0.0,0.0), upBounds = cms.vdouble(0.54,1.5,22.5),      values = cms.vdouble(0.603067311807), uncertainties = cms.vdouble(0.00796165968882, 0.00796165968882)),
        cms.PSet(lowBounds = cms.vdouble(0.0,0.0,22.5), upBounds = cms.vdouble(0.54,1.5,25.0),     values = cms.vdouble(0.624492850153), uncertainties = cms.vdouble(0.00926893494893, 0.00926893494893)),
        cms.PSet(lowBounds = cms.vdouble(0.0,0.0,25.0), upBounds = cms.vdouble(0.54,1.5,27.5),     values = cms.vdouble(0.648788979569), uncertainties = cms.vdouble(0.00643750002229, 0.00643750002229)),
        cms.PSet(lowBounds = cms.vdouble(0.0,0.0,27.5), upBounds = cms.vdouble(0.54,1.5,30.0),     values = cms.vdouble(0.654133439859), uncertainties = cms.vdouble(0.00560733004707, 0.00560733004707)),
        cms.PSet(lowBounds = cms.vdouble(0.0,0.0,30.0), upBounds = cms.vdouble(0.54,1.5,32.5),     values = cms.vdouble(0.676589314263), uncertainties = cms.vdouble(0.00478261673364, 0.00478261673364)),
        cms.PSet(lowBounds = cms.vdouble(0.0,0.0,32.5), upBounds = cms.vdouble(0.54,1.5,35.0),     values = cms.vdouble(0.683202863128), uncertainties = cms.vdouble(0.00602767084288, 0.00602767084288)),
        cms.PSet(lowBounds = cms.vdouble(0.0,0.0,35.0), upBounds = cms.vdouble(0.54,1.5,40.0),     values = cms.vdouble(0.704606499829), uncertainties = cms.vdouble(0.00491050256218, 0.00491050256218)),
        cms.PSet(lowBounds = cms.vdouble(0.0,0.0,40.0), upBounds = cms.vdouble(0.54,1.5,45.0),     values = cms.vdouble(0.732923798151), uncertainties = cms.vdouble(0.00526686396656, 0.00526686396656)),
        cms.PSet(lowBounds = cms.vdouble(0.0,0.0,45.0), upBounds = cms.vdouble(0.54,1.5,50.0),     values = cms.vdouble(0.755447011273), uncertainties = cms.vdouble(0.00360816189604, 0.00360816189604)),
        cms.PSet(lowBounds = cms.vdouble(0.0,0.0,50.0), upBounds = cms.vdouble(0.54,1.5,60.0),     values = cms.vdouble(0.770751155231), uncertainties = cms.vdouble(0.00941141936066, 0.00941141936066)),
        cms.PSet(lowBounds = cms.vdouble(0.0,0.0,60.0), upBounds = cms.vdouble(0.54,1.5,70.0),     values = cms.vdouble(0.790198511256), uncertainties = cms.vdouble(0.0221609073824, 0.0221609073824)),
        cms.PSet(lowBounds = cms.vdouble(0.0,0.0,70.0), upBounds = cms.vdouble(0.54,1.5,90.0),     values = cms.vdouble(0.833694375128), uncertainties = cms.vdouble(0.0245645104497, 0.0245645104497)),
        cms.PSet(lowBounds = cms.vdouble(0.0,0.0,90.0), upBounds = cms.vdouble(0.54,1.5,99999999), values = cms.vdouble(0.795746634733), uncertainties = cms.vdouble(0.061430899873, 0.061430899873)),
        
        #unseeded leg, R9 plateau (0.54-0.85), cat2, barrel
        cms.PSet(lowBounds = cms.vdouble(0.54,0.0,0.0), upBounds = cms.vdouble(0.85,1.5,22.5),      values = cms.vdouble(0.985693698514), uncertainties = cms.vdouble(0.001, 0.001)),
        cms.PSet(lowBounds = cms.vdouble(0.54,0.0,22.5), upBounds = cms.vdouble(0.85,1.5,25.0),     values = cms.vdouble(0.98720784946), uncertainties = cms.vdouble(0.001, 0.001)),
        cms.PSet(lowBounds = cms.vdouble(0.54,0.0,25.0), upBounds = cms.vdouble(0.85,1.5,27.5),     values = cms.vdouble(0.990136850966), uncertainties = cms.vdouble(0.001, 0.001)),
        cms.PSet(lowBounds = cms.vdouble(0.54,0.0,27.5), upBounds = cms.vdouble(0.85,1.5,30.0),     values = cms.vdouble(0.990703680436), uncertainties = cms.vdouble(0.001, 0.001)),
        cms.PSet(lowBounds = cms.vdouble(0.54,0.0,30.0), upBounds = cms.vdouble(0.85,1.5,32.5),     values = cms.vdouble(0.991401108045), uncertainties = cms.vdouble(0.001, 0.001)),
        cms.PSet(lowBounds = cms.vdouble(0.54,0.0,32.5), upBounds = cms.vdouble(0.85,1.5,35.0),     values = cms.vdouble(0.991139606368), uncertainties = cms.vdouble(0.001, 0.001)),
        cms.PSet(lowBounds = cms.vdouble(0.54,0.0,35.0), upBounds = cms.vdouble(0.85,1.5,40.0),     values = cms.vdouble(0.990594834177), uncertainties = cms.vdouble(0.001, 0.001)),
        cms.PSet(lowBounds = cms.vdouble(0.54,0.0,40.0), upBounds = cms.vdouble(0.85,1.5,45.0),     values = cms.vdouble(0.992264476341), uncertainties = cms.vdouble(0.001, 0.001)),
        cms.PSet(lowBounds = cms.vdouble(0.54,0.0,45.0), upBounds = cms.vdouble(0.85,1.5,50.0),     values = cms.vdouble(0.99215428945), uncertainties = cms.vdouble(0.001, 0.001)),
        cms.PSet(lowBounds = cms.vdouble(0.54,0.0,50.0), upBounds = cms.vdouble(0.85,1.5,60.0),     values = cms.vdouble(0.991657615297), uncertainties = cms.vdouble(0.001, 0.001)),
        cms.PSet(lowBounds = cms.vdouble(0.54,0.0,60.0), upBounds = cms.vdouble(0.85,1.5,70.0),     values = cms.vdouble(0.992376937273), uncertainties = cms.vdouble(0.001, 0.001)),
        cms.PSet(lowBounds = cms.vdouble(0.54,0.0,70.0), upBounds = cms.vdouble(0.85,1.5,90.0),     values = cms.vdouble(0.99132752396), uncertainties = cms.vdouble(0.001, 0.001)),
        cms.PSet(lowBounds = cms.vdouble(0.54,0.0,90.0), upBounds = cms.vdouble(0.85,1.5,99999999), values = cms.vdouble(0.992327711897), uncertainties = cms.vdouble(0.001, 0.001)),

        #unseeded leg, cat0 (R9>0.85) barrel
        cms.PSet(lowBounds = cms.vdouble(0.85,0.0,0.0), upBounds = cms.vdouble(999,1.5,22.5),     values = cms.vdouble(0.991761096067), uncertainties = cms.vdouble(0.00255160879609, 0.00255160879609)),
        cms.PSet(lowBounds = cms.vdouble(0.85,0.0,22.5), upBounds = cms.vdouble(999,1.5,25.0),    values = cms.vdouble(0.99209301576), uncertainties = cms.vdouble(0.001, 0.001)),
        cms.PSet(lowBounds = cms.vdouble(0.85,0.0,25.0), upBounds = cms.vdouble(999,1.5,27.5),    values = cms.vdouble(0.995650862874), uncertainties = cms.vdouble(0.001, 0.001)),
        cms.PSet(lowBounds = cms.vdouble(0.85,0.0,27.5), upBounds = cms.vdouble(999,1.5,30.0),    values = cms.vdouble(0.997325955916), uncertainties = cms.vdouble(0.001, 0.001)),
        cms.PSet(lowBounds = cms.vdouble(0.85,0.0,30.0), upBounds = cms.vdouble(999,1.5,32.5),    values = cms.vdouble(0.99777623069), uncertainties = cms.vdouble(0.001, 0.001)),
        cms.PSet(lowBounds = cms.vdouble(0.85,0.0,32.5), upBounds = cms.vdouble(999,1.5,35.0),    values = cms.vdouble(0.998034021176), uncertainties = cms.vdouble(0.001, 0.001)),
        cms.PSet(lowBounds = cms.vdouble(0.85,0.0,35.0), upBounds = cms.vdouble(999,1.5,40.0),    values = cms.vdouble(0.998504649133), uncertainties = cms.vdouble(0.001, 0.001)),
        cms.PSet(lowBounds = cms.vdouble(0.85,0.0,40.0), upBounds = cms.vdouble(999,1.5,45.0),    values = cms.vdouble(0.999197866395), uncertainties = cms.vdouble(0.001, 0.001)),
        cms.PSet(lowBounds = cms.vdouble(0.85,0.0,45.0), upBounds = cms.vdouble(999,1.5,50.0),    values = cms.vdouble(0.999484680768), uncertainties = cms.vdouble(0.001, 0.001)),
        cms.PSet(lowBounds = cms.vdouble(0.85,0.0,50.0), upBounds = cms.vdouble(999,1.5,60.0),    values = cms.vdouble(0.999525695621), uncertainties = cms.vdouble(0.001, 0.001)),
        cms.PSet(lowBounds = cms.vdouble(0.85,0.0,60.0), upBounds = cms.vdouble(999,1.5,70.0),    values = cms.vdouble(0.999489148388), uncertainties = cms.vdouble(0.001, 0.001)),
        cms.PSet(lowBounds = cms.vdouble(0.85,0.0,70.0), upBounds = cms.vdouble(999,1.5,90.0),    values = cms.vdouble(0.999586900113), uncertainties = cms.vdouble(0.001, 0.001)),
        cms.PSet(lowBounds = cms.vdouble(0.85,0.0,90.0), upBounds = cms.vdouble(999,1.5,99999999),values = cms.vdouble(0.99976876111), uncertainties = cms.vdouble(0.001, 0.001)),
        
        
         
        
        #unseeded leg, R9 turn-on (0.80-0.84), cat3, endcaps
        cms.PSet(lowBounds = cms.vdouble(0.0,1.5,0.0), upBounds = cms.vdouble(0.84,3.0,22.5),     values = cms.vdouble(0.397904659653), uncertainties = cms.vdouble(0.0103701256472, 0.0103701256472)),
        cms.PSet(lowBounds = cms.vdouble(0.0,1.5,22.5), upBounds = cms.vdouble(0.84,3.0,25.0),    values = cms.vdouble(0.476338280641), uncertainties = cms.vdouble(0.00724663100975, 0.00724663100975)),
        cms.PSet(lowBounds = cms.vdouble(0.0,1.5,25.0), upBounds = cms.vdouble(0.84,3.0,27.5),    values = cms.vdouble(0.476760079505), uncertainties = cms.vdouble(0.00662897317704, 0.00662897317704)),
        cms.PSet(lowBounds = cms.vdouble(0.0,1.5,27.5), upBounds = cms.vdouble(0.84,3.0,30.0),    values = cms.vdouble(0.492456742258), uncertainties = cms.vdouble(0.00559388321773, 0.00559388321773)),
        cms.PSet(lowBounds = cms.vdouble(0.0,1.5,30.0), upBounds = cms.vdouble(0.84,3.0,32.5),    values = cms.vdouble(0.499574658857), uncertainties = cms.vdouble(0.00489237082551, 0.00489237082551)),
        cms.PSet(lowBounds = cms.vdouble(0.0,1.5,32.5), upBounds = cms.vdouble(0.84,3.0,35.0),    values = cms.vdouble(0.514883766738), uncertainties = cms.vdouble(0.00461060196901, 0.00461060196901)),
        cms.PSet(lowBounds = cms.vdouble(0.0,1.5,35.0), upBounds = cms.vdouble(0.84,3.0,40.0),    values = cms.vdouble(0.546928353821), uncertainties = cms.vdouble(0.00676410575364, 0.00676410575364)),
        cms.PSet(lowBounds = cms.vdouble(0.0,1.5,40.0), upBounds = cms.vdouble(0.84,3.0,45.0),    values = cms.vdouble(0.586218267133), uncertainties = cms.vdouble(0.00238871302656, 0.00238871302656)),
        cms.PSet(lowBounds = cms.vdouble(0.0,1.5,45.0), upBounds = cms.vdouble(0.84,3.0,50.0),    values = cms.vdouble(0.634948129085), uncertainties = cms.vdouble(0.00486082592408, 0.00486082592408)),
        cms.PSet(lowBounds = cms.vdouble(0.0,1.5,50.0), upBounds = cms.vdouble(0.84,3.0,60.0),    values = cms.vdouble(0.683344012046), uncertainties = cms.vdouble(0.0109524874021, 0.0109524874021)),
        cms.PSet(lowBounds = cms.vdouble(0.0,1.5,60.0), upBounds = cms.vdouble(0.84,3.0,70.0),    values = cms.vdouble(0.687893457258), uncertainties = cms.vdouble(0.0154297645218, 0.0154297645218)),
        cms.PSet(lowBounds = cms.vdouble(0.0,1.5,70.0), upBounds = cms.vdouble(0.84,3.0,90.0),    values = cms.vdouble(0.69733866744), uncertainties = cms.vdouble(0.0157135517974, 0.0157135517974)),
        cms.PSet(lowBounds = cms.vdouble(0.0,1.5,90.0), upBounds = cms.vdouble(0.84,3.0,99999999),values = cms.vdouble(0.77691019609), uncertainties = cms.vdouble(0.0192625793772, 0.0192625793772)),

        
        #unseeded leg, R9 plateau (0.84-0.90), cat3, endcaps
        cms.PSet(lowBounds = cms.vdouble(0.84,1.5,0.0), upBounds = cms.vdouble(0.9,3.0,22.5),      values = cms.vdouble(0.914440546843), uncertainties = cms.vdouble(0.00369877727173, 0.00369877727173)),
        cms.PSet(lowBounds = cms.vdouble(0.84,1.5,22.5), upBounds = cms.vdouble(0.9,3.0,25.0),     values = cms.vdouble(0.946240309449), uncertainties = cms.vdouble(0.00276212111018, 0.00276212111018)),
        cms.PSet(lowBounds = cms.vdouble(0.84,1.5,25.0), upBounds = cms.vdouble(0.9,3.0,27.5),     values = cms.vdouble(0.957590085724), uncertainties = cms.vdouble(0.00203378796898, 0.00203378796898)),
        cms.PSet(lowBounds = cms.vdouble(0.84,1.5,27.5), upBounds = cms.vdouble(0.9,3.0,30.0),     values = cms.vdouble(0.96673997202), uncertainties = cms.vdouble(0.00164416878122, 0.00164416878122)),
        cms.PSet(lowBounds = cms.vdouble(0.84,1.5,30.0), upBounds = cms.vdouble(0.9,3.0,32.5),     values = cms.vdouble(0.969644566245), uncertainties = cms.vdouble(0.00109672123756, 0.00109672123756)),
        cms.PSet(lowBounds = cms.vdouble(0.84,1.5,32.5), upBounds = cms.vdouble(0.9,3.0,35.0),     values = cms.vdouble(0.97205519699), uncertainties = cms.vdouble(0.001, 0.001)),
        cms.PSet(lowBounds = cms.vdouble(0.84,1.5,35.0), upBounds = cms.vdouble(0.9,3.0,40.0),     values = cms.vdouble(0.979950203254), uncertainties = cms.vdouble(0.001, 0.001)),
        cms.PSet(lowBounds = cms.vdouble(0.84,1.5,40.0), upBounds = cms.vdouble(0.9,3.0,45.0),     values = cms.vdouble(0.986317913523), uncertainties = cms.vdouble(0.001, 0.001)),
        cms.PSet(lowBounds = cms.vdouble(0.84,1.5,45.0), upBounds = cms.vdouble(0.9,3.0,50.0),     values = cms.vdouble(0.989066835191), uncertainties = cms.vdouble(0.001, 0.001)),
        cms.PSet(lowBounds = cms.vdouble(0.84,1.5,50.0), upBounds = cms.vdouble(0.9,3.0,60.0),     values = cms.vdouble(0.989234906512), uncertainties = cms.vdouble(0.00150791913682, 0.00150791913682)),
        cms.PSet(lowBounds = cms.vdouble(0.84,1.5,60.0), upBounds = cms.vdouble(0.9,3.0,70.0),     values = cms.vdouble(0.987547467088), uncertainties = cms.vdouble(0.00216103269916, 0.00216103269916)),
        cms.PSet(lowBounds = cms.vdouble(0.84,1.5,70.0), upBounds = cms.vdouble(0.9,3.0,90.0),     values = cms.vdouble(0.993179409024), uncertainties = cms.vdouble(0.00192881095814, 0.00192881095814)),
        cms.PSet(lowBounds = cms.vdouble(0.84,1.5,90.0), upBounds = cms.vdouble(0.9,3.0,99999999), values = cms.vdouble(0.99280484038), uncertainties = cms.vdouble(0.00178969381893, 0.00178969381893)),

        
        #unseeded leg, cat1 (R9>0,90), endcaps
        cms.PSet(lowBounds = cms.vdouble(0.9,1.5,0.0), upBounds = cms.vdouble(999,3.0,22.5),      values = cms.vdouble(0.979742764312), uncertainties = cms.vdouble(0.00125602429117, 0.00125602429117)),
        cms.PSet(lowBounds = cms.vdouble(0.9,1.5,22.5), upBounds = cms.vdouble(999,3.0,25.0),     values = cms.vdouble(0.995787652356), uncertainties = cms.vdouble(0.001, 0.001)),
        cms.PSet(lowBounds = cms.vdouble(0.9,1.5,25.0), upBounds = cms.vdouble(999,3.0,27.5),     values = cms.vdouble(0.996953828564), uncertainties = cms.vdouble(0.001, 0.001)),
        cms.PSet(lowBounds = cms.vdouble(0.9,1.5,27.5), upBounds = cms.vdouble(999,3.0,30.0),     values = cms.vdouble(0.997628232725), uncertainties = cms.vdouble(0.00136058215658, 0.00136058215658)),
        cms.PSet(lowBounds = cms.vdouble(0.9,1.5,30.0), upBounds = cms.vdouble(999,3.0,32.5),     values = cms.vdouble(0.996152491005), uncertainties = cms.vdouble(0.001, 0.001)),
        cms.PSet(lowBounds = cms.vdouble(0.9,1.5,32.5), upBounds = cms.vdouble(999,3.0,35.0),     values = cms.vdouble(0.997929487158), uncertainties = cms.vdouble(0.001, 0.001)),
        cms.PSet(lowBounds = cms.vdouble(0.9,1.5,35.0), upBounds = cms.vdouble(999,3.0,40.0),     values = cms.vdouble(0.998542785924), uncertainties = cms.vdouble(0.001, 0.001)),
        cms.PSet(lowBounds = cms.vdouble(0.9,1.5,40.0), upBounds = cms.vdouble(999,3.0,45.0),     values = cms.vdouble(0.998890613687), uncertainties = cms.vdouble(0.001, 0.001)),
        cms.PSet(lowBounds = cms.vdouble(0.9,1.5,45.0), upBounds = cms.vdouble(999,3.0,50.0),     values = cms.vdouble(0.999015233052), uncertainties = cms.vdouble(0.001, 0.001)),
        cms.PSet(lowBounds = cms.vdouble(0.9,1.5,50.0), upBounds = cms.vdouble(999,3.0,60.0),     values = cms.vdouble(0.99859984424), uncertainties = cms.vdouble(0.001, 0.001)),
        cms.PSet(lowBounds = cms.vdouble(0.9,1.5,60.0), upBounds = cms.vdouble(999,3.0,70.0),     values = cms.vdouble(0.999485615485), uncertainties = cms.vdouble(0.001, 0.001)),
        cms.PSet(lowBounds = cms.vdouble(0.9,1.5,70.0), upBounds = cms.vdouble(999,3.0,90.0),     values = cms.vdouble(0.999696303764), uncertainties = cms.vdouble(0.001, 0.001)),
        cms.PSet(lowBounds = cms.vdouble(0.9,1.5,90.0), upBounds = cms.vdouble(999,3.0,99999999), values = cms.vdouble(0.999529312359), uncertainties = cms.vdouble(0.001, 0.001)),

        

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
        cms.PSet(lowBounds = cms.vdouble(0),   upBounds = cms.vdouble(5),   values = cms.vdouble(1.01253, 0.985048), uncertainties = cms.vdouble(0.0227895, 0.0227895, 0.0272039, 0.0272039)),
        cms.PSet(lowBounds = cms.vdouble(5),   upBounds = cms.vdouble(10),  values = cms.vdouble(1.02657, 0.963112), uncertainties = cms.vdouble(0.0232292, 0.0232292, 0.0322519, 0.0322519)),
        cms.PSet(lowBounds = cms.vdouble(10),  upBounds = cms.vdouble(15),  values = cms.vdouble(1.01611, 0.974286), uncertainties = cms.vdouble(0.0204214, 0.0204214, 0.0325975, 0.0325975)),
        cms.PSet(lowBounds = cms.vdouble(15),  upBounds = cms.vdouble(20),  values = cms.vdouble(1.0055, 0.988376), uncertainties = cms.vdouble(0.0164169, 0.0164169, 0.0346856, 0.0346856)),
        cms.PSet(lowBounds = cms.vdouble(20),  upBounds = cms.vdouble(30),  values = cms.vdouble(0.997655, 1.00661), uncertainties = cms.vdouble(0.0111265, 0.0111265, 0.031362, 0.031362)),
        cms.PSet(lowBounds = cms.vdouble(30),  upBounds = cms.vdouble(40),  values = cms.vdouble(0.994367, 1.02555), uncertainties = cms.vdouble(0.00662661, 0.00662661, 0.0300579, 0.0300579)),
        cms.PSet(lowBounds = cms.vdouble(40),  upBounds = cms.vdouble(50),  values = cms.vdouble(0.992503, 1.05304), uncertainties = cms.vdouble(0.00455545, 0.00455545, 0.03228, 0.03228)),
        cms.PSet(lowBounds = cms.vdouble(50),  upBounds = cms.vdouble(60),  values = cms.vdouble(0.990506, 1.09707), uncertainties = cms.vdouble(0.00315745, 0.00315745, 0.0326274, 0.0326274)),
        cms.PSet(lowBounds = cms.vdouble(60),  upBounds = cms.vdouble(80),  values = cms.vdouble(0.992573, 1.11839), uncertainties = cms.vdouble(0.00223168, 0.00223168, 0.0361956, 0.0361956)),
        cms.PSet(lowBounds = cms.vdouble(80),  upBounds = cms.vdouble(100), values = cms.vdouble(0.992787, 1.16605), uncertainties = cms.vdouble(0.00157028, 0.00157028, 0.0389978, 0.0389978)),
        cms.PSet(lowBounds = cms.vdouble(100), upBounds = cms.vdouble(140), values = cms.vdouble(0.997477, 1.08442), uncertainties = cms.vdouble(0.00119315, 0.00119315, 0.0407442, 0.0407442)),
        cms.PSet(lowBounds = cms.vdouble(140), upBounds = cms.vdouble(200), values = cms.vdouble(0.999887, 1.00884), uncertainties = cms.vdouble(0.00104511, 0.00104511, 0.0814968, 0.0814968)),
        cms.PSet(lowBounds = cms.vdouble(200), upBounds = cms.vdouble(400), values = cms.vdouble(1.0006, 0.941291), uncertainties = cms.vdouble(0.00141694, 0.00141694, 0.139147, 0.139147)),
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

