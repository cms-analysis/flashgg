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

#JTao: slide 34 of https://indico.cern.ch/event/850506/contributions/3606914/attachments/1927946/3192290/201910_Zmmg_RhoBugFixed.pdf with 2018 data and MC samples, with new shower-shape and isolation corrections applied and photon PT > 20 GeV
electronVetoBins = cms.PSet(
    variables = cms.vstring("abs(superCluster.eta)","full5x5_r9"),
    bins = cms.VPSet(
        cms.PSet( lowBounds = cms.vdouble( 0.0, 0.00 ) , upBounds = cms.vdouble( 1.5, 0.85 ) , values = cms.vdouble( 0.9819 ) , uncertainties = cms.vdouble( 0.0020 )  ) ,
        cms.PSet( lowBounds = cms.vdouble( 0.0, 0.85 ) , upBounds = cms.vdouble( 1.5, 999. ) , values = cms.vdouble( 0.9931 ) , uncertainties = cms.vdouble( 0.0005 )  ) ,
        cms.PSet( lowBounds = cms.vdouble( 1.5, 0.00 ) , upBounds = cms.vdouble( 6.0, 0.90 ) , values = cms.vdouble( 0.9680 ) , uncertainties = cms.vdouble( 0.0069 )  ) ,
        cms.PSet( lowBounds = cms.vdouble( 1.5, 0.90 ) , upBounds = cms.vdouble( 6.0, 999. ) , values = cms.vdouble( 0.9785 ) , uncertainties = cms.vdouble( 0.0018 )  )
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


#for full 2018 (Era2018_RR-17Sep2018_v2-legacyRun2FullV2-v0, rereco A-D dataset).  Trigger scale factors for use without HLT applied in MC
leadTriggerScaleBins = cms.PSet(
    variables = cms.vstring("full5x5_r9","abs(superCluster.eta)","pt"),
    bins = cms.VPSet(

        #0 <eta < 1.5, R9<0.54 (cat 2 r9 turn-on bin)
        cms.PSet(lowBounds = cms.vdouble(0.0,0.0,0.0), upBounds = cms.vdouble(0.54,1.5,35.0), values = cms.vdouble(0.8020693032), uncertainties = cms.vdouble(0.0108370679,0.0108370679)),
        cms.PSet(lowBounds = cms.vdouble(0.0,0.0,35.0), upBounds = cms.vdouble(0.54,1.5,37.0), values = cms.vdouble(0.9155469694), uncertainties = cms.vdouble(0.0042798703,0.0042798703)),
        cms.PSet(lowBounds = cms.vdouble(0.0,0.0,37.0), upBounds = cms.vdouble(0.54,1.5,40.0), values = cms.vdouble(0.9266209669), uncertainties = cms.vdouble(0.0020932481,0.0020932481)), 
        cms.PSet(lowBounds = cms.vdouble(0.0,0.0,40.0), upBounds = cms.vdouble(0.54,1.5,45.0), values = cms.vdouble(0.9295708491), uncertainties = cms.vdouble(0.0012322936,0.0012322936)),
        cms.PSet(lowBounds = cms.vdouble(0.0,0.0,45.0), upBounds = cms.vdouble(0.54,1.5,50.0), values = cms.vdouble(0.9303265907), uncertainties = cms.vdouble(0.0024324819,0.0024324819)),
        cms.PSet(lowBounds = cms.vdouble(0.0,0.0,50.0), upBounds = cms.vdouble(0.54,1.5,60.0), values = cms.vdouble(0.9416725453), uncertainties = cms.vdouble(0.0057677517,0.0057677517)),
        cms.PSet(lowBounds = cms.vdouble(0.0,0.0,60.0), upBounds = cms.vdouble(0.54,1.5,70.0), values = cms.vdouble(0.9503439663), uncertainties = cms.vdouble(0.0143137857,0.0143137857)),
        cms.PSet(lowBounds = cms.vdouble(0.0,0.0,70.0), upBounds = cms.vdouble(0.54,1.5,90.0), values = cms.vdouble(0.9574423947), uncertainties = cms.vdouble(0.0120714856,0.0120714856)),
        cms.PSet(lowBounds = cms.vdouble(0.0,0.0,90.0), upBounds = cms.vdouble(0.54,1.5,99999), values = cms.vdouble(0.9523387414), uncertainties = cms.vdouble(0.0267745254,0.0267745254)),

        #0 <eta < 1.5, 0.54 < R9<0.85 (cat 2 r9 plateau-bin) 
        cms.PSet(lowBounds = cms.vdouble(0.54,0.0,0.0), upBounds = cms.vdouble(0.85,1.5,35.0), values = cms.vdouble(0.8524002552), uncertainties = cms.vdouble(0.0010000000,0.0010000000)),
        cms.PSet(lowBounds = cms.vdouble(0.54,0.0,35.0), upBounds = cms.vdouble(0.85,1.5,37.0), values = cms.vdouble(0.9476119412), uncertainties = cms.vdouble(0.0010000000,0.0010000000)),
        cms.PSet(lowBounds = cms.vdouble(0.54,0.0,37.0), upBounds = cms.vdouble(0.85,1.5,40.0), values = cms.vdouble(0.9537999031), uncertainties = cms.vdouble(0.0010000000,0.0010000000)),
        cms.PSet(lowBounds = cms.vdouble(0.54,0.0,40.0), upBounds = cms.vdouble(0.85,1.5,45.0), values = cms.vdouble(0.9606177455), uncertainties = cms.vdouble(0.0010000000,0.0010000000)),
        cms.PSet(lowBounds = cms.vdouble(0.54,0.0,45.0), upBounds = cms.vdouble(0.85,1.5,50.0), values = cms.vdouble(0.9652220994), uncertainties = cms.vdouble(0.0010000000,0.0010000000)),
        cms.PSet(lowBounds = cms.vdouble(0.54,0.0,50.0), upBounds = cms.vdouble(0.85,1.5,60.0), values = cms.vdouble(0.9677140340), uncertainties = cms.vdouble(0.0010000000,0.0010000000)),
        cms.PSet(lowBounds = cms.vdouble(0.54,0.0,60.0), upBounds = cms.vdouble(0.85,1.5,70.0), values = cms.vdouble(0.9714560889), uncertainties = cms.vdouble(0.0012234955,0.0012234955)),
        cms.PSet(lowBounds = cms.vdouble(0.54,0.0,70.0), upBounds = cms.vdouble(0.85,1.5,90.0), values = cms.vdouble(0.9792025583), uncertainties = cms.vdouble(0.0021872047,0.0021872047)),
        cms.PSet(lowBounds = cms.vdouble(0.54,0.0,90.0), upBounds = cms.vdouble(0.85,1.5,99999), values = cms.vdouble(0.9846367363), uncertainties = cms.vdouble(0.0016488000,0.0016488000)),

        #0 <eta < 1.5, R9>0.85 (cat 0)
        cms.PSet(lowBounds = cms.vdouble(0.85,0.0,0.0), upBounds = cms.vdouble(999,1.5,35.0), values = cms.vdouble(0.8855419724), uncertainties = cms.vdouble(0.0010000000,0.0010000000)),
        cms.PSet(lowBounds = cms.vdouble(0.85,0.0,35.0), upBounds = cms.vdouble(999,1.5,37.0), values = cms.vdouble(0.9599693694), uncertainties = cms.vdouble(0.0012218737,0.0012218737)),
        cms.PSet(lowBounds = cms.vdouble(0.85,0.0,37.0), upBounds = cms.vdouble(999,1.5,40.0), values = cms.vdouble(0.9665558231), uncertainties = cms.vdouble(0.0010000000,0.0010000000)),
        cms.PSet(lowBounds = cms.vdouble(0.85,0.0,40.0), upBounds = cms.vdouble(999,1.5,45.0), values = cms.vdouble(0.9729324583), uncertainties = cms.vdouble(0.0010000000,0.0010000000)),
        cms.PSet(lowBounds = cms.vdouble(0.85,0.0,45.0), upBounds = cms.vdouble(999,1.5,50.0), values = cms.vdouble(0.9784734824), uncertainties = cms.vdouble(0.0010000000,0.0010000000)),
        cms.PSet(lowBounds = cms.vdouble(0.85,0.0,50.0), upBounds = cms.vdouble(999,1.5,60.0), values = cms.vdouble(0.9806145301), uncertainties = cms.vdouble(0.0010000000,0.0010000000)),
        cms.PSet(lowBounds = cms.vdouble(0.85,0.0,60.0), upBounds = cms.vdouble(999,1.5,70.0), values = cms.vdouble(0.9814731790), uncertainties = cms.vdouble(0.0010000000,0.0010000000)),
        cms.PSet(lowBounds = cms.vdouble(0.85,0.0,70.0), upBounds = cms.vdouble(999,1.5,90.0), values = cms.vdouble(0.9830517669), uncertainties = cms.vdouble(0.0010000000,0.0010000000)),
        cms.PSet(lowBounds = cms.vdouble(0.85,0.0,90.0), upBounds = cms.vdouble(999,1.5,99999), values = cms.vdouble(0.9903382393), uncertainties = cms.vdouble(0.0010000000,0.0010000000)),

        #1.5 <eta <3, R9<0.84 (cat 3 r9 turn-on bin)
        cms.PSet(lowBounds = cms.vdouble(0.0,1.5,0.0), upBounds = cms.vdouble(0.84,3.0,35.0), values = cms.vdouble(0.8656534670), uncertainties = cms.vdouble(0.0019334223,0.0019334223)),
        cms.PSet(lowBounds = cms.vdouble(0.0,1.5,35.0), upBounds = cms.vdouble(0.84,3.0,37.0), values = cms.vdouble(0.9482903730), uncertainties = cms.vdouble(0.0014018856,0.0014018856)),
        cms.PSet(lowBounds = cms.vdouble(0.0,1.5,37.0), upBounds = cms.vdouble(0.84,3.0,40.0), values = cms.vdouble(0.9560211271), uncertainties = cms.vdouble(0.0022246022,0.0022246022)),
        cms.PSet(lowBounds = cms.vdouble(0.0,1.5,40.0), upBounds = cms.vdouble(0.84,3.0,45.0), values = cms.vdouble(0.9656682311), uncertainties = cms.vdouble(0.0010000000,0.0010000000)),
        cms.PSet(lowBounds = cms.vdouble(0.0,1.5,45.0), upBounds = cms.vdouble(0.84,3.0,50.0), values = cms.vdouble(0.9714389718), uncertainties = cms.vdouble(0.0011571498,0.0011571498)),
        cms.PSet(lowBounds = cms.vdouble(0.0,1.5,50.0), upBounds = cms.vdouble(0.84,3.0,60.0), values = cms.vdouble(0.9727868307), uncertainties = cms.vdouble(0.0027277860,0.0027277860)),
        cms.PSet(lowBounds = cms.vdouble(0.0,1.5,60.0), upBounds = cms.vdouble(0.84,3.0,70.0), values = cms.vdouble(0.9716310829), uncertainties = cms.vdouble(0.0053509351,0.0053509351)),
        cms.PSet(lowBounds = cms.vdouble(0.0,1.5,70.0), upBounds = cms.vdouble(0.84,3.0,90.0), values = cms.vdouble(0.9749593833), uncertainties = cms.vdouble(0.0043547018,0.0043547018)),
        cms.PSet(lowBounds = cms.vdouble(0.,1.5,90.0), upBounds = cms.vdouble(0.84,3.0,999999), values = cms.vdouble(0.9781950680), uncertainties = cms.vdouble(0.0051854430,0.0051854430)),

        #1.5 <eta <3, 0.84< R9<0.90 (cat 3 r9 plateau bin)
        cms.PSet(lowBounds = cms.vdouble(0.84,1.5,0.0), upBounds = cms.vdouble(0.9,3.0,35.0), values = cms.vdouble(0.8790818182), uncertainties = cms.vdouble(0.0011724123,0.0011724123)),
        cms.PSet(lowBounds = cms.vdouble(0.84,1.5,35.0), upBounds = cms.vdouble(0.9,3.0,37.0), values = cms.vdouble(0.9730292260), uncertainties = cms.vdouble(0.0010000000,0.0010000000)),
        cms.PSet(lowBounds = cms.vdouble(0.84,1.5,37.0), upBounds = cms.vdouble(0.9,3.0,40.0), values = cms.vdouble(0.9788279858), uncertainties = cms.vdouble(0.0010000000,0.0010000000)),
        cms.PSet(lowBounds = cms.vdouble(0.84,1.5,40.0), upBounds = cms.vdouble(0.9,3.0,45.0), values = cms.vdouble(0.9824542096), uncertainties = cms.vdouble(0.0010000000,0.0010000000)),
        cms.PSet(lowBounds = cms.vdouble(0.84,1.5,45.0), upBounds = cms.vdouble(0.9,3.0,50.0), values = cms.vdouble(0.9841886360), uncertainties = cms.vdouble(0.0010000000,0.0010000000)),
        cms.PSet(lowBounds = cms.vdouble(0.84,1.5,50.0), upBounds = cms.vdouble(0.9,3.0,60.0), values = cms.vdouble(0.9848402006), uncertainties = cms.vdouble(0.0011284238,0.0011284238)),
        cms.PSet(lowBounds = cms.vdouble(0.84,1.5,60.0), upBounds = cms.vdouble(0.9,3.0,70.0), values = cms.vdouble(0.9875812187), uncertainties = cms.vdouble(0.0018108700,0.0018108700)),
        cms.PSet(lowBounds = cms.vdouble(0.84,1.5,70.0), upBounds = cms.vdouble(0.9,3.0,90.0), values = cms.vdouble(0.9885875172), uncertainties = cms.vdouble(0.0016709154,0.0016709154)),
        cms.PSet(lowBounds = cms.vdouble(0.84,1.5,90.0), upBounds = cms.vdouble(0.9,3.0,99999), values = cms.vdouble(0.9941926927), uncertainties = cms.vdouble(0.0014826775,0.0014826775)),
       

        #1.5 <eta <3, R9>0.90 (cat 1)
        cms.PSet(lowBounds = cms.vdouble(0.9,1.5,0.0), upBounds = cms.vdouble(999,3.0,35.0), values = cms.vdouble(0.8425282382), uncertainties = cms.vdouble(0.0010416570,0.0010416570)),
        cms.PSet(lowBounds = cms.vdouble(0.9,1.5,35.0), upBounds = cms.vdouble(999,3.0,37.0), values = cms.vdouble(0.9729130893), uncertainties = cms.vdouble(0.0010000000,0.0010000000)),
        cms.PSet(lowBounds = cms.vdouble(0.9,1.5,37.0), upBounds = cms.vdouble(999,3.0,40.0), values = cms.vdouble(0.9781362068), uncertainties = cms.vdouble(0.0010000000,0.0010000000)),
        cms.PSet(lowBounds = cms.vdouble(0.9,1.5,40.0), upBounds = cms.vdouble(999,3.0,45.0), values = cms.vdouble(0.9826974813), uncertainties = cms.vdouble(0.0010000000,0.0010000000)),
        cms.PSet(lowBounds = cms.vdouble(0.9,1.5,45.0), upBounds = cms.vdouble(999,3.0,50.0), values = cms.vdouble(0.9858318192), uncertainties = cms.vdouble(0.0010000000,0.0010000000)),
        cms.PSet(lowBounds = cms.vdouble(0.9,1.5,50.0), upBounds = cms.vdouble(999,3.0,60.0), values = cms.vdouble(0.9794794506), uncertainties = cms.vdouble(0.0010000000,0.0010000000)),
        cms.PSet(lowBounds = cms.vdouble(0.9,1.5,60.0), upBounds = cms.vdouble(999,3.0,70.0), values = cms.vdouble(0.9925981035), uncertainties = cms.vdouble(0.0010298667,0.0010298667)),
        cms.PSet(lowBounds = cms.vdouble(0.9,1.5,70.0), upBounds = cms.vdouble(999,3.0,90.0), values = cms.vdouble(0.9920874219), uncertainties = cms.vdouble(0.0010000000,0.0010000000)),
        cms.PSet(lowBounds = cms.vdouble(0.9,1.5,90.0), upBounds = cms.vdouble(999,3.0,99999), values = cms.vdouble(0.9942726543), uncertainties = cms.vdouble(0.0010000000,0.0010000000)),
        
        cms.PSet(lowBounds = cms.vdouble(0.0,3.,0), upBounds = cms.vdouble(999,999,999999), values = cms.vdouble(1.), uncertainties = cms.vdouble(1.,1.))
        )
    )


subleadTriggerScaleBins = cms.PSet(
    variables = cms.vstring("full5x5_r9","abs(superCluster.eta)","pt"),
    bins = cms.VPSet(
      
        #0 <eta < 1.5, R9<0.54 (cat 2 r9 turn-on bin)
        cms.PSet(lowBounds = cms.vdouble(0.0,0.0,0.0), upBounds = cms.vdouble(0.54,1.5,28.0), values = cms.vdouble(0.8884153471), uncertainties = cms.vdouble(0.0035078397,0.0035078397)),
        cms.PSet(lowBounds = cms.vdouble(0.0,0.0,28.0), upBounds = cms.vdouble(0.54,1.5,31.0), values = cms.vdouble(0.9517603567), uncertainties = cms.vdouble(0.0012683293,0.0012683293)),
        cms.PSet(lowBounds = cms.vdouble(0.0,0.0,31.0), upBounds = cms.vdouble(0.54,1.5,35.0), values = cms.vdouble(0.9690496396), uncertainties = cms.vdouble(0.0014644761,0.0014644761)),
        cms.PSet(lowBounds = cms.vdouble(0.0,0.0,35.0), upBounds = cms.vdouble(0.54,1.5,40.0), values = cms.vdouble(0.9717160162), uncertainties = cms.vdouble(0.0013420893,0.0013420893)),
        cms.PSet(lowBounds = cms.vdouble(0.0,0.0,40.0), upBounds = cms.vdouble(0.54,1.5,45.0), values = cms.vdouble(0.9796815413), uncertainties = cms.vdouble(0.0010000000,0.0010000000)), 
        cms.PSet(lowBounds = cms.vdouble(0.0,0.0,45.0), upBounds = cms.vdouble(0.54,1.5,50.0), values = cms.vdouble(0.9843574751), uncertainties = cms.vdouble(0.0019132259,0.0019132259)),
        cms.PSet(lowBounds = cms.vdouble(0.0,0.0,50.0), upBounds = cms.vdouble(0.54,1.5,60.0), values = cms.vdouble(0.9844776733), uncertainties = cms.vdouble(0.0040644219,0.0040644219)),
        cms.PSet(lowBounds = cms.vdouble(0.0,0.0,60.0), upBounds = cms.vdouble(0.54,1.5,70.0), values = cms.vdouble(0.9873999094), uncertainties = cms.vdouble(0.0053801091,0.0053801091)),
        cms.PSet(lowBounds = cms.vdouble(0.0,0.0,70.0), upBounds = cms.vdouble(0.54,1.5,90.0), values = cms.vdouble(0.9933255120), uncertainties = cms.vdouble(0.0063638347,0.0063638347)),
        cms.PSet(lowBounds = cms.vdouble(0.0,0.0,90.0), upBounds = cms.vdouble(0.54,1.5,999999), values = cms.vdouble(0.9804395842), uncertainties = cms.vdouble(0.0310825604,0.019560)),

        #0 <eta < 1.5, 0.54<R9<0.85 (cat 2 r9 plateau bin)
        cms.PSet(lowBounds = cms.vdouble(0.54,0.0,0.0), upBounds = cms.vdouble(0.85,1.5,28.0), values = cms.vdouble(0.9184639454), uncertainties = cms.vdouble(0.0010000000,0.0010000000)),
        cms.PSet(lowBounds = cms.vdouble(0.54,0.0,28.0), upBounds = cms.vdouble(0.85,1.5,31.0), values = cms.vdouble(0.9950437401), uncertainties = cms.vdouble(0.0010000000,0.0010000000)),
        cms.PSet(lowBounds = cms.vdouble(0.54,0.0,31.0), upBounds = cms.vdouble(0.85,1.5,35.0), values = cms.vdouble(0.9917999437), uncertainties = cms.vdouble(0.0010000000,0.0010000000)),
        cms.PSet(lowBounds = cms.vdouble(0.54,0.0,35.0), upBounds = cms.vdouble(0.85,1.5,40.0), values = cms.vdouble(0.9939153322), uncertainties = cms.vdouble(0.0010000000,0.0010000000)),
        cms.PSet(lowBounds = cms.vdouble(0.54,0.0,40.0), upBounds = cms.vdouble(0.85,1.5,45.0), values = cms.vdouble(0.9947390674), uncertainties = cms.vdouble(0.0010000000,0.0010000000)),
        cms.PSet(lowBounds = cms.vdouble(0.54,0.0,45.0), upBounds = cms.vdouble(0.85,1.5,50.0), values = cms.vdouble(0.9945127334), uncertainties = cms.vdouble(0.0010000000,0.0010000000)),
        cms.PSet(lowBounds = cms.vdouble(0.54,0.0,50.0), upBounds = cms.vdouble(0.85,1.5,60.0), values = cms.vdouble(0.9953081007), uncertainties = cms.vdouble(0.0010000000,0.0010000000)),
        cms.PSet(lowBounds = cms.vdouble(0.54,0.0,60.0), upBounds = cms.vdouble(0.85,1.5,70.0), values = cms.vdouble(0.9968578311), uncertainties = cms.vdouble(0.0024445012,0.0024445012)),
        cms.PSet(lowBounds = cms.vdouble(0.54,0.0,70.0), upBounds = cms.vdouble(0.85,1.5,90.0), values = cms.vdouble(0.9956645030), uncertainties = cms.vdouble(0.0010000000,0.0010000000)),
        cms.PSet(lowBounds = cms.vdouble(0.54,0.0,90.0), upBounds = cms.vdouble(0.85,1.5,99999), values = cms.vdouble(0.9957331016), uncertainties = cms.vdouble(0.0010000000,0.0010000000)),

        #0 <eta < 1.5, R9>0.85 (cat 0)
        cms.PSet(lowBounds = cms.vdouble(0.85,0.0,0.0), upBounds = cms.vdouble(999,1.5,28.0), values = cms.vdouble(0.9482245748), uncertainties = cms.vdouble(0.0024089227,0.0024089227)),
        cms.PSet(lowBounds = cms.vdouble(0.85,0.0,28.0), upBounds = cms.vdouble(999,1.5,31.0), values = cms.vdouble(0.9999020755), uncertainties = cms.vdouble(0.0010000000,0.0010000000)),
        cms.PSet(lowBounds = cms.vdouble(0.85,0.0,31.0), upBounds = cms.vdouble(999,1.5,35.0), values = cms.vdouble(0.9999120130), uncertainties = cms.vdouble(0.0010000000,0.0010000000)),
        cms.PSet(lowBounds = cms.vdouble(0.85,0.0,35.0), upBounds = cms.vdouble(999,1.5,40.0), values = cms.vdouble(0.9999573970), uncertainties = cms.vdouble(0.0026996598,0.0026996598)),
        cms.PSet(lowBounds = cms.vdouble(0.85,0.0,40.0), upBounds = cms.vdouble(999,1.5,45.0), values = cms.vdouble(0.9999830622), uncertainties = cms.vdouble(0.0010000000,0.0010000000)),
        cms.PSet(lowBounds = cms.vdouble(0.85,0.0,45.0), upBounds = cms.vdouble(999,1.5,50.0), values = cms.vdouble(0.9999880454), uncertainties = cms.vdouble(0.0010000000,0.0010000000)),
        cms.PSet(lowBounds = cms.vdouble(0.85,0.0,50.0), upBounds = cms.vdouble(999,1.5,60.0), values = cms.vdouble(0.9999938661), uncertainties = cms.vdouble(0.0010000000,0.0010000000)),
        cms.PSet(lowBounds = cms.vdouble(0.85,0.0,60.0), upBounds = cms.vdouble(999,1.5,70.0), values = cms.vdouble(0.9999867470), uncertainties = cms.vdouble(0.0010000000,0.0010000000)),
        cms.PSet(lowBounds = cms.vdouble(0.85,0.0,70.0), upBounds = cms.vdouble(999,1.5,90.0),values = cms.vdouble(0.9999972153), uncertainties = cms.vdouble(0.0010000000,0.0010000000)),
        cms.PSet(lowBounds = cms.vdouble(0.85,0.0,90.0), upBounds = cms.vdouble(999,1.5,999999), values = cms.vdouble(0.9999550063), uncertainties = cms.vdouble(0.0010000000,0.0010000000)),
    
        
        #1.5 <eta < 3, R9<0.84 (cat 3 r9 turn-on bin)
        cms.PSet(lowBounds = cms.vdouble(0.0,1.5,0.0), upBounds = cms.vdouble(0.84,3.0,28.0), values = cms.vdouble(0.9024458938), uncertainties = cms.vdouble(0.0028471768,0.0028471768)), 
        cms.PSet(lowBounds = cms.vdouble(0.0,1.5,28.0), upBounds = cms.vdouble(0.84,3.0,31.0), values = cms.vdouble(0.9706150264), uncertainties = cms.vdouble(0.0046523365,0.0046523365)), 
        cms.PSet(lowBounds = cms.vdouble(0.0,1.5,31.0), upBounds = cms.vdouble(0.84,3.0,35.0), values = cms.vdouble(0.9697381479), uncertainties = cms.vdouble(0.0034363710,0.0034363710)),
        cms.PSet(lowBounds = cms.vdouble(0.0,1.5,35.0), upBounds = cms.vdouble(0.84,3.0,40.0), values = cms.vdouble(0.9723474700), uncertainties = cms.vdouble(0.0010000000,0.0010000000)),
        cms.PSet(lowBounds = cms.vdouble(0.0,1.5,40.0), upBounds = cms.vdouble(0.84,3.0,45.0), values = cms.vdouble(0.9776980417), uncertainties = cms.vdouble(0.0010000000,0.0010000000)),
        cms.PSet(lowBounds = cms.vdouble(0.0,1.5,45.0), upBounds = cms.vdouble(0.84,3.0,50.0), values = cms.vdouble(0.9796103567), uncertainties = cms.vdouble(0.0012977858,0.0012977858)),
        cms.PSet(lowBounds = cms.vdouble(0.0,1.5,50.0), upBounds = cms.vdouble(0.84,3.0,60.0), values = cms.vdouble(0.9761773122), uncertainties = cms.vdouble(0.0028290744,0.0028290744)), 
        cms.PSet(lowBounds = cms.vdouble(0.0,1.5,60.0), upBounds = cms.vdouble(0.84,3.0,70.0), values = cms.vdouble(0.9767940972), uncertainties = cms.vdouble(0.0038576930,0.0038576930)), 
        cms.PSet(lowBounds = cms.vdouble(0.0,1.5,70.0), upBounds = cms.vdouble(0.84,3.0,90.0), values = cms.vdouble(0.9820266168), uncertainties = cms.vdouble(0.0037175753,0.0037175753)),
        cms.PSet(lowBounds = cms.vdouble(0.0,1.5,90.0), upBounds = cms.vdouble(0.84,3.0,9999999), values = cms.vdouble(0.9786811223), uncertainties = cms.vdouble(0.0020066146,0.0020066146)),


        #1.5 <eta < 3, 0.84<R9<0.9 (cat 3 r9 plateau bin)
        cms.PSet(lowBounds = cms.vdouble(0.84,1.5,0.0), upBounds = cms.vdouble(0.9,3.0,28.0), values = cms.vdouble(0.9187774368), uncertainties = cms.vdouble(0.0016372591,0.0016372591)),
        cms.PSet(lowBounds = cms.vdouble(0.84,1.5,28.0), upBounds = cms.vdouble(0.9,3.0,31.0), values = cms.vdouble(0.9912278169), uncertainties = cms.vdouble(0.0018807126,0.0018807126)),
        cms.PSet(lowBounds = cms.vdouble(0.84,1.5,31.0), upBounds = cms.vdouble(0.9,3.0,35.0), values = cms.vdouble(0.9901680254), uncertainties = cms.vdouble(0.0010000000,0.0010000000)),
        cms.PSet(lowBounds = cms.vdouble(0.84,1.5,35.0), upBounds = cms.vdouble(0.9,3.0,40.0), values = cms.vdouble(0.9928358746), uncertainties = cms.vdouble(0.0010000000,0.0010000000)),
        cms.PSet(lowBounds = cms.vdouble(0.84,1.5,40.0), upBounds = cms.vdouble(0.9,3.0,45.0), values = cms.vdouble(0.9935834235), uncertainties = cms.vdouble(0.0010000000,0.0010000000)),
        cms.PSet(lowBounds = cms.vdouble(0.84,1.5,45.0), upBounds = cms.vdouble(0.9,3.0,50.0), values = cms.vdouble(0.9935942765), uncertainties = cms.vdouble(0.0010000000,0.0010000000)),
        cms.PSet(lowBounds = cms.vdouble(0.84,1.5,50.0), upBounds = cms.vdouble(0.9,3.0,60.0), values = cms.vdouble(0.9940966594), uncertainties = cms.vdouble(0.0010000000,0.0010000000)),
        cms.PSet(lowBounds = cms.vdouble(0.84,1.5,60.0), upBounds = cms.vdouble(0.9,3.0,70.0), values = cms.vdouble(0.9944723732), uncertainties = cms.vdouble(0.0014841931,0.0014841931)),
        cms.PSet(lowBounds = cms.vdouble(0.84,1.5,70.0), upBounds = cms.vdouble(0.9,3.0,90.0), values = cms.vdouble(0.9966527166), uncertainties = cms.vdouble(0.0010000000,0.0010000000)),
        cms.PSet(lowBounds = cms.vdouble(0.84,1.5,90.0), upBounds = cms.vdouble(0.9,3.0,999999), values = cms.vdouble(0.9963826143), uncertainties = cms.vdouble(0.0013134571,0.0013134571)),


        #1.5 <eta < 3, R9>0.9 (cat 1)
        cms.PSet(lowBounds = cms.vdouble(0.9,1.5,0.0), upBounds = cms.vdouble(999,3.0,28.0), values = cms.vdouble(0.9126823457), uncertainties = cms.vdouble(0.0030246593,0.0030246593)), 
        cms.PSet(lowBounds = cms.vdouble(0.9,1.5,28.0), upBounds = cms.vdouble(999,3.0,31.0), values = cms.vdouble(0.9997440713), uncertainties = cms.vdouble(0.0010000000,0.0010000000)), 
        cms.PSet(lowBounds = cms.vdouble(0.9,1.5,31.0), upBounds = cms.vdouble(999,3.0,35.0), values = cms.vdouble(0.9996615281), uncertainties = cms.vdouble(0.0010000000,0.0010000000)), 
        cms.PSet(lowBounds = cms.vdouble(0.9,1.5,35.0), upBounds = cms.vdouble(999,3.0,40.0), values = cms.vdouble(0.9997655760), uncertainties = cms.vdouble(0.0010000000,0.0010000000)), 
        cms.PSet(lowBounds = cms.vdouble(0.9,1.5,40.0), upBounds = cms.vdouble(999,3.0,45.0), values = cms.vdouble(0.9998579409), uncertainties = cms.vdouble(0.0010000000,0.0010000000)), 
        cms.PSet(lowBounds = cms.vdouble(0.9,1.5,45.0), upBounds = cms.vdouble(999,3.0,50.0), values = cms.vdouble(0.9998936352), uncertainties = cms.vdouble(0.0010000000,0.0010000000)), 
        cms.PSet(lowBounds = cms.vdouble(0.9,1.5,50.0), upBounds = cms.vdouble(999,3.0,60.0), values = cms.vdouble(0.9998729705), uncertainties = cms.vdouble(0.0010000000,0.0010000000)), 
        cms.PSet(lowBounds = cms.vdouble(0.9,1.5,60.0), upBounds = cms.vdouble(999,3.0,70.0), values = cms.vdouble(0.9997750397), uncertainties = cms.vdouble(0.0010000000,0.0010000000)), 
        cms.PSet(lowBounds = cms.vdouble(0.9,1.5,70.0), upBounds = cms.vdouble(999,3.0,90.0), values = cms.vdouble(0.9999724487), uncertainties = cms.vdouble(0.0010000000,0.0010000000)), 
        cms.PSet(lowBounds = cms.vdouble(0.9,1.5,90.0), upBounds = cms.vdouble(999,3.0,9999999), values = cms.vdouble(0.9999686824), uncertainties = cms.vdouble(0.0010000000,0.0010000000)),

        cms.PSet(lowBounds = cms.vdouble(0.0,3.,0), upBounds = cms.vdouble(999,999,999999), values = cms.vdouble(1.), uncertainties = cms.vdouble(1.,1.))
        )
    )

# from Arnab via Martina 10/03/2016
looseMvaBins = cms.PSet(
    variables = cms.vstring("abs(superCluster.eta)","full5x5_r9"),
    bins = cms.VPSet(
        cms.PSet( lowBounds = cms.vdouble( 0.0, 0.0  ) , upBounds = cms.vdouble( 1.5, 0.85  ) , values = cms.vdouble( 0.9999 ) , uncertainties = cms.vdouble( 0.0001 )  ) ,
        cms.PSet( lowBounds = cms.vdouble( 0.0, 0.85 ) , upBounds = cms.vdouble( 1.5, 999.0 ) , values = cms.vdouble( 1.0003 ) , uncertainties = cms.vdouble( 0.0000 )  ) ,
        cms.PSet( lowBounds = cms.vdouble( 1.5, 0.0  ) , upBounds = cms.vdouble( 6.0, 0.9   ) , values = cms.vdouble( 1.0003 ) , uncertainties = cms.vdouble( 0.0000 )  ) ,
        cms.PSet( lowBounds = cms.vdouble( 1.5, 0.9  ) , upBounds = cms.vdouble( 6.0, 999.0 ) , values = cms.vdouble( 1.0004 ) , uncertainties = cms.vdouble( 0.0000 ) ) ) ) 

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
        cms.PSet(lowBounds = cms.vdouble(0),   upBounds = cms.vdouble(5),   values = cms.vdouble(0.993565, 1.00664), uncertainties = cms.vdouble(0.0225433, 0.0225433, 0.023279, 0.023279)),
        cms.PSet(lowBounds = cms.vdouble(5),   upBounds = cms.vdouble(10),  values = cms.vdouble(1.01856, 0.978069), uncertainties = cms.vdouble(0.0240601, 0.0240601, 0.0284297, 0.0284297)),
        cms.PSet(lowBounds = cms.vdouble(10),  upBounds = cms.vdouble(15),  values = cms.vdouble(1.01375, 0.980025), uncertainties = cms.vdouble(0.0240097, 0.0240097, 0.0348869, 0.0348869)),
        cms.PSet(lowBounds = cms.vdouble(15),  upBounds = cms.vdouble(20),  values = cms.vdouble(1.00379, 0.99301), uncertainties = cms.vdouble(0.02055, 0.02055, 0.0379198, 0.0379198)),
        cms.PSet(lowBounds = cms.vdouble(20),  upBounds = cms.vdouble(30),  values = cms.vdouble(0.989918, 1.02713), uncertainties = cms.vdouble(0.0153448, 0.0153448, 0.0412937, 0.0412937)),
        cms.PSet(lowBounds = cms.vdouble(30),  upBounds = cms.vdouble(40),  values = cms.vdouble(0.982182, 1.07812), uncertainties = cms.vdouble(0.0095869, 0.0095869, 0.0420487, 0.0420487)),
        cms.PSet(lowBounds = cms.vdouble(40),  upBounds = cms.vdouble(50),  values = cms.vdouble(0.984574, 1.10344), uncertainties = cms.vdouble(0.00598576, 0.00598576, 0.0401913, 0.0401913)),
        cms.PSet(lowBounds = cms.vdouble(50),  upBounds = cms.vdouble(60),  values = cms.vdouble(0.988021, 1.11469), uncertainties = cms.vdouble(0.00382511, 0.00382511, 0.0367491, 0.0367491)),
        cms.PSet(lowBounds = cms.vdouble(60),  upBounds = cms.vdouble(80),  values = cms.vdouble(0.992915, 1.11058), uncertainties = cms.vdouble(0.00318309, 0.00318309, 0.0497911, 0.0497911)),
        cms.PSet(lowBounds = cms.vdouble(80),  upBounds = cms.vdouble(100), values = cms.vdouble(0.996389, 1.08094), uncertainties = cms.vdouble(0.00186217, 0.00186217, 0.0419005, 0.0419005)),
        cms.PSet(lowBounds = cms.vdouble(100), upBounds = cms.vdouble(140), values = cms.vdouble(0.998132, 1.0714), uncertainties = cms.vdouble(0.0010437, 0.0010437, 0.0401096, 0.0401096)),
        cms.PSet(lowBounds = cms.vdouble(140), upBounds = cms.vdouble(200), values = cms.vdouble(0.997934, 1.12722), uncertainties = cms.vdouble(0.000736251, 0.000736251, 0.0467527, 0.0467527)),
        cms.PSet(lowBounds = cms.vdouble(200), upBounds = cms.vdouble(400), values = cms.vdouble(0.998515, 1.17222), uncertainties = cms.vdouble(0.000719232, 0.000719232, 0.0862914, 0.0862914)),
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


scalesAndSmearingsPrefix = cms.string("EgammaAnalysis/ElectronTools/data/ScalesSmearings/Run2018_Step2Closure_CoarseEtaR9Gain_v2")
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
          CorrectionFile = cms.FileInPath("flashgg/Systematics/data/SystematicsIDMVA_LegRunII_v1_2018.root"),
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

