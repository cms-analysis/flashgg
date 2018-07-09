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
		0.0313375,0.000520014,0.0011567,0.00133004,0.00074002,0.000970026,0.00140337,0.00138004,0.00180671,0.000600016,0.00108336,0.00199005,0.00355009,0.00583016,0.00826022,0.0114736,0.0185538,0.0184105,0.0196605,0.0217172,0.0227239,0.0229039,0.0230873,0.0238773,0.023914,0.0265974,0.026584,0.026824,0.0289774,0.0319342,0.0275874,0.0282274,0.0280441,0.0280541,0.0271274,0.0269574,0.026254,0.024384,0.0231173,0.0224973,0.026284,0.0188405,0.0178305,0.0159738,0.013997,0.0131037,0.0118903,0.011847,0.0112003,0.010937,0.0115836,0.0112936,0.0114303,0.012317,0.011387,0.011877,0.0113736,0.0110136,0.0118036,0.00974693,0.00895357,0.00722686,0.0112136,0.00500013,0.00407678,0.00351343,0.00285674,0.00593682,0.0014367,0.00103669,0.000746687,0.00566682,0.000346676,0.000200005,0.000203339,0.000110003,0.00156004,5.00013e-05,4.33345e-05,1.66671e-05,2.00005e-05,2.00005e-05,1.00003e-05,6.66684e-06,1.00003e-05,0.000600016,3.33342e-06,0,3.33342e-06,0,0,6.66684e-06,0,0,0,0,0,0,0.00191005,3.66676e-05	
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
