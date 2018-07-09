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
		4.47782e-05,6.96549e-06,1.19408e-05,3.18423e-05,7.36352e-05,9.65218e-05,0.00014528,0.000123389,0.000369171,0.000343299,0.000694559,0.00138315,0.00246777,0.00390565,0.00594455,0.00854666,0.0114841,0.0150883,0.0179531,0.0208935,0.0234578,0.0247166,0.0263564,0.0272321,0.0284481,0.029275,0.0303924,0.0311557,0.0311477,0.0309328,0.0305397,0.0309785,0.0311039,0.0314611,0.0306014,0.0301566,0.0287436,0.0273903,0.0264331,0.0255454,0.0236021,0.0216199,0.0196427,0.0178426,0.0161032,0.0144713,0.0132922,0.0128464,0.0123538,0.0123021,0.0123866,0.0122165,0.0124752,0.0122254,0.0123916,0.012508,0.0122274,0.012316,0.0117179,0.010908,0.00966114,0.00833172,0.00680429,0.00539527,0.00427084,0.00363798,0.0031484,0.00222597,0.00168863,0.00117219,0.000752273,0.000527387,0.000346285,0.000248768,0.000202994,0.000164187,9.75169e-05,6.86599e-05,4.47782e-05,2.8857e-05,1.3931e-05,1.49261e-05,9.9507e-06,3.98028e-06,1.09458e-05,9.9507e-07,9.9507e-07,0,0,0,9.9507e-07,0,0,0,0,0,0,0,0,0	
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
