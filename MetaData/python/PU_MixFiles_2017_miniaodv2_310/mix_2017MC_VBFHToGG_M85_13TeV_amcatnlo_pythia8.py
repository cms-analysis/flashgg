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
                5.33333e-05, 1.66667e-05, 6.66667e-06, 7.66667e-05, 5.33333e-05, 0.0001, 0.000126667, 0.00012, 0.00034, 0.00037, 0.000713333, 0.00134667, 0.00244333, 0.00378, 0.00604, 0.00841, 0.0115533, 0.01497, 0.0177267, 0.0210367, 0.02321, 0.02489, 0.0260633, 0.0273467, 0.0288967, 0.0293467, 0.0297867, 0.0309233, 0.0312633, 0.0311667, 0.0303767, 0.0309333, 0.03123, 0.0312233, 0.0310167, 0.0299033, 0.0288033, 0.02738, 0.0271533, 0.0255433, 0.02402, 0.02157, 0.0195267, 0.0174933, 0.0158767, 0.0145233, 0.01324, 0.01257, 0.01257, 0.01276, 0.01252, 0.0127567, 0.0122533, 0.0123, 0.0122967, 0.0126, 0.01221, 0.0125333, 0.01166, 0.0105567, 0.00975333, 0.00811667, 0.00681333, 0.00547, 0.00414667, 0.00351667, 0.003, 0.00209333, 0.0017, 0.00121667, 0.000826667, 0.000533333, 0.000363333, 0.00026, 0.00019, 0.000136667, 9.66667e-05, 7.33333e-05, 4e-05, 2.33333e-05, 6.66667e-06, 3.33333e-06, 1e-05, 1e-05, 6.66667e-06, 6.66667e-06, 6.66667e-06, 0, 3.33333e-06, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
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
