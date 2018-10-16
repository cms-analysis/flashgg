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
		2.50899e-05,8.3633e-06,0,5.01798e-05,0.000117086,7.52697e-05,7.52697e-05,0.000133813,0.000376349,0.000259262,0.000660701,0.00127959,0.00224973,0.00400602,0.0056285,0.00840512,0.0112905,0.0154637,0.0181484,0.0214686,0.0238354,0.0252321,0.0263946,0.0264615,0.0274985,0.0290876,0.0301664,0.0317471,0.0304591,0.0308439,0.0304257,0.0309777,0.0304591,0.0310948,0.0309442,0.0292297,0.0293134,0.0269466,0.0267626,0.0257004,0.0237267,0.022422,0.0198461,0.0179226,0.0160324,0.01441,0.0141591,0.0133896,0.0132893,0.0124111,0.0124195,0.011717,0.012846,0.0130133,0.01257,0.011993,0.0114912,0.0116334,0.0119093,0.0106632,0.0102367,0.0082964,0.00695827,0.00532742,0.00398929,0.00345404,0.00301915,0.00230827,0.00165593,0.00104541,0.00078615,0.000501798,0.000451618,0.000250899,0.000234172,0.000142176,9.19963e-05,2.50899e-05,1.67266e-05,8.3633e-06,8.3633e-06,1.67266e-05,0,8.3633e-06,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0	
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
