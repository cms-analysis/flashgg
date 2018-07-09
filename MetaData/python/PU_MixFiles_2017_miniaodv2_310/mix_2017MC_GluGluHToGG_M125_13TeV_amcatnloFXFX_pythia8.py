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
		0.0203742,0.000847726,0.00195594,0.00163894,0.00127878,0.0011565,0.00124847,0.00132708,0.00158705,0.00160606,0.00259661,0.00256013,0.00421397,0.00674276,0.00974782,0.0115291,0.0144807,0.0166201,0.0193009,0.0219309,0.0237815,0.0247695,0.025513,0.0263679,0.026502,0.0272809,0.0290297,0.0287004,0.0284523,0.0290539,0.0285704,0.0284636,0.0287441,0.0289434,0.0281312,0.0269356,0.0260591,0.0248461,0.0243344,0.0246704,0.0212661,0.0198455,0.0177781,0.0162948,0.0144458,0.0132569,0.0122925,0.0123126,0.0114494,0.0113205,0.0114962,0.0115918,0.0121379,0.0123532,0.0114042,0.0121847,0.0116144,0.011509,0.0109665,0.0109778,0.00901313,0.00785662,0.00655215,0.00499593,0.00397404,0.00342481,0.0029691,0.00212856,0.00153002,0.00107636,0.000741889,0.000796349,0.000335494,0.000227088,0.000193693,0.000153105,0.000122792,6.73043e-05,4.05881e-05,0.000557444,0.000674585,0.000304154,7.19283e-06,0.00154029,0.000797376,4.52121e-05,1.02755e-06,0.00127108,0.00016338,1.54132e-06,1.02755e-06,0,5.13773e-07,1.48994e-05,0,0,1.02755e-06,0,1.02755e-06,2.82575e-05	
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
