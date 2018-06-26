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
		4.52527e-05,6.46467e-06,9.697e-06,4.52527e-05,4.8485e-05,7.7576e-05,0.000116364,0.00015192,0.000452527,0.000320001,0.00073374,0.0012897,0.00233374,0.00386587,0.00597335,0.00896326,0.0122085,0.0148655,0.0178296,0.0210845,0.0241972,0.0249181,0.0254449,0.0274134,0.0284413,0.0293658,0.0300898,0.0311435,0.0305132,0.0313213,0.0310466,0.0309431,0.0306684,0.0310078,0.0304583,0.0299864,0.02891,0.0274393,0.0260623,0.0257552,0.0237867,0.021453,0.0190837,0.0180267,0.0161778,0.014413,0.0136663,0.0129972,0.0125221,0.0124542,0.0126223,0.0125641,0.0123572,0.0124348,0.0125188,0.0124348,0.0121924,0.0125415,0.0113002,0.0108606,0.00949336,0.00825215,0.00656487,0.00531719,0.00416971,0.00355557,0.0031289,0.00229172,0.0016097,0.00106344,0.000778992,0.000517173,0.000355557,0.000242425,0.000245657,0.000145455,0.000119596,5.17173e-05,4.20203e-05,3.8788e-05,2.26263e-05,9.697e-06,9.697e-06,9.697e-06,3.23233e-06,0,0,6.46467e-06,0,0,0,0,0,0,0,0,0,0,0,0	
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
