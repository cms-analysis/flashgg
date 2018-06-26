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
		0.000989514,0.000192499,0.000330964,0.00105368,0.0013171,0.000381621,0.0005032,0.000246534,0.00114486,0.000412016,0.000905084,0.00168184,0.00287736,0.00489353,0.00722379,0.0110332,0.0140423,0.0182368,0.0224447,0.0231202,0.0250013,0.0258793,0.0260921,0.0261259,0.0273383,0.0279158,0.0339846,0.0290302,0.0291214,0.028679,0.0305804,0.0292194,0.029395,0.0289796,0.0289559,0.0285777,0.0275443,0.0254707,0.0246838,0.0247885,0.0216207,0.0197227,0.0187366,0.01643,0.0153797,0.0160382,0.0125699,0.0129582,0.0116107,0.0115162,0.0118471,0.0119113,0.0117729,0.0116783,0.0118438,0.0115668,0.0117323,0.0118539,0.0111447,0.0100133,0.00919606,0.0077844,0.00619375,0.00521774,0.00870975,0.00363385,0.00316104,0.00212425,0.00164469,0.00116175,0.000786883,0.000553858,0.000320832,0.000226271,0.000175613,0.00013171,0.0056095,7.09208e-05,5.74121e-05,5.40349e-05,6.75436e-06,3.37718e-06,0,6.75436e-06,6.75436e-06,0,0,3.37718e-06,0.000783506,3.37718e-06,0,0,0,3.37718e-06,0,0,0,0,0,2.70174e-05	
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
