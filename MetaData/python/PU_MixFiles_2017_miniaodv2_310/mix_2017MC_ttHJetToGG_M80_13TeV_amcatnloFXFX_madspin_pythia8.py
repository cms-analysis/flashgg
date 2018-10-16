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
		0.0381496,0.000122964,0.000190594,0.000645562,0.00236911,0.00238141,0.00277489,0.00163337,0.00234862,0.0015514,0.00191824,0.00304336,0.00367048,0.00617075,0.00777339,0.0100974,0.0127063,0.0160796,0.0190717,0.0212298,0.0224246,0.0235825,0.0285994,0.0253798,0.0263615,0.0288433,0.0292081,0.0299213,0.0290708,0.028417,0.0284457,0.0280891,0.0286404,0.0286384,0.0276444,0.0270275,0.0253942,0.0243264,0.0237013,0.0230045,0.0209449,0.0191394,0.019166,0.0160325,0.0143479,0.0128539,0.0118148,0.0112881,0.0109889,0.0110032,0.0125997,0.0115135,0.0118824,0.0119788,0.0112656,0.0113168,0.0110791,0.0109623,0.0108721,0.00978385,0.00869357,0.00744958,0.00631626,0.00502514,0.00386517,0.00338561,0.00293884,0.0022318,0.0015391,0.00105749,0.000707044,0.000454967,0.000401683,0.000215187,0.000215187,0.00017215,6.76303e-05,5.12351e-05,3.48398e-05,2.66422e-05,1.22964e-05,8.19761e-06,8.19761e-06,4.09881e-06,2.0494e-06,2.0494e-06,4.09881e-06,0.00353727,6.35315e-05,0,2.0494e-06,0,0,0,0,0,2.0494e-06,2.0494e-06,2.0494e-06,1.84446e-05	
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
