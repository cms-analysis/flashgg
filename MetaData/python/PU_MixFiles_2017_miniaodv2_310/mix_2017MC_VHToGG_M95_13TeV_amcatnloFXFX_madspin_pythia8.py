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
		2.83951e-05,4.05645e-06,8.1129e-06,2.43387e-05,5.67903e-05,9.53266e-05,0.000141976,0.000103439,0.00030829,0.000361024,0.000701766,0.00136297,0.00236694,0.00397126,0.00606236,0.00847392,0.0115589,0.0150778,0.018327,0.0208481,0.0227161,0.0247139,0.0260465,0.0275656,0.0279996,0.0296993,0.0304883,0.0309041,0.0305552,0.0315511,0.0306972,0.0312854,0.0312509,0.0308371,0.0306627,0.030399,0.029032,0.0275068,0.0261824,0.0255759,0.0236085,0.0214748,0.0193209,0.0178058,0.0159114,0.0147351,0.0131652,0.0130293,0.0126277,0.0125121,0.0123052,0.0125243,0.0128326,0.0123559,0.0121876,0.0125365,0.0121166,0.0119645,0.0115568,0.0109139,0.0093055,0.00846987,0.00697304,0.00540928,0.00417814,0.00370557,0.00323502,0.00228784,0.00159824,0.00112364,0.00077681,0.000571959,0.000432012,0.000245415,0.000188625,0.000152117,9.73548e-05,5.88185e-05,3.85363e-05,4.86774e-05,1.01411e-05,1.41976e-05,1.21693e-05,8.1129e-06,1.01411e-05,0,4.05645e-06,6.08467e-06,0,0,0,0,0,2.02822e-06,0,0,0,0,0,0	
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
