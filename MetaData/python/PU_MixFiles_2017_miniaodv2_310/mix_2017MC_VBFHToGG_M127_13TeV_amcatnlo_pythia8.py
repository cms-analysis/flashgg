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
		3e-05,2e-06,1.6e-05,4.4e-05,7.2e-05,0.000106,0.000152,0.000116,0.000378,0.000354,0.000678,0.001384,0.002482,0.003782,0.006014,0.008758,0.011886,0.014934,0.017964,0.021086,0.022986,0.02515,0.026088,0.0274,0.02844,0.029452,0.030184,0.030604,0.030958,0.031168,0.030608,0.030782,0.03113,0.031148,0.030844,0.03019,0.02855,0.027462,0.025838,0.025602,0.023516,0.021386,0.019678,0.018068,0.01611,0.014498,0.013194,0.012814,0.01262,0.012506,0.012252,0.012562,0.012572,0.012478,0.012288,0.012444,0.012392,0.012322,0.01177,0.010898,0.009604,0.008158,0.006702,0.005336,0.00435,0.00365,0.003288,0.002372,0.001646,0.001164,0.00079,0.00052,0.000338,0.00024,0.000186,0.000164,9.4e-05,5.8e-05,5.6e-05,3e-05,2.4e-05,1e-05,8e-06,1.2e-05,4e-06,4e-06,0,0,0,0,0,0,2e-06,0,0,0,0,0,0,0	
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
