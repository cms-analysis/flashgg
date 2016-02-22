import FWCore.ParameterSet.Config as cms

photonSmearBins = cms.PSet(
    variables = cms.vstring("abs(superCluster.eta)","r9"),
    bins = cms.VPSet(
            # smearings for rereco 2015, photons,
            # see Fasanella et al., ECAL DPG 17/12/2015, https://indico.cern.ch/event/402608/contribution/1/attachments/1206871/1758949/DPG_reReco_corrections.pdf
            cms.PSet( lowBounds = cms.vdouble(0.000,-999.000 ) , upBounds = cms.vdouble(1.000,0.940   ) , values = cms.vdouble( 0.0105 ) , uncertainties = cms.vdouble( 0.00024652 )  ) ,
            cms.PSet( lowBounds = cms.vdouble(0.000,0.940    ) , upBounds = cms.vdouble(1.000,999.000 ) , values = cms.vdouble( 0.0087 ) , uncertainties = cms.vdouble( 0.00018319 )  ) ,
            cms.PSet( lowBounds = cms.vdouble(1.000,-999.000 ) , upBounds = cms.vdouble(1.500,0.940   ) , values = cms.vdouble( 0.0181 ) , uncertainties = cms.vdouble( 0.00032435 )  ) ,
            cms.PSet( lowBounds = cms.vdouble(1.000,0.940    ) , upBounds = cms.vdouble(1.500,999.000 ) , values = cms.vdouble( 0.0130 ) , uncertainties = cms.vdouble( 0.00098551 )  ) ,
            cms.PSet( lowBounds = cms.vdouble(1.500,-999.000 ) , upBounds = cms.vdouble(2.000,0.940   ) , values = cms.vdouble( 0.0223 ) , uncertainties = cms.vdouble( 0.00045816 )  ) ,
            cms.PSet( lowBounds = cms.vdouble(1.500,0.940    ) , upBounds = cms.vdouble(2.000,999.000 ) , values = cms.vdouble( 0.0204 ) , uncertainties = cms.vdouble( 0.00076386 )  ) ,
            cms.PSet( lowBounds = cms.vdouble(2.000,-999.000 ) , upBounds = cms.vdouble(6.000,0.940   ) , values = cms.vdouble( 0.0260 ) , uncertainties = cms.vdouble( 0.00061517 )  ) ,
            cms.PSet( lowBounds = cms.vdouble(2.000,0.940    ) , upBounds = cms.vdouble(6.000,999.000 ) , values = cms.vdouble( 0.0231 ) , uncertainties = cms.vdouble( 0.00042066 )  ) ,
                    ))

photonScaleBinsData = scaleBinsRereco = cms.PSet(
    variables = cms.vstring("abs(superCluster.eta)","r9"),
    bins = cms.VPSet(
            # scale for rereco 2015, photons
            # see Fasanella et al., ECAL DPG 17/12/2015, https://indico.cern.ch/event/402608/contribution/1/attachments/1206871/1758949/DPG_reReco_corrections.pdf
            # w.r.t. the numbers in slide 28, the scale is computed as provided_scale - 1.
            cms.PSet( lowBounds = cms.vdouble(0.000 , -999.000 ) , upBounds = cms.vdouble(1.000 , 0.940   ) , values = cms.vdouble( -0.0002  ) ,  uncertainties = cms.vdouble( 0.00050 )   ) ,
            cms.PSet( lowBounds = cms.vdouble(0.000 , 0.940    ) , upBounds = cms.vdouble(1.000 , 999.000 ) , values = cms.vdouble( -0.00438  ) , uncertainties = cms.vdouble( 0.00050 )   ) ,
            cms.PSet( lowBounds = cms.vdouble(1.000 , -999.000 ) , upBounds = cms.vdouble(1.500 , 0.940   ) , values = cms.vdouble( 0.00672  ) ,  uncertainties = cms.vdouble( 0.00120 )   ) ,
            cms.PSet( lowBounds = cms.vdouble(1.000 , 0.940    ) , upBounds = cms.vdouble(1.500 , 999.000 ) , values = cms.vdouble( -0.00488 ) ,  uncertainties = cms.vdouble( 0.00060 )   ) ,
            cms.PSet( lowBounds = cms.vdouble(1.500 , -999.000 ) , upBounds = cms.vdouble(2.000 , 0.940   ) , values = cms.vdouble( 0.00736  ) ,  uncertainties = cms.vdouble( 0.00200 )   ) ,
            cms.PSet( lowBounds = cms.vdouble(1.500 , 0.940    ) , upBounds = cms.vdouble(2.000 , 999.000 ) , values = cms.vdouble( -0.0007  ) ,  uncertainties = cms.vdouble( 0.00300 )   ) ,
            cms.PSet( lowBounds = cms.vdouble(2.000 , -999.000 ) , upBounds = cms.vdouble(6.000 , 0.940   ) , values = cms.vdouble( 0.00503  ) ,  uncertainties = cms.vdouble( 0.00200 )   ) ,
            cms.PSet( lowBounds = cms.vdouble(2.000 , 0.940    ) , upBounds = cms.vdouble(6.000 , 999.000 ) , values = cms.vdouble( -0.00229  ) , uncertainties = cms.vdouble( 0.00300 )   ) ,
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
