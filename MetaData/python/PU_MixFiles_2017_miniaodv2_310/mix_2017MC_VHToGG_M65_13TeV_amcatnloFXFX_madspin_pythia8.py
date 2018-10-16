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
		3.66719e-05,8.7314e-06,1.2224e-05,3.66719e-05,7.509e-05,8.03289e-05,0.000120493,0.000144941,0.00036148,0.000385928,0.000747408,0.00142147,0.00231557,0.00385928,0.00602816,0.00873314,0.011604,0.0148923,0.0181386,0.0208209,0.0231382,0.0248426,0.0261802,0.0275528,0.028536,0.0291349,0.0306158,0.0311047,0.0309109,0.0310541,0.0309423,0.0309546,0.0311397,0.0311833,0.0308201,0.0297077,0.0283893,0.0272001,0.0266273,0.0255009,0.0234176,0.0215893,0.0194082,0.0178225,0.0161898,0.0146722,0.013043,0.0128858,0.0125889,0.0122065,0.0122921,0.0125715,0.0123252,0.01254,0.0125103,0.0123968,0.0123217,0.0120703,0.0115481,0.0108706,0.00974948,0.00844152,0.00689257,0.00550777,0.00411773,0.00364274,0.00329872,0.00225445,0.00166595,0.00114207,0.00081202,0.000546585,0.000366719,0.000247972,0.000188598,0.000171135,0.000110016,6.46123e-05,5.58809e-05,2.96868e-05,1.57165e-05,1.92091e-05,6.98512e-06,6.98512e-06,3.49256e-06,5.23884e-06,1.74628e-06,0,3.49256e-06,0,0,0,0,1.74628e-06,0,0,0,0,0,0	
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
