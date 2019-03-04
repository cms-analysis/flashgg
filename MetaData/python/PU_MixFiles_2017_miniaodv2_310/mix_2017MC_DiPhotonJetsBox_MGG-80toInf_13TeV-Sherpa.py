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
                3.26777e-05, 6.85405e-06, 1.35871e-05, 3.53387e-05, 6.06785e-05, 9.23885e-05, 0.000117749, 0.000129562, 0.000360725, 0.000359414, 0.000705362, 0.00135279, 0.00239233, 0.00389848, 0.00595183, 0.00856647, 0.0116702, 0.0148918, 0.017971, 0.0208698, 0.0232566, 0.0249577, 0.0262321, 0.0273188, 0.028307, 0.0294292, 0.0302919, 0.0309576, 0.0309133, 0.0310445, 0.0309276, 0.0310518, 0.0310053, 0.0310816, 0.0308109, 0.0300332, 0.0288562, 0.0273382, 0.0264614, 0.0255926, 0.0235757, 0.0215459, 0.019547, 0.0177379, 0.0160384, 0.0145823, 0.0133747, 0.0129583, 0.0124985, 0.0124419, 0.0123811, 0.0124056, 0.0124209, 0.0124457, 0.0123283, 0.0124554, 0.012328, 0.0122115, 0.0117078, 0.0108403, 0.00962105, 0.0082907, 0.00682476, 0.0054551, 0.00420171, 0.00367661, 0.00313674, 0.00228488, 0.00162715, 0.00113689, 0.000784244, 0.000531451, 0.000358729, 0.000241605, 0.000195199, 0.000159901, 0.000103536, 6.86211e-05, 4.34426e-05, 2.88676e-05, 1.79213e-05, 1.1289e-05, 7.55961e-06, 6.10817e-06, 5.28165e-06, 2.92305e-06, 1.95542e-06, 1.29017e-06, 8.06359e-07, 6.85405e-07, 5.03974e-07, 2.0159e-07, 2.82226e-07, 2.21749e-07, 2.0159e-08, 2.0159e-08, 0, 0, 0, 0
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
