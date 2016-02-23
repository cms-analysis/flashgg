import FWCore.ParameterSet.Config as cms

# default energy scale uncertainties and smearings from 76X re-reco
from flashgg.Systematics.escales.escale76X_16DecRereco_2015 import photonSmearBins, photonScaleUncertBins 

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

# slide 5 of https://indico.cern.ch/event/489840/session/1/contribution/17/attachments/1232312/1807794/20160223UpdateOnZmmgForHgg_preselectionIssues.pdf
electronVetoBins = cms.PSet(
    variables = cms.vstring("abs(superCluster.eta)","r9"),
    bins = cms.VPSet(
        cms.PSet( lowBounds = cms.vdouble( 0.0, 0.00 ) , upBounds = cms.vdouble( 1.5, 0.85 ) , values = cms.vdouble( 0.9904 ) , uncertainties = cms.vdouble( 0.0045 )  ) ,
        cms.PSet( lowBounds = cms.vdouble( 0.0, 0.85 ) , upBounds = cms.vdouble( 1.5, 999. ) , values = cms.vdouble( 0.9978 ) , uncertainties = cms.vdouble( 0.0011 )  ) ,
        cms.PSet( lowBounds = cms.vdouble( 1.5, 0.00 ) , upBounds = cms.vdouble( 6.0, 0.90 ) , values = cms.vdouble( 1.0095 ) , uncertainties = cms.vdouble( 0.0163 )  ) ,
        cms.PSet( lowBounds = cms.vdouble( 1.5, 0.90 ) , upBounds = cms.vdouble( 6.0, 999. ) , values = cms.vdouble( 0.9989 ) , uncertainties = cms.vdouble( 0.0028 )  ) 
        )
    )

#for 74x MC uses trigger efficiency not just scale factor since the trigger isn't in the MC

leadTriggerScaleBins = cms.PSet(
    variables = cms.vstring("r9","abs(superCluster.eta)","pt"),
    bins = cms.VPSet(
        #cat0
        cms.PSet(lowBounds = cms.vdouble(0.94,0,0), upBounds = cms.vdouble(999,1.5,30), values = cms.vdouble(0), uncertainties = cms.vdouble(0,0)),
        cms.PSet(lowBounds = cms.vdouble(0.94,0,30), upBounds = cms.vdouble(999,1.5,33.33), values = cms.vdouble(0.956219), uncertainties = cms.vdouble(0.00277459,0.00266194)),
        cms.PSet(lowBounds = cms.vdouble(0.94,0,33.33), upBounds = cms.vdouble(999,1.5,35), values = cms.vdouble(0.995639), uncertainties = cms.vdouble(0.00111805,0.00436078)),
        cms.PSet(lowBounds = cms.vdouble(0.94,0,35), upBounds = cms.vdouble(999,1.5,40), values = cms.vdouble(0.997032), uncertainties = cms.vdouble(0.000421703,0.000385255)),
        cms.PSet(lowBounds = cms.vdouble(0.94,0,40), upBounds = cms.vdouble(999,1.5,45), values = cms.vdouble(0.998391), uncertainties = cms.vdouble(0.000257508,0.000257508)),
        cms.PSet(lowBounds = cms.vdouble(0.94,0,45), upBounds = cms.vdouble(999,1.5,50), values = cms.vdouble(0.998741), uncertainties = cms.vdouble(0.000302614,0.000261119)),
        cms.PSet(lowBounds = cms.vdouble(0.94,0,50), upBounds = cms.vdouble(999,1.5,60), values = cms.vdouble(0.999679), uncertainties = cms.vdouble(0.000250081,0.000321458)),
        cms.PSet(lowBounds = cms.vdouble(0.94,0,60), upBounds = cms.vdouble(999,1.5,70), values = cms.vdouble(1), uncertainties = cms.vdouble(0.00071693,0)),
        cms.PSet(lowBounds = cms.vdouble(0.94,0,70), upBounds = cms.vdouble(999,1.5,90), values = cms.vdouble(1), uncertainties = cms.vdouble(0.00145938,0)),
        cms.PSet(lowBounds = cms.vdouble(0.94,0,90), upBounds = cms.vdouble(999,1.5,120), values = cms.vdouble(1), uncertainties = cms.vdouble(0.0032519,0)),
        cms.PSet(lowBounds = cms.vdouble(0.94,0,120), upBounds = cms.vdouble(999,1.5,150), values = cms.vdouble(1), uncertainties = cms.vdouble(0.00797462,0)),
        cms.PSet(lowBounds = cms.vdouble(0.94,0,150), upBounds = cms.vdouble(999,1.5,10000), values = cms.vdouble(1), uncertainties = cms.vdouble(0.00930901,0)),
        #cat1
        cms.PSet(lowBounds = cms.vdouble(0,0,0), upBounds = cms.vdouble(0.94,1.5,30), values = cms.vdouble(0), uncertainties = cms.vdouble(0,0)),
        cms.PSet(lowBounds = cms.vdouble(0,0,30), upBounds = cms.vdouble(0.94,1.5,33.33), values = cms.vdouble(0.632284), uncertainties = cms.vdouble(0.0204243,0.0168544)),
        cms.PSet(lowBounds = cms.vdouble(0,0,33.33), upBounds = cms.vdouble(0.94,1.5,35), values = cms.vdouble(0.925289), uncertainties = cms.vdouble(0.00320364,0.00312216)),
        cms.PSet(lowBounds = cms.vdouble(0,0,35), upBounds = cms.vdouble(0.94,1.5,40), values = cms.vdouble(0.990161), uncertainties = cms.vdouble(0.000728186,0.000705861)),
        cms.PSet(lowBounds = cms.vdouble(0,0,40), upBounds = cms.vdouble(0.94,1.5,45), values = cms.vdouble(0.9956), uncertainties = cms.vdouble(0.000386337,0.000386337)),
        cms.PSet(lowBounds = cms.vdouble(0,0,45), upBounds = cms.vdouble(0.94,1.5,50), values = cms.vdouble(0.996948), uncertainties = cms.vdouble(0.000442592,0.000403434)),
        cms.PSet(lowBounds = cms.vdouble(0,0,50), upBounds = cms.vdouble(0.94,1.5,60), values = cms.vdouble(0.99712), uncertainties = cms.vdouble(0.0005534,0.0005534)),
        cms.PSet(lowBounds = cms.vdouble(0,0,60), upBounds = cms.vdouble(0.94,1.5,70), values = cms.vdouble(0.996314), uncertainties = cms.vdouble(0.00128866,0.00104443)),
        cms.PSet(lowBounds = cms.vdouble(0,0,70), upBounds = cms.vdouble(0.94,1.5,90), values = cms.vdouble(0.997495), uncertainties = cms.vdouble(0.0014445,0.0014445)),
        cms.PSet(lowBounds = cms.vdouble(0,0,90), upBounds = cms.vdouble(0.94,1.5,120), values = cms.vdouble(1), uncertainties = cms.vdouble(0.00334379,0)),
        cms.PSet(lowBounds = cms.vdouble(0,0,120), upBounds = cms.vdouble(0.94,1.5,150), values = cms.vdouble(1), uncertainties = cms.vdouble(0.0100245,0)),
        cms.PSet(lowBounds = cms.vdouble(0,0,150), upBounds = cms.vdouble(0.94,1.5,10000), values = cms.vdouble(1), uncertainties = cms.vdouble(0.0141262,0)),
        #cat2
        cms.PSet(lowBounds = cms.vdouble(0.94,1.5,0), upBounds = cms.vdouble(999,6,30), values = cms.vdouble(0), uncertainties = cms.vdouble(0,0)),
        cms.PSet(lowBounds = cms.vdouble(0.94,1.5,30), upBounds = cms.vdouble(999,6,33.33), values = cms.vdouble(0.994933), uncertainties = cms.vdouble(0.00260634,0.00506682)),
        cms.PSet(lowBounds = cms.vdouble(0.94,1.5,33.33), upBounds = cms.vdouble(999,6,35), values = cms.vdouble(0.998649), uncertainties = cms.vdouble(0,0.000515804)),
        cms.PSet(lowBounds = cms.vdouble(0.94,1.5,35), upBounds = cms.vdouble(999,6,40), values = cms.vdouble(0.998545), uncertainties = cms.vdouble(0.000749831,0.000557251)),
        cms.PSet(lowBounds = cms.vdouble(0.94,1.5,40), upBounds = cms.vdouble(999,6,45), values = cms.vdouble(0.996542), uncertainties = cms.vdouble(0.00140904,0.00140904)),
        cms.PSet(lowBounds = cms.vdouble(0.94,1.5,45), upBounds = cms.vdouble(999,6,50), values = cms.vdouble(0.995326), uncertainties = cms.vdouble(0.00240807,0.00178999)),
        cms.PSet(lowBounds = cms.vdouble(0.94,1.5,50), upBounds = cms.vdouble(999,6,60), values = cms.vdouble(1), uncertainties = cms.vdouble(0.00730364,0)),
        cms.PSet(lowBounds = cms.vdouble(0.94,1.5,60), upBounds = cms.vdouble(999,6,70), values = cms.vdouble(1), uncertainties = cms.vdouble(0.0114313,0)),
        cms.PSet(lowBounds = cms.vdouble(0.94,1.5,70), upBounds = cms.vdouble(999,6,90), values = cms.vdouble(1), uncertainties = cms.vdouble(0.0239662,0)),
        cms.PSet(lowBounds = cms.vdouble(0.94,1.5,90), upBounds = cms.vdouble(999,6,120), values = cms.vdouble(1), uncertainties = cms.vdouble(0.0521317,0)),
        cms.PSet(lowBounds = cms.vdouble(0.94,1.5,120), upBounds = cms.vdouble(999,6,10000), values = cms.vdouble(1), uncertainties = cms.vdouble(0.0806517,0)),
        #cat3
        cms.PSet(lowBounds = cms.vdouble(0,1.5,0), upBounds = cms.vdouble(0.94,6,30), values = cms.vdouble(0), uncertainties = cms.vdouble(0,0)),
        cms.PSet(lowBounds = cms.vdouble(0,1.5,30), upBounds = cms.vdouble(0.94,6,33.33), values = cms.vdouble(0.792515), uncertainties = cms.vdouble(0.00907658,0.0089043)),
        cms.PSet(lowBounds = cms.vdouble(0,1.5,33.33), upBounds = cms.vdouble(0.94,6,35), values = cms.vdouble(0.964653), uncertainties = cms.vdouble(0.00536436,0.00490242)),
        cms.PSet(lowBounds = cms.vdouble(0,1.5,35), upBounds = cms.vdouble(0.94,6,40), values = cms.vdouble(0.97578), uncertainties = cms.vdouble(0.0023594,0.00222999)),
        cms.PSet(lowBounds = cms.vdouble(0,1.5,40), upBounds = cms.vdouble(0.94,6,45), values = cms.vdouble(0.974525), uncertainties = cms.vdouble(0.0025812,0.00242506)),
        cms.PSet(lowBounds = cms.vdouble(0,1.5,45), upBounds = cms.vdouble(0.94,6,50), values = cms.vdouble(0.982105), uncertainties = cms.vdouble(0.00321228,0.00287424)),
        cms.PSet(lowBounds = cms.vdouble(0,1.5,50), upBounds = cms.vdouble(0.94,6,60), values = cms.vdouble(0.987108), uncertainties = cms.vdouble(0.00361022,0.00304097)),
        cms.PSet(lowBounds = cms.vdouble(0,1.5,60), upBounds = cms.vdouble(0.94,6,70), values = cms.vdouble(0.971181), uncertainties = cms.vdouble(0.0113546,0.00899819)),
        cms.PSet(lowBounds = cms.vdouble(0,1.5,70), upBounds = cms.vdouble(0.94,6,90), values = cms.vdouble(0.984453), uncertainties = cms.vdouble(0,0.0073096)),
        cms.PSet(lowBounds = cms.vdouble(0,1.5,90), upBounds = cms.vdouble(0.94,6,120), values = cms.vdouble(0.987206), uncertainties = cms.vdouble(0.0126855,0.0126855)),
        cms.PSet(lowBounds = cms.vdouble(0,1.5,120), upBounds = cms.vdouble(0.94,6,150), values = cms.vdouble(1), uncertainties = cms.vdouble(0.0808998,0)),
        cms.PSet(lowBounds = cms.vdouble(0,1.5,150), upBounds = cms.vdouble(0.94,6,10000), values = cms.vdouble(1), uncertainties = cms.vdouble(0.113917,0)),
        )
    )


subleadTriggerScaleBins = cms.PSet(
    variables = cms.vstring("r9","abs(superCluster.eta)","pt"),
    bins = cms.VPSet(
        #cat0
        cms.PSet(lowBounds = cms.vdouble(0.94,0,0), upBounds = cms.vdouble(999,1.5,20.), values = cms.vdouble(0), uncertainties = cms.vdouble(0,0)),
        cms.PSet(lowBounds = cms.vdouble(0.94,0,20), upBounds = cms.vdouble(999,1.5,22.5), values = cms.vdouble(0.98189), uncertainties = cms.vdouble(0.00578631,0.00473515)),
        cms.PSet(lowBounds = cms.vdouble(0.94,0,22.5), upBounds = cms.vdouble(999,1.5,25), values = cms.vdouble(0.979904), uncertainties = cms.vdouble(0.00442411,0.00419499)),
        cms.PSet(lowBounds = cms.vdouble(0.94,0,25), upBounds = cms.vdouble(999,1.5,27.5), values = cms.vdouble(0.989441), uncertainties = cms.vdouble(0,0.00214419)),
        cms.PSet(lowBounds = cms.vdouble(0.94,0,27.5), upBounds = cms.vdouble(999,1.5,30), values = cms.vdouble(0.993172), uncertainties = cms.vdouble(0.00167577,0.00149272)),
        cms.PSet(lowBounds = cms.vdouble(0.94,0,30), upBounds = cms.vdouble(999,1.5,33.33), values = cms.vdouble(0.993713), uncertainties = cms.vdouble(0.00109087,0.000978728)),
        cms.PSet(lowBounds = cms.vdouble(0.94,0,33.33), upBounds = cms.vdouble(999,1.5,35), values = cms.vdouble(0.995689), uncertainties = cms.vdouble(0.0011327,0.000965103)),
        cms.PSet(lowBounds = cms.vdouble(0.94,0,35), upBounds = cms.vdouble(999,1.5,40), values = cms.vdouble(0.996936), uncertainties = cms.vdouble(0.000435325,0.000397681)),
        cms.PSet(lowBounds = cms.vdouble(0.94,0,40), upBounds = cms.vdouble(999,1.5,45), values = cms.vdouble(0.998445), uncertainties = cms.vdouble(0.000269435,0.00155496)),
        cms.PSet(lowBounds = cms.vdouble(0.94,0,45), upBounds = cms.vdouble(999,1.5,50), values = cms.vdouble(0.998718), uncertainties = cms.vdouble(0.000308196,0.000266155)),
        cms.PSet(lowBounds = cms.vdouble(0.94,0,50), upBounds = cms.vdouble(999,1.5,60), values = cms.vdouble(0.999654), uncertainties = cms.vdouble(0.000238223,0.000164415)),
        cms.PSet(lowBounds = cms.vdouble(0.94,0,60), upBounds = cms.vdouble(999,1.5,70), values = cms.vdouble(1), uncertainties = cms.vdouble(0.00078206,0)),
        cms.PSet(lowBounds = cms.vdouble(0.94,0,70), upBounds = cms.vdouble(999,1.5,90), values = cms.vdouble(1), uncertainties = cms.vdouble(0.00155038,0)),
        cms.PSet(lowBounds = cms.vdouble(0.94,0,90), upBounds = cms.vdouble(999,1.5,120), values = cms.vdouble(1), uncertainties = cms.vdouble(0.0033556,0)),
        cms.PSet(lowBounds = cms.vdouble(0.94,0,120), upBounds = cms.vdouble(999,1.5,150), values = cms.vdouble(1), uncertainties = cms.vdouble(0.00819353,0)),
        cms.PSet(lowBounds = cms.vdouble(0.94,0,150), upBounds = cms.vdouble(999,1.5,10000), values = cms.vdouble(1), uncertainties = cms.vdouble(0.00938149,0)),
        #cat1
        cms.PSet(lowBounds = cms.vdouble(0,0,0), upBounds = cms.vdouble(0.94,1.5,20.), values = cms.vdouble(0), uncertainties = cms.vdouble(0,0)),
        cms.PSet(lowBounds = cms.vdouble(0,0,20), upBounds = cms.vdouble(0.94,1.5,22.5), values = cms.vdouble(0.873381), uncertainties = cms.vdouble(0.0108781,0.0105409)),
        cms.PSet(lowBounds = cms.vdouble(0,0,22.5), upBounds = cms.vdouble(0.94,1.5,25), values = cms.vdouble(0.960687), uncertainties = cms.vdouble(0.00466068,0.00445531)),
        cms.PSet(lowBounds = cms.vdouble(0,0,25), upBounds = cms.vdouble(0.94,1.5,27.5), values = cms.vdouble(0.982718), uncertainties = cms.vdouble(0.00293048,0.00361682)),
        cms.PSet(lowBounds = cms.vdouble(0,0,27.5), upBounds = cms.vdouble(0.94,1.5,30), values = cms.vdouble(0.986115), uncertainties = cms.vdouble(0.00201698,0.00201698)),
        cms.PSet(lowBounds = cms.vdouble(0,0,30), upBounds = cms.vdouble(0.94,1.5,33.33), values = cms.vdouble(0.988927), uncertainties = cms.vdouble(0.00106401,0.000999238)),
        cms.PSet(lowBounds = cms.vdouble(0,0,33.33), upBounds = cms.vdouble(0.94,1.5,35), values = cms.vdouble(0.992961), uncertainties = cms.vdouble(0.00107275,0.000974655)),
        cms.PSet(lowBounds = cms.vdouble(0,0,35), upBounds = cms.vdouble(0.94,1.5,40), values = cms.vdouble(0.993573), uncertainties = cms.vdouble(0.000511681,0.000486147)),
        cms.PSet(lowBounds = cms.vdouble(0,0,40), upBounds = cms.vdouble(0.94,1.5,45), values = cms.vdouble(0.99472), uncertainties = cms.vdouble(0.000425713,0.000425713)),
        cms.PSet(lowBounds = cms.vdouble(0,0,45), upBounds = cms.vdouble(0.94,1.5,50), values = cms.vdouble(0.996097), uncertainties = cms.vdouble(0.000503322,0.000463812)),
        cms.PSet(lowBounds = cms.vdouble(0,0,50), upBounds = cms.vdouble(0.94,1.5,60), values = cms.vdouble(0.995321), uncertainties = cms.vdouble(0.000758499,0.000684666)),
        cms.PSet(lowBounds = cms.vdouble(0,0,60), upBounds = cms.vdouble(0.94,1.5,70), values = cms.vdouble(0.994265), uncertainties = cms.vdouble(0.00173579,0.00144496)),
        cms.PSet(lowBounds = cms.vdouble(0,0,70), upBounds = cms.vdouble(0.94,1.5,90), values = cms.vdouble(0.997302), uncertainties = cms.vdouble(0.00186958,0.00127299)),
        cms.PSet(lowBounds = cms.vdouble(0,0,90), upBounds = cms.vdouble(0.94,1.5,120), values = cms.vdouble(1), uncertainties = cms.vdouble(0.00346301,0)),
        cms.PSet(lowBounds = cms.vdouble(0,0,120), upBounds = cms.vdouble(0.94,1.5,150), values = cms.vdouble(1), uncertainties = cms.vdouble(0.0100028,0)),
        cms.PSet(lowBounds = cms.vdouble(0,0,150), upBounds = cms.vdouble(0.94,1.5,10000), values = cms.vdouble(1), uncertainties = cms.vdouble(0.0154601,0)),
        #cat2
        cms.PSet(lowBounds = cms.vdouble(0.94,1.5,0), upBounds = cms.vdouble(999,6,20.), values = cms.vdouble(0), uncertainties = cms.vdouble(0,0)),
        cms.PSet(lowBounds = cms.vdouble(0.94,1.5,20), upBounds = cms.vdouble(999,6,22.5), values = cms.vdouble(0.999999), uncertainties = cms.vdouble(0.00181133,0.00181133)),
        cms.PSet(lowBounds = cms.vdouble(0.94,1.5,22.5), upBounds = cms.vdouble(999,6,25), values = cms.vdouble(1), uncertainties = cms.vdouble(0.0087868,6.95923e-08)),
        cms.PSet(lowBounds = cms.vdouble(0.94,1.5,25), upBounds = cms.vdouble(999,6,27.5), values = cms.vdouble(1), uncertainties = cms.vdouble(0.00280262,0)),
        cms.PSet(lowBounds = cms.vdouble(0.94,1.5,27.5), upBounds = cms.vdouble(999,6,30), values = cms.vdouble(1), uncertainties = cms.vdouble(0.00116971,0)),
        cms.PSet(lowBounds = cms.vdouble(0.94,1.5,30), upBounds = cms.vdouble(999,6,33.33), values = cms.vdouble(1), uncertainties = cms.vdouble(0.00107293,0)),
        cms.PSet(lowBounds = cms.vdouble(0.94,1.5,33.33), upBounds = cms.vdouble(999,6,35), values = cms.vdouble(1), uncertainties = cms.vdouble(0.00190493,0)),
        cms.PSet(lowBounds = cms.vdouble(0.94,1.5,35), upBounds = cms.vdouble(999,6,40), values = cms.vdouble(0.999446), uncertainties = cms.vdouble(0.000489574,0.000553792)),
        cms.PSet(lowBounds = cms.vdouble(0.94,1.5,40), upBounds = cms.vdouble(999,6,45), values = cms.vdouble(0.999701), uncertainties = cms.vdouble(0.000298843,0.000298843)),
        cms.PSet(lowBounds = cms.vdouble(0.94,1.5,45), upBounds = cms.vdouble(999,6,50), values = cms.vdouble(1), uncertainties = cms.vdouble(0.00114621,0)),
        cms.PSet(lowBounds = cms.vdouble(0.94,1.5,50), upBounds = cms.vdouble(999,6,60), values = cms.vdouble(1), uncertainties = cms.vdouble(0.00188188,0)),
        cms.PSet(lowBounds = cms.vdouble(0.94,1.5,60), upBounds = cms.vdouble(999,6,70), values = cms.vdouble(1), uncertainties = cms.vdouble(0.00829012,0)),
        cms.PSet(lowBounds = cms.vdouble(0.94,1.5,70), upBounds = cms.vdouble(999,6,90), values = cms.vdouble(1), uncertainties = cms.vdouble(0.0108213,0)),
        cms.PSet(lowBounds = cms.vdouble(0.94,1.5,90), upBounds = cms.vdouble(999,6,120), values = cms.vdouble(1), uncertainties = cms.vdouble(0.0243501,0)),
        cms.PSet(lowBounds = cms.vdouble(0.94,1.5,120), upBounds = cms.vdouble(999,6,150), values = cms.vdouble(1), uncertainties = cms.vdouble(0.0521317,0)),
        cms.PSet(lowBounds = cms.vdouble(0.94,1.5,150), upBounds = cms.vdouble(999,6,10000), values = cms.vdouble(1), uncertainties = cms.vdouble(0.0843269,0)),
        #cat 3
        cms.PSet(lowBounds = cms.vdouble(0,1.5,0), upBounds = cms.vdouble(0.94,6,20), values = cms.vdouble(0), uncertainties = cms.vdouble(0,0)),
        cms.PSet(lowBounds = cms.vdouble(0,1.5,20), upBounds = cms.vdouble(0.94,6,22.5), values = cms.vdouble(0.937199), uncertainties = cms.vdouble(0.0195123,0.0628008)),
        cms.PSet(lowBounds = cms.vdouble(0,1.5,22.5), upBounds = cms.vdouble(0.94,6,25), values = cms.vdouble(0.960077), uncertainties = cms.vdouble(0.00907036,0.0399231)),
        cms.PSet(lowBounds = cms.vdouble(0,1.5,25), upBounds = cms.vdouble(0.94,6,27.5), values = cms.vdouble(0.963207), uncertainties = cms.vdouble(0.0119252,0.0367926)),
        cms.PSet(lowBounds = cms.vdouble(0,1.5,27.5), upBounds = cms.vdouble(0.94,6,30), values = cms.vdouble(0.968227), uncertainties = cms.vdouble(0.00800151,0.00614946)),
        cms.PSet(lowBounds = cms.vdouble(0,1.5,30), upBounds = cms.vdouble(0.94,6,33.33), values = cms.vdouble(0.978704), uncertainties = cms.vdouble(0.00341527,0.00349418)),
        cms.PSet(lowBounds = cms.vdouble(0,1.5,33.33), upBounds = cms.vdouble(0.94,6,35), values = cms.vdouble(0.971527), uncertainties = cms.vdouble(0.00497568,0.00446629)),
        cms.PSet(lowBounds = cms.vdouble(0,1.5,35), upBounds = cms.vdouble(0.94,6,40), values = cms.vdouble(0.977258), uncertainties = cms.vdouble(0.00228581,0.00214555)),
        cms.PSet(lowBounds = cms.vdouble(0,1.5,40), upBounds = cms.vdouble(0.94,6,45), values = cms.vdouble(0.974762), uncertainties = cms.vdouble(0.00259899,0.00243617)),
        cms.PSet(lowBounds = cms.vdouble(0,1.5,45), upBounds = cms.vdouble(0.94,6,50), values = cms.vdouble(0.980811), uncertainties = cms.vdouble(0.00321191,0.00321191)),
        cms.PSet(lowBounds = cms.vdouble(0,1.5,50), upBounds = cms.vdouble(0.94,6,60), values = cms.vdouble(0.990012), uncertainties = cms.vdouble(0,0.00295888)),
        cms.PSet(lowBounds = cms.vdouble(0,1.5,60), upBounds = cms.vdouble(0.94,6,70), values = cms.vdouble(0.971907), uncertainties = cms.vdouble(0.019931,0.0132091)),
        cms.PSet(lowBounds = cms.vdouble(0,1.5,70), upBounds = cms.vdouble(0.94,6,90), values = cms.vdouble(0.9881), uncertainties = cms.vdouble(0.00835685,0.00835685)),
        cms.PSet(lowBounds = cms.vdouble(0,1.5,90), upBounds = cms.vdouble(0.94,6,120), values = cms.vdouble(0.986682), uncertainties = cms.vdouble(0.0177913,0.00928659)),
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
        cms.PSet(lowBounds = cms.vdouble(0), upBounds = cms.vdouble(5), values = cms.vdouble(1.03491,0.949336), uncertainties = cms.vdouble(0.00442361,0.00442361,0.00642325,0.00642325)),
        cms.PSet(lowBounds = cms.vdouble(5), upBounds = cms.vdouble(10), values = cms.vdouble(1.03103,0.947076), uncertainties = cms.vdouble(0.00336096,0.00336096,0.00573389,0.00573389)),
        cms.PSet(lowBounds = cms.vdouble(10), upBounds = cms.vdouble(15), values = cms.vdouble(1.01637,0.965526), uncertainties = cms.vdouble(0.00306851,0.00306851,0.00646403,0.00646403)),
        cms.PSet(lowBounds = cms.vdouble(15), upBounds = cms.vdouble(20), values = cms.vdouble(1.00566,0.984498), uncertainties = cms.vdouble(0.00289546,0.00289546,0.007929,0.007929)),
        cms.PSet(lowBounds = cms.vdouble(20), upBounds = cms.vdouble(30), values = cms.vdouble(0.991146,1.03475), uncertainties = cms.vdouble(0.00196846,0.00196846,0.0077251,0.0077251)),
        cms.PSet(lowBounds = cms.vdouble(30), upBounds = cms.vdouble(40), values = cms.vdouble(0.98022,1.13845), uncertainties = cms.vdouble(0.0018857,0.0018857,0.0131993,0.0131993)),
        cms.PSet(lowBounds = cms.vdouble(40), upBounds = cms.vdouble(50), values = cms.vdouble(0.982686,1.18848), uncertainties = cms.vdouble(0.00189045,0.00189045,0.0205798,0.0205798)),
        cms.PSet(lowBounds = cms.vdouble(50), upBounds = cms.vdouble(60), values = cms.vdouble(0.987457,1.19062), uncertainties = cms.vdouble(0.00197685,0.00197685,0.0300427,0.0300427)),
        cms.PSet(lowBounds = cms.vdouble(60), upBounds = cms.vdouble(80), values = cms.vdouble(0.994407,1.11126), uncertainties = cms.vdouble(0.00157416,0.00157416,0.0313173,0.0313173)),
        cms.PSet(lowBounds = cms.vdouble(80), upBounds = cms.vdouble(100), values = cms.vdouble(0.999829,1.00413), uncertainties = cms.vdouble(0.00188478,0.00188478,0.0453944,0.0453944)),
        cms.PSet(lowBounds = cms.vdouble(100), upBounds = cms.vdouble(140), values = cms.vdouble(1.00126,0.957765), uncertainties = cms.vdouble(0.00171718,0.00171718,0.0574166,0.0574166)),
        cms.PSet(lowBounds = cms.vdouble(140), upBounds = cms.vdouble(200), values = cms.vdouble(0.998821,1.06351), uncertainties = cms.vdouble(0.00176983,0.00176983,0.0953629,0.0953629)),
        cms.PSet(lowBounds = cms.vdouble(200), upBounds = cms.vdouble(400), values = cms.vdouble(1,1), uncertainties = cms.vdouble(0.00212644,0.00212644,0.240175,0.240175)),
        # maximum energy beyond 7000 because of wonky DiPhotons in data
        cms.PSet(lowBounds = cms.vdouble(400 ) , upBounds = cms.vdouble(999999999 ) , values = cms.vdouble(1,1              ) , uncertainties = cms.vdouble(0.,0.,0.,0.)) 
        
        )
    )       



flashggDiPhotonSystematics = cms.EDProducer('FlashggDiPhotonSystematicProducer',
		src = cms.InputTag("flashggUpdatedIdMVADiPhotons"),
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
                                                  BinList = photonScaleUncertBins,
                                                  ApplyCentralValue = cms.bool(False),
                                                  Debug = cms.untracked.bool(False)
                                                  ),
                                        cms.PSet( PhotonMethodName = cms.string("FlashggPhotonScale"),
                                                  MethodName = cms.string("FlashggDiPhotonFromPhoton"),
                                                  Label = cms.string("MCScaleLowR9EB"),
                                                  NSigmas = cms.vint32(-1,1),
                                                  OverallRange = cms.string("r9<0.94&&abs(superCluster.eta)<1.5"),
                                                  BinList = photonScaleUncertBins,
                                                  ApplyCentralValue = cms.bool(False),
                                                  Debug = cms.untracked.bool(False)
                                                  ),
                                        cms.PSet( PhotonMethodName = cms.string("FlashggPhotonScale"),
                                                  MethodName = cms.string("FlashggDiPhotonFromPhoton"),
                                                  Label = cms.string("MCScaleHighR9EE"),
                                                  NSigmas = cms.vint32(-1,1),
                                                  OverallRange = cms.string("r9>0.94&&abs(superCluster.eta)>=1.5"),
                                                  BinList = photonScaleUncertBins,
                                                  ApplyCentralValue = cms.bool(False),
                                                  Debug = cms.untracked.bool(False)
                                                  ),
                                        cms.PSet( PhotonMethodName = cms.string("FlashggPhotonScale"),
                                                  MethodName = cms.string("FlashggDiPhotonFromPhoton"),
                                                  Label = cms.string("MCScaleLowR9EE"),
                                                  NSigmas = cms.vint32(-1,1),
                                                  OverallRange = cms.string("r9<0.94&&abs(superCluster.eta)>=1.5"),
                                                  BinList = photonScaleUncertBins,
                                                  ApplyCentralValue = cms.bool(False),
                                                  Debug = cms.untracked.bool(False)
                                                  ),
                                        cms.PSet( PhotonMethodName = cms.string("FlashggPhotonSmearConstant"),
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
                                                  ),
                                        cms.PSet( PhotonMethodName = cms.string("FlashggPhotonSmearConstant"),
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
                                                  ),
                                        cms.PSet( PhotonMethodName = cms.string("FlashggPhotonSmearConstant"),
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
                                                  ),
                                        cms.PSet( PhotonMethodName = cms.string("FlashggPhotonSmearConstant"),
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
                                                  ),
                                        cms.PSet( PhotonMethodName = cms.string("FlashggPhotonMvaShift"),
                                                  MethodName = cms.string("FlashggDiPhotonFromPhoton"),
                                                  Label = cms.string("MvaShift"),
                                                  NSigmas = cms.vint32(-1,1),
                                                  OverallRange = cms.string("1"),
                                                  BinList = mvaShiftBins,
                                                  Debug = cms.untracked.bool(False),
                                                  ApplyCentralValue = cms.bool(True)
                                                  ),
                                        cms.PSet( PhotonMethodName = cms.string("FlashggPhotonWeight"),
                                                  MethodName = cms.string("FlashggDiPhotonFromPhoton"),
                                                  Label = cms.string("PreselSF"),
                                                  NSigmas = cms.vint32(-1,1),
                                                  OverallRange = cms.string("1"),
                                                  BinList = preselBins,
                                                  Debug = cms.untracked.bool(False),
                                                  ApplyCentralValue = cms.bool(True)
                                                  ),
                                        cms.PSet( PhotonMethodName = cms.string("FlashggPhotonWeight"),
                                                  MethodName = cms.string("FlashggDiPhotonFromPhoton"),
                                                  Label = cms.string("electronVetoSF"),
                                                  NSigmas = cms.vint32(-1,1),
                                                  OverallRange = cms.string("1"),
                                                  BinList = electronVetoBins,
                                                  Debug = cms.untracked.bool(False),
                                                  ApplyCentralValue = cms.bool(True)
                                                  ),
                                        cms.PSet( PhotonMethodName = cms.string("FlashggPhotonWeight"),
                                                  MethodName = cms.string("FlashggDiPhotonFromPhoton"),
                                                  Label = cms.string("TriggerWeight"),
                                                  NSigmas = cms.vint32(-1,1),
                                                  OverallRange = cms.string("1"),
                                                  BinList = leadTriggerScaleBins,
                                                  BinList2 = subleadTriggerScaleBins,
                                                  Debug = cms.untracked.bool(False),
                                                  ApplyCentralValue = cms.bool(True)
                                                  ),
                                        cms.PSet( PhotonMethodName = cms.string("FlashggPhotonWeight"),
                                                  MethodName = cms.string("FlashggDiPhotonFromPhoton"),
                                                  Label = cms.string("LooseMvaSF"),
                                                  NSigmas = cms.vint32(-1,1),
                                                  OverallRange = cms.string("1"),
                                                  BinList = looseMvaBins,
                                                  Debug = cms.untracked.bool(False),
                                                  ApplyCentralValue = cms.bool(True)
                                                  ),
                                        cms.PSet( PhotonMethodName = cms.string("FlashggPhotonSigEOverEShift"),
                                                  MethodName = cms.string("FlashggDiPhotonFromPhoton"),
                                                  Label = cms.string("SigmaEOverEShift"),
                                                  NSigmas = cms.vint32(-1,1),
                                                  OverallRange = cms.string("1"),
                                                  BinList = sigmaEOverEShiftBins,
                                                  Debug = cms.untracked.bool(False),
                                                  ApplyCentralValue = cms.bool(True)
                                                  ),
                                        cms.PSet( PhotonMethodName = cms.string("FlashggPhotonSigEoverESmearing"),
                                                  MethodName = cms.string("FlashggDiPhotonFromPhoton"),
                                                  Label = cms.string("SigmaEOverESmearing"),
                                                  NSigmas = cms.vint32(),
                                                  OverallRange = cms.string("1"),
                                                  BinList = photonSmearBins,
                                                  Debug = cms.untracked.bool(False),
                                                  ApplyCentralValue = cms.bool(True)
                                                  ),
                                        cms.PSet( MethodName = cms.string("FlashggDiPhotonWeightFromFracRV"),
                                                  Label = cms.string("FracRVWeight"),
                                                  NSigmas = cms.vint32(-1,1),
                                                  OverallRange = cms.string("1"),
                                                  BinList = RVBins,
                                                  Debug = cms.untracked.bool(False),
                                                  ApplyCentralValue = cms.bool(True)
                                                  )
                                        )
                                            )


