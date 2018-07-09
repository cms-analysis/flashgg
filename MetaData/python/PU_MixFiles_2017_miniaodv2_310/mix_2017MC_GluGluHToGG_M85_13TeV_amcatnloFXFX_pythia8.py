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
		7.58182e-05,8.42424e-06,0,5.05455e-05,6.7394e-05,0.000101091,0.000117939,0.000101091,0.000438061,0.000294849,0.000716061,0.00137315,0.00241776,0.00407733,0.00569479,0.00847479,0.0119035,0.0148856,0.0178931,0.0211196,0.0241439,0.0253823,0.0262499,0.0265785,0.027741,0.0287351,0.0304199,0.030302,0.0314393,0.0313887,0.0318099,0.0296028,0.0309338,0.0306474,0.0309759,0.0300324,0.0288615,0.0270587,0.0258372,0.0251548,0.0237732,0.0207658,0.0200329,0.0175224,0.0157112,0.0144813,0.0132008,0.0129144,0.0131671,0.0128554,0.0126953,0.0126111,0.0124679,0.0126111,0.012788,0.0125521,0.0121309,0.0122657,0.0116929,0.0112042,0.00943515,0.00831473,0.00720273,0.00536624,0.00440588,0.00348764,0.00307485,0.002502,0.00171855,0.00123836,0.000850849,0.000530727,0.000353818,0.000269576,0.000202182,0.000193758,0.000101091,7.58182e-05,3.3697e-05,4.21212e-05,3.3697e-05,0,0,8.42424e-06,0,0,0,0,8.42424e-06,0,0,0,0,0,0,0,0,0,0,0	
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
