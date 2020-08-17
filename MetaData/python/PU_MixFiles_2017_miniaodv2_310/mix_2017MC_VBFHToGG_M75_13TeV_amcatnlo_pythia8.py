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
		3.13788e-05,3.48653e-06,1.04596e-05,4.53249e-05,6.62441e-05,8.01902e-05,0.000122029,0.000146434,0.000362599,0.000359113,0.000638035,0.00138415,0.0022523,0.00389446,0.00620603,0.00878955,0.0115683,0.0150339,0.0176558,0.0207414,0.0230076,0.0253262,0.0260514,0.0271845,0.0289347,0.0292136,0.0301446,0.0301341,0.0308558,0.0312184,0.0309848,0.0307826,0.0310685,0.0314346,0.0312533,0.0302457,0.0290881,0.0268428,0.0261804,0.0259921,0.0238897,0.022251,0.0198209,0.0177569,0.0157173,0.0143506,0.013078,0.0131303,0.0121785,0.0123319,0.0122831,0.0125062,0.0124399,0.0123493,0.0126631,0.0123946,0.0124992,0.0124539,0.0115892,0.0107281,0.00947291,0.008155,0.00701141,0.00541807,0.00422916,0.00348653,0.00307861,0.00239873,0.0015027,0.000993661,0.000739145,0.000491601,0.000404438,0.000226625,0.000153407,0.000153407,0.000118542,5.9271e-05,2.44057e-05,5.57845e-05,2.44057e-05,1.39461e-05,3.48653e-06,1.39461e-05,1.04596e-05,3.48653e-06,3.48653e-06,0,0,3.48653e-06,3.48653e-06,0,0,0,0,0,0,0,0,0	
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
