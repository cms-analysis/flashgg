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
		0.0340441,0.00157639,0.0043257,0.00152647,0.00092545,0.00257462,0.00141625,0.000632218,0.00175732,0.00158054,0.00643032,0.00265157,0.00349799,0.00514717,0.00896127,0.0140003,0.0144869,0.0160238,0.0180515,0.0208195,0.0234274,0.0244049,0.0273601,0.025767,0.0262662,0.0270044,0.0286598,0.0276866,0.0285683,0.0274474,0.0276782,0.0275743,0.0280255,0.0274037,0.0281711,0.0265116,0.0258731,0.0242863,0.0236603,0.0222711,0.0209214,0.020859,0.0172404,0.015402,0.0143393,0.015088,0.011929,0.0117064,0.0112905,0.0110825,0.0111262,0.0112863,0.0111512,0.011303,0.0114465,0.011382,0.0115026,0.011174,0.0105855,0.00992,0.0088968,0.00740776,0.0062993,0.00515965,0.00374547,0.00324635,0.00272852,0.00206511,0.00152231,0.000971203,0.00126444,0.000519916,0.000295312,0.000243321,0.000212126,0.000942088,0.000653014,0.000463765,0.000326507,0.000162214,0.000128939,7.27882e-05,6.23899e-05,0.00325675,2.91153e-05,1.66373e-05,1.8717e-05,6.23899e-06,2.07966e-06,2.07966e-06,2.07966e-06,2.07966e-06,2.07966e-06,0,0,0,4.15933e-06,0,0,5.61509e-05	
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
