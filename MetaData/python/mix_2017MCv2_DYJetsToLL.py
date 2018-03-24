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
                0.0235,
                0.000819,
                0.000889,
                0.00145,
                0.00149,
                0.00144,
                0.00135,
                0.0012,
                0.00209,
                0.00162,
                0.00243,
                0.00332,
                0.00483,
                0.00647,
                0.00858,
                0.0111,
                0.014,
                0.0167,
                0.0193,
                0.0216,
                0.0233,
                0.0243,
                0.0253,
                0.026,
                0.0266,
                0.0277,
                0.0284,
                0.0287,
                0.0286,
                0.0287,
                0.0284,
                0.0287,
                0.0286,
                0.0284,
                0.0281,
                0.0273,
                0.0263,
                0.0251,
                0.0244,
                0.0232,
                0.0216,
                0.0197,
                0.018,
                0.0163,
                0.0147,
                0.0133,
                0.0124,
                0.0121,
                0.0118,
                0.0117,
                0.012,
                0.0119,
                0.0118,
                0.0118,
                0.0116,
                0.0119,
                0.0117,
                0.0114,
                0.011,
                0.0102,
                0.00919,
                0.00791,
                0.00641,
                0.00533,
                0.00431,
                0.00358,
                0.00305,
                0.00216,
                0.00173,
                0.00121,
                0.00089,
                0.000838,
                0.000476,
                0.000303,
                0.000346,
                0.000338,
                0.000194,
                0.000211,
                0.000315,
                0.000124,
                0.00058,
                0.000641,
                0.00015,
                0.000328,
                0.000242,
                0.000428,
                0.000212,
                0.000184,
                0.000406,
                0.000333,
                3.19e-05,
                6.63e-05,
                1.69e-05,
                4.83e-07,
                4.95e-05,
                0.000147,
                3.68e-06,
                6.31e-07,
                8.54e-07,
                3.02e-05),
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
