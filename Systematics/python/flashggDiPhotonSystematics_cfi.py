import FWCore.ParameterSet.Config as cms

# default energy scale uncertainties and smearings from 76X re-reco
from flashgg.Systematics.escales.escale76X_16DecRereco_2015 import photonSmearBins, photonScaleUncertBins 

mvaShiftBins = cms.PSet(
    variables = cms.vstring("abs(superCluster.eta)"),
    bins = cms.VPSet(
                     cms.PSet( lowBounds = cms.vdouble(0.000), upBounds = cms.vdouble(999.), values = cms.vdouble( 0.0 ), uncertainties = cms.vdouble( 0.00 ))
                     )
    )

# AN version 5
preselBins = cms.PSet(
    variables = cms.vstring("abs(superCluster.eta)","r9"),
    bins = cms.VPSet(
        cms.PSet( lowBounds = cms.vdouble( 0.0, 0.0 ) , upBounds = cms.vdouble( 1.5, 0.85 ) , values = cms.vdouble( 1.032 ) , uncertainties = cms.vdouble( 0.008 )  ) ,
        cms.PSet( lowBounds = cms.vdouble( 0.0, 0.85 ), upBounds = cms.vdouble( 1.5, 999. ) , values = cms.vdouble( 1.004 ) , uncertainties = cms.vdouble( 0.003 )  ) ,
        cms.PSet( lowBounds = cms.vdouble( 1.5, 0.0 ) , upBounds = cms.vdouble( 6.0, 0.9  ) , values = cms.vdouble( 1.006 ) , uncertainties = cms.vdouble( 0.032 )  ) ,
        cms.PSet( lowBounds = cms.vdouble( 1.5, 0.9 ) , upBounds = cms.vdouble( 6.0, 999. ) , values = cms.vdouble( 1.004 ) , uncertainties = cms.vdouble( 0.022 )  ) 
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
        cms.PSet(lowBounds = cms.vdouble(0.94,0,30), upBounds = cms.vdouble(999,1.5,33.33), values = cms.vdouble(0.880799), uncertainties = cms.vdouble(0.00291219,0.119201)),
        cms.PSet(lowBounds = cms.vdouble(0.94,0,33.33), upBounds = cms.vdouble(999,1.5,35), values = cms.vdouble(0.981046), uncertainties = cms.vdouble(0.00171793,0.00164683)),
        cms.PSet(lowBounds = cms.vdouble(0.94,0,35), upBounds = cms.vdouble(999,1.5,40), values = cms.vdouble(0.986396), uncertainties = cms.vdouble(0.000659068,0.000646598)),
        cms.PSet(lowBounds = cms.vdouble(0.94,0,40), upBounds = cms.vdouble(999,1.5,45), values = cms.vdouble(0.990453), uncertainties = cms.vdouble(0.00103027,0.00954664)),
        cms.PSet(lowBounds = cms.vdouble(0.94,0,45), upBounds = cms.vdouble(999,1.5,50), values = cms.vdouble(0.992037), uncertainties = cms.vdouble(0.000463905,0.000442004)),
        cms.PSet(lowBounds = cms.vdouble(0.94,0,50), upBounds = cms.vdouble(999,1.5,60), values = cms.vdouble(0.991062), uncertainties = cms.vdouble(0.000604877,0.000579337)),
        cms.PSet(lowBounds = cms.vdouble(0.94,0,60), upBounds = cms.vdouble(999,1.5,70), values = cms.vdouble(0.991421), uncertainties = cms.vdouble(0.00111165,0.00111165)),
        cms.PSet(lowBounds = cms.vdouble(0.94,0,70), upBounds = cms.vdouble(999,1.5,90), values = cms.vdouble(0.996709), uncertainties = cms.vdouble(0.00109278,0.000894326)),
        cms.PSet(lowBounds = cms.vdouble(0.94,0,90), upBounds = cms.vdouble(999,1.5,120), values = cms.vdouble(0.99634), uncertainties = cms.vdouble(0.00163375,0.00163375)),
        cms.PSet(lowBounds = cms.vdouble(0.94,0,120), upBounds = cms.vdouble(999,1.5,150), values = cms.vdouble(0.998182), uncertainties = cms.vdouble(0,0.00156094)),
        cms.PSet(lowBounds = cms.vdouble(0.94,0,150), upBounds = cms.vdouble(999,1.5,10000), values = cms.vdouble(0.997684), uncertainties = cms.vdouble(0.00231219,0.00231219)),
        
        cms.PSet(lowBounds = cms.vdouble(0,0,0), upBounds = cms.vdouble(0.94,1.5,30), values = cms.vdouble(0), uncertainties = cms.vdouble(0,0)),
        cms.PSet(lowBounds = cms.vdouble(0,0,30), upBounds = cms.vdouble(0.94,1.5,33.33), values = cms.vdouble(0.579143), uncertainties = cms.vdouble(0.00459701,0.00450932)),
        cms.PSet(lowBounds = cms.vdouble(0,0,33.33), upBounds = cms.vdouble(0.94,1.5,35), values = cms.vdouble(0.90269), uncertainties = cms.vdouble(0.00314675,0.0973095)),
        cms.PSet(lowBounds = cms.vdouble(0,0,35), upBounds = cms.vdouble(0.94,1.5,40), values = cms.vdouble(0.977272), uncertainties = cms.vdouble(0.000547311,0.000539963)),
        cms.PSet(lowBounds = cms.vdouble(0,0,40), upBounds = cms.vdouble(0.94,1.5,45), values = cms.vdouble(0.988631), uncertainties = cms.vdouble(0.000364602,0.000356913)),
        cms.PSet(lowBounds = cms.vdouble(0,0,45), upBounds = cms.vdouble(0.94,1.5,50), values = cms.vdouble(0.991244), uncertainties = cms.vdouble(0.00049511,0.000508301)),
        cms.PSet(lowBounds = cms.vdouble(0,0,50), upBounds = cms.vdouble(0.94,1.5,60), values = cms.vdouble(0.991344), uncertainties = cms.vdouble(0.000682174,0.00865557)),
        cms.PSet(lowBounds = cms.vdouble(0,0,60), upBounds = cms.vdouble(0.94,1.5,70), values = cms.vdouble(0.98917), uncertainties = cms.vdouble(0.00132479,0.00123585)),
        cms.PSet(lowBounds = cms.vdouble(0,0,70), upBounds = cms.vdouble(0.94,1.5,90), values = cms.vdouble(0.990803), uncertainties = cms.vdouble(0.001747,0.00155081)),
        cms.PSet(lowBounds = cms.vdouble(0,0,90), upBounds = cms.vdouble(0.94,1.5,120), values = cms.vdouble(0.995414), uncertainties = cms.vdouble(0,0.00146179)),
        cms.PSet(lowBounds = cms.vdouble(0,0,120), upBounds = cms.vdouble(0.94,1.5,150), values = cms.vdouble(0.995504), uncertainties = cms.vdouble(0.0039916,0.00449605)),
        cms.PSet(lowBounds = cms.vdouble(0,0,150), upBounds = cms.vdouble(0.94,1.5,10000), values = cms.vdouble(0.990387), uncertainties = cms.vdouble(0.00723651,0.00961344)),
        
        cms.PSet(lowBounds = cms.vdouble(0.94,1.5,0), upBounds = cms.vdouble(999,6,30), values = cms.vdouble(0), uncertainties = cms.vdouble(0,0)),
        cms.PSet(lowBounds = cms.vdouble(0.94,1.5,30), upBounds = cms.vdouble(999,6,33.33), values = cms.vdouble(0.611041), uncertainties = cms.vdouble(0.00757808,0.0075544)),
        cms.PSet(lowBounds = cms.vdouble(0.94,1.5,33.33), upBounds = cms.vdouble(999,6,35), values = cms.vdouble(0.947974), uncertainties = cms.vdouble(0.00334286,0.00332333)),
        cms.PSet(lowBounds = cms.vdouble(0.94,1.5,35), upBounds = cms.vdouble(999,6,40), values = cms.vdouble(0.989328), uncertainties = cms.vdouble(0.000918502,0.000901687)),
        cms.PSet(lowBounds = cms.vdouble(0.94,1.5,40), upBounds = cms.vdouble(999,6,45), values = cms.vdouble(0.992206), uncertainties = cms.vdouble(0.00067688,0.000640296)),
        cms.PSet(lowBounds = cms.vdouble(0.94,1.5,45), upBounds = cms.vdouble(999,6,50), values = cms.vdouble(0.992501), uncertainties = cms.vdouble(0.000922398,0.00749887)),
        cms.PSet(lowBounds = cms.vdouble(0.94,1.5,50), upBounds = cms.vdouble(999,6,60), values = cms.vdouble(0.993176), uncertainties = cms.vdouble(0.00121553,0.00108811)),
        cms.PSet(lowBounds = cms.vdouble(0.94,1.5,60), upBounds = cms.vdouble(999,6,70), values = cms.vdouble(0.993988), uncertainties = cms.vdouble(0.00199776,0.00199776)),
        cms.PSet(lowBounds = cms.vdouble(0.94,1.5,70), upBounds = cms.vdouble(999,6,90), values = cms.vdouble(0.995923), uncertainties = cms.vdouble(0.00282325,0.00192436)),
        cms.PSet(lowBounds = cms.vdouble(0.94,1.5,90), upBounds = cms.vdouble(999,6,120), values = cms.vdouble(0.994124), uncertainties = cms.vdouble(0.00523493,0.0032662)),
        cms.PSet(lowBounds = cms.vdouble(0.94,1.5,120), upBounds = cms.vdouble(999,6,150), values = cms.vdouble(1), uncertainties = cms.vdouble(0.019713,0)),
        cms.PSet(lowBounds = cms.vdouble(0.94,1.5,150), upBounds = cms.vdouble(999,6,10000), values = cms.vdouble(1), uncertainties = cms.vdouble(0.0213186,0)),
        
        cms.PSet(lowBounds = cms.vdouble(0,1.5,0), upBounds = cms.vdouble(0.94,6,30), values = cms.vdouble(0), uncertainties = cms.vdouble(0,0)),
        cms.PSet(lowBounds = cms.vdouble(0,1.5,30), upBounds = cms.vdouble(0.94,6,33.33), values = cms.vdouble(0.669099), uncertainties = cms.vdouble(0.00576017,0.00570585)),
        cms.PSet(lowBounds = cms.vdouble(0,1.5,33.33), upBounds = cms.vdouble(0.94,6,35), values = cms.vdouble(0.936254), uncertainties = cms.vdouble(0.00332581,0.00322233)),
        cms.PSet(lowBounds = cms.vdouble(0,1.5,35), upBounds = cms.vdouble(0.94,6,40), values = cms.vdouble(0.969524), uncertainties = cms.vdouble(0.00126454,0.00123185)),
        cms.PSet(lowBounds = cms.vdouble(0,1.5,40), upBounds = cms.vdouble(0.94,6,45), values = cms.vdouble(0.974312), uncertainties = cms.vdouble(0.00122724,0.00119049)),
        cms.PSet(lowBounds = cms.vdouble(0,1.5,45), upBounds = cms.vdouble(0.94,6,50), values = cms.vdouble(0.980103), uncertainties = cms.vdouble(0,0.00149348)),
        cms.PSet(lowBounds = cms.vdouble(0,1.5,50), upBounds = cms.vdouble(0.94,6,60), values = cms.vdouble(0.985201), uncertainties = cms.vdouble(0.00178231,0.00178231)),
        cms.PSet(lowBounds = cms.vdouble(0,1.5,60), upBounds = cms.vdouble(0.94,6,70), values = cms.vdouble(0.98313), uncertainties = cms.vdouble(0.00364952,0.00364952)),
        cms.PSet(lowBounds = cms.vdouble(0,1.5,70), upBounds = cms.vdouble(0.94,6,90), values = cms.vdouble(0.9837), uncertainties = cms.vdouble(0.00567071,0.00460854)),
        cms.PSet(lowBounds = cms.vdouble(0,1.5,90), upBounds = cms.vdouble(0.94,6,120), values = cms.vdouble(0.989067), uncertainties = cms.vdouble(0.00753849,0.00514723)),
        cms.PSet(lowBounds = cms.vdouble(0,1.5,120), upBounds = cms.vdouble(0.94,6,150), values = cms.vdouble(1), uncertainties = cms.vdouble(0.0267677,0)),
        cms.PSet(lowBounds = cms.vdouble(0,1.5,150), upBounds = cms.vdouble(0.94,6,10000), values = cms.vdouble(1), uncertainties = cms.vdouble(0.0340269,0)),
        )
    )


subleadTriggerScaleBins = cms.PSet(
    variables = cms.vstring("r9","abs(superCluster.eta)","pt"),
    bins = cms.VPSet(
        #cat0
        cms.PSet(lowBounds = cms.vdouble(0.94,0,0), upBounds = cms.vdouble(999,1.5,20), values = cms.vdouble(0), uncertainties = cms.vdouble(0,0)),
        cms.PSet(lowBounds = cms.vdouble(0.94,0,20), upBounds = cms.vdouble(999,1.5,22.5), values = cms.vdouble(0.96377), uncertainties = cms.vdouble(0.0032693,0.00322566)),
        cms.PSet(lowBounds = cms.vdouble(0.94,0,22.5), upBounds = cms.vdouble(999,1.5,25), values = cms.vdouble(0.982341), uncertainties = cms.vdouble(0.00491418,0.00302394)),
        cms.PSet(lowBounds = cms.vdouble(0.94,0,25), upBounds = cms.vdouble(999,1.5,27.5), values = cms.vdouble(0.982896), uncertainties = cms.vdouble(0.00133382,0.00127249)),
        cms.PSet(lowBounds = cms.vdouble(0.94,0,27.5), upBounds = cms.vdouble(999,1.5,30), values = cms.vdouble(0.990292), uncertainties = cms.vdouble(0.00147533,0.00160349)),
        cms.PSet(lowBounds = cms.vdouble(0.94,0,30), upBounds = cms.vdouble(999,1.5,33.33), values = cms.vdouble(0.992227), uncertainties = cms.vdouble(0.000490475,0.000471679)),
        cms.PSet(lowBounds = cms.vdouble(0.94,0,33.33), upBounds = cms.vdouble(999,1.5,35), values = cms.vdouble(0.993892), uncertainties = cms.vdouble(0.000504127,0.00610826)),
        cms.PSet(lowBounds = cms.vdouble(0.94,0,35), upBounds = cms.vdouble(999,1.5,40), values = cms.vdouble(0.996189), uncertainties = cms.vdouble(0,0.000183537)),
        cms.PSet(lowBounds = cms.vdouble(0.94,0,40), upBounds = cms.vdouble(999,1.5,45), values = cms.vdouble(0.998099), uncertainties = cms.vdouble(0.000115606,0.000111348)),
        cms.PSet(lowBounds = cms.vdouble(0.94,0,45), upBounds = cms.vdouble(999,1.5,50), values = cms.vdouble(0.999057), uncertainties = cms.vdouble(0.000112483,0.000107126)),
        cms.PSet(lowBounds = cms.vdouble(0.94,0,50), upBounds = cms.vdouble(999,1.5,60), values = cms.vdouble(0.999089), uncertainties = cms.vdouble(0.000136063,0.000124275)),
        cms.PSet(lowBounds = cms.vdouble(0.94,0,60), upBounds = cms.vdouble(999,1.5,70), values = cms.vdouble(0.999542), uncertainties = cms.vdouble(0.0002825,0.000457499)),
        cms.PSet(lowBounds = cms.vdouble(0.94,0,70), upBounds = cms.vdouble(999,1.5,90), values = cms.vdouble(0.99911), uncertainties = cms.vdouble(0,0.000315444)),
        cms.PSet(lowBounds = cms.vdouble(0.94,0,90), upBounds = cms.vdouble(999,1.5,120), values = cms.vdouble(0.99966), uncertainties = cms.vdouble(0.00033969,0.00033969)),
        cms.PSet(lowBounds = cms.vdouble(0.94,0,120), upBounds = cms.vdouble(999,1.5,150), values = cms.vdouble(1), uncertainties = cms.vdouble(0.00161481,0)),
        cms.PSet(lowBounds = cms.vdouble(0.94,0,150), upBounds = cms.vdouble(999,1.5,10000), values = cms.vdouble(1), uncertainties = cms.vdouble(0.00188115,0)),
        
        cms.PSet(lowBounds = cms.vdouble(0,0,0), upBounds = cms.vdouble(0.94,1.5,20), values = cms.vdouble(0), uncertainties = cms.vdouble(0,0)),
        cms.PSet(lowBounds = cms.vdouble(0,0,20), upBounds = cms.vdouble(0.94,1.5,22.5), values = cms.vdouble(0.862861), uncertainties = cms.vdouble(0.00567222,0.00564814)),
        cms.PSet(lowBounds = cms.vdouble(0,0,22.5), upBounds = cms.vdouble(0.94,1.5,25), values = cms.vdouble(0.961769), uncertainties = cms.vdouble(0,0.00324665)),
        cms.PSet(lowBounds = cms.vdouble(0,0,25), upBounds = cms.vdouble(0.94,1.5,27.5), values = cms.vdouble(0.986686), uncertainties = cms.vdouble(0.00204279,0.00145926)),
        cms.PSet(lowBounds = cms.vdouble(0,0,27.5), upBounds = cms.vdouble(0.94,1.5,30), values = cms.vdouble(0.984828), uncertainties = cms.vdouble(0.00151225,0.0151723)),
        cms.PSet(lowBounds = cms.vdouble(0,0,30), upBounds = cms.vdouble(0.94,1.5,33.33), values = cms.vdouble(0.989086), uncertainties = cms.vdouble(0.000434248,0.0109143)),
        cms.PSet(lowBounds = cms.vdouble(0,0,33.33), upBounds = cms.vdouble(0.94,1.5,35), values = cms.vdouble(0.992549), uncertainties = cms.vdouble(0,0.000686226)),
        cms.PSet(lowBounds = cms.vdouble(0,0,35), upBounds = cms.vdouble(0.94,1.5,40), values = cms.vdouble(0.993096), uncertainties = cms.vdouble(0.000214592,0.00690434)),
        cms.PSet(lowBounds = cms.vdouble(0,0,40), upBounds = cms.vdouble(0.94,1.5,45), values = cms.vdouble(0.994831), uncertainties = cms.vdouble(0.000166291,0.000163725)),
        cms.PSet(lowBounds = cms.vdouble(0,0,45), upBounds = cms.vdouble(0.94,1.5,50), values = cms.vdouble(0.995646), uncertainties = cms.vdouble(0.000203758,0.000196788)),
        cms.PSet(lowBounds = cms.vdouble(0,0,50), upBounds = cms.vdouble(0.94,1.5,60), values = cms.vdouble(0.99506), uncertainties = cms.vdouble(0.000304781,0.00029481)),
        cms.PSet(lowBounds = cms.vdouble(0,0,60), upBounds = cms.vdouble(0.94,1.5,70), values = cms.vdouble(0.996137), uncertainties = cms.vdouble(0.000833509,0.00081679)),
        cms.PSet(lowBounds = cms.vdouble(0,0,70), upBounds = cms.vdouble(0.94,1.5,90), values = cms.vdouble(0.99681), uncertainties = cms.vdouble(0.000746602,0.000646052)),
        cms.PSet(lowBounds = cms.vdouble(0,0,90), upBounds = cms.vdouble(0.94,1.5,120), values = cms.vdouble(0.997444), uncertainties = cms.vdouble(0.00110606,0.000858454)),
        cms.PSet(lowBounds = cms.vdouble(0,0,120), upBounds = cms.vdouble(0.94,1.5,150), values = cms.vdouble(0.997869), uncertainties = cms.vdouble(0.00187868,0.00117437)),
        cms.PSet(lowBounds = cms.vdouble(0,0,150), upBounds = cms.vdouble(0.94,1.5,10000), values = cms.vdouble(0.998518), uncertainties = cms.vdouble(0.00201572,0.00148182)),
        
        cms.PSet(lowBounds = cms.vdouble(0.94,1.5,0), upBounds = cms.vdouble(999,6,20), values = cms.vdouble(0), uncertainties = cms.vdouble(0,0)),
        cms.PSet(lowBounds = cms.vdouble(0.94,1.5,20), upBounds = cms.vdouble(999,6,22.5), values = cms.vdouble(0.983702), uncertainties = cms.vdouble(0.00249174,0.00220812)),
        cms.PSet(lowBounds = cms.vdouble(0.94,1.5,22.5), upBounds = cms.vdouble(999,6,25), values = cms.vdouble(0.998161), uncertainties = cms.vdouble(0.000612162,0.000612162)),
        cms.PSet(lowBounds = cms.vdouble(0.94,1.5,25), upBounds = cms.vdouble(999,6,27.5), values = cms.vdouble(0.999226), uncertainties = cms.vdouble(0.000416447,0.000327486)),
        cms.PSet(lowBounds = cms.vdouble(0.94,1.5,27.5), upBounds = cms.vdouble(999,6,30), values = cms.vdouble(0.999003), uncertainties = cms.vdouble(0.000349172,0.000283165)),
        cms.PSet(lowBounds = cms.vdouble(0.94,1.5,30), upBounds = cms.vdouble(999,6,33.33), values = cms.vdouble(0.999574), uncertainties = cms.vdouble(0.000169118,0.000133713)),
        cms.PSet(lowBounds = cms.vdouble(0.94,1.5,33.33), upBounds = cms.vdouble(999,6,35), values = cms.vdouble(0.999736), uncertainties = cms.vdouble(0.000152489,0.000152489)),
        cms.PSet(lowBounds = cms.vdouble(0.94,1.5,35), upBounds = cms.vdouble(999,6,40), values = cms.vdouble(0.999647), uncertainties = cms.vdouble(0.000102708,8.63949e-05)),
        cms.PSet(lowBounds = cms.vdouble(0.94,1.5,40), upBounds = cms.vdouble(999,6,45), values = cms.vdouble(0.999787), uncertainties = cms.vdouble(7.52103e-05,7.52103e-05)),
        cms.PSet(lowBounds = cms.vdouble(0.94,1.5,45), upBounds = cms.vdouble(999,6,50), values = cms.vdouble(0.999849), uncertainties = cms.vdouble(8.70141e-05,8.70141e-05)),
        cms.PSet(lowBounds = cms.vdouble(0.94,1.5,50), upBounds = cms.vdouble(999,6,60), values = cms.vdouble(0.999709), uncertainties = cms.vdouble(0.000168037,0.000168037)),
        cms.PSet(lowBounds = cms.vdouble(0.94,1.5,60), upBounds = cms.vdouble(999,6,70), values = cms.vdouble(1), uncertainties = cms.vdouble(0.000747244,0)),
        cms.PSet(lowBounds = cms.vdouble(0.94,1.5,70), upBounds = cms.vdouble(999,6,90), values = cms.vdouble(1), uncertainties = cms.vdouble(0.00129581,0)),
        cms.PSet(lowBounds = cms.vdouble(0.94,1.5,90), upBounds = cms.vdouble(999,6,120), values = cms.vdouble(1), uncertainties = cms.vdouble(0.00255171,0)),
        cms.PSet(lowBounds = cms.vdouble(0.94,1.5,120), upBounds = cms.vdouble(999,6,150), values = cms.vdouble(1), uncertainties = cms.vdouble(0.00710992,0)),
        cms.PSet(lowBounds = cms.vdouble(0.94,1.5,150), upBounds = cms.vdouble(999,6,10000), values = cms.vdouble(1), uncertainties = cms.vdouble(0.00956974,0)),
        
        cms.PSet(lowBounds = cms.vdouble(0,1.5,0), upBounds = cms.vdouble(0.94,6,20), values = cms.vdouble(0), uncertainties = cms.vdouble(0,0)),
        cms.PSet(lowBounds = cms.vdouble(0,1.5,20), upBounds = cms.vdouble(0.94,6,22.5), values = cms.vdouble(0.941373), uncertainties = cms.vdouble(0.00483615,0.00404553)),
        cms.PSet(lowBounds = cms.vdouble(0,1.5,22.5), upBounds = cms.vdouble(0.94,6,25), values = cms.vdouble(0.966338), uncertainties = cms.vdouble(0.00282747,0.00268354)),
        cms.PSet(lowBounds = cms.vdouble(0,1.5,25), upBounds = cms.vdouble(0.94,6,27.5), values = cms.vdouble(0.975773), uncertainties = cms.vdouble(0.0077382,0.00296388)),
        cms.PSet(lowBounds = cms.vdouble(0,1.5,27.5), upBounds = cms.vdouble(0.94,6,30), values = cms.vdouble(0.973424), uncertainties = cms.vdouble(0,0.00235801)),
        cms.PSet(lowBounds = cms.vdouble(0,1.5,30), upBounds = cms.vdouble(0.94,6,33.33), values = cms.vdouble(0.972833), uncertainties = cms.vdouble(0.00509085,0.00435287)),
        cms.PSet(lowBounds = cms.vdouble(0,1.5,33.33), upBounds = cms.vdouble(0.94,6,35), values = cms.vdouble(0.976094), uncertainties = cms.vdouble(0.00144083,0.00138285)),
        cms.PSet(lowBounds = cms.vdouble(0,1.5,35), upBounds = cms.vdouble(0.94,6,40), values = cms.vdouble(0.977027), uncertainties = cms.vdouble(0.0007438,0.0007438)),
        cms.PSet(lowBounds = cms.vdouble(0,1.5,40), upBounds = cms.vdouble(0.94,6,45), values = cms.vdouble(0.97988), uncertainties = cms.vdouble(0.000736546,0.000719897)),
        cms.PSet(lowBounds = cms.vdouble(0,1.5,45), upBounds = cms.vdouble(0.94,6,50), values = cms.vdouble(0.984714), uncertainties = cms.vdouble(0.00104331,0.00101031)),
        cms.PSet(lowBounds = cms.vdouble(0,1.5,50), upBounds = cms.vdouble(0.94,6,60), values = cms.vdouble(0.985681), uncertainties = cms.vdouble(0,0.00120877)),
        cms.PSet(lowBounds = cms.vdouble(0,1.5,60), upBounds = cms.vdouble(0.94,6,70), values = cms.vdouble(0.979352), uncertainties = cms.vdouble(0,0.00319006)),
        cms.PSet(lowBounds = cms.vdouble(0,1.5,70), upBounds = cms.vdouble(0.94,6,90), values = cms.vdouble(0.987411), uncertainties = cms.vdouble(0.00513288,0.0125888)),
        cms.PSet(lowBounds = cms.vdouble(0,1.5,90), upBounds = cms.vdouble(0.94,6,120), values = cms.vdouble(0.987656), uncertainties = cms.vdouble(0.00524227,0.00408727)),
        cms.PSet(lowBounds = cms.vdouble(0,1.5,120), upBounds = cms.vdouble(0.94,6,150), values = cms.vdouble(1), uncertainties = cms.vdouble(0.0112127,0)),
        cms.PSet(lowBounds = cms.vdouble(0,1.5,150), upBounds = cms.vdouble(0.94,6,10000), values = cms.vdouble(1), uncertainties = cms.vdouble(0.0185065,0)),
        )
    )


# AN version 5
# N.B. boundary changed accordingly to the preselection
looseMvaBins = cms.PSet(
    variables = cms.vstring("abs(superCluster.eta)","r9"),
    bins = cms.VPSet(
        cms.PSet( lowBounds = cms.vdouble( 0.0, 0.0  ) , upBounds = cms.vdouble( 1.5, 0.85  ) , values = cms.vdouble( 0.9996 ) , uncertainties = cms.vdouble( 0.0008 )  ) ,
        cms.PSet( lowBounds = cms.vdouble( 0.0, 0.85 ) , upBounds = cms.vdouble( 1.5, 999.0 ) , values = cms.vdouble( 0.9992 ) , uncertainties = cms.vdouble( 0.0005 )  ) ,
        cms.PSet( lowBounds = cms.vdouble( 1.5, 0.0  ) , upBounds = cms.vdouble( 6.0, 0.9   ) , values = cms.vdouble( 1.0014 ) , uncertainties = cms.vdouble( 0.0016 )  ) ,
        cms.PSet( lowBounds = cms.vdouble( 1.5, 0.9  ) , upBounds = cms.vdouble( 6.0, 999.0 ) , values = cms.vdouble( 0.9998 ) , uncertainties = cms.vdouble( 0.0019 )  ) 
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

emptyBins = cms.PSet(
    variables = cms.vstring("1"),
    bins = cms.VPSet()
    )

scalesAndSmearingsPrefix = cms.string("EgammaAnalysis/ElectronTools/data/76X_16DecRereco_2015_photons")


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
          OverallRange = cms.string("abs(superCluster.eta)<1.0"),
          BinList = materialBinsRun1,
          ApplyCentralValue = cms.bool(False),
          Debug = cms.untracked.bool(False)
          )

MaterialForward = cms.PSet( PhotonMethodName = cms.string("FlashggPhotonScale"),
          MethodName = cms.string("FlashggDiPhotonFromPhoton"),
          Label = cms.string("MaterialForward"),
          NSigmas = cms.vint32(-1,1),
          OverallRange = cms.string("abs(superCluster.eta)>=1.0"),
          BinList = materialBinsRun1,
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
          OverallRange = cms.string("1"),
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
          ApplyCentralValue = cms.bool(True)
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

FracRVWeight = cms.PSet( MethodName = cms.string("FlashggDiPhotonWeightFromFracRV"),
          Label = cms.string("FracRVWeight"),
          NSigmas = cms.vint32(-1,1),
          OverallRange = cms.string("1"),
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
