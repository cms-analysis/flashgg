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
		3.92364e-05,1.07008e-05,7.13389e-06,3.92364e-05,6.77719e-05,9.27406e-05,0.000117709,0.000103441,0.000328159,0.00033886,0.000677719,0.00141964,0.00254323,0.00361331,0.00570711,0.00842869,0.0114356,0.0153379,0.0179631,0.0213196,0.0229141,0.0251969,0.0261493,0.0271623,0.0283501,0.0296449,0.0302156,0.0315817,0.0305937,0.0310431,0.0309932,0.0308469,0.0316103,0.0310788,0.0308826,0.0299873,0.0286069,0.026959,0.0265809,0.0262206,0.0235561,0.0213054,0.0196111,0.017692,0.0156268,0.0142285,0.0135722,0.0128874,0.0125449,0.0123488,0.01191,0.0122097,0.0122346,0.0125877,0.012577,0.0125842,0.0124058,0.0122667,0.0119742,0.0106438,0.00951661,0.00832882,0.00703045,0.00548953,0.00403065,0.00382733,0.00322095,0.00227928,0.00160512,0.00107008,0.000781161,0.000474404,0.000349561,0.000271088,0.000192615,0.000228284,9.63075e-05,6.4205e-05,6.06381e-05,4.63703e-05,2.49686e-05,3.56694e-06,3.56694e-06,1.78347e-05,3.56694e-06,3.56694e-06,0,0,0,0,0,3.56694e-06,0,0,0,0,0,0,0,0	
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
