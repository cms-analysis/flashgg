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
		0.0204467,0.000577243,0.00103128,0.00139815,0.00134876,0.00125122,0.00133657,0.00104441,0.00196555,0.00163345,0.00217042,0.0030126,0.00466158,0.00643152,0.00880965,0.0113798,0.0141554,0.0169777,0.0193937,0.02194,0.0236166,0.0248169,0.0255024,0.0261524,0.0268777,0.0277338,0.0284475,0.0288513,0.0287229,0.0286987,0.0286568,0.0287423,0.0287958,0.0285109,0.0282406,0.0275528,0.0263979,0.0250713,0.0242977,0.0234002,0.0217745,0.0197212,0.0179421,0.0163009,0.0147177,0.0134513,0.0123832,0.0121696,0.0116545,0.0115603,0.01189,0.011813,0.0118609,0.0118978,0.0117923,0.0119397,0.0117311,0.0115632,0.0110385,0.0102986,0.00910914,0.00783519,0.00653477,0.00543697,0.00424247,0.00359169,0.00314555,0.00233095,0.00162811,0.00119652,0.000929393,0.000714677,0.00050125,0.00037868,0.000403841,0.0002939,0.00025011,0.000209564,0.000237989,0.000337866,0.000509705,0.000303807,0.000164367,0.000483892,0.000241148,0.000259386,0.000152076,0.000222291,0.0002755,0.00012158,6.40143e-05,3.31405e-05,1.7417e-05,2.31358e-05,7.61684e-05,8.47336e-05,2.30772e-05,1.49484e-05,3.40068e-05,3.42869e-05	
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
