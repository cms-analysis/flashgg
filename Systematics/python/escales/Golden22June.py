import FWCore.ParameterSet.Config as cms

photonSmearBins = cms.PSet(
    variables = cms.vstring("abs(superCluster.eta)","r9"),
    bins = cms.VPSet(
        cms.PSet(lowBounds=cms.vdouble(0.000,-999.000), upBounds=cms.vdouble(1.000,0.940),   values=cms.vdouble(0.0088), uncertainties=cms.vdouble(0.0003)),
        cms.PSet(lowBounds=cms.vdouble(0.000,0.940),    upBounds=cms.vdouble(1.000,999.000), values=cms.vdouble(0.0075), uncertainties=cms.vdouble(0.0003)),
        cms.PSet(lowBounds=cms.vdouble(1.000,-999.000), upBounds=cms.vdouble(1.500,0.940),   values=cms.vdouble(0.0148), uncertainties=cms.vdouble(0.0004)),
        cms.PSet(lowBounds=cms.vdouble(1.000,0.940),    upBounds=cms.vdouble(1.500,999.000), values=cms.vdouble(0.0116), uncertainties=cms.vdouble(0.0012)),
        cms.PSet(lowBounds=cms.vdouble(1.500,-999.000), upBounds=cms.vdouble(2.000,0.940),   values=cms.vdouble(0.0192), uncertainties=cms.vdouble(0.0006)),
        cms.PSet(lowBounds=cms.vdouble(1.500,0.940),    upBounds=cms.vdouble(2.000,999.000), values=cms.vdouble(0.0170), uncertainties=cms.vdouble(0.0009)),
        cms.PSet(lowBounds=cms.vdouble(2.000,-999.000), upBounds=cms.vdouble(6.000,0.940),   values=cms.vdouble(0.0220), uncertainties=cms.vdouble(0.0006)),
        cms.PSet(lowBounds=cms.vdouble(2.000,0.940),    upBounds=cms.vdouble(6.000,999.000), values=cms.vdouble(0.0200), uncertainties=cms.vdouble(0.0004)),
        ))

from flashgg.Systematics.ecalElfTools import getRunDependentScaleBins

photonScaleBinsData = getRunDependentScaleBins('flashgg/Systematics/data/Golden22June_scales.dat')

photonScaleUncertBins = cms.PSet(
    variables = cms.vstring("abs(superCluster.eta)","r9"),
    bins = cms.VPSet(
        ## only uncertainties here. scales loaded by SystematicsCustomize according to process.photonScaleBinsData
            cms.PSet( lowBounds = cms.vdouble(0.000 , -999.000 ) , upBounds = cms.vdouble(1.000 , 0.940   ) , values = cms.vdouble( 0. ) ,  uncertainties = cms.vdouble( 0.00050 )   ) ,
            cms.PSet( lowBounds = cms.vdouble(0.000 , 0.940    ) , upBounds = cms.vdouble(1.000 , 999.000 ) , values = cms.vdouble( 0.  ) , uncertainties = cms.vdouble( 0.00050 )   ) ,
            cms.PSet( lowBounds = cms.vdouble(1.000 , -999.000 ) , upBounds = cms.vdouble(1.500 , 0.940   ) , values = cms.vdouble( 0. ) ,  uncertainties = cms.vdouble( 0.00120 )   ) ,
            cms.PSet( lowBounds = cms.vdouble(1.000 , 0.940    ) , upBounds = cms.vdouble(1.500 , 999.000 ) , values = cms.vdouble( 0. ) ,  uncertainties = cms.vdouble( 0.00060 )   ) ,
            cms.PSet( lowBounds = cms.vdouble(1.500 , -999.000 ) , upBounds = cms.vdouble(2.000 , 0.940   ) , values = cms.vdouble( 0. ) ,  uncertainties = cms.vdouble( 0.00200 )   ) ,
            cms.PSet( lowBounds = cms.vdouble(1.500 , 0.940    ) , upBounds = cms.vdouble(2.000 , 999.000 ) , values = cms.vdouble( 0. ) ,  uncertainties = cms.vdouble( 0.00300 )   ) ,
            cms.PSet( lowBounds = cms.vdouble(2.000 , -999.000 ) , upBounds = cms.vdouble(6.000 , 0.940   ) , values = cms.vdouble( 0. ) ,  uncertainties = cms.vdouble( 0.00200 )   ) ,
            cms.PSet( lowBounds = cms.vdouble(2.000 , 0.940    ) , upBounds = cms.vdouble(6.000 , 999.000 ) , values = cms.vdouble( 0.  ) , uncertainties = cms.vdouble( 0.00300 )   ) ,
                    ))
