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
               3.8513e-05, 5.25178e-06, 2.62589e-05, 5.07672e-05, 6.30213e-05, 0.00011904, 0.000136546, 0.000145299, 0.000376377, 0.000362373, 0.000684482, 0.00136196, 0.00234579, 0.00391608, 0.005903, 0.00838009, 0.0116677, 0.0150411, 0.0180451, 0.0207813, 0.0233722, 0.0248584, 0.0261906, 0.0273653, 0.0283368, 0.0293907, 0.0302923, 0.0305864, 0.0310275, 0.0311255, 0.0310573, 0.0312516, 0.0312341, 0.0312638, 0.0307229, 0.0304673, 0.0288043, 0.0273898, 0.026441, 0.0256304, 0.0235682, 0.0213187, 0.0194911, 0.0181572, 0.0160862, 0.0144336, 0.0136284, 0.0125745, 0.012529, 0.0123172, 0.0123049, 0.0124922, 0.0123329, 0.0124345, 0.0121596, 0.0124082, 0.0120546, 0.0122839, 0.0117132, 0.0104493, 0.00964577, 0.00814026, 0.00689208, 0.00560715, 0.00415766, 0.00376903, 0.00305654, 0.00233354, 0.00167707, 0.00111163, 0.00079652, 0.000558439, 0.000360622, 0.000222325, 0.000218824, 0.000143549, 8.92802e-05, 8.40285e-05, 4.37648e-05, 2.62589e-05, 2.62589e-05, 1.22542e-05, 1.05036e-05, 7.00237e-06, 5.25178e-06, 1.75059e-06, 1.75059e-06, 0, 0, 1.75059e-06, 1.75059e-06, 1.75059e-06, 0, 0, 0, 0, 0, 0, 0, 0
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
