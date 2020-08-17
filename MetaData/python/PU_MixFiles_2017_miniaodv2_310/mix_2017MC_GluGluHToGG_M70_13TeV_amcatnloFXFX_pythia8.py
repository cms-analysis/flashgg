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
		0.0407763,0.000242777,0.000428231,0.00108575,0.000644032,0.00100145,0.00111273,0.00130155,0.00339213,0.00159491,0.00241765,0.00453183,0.00350677,0.00430928,0.00669321,0.00929969,0.0151904,0.0156456,0.0190242,0.020727,0.0249385,0.0233504,0.0239978,0.0255657,0.0254207,0.0268808,0.0272517,0.0277237,0.0270831,0.0277271,0.0273022,0.0281891,0.0273191,0.0281115,0.0274742,0.0265166,0.0252285,0.0239641,0.0234246,0.022848,0.0209766,0.0192164,0.0174631,0.0157838,0.0143002,0.0130526,0.0119905,0.0156456,0.0110396,0.0113195,0.0238224,0.0122063,0.0116229,0.0116836,0.0116533,0.0113127,0.0117241,0.011397,0.010962,0.0100415,0.00878379,0.00749573,0.00645718,0.00531074,0.00395186,0.00345957,0.00299087,0.00219848,0.00136225,0.00110261,0.000799139,0.00434975,0.000313586,0.000256264,0.000171967,0.000155107,0.000131504,6.74379e-05,3.03471e-05,2.69752e-05,2.02314e-05,1.34876e-05,1.01157e-05,1.01157e-05,6.74379e-06,3.3719e-06,0,0,0,0,0,3.03471e-05,0,0,0,0,6.74379e-06,0,0,2.69752e-05	
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
