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
		3.43907e-05,9.55298e-06,1.14636e-05,2.48377e-05,5.54073e-05,7.83344e-05,0.000120368,0.000129921,0.000343907,0.000324801,0.000693546,0.00137181,0.00249906,0.00376578,0.006005,0.00855183,0.0115476,0.014853,0.0178163,0.0211159,0.023378,0.0250078,0.0258026,0.0269948,0.028187,0.02932,0.0300441,0.031072,0.030623,0.030837,0.0312306,0.0309306,0.0309345,0.0310128,0.0307377,0.0302103,0.0287697,0.027354,0.0265477,0.0252944,0.0237105,0.0218018,0.0196123,0.0176787,0.0159936,0.0145721,0.0132729,0.0131258,0.0124838,0.01278,0.0124934,0.0121609,0.0121705,0.0125201,0.0126348,0.0124628,0.012524,0.0124934,0.0117291,0.010749,0.00976314,0.00839134,0.00689916,0.00547195,0.00418803,0.00376769,0.00320025,0.00234621,0.00165649,0.00109477,0.000754685,0.000584642,0.000341997,0.000271305,0.000189149,0.000171954,0.000116546,5.54073e-05,4.39437e-05,2.67483e-05,1.33742e-05,2.10166e-05,3.82119e-06,7.64238e-06,5.73179e-06,0,0,0,1.9106e-06,0,0,0,0,0,0,0,0,0,0,0	
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
