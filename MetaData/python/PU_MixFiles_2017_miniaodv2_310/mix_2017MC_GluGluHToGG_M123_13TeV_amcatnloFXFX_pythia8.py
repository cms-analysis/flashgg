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
		2.77827e-05,7.93791e-06,1.38913e-05,3.77051e-05,5.95343e-05,6.94567e-05,8.93015e-05,0.000128991,0.000363159,0.00035919,0.000670753,0.00134349,0.0023397,0.00382409,0.00572522,0.00863446,0.0116429,0.0147863,0.0180468,0.0210116,0.0232065,0.0248635,0.0266297,0.0273679,0.027995,0.0296957,0.0304141,0.0312059,0.0307455,0.0310015,0.0307713,0.0311007,0.0311841,0.0307435,0.0306225,0.0303248,0.0290408,0.0272667,0.0263003,0.0255005,0.0237701,0.0217261,0.0196265,0.0174396,0.0158719,0.0145165,0.0134389,0.012911,0.0126451,0.0124347,0.0125201,0.0125399,0.0123256,0.0124863,0.0125042,0.0125062,0.0124824,0.0123415,0.0115556,0.0108372,0.00956518,0.00819589,0.00673532,0.00530848,0.00417732,0.00369311,0.00308586,0.00236351,0.00154392,0.00110138,0.000773946,0.000523902,0.000414756,0.000234168,0.000206386,0.00015082,9.92239e-05,7.34257e-05,4.36585e-05,1.58758e-05,2.57982e-05,5.95343e-06,5.95343e-06,3.96895e-06,5.95343e-06,0,3.96895e-06,0,0,0,0,0,0,0,0,0,0,0,0,0	
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
