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
                0,1.1833e-06,1.1833e-06,5.42924e-06,1.70534e-05,3.52901e-05,7.3086e-05,0.000145197,0.000259142,0.000476381,0.000840002,0.00140235,0.00217497,0.00317451,0.00447168,0.00598435,0.00781714,0.00978816,0.0118445,0.0139877,0.0162187,0.0184727,0.0206128,0.0227432,0.024725,0.0267078,0.0285224,0.0302087,0.031804,0.0331733,0.0343964,0.035512,0.0361558,0.0368188,0.0371206,0.0369654,0.0366469,0.0359945,0.0352031,0.0340013,0.0326721,0.0311709,0.0294757,0.0275968,0.0256496,0.0236443,0.0215731,0.019495,0.0174551,0.0155396,0.0136965,0.0118728,0.0102075,0.00867872,0.00725493,0.00604804,0.00496776,0.00401277,0.00326534,0.00256253,0.00205747,0.0015852,0.00123696,0.00094977,0.000731208,0.000548771,0.000413875,0.000311903,0.000229977,0.000167123,0.000124455,8.89561e-05,6.1253e-05,4.29467e-05,3.30627e-05,2.53365e-05,1.65662e-05,9.25756e-06,7.58702e-06,5.70767e-06,3.6195e-06,2.36659e-06,1.46172e-06,7.65663e-07,9.04874e-07,7.65663e-07,3.48028e-07,6.96057e-08,0,2.78423e-07,2.78423e-07,6.96057e-08,6.96057e-08,0,6.96057e-08,6.96057e-08,0,0,0,1.39211e-07),
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
