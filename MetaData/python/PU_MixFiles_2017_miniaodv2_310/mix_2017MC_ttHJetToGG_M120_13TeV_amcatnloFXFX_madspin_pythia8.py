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
		0.0137993,0.000139617,0.00013637,0.000162345,0.000246764,0.000431838,0.000545479,0.000253258,0.000412356,0.000451319,0.000892898,0.00168839,0.00293845,0.00511712,0.00734774,0.0103284,0.0142084,0.0165397,0.0189619,0.0205074,0.0222965,0.0240693,0.0243583,0.0271279,0.0278811,0.0288422,0.0299949,0.0301832,0.0294169,0.0294267,0.0289461,0.0281214,0.0285922,0.0284299,0.0293487,0.027485,0.0267285,0.0254492,0.0245141,0.0239589,0.0216211,0.0208743,0.0225367,0.0166826,0.0150786,0.0138643,0.0122311,0.0123967,0.0118739,0.0116694,0.011507,0.0116369,0.0116823,0.0117765,0.0115525,0.0119973,0.0114973,0.0111758,0.0109485,0.0101238,0.0128155,0.00909457,0.00999721,0.00520478,0.003669,0.00346769,0.00303261,0.00229231,0.00153578,0.000970823,0.00454241,0.000516257,0.000327937,0.00139292,0.00521128,0.000152604,8.11725e-05,6.16911e-05,0.000353912,3.57159e-05,0.000873416,1.29876e-05,3.2469e-06,3.2469e-06,0,0,0,0,0.0013085,0,0,0,0,0,0,0,0,0,0,3.57159e-05	
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
