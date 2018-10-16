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
		0.0251025,0.000480868,0.000965631,0.000311494,0.000465294,0.000379633,0.00377881,0.000584051,0.00183392,0.00136279,0.00264964,0.00296503,0.00405526,0.0065511,0.00835971,0.0114026,0.0145292,0.0181776,0.020031,0.0209441,0.0224937,0.0237611,0.0247579,0.0256087,0.0255444,0.0273803,0.0289981,0.0292103,0.0281201,0.0278436,0.0277736,0.0275867,0.0288054,0.027501,0.0270299,0.0269442,0.0254023,0.0240298,0.0237631,0.0225716,0.0210862,0.0222445,0.0175994,0.0161938,0.0142528,0.013219,0.0121969,0.0117453,0.0113131,0.0154287,0.0134565,0.0115136,0.0120626,0.0116849,0.0119516,0.0118231,0.0117725,0.0116343,0.0109704,0.0103688,0.0115525,0.0079976,0.0062143,0.00528566,0.00445631,0.00350041,0.00307211,0.00223886,0.00153995,0.00108439,0.000817671,0.000443879,0.000340696,0.000219992,0.000219992,0.000194684,0.000105129,5.45114e-05,4.08836e-05,0.00154384,1.36279e-05,1.75215e-05,0.00250558,0.00327652,7.78734e-06,0,0,0,0,0,0,0,0,1.94684e-06,0.000634668,1.1681e-05,5.84051e-06,0,0,2.72557e-05	
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
