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
		0.0259592,0.000985405,0.000201155,0.00433884,0.00152267,0.00363097,0.00241895,0.00260228,0.00365644,0.00138262,0.00295876,0.00407403,0.00521221,0.00550757,0.00617469,0.0086242,0.015438,0.015275,0.018985,0.0220583,0.0240775,0.0244187,0.0240622,0.0278383,0.0265932,0.0278511,0.0299797,0.0287601,0.02903,0.0287372,0.0284723,0.0292184,0.0307894,0.0282304,0.0278052,0.0269522,0.0266518,0.024709,0.0234511,0.0228349,0.0240546,0.0194662,0.0177424,0.0156621,0.0141776,0.0129325,0.0118223,0.0115728,0.0113691,0.0114149,0.0114022,0.0116262,0.0113894,0.0115524,0.0115015,0.0114276,0.0116619,0.0113182,0.0108445,0.00996608,0.00871587,0.00745547,0.00619252,0.00575456,0.00377357,0.00337126,0.00298422,0.00213377,0.00159396,0.000947211,0.000733325,0.000488883,0.00048379,0.000178239,0.000196062,0.000183331,9.93044e-05,4.07403e-05,4.58328e-05,2.54627e-05,2.80089e-05,7.6388e-06,9.16656e-05,0,2.54627e-06,2.54627e-06,0,2.54627e-06,0,0,0,0,0,7.6388e-06,0,0,0,5.85641e-05,0,2.80089e-05	
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
