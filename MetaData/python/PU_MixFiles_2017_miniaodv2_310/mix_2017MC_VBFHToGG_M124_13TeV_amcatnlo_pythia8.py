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
		2.6e-05,6e-06,1.4e-05,3.8e-05,7.2e-05,7.8e-05,0.00012,0.000166,0.000364,0.000376,0.000688,0.00134,0.002306,0.003912,0.005902,0.008396,0.011532,0.014972,0.018368,0.020602,0.022872,0.025316,0.0261,0.027546,0.028422,0.029318,0.030846,0.030812,0.030846,0.031218,0.03095,0.031036,0.031332,0.030574,0.03088,0.030352,0.02897,0.027516,0.026116,0.025404,0.023468,0.021846,0.019446,0.017748,0.016032,0.014782,0.013536,0.01285,0.01216,0.012356,0.012224,0.012366,0.012354,0.012332,0.01255,0.01222,0.012498,0.012578,0.011406,0.010818,0.0096,0.008172,0.006888,0.005324,0.004234,0.003694,0.00317,0.0023,0.00166,0.001038,0.000768,0.000526,0.000408,0.00024,0.00021,0.000178,0.000122,6.8e-05,3.8e-05,3.2e-05,2.6e-05,8e-06,4e-06,1e-05,4e-06,0,2e-06,0,0,2e-06,0,0,0,0,0,0,0,0,0,0	
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
