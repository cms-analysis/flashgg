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
                4e-05, 1e-05, 1.66667e-05, 7e-05, 5.33333e-05, 0.00011, 9e-05, 0.000156667, 0.000366667, 0.00032, 0.000673333, 0.00129333, 0.00228, 0.0037, 0.00590667, 0.00855333, 0.0121267, 0.01484, 0.01796, 0.0209167, 0.02334, 0.02488, 0.02697, 0.02718, 0.02823, 0.0287767, 0.0299967, 0.0311967, 0.0310233, 0.0309667, 0.0313933, 0.0309367, 0.03122, 0.03071, 0.0306633, 0.03, 0.0295933, 0.02728, 0.0268967, 0.0255233, 0.0232933, 0.0219967, 0.0197933, 0.0176033, 0.01608, 0.0144967, 0.0131433, 0.01286, 0.01234, 0.0126167, 0.0128433, 0.0125433, 0.0125433, 0.0119967, 0.0121533, 0.0123133, 0.0123967, 0.0121233, 0.01179, 0.01059, 0.00953333, 0.00812, 0.00657333, 0.00552, 0.00422333, 0.00352333, 0.00304, 0.00227667, 0.00170667, 0.00117333, 0.000813333, 0.00052, 0.000333333, 0.000226667, 0.000153333, 0.000163333, 9.66667e-05, 8.33333e-05, 3.33333e-05, 4.33333e-05, 1.33333e-05, 1.66667e-05, 6.66667e-06, 6.66667e-06, 1e-05, 6.66667e-06, 3.33333e-06, 0, 3.33333e-06, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
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
