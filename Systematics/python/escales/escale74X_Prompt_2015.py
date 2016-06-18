import FWCore.ParameterSet.Config as cms

photonSmearBins = cms.PSet(
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


photonScaleBinsData = cms.PSet(
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
