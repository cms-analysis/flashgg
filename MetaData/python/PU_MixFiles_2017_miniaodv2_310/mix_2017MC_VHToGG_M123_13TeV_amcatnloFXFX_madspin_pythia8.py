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
		2.93069e-05,5.86139e-06,2.63763e-05,1.17228e-05,4.98218e-05,7.91288e-05,0.000117228,8.79208e-05,0.000339961,0.000398575,0.000694575,0.00132174,0.00232111,0.00401212,0.00587897,0.00851367,0.011714,0.0147179,0.0179036,0.0210072,0.023258,0.02499,0.0262327,0.0273024,0.0290197,0.0293919,0.030277,0.0309481,0.030784,0.0315138,0.0309335,0.0306727,0.0306082,0.0311533,0.03108,0.0308514,0.0288879,0.0269507,0.0263264,0.0255351,0.0236038,0.0214439,0.0194188,0.0174904,0.0158726,0.0142813,0.0134871,0.0127104,0.0126723,0.0126753,0.0122591,0.0125873,0.0124467,0.0124672,0.012262,0.0126606,0.0121624,0.0122767,0.0117081,0.0108172,0.0094808,0.00838472,0.00667612,0.00547747,0.00418503,0.0038099,0.00314464,0.00222147,0.00167636,0.00108436,0.000788357,0.000574416,0.000383921,0.000234456,0.000181703,0.000146535,0.000123089,7.61981e-05,4.39604e-05,2.05149e-05,1.17228e-05,8.79208e-06,5.86139e-06,5.86139e-06,0,0,0,2.93069e-06,0,0,0,0,0,0,0,0,0,0,0,0	
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
