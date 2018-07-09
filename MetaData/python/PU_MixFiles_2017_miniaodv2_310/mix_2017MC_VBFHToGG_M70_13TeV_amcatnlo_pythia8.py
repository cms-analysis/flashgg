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
		0.026107,0.00224003,0.00174002,0.00486673,0.0163936,0.00812011,0.00538674,0.00433339,0.0125535,0.00892012,0.0133668,0.0053734,0.00449339,0.00950679,0.0101868,0.00950013,0.0119535,0.0151535,0.0177402,0.0196869,0.0207403,0.0215203,0.0222003,0.0224803,0.0238537,0.026167,0.0254203,0.0261803,0.026167,0.0268004,0.0262804,0.0257137,0.0262804,0.025967,0.0256203,0.0248537,0.0245603,0.023027,0.0223936,0.0218203,0.0201403,0.0183536,0.0164736,0.0148669,0.0130402,0.0122602,0.0115135,0.0107401,0.0105868,0.0105268,0.0112068,0.0116135,0.0110068,0.0111401,0.0110335,0.0114468,0.0108135,0.0114535,0.0103068,0.00935346,0.00892012,0.00787344,0.00577341,0.0053134,0.00430672,0.00340005,0.00291337,0.00223336,0.00147335,0.000893345,0.00072001,0.00051334,0.000346671,0.00026667,0.000193336,0.000146669,6.66676e-05,8.66678e-05,4.00005e-05,0,2.00003e-05,0.000546674,1.33335e-05,2.00003e-05,0.0103468,0,0,0,0,0,0,0,0,0,0,0,6.66676e-06,0,0,2.00003e-05	
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
