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
		3.43578e-05,4.90825e-06,3.43578e-05,1.9633e-05,5.39908e-05,0.000100619,0.000134977,0.000147248,0.000358302,0.000365665,0.000709242,0.0013964,0.00228479,0.00384807,0.00590954,0.00857963,0.0116203,0.0147616,0.0180722,0.0210908,0.0237903,0.0250149,0.0261733,0.0275893,0.0279844,0.0289341,0.0301538,0.0310594,0.0303944,0.0308827,0.0305956,0.030922,0.0310815,0.0311036,0.0309269,0.0299649,0.028539,0.0273954,0.0266665,0.0253168,0.0232774,0.0210809,0.019633,0.0178709,0.0160672,0.0145775,0.0135026,0.0128056,0.0126584,0.0125897,0.0123565,0.0126608,0.0126927,0.0125308,0.0125283,0.0126191,0.0124228,0.0122657,0.0121406,0.0108546,0.00976251,0.00830722,0.00704089,0.00547761,0.00400513,0.0035364,0.00316337,0.00223816,0.00162709,0.00106018,0.000807408,0.00057672,0.000331307,0.000284679,0.000208601,0.000132523,9.8165e-05,7.8532e-05,4.41743e-05,3.43578e-05,9.8165e-06,1.47248e-05,0,2.45413e-06,0,2.45413e-06,0,2.45413e-06,2.45413e-06,0,0,0,0,0,0,0,0,0,0,0	
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
