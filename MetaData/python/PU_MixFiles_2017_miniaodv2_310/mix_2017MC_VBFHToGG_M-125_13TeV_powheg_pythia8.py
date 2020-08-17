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
		0.022914,0.000776747,0.00127952,0.00188065,0.00183248,0.00343113,0.00219576,0.00229813,0.00313107,0.00272965,0.00281696,0.00408244,0.00523551,0.00744533,0.00908914,0.0113893,0.0126066,0.0154717,0.0185295,0.0215111,0.0235051,0.0243411,0.024891,0.0252051,0.0263221,0.0283944,0.0281084,0.0284476,0.0283793,0.0280943,0.0279599,0.0290005,0.0280672,0.0280201,0.027714,0.0284827,0.0260381,0.0254811,0.0248258,0.023471,0.0221804,0.019516,0.0178772,0.0158932,0.0145996,0.0131435,0.0120657,0.0116703,0.0112568,0.0111314,0.0114906,0.0115067,0.0114756,0.0127511,0.0114736,0.0114645,0.0116582,0.0113792,0.0107841,0.0102011,0.00869776,0.00775542,0.00789893,0.00507796,0.00386768,0.0034231,0.00291029,0.00217168,0.00146418,0.00109788,0.000776747,0.000493746,0.000319128,0.00105573,0.000183649,0.000145515,0.00123738,5.01774e-05,0.000336189,2.40852e-05,0.00156955,1.00355e-05,7.02484e-06,7.02484e-06,7.02484e-06,0.00035626,6.52307e-05,2.91029e-05,2.0071e-06,1.00355e-06,0,2.0071e-06,1.00355e-06,0,0,0,2.0071e-06,1.00355e-06,2.0071e-06,3.61277e-05	
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
