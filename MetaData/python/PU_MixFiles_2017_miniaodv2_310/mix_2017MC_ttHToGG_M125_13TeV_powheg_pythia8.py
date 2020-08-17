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
		0.0288602,9.19382e-05,0.0009058,0.00185548,0.000642522,0.00102177,0.000888039,0.000990425,0.00185235,0.00153892,0.00229532,0.00455407,0.00353649,0.00539614,0.00770505,0.0107212,0.0136654,0.0163232,0.0191096,0.0229501,0.0242821,0.0237253,0.0259464,0.0266349,0.0254303,0.0275292,0.0282564,0.0280453,0.0274018,0.0284381,0.0299823,0.0299833,0.0298298,0.0272701,0.0273464,0.0266485,0.0265221,0.0239896,0.0249529,0.0224946,0.020755,0.0196988,0.0175383,0.0157872,0.0142212,0.0129633,0.011673,0.0157162,0.0126269,0.011182,0.0112959,0.0122549,0.0110441,0.011088,0.0124231,0.0112468,0.0111078,0.0112018,0.0104956,0.010903,0.00889084,0.008241,0.00627269,0.00500018,0.00369947,0.00524988,0.00303291,0.00364827,0.00168205,0.00122236,0.00111997,0.000563121,0.000342679,0.00025283,0.000228801,0.000469094,0.000108654,6.47746e-05,7.94011e-05,2.61188e-05,2.29845e-05,0.00163399,8.35802e-06,4.17901e-06,0.00161937,0.0016225,1.04475e-06,5.22376e-06,3.13426e-06,0,5.22376e-06,1.04475e-06,0,1.04475e-06,0,0,5.22376e-06,2.0895e-06,1.04475e-06,3.23873e-05	
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
