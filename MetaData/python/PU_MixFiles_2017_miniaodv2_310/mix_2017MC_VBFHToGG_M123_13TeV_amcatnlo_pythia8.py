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
		1.62994e-05,8.14969e-06,1.01871e-05,4.27859e-05,5.09356e-05,0.000120208,0.000112058,0.000118171,0.000338212,0.000350437,0.00067235,0.00137119,0.00233285,0.00390167,0.00601855,0.008384,0.0117213,0.0148528,0.0180903,0.020796,0.0235648,0.025154,0.026731,0.0269612,0.0281001,0.0293267,0.0304024,0.0305552,0.0305919,0.0311889,0.0309322,0.0312133,0.0316697,0.0310096,0.0303698,0.0297138,0.0292493,0.02755,0.025912,0.0257897,0.0235547,0.0215417,0.0197671,0.0180781,0.0160121,0.0145268,0.0131719,0.012956,0.0123631,0.0123468,0.012361,0.0124038,0.0123101,0.0123203,0.0123223,0.0125098,0.0123672,0.0122164,0.0118171,0.0108371,0.00947198,0.0082536,0.00681518,0.00554587,0.00423784,0.00355327,0.00325784,0.00241842,0.00171551,0.00113077,0.000733472,0.000566404,0.00038711,0.000220042,0.000215967,0.000128358,0.000105946,6.92724e-05,3.66736e-05,1.22245e-05,1.22245e-05,1.01871e-05,6.11227e-06,2.03742e-06,6.11227e-06,0,2.03742e-06,2.03742e-06,2.03742e-06,0,0,0,4.07485e-06,0,0,0,0,0,0,0	
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
