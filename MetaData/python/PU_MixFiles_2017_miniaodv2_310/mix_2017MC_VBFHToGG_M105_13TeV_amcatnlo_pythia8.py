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
                2e-05, 3.33333e-06, 1.66667e-05, 2.33333e-05, 2.33333e-05, 8.66667e-05, 0.00014, 0.00015, 0.000356667, 0.00036, 0.000763333, 0.00130333, 0.00236667, 0.00388, 0.00604, 0.00872333, 0.0120167, 0.0150167, 0.0179833, 0.0210333, 0.0232733, 0.02505, 0.0261833, 0.0270467, 0.02856, 0.0294333, 0.03074, 0.0309267, 0.0308267, 0.0310767, 0.0305033, 0.0309033, 0.0313833, 0.0308333, 0.03107, 0.0293467, 0.0289433, 0.02762, 0.0264367, 0.0260767, 0.0232133, 0.02195, 0.0197933, 0.0174533, 0.01577, 0.0146933, 0.0135433, 0.0128933, 0.0124433, 0.0124067, 0.0120533, 0.0122633, 0.01249, 0.01244, 0.0123833, 0.01248, 0.0123867, 0.01182, 0.0120567, 0.0108233, 0.00939333, 0.00808, 0.0069, 0.00531, 0.00417667, 0.00363, 0.00305667, 0.00212667, 0.00161333, 0.00118333, 0.000933333, 0.000536667, 0.00032, 0.000223333, 0.000223333, 0.0001, 7.66667e-05, 9.66667e-05, 3.66667e-05, 3.66667e-05, 1.33333e-05, 6.66667e-06, 0, 1.33333e-05, 6.66667e-06, 3.33333e-06, 0, 6.66667e-06, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 
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
