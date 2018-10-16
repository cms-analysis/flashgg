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
		0.0114116,0.000145876,0.0023488,0.00081063,0.00120025,0.00175421,0.000710917,0.000836482,0.00212537,0.00214568,0.00288799,0.00367276,0.0051223,0.00820232,0.00984574,0.0121724,0.0133763,0.0159836,0.0187165,0.0251462,0.0243374,0.0244962,0.0254121,0.0265458,0.0270425,0.0274488,0.0281043,0.0285641,0.0303534,0.0287654,0.0281006,0.0310292,0.0275097,0.0280434,0.027879,0.0267083,0.0255192,0.0246827,0.0246051,0.0259309,0.0210284,0.0194939,0.0194182,0.0155035,0.0142276,0.013092,0.0126192,0.0118049,0.0126654,0.0109998,0.0138361,0.0116794,0.0115833,0.011611,0.0117089,0.0116369,0.0134299,0.0114818,0.010926,0.00993253,0.00930655,0.00760404,0.00789949,0.00503182,0.00376509,0.00336993,0.00299878,0.00209767,0.00170251,0.00105437,0.000747848,0.000524417,0.000297293,0.000201273,0.000184654,0.00182807,8.67873e-05,8.12477e-05,3.32377e-05,3.13911e-05,1.84654e-05,1.29258e-05,1.10792e-05,0.00266086,7.38615e-06,0,1.84654e-06,1.47723e-05,2.21585e-05,0,1.84654e-06,0,0,3.69308e-06,1.84654e-06,0,1.84654e-06,0,0,2.21585e-05	
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
