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
                2.66841e-05, 1.21291e-05, 1.45549e-05, 3.39615e-05, 5.5794e-05, 8.2478e-05, 9.94588e-05, 0.000143124, 0.000339615, 0.000356596, 0.000727747, 0.00139, 0.00243068, 0.00385949, 0.00590931, 0.00864564, 0.0117264, 0.0149455, 0.0181282, 0.0208184, 0.0233316, 0.0248623, 0.0263857, 0.0270989, 0.0289619, 0.0293549, 0.0295441, 0.0312543, 0.0304926, 0.0310045, 0.0307425, 0.0311694, 0.0312034, 0.0314217, 0.0306139, 0.0296072, 0.0289231, 0.0272687, 0.0262765, 0.0255658, 0.0236809, 0.0215874, 0.0194139, 0.0176867, 0.0163137, 0.0145719, 0.0132426, 0.0128108, 0.0124226, 0.012493, 0.0124469, 0.0122165, 0.0124372, 0.0124906, 0.0123766, 0.0122213, 0.0124372, 0.0122917, 0.0116124, 0.0110836, 0.00982216, 0.00850979, 0.00694756, 0.00557212, 0.00415786, 0.00377216, 0.00311718, 0.00224874, 0.00153312, 0.00115227, 0.000802948, 0.000446352, 0.000368725, 0.000201343, 0.000189214, 0.000169808, 0.000109162, 7.27747e-05, 4.36648e-05, 3.15357e-05, 2.42582e-05, 1.21291e-05, 4.85165e-06, 4.85165e-06, 1.69808e-05, 0, 2.42582e-06, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
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
