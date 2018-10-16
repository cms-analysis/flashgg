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
		3.38498e-05,0,2.53874e-05,4.23123e-05,9.3087e-05,4.23123e-05,0.000110012,9.3087e-05,0.000321573,0.000431585,0.000795471,0.00155709,0.00242026,0.00388427,0.00622837,0.00820012,0.0119236,0.0147501,0.0176611,0.0205807,0.0228909,0.0249896,0.0269783,0.027393,0.0278161,0.0293478,0.0290685,0.0310064,0.0304733,0.0314888,0.0306849,0.0310064,0.0310149,0.0314042,0.0294324,0.0303379,0.0288316,0.0283577,0.0265129,0.0261067,0.0242026,0.0216893,0.0200814,0.0173903,0.0157486,0.0144877,0.0137176,0.0128629,0.0126006,0.0120675,0.0121859,0.0129306,0.0129476,0.0120929,0.0123383,0.012059,0.0115089,0.0123213,0.0119405,0.011035,0.00940179,0.00820858,0.00672765,0.00572062,0.00415507,0.00396043,0.00335113,0.00257259,0.00159094,0.00109166,0.000761621,0.000693921,0.000313111,0.000186174,0.000245411,0.000186174,0.000110012,7.61621e-05,3.38498e-05,8.46246e-06,1.69249e-05,8.46246e-06,8.46246e-06,1.69249e-05,0,0,0,8.46246e-06,0,0,0,0,0,0,0,0,0,0,0,0	
                ),
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
