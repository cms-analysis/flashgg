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
		0.0214592,0.000528753,0.000948858,0.0024144,0.00173837,0.00354434,0.0016273,0.00139794,0.00439903,0.00193152,0.00370851,0.00484087,0.00636918,0.00746773,0.00838762,0.0103336,0.0145854,0.016174,0.0183929,0.0207735,0.0224708,0.0237963,0.0241319,0.0258147,0.0261528,0.0274227,0.0284464,0.0300423,0.0279877,0.027797,0.0283112,0.0305518,0.0282267,0.0282992,0.0282774,0.0270944,0.0260924,0.0253874,0.0243057,0.0227774,0.0212612,0.0197136,0.0175261,0.0159036,0.0145564,0.0133299,0.0120092,0.0117895,0.0146168,0.0113694,0.0120382,0.0116398,0.0116398,0.0116519,0.0115795,0.01107,0.0114877,0.0116205,0.0109396,0.0102153,0.00914574,0.00771641,0.0100825,0.00500505,0.00391132,0.00356607,0.00305904,0.00216571,0.00160799,0.000987489,0.000712247,0.000533582,0.000313872,0.000200395,0.000188323,0.000149693,7.96751e-05,6.03599e-05,4.10448e-05,2.17296e-05,2.17296e-05,2.4144e-06,2.4144e-06,4.8288e-06,1.2072e-05,2.4144e-06,4.8288e-06,0,2.4144e-06,0,2.4144e-06,0,0,0,0,0,9.65759e-06,0,0,1.93152e-05	
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
