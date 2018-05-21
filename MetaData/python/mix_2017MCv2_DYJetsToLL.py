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
                0.0238775, 0.000874622, 0.00124831, 0.00153105, 0.00147725, 0.00141303, 0.00126538, 0.00115647, 0.00218504, 0.00171699, 0.00229043, 0.00333221, 0.0048993, 0.00663561, 0.00897475, 0.011538, 0.0141801, 0.0169121, 0.0192551, 0.0214591, 0.0231318, 0.0243014, 0.0251415, 0.0257144, 0.0264036, 0.0272533, 0.0279565, 0.028333, 0.0282152, 0.0281867, 0.0280371, 0.0282786, 0.0282462, 0.0279432, 0.0276964, 0.0270571, 0.0259487, 0.0247456, 0.0239501, 0.0231562, 0.0214979, 0.0197288, 0.0179504, 0.0162761, 0.0148308, 0.0136544, 0.0125744, 0.0122772, 0.0120571, 0.0118004, 0.0120413, 0.0120674, 0.0120442, 0.0120637, 0.0119306, 0.0121372, 0.0119726, 0.0117524, 0.0113325, 0.0105063, 0.00941031, 0.00811139, 0.00662999, 0.00554745, 0.00434088, 0.00362642, 0.00318208, 0.00228125, 0.00171769, 0.00117662, 0.000909775, 0.000725548, 0.000478647, 0.000399337, 0.000367371, 0.000333013, 0.000231815, 0.000234494, 0.000226693, 0.000381801, 0.000485566, 0.000441125, 0.000234348, 0.000323903, 0.000166324, 0.000254228, 0.000143739, 0.000292395, 0.000254309, 0.000174261, 8.52316e-05, 5.89512e-05, 1.01815e-05, 3.82778e-05, 2.63483e-05, 0.000187418, 1.01005e-05, 2.22584e-05, 3.01709e-05, 3.36291e-05),
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
