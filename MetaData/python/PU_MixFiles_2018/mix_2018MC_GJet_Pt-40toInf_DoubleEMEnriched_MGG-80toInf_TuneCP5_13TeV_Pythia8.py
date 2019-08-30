import FWCore.ParameterSet.Config as cms

# configuration to model pileup for initial physics phase
from SimGeneral.MixingModule.mixObjects_cfi import theMixObjects
from SimGeneral.MixingModule.mixPoolSource_cfi import *
from SimGeneral.MixingModule.digitizers_cfi import *

mix = cms.EDProducer("MixingModule",
    digitizers = cms.PSet(theDigitizers),
    LabelPlayback = cms.string(''),
    maxBunch = cms.int32(3),
    minBunch = cms.int32(-12), ## in terms of 25 nsec

    bunchspace = cms.int32(25), ##ns
    mixProdStep1 = cms.bool(False),
    mixProdStep2 = cms.bool(False),

    playback = cms.untracked.bool(False),
    useCurrentProcessOnly = cms.bool(False),

    input = cms.SecSource("EmbeddedRootSource",
        type = cms.string('probFunction'),
        nbPileupEvents = cms.PSet(
          probFunctionVariable = cms.vint32(0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,96,97,98,99),
            probValue = cms.vdouble(
                0,1.66576e-06,1.07785e-06,5.97715e-06,1.77355e-05,3.86065e-05,6.91782e-05,0.000135613,0.000262897,0.000485717,0.000850127,0.00139013,0.00217558,0.00321345,0.00449746,0.00603937,0.00778029,0.00974021,0.0118532,0.0139816,0.0162381,0.0184173,0.020624,0.0227238,0.0247035,0.0267738,0.0285695,0.0302606,0.0317983,0.0333082,0.0345112,0.0354645,0.0361998,0.0366788,0.0370105,0.0369095,0.0365097,0.0360364,0.0352174,0.0340111,0.0327772,0.0311541,0.0294992,0.0276182,0.025663,0.0236218,0.0215596,0.0194767,0.0175001,0.015579,0.0136568,0.0118815,0.0101609,0.00862072,0.00726861,0.0060183,0.00497122,0.00401135,0.00325755,0.00255361,0.00202998,0.00160727,0.00122679,0.000945076,0.000725195,0.000540687,0.000416637,0.000320316,0.000231345,0.00016981,0.00012356,9.27928e-05,6.17312e-05,4.79152e-05,3.11596e-05,2.23408e-05,1.80294e-05,1.09744e-05,6.85902e-06,4.40937e-06,3.23354e-06,2.93958e-06,1.3718e-06,9.79861e-07,1.17583e-06,7.83889e-07,4.8993e-07,3.91944e-07,9.79861e-08,9.79861e-08,9.79861e-08,0,0,0,9.79861e-08,0,1.95972e-07,0,0,0),
          histoFileName = cms.untracked.string('histProbFunction.root'),
        ),
                          sequential = cms.untracked.bool(False),
                          manage_OOT = cms.untracked.bool(True),  ## manage out-of-time pileup
                          ## setting this to True means that the out-of-time pileup
        ## will have a different distribution than in-time, given
        ## by what is described on the next line:
        OOT_type = cms.untracked.string('Poisson'),  ## generate OOT with a Poisson matching the number chosen for in-time
        #OOT_type = cms.untracked.string('fixed'),  ## generate OOT with a fixed distribution
        #intFixed_OOT = cms.untracked.int32(2),
        fileNames = FileNames
    ),
    mixObjects = cms.PSet(theMixObjects)
)
