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
		0.0156178,0.000475598,0.000919722,0.000325225,0.000482592,0.000591,0.000786834,0.00083929,0.00223986,0.00244268,0.00316832,0.00383451,0.00560925,0.00735253,0.00898915,0.012411,0.0149201,0.0189365,0.0196027,0.022444,0.0236627,0.0241558,0.0244041,0.0272402,0.0262453,0.026873,0.0285866,0.0292125,0.0284607,0.0289135,0.029863,0.0289065,0.0290115,0.0287981,0.0283837,0.0277001,0.0271231,0.0250476,0.0250301,0.0233882,0.0217341,0.0203213,0.0188823,0.0162595,0.0146578,0.013621,0.0123166,0.0131226,0.0114773,0.0115839,0.0114161,0.0114895,0.0118777,0.0117763,0.0113794,0.0114511,0.0115909,0.0118078,0.0111608,0.00995083,0.00908881,0.00778441,0.00631566,0.00504973,0.00549036,0.00398838,0.00307215,0.00204227,0.00139357,0.00105611,0.000751864,0.000515814,0.000360195,0.000199331,0.000199331,0.000164361,0.0011855,6.29467e-05,4.72101e-05,2.79763e-05,8.7426e-06,1.04911e-05,8.7426e-06,6.99408e-06,1.04911e-05,5.24556e-06,0,4.89586e-05,0.00020108,1.74852e-06,0,0,0,0,1.74852e-06,0,1.74852e-06,0,0,2.62278e-05	
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
