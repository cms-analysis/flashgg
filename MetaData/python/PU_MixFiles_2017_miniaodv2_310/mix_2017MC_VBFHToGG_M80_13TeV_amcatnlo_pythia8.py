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
		0.00837,0.01013,3e-05,0.00018,0.00361,0.00226,0.003,0.00346,0.00408,0.00046,0.0009,0.00208,0.00325,0.00758,0.00813,0.01162,0.01479,0.01868,0.01791,0.03658,0.02133,0.02347,0.02448,0.02477,0.04133,0.02691,0.02739,0.0281,0.02887,0.02802,0.02771,0.02883,0.02888,0.02853,0.02728,0.02691,0.02585,0.02434,0.02384,0.02238,0.0217,0.0195,0.0169,0.016,0.01466,0.01292,0.01158,0.01118,0.011,0.01071,0.01168,0.01149,0.01167,0.01114,0.01137,0.01104,0.01146,0.01135,0.0112,0.01004,0.00894,0.00775,0.00628,0.00471,0.00414,0.00345,0.00277,0.00209,0.00139,0.00099,0.00071,0.00057,0.00045,0.00026,0.00016,0.00013,0.00011,8e-05,2e-05,4e-05,1e-05,2e-05,1e-05,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1e-05	
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
