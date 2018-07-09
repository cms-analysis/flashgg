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
		2.63034e-05,2.9226e-06,2.04582e-05,3.50712e-05,5.55294e-05,8.7678e-05,0.000122749,0.00014613,0.000379938,0.000409164,0.000721882,0.00133271,0.00233808,0.00393382,0.00599133,0.00875611,0.011629,0.0150163,0.0181318,0.0209317,0.0227115,0.0251431,0.0260784,0.0276069,0.0283463,0.0297345,0.0303424,0.030889,0.0310439,0.0307107,0.0307019,0.0311315,0.0306142,0.0315582,0.030962,0.0298193,0.0284515,0.0268762,0.0264086,0.0255757,0.0237461,0.0215103,0.0196545,0.0178103,0.0161006,0.0144376,0.0132102,0.0133738,0.0123801,0.0129121,0.0123217,0.0122749,0.0123305,0.0123217,0.012538,0.0126402,0.012424,0.0121989,0.0115589,0.0107289,0.00955106,0.00834403,0.00684181,0.0053016,0.00437806,0.00363864,0.00302489,0.00221241,0.00173018,0.00117489,0.000792025,0.000479307,0.000406242,0.000251344,0.000187046,0.000149053,0.000116904,7.59876e-05,3.50712e-05,2.04582e-05,1.4613e-05,5.8452e-06,0,0,8.7678e-06,8.7678e-06,2.9226e-06,0,0,0,0,0,0,0,0,0,0,0,0,0	
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
