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
               4.26205e-05, 1.21773e-05, 1.21773e-05, 2.2325e-05, 5.88568e-05, 9.13296e-05, 0.000107566, 0.000129891, 0.000318639, 0.0003572, 0.000720489, 0.00134965, 0.00246184, 0.00393529, 0.00576797, 0.00864993, 0.0116577, 0.0148664, 0.0177788, 0.0207379, 0.023078, 0.0251887, 0.0263597, 0.0276262, 0.0283507, 0.0291727, 0.0303255, 0.0311617, 0.0308024, 0.0306441, 0.0308795, 0.031119, 0.0307537, 0.0315615, 0.030496, 0.0299541, 0.0288135, 0.0274334, 0.0261527, 0.0254424, 0.0237721, 0.0215761, 0.0198307, 0.0173628, 0.015869, 0.014633, 0.0134315, 0.012912, 0.0127192, 0.0123782, 0.0124005, 0.0124878, 0.0126745, 0.0123681, 0.0126197, 0.0126948, 0.0120941, 0.0122564, 0.0117795, 0.0108033, 0.00969717, 0.00831708, 0.00683145, 0.00553051, 0.00419101, 0.00367145, 0.00303214, 0.00218785, 0.00161146, 0.00122179, 0.000797612, 0.000507387, 0.000387643, 0.000221221, 0.000251664, 0.000156275, 0.000121773, 6.49455e-05, 5.27682e-05, 3.24727e-05, 1.62364e-05, 1.62364e-05, 1.01477e-05, 4.05909e-06, 6.08864e-06, 0, 0, 0, 0, 0, 0, 0, 2.02955e-06, 0, 2.02955e-06, 0, 0, 0, 0, 0
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
