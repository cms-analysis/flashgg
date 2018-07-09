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
		0.0359157,0.000453235,0.00134916,0.00131069,0.00122373,0.00132492,0.00109936,0.00113572,0.00157262,0.00164798,0.00211018,0.00271572,0.00583935,0.00599377,0.00833478,0.0108213,0.0142369,0.0186132,0.019907,0.0219998,0.0229295,0.0239582,0.0256057,0.0252231,0.0254465,0.0269359,0.0279045,0.0283809,0.0285691,0.0277875,0.0285254,0.0279846,0.0284089,0.0276384,0.0282835,0.0276563,0.0255308,0.0245279,0.0236336,0.0229358,0.0215181,0.0193421,0.0177357,0.01581,0.0144334,0.0131976,0.0118474,0.0118879,0.0127338,0.0110621,0.0123781,0.0112766,0.0115596,0.0114721,0.0117562,0.01149,0.0112318,0.0111327,0.0107996,0.010133,0.00869632,0.0076133,0.00703674,0.00504883,0.00458189,0.00336343,0.00370177,0.00209858,0.00147249,0.00102505,0.00105931,0.00120792,0.000323589,0.000577084,0.000176024,0.00012543,8.9066e-05,6.37691e-05,8.37958e-05,0.000840593,0.000755743,0.00109409,2.10807e-05,4.74316e-06,3.68912e-06,2.47698e-05,1.58105e-06,1.58105e-06,0,1.05404e-06,0.000218712,5.27018e-07,1.05404e-06,5.27018e-07,0,5.27018e-07,0.000387358,1.05404e-06,5.27018e-07,2.74049e-05	
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
