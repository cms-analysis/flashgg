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
		2.53156e-05,8.43853e-06,0,2.53156e-05,6.75083e-05,5.90697e-05,0.000126578,0.000126578,0.000413488,0.000396611,0.000675083,0.00127422,0.00237123,0.00406737,0.00594073,0.00862418,0.0119152,0.0149953,0.0177716,0.021206,0.0236954,0.0252481,0.0258304,0.0278894,0.0279822,0.029307,0.0314082,0.0302775,0.0313407,0.0299652,0.0305391,0.0313407,0.0309188,0.0318555,0.0302521,0.0300243,0.0281931,0.0275096,0.026708,0.0254844,0.0234591,0.0217461,0.0186576,0.0179741,0.0164129,0.0147252,0.0138645,0.0128097,0.0126072,0.0126409,0.0119912,0.0124806,0.0122021,0.0127337,0.0124722,0.012565,0.0118646,0.0118646,0.0117971,0.0109448,0.00950179,0.00802505,0.0070293,0.00529096,0.00414332,0.00366232,0.00337541,0.00208432,0.00165395,0.00108857,0.000742591,0.000607574,0.000303787,0.00022784,0.000168771,0.000126578,0.000101262,9.28239e-05,4.21927e-05,1.68771e-05,3.37541e-05,0,0,0,0,0,8.43853e-06,0,0,0,0,0,0,0,0,0,0,0,0,0	
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
