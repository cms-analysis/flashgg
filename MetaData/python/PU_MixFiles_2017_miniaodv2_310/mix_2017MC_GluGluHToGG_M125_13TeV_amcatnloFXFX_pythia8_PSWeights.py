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
		3.01971e-05,5.03284e-06,2.41577e-05,4.02628e-05,5.43547e-05,9.86437e-05,0.000120788,0.000135887,0.00037545,0.000348273,0.00068648,0.00137195,0.00237651,0.00385214,0.00599412,0.00864139,0.0118443,0.0147784,0.0179501,0.0210655,0.0232326,0.0250303,0.0261245,0.0274763,0.0285171,0.029262,0.0305282,0.030949,0.0309902,0.0308403,0.0310778,0.0314211,0.0310979,0.0313727,0.0307134,0.0299495,0.0286822,0.0270848,0.0264687,0.0253122,0.0234128,0.0213846,0.0194691,0.0177619,0.016089,0.0144342,0.013477,0.0126938,0.0123969,0.0124492,0.0123576,0.0124533,0.0125519,0.0124643,0.012416,0.0124009,0.0122177,0.0121634,0.0116983,0.0107481,0.00966004,0.00834244,0.00679937,0.00533381,0.00433831,0.00375953,0.00316566,0.00227082,0.00163869,0.00116259,0.000815321,0.000496238,0.000373437,0.000241577,0.000212386,0.0001379,9.26043e-05,7.34795e-05,4.52956e-05,2.41577e-05,2.21445e-05,7.04598e-06,8.05255e-06,7.04598e-06,4.02628e-06,2.01314e-06,0,0,1.00657e-06,0,0,0,0,0,0,0,0,0,0,0	
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
