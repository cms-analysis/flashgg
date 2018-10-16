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
		0.0201143,0.00136302,0.000493006,0.00105401,0.00230403,0.00074501,0.00217203,0.000642008,0.00189602,0.00156102,0.00179702,0.00310704,0.00455306,0.00608408,0.0080251,0.0105611,0.0144082,0.0166352,0.0193973,0.0212173,0.0236753,0.0246033,0.0252473,0.0265993,0.0271634,0.0287614,0.0284044,0.0287724,0.0285464,0.0289454,0.0289274,0.0287064,0.0293234,0.0288054,0.0294204,0.0277604,0.0267553,0.0250623,0.0240583,0.0242893,0.0212883,0.0197723,0.0181072,0.0162312,0.0146662,0.0133852,0.0128422,0.0119192,0.0113471,0.0113411,0.0117712,0.0115231,0.0115361,0.0118792,0.0114551,0.0116032,0.0114861,0.0113911,0.0109441,0.0101781,0.00887812,0.0076291,0.00632608,0.00660009,0.00396505,0.00338304,0.00307004,0.00211203,0.00154902,0.00105801,0.000722009,0.00184202,0.000317004,0.000213003,0.000159002,0.000162002,9.40012e-05,0.000563007,0.00121202,2.90004e-05,7.00009e-06,7.00009e-06,7.00009e-06,0.00155802,2.00003e-06,1.00001e-06,5.00007e-06,2.00003e-06,0.00120002,2.00003e-06,0.000198003,0,1.00001e-06,0,0,0.000464006,5.00007e-06,0,4.00005e-06,2.90004e-05	
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
