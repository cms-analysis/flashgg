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
		0.0199454,0.000499767,0.000874228,0.00125383,0.00143597,0.00120238,0.00111839,0.000799209,0.00183788,0.00110534,0.00189078,0.00293119,0.0043259,0.0061355,0.00870814,0.0114102,0.0138984,0.0167601,0.0193154,0.0217072,0.023624,0.0246299,0.0258793,0.0263622,0.0275296,0.0279682,0.0284806,0.0290465,0.0289994,0.029014,0.0287405,0.0287822,0.0289066,0.0287252,0.0283529,0.0274368,0.0265233,0.0250985,0.0244505,0.0234255,0.0219865,0.0197738,0.0181404,0.0161971,0.0148457,0.0134801,0.0124413,0.012116,0.0118319,0.0117588,0.0116996,0.0116299,0.0118758,0.0120651,0.0116661,0.012054,0.0116676,0.0114893,0.0110233,0.0103236,0.00925533,0.00784584,0.00634678,0.0052653,0.0040138,0.00350962,0.00298774,0.00232208,0.00162916,0.00110113,0.000922915,0.00071105,0.000544525,0.000489971,0.000364181,0.000459518,0.000218605,0.000413207,0.000200421,0.000357198,0.000556115,0.000479594,0.000249835,0.000351282,0.00038285,0.000168512,0.000231408,0.000344832,0.000365684,0.000151685,0.000163906,8.24377e-07,4.41769e-05,8.86933e-05,1.30931e-06,0.000195232,2.57012e-06,1.11533e-06,3.59816e-05,2.90957e-05	
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
