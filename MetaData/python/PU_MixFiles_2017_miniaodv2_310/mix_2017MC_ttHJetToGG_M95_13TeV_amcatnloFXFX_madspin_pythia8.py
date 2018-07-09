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
		4.17533e-05,2.60958e-06,1.04383e-05,4.17533e-05,7.82875e-05,0.000112212,0.000120041,9.91642e-05,0.000326198,0.00037578,0.000707197,0.0013335,0.00226773,0.00368995,0.0059916,0.00867686,0.0115057,0.0147546,0.0178835,0.0209236,0.0233505,0.0253965,0.0262942,0.0272571,0.0283636,0.0293161,0.0307565,0.030947,0.0305347,0.0316125,0.0306783,0.0306365,0.0313359,0.0306809,0.0305217,0.030112,0.0286062,0.0273954,0.0259262,0.0254173,0.023468,0.0214299,0.0196241,0.0179539,0.016002,0.0148564,0.0133376,0.0133402,0.012787,0.0124503,0.0123407,0.0128391,0.0121554,0.0122624,0.0125495,0.0125938,0.0123172,0.0123694,0.0114926,0.0108089,0.00964241,0.00825411,0.00707458,0.00528441,0.00413097,0.00368734,0.00319152,0.00235123,0.0015788,0.00118214,0.000816799,0.000485382,0.000383609,0.000266177,0.00018789,0.000153965,0.000104383,5.48012e-05,3.65342e-05,3.1315e-05,1.56575e-05,7.82875e-06,2.60958e-06,7.82875e-06,0,7.82875e-06,0,0,0,0,0,0,0,0,0,0,0,0,0,0	
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
