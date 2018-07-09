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
		3.87339e-05,1.61391e-05,2.58226e-05,3.55061e-05,4.84174e-05,0.000100063,0.000112974,0.000164619,0.000361517,0.000377656,0.000652021,0.00139119,0.00232081,0.00406706,0.00603281,0.00850532,0.0115782,0.015103,0.0178434,0.0210035,0.0231629,0.0246154,0.026026,0.0270653,0.0282984,0.0293248,0.0302092,0.0309355,0.0309226,0.0314261,0.0313583,0.0307773,0.0306966,0.0311679,0.031042,0.0295992,0.0286792,0.0270589,0.0264359,0.026423,0.0234405,0.0219492,0.019354,0.0177111,0.0161359,0.0143574,0.0132696,0.0128467,0.0126821,0.0128306,0.0124917,0.0122464,0.0124885,0.0123077,0.0124045,0.012369,0.0121366,0.0121269,0.0115363,0.0109004,0.00937361,0.0085602,0.00699147,0.00561965,0.00436402,0.00355384,0.003202,0.00225302,0.00166233,0.0011943,0.000784362,0.000577781,0.000351833,0.000248543,0.000183986,0.000154936,0.00011943,6.77844e-05,3.87339e-05,3.55061e-05,1.9367e-05,1.9367e-05,9.68348e-06,9.68348e-06,0,6.45565e-06,3.22783e-06,0,3.22783e-06,0,3.22783e-06,0,0,0,0,0,0,0,0,0	
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
