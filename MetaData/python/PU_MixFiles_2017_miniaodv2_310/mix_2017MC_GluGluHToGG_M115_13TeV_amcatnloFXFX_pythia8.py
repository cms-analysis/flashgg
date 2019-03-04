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
               2.50424e-05, 8.34745e-06, 3.33898e-05, 5.84322e-05, 7.51271e-05, 8.34745e-05, 0.000133559, 0.000116864, 0.000325551, 0.000383983, 0.000742923, 0.0014608, 0.00231224, 0.0040986, 0.00583487, 0.00855614, 0.0111605, 0.0150087, 0.0185564, 0.0210606, 0.0223628, 0.0255599, 0.026378, 0.0274297, 0.0287152, 0.029191, 0.0307353, 0.0307353, 0.0311444, 0.0312529, 0.0316452, 0.0311527, 0.0307103, 0.0306519, 0.0301176, 0.0299173, 0.0285566, 0.0275716, 0.0263195, 0.0248838, 0.0234146, 0.0217618, 0.0190739, 0.0175213, 0.0159269, 0.014583, 0.0133559, 0.01278, 0.0123041, 0.012037, 0.0123041, 0.0126297, 0.0123208, 0.0124461, 0.0121038, 0.013214, 0.0125963, 0.0120287, 0.0115863, 0.0110687, 0.00995851, 0.00890673, 0.00707864, 0.0052589, 0.00421546, 0.00365618, 0.0032889, 0.00231224, 0.00164445, 0.00112691, 0.000767966, 0.000500847, 0.000342246, 0.000300508, 0.000141907, 0.000141907, 7.51271e-05, 6.67796e-05, 2.50424e-05, 2.50424e-05, 2.50424e-05, 0, 8.34745e-06, 0, 8.34745e-06, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
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
