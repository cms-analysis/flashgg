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
		0.0146274,6.69445e-05,4.01667e-05,0.000174056,0.000254389,0.000425097,0.000528861,0.00569363,0.00151629,0.00150625,0.00283175,0.00475975,0.00631287,0.00859567,0.00999816,0.0110793,0.0139814,0.0157755,0.0186909,0.0217268,0.024254,0.0248565,0.0241301,0.0256565,0.0261619,0.0274205,0.0277719,0.0287326,0.0339743,0.02891,0.029238,0.0288732,0.0287091,0.0285686,0.0285016,0.0283343,0.0271226,0.0247259,0.0253452,0.0233871,0.0216097,0.0199193,0.0180583,0.0163378,0.0148617,0.0133387,0.0123412,0.0121672,0.0110626,0.0112199,0.0143964,0.0117153,0.0115211,0.0114575,0.0113337,0.0118291,0.0115379,0.0115178,0.0109956,0.0102827,0.00896052,0.00771535,0.00649362,0.0051882,0.00400328,0.00345768,0.00279493,0.00227611,0.00173721,0.00106442,0.000692875,0.000505431,0.00063932,0.000194139,0.000194139,0.000187445,9.37223e-05,7.69862e-05,0.00283175,2.34306e-05,2.00833e-05,1.00417e-05,1.00417e-05,6.69445e-06,3.34722e-06,3.34722e-06,3.34722e-06,0,0,3.34722e-06,0,0,0,0,0,0,3.34722e-06,0,0,4.35139e-05	
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
