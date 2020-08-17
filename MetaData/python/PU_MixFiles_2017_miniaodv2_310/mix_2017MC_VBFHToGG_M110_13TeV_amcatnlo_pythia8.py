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
		0.0225573,2.66674e-05,4.66679e-05,0.00011667,0.000180005,0.00037001,0.000403344,0.000456679,0.00117336,0.00121003,0.0024034,0.0036601,0.00528014,0.00660684,0.00937692,0.0114736,0.013867,0.0169438,0.0203239,0.0221673,0.0245273,0.0256274,0.0260874,0.024834,0.025654,0.0270874,0.0278307,0.0282574,0.0285341,0.0283974,0.0337276,0.0276707,0.0276641,0.0279407,0.0278041,0.026324,0.0259107,0.024614,0.0236773,0.0224606,0.0210339,0.0217072,0.0177205,0.0158671,0.0144537,0.012947,0.0134704,0.0116636,0.0112503,0.0159004,0.0111703,0.011307,0.0114303,0.0115636,0.011257,0.011447,0.010997,0.011227,0.0105136,0.0100136,0.00902691,0.0075802,0.00609016,0.00655351,0.00395677,0.00348009,0.00290008,0.00193672,0.00144337,0.00100669,0.000716686,0.000483346,0.000303341,0.000220006,0.000210006,0.000123337,0.000103336,6.00016e-05,4.00011e-05,2.3334e-05,0.000176671,0.00298341,1.00003e-05,1.33337e-05,0,0.00517014,0,3.33342e-06,0,0.00515014,0,0,0,0,0,0,0,0,3.33342e-06,1.66671e-05	
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
