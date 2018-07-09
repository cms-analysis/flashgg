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
		2.59971e-05,4.99945e-06,8.999e-06,3.99956e-05,5.29941e-05,9.59893e-05,0.000105988,0.000118987,0.000354961,0.000346961,0.000662926,0.00134685,0.00232874,0.00375558,0.00596434,0.00846206,0.0118047,0.0147984,0.017858,0.0207857,0.0230914,0.0250502,0.0261571,0.027165,0.0286448,0.0295537,0.0305466,0.0312535,0.0311075,0.0310356,0.0312285,0.0312455,0.0308616,0.0308786,0.0306156,0.0300037,0.0287268,0.027118,0.026733,0.0254612,0.0235514,0.0216266,0.0196208,0.017834,0.0162432,0.0145094,0.0132765,0.0129786,0.0126456,0.0122976,0.0122796,0.0124086,0.0122666,0.0123676,0.0123096,0.0123476,0.0124526,0.0121676,0.0116427,0.0109338,0.00961593,0.00833407,0.00690423,0.00535041,0.00418054,0.00368659,0.00309266,0.00232874,0.00161682,0.00112687,0.000776914,0.000518942,0.000374958,0.00026897,0.000193978,0.000160982,0.000100989,7.39918e-05,4.59949e-05,3.49961e-05,1.99978e-05,8.999e-06,2.99967e-06,4.99945e-06,6.99922e-06,9.99889e-07,1.99978e-06,2.99967e-06,9.99889e-07,0,0,0,0,0,0,0,0,0,0,0	
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
