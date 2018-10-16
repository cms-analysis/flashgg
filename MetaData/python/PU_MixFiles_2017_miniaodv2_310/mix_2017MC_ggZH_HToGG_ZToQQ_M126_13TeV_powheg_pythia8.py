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
		0.0230023,0.000237103,0.00218736,0.00648527,0.00195359,0.00138254,0.000524298,0.000387379,0.00103858,0.0062515,0.00499252,0.00393391,0.00614464,0.00628824,0.01447,0.011047,0.0149174,0.019222,0.0192688,0.0214662,0.0231593,0.0232762,0.0240275,0.0248056,0.0252465,0.0263251,0.0274171,0.0282353,0.0284457,0.0282587,0.0276442,0.0283655,0.0287462,0.0281284,0.0277344,0.0276642,0.0260012,0.024435,0.0239274,0.0228988,0.0210087,0.0196495,0.0177694,0.0159193,0.0144165,0.012867,0.0117817,0.0110503,0.0113709,0.0111873,0.0115513,0.0117049,0.0138689,0.0113642,0.0132043,0.0111973,0.011401,0.0111472,0.0107498,0.00976797,0.00865259,0.00776095,0.00623481,0.00471534,0.00388047,0.00321592,0.00279514,0.00210721,0.00148607,0.00111539,0.000677914,0.000464187,0.000307232,0.000207048,0.000173653,0.000103524,9.35054e-05,3.67343e-05,0.000935054,0.00192354,1.00184e-05,1.33579e-05,1.00184e-05,1.00184e-05,3.33948e-06,1.33579e-05,3.33948e-06,3.33948e-06,0,0,0,0,0,0,0,0,0,9.35054e-05,0,3.33948e-05	
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
