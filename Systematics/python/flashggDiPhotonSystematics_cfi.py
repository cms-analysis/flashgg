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

# PR 1045
preselBins = cms.PSet(
    variables = cms.vstring("abs(superCluster.eta)","full5x5_r9"),
    bins = cms.VPSet(
        cms.PSet( lowBounds = cms.vdouble( 0.0, 0.0 ) , upBounds = cms.vdouble( 1.5, 0.85 ) , values = cms.vdouble( 0.998 ) , uncertainties = cms.vdouble( 0.005 )  ) ,
        cms.PSet( lowBounds = cms.vdouble( 0.0, 0.85 ), upBounds = cms.vdouble( 1.5, 999. ) , values = cms.vdouble( 1.001 ) , uncertainties = cms.vdouble( 0.003 )  ) ,
        cms.PSet( lowBounds = cms.vdouble( 1.5, 0.0 ) , upBounds = cms.vdouble( 6.0, 0.9  ) , values = cms.vdouble( 0.989 ) , uncertainties = cms.vdouble( 0.006 )  ) ,
        cms.PSet( lowBounds = cms.vdouble( 1.5, 0.9 ) , upBounds = cms.vdouble( 6.0, 999. ) , values = cms.vdouble( 1.002 ) , uncertainties = cms.vdouble( 0.009 )  ) 
        )
    )

# JTao: Common AN AN-18-105
electronVetoBins = cms.PSet(
    variables = cms.vstring("abs(superCluster.eta)","full5x5_r9"),
    bins = cms.VPSet(
        cms.PSet( lowBounds = cms.vdouble( 0.0, 0.00 ) , upBounds = cms.vdouble( 1.5, 0.85 ) , values = cms.vdouble( 0.9864 ) , uncertainties = cms.vdouble( 0.0022 )  ) ,
        cms.PSet( lowBounds = cms.vdouble( 0.0, 0.85 ) , upBounds = cms.vdouble( 1.5, 999. ) , values = cms.vdouble( 0.9925 ) , uncertainties = cms.vdouble( 0.0006 )  ) ,
        cms.PSet( lowBounds = cms.vdouble( 1.5, 0.00 ) , upBounds = cms.vdouble( 6.0, 0.90 ) , values = cms.vdouble( 0.9771 ) , uncertainties = cms.vdouble( 0.0074 )  ) ,
        cms.PSet( lowBounds = cms.vdouble( 1.5, 0.90 ) , upBounds = cms.vdouble( 6.0, 999. ) , values = cms.vdouble( 0.9855 ) , uncertainties = cms.vdouble( 0.0018 )  )
        )
    )


# JMalcles - based on JTao SF + ttH efficiencies. calculated to preserve nTot ttH. 

leadPixelSeedBins = cms.PSet(
    variables = cms.vstring("abs(superCluster.eta)","full5x5_r9", "hasPixelSeed"),
    bins = cms.VPSet(
        # No Pixel Seed
        cms.PSet( lowBounds = cms.vdouble( 0.0, 0.00, -0.1 ) , upBounds = cms.vdouble( 1.5, 0.85 , 0.1) , values = cms.vdouble(0.978 ) , uncertainties = cms.vdouble( -0.00401807 )  ) ,
        cms.PSet( lowBounds = cms.vdouble( 0.0, 0.85, -0.1 ) , upBounds = cms.vdouble( 1.5, 999. , 0.1) , values = cms.vdouble(0.9824) , uncertainties = cms.vdouble( -0.00200421 )  ) ,
        cms.PSet( lowBounds = cms.vdouble( 1.5, 0.00, -0.1 ) , upBounds = cms.vdouble( 6.0, 0.90 , 0.1) , values = cms.vdouble(0.9168 ) , uncertainties = cms.vdouble( -0.0224756 )  ) ,
        cms.PSet( lowBounds = cms.vdouble( 1.5, 0.90 ,-0.1) , upBounds = cms.vdouble( 6.0, 999., 0.1) , values = cms.vdouble( 0.9403 ) , uncertainties = cms.vdouble(  -0.00631264  )  ),        
        # Yes Pixel Seed
        cms.PSet( lowBounds = cms.vdouble( 0.0, 0.00 ,0.9) , upBounds = cms.vdouble( 1.5, 0.85, 1.1 ) , values = cms.vdouble( 1.08876 ) , uncertainties = cms.vdouble( 0.0162106 )  ) ,
        cms.PSet( lowBounds = cms.vdouble( 0.0, 0.85 ,0.9) , upBounds = cms.vdouble( 1.5, 999., 1.1 ) , values = cms.vdouble( 1.5961) , uncertainties = cms.vdouble( 0.0678807 )  ) ,
        cms.PSet( lowBounds = cms.vdouble( 1.5, 0.00 ,0.9) , upBounds = cms.vdouble( 6.0, 0.90, 1.1 ) , values = cms.vdouble( 1.09763 ) , uncertainties = cms.vdouble( 0.0263745 )  ) ,
        cms.PSet( lowBounds = cms.vdouble( 1.5, 0.90 ,0.9) , upBounds = cms.vdouble( 6.0, 999., 1.1 ) , values = cms.vdouble( 1.20264 ) , uncertainties = cms.vdouble(  0.0214274 )  )
        )
    )


subleadPixelSeedBins = cms.PSet(
    variables = cms.vstring("abs(superCluster.eta)","full5x5_r9", "hasPixelSeed"),
    bins = cms.VPSet(
        # No Pixel Seed
        cms.PSet( lowBounds = cms.vdouble( 0.0, 0.00, -0.1 ) , upBounds = cms.vdouble( 1.5, 0.85 , 0.1) , values = cms.vdouble( 0.978  ) , uncertainties = cms.vdouble(  -0.00415083)  ) ,
        cms.PSet( lowBounds = cms.vdouble( 0.0, 0.85, -0.1 ) , upBounds = cms.vdouble( 1.5, 999. , 0.1) , values = cms.vdouble( 0.9824 ) , uncertainties = cms.vdouble( -0.00280026 )  ) ,
        cms.PSet( lowBounds = cms.vdouble( 1.5, 0.00, -0.1 ) , upBounds = cms.vdouble( 6.0, 0.90 , 0.1) , values = cms.vdouble( 0.9168 ) , uncertainties = cms.vdouble( -0.0225538  )  ) ,
        cms.PSet( lowBounds = cms.vdouble( 1.5, 0.90 ,-0.1) , upBounds = cms.vdouble( 6.0, 999., 0.1) , values = cms.vdouble( 0.9403 ) , uncertainties = cms.vdouble( -0.00655045 )  ),        
        # Yes Pixel Seed
        cms.PSet( lowBounds = cms.vdouble( 0.0, 0.00 ,0.9) , upBounds = cms.vdouble( 1.5, 0.85, 1.1 ) , values = cms.vdouble( 1.13196) , uncertainties = cms.vdouble( 0.0248967 )  ) ,
        cms.PSet( lowBounds = cms.vdouble( 0.0, 0.85 ,0.9) , upBounds = cms.vdouble( 1.5, 999., 1.1 ) , values = cms.vdouble( 1.61512 ) , uncertainties = cms.vdouble(  0.0978689 )  ) ,
        cms.PSet( lowBounds = cms.vdouble( 1.5, 0.00 ,0.9) , upBounds = cms.vdouble( 6.0, 0.90, 1.1 ) , values = cms.vdouble( 1.10623 ) , uncertainties = cms.vdouble(  0.0287957 )  ) ,
        cms.PSet( lowBounds = cms.vdouble( 1.5, 0.90 ,0.9) , upBounds = cms.vdouble( 6.0, 999., 1.1 ) , values = cms.vdouble( 1.20311 ) , uncertainties = cms.vdouble(  0.0222861 )  )
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


#for full 2017 rereco B-F dataset.  Trigger scale factors for use without HLT applied in MC
leadTriggerScaleBins = cms.PSet(
    variables = cms.vstring("full5x5_r9","abs(superCluster.eta)","pt"),
    bins = cms.VPSet(

        #0 <eta < 1.5, R9<0.54
        cms.PSet(lowBounds = cms.vdouble(0.0,0.0,0.0), upBounds = cms.vdouble(0.54,1.5,37.0),values = cms.vdouble(0.9204369513), uncertainties = cms.vdouble(0.0015550248,0.0015550248)), 
        cms.PSet(lowBounds = cms.vdouble(0.0,0.0,37), upBounds = cms.vdouble(0.54,1.5,40.0), values = cms.vdouble(0.9268543618), uncertainties = cms.vdouble(0.0012001964,0.0012001964)),
        cms.PSet(lowBounds = cms.vdouble(0.0,0.0,40.0), upBounds = cms.vdouble(0.54,1.5,45.0),values = cms.vdouble(0.9296864506), uncertainties = cms.vdouble(0.0010500218,0.0010500218)), 
        cms.PSet(lowBounds = cms.vdouble(0.0,0.0,45.0), upBounds = cms.vdouble(0.54,1.5,50.0),values = cms.vdouble(0.9312181087), uncertainties = cms.vdouble(0.0016928371,0.0016928371)), 
        cms.PSet(lowBounds = cms.vdouble(0.0,0.0,50.0), upBounds = cms.vdouble(0.54,1.5,60.0),values = cms.vdouble(0.9349061049), uncertainties = cms.vdouble(0.0024479457,0.0024479457)), 
        cms.PSet(lowBounds = cms.vdouble(0.0,0.0,60.0), upBounds = cms.vdouble(0.54,1.5,70.0),values = cms.vdouble(0.9425532022), uncertainties = cms.vdouble(0.0052975139,0.0052975139)), 
        cms.PSet(lowBounds = cms.vdouble(0.0,0.0,70.0), upBounds = cms.vdouble(0.54,1.5,90.0),values = cms.vdouble(0.9347621741), uncertainties = cms.vdouble(0.0089854906,0.0089854906)), 
        cms.PSet(lowBounds = cms.vdouble(0.0,0.0,90.0), upBounds = cms.vdouble(0.54,1.5,99999999), values = cms.vdouble(0.9489236873), uncertainties = cms.vdouble(0.0177143070,0.0177143070)),

        #0 <eta < 1.5, 0.54 < R9<0.85 
        cms.PSet(lowBounds = cms.vdouble(0.54,0.0,0.0), upBounds = cms.vdouble(0.85,1.5,37.0),values = cms.vdouble(0.9505374744), uncertainties = cms.vdouble(0.0010000000,0.0010000000)), 
        cms.PSet(lowBounds = cms.vdouble(0.54,0.0,37.0), upBounds = cms.vdouble(0.85,1.5,40.0),values = cms.vdouble(0.9559316002), uncertainties = cms.vdouble(0.0010000000,0.0010000000)), 
        cms.PSet(lowBounds = cms.vdouble(0.54,0.0,40.0), upBounds = cms.vdouble(0.85,1.5,45.0), values = cms.vdouble(0.9603227460), uncertainties = cms.vdouble(0.0010000000,0.0010000000)),  
        cms.PSet(lowBounds = cms.vdouble(0.54,0.0,45.0), upBounds = cms.vdouble(0.85,1.5,50.0),values = cms.vdouble(0.9628461917), uncertainties = cms.vdouble(0.0010000000,0.0010000000)), 
        cms.PSet(lowBounds = cms.vdouble(0.54,0.0,50.0), upBounds = cms.vdouble(0.85,1.5,60.0),values = cms.vdouble(0.9662884361), uncertainties = cms.vdouble(0.0010000000,0.0010000000)), 
        cms.PSet(lowBounds = cms.vdouble(0.54,0.0,60.0), upBounds = cms.vdouble(0.85,1.5,70.0),values = cms.vdouble(0.9715136996), uncertainties = cms.vdouble(0.0010000000,0.0010000000)), 
        cms.PSet(lowBounds = cms.vdouble(0.54,0.0,70.0), upBounds = cms.vdouble(0.85,1.5,90.0),values = cms.vdouble(0.9753661262), uncertainties = cms.vdouble(0.0010000000,0.0010000000)), 
        cms.PSet(lowBounds = cms.vdouble(0.54,0.0,90.0), upBounds = cms.vdouble(0.85,1.5,99999999),values = cms.vdouble(0.9818196687), uncertainties = cms.vdouble(0.0010000000,0.0010000000)), 

        #0 <eta < 1.5, R9>0.85 
        cms.PSet(lowBounds = cms.vdouble(0.85,0.0,0.0), upBounds = cms.vdouble(999,1.5,37.0),values = cms.vdouble(0.9630117411), uncertainties = cms.vdouble(0.0010000000,0.0010000000)), 
        cms.PSet(lowBounds = cms.vdouble(0.85,0.0,37.0), upBounds = cms.vdouble(999,1.5,40.0),values = cms.vdouble(0.9680310533), uncertainties = cms.vdouble(0.0010000000,0.0010000000)), 
        cms.PSet(lowBounds = cms.vdouble(0.85,0.0,40.0), upBounds = cms.vdouble(999,1.5,45.0),values = cms.vdouble(0.9721152461), uncertainties = cms.vdouble(0.0010000000,0.0010000000)), 
        cms.PSet(lowBounds = cms.vdouble(0.85,0.0,45.0), upBounds = cms.vdouble(999,1.5,50.0),values = cms.vdouble(0.9748189645), uncertainties = cms.vdouble(0.0010000000,0.0010000000)), 
        cms.PSet(lowBounds = cms.vdouble(0.85,0.0,50.0), upBounds = cms.vdouble(999,1.5,60.0),values = cms.vdouble(0.9747597366), uncertainties = cms.vdouble(0.0010000000,0.0010000000)), 
        cms.PSet(lowBounds = cms.vdouble(0.85,0.0,60.0), upBounds = cms.vdouble(999,1.5,70.0),values = cms.vdouble(0.9753162174), uncertainties = cms.vdouble(0.0010000000,0.0010000000)), 
        cms.PSet(lowBounds = cms.vdouble(0.85,0.0,70.0), upBounds = cms.vdouble(999,1.5,90.0),values = cms.vdouble(0.9803243059), uncertainties = cms.vdouble(0.0010000000,0.0010000000)), 
        cms.PSet(lowBounds = cms.vdouble(0.85,0.0,90.0), upBounds = cms.vdouble(999,1.5,99999999),values = cms.vdouble(0.9865482764), uncertainties = cms.vdouble(0.0010000000,0.0010000000)), 

        #1.5 <eta <3, R9<0.84
        cms.PSet(lowBounds = cms.vdouble(0.0,1.5,0.0), upBounds = cms.vdouble(0.84,3.0,37.0),values = cms.vdouble(0.9093169942), uncertainties = cms.vdouble(0.0017122892,0.0017122892)), 
        cms.PSet(lowBounds = cms.vdouble(0.0,1.5,37.0), upBounds = cms.vdouble(0.84,3.0,40.0),values = cms.vdouble(0.9214136806), uncertainties = cms.vdouble(0.0012578761,0.0012578761)), 
        cms.PSet(lowBounds = cms.vdouble(0.0,1.5,40.0), upBounds = cms.vdouble(0.84,3.0,45.0),values = cms.vdouble(0.9396498081), uncertainties = cms.vdouble(0.0010000000,0.0010000000)), 
        cms.PSet(lowBounds = cms.vdouble(0.0,1.5,45.0), upBounds = cms.vdouble(0.84,3.0,50.0),values = cms.vdouble(0.9455103913), uncertainties = cms.vdouble(0.0013009169,0.0013009169)), 
        cms.PSet(lowBounds = cms.vdouble(0.0,1.5,50.0), upBounds = cms.vdouble(0.84,3.0,60.0),values = cms.vdouble(0.9519493209), uncertainties = cms.vdouble(0.0016579300,0.0016579300)), 
        cms.PSet(lowBounds = cms.vdouble(0.0,1.5,60.0), upBounds = cms.vdouble(0.84,3.0,70.0),values = cms.vdouble(0.9560088523), uncertainties = cms.vdouble(0.0031358462,0.0031358462)), 
        cms.PSet(lowBounds = cms.vdouble(0.0,1.5,70.0), upBounds = cms.vdouble(0.84,3.0,90.0),values = cms.vdouble(0.9638480843), uncertainties = cms.vdouble(0.0036329374,0.0036329374)), 
        cms.PSet(lowBounds = cms.vdouble(0.,1.5,90.0), upBounds = cms.vdouble(0.84,3.0,99999999),values = cms.vdouble(0.9714734218), uncertainties = cms.vdouble(0.0051321702,0.0051321702)), 

        #1.5 <eta <3, 0.84< R9<0.90
        cms.PSet(lowBounds = cms.vdouble(0.84,1.5,0.0), upBounds = cms.vdouble(0.9,3.0,37.0),values = cms.vdouble(0.9614951080), uncertainties = cms.vdouble(0.0010000000,0.0010000000)), 
        cms.PSet(lowBounds = cms.vdouble(0.84,1.5,37.0), upBounds = cms.vdouble(0.9,3.0,40.0),values = cms.vdouble(0.9750837371), uncertainties = cms.vdouble(0.0010000000,0.0010000000)),
        cms.PSet(lowBounds = cms.vdouble(0.84,1.5,40.0), upBounds = cms.vdouble(0.9,3.0,45.0),values = cms.vdouble(0.9814701227), uncertainties = cms.vdouble(0.0010000000,0.0010000000)),
        cms.PSet(lowBounds = cms.vdouble(0.84,1.5,45.0), upBounds = cms.vdouble(0.9,3.0,50.0),values = cms.vdouble(0.9835352074), uncertainties = cms.vdouble(0.0010000000,0.0010000000)),
        cms.PSet(lowBounds = cms.vdouble(0.84,1.5,50.0), upBounds = cms.vdouble(0.9,3.0,60.0),values = cms.vdouble(0.9851237595), uncertainties = cms.vdouble(0.0010000000,0.0010000000)),
        cms.PSet(lowBounds = cms.vdouble(0.84,1.5,60.0), upBounds = cms.vdouble(0.9,3.0,70.0),values = cms.vdouble(0.9869551174), uncertainties = cms.vdouble(0.0010000000,0.0010000000)),
        cms.PSet(lowBounds = cms.vdouble(0.84,1.5,70.0), upBounds = cms.vdouble(0.9,3.0,90.0),values = cms.vdouble(0.9916069861), uncertainties = cms.vdouble(0.0010000000,0.0010000000)),
        cms.PSet(lowBounds = cms.vdouble(0.84,1.5,90.0), upBounds = cms.vdouble(0.9,3.0,99999999),values = cms.vdouble(0.9958525312), uncertainties = cms.vdouble(0.0010000000,0.0010000000)), 
       

        #1.5 <eta <3, R9>0.90
        cms.PSet(lowBounds = cms.vdouble(0.9,1.5,0.0), upBounds = cms.vdouble(999,3.0,37.0),values = cms.vdouble(0.9470889409), uncertainties = cms.vdouble(0.0010000000,0.0010000000)), 
        cms.PSet(lowBounds = cms.vdouble(0.9,1.5,37.0), upBounds = cms.vdouble(999,3.0,40.0),values = cms.vdouble(0.9646677794), uncertainties = cms.vdouble(0.0010000000,0.0010000000)),
        cms.PSet(lowBounds = cms.vdouble(0.9,1.5,40.0), upBounds = cms.vdouble(999,3.0,45.0),values = cms.vdouble(0.9738993606), uncertainties = cms.vdouble(0.0010000000,0.0010000000)),
        cms.PSet(lowBounds = cms.vdouble(0.9,1.5,45.0), upBounds = cms.vdouble(999,3.0,50.0),values = cms.vdouble(0.9784039943), uncertainties = cms.vdouble(0.0010000000,0.0010000000)),
        cms.PSet(lowBounds = cms.vdouble(0.9,1.5,50.0), upBounds = cms.vdouble(999,3.0,60.0),values = cms.vdouble(0.9802345170), uncertainties = cms.vdouble(0.0010000000,0.0010000000)),
        cms.PSet(lowBounds = cms.vdouble(0.9,1.5,60.0), upBounds = cms.vdouble(999,3.0,70.0),values = cms.vdouble(0.9828741312), uncertainties = cms.vdouble(0.0010000000,0.0010000000)),
        cms.PSet(lowBounds = cms.vdouble(0.9,1.5,70.0), upBounds = cms.vdouble(999,3.0,90.0),values = cms.vdouble(0.9870534958), uncertainties = cms.vdouble(0.0010000000,0.0010000000)),
        cms.PSet(lowBounds = cms.vdouble(0.9,1.5,90.0), upBounds = cms.vdouble(999,3.0,99999999),values = cms.vdouble(0.9891255039), uncertainties = cms.vdouble(0.0010000000,0.0010000000)), 
        
        cms.PSet(lowBounds = cms.vdouble(0.0,3.,0), upBounds = cms.vdouble(999,999,999999), values = cms.vdouble(1.), uncertainties = cms.vdouble(1.,1.))
        )
    )


subleadTriggerScaleBins = cms.PSet(
    variables = cms.vstring("full5x5_r9","abs(superCluster.eta)","pt"),
    bins = cms.VPSet(
      
        #0 <eta < 1.5, R9<0.54
        cms.PSet(lowBounds = cms.vdouble(0.0,0.0,0.0), upBounds = cms.vdouble(0.54,1.5,28.0), values = cms.vdouble(0.9503915847), uncertainties = cms.vdouble(0.0014203590,0.0014203590)),
        cms.PSet(lowBounds = cms.vdouble(0.0,0.0,28.0), upBounds = cms.vdouble(0.54,1.5,31.0), values = cms.vdouble(0.9563040539), uncertainties = cms.vdouble(0.0011666773,0.0011666773)),
        cms.PSet(lowBounds = cms.vdouble(0.0,0.0,31.0), upBounds = cms.vdouble(0.54,1.5,35.0), values = cms.vdouble(0.9554701294), uncertainties = cms.vdouble(0.0010000000,0.0010000000)), 
        cms.PSet(lowBounds = cms.vdouble(0.0,0.0,35.0), upBounds = cms.vdouble(0.54,1.5,40.0), values = cms.vdouble(0.9573495211), uncertainties = cms.vdouble(0.0010000000,0.0010000000)), 
        cms.PSet(lowBounds = cms.vdouble(0.0,0.0,40.0), upBounds = cms.vdouble(0.54,1.5,45.0),  values = cms.vdouble(0.9615640328), uncertainties = cms.vdouble(0.0010000000,0.0010000000)),
        cms.PSet(lowBounds = cms.vdouble(0.0,0.0,45.0), upBounds = cms.vdouble(0.54,1.5,50.0),  values = cms.vdouble(0.9635847522), uncertainties = cms.vdouble(0.0012149116,0.0012149116)),
        cms.PSet(lowBounds = cms.vdouble(0.0,0.0,50.0), upBounds = cms.vdouble(0.54,1.5,60.0), values = cms.vdouble(0.9613424282), uncertainties = cms.vdouble(0.0018398439,0.0018398439)), 
        cms.PSet(lowBounds = cms.vdouble(0.0,0.0,60.0), upBounds = cms.vdouble(0.54,1.5,70.0), values = cms.vdouble(0.9531798460), uncertainties = cms.vdouble(0.0046295174,0.0046295174)),
        cms.PSet(lowBounds = cms.vdouble(0.0,0.0,70.0), upBounds = cms.vdouble(0.54,1.5,90.0),  values = cms.vdouble(0.9427949472), uncertainties = cms.vdouble(0.0082547803,0.0082547803)),
        cms.PSet(lowBounds = cms.vdouble(0.0,0.0,90.0), upBounds = cms.vdouble(0.54,1.5,99999999), values = cms.vdouble(0.9180892383), uncertainties = cms.vdouble(0.0210727517,0.0210727517)), 

        #0 <eta < 1.5, 0.54<R9<0.85
        cms.PSet(lowBounds = cms.vdouble(0.54,0.0,0.0), upBounds = cms.vdouble(0.85,1.5,28.0),values = cms.vdouble(0.9821722615), uncertainties = cms.vdouble(0.0010000000,0.0010000000)),
        cms.PSet(lowBounds = cms.vdouble(0.54,0.0,28.0), upBounds = cms.vdouble(0.85,1.5,31.0), values = cms.vdouble(0.9846684256), uncertainties = cms.vdouble(0.0010000000,0.0010000000)),
        cms.PSet(lowBounds = cms.vdouble(0.54,0.0,31.0), upBounds = cms.vdouble(0.85,1.5,35.0), values = cms.vdouble(0.9853409044), uncertainties = cms.vdouble(0.0010000000,0.0010000000)),
        cms.PSet(lowBounds = cms.vdouble(0.54,0.0,35.0), upBounds = cms.vdouble(0.85,1.5,40.0), values = cms.vdouble(0.9851741151), uncertainties = cms.vdouble(0.0010000000,0.0010000000)),
        cms.PSet(lowBounds = cms.vdouble(0.54,0.0,40.0), upBounds = cms.vdouble(0.85,1.5,45.0),  values = cms.vdouble(0.9847237955), uncertainties = cms.vdouble(0.0010000000,0.0010000000)),
        cms.PSet(lowBounds = cms.vdouble(0.54,0.0,45.0), upBounds = cms.vdouble(0.85,1.5,50.0),values = cms.vdouble(0.9836985511), uncertainties = cms.vdouble(0.0010000000,0.0010000000)),
        cms.PSet(lowBounds = cms.vdouble(0.54,0.0,50.0), upBounds = cms.vdouble(0.85,1.5,60.0), values = cms.vdouble(0.9818687985), uncertainties = cms.vdouble(0.0010000000,0.0010000000)),
        cms.PSet(lowBounds = cms.vdouble(0.54,0.0,60.0), upBounds = cms.vdouble(0.85,1.5,70.0), values = cms.vdouble(0.9793978257), uncertainties = cms.vdouble(0.0010000000,0.0010000000)),
        cms.PSet(lowBounds = cms.vdouble(0.54,0.0,70.0), upBounds = cms.vdouble(0.85,1.5,90.0), values = cms.vdouble(0.9766027088), uncertainties = cms.vdouble(0.0010000000,0.0010000000)),
        cms.PSet(lowBounds = cms.vdouble(0.54,0.0,90.0), upBounds = cms.vdouble(0.85,1.5,99999999), values = cms.vdouble(0.9723314370), uncertainties = cms.vdouble(0.0011171683,0.0011171683)),

        #0 <eta < 1.5, R9>0.85
        cms.PSet(lowBounds = cms.vdouble(0.85,0.0,0.0), upBounds = cms.vdouble(999,1.5,28.0), values = cms.vdouble(0.9997101832), uncertainties = cms.vdouble(0.0010000000,0.0010000000)),
        cms.PSet(lowBounds = cms.vdouble(0.85,0.0,28.0), upBounds = cms.vdouble(999,1.5,31.0),  values = cms.vdouble(0.9998675478), uncertainties = cms.vdouble(0.0010000000,0.0010000000)),
        cms.PSet(lowBounds = cms.vdouble(0.85,0.0,31.0), upBounds = cms.vdouble(999,1.5,35.0), values = cms.vdouble(0.9999351175), uncertainties = cms.vdouble(0.0010000000,0.0010000000)),
        cms.PSet(lowBounds = cms.vdouble(0.85,0.0,35.0), upBounds = cms.vdouble(999,1.5,40.0),values = cms.vdouble(0.9999541560), uncertainties = cms.vdouble(0.0010000000,0.0010000000)),
        cms.PSet(lowBounds = cms.vdouble(0.85,0.0,40.0), upBounds = cms.vdouble(999,1.5,45.0), values = cms.vdouble(0.9999645283), uncertainties = cms.vdouble(0.0010000000,0.0010000000)),
        cms.PSet(lowBounds = cms.vdouble(0.85,0.0,45.0), upBounds = cms.vdouble(999,1.5,50.0),  values = cms.vdouble(0.9999677894), uncertainties = cms.vdouble(0.0010000000,0.0010000000)),
        cms.PSet(lowBounds = cms.vdouble(0.85,0.0,50.0), upBounds = cms.vdouble(999,1.5,60.0),  values = cms.vdouble(0.9999665309), uncertainties = cms.vdouble(0.0010000000,0.0010000000)),
        cms.PSet(lowBounds = cms.vdouble(0.85,0.0,60.0), upBounds = cms.vdouble(999,1.5,70.0), values = cms.vdouble(0.9999602573), uncertainties = cms.vdouble(0.0010000000,0.0010000000)),
        cms.PSet(lowBounds = cms.vdouble(0.85,0.0,70.0), upBounds = cms.vdouble(999,1.5,90.0), values = cms.vdouble(0.9999369329), uncertainties = cms.vdouble(0.0010000000,0.0010000000)),
        cms.PSet(lowBounds = cms.vdouble(0.85,0.0,90.0), upBounds = cms.vdouble(999,1.5,99999999), values = cms.vdouble(0.9999456063), uncertainties = cms.vdouble(0.0010000000,0.0010000000)),
    
        
        #1.5 <eta < 3, R9<0.84
        cms.PSet(lowBounds = cms.vdouble(0.0,1.5,0.0), upBounds = cms.vdouble(0.84,3.0,28.0),  values = cms.vdouble(0.8799714219), uncertainties = cms.vdouble(0.0027302573,0.0027302573)),
        cms.PSet(lowBounds = cms.vdouble(0.0,1.5,28.0), upBounds = cms.vdouble(0.84,3.0,31.0),  values = cms.vdouble(0.8962741603), uncertainties = cms.vdouble(0.0021032366,0.0021032366)),
        cms.PSet(lowBounds = cms.vdouble(0.0,1.5,31.0), upBounds = cms.vdouble(0.84,3.0,35.0),values = cms.vdouble(0.9042685088), uncertainties = cms.vdouble(0.0014198635,0.0014198635)),
        cms.PSet(lowBounds = cms.vdouble(0.0,1.5,35.0), upBounds = cms.vdouble(0.84,3.0,40.0),values = cms.vdouble(0.9130894467), uncertainties = cms.vdouble(0.0010478241,0.0010478241)),
        cms.PSet(lowBounds = cms.vdouble(0.0,1.5,40.0), upBounds = cms.vdouble(0.84,3.0,45.0),values = cms.vdouble(0.9236887059), uncertainties = cms.vdouble(0.0010511874,0.0010511874)),
        cms.PSet(lowBounds = cms.vdouble(0.0,1.5,45.0), upBounds = cms.vdouble(0.84,3.0,50.0),values = cms.vdouble(0.9269092960), uncertainties = cms.vdouble(0.0015068743,0.0015068743)),
        cms.PSet(lowBounds = cms.vdouble(0.0,1.5,50.0), upBounds = cms.vdouble(0.84,3.0,60.0), values = cms.vdouble(0.9300621013), uncertainties = cms.vdouble(0.0019932870,0.0019932870)), 
        cms.PSet(lowBounds = cms.vdouble(0.0,1.5,60.0), upBounds = cms.vdouble(0.84,3.0,70.0), values = cms.vdouble(0.9313708557), uncertainties = cms.vdouble(0.0038708618,0.0038708618)), 
        cms.PSet(lowBounds = cms.vdouble(0.0,1.5,70.0), upBounds = cms.vdouble(0.84,3.0,90.0),values = cms.vdouble(0.9387860583), uncertainties = cms.vdouble(0.0046452036,0.0046452036)), 
        cms.PSet(lowBounds = cms.vdouble(0.0,1.5,90.0), upBounds = cms.vdouble(0.84,3.0,99999999), values = cms.vdouble(0.9316696569), uncertainties = cms.vdouble(0.0077182350,0.0077182350)),


        #1.5 <eta < 3, 0.84<R9<0.9
        cms.PSet(lowBounds = cms.vdouble(0.84,1.5,0.0), upBounds = cms.vdouble(0.9,3.0,28.0), values = cms.vdouble(0.9482346612), uncertainties = cms.vdouble(0.0011380402,0.0011380402)),
        cms.PSet(lowBounds = cms.vdouble(0.84,1.5,28.0), upBounds = cms.vdouble(0.9,3.0,31.0), values = cms.vdouble(0.9753440965), uncertainties = cms.vdouble(0.0010000000,0.0010000000)),
        cms.PSet(lowBounds = cms.vdouble(0.84,1.5,31.0), upBounds = cms.vdouble(0.9,3.0,35.0), values = cms.vdouble(0.9760310914), uncertainties = cms.vdouble(0.0010000000,0.0010000000)),
        cms.PSet(lowBounds = cms.vdouble(0.84,1.5,35.0), upBounds = cms.vdouble(0.9,3.0,40.0),  values = cms.vdouble(0.9752981073), uncertainties = cms.vdouble(0.0010000000,0.0010000000)), 
        cms.PSet(lowBounds = cms.vdouble(0.84,1.5,40.0), upBounds = cms.vdouble(0.9,3.0,45.0), values = cms.vdouble(0.9737050160), uncertainties = cms.vdouble(0.0010000000,0.0010000000)),
        cms.PSet(lowBounds = cms.vdouble(0.84,1.5,45.0), upBounds = cms.vdouble(0.9,3.0,50.0),  values = cms.vdouble(0.9719236369), uncertainties = cms.vdouble(0.0010000000,0.0010000000)), 
        cms.PSet(lowBounds = cms.vdouble(0.84,1.5,50.0), upBounds = cms.vdouble(0.9,3.0,60.0),  values = cms.vdouble(0.9704295117), uncertainties = cms.vdouble(0.0010000000,0.0010000000)), 
        cms.PSet(lowBounds = cms.vdouble(0.84,1.5,60.0), upBounds = cms.vdouble(0.9,3.0,70.0),   values = cms.vdouble(0.9692422639), uncertainties = cms.vdouble(0.0014503236,0.0014503236)),
        cms.PSet(lowBounds = cms.vdouble(0.84,1.5,70.0), upBounds = cms.vdouble(0.9,3.0,90.0), values = cms.vdouble(0.9692330221), uncertainties = cms.vdouble(0.0018171502,0.0018171502)), 
        cms.PSet(lowBounds = cms.vdouble(0.84,1.5,90.0), upBounds = cms.vdouble(0.9,3.0,99999999),  values = cms.vdouble(0.9726630360), uncertainties = cms.vdouble(0.0024173224,0.0024173224)),


        #1.5 <eta < 3, R9>0.9
        cms.PSet(lowBounds = cms.vdouble(0.9,1.5,0.0), upBounds = cms.vdouble(999,3.0,28.0),  values = cms.vdouble(0.9706915047), uncertainties = cms.vdouble(0.0010000000,0.0010000000)), 
        cms.PSet(lowBounds = cms.vdouble(0.9,1.5,28.0), upBounds = cms.vdouble(999,3.0,31.0),  values = cms.vdouble(0.9975115605), uncertainties = cms.vdouble(0.0010000000,0.0010000000)),
        cms.PSet(lowBounds = cms.vdouble(0.9,1.5,31.0), upBounds = cms.vdouble(999,3.0,35.0), values = cms.vdouble(0.9992706525), uncertainties = cms.vdouble(0.0010000000,0.0010000000)),
        cms.PSet(lowBounds = cms.vdouble(0.9,1.5,35.0), upBounds = cms.vdouble(999,3.0,40.0), values = cms.vdouble(0.9995305126), uncertainties = cms.vdouble(0.0010000000,0.0010000000)),
        cms.PSet(lowBounds = cms.vdouble(0.9,1.5,40.0), upBounds = cms.vdouble(999,3.0,45.0), values = cms.vdouble(0.9995836941), uncertainties = cms.vdouble(0.0010000000,0.0010000000)),
        cms.PSet(lowBounds = cms.vdouble(0.9,1.5,45.0), upBounds = cms.vdouble(999,3.0,50.0),  values = cms.vdouble(0.9996279004), uncertainties = cms.vdouble(0.0010000000,0.0010000000)),
        cms.PSet(lowBounds = cms.vdouble(0.9,1.5,50.0), upBounds = cms.vdouble(999,3.0,60.0),  values = cms.vdouble(0.9995678968), uncertainties = cms.vdouble(0.0010000000,0.0010000000)),
        cms.PSet(lowBounds = cms.vdouble(0.9,1.5,60.0), upBounds = cms.vdouble(999,3.0,70.0), values = cms.vdouble(0.9996782105), uncertainties = cms.vdouble(0.0010000000,0.0010000000)),
        cms.PSet(lowBounds = cms.vdouble(0.9,1.5,70.0), upBounds = cms.vdouble(999,3.0,90.0), values = cms.vdouble(0.9997202663), uncertainties = cms.vdouble(0.0010000000,0.0010000000)),
        cms.PSet(lowBounds = cms.vdouble(0.9,1.5,90.0), upBounds = cms.vdouble(999,3.0,99999999),values = cms.vdouble(0.9993842985), uncertainties = cms.vdouble(0.0010000000,0.0010000000)),

        cms.PSet(lowBounds = cms.vdouble(0.0,3.,0), upBounds = cms.vdouble(999,999,999999), values = cms.vdouble(1.), uncertainties = cms.vdouble(1.,1.))
        )
    )

# from Arnab via Julie 2017 dataset updated 06/11/2018
looseMvaBins = cms.PSet(
    variables = cms.vstring("abs(superCluster.eta)","full5x5_r9"),
    bins = cms.VPSet(
        cms.PSet( lowBounds = cms.vdouble( 0.0, 0.0  ) , upBounds = cms.vdouble( 1.5, 0.85  ) , values = cms.vdouble( 0.998 ) , uncertainties = cms.vdouble( 0.003 )  ) ,
        cms.PSet( lowBounds = cms.vdouble( 0.0, 0.85 ) , upBounds = cms.vdouble( 1.5, 999.0 ) , values = cms.vdouble( 0.999 ) , uncertainties = cms.vdouble( 0.005 )  ) ,
        cms.PSet( lowBounds = cms.vdouble( 1.5, 0.0  ) , upBounds = cms.vdouble( 6.0, 0.9   ) , values = cms.vdouble( 1.000 ) , uncertainties = cms.vdouble( 0.004 )  ) ,
        cms.PSet( lowBounds = cms.vdouble( 1.5, 0.9  ) , upBounds = cms.vdouble( 6.0, 999.0 ) , values = cms.vdouble( 1.000 ) , uncertainties = cms.vdouble( 0.003 ) ) ) ) 


# from Arnab via Julie 2017 dataset updated 06/11/2018
tightMvaBins = cms.PSet(
    variables = cms.vstring("abs(superCluster.eta)","full5x5_r9"),
    bins = cms.VPSet(
        cms.PSet( lowBounds = cms.vdouble( 0.0, 0.0  ) , upBounds = cms.vdouble( 1.5, 0.85  ) , values = cms.vdouble( 0.990 ) , uncertainties = cms.vdouble( 0.006 )  ) ,
        cms.PSet( lowBounds = cms.vdouble( 0.0, 0.85 ) , upBounds = cms.vdouble( 1.5, 999.0 ) , values = cms.vdouble( 0.999 ) , uncertainties = cms.vdouble( 0.009 )  ) ,
        cms.PSet( lowBounds = cms.vdouble( 1.5, 0.0  ) , upBounds = cms.vdouble( 6.0, 0.9   ) , values = cms.vdouble( 1.004 ) , uncertainties = cms.vdouble( 0.013 )  ) ,
        cms.PSet( lowBounds = cms.vdouble( 1.5, 0.9  ) , upBounds = cms.vdouble( 6.0, 999.0 ) , values = cms.vdouble( 1.004 ) , uncertainties = cms.vdouble( 0.008 ) ) ) ) 


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


#scalesAndSmearingsPrefix = cms.string("EgammaAnalysis/ElectronTools/data/80X_ichepV2_2016_pho")
#scalesAndSmearingsPrefixForSigmaEOverE = cms.string("EgammaAnalysis/ElectronTools/data/Golden22June")
##scalesAndSmearingsPrefix = cms.string("EgammaAnalysis/ElectronTools/data/Winter_2016_reReco_v1_ele")
##scalesAndSmearingsPrefixForSigmaEOverE = cms.string("EgammaAnalysis/ElectronTools/data/Winter_2016_reReco_v1_ele")
scalesAndSmearingsPrefix = cms.string("RecoEgamma/ScalesSmearings/data/Run2017_17Nov2017_v1_ele_unc")
scalesAndSmearingsPrefixForSigmaEOverE = cms.string("RecoEgamma/ScalesSmearings/data/Run2017_17Nov2017_v1_ele_unc")
#scalesAndSmearingsPrefix = cms.string("flashgg/Systematics/data/Moriond17_74x_pho")
#scalesAndSmearingsPrefixForSigmaEOverE = cms.string("flashgg/Systematics/data/Winter_2016_reReco_v1_ele")





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

PixelSeedWeight = cms.PSet( PhotonMethodName = cms.string("FlashggPhotonWeight"),
          MethodName = cms.string("FlashggDiPhotonFromPhoton"),
          Label = cms.string("PixelSeedWeight"),
          NSigmas = cms.vint32(-1,1),
          OverallRange = cms.string("pt<99999"),
          BinList = leadPixelSeedBins,
          BinList2 = subleadPixelSeedBins,
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

TightMvaSF = cms.PSet( PhotonMethodName = cms.string("FlashggPhotonWeight"),
          MethodName = cms.string("FlashggDiPhotonFromPhoton"),
          Label = cms.string("TightMvaSF"),
          NSigmas = cms.vint32(-1,1),
          OverallRange = cms.string("1"),
          BinList = tightMvaBins,
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
          CorrectionFile = scalesAndSmearingsPrefixForSigmaEOverE,
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
                    MCScaleGain6EB_EGM,
                    MCScaleGain1EB_EGM,
                    MaterialCentralBarrel,
                    MaterialOuterBarrel,
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
                    FracRVWeight,
                    FracRVNvtxWeight
                )
)
