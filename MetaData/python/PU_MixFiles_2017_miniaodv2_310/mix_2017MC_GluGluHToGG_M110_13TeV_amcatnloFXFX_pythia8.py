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
		0.0213483,0.000190431,0.000608043,0.00921419,0.00285647,0.0027529,0.0030636,0.00113256,0.00141988,0.00146331,0.00663168,0.00511157,0.00601027,0.00619068,0.00914069,0.0125684,0.0154416,0.0187257,0.0219363,0.0213249,0.0228918,0.0238674,0.024462,0.0253407,0.0254209,0.028274,0.027823,0.028284,0.0270512,0.0269042,0.0278731,0.0272751,0.0276359,0.027245,0.0288119,0.0266436,0.0251503,0.0242415,0.0228049,0.0224408,0.0199318,0.0180843,0.0171121,0.015622,0.0160764,0.0140351,0.0119838,0.0112555,0.0149605,0.0122711,0.0111319,0.011025,0.0110417,0.0111352,0.0105706,0.0111686,0.0109047,0.0109414,0.0106007,0.00959505,0.00863955,0.00754708,0.00588332,0.00497125,0.00385205,0.00327074,0.00283976,0.00205131,0.00148336,0.00106909,0.00492448,0.000497793,0.000334089,0.000203795,0.004273,0.000103568,9.35451e-05,6.3477e-05,3.00681e-05,3.34089e-05,1.00227e-05,6.68179e-06,0.000731656,0,3.34089e-06,3.34089e-06,0,0,0,0,0,0,0,0,0,0,3.34089e-06,0,0,5.67952e-05	
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
