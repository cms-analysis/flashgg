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
		0.0339805,0.000140002,0.000233336,0.00121002,0.00146335,0.00213336,0.00244003,0.0025467,0.0025667,0.0024667,0.00309671,0.00228003,0.00500007,0.00696343,0.0100968,0.0119702,0.0161769,0.0176902,0.0192903,0.0221703,0.023397,0.0288337,0.024627,0.024827,0.0257903,0.0265104,0.026947,0.027317,0.026667,0.0266637,0.028157,0.026197,0.026047,0.0269104,0.0258203,0.0259837,0.024417,0.023147,0.0232103,0.0259303,0.0203269,0.0183702,0.0165602,0.0156102,0.0138302,0.0127168,0.0118035,0.0110801,0.0108935,0.0106901,0.0109268,0.0112735,0.0156435,0.0114635,0.0109735,0.0110001,0.0109535,0.0135902,0.0103768,0.00955679,0.00845678,0.0072801,0.00610008,0.0048234,0.00371338,0.00336004,0.00272337,0.00201669,0.00138335,0.00102001,0.000736676,0.000510007,0.000320004,0.000163336,0.000173336,0.00137335,0.00208003,0.00148002,0.000950013,0.000633342,0.00554674,0.000393339,0.000196669,0.00530674,8.00011e-05,6.66676e-05,3.00004e-05,2.6667e-05,1.33335e-05,3.33338e-06,6.66676e-06,0,6.66676e-06,3.33338e-06,0,0,3.33338e-06,0,0,6.33342e-05	
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
