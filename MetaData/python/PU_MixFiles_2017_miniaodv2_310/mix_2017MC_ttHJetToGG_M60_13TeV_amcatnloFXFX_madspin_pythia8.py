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
		0.0415143,0.000499868,0.000878981,0.0045297,0.000261167,0.000421237,0.0002724,0.000328565,0.000775076,0.000749802,0.00157262,0.00329127,0.00554348,0.00823378,0.00886564,0.0137492,0.013634,0.0159789,0.0194387,0.0252012,0.0234067,0.0246452,0.0249766,0.0249429,0.0257011,0.0261139,0.0266475,0.0275573,0.0267907,0.0272175,0.0273214,0.0315507,0.0272203,0.0272316,0.0276191,0.0263357,0.0251731,0.0240948,0.0236791,0.0227608,0.0205901,0.0190905,0.0173044,0.0153667,0.0140047,0.0129544,0.0114324,0.0113144,0.0110252,0.0110364,0.0109831,0.0112442,0.0110168,0.011087,0.0150775,0.0116767,0.0115026,0.0124068,0.0105759,0.0101883,0.00894427,0.00730144,0.00643089,0.00911838,0.00386134,0.00336428,0.00288126,0.00209776,0.00159789,0.000988503,0.000724528,0.000466169,0.000370689,0.000205002,0.000193769,0.000154454,9.54804e-05,7.30144e-05,3.65072e-05,3.3699e-05,1.96577e-05,8.42474e-06,2.80825e-06,0.00435278,0,5.61649e-06,2.80825e-06,2.80825e-06,2.80825e-06,0,0,0,0,2.80825e-06,0,0,2.80825e-06,2.80825e-06,0,5.05485e-05	
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
