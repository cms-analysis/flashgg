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
		0.0229428,0.00065346,0.00156364,0.00338732,0.00162531,0.000916844,0.000970188,0.00114355,0.000771816,0.000681798,0.00134359,0.00284222,0.00750478,0.00685799,0.00955351,0.00981523,0.0134426,0.0161381,0.018867,0.0221343,0.0243864,0.0246814,0.0250382,0.0255666,0.02577,0.0271919,0.0278771,0.0284572,0.0280954,0.0282155,0.0280738,0.0285022,0.029379,0.0282105,0.0277804,0.0297374,0.0264151,0.0249598,0.023998,0.0232778,0.0223043,0.0195805,0.0179501,0.0182519,0.0146578,0.0132176,0.0123157,0.0117756,0.0113139,0.0136343,0.0117289,0.0113289,0.0128208,0.013546,0.0114272,0.0115656,0.0113272,0.0110871,0.0109071,0.00983357,0.00874502,0.00754813,0.00622954,0.00507932,0.00385575,0.00338565,0.00310227,0.00208374,0.00153363,0.00111855,0.00074181,0.000458422,0.000351735,0.000220043,0.000190037,0.000125024,0.000116689,6.00116e-05,4.83427e-05,2.50048e-05,1.33359e-05,1.66699e-05,0.00258883,1.16689e-05,1.00019e-05,8.33494e-06,6.66796e-06,1.66699e-06,0.0025855,1.66699e-06,0,0,0,0,0,0,1.66699e-06,0.000363404,1.66699e-06,2.66718e-05	
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
