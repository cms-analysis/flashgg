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
		2.4e-05,6e-06,1.8e-05,3.6e-05,6.6e-05,9e-05,0.000132,0.00013,0.000354,0.000402,0.000732,0.001388,0.002426,0.003892,0.005844,0.00863,0.01155,0.014908,0.017916,0.02076,0.02314,0.025118,0.025936,0.027124,0.028276,0.029346,0.03026,0.03082,0.031006,0.03113,0.031072,0.03089,0.031104,0.031304,0.030668,0.030276,0.028604,0.027132,0.026398,0.025434,0.023822,0.021716,0.019606,0.017936,0.016022,0.01453,0.013678,0.012932,0.012736,0.012384,0.012266,0.012338,0.012296,0.01244,0.01234,0.012602,0.012268,0.012478,0.011732,0.010718,0.009526,0.008384,0.006766,0.00553,0.00424,0.003642,0.003146,0.00235,0.001566,0.001098,0.000816,0.000526,0.000346,0.000254,0.000196,0.00017,7e-05,5.8e-05,5.4e-05,2.6e-05,1.2e-05,1.2e-05,8e-06,4e-06,1e-05,8e-06,0,0,0,0,0,0,0,0,0,0,0,0,0,0	
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
