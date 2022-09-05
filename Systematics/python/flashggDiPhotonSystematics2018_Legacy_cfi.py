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

# Prasant Kumar Rout : UL18 Preselection SF and total uncertainties.
preselBins = cms.PSet(
    variables = cms.vstring("abs(superCluster.eta)","full5x5_r9"),
    bins = cms.VPSet(
        cms.PSet( lowBounds = cms.vdouble( 0.0, 0.0 ) , upBounds = cms.vdouble( 1.5, 0.85 ) , values = cms.vdouble( 1.0017 ) , uncertainties = cms.vdouble( 0.0237 )  ) ,
        cms.PSet( lowBounds = cms.vdouble( 0.0, 0.85 ), upBounds = cms.vdouble( 1.5, 999. ) , values = cms.vdouble( 0.9973 ) , uncertainties = cms.vdouble( 0.0067 )  ) ,
        cms.PSet( lowBounds = cms.vdouble( 1.5, 0.0 ) , upBounds = cms.vdouble( 6.0, 0.9  ) , values = cms.vdouble( 1.0030 ) , uncertainties = cms.vdouble( 0.0210 )  ) ,
        cms.PSet( lowBounds = cms.vdouble( 1.5, 0.9 ) , upBounds = cms.vdouble( 6.0, 999. ) , values = cms.vdouble( 1.0031 ) , uncertainties = cms.vdouble( 0.0024 )  )
        )
    )

#JTao: slide 11 of https://indico.cern.ch/event/996926/contributions/4264412/attachments/2203745/3728187/202103ZmmgUL2018_ForHggUpdate.pdf for UL18
electronVetoBins = cms.PSet(
    variables = cms.vstring("abs(superCluster.eta)","full5x5_r9"),
    bins = cms.VPSet(
        cms.PSet( lowBounds = cms.vdouble( 0.0, 0.00 ) , upBounds = cms.vdouble( 1.5, 0.85 ) , values = cms.vdouble( 0.9830 ) , uncertainties = cms.vdouble( 0.0021 )  ) ,
        cms.PSet( lowBounds = cms.vdouble( 0.0, 0.85 ) , upBounds = cms.vdouble( 1.5, 999. ) , values = cms.vdouble( 0.9939 ) , uncertainties = cms.vdouble( 0.0005 )  ) ,
        cms.PSet( lowBounds = cms.vdouble( 1.5, 0.00 ) , upBounds = cms.vdouble( 6.0, 0.90 ) , values = cms.vdouble( 0.9603 ) , uncertainties = cms.vdouble( 0.0077 )  ) ,
        cms.PSet( lowBounds = cms.vdouble( 1.5, 0.90 ) , upBounds = cms.vdouble( 6.0, 999. ) , values = cms.vdouble( 0.9754 ) , uncertainties = cms.vdouble( 0.0017 )  )
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

# Based on the updated derivation by N. Schroeder for HIG-19-004. Re-evaluated in coarse Eta-R9 bins
FNUFBins = cms.PSet(
    variables = cms.vstring("abs(superCluster.eta)","full5x5_r9"),
    bins = cms.VPSet(
        cms.PSet( lowBounds = cms.vdouble( 0.0, 0.00 ) , upBounds = cms.vdouble( 1.5, 0.94 ) , values = cms.vdouble( 0. ) , uncertainties = cms.vdouble(  0.0007 )  ) ,
        cms.PSet( lowBounds = cms.vdouble( 0.0, 0.94 ) , upBounds = cms.vdouble( 1.5, 999. ) , values = cms.vdouble( 0. ) , uncertainties = cms.vdouble(  0.0022 )  ) ,
        cms.PSet( lowBounds = cms.vdouble( 1.5, 0.00 ) , upBounds = cms.vdouble( 6.0, 0.94 ) , values = cms.vdouble( 0. ) , uncertainties = cms.vdouble(  0.00005 )  ) ,
        cms.PSet( lowBounds = cms.vdouble( 1.5, 0.94 ) , upBounds = cms.vdouble( 6.0, 999. ) , values = cms.vdouble( 0. ) , uncertainties = cms.vdouble(  0.00251 )  )
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


#for full 2018 UL (Era2018_RR-17Sep2018_v2-legacyRun2FullV2-v0, rereco A-D dataset).  Trigger scale factors for use without HLT applied in MC
leadTriggerScaleBins = cms.PSet(
    variables = cms.vstring("full5x5_r9","abs(superCluster.eta)","pt"),
    bins = cms.VPSet(

        #0 <eta < 1.5, R9 < 0.56 (cat 2 r9 turn-on bin)
        cms.PSet(lowBounds = cms.vdouble(0.0,0.0, 0.0), upBounds = cms.vdouble(0.56,1.5,35.0), values = cms.vdouble(0.6196012864), uncertainties = cms.vdouble(0.0073267388,0.0073267388)),
        cms.PSet(lowBounds = cms.vdouble(0.0,0.0,35.0), upBounds = cms.vdouble(0.56,1.5,37.0), values = cms.vdouble(0.7761489544), uncertainties = cms.vdouble(0.0025113515,0.0025113515)),
        cms.PSet(lowBounds = cms.vdouble(0.0,0.0,37.0), upBounds = cms.vdouble(0.56,1.5,40.0), values = cms.vdouble(0.8002387986), uncertainties = cms.vdouble(0.0019198622,0.0019198622)), 
        cms.PSet(lowBounds = cms.vdouble(0.0,0.0,40.0), upBounds = cms.vdouble(0.56,1.5,45.0), values = cms.vdouble(0.8231650667), uncertainties = cms.vdouble(0.0025080484,0.0025080484)),
        cms.PSet(lowBounds = cms.vdouble(0.0,0.0,45.0), upBounds = cms.vdouble(0.56,1.5,50.0), values = cms.vdouble(0.8453928052), uncertainties = cms.vdouble(0.0078622969,0.0078622969)),
        cms.PSet(lowBounds = cms.vdouble(0.0,0.0,50.0), upBounds = cms.vdouble(0.56,1.5,60.0), values = cms.vdouble(0.8617735570), uncertainties = cms.vdouble(0.0097098121,0.0097098121)),
        cms.PSet(lowBounds = cms.vdouble(0.0,0.0,60.0), upBounds = cms.vdouble(0.56,1.5,70.0), values = cms.vdouble(0.8712109006), uncertainties = cms.vdouble(0.0133002528,0.0133002528)),
        cms.PSet(lowBounds = cms.vdouble(0.0,0.0,70.0), upBounds = cms.vdouble(0.56,1.5,90.0), values = cms.vdouble(0.8865923420), uncertainties = cms.vdouble(0.0179465197,0.0179465197)),
        cms.PSet(lowBounds = cms.vdouble(0.0,0.0,90.0), upBounds = cms.vdouble(0.56,1.5,99999),values = cms.vdouble(0.8891188478), uncertainties = cms.vdouble(0.0424957054,0.0424957054)),

        #0 <eta < 1.5, 0.56 < R9 < 0.85 (cat 2 r9 plateau-bin) 
        cms.PSet(lowBounds = cms.vdouble(0.56,0.0, 0.0), upBounds = cms.vdouble(0.85,1.5,35.0), values = cms.vdouble(0.7976408633), uncertainties = cms.vdouble(0.0012100047,0.0012100047)),
        cms.PSet(lowBounds = cms.vdouble(0.56,0.0,35.0), upBounds = cms.vdouble(0.85,1.5,37.0), values = cms.vdouble(0.9473792086), uncertainties = cms.vdouble(0.0010042354,0.0010042354)),
        cms.PSet(lowBounds = cms.vdouble(0.56,0.0,37.0), upBounds = cms.vdouble(0.85,1.5,40.0), values = cms.vdouble(0.9578851687), uncertainties = cms.vdouble(0.0035078869,0.0035078869)),
        cms.PSet(lowBounds = cms.vdouble(0.56,0.0,40.0), upBounds = cms.vdouble(0.85,1.5,45.0), values = cms.vdouble(0.9606883609), uncertainties = cms.vdouble(0.0011361977,0.0011361977)),
        cms.PSet(lowBounds = cms.vdouble(0.56,0.0,45.0), upBounds = cms.vdouble(0.85,1.5,50.0), values = cms.vdouble(0.9638132287), uncertainties = cms.vdouble(0.0011771561,0.0011771561)),
        cms.PSet(lowBounds = cms.vdouble(0.56,0.0,50.0), upBounds = cms.vdouble(0.85,1.5,60.0), values = cms.vdouble(0.9653299268), uncertainties = cms.vdouble(0.0018320733,0.0018320733)),
        cms.PSet(lowBounds = cms.vdouble(0.56,0.0,60.0), upBounds = cms.vdouble(0.85,1.5,70.0), values = cms.vdouble(0.9708310992), uncertainties = cms.vdouble(0.0013462303,0.0013462303)),
        cms.PSet(lowBounds = cms.vdouble(0.56,0.0,70.0), upBounds = cms.vdouble(0.85,1.5,90.0), values = cms.vdouble(0.9811906983), uncertainties = cms.vdouble(0.0026482376,0.0026482376)),
        cms.PSet(lowBounds = cms.vdouble(0.56,0.0,90.0), upBounds = cms.vdouble(0.85,1.5,99999),values = cms.vdouble(0.9858675076), uncertainties = cms.vdouble(0.0012204025,0.0012204025)),

        #0 <eta < 1.5, R9 > 0.85 (cat 0)
        cms.PSet(lowBounds = cms.vdouble(0.85,0.0, 0.0), upBounds = cms.vdouble(999,1.5,35.0), values = cms.vdouble(0.8564371935), uncertainties = cms.vdouble(0.0012903214,0.0012903214)),
        cms.PSet(lowBounds = cms.vdouble(0.85,0.0,35.0), upBounds = cms.vdouble(999,1.5,37.0), values = cms.vdouble(0.9594766157), uncertainties = cms.vdouble(0.0012742575,0.0012742575)),
        cms.PSet(lowBounds = cms.vdouble(0.85,0.0,37.0), upBounds = cms.vdouble(999,1.5,40.0), values = cms.vdouble(0.9663358811), uncertainties = cms.vdouble(0.0012090481,0.0012090481)),
        cms.PSet(lowBounds = cms.vdouble(0.85,0.0,40.0), upBounds = cms.vdouble(999,1.5,45.0), values = cms.vdouble(0.9724797256), uncertainties = cms.vdouble(0.0010947335,0.0010947335)),
        cms.PSet(lowBounds = cms.vdouble(0.85,0.0,45.0), upBounds = cms.vdouble(999,1.5,50.0), values = cms.vdouble(0.9763581853), uncertainties = cms.vdouble(0.0015287758,0.0015287758)),
        cms.PSet(lowBounds = cms.vdouble(0.85,0.0,50.0), upBounds = cms.vdouble(999,1.5,60.0), values = cms.vdouble(0.9764434357), uncertainties = cms.vdouble(0.0011321579,0.0011321579)),
        cms.PSet(lowBounds = cms.vdouble(0.85,0.0,60.0), upBounds = cms.vdouble(999,1.5,70.0), values = cms.vdouble(0.9802652970), uncertainties = cms.vdouble(0.0010207420,0.0010207420)),
        cms.PSet(lowBounds = cms.vdouble(0.85,0.0,70.0), upBounds = cms.vdouble(999,1.5,90.0), values = cms.vdouble(0.9840348160), uncertainties = cms.vdouble(0.0010310573,0.0010310573)),
        cms.PSet(lowBounds = cms.vdouble(0.85,0.0,90.0), upBounds = cms.vdouble(999,1.5,99999), values = cms.vdouble(0.9899930822), uncertainties = cms.vdouble(0.0010104819,0.0010104819)),


        #1.5 <eta <3, R9 < 0.85 (cat 3 r9 turn-on bin)
        cms.PSet(lowBounds = cms.vdouble(0.0,1.5, 0.0), upBounds = cms.vdouble(0.85,3.0,35.0), values = cms.vdouble(0.6595211482), uncertainties = cms.vdouble(0.0035675804,0.0035675804)),
        cms.PSet(lowBounds = cms.vdouble(0.0,1.5,35.0), upBounds = cms.vdouble(0.85,3.0,37.0), values = cms.vdouble(0.7445913750), uncertainties = cms.vdouble(0.0023031514,0.0023031514)),
        cms.PSet(lowBounds = cms.vdouble(0.0,1.5,37.0), upBounds = cms.vdouble(0.85,3.0,40.0), values = cms.vdouble(0.7721763963), uncertainties = cms.vdouble(0.0065209097,0.0065209097)),
        cms.PSet(lowBounds = cms.vdouble(0.0,1.5,40.0), upBounds = cms.vdouble(0.85,3.0,45.0), values = cms.vdouble(0.8107180165), uncertainties = cms.vdouble(0.0014154022,0.0014154022)),
        cms.PSet(lowBounds = cms.vdouble(0.0,1.5,45.0), upBounds = cms.vdouble(0.85,3.0,50.0), values = cms.vdouble(0.8439171883), uncertainties = cms.vdouble(0.0036167742,0.0036167742)),
        cms.PSet(lowBounds = cms.vdouble(0.0,1.5,50.0), upBounds = cms.vdouble(0.85,3.0,60.0), values = cms.vdouble(0.8596941548), uncertainties = cms.vdouble(0.0059069771,0.0059069771)),
        cms.PSet(lowBounds = cms.vdouble(0.0,1.5,60.0), upBounds = cms.vdouble(0.85,3.0,70.0), values = cms.vdouble(0.8831719219), uncertainties = cms.vdouble(0.0076900377,0.0076900377)),
        cms.PSet(lowBounds = cms.vdouble(0.0,1.5,70.0), upBounds = cms.vdouble(0.85,3.0,90.0), values = cms.vdouble(0.9172864768), uncertainties = cms.vdouble(0.0075744602,0.0075744602)),
        cms.PSet(lowBounds = cms.vdouble(0.,1.5,90.0), upBounds = cms.vdouble(0.85,3.0,999999), values = cms.vdouble(0.9417547952), uncertainties = cms.vdouble(0.0069369473,0.0069369473)),

        #1.5 <eta <3, 0.85 < R9 < 0.90 (cat 3 r9 plateau bin)
        cms.PSet(lowBounds = cms.vdouble(0.85,1.5, 0.0), upBounds = cms.vdouble(0.9,3.0,35.0), values = cms.vdouble(0.8923336432), uncertainties = cms.vdouble(0.0018534140,0.0018534140)),
        cms.PSet(lowBounds = cms.vdouble(0.85,1.5,35.0), upBounds = cms.vdouble(0.9,3.0,37.0), values = cms.vdouble(0.9732925196), uncertainties = cms.vdouble(0.0010381313,0.0010381313)),
        cms.PSet(lowBounds = cms.vdouble(0.85,1.5,37.0), upBounds = cms.vdouble(0.9,3.0,40.0), values = cms.vdouble(0.9778957733), uncertainties = cms.vdouble(0.0016393361,0.0016393361)),
        cms.PSet(lowBounds = cms.vdouble(0.85,1.5,40.0), upBounds = cms.vdouble(0.9,3.0,45.0), values = cms.vdouble(0.9824372073), uncertainties = cms.vdouble(0.0010001192,0.0010001192)),
        cms.PSet(lowBounds = cms.vdouble(0.85,1.5,45.0), upBounds = cms.vdouble(0.9,3.0,50.0), values = cms.vdouble(0.9867443152), uncertainties = cms.vdouble(0.0016565749,0.0016565749)),
        cms.PSet(lowBounds = cms.vdouble(0.85,1.5,50.0), upBounds = cms.vdouble(0.9,3.0,60.0), values = cms.vdouble(0.9875279678), uncertainties = cms.vdouble(0.0016035797,0.0016035797)),
        cms.PSet(lowBounds = cms.vdouble(0.85,1.5,60.0), upBounds = cms.vdouble(0.9,3.0,70.0), values = cms.vdouble(0.9905722669), uncertainties = cms.vdouble(0.0024178380,0.0024178380)),
        cms.PSet(lowBounds = cms.vdouble(0.85,1.5,70.0), upBounds = cms.vdouble(0.9,3.0,90.0), values = cms.vdouble(0.9875423811), uncertainties = cms.vdouble(0.0018211930,0.0018211930)),
        cms.PSet(lowBounds = cms.vdouble(0.85,1.5,90.0), upBounds = cms.vdouble(0.9,3.0,99999), values = cms.vdouble(0.9939461454), uncertainties = cms.vdouble(0.0014681992,0.0014681992)),

        #1.5 <eta <3, R9 > 0.90 (cat 1)
        cms.PSet(lowBounds = cms.vdouble(0.9,1.5, 0.0), upBounds = cms.vdouble(999,3.0,35.0), values = cms.vdouble(0.8832344791), uncertainties = cms.vdouble(0.0015267508,0.0015267508)),
        cms.PSet(lowBounds = cms.vdouble(0.9,1.5,35.0), upBounds = cms.vdouble(999,3.0,37.0), values = cms.vdouble(0.9780127017), uncertainties = cms.vdouble(0.0010456392,0.0010456392)),
        cms.PSet(lowBounds = cms.vdouble(0.9,1.5,37.0), upBounds = cms.vdouble(999,3.0,40.0), values = cms.vdouble(0.9798867348), uncertainties = cms.vdouble(0.0010097728,0.0010097728)),
        cms.PSet(lowBounds = cms.vdouble(0.9,1.5,40.0), upBounds = cms.vdouble(999,3.0,45.0), values = cms.vdouble(0.9842874232), uncertainties = cms.vdouble(0.0010083699,0.0010083699)),
        cms.PSet(lowBounds = cms.vdouble(0.9,1.5,45.0), upBounds = cms.vdouble(999,3.0,50.0), values = cms.vdouble(0.9884889713), uncertainties = cms.vdouble(0.0015377887,0.0015377887)),
        cms.PSet(lowBounds = cms.vdouble(0.9,1.5,50.0), upBounds = cms.vdouble(999,3.0,60.0), values = cms.vdouble(0.9865944794), uncertainties = cms.vdouble(0.0010052555,0.0010052555)),
        cms.PSet(lowBounds = cms.vdouble(0.9,1.5,60.0), upBounds = cms.vdouble(999,3.0,70.0), values = cms.vdouble(0.9874199891), uncertainties = cms.vdouble(0.0012212860,0.0012212860)),
        cms.PSet(lowBounds = cms.vdouble(0.9,1.5,70.0), upBounds = cms.vdouble(999,3.0,90.0), values = cms.vdouble(0.9901687777), uncertainties = cms.vdouble(0.0023366205,0.0023366205)),
        cms.PSet(lowBounds = cms.vdouble(0.9,1.5,90.0), upBounds = cms.vdouble(999,3.0,99999), values = cms.vdouble(0.9939249798), uncertainties = cms.vdouble(0.0019178013,0.0019178013)), 
        
        cms.PSet(lowBounds = cms.vdouble(0.0,3.,0), upBounds = cms.vdouble(999,999,999999), values = cms.vdouble(1.), uncertainties = cms.vdouble(1.,1.))
        )
    )


subleadTriggerScaleBins = cms.PSet(
    variables = cms.vstring("full5x5_r9","abs(superCluster.eta)","pt"),
    bins = cms.VPSet(
  
        #0 <eta < 1.5, R9<0.56 (cat 2 r9 turn-on bin)
        cms.PSet(lowBounds = cms.vdouble(0.0,0.0,0.0),  upBounds = cms.vdouble(0.56,1.5,28.0), values = cms.vdouble(0.6700566984), uncertainties = cms.vdouble(0.0033591243,0.0033591243)),
        cms.PSet(lowBounds = cms.vdouble(0.0,0.0,28.0), upBounds = cms.vdouble(0.56,1.5,31.0), values = cms.vdouble(0.7965842168), uncertainties = cms.vdouble(0.0029087664,0.0029087664)),
        cms.PSet(lowBounds = cms.vdouble(0.0,0.0,31.0), upBounds = cms.vdouble(0.56,1.5,35.0), values = cms.vdouble(0.8081304828), uncertainties = cms.vdouble(0.0021488574,0.0021488574)),
        cms.PSet(lowBounds = cms.vdouble(0.0,0.0,35.0), upBounds = cms.vdouble(0.56,1.5,40.0), values = cms.vdouble(0.8330735890), uncertainties = cms.vdouble(0.0014600345,0.0014600345)),
        cms.PSet(lowBounds = cms.vdouble(0.0,0.0,40.0), upBounds = cms.vdouble(0.56,1.5,45.0), values = cms.vdouble(0.8651728271), uncertainties = cms.vdouble(0.0017961360,0.0017961360)), 
        cms.PSet(lowBounds = cms.vdouble(0.0,0.0,45.0), upBounds = cms.vdouble(0.56,1.5,50.0), values = cms.vdouble(0.8911488537), uncertainties = cms.vdouble(0.0032747718,0.0032747718)),
        cms.PSet(lowBounds = cms.vdouble(0.0,0.0,50.0), upBounds = cms.vdouble(0.56,1.5,60.0), values = cms.vdouble(0.9011784801), uncertainties = cms.vdouble(0.0053724854,0.0053724854)),
        cms.PSet(lowBounds = cms.vdouble(0.0,0.0,60.0), upBounds = cms.vdouble(0.56,1.5,70.0), values = cms.vdouble(0.9076775812), uncertainties = cms.vdouble(0.0102298395,0.0102298395)),
        cms.PSet(lowBounds = cms.vdouble(0.0,0.0,70.0), upBounds = cms.vdouble(0.56,1.5,90.0), values = cms.vdouble(0.9314218547), uncertainties = cms.vdouble(0.0117083658,0.0117083658)),
        cms.PSet(lowBounds = cms.vdouble(0.0,0.0,90.0), upBounds = cms.vdouble(0.56,1.5,999999), values = cms.vdouble(0.8688854629), uncertainties = cms.vdouble(0.0272240897,0.0272240897)),

        #0 <eta < 1.5, 0.56<R9<0.85 (cat 2 r9 plateau bin)
        cms.PSet(lowBounds = cms.vdouble(0.56,0.0,0.0),  upBounds = cms.vdouble(0.85,1.5,28.0), values = cms.vdouble(0.8894513966), uncertainties = cms.vdouble(0.0036089699,0.0036089699)),
        cms.PSet(lowBounds = cms.vdouble(0.56,0.0,28.0), upBounds = cms.vdouble(0.85,1.5,31.0), values = cms.vdouble(0.9884665549), uncertainties = cms.vdouble(0.0015007956,0.0015007956)),
        cms.PSet(lowBounds = cms.vdouble(0.56,0.0,31.0), upBounds = cms.vdouble(0.85,1.5,35.0), values = cms.vdouble(0.9917398218), uncertainties = cms.vdouble(0.0011683208,0.0011683208)),
        cms.PSet(lowBounds = cms.vdouble(0.56,0.0,35.0), upBounds = cms.vdouble(0.85,1.5,40.0), values = cms.vdouble(0.9933705148), uncertainties = cms.vdouble(0.0010012892,0.0010012892)),
        cms.PSet(lowBounds = cms.vdouble(0.56,0.0,40.0), upBounds = cms.vdouble(0.85,1.5,45.0), values = cms.vdouble(0.9942898736), uncertainties = cms.vdouble(0.0010136787,0.0010136787)),
        cms.PSet(lowBounds = cms.vdouble(0.56,0.0,45.0), upBounds = cms.vdouble(0.85,1.5,50.0), values = cms.vdouble(0.9942864841), uncertainties = cms.vdouble(0.0010026866,0.0010026866)),
        cms.PSet(lowBounds = cms.vdouble(0.56,0.0,50.0), upBounds = cms.vdouble(0.85,1.5,60.0), values = cms.vdouble(0.9944882129), uncertainties = cms.vdouble(0.0011519019,0.0011519019)),
        cms.PSet(lowBounds = cms.vdouble(0.56,0.0,60.0), upBounds = cms.vdouble(0.85,1.5,70.0), values = cms.vdouble(0.9957957201), uncertainties = cms.vdouble(0.0010617143,0.0010617143)),
        cms.PSet(lowBounds = cms.vdouble(0.56,0.0,70.0), upBounds = cms.vdouble(0.85,1.5,90.0), values = cms.vdouble(0.9968300712), uncertainties = cms.vdouble(0.0013330812,0.0013330812)),
        cms.PSet(lowBounds = cms.vdouble(0.56,0.0,90.0), upBounds = cms.vdouble(0.85,1.5,99999), values = cms.vdouble(0.9955940296), uncertainties = cms.vdouble(0.0010014183,0.0010014183)),

        #0 <eta < 1.5, R9>0.85 (cat 0)
        cms.PSet(lowBounds = cms.vdouble(0.85,0.0, 0.0), upBounds = cms.vdouble(999,1.5,28.0),  values = cms.vdouble(0.8954487977), uncertainties = cms.vdouble(0.0017249353,0.0017249353)),
        cms.PSet(lowBounds = cms.vdouble(0.85,0.0,28.0), upBounds = cms.vdouble(999,1.5,31.0), values = cms.vdouble(0.9994025520), uncertainties = cms.vdouble(0.0019219042,0.0019219042)),
        cms.PSet(lowBounds = cms.vdouble(0.85,0.0,31.0), upBounds = cms.vdouble(999,1.5,35.0), values = cms.vdouble(0.9984810955), uncertainties = cms.vdouble(0.0010594654,0.0010594654)),
        cms.PSet(lowBounds = cms.vdouble(0.85,0.0,35.0), upBounds = cms.vdouble(999,1.5,40.0), values = cms.vdouble(0.9994189377), uncertainties = cms.vdouble(0.0010043235,0.0010043235)),
        cms.PSet(lowBounds = cms.vdouble(0.85,0.0,40.0), upBounds = cms.vdouble(999,1.5,45.0), values = cms.vdouble(0.9997816787), uncertainties = cms.vdouble(0.0010001893,0.0010001893)),
        cms.PSet(lowBounds = cms.vdouble(0.85,0.0,45.0), upBounds = cms.vdouble(999,1.5,50.0), values = cms.vdouble(0.9998486516), uncertainties = cms.vdouble(0.0010000204,0.0010000204)),
        cms.PSet(lowBounds = cms.vdouble(0.85,0.0,50.0), upBounds = cms.vdouble(999,1.5,60.0), values = cms.vdouble(0.9998481269), uncertainties = cms.vdouble(0.0010010322,0.0010010322)),
        cms.PSet(lowBounds = cms.vdouble(0.85,0.0,60.0), upBounds = cms.vdouble(999,1.5,70.0), values = cms.vdouble(0.9999123320), uncertainties = cms.vdouble(0.0010002623,0.0010002623)),
        cms.PSet(lowBounds = cms.vdouble(0.85,0.0,70.0), upBounds = cms.vdouble(999,1.5,90.0), values = cms.vdouble(0.9998477863), uncertainties = cms.vdouble(0.0010002682,0.0010002682)),
        cms.PSet(lowBounds = cms.vdouble(0.85,0.0,90.0), upBounds = cms.vdouble(999,1.5,999999), values = cms.vdouble(0.9998191870), uncertainties = cms.vdouble(0.0010000044,0.0010000044)),
    
       
        #1.5 <eta < 3, R9<0.85 (cat 3 r9 turn-on bin)
        cms.PSet(lowBounds = cms.vdouble(0.0,1.5,0.0), upBounds = cms.vdouble(0.85,3.0,28.0), values = cms.vdouble(0.6077667927), uncertainties = cms.vdouble(0.0046122696,0.0046122696)),
        cms.PSet(lowBounds = cms.vdouble(0.0,1.5,28.0), upBounds = cms.vdouble(0.85,3.0,31.0), values = cms.vdouble(0.7029913014), uncertainties = cms.vdouble(0.0043048501,0.0043048501)), 
        cms.PSet(lowBounds = cms.vdouble(0.0,1.5,31.0), upBounds = cms.vdouble(0.85,3.0,35.0), values = cms.vdouble(0.7348248590), uncertainties = cms.vdouble(0.0025178296,0.0025178296)),
        cms.PSet(lowBounds = cms.vdouble(0.0,1.5,35.0), upBounds = cms.vdouble(0.85,3.0,40.0), values = cms.vdouble(0.7751918056), uncertainties = cms.vdouble(0.0022623129,0.0022623129)),
        cms.PSet(lowBounds = cms.vdouble(0.0,1.5,40.0), upBounds = cms.vdouble(0.85,3.0,45.0), values = cms.vdouble(0.8177996767), uncertainties = cms.vdouble(0.0015943345,0.0015943345)),
        cms.PSet(lowBounds = cms.vdouble(0.0,1.5,45.0), upBounds = cms.vdouble(0.85,3.0,50.0), values = cms.vdouble(0.8498648292), uncertainties = cms.vdouble(0.0043238986,0.0043238986)),
        cms.PSet(lowBounds = cms.vdouble(0.0,1.5,50.0), upBounds = cms.vdouble(0.85,3.0,60.0), values = cms.vdouble(0.8663397128), uncertainties = cms.vdouble(0.0053868895,0.0053868895)), 
        cms.PSet(lowBounds = cms.vdouble(0.0,1.5,60.0), upBounds = cms.vdouble(0.85,3.0,70.0), values = cms.vdouble(0.8949914647), uncertainties = cms.vdouble(0.0080114689,0.0080114689)),
        cms.PSet(lowBounds = cms.vdouble(0.0,1.5,70.0), upBounds = cms.vdouble(0.85,3.0,90.0), values = cms.vdouble(0.9172053758), uncertainties = cms.vdouble(0.0072992270,0.0072992270)),
        cms.PSet(lowBounds = cms.vdouble(0.0,1.5,90.0), upBounds = cms.vdouble(0.85,3.0,9999999), values = cms.vdouble(0.9422693522), uncertainties = cms.vdouble(0.0065863094,0.0065863094)),


        #1.5 <eta < 3, 0.85<R9<0.9 (cat 3 r9 plateau bin)
        cms.PSet(lowBounds = cms.vdouble(0.85,1.5,0.0), upBounds = cms.vdouble(0.9,3.0,28.0), values = cms.vdouble(0.9353581676), uncertainties = cms.vdouble(0.0018033312,0.0018033312)),
        cms.PSet(lowBounds = cms.vdouble(0.85,1.5,28.0), upBounds = cms.vdouble(0.9,3.0,31.0), values = cms.vdouble(0.9899824821), uncertainties = cms.vdouble(0.0018037358,0.0018037358)),
        cms.PSet(lowBounds = cms.vdouble(0.85,1.5,31.0), upBounds = cms.vdouble(0.9,3.0,35.0), values = cms.vdouble(0.9871141083), uncertainties = cms.vdouble(0.0013566255,0.0013566255)),
        cms.PSet(lowBounds = cms.vdouble(0.85,1.5,35.0), upBounds = cms.vdouble(0.9,3.0,40.0), values = cms.vdouble(0.9904200761), uncertainties = cms.vdouble(0.0011197038,0.0011197038)),
        cms.PSet(lowBounds = cms.vdouble(0.85,1.5,40.0), upBounds = cms.vdouble(0.9,3.0,45.0), values = cms.vdouble(0.9918951123), uncertainties = cms.vdouble(0.0010012658,0.0010012658)),
        cms.PSet(lowBounds = cms.vdouble(0.85,1.5,45.0), upBounds = cms.vdouble(0.9,3.0,50.0), values = cms.vdouble(0.9917714091), uncertainties = cms.vdouble(0.0010000000,0.0010000000)),
        cms.PSet(lowBounds = cms.vdouble(0.85,1.5,50.0), upBounds = cms.vdouble(0.9,3.0,60.0), values = cms.vdouble(0.9928356527), uncertainties = cms.vdouble(0.0017400600,0.0017400600)),
        cms.PSet(lowBounds = cms.vdouble(0.85,1.5,60.0), upBounds = cms.vdouble(0.9,3.0,70.0), values = cms.vdouble(0.9938830616), uncertainties = cms.vdouble(0.0014184516,0.0014184516)),
        cms.PSet(lowBounds = cms.vdouble(0.85,1.5,70.0), upBounds = cms.vdouble(0.9,3.0,90.0), values = cms.vdouble(0.9943289581), uncertainties = cms.vdouble(0.0012331963,0.0012331963)),
        cms.PSet(lowBounds = cms.vdouble(0.85,1.5,90.0), upBounds = cms.vdouble(0.9,3.0,999999), values = cms.vdouble(0.9953508596), uncertainties = cms.vdouble(0.0012031112,0.0012031112)),


        #1.5 <eta < 3, R9>0.9 (cat 1)
        cms.PSet(lowBounds = cms.vdouble(0.9,1.5,0.0), upBounds = cms.vdouble(999,3.0,28.0), values = cms.vdouble(0.9363550975), uncertainties = cms.vdouble(0.0011873982,0.0011873982)), 
        cms.PSet(lowBounds = cms.vdouble(0.9,1.5,28.0), upBounds = cms.vdouble(999,3.0,31.0), values = cms.vdouble(0.9971266544), uncertainties = cms.vdouble(0.0011095861,0.0011095861)), 
        cms.PSet(lowBounds = cms.vdouble(0.9,1.5,31.0), upBounds = cms.vdouble(999,3.0,35.0), values = cms.vdouble(0.9982863440), uncertainties = cms.vdouble(0.0010000850,0.0010000850)), 
        cms.PSet(lowBounds = cms.vdouble(0.9,1.5,35.0), upBounds = cms.vdouble(999,3.0,40.0), values = cms.vdouble(0.9990711368), uncertainties = cms.vdouble(0.0010000639,0.0010000639)), 
        cms.PSet(lowBounds = cms.vdouble(0.9,1.5,40.0), upBounds = cms.vdouble(999,3.0,45.0), values = cms.vdouble(0.9994109362), uncertainties = cms.vdouble(0.0010001205,0.0010001205)), 
        cms.PSet(lowBounds = cms.vdouble(0.9,1.5,45.0), upBounds = cms.vdouble(999,3.0,50.0), values = cms.vdouble(0.9995441995), uncertainties = cms.vdouble(0.0010016283,0.0010016283)), 
        cms.PSet(lowBounds = cms.vdouble(0.9,1.5,50.0), upBounds = cms.vdouble(999,3.0,60.0), values = cms.vdouble(0.9995876301), uncertainties = cms.vdouble(0.0010000157,0.0010000157)), 
        cms.PSet(lowBounds = cms.vdouble(0.9,1.5,60.0), upBounds = cms.vdouble(999,3.0,70.0), values = cms.vdouble(0.9994394094), uncertainties = cms.vdouble(0.0010479168,0.0010479168)), 
        cms.PSet(lowBounds = cms.vdouble(0.9,1.5,70.0), upBounds = cms.vdouble(999,3.0,90.0), values = cms.vdouble(0.9997564478), uncertainties = cms.vdouble(0.0010000844,0.0010000844)), 
        cms.PSet(lowBounds = cms.vdouble(0.9,1.5,90.0), upBounds = cms.vdouble(999,3.0,9999999), values = cms.vdouble(0.9997131724), uncertainties = cms.vdouble(0.0010003946,0.0010003946)),

        cms.PSet(lowBounds = cms.vdouble(0.0,3.,0), upBounds = cms.vdouble(999,999,999999), values = cms.vdouble(1.), uncertainties = cms.vdouble(1.,1.))
        )
    )

# Prasant Kumar Rout: UL18 Loose Photon IDMVA cut [-0.9] SF and total uncertainties.
looseMvaBins = cms.PSet(
    variables = cms.vstring("abs(superCluster.eta)","full5x5_r9"),
    bins = cms.VPSet(
        cms.PSet( lowBounds = cms.vdouble( 0.0, 0.0  ) , upBounds = cms.vdouble( 1.5, 0.85  ) , values = cms.vdouble( 1.0022 ) , uncertainties = cms.vdouble( 0.0012 )  ) ,
        cms.PSet( lowBounds = cms.vdouble( 0.0, 0.85 ) , upBounds = cms.vdouble( 1.5, 999.0 ) , values = cms.vdouble( 1.0005 ) , uncertainties = cms.vdouble( 0.0033 )  ) ,
        cms.PSet( lowBounds = cms.vdouble( 1.5, 0.0  ) , upBounds = cms.vdouble( 6.0, 0.9   ) , values = cms.vdouble( 1.0058 ) , uncertainties = cms.vdouble( 0.0020 )  ) ,
        cms.PSet( lowBounds = cms.vdouble( 1.5, 0.9  ) , upBounds = cms.vdouble( 6.0, 999.0 ) , values = cms.vdouble( 1.0013 ) , uncertainties = cms.vdouble( 0.0028 ) ) ) )

# RELATIVE shift of sigmaE/E --> 0.05 corresponds to a shift of 5%
sigmaEOverEShiftBins = cms.PSet(
    variables = cms.vstring("abs(superCluster.eta)"),
    bins = cms.VPSet(
                     cms.PSet( lowBounds = cms.vdouble(0.000), upBounds = cms.vdouble(999.), values = cms.vdouble( 0.0 ), uncertainties = cms.vdouble( 0.05 ))
                     )
    )

#Diphoton vertex ID SF (You-Ying's slides page 5 in https://indico.cern.ch/event/996868/contributions/4198278/attachments/2177607/3677444/HggMeeting_25012021.pdf)
RVBins = cms.PSet(
    variables = cms.vstring("pt"),
    bins = cms.VPSet(
        cms.PSet(lowBounds = cms.vdouble(0),   upBounds = cms.vdouble(5),   values = cms.vdouble(0.974039, 1.0271), uncertainties = cms.vdouble(0.0198617, 0.0198617, 0.0207336, 0.0207336)),
        cms.PSet(lowBounds = cms.vdouble(5),   upBounds = cms.vdouble(10),  values = cms.vdouble(0.993597, 1.00755), uncertainties = cms.vdouble(0.0213774, 0.0213774, 0.0252034, 0.0252034)),
        cms.PSet(lowBounds = cms.vdouble(10),  upBounds = cms.vdouble(15),  values = cms.vdouble(0.986321, 1.01945), uncertainties = cms.vdouble(0.0210868, 0.0210868, 0.0299771, 0.0299771)),
        cms.PSet(lowBounds = cms.vdouble(15),  upBounds = cms.vdouble(20),  values = cms.vdouble(0.978842, 1.03884), uncertainties = cms.vdouble(0.017653, 0.017653, 0.0324115, 0.0324115)),
        cms.PSet(lowBounds = cms.vdouble(20),  upBounds = cms.vdouble(30),  values = cms.vdouble(0.978248, 1.05737), uncertainties = cms.vdouble(0.0129109, 0.0129109, 0.0340538, 0.0340538)),
        cms.PSet(lowBounds = cms.vdouble(30),  upBounds = cms.vdouble(40),  values = cms.vdouble(0.987734, 1.05473), uncertainties = cms.vdouble(0.0082854, 0.0082854, 0.0369767, 0.0369767)),
        cms.PSet(lowBounds = cms.vdouble(40),  upBounds = cms.vdouble(50),  values = cms.vdouble(0.992513, 1.0524), uncertainties = cms.vdouble(0.00632247, 0.00632247, 0.0442674, 0.0442674)),
        cms.PSet(lowBounds = cms.vdouble(50),  upBounds = cms.vdouble(60),  values = cms.vdouble(0.993165, 1.0708), uncertainties = cms.vdouble(0.00324972, 0.00324972, 0.0337167, 0.0337167)),
        cms.PSet(lowBounds = cms.vdouble(60),  upBounds = cms.vdouble(80),  values = cms.vdouble(0.995448, 1.06831), uncertainties = cms.vdouble(0.00214167, 0.00214167, 0.032201, 0.032201)),
        cms.PSet(lowBounds = cms.vdouble(80),  upBounds = cms.vdouble(100), values = cms.vdouble(0.99768, 1.05421), uncertainties = cms.vdouble(0.00204705, 0.00204705, 0.0478903, 0.0478903)),
        cms.PSet(lowBounds = cms.vdouble(100), upBounds = cms.vdouble(140), values = cms.vdouble(0.998417, 1.05629), uncertainties = cms.vdouble(0.00109351, 0.00109351, 0.0389869, 0.0389869)),
        cms.PSet(lowBounds = cms.vdouble(140), upBounds = cms.vdouble(200), values = cms.vdouble(0.998583, 1.09708), uncertainties = cms.vdouble(0.000660099, 0.000660099, 0.0461481, 0.0461481)),
        cms.PSet(lowBounds = cms.vdouble(200), upBounds = cms.vdouble(400), values = cms.vdouble(0.999684, 1.04511), uncertainties = cms.vdouble(0.000879131, 0.000879131, 0.125677, 0.125677)),
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


scalesAndSmearingsPrefix = cms.string("EgammaAnalysis/ElectronTools/data/ScalesSmearings/Run2018_29Sep2020_RunFineEtaR9Gain_oldFormat")
#scalesAndSmearingsPrefixForSigmaEOverE = scalesAndSmearingsPrefix

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
          CorrectionFile = cms.FileInPath("flashgg/Systematics/data/SystematicsIDMVA_LegRunII_v1_UL2018.root"),
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

