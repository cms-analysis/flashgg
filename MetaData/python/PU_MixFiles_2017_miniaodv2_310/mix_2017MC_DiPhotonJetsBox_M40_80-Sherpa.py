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
                3.06504e-05, 9.87048e-06, 1.63642e-05, 3.55857e-05, 5.68851e-05, 8.00028e-05, 0.000118705, 0.000135849, 0.000354038, 0.000348584, 0.000714051, 0.00135537, 0.00238034, 0.00391468, 0.00590696, 0.00858108, 0.0116986, 0.0149153, 0.0178762, 0.020962, 0.0232569, 0.025025, 0.0262269, 0.0272269, 0.0283527, 0.0294553, 0.0301626, 0.0309042, 0.0309502, 0.0309588, 0.0310658, 0.0310972, 0.0309432, 0.031106, 0.0306429, 0.0299366, 0.0288397, 0.0273363, 0.0265669, 0.0256051, 0.0234806, 0.0214849, 0.0196274, 0.0177297, 0.0161582, 0.0145626, 0.0134324, 0.0128908, 0.0124381, 0.0124295, 0.0122973, 0.0125456, 0.0124472, 0.0124358, 0.0123984, 0.0124701, 0.0122978, 0.0122355, 0.0117448, 0.0108082, 0.00965203, 0.00838497, 0.00686778, 0.00541084, 0.00416041, 0.0036874, 0.0031718, 0.00227566, 0.00164785, 0.00112212, 0.000779768, 0.000527032, 0.000357935, 0.000236632, 0.000193254, 0.000149096, 0.000112212, 7.01324e-05, 4.00014e-05, 2.64944e-05, 2.10397e-05, 1.09095e-05, 7.27298e-06, 7.53273e-06, 5.97424e-06, 2.59749e-06, 1.5585e-06, 1.29875e-06, 1.039e-06, 5.19499e-07, 7.79248e-07, 0, 2.59749e-07, 0, 0, 2.59749e-07, 0, 0, 0, 0
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
