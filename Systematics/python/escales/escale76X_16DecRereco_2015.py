import FWCore.ParameterSet.Config as cms

raise Exception,"This should not be used anymore, you should put useEGMTools(process) in your config instead"

photonSmearBins = cms.PSet(
    variables = cms.vstring("abs(superCluster.eta)","r9"),
    bins = cms.VPSet(
            # smearings for rereco 2015, photons,
            # see Fasanella et al., ECAL DPG 17/12/2015, https://indico.cern.ch/event/402608/contribution/1/attachments/1206871/1758949/DPG_reReco_corrections.pdf
        cms.PSet(lowBounds=cms.vdouble(0.000,-999.000), upBounds=cms.vdouble(1.000,0.940),   values=cms.vdouble(0.0094), uncertainties=cms.vdouble(0.0005)),
        cms.PSet(lowBounds=cms.vdouble(0.000,0.940),    upBounds=cms.vdouble(1.000,999.000), values=cms.vdouble(0.0080), uncertainties=cms.vdouble(0.0012)),
        cms.PSet(lowBounds=cms.vdouble(1.000,-999.000), upBounds=cms.vdouble(1.500,0.940),   values=cms.vdouble(0.0183), uncertainties=cms.vdouble(0.0021)),
        cms.PSet(lowBounds=cms.vdouble(1.000,0.940),    upBounds=cms.vdouble(1.500,999.000), values=cms.vdouble(0.0115), uncertainties=cms.vdouble(0.0005)),
        cms.PSet(lowBounds=cms.vdouble(1.500,-999.000), upBounds=cms.vdouble(2.000,0.940),   values=cms.vdouble(0.0201), uncertainties=cms.vdouble(0.0016)),
        cms.PSet(lowBounds=cms.vdouble(1.500,0.940),    upBounds=cms.vdouble(2.000,999.000), values=cms.vdouble(0.0221), uncertainties=cms.vdouble(0.0006)),
        cms.PSet(lowBounds=cms.vdouble(2.000,-999.000), upBounds=cms.vdouble(6.000,0.940),   values=cms.vdouble(0.0268), uncertainties=cms.vdouble(0.0008)),
        cms.PSet(lowBounds=cms.vdouble(2.000,0.940),    upBounds=cms.vdouble(6.000,999.000), values=cms.vdouble(0.0230), uncertainties=cms.vdouble(0.0010)),
        ))

from flashgg.Systematics.ecalElfTools import getRunDependentScaleBins

# photonScaleBinsData = getRunDependentScaleBins('EgammaAnalysis/ElectronTools/data/76X_16DecRereco_2015_scales.dat')
photonScaleBinsData = getRunDependentScaleBins('flashgg/Systematics/data/76X_16DecRereco_2015_scales.dat')

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
