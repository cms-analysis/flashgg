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
		3.92095e-05,2.4506e-06,1.2253e-05,3.67589e-05,5.14625e-05,9.06721e-05,0.000142135,0.000132332,0.000414151,0.000355336,0.000666562,0.00139684,0.00239423,0.00363423,0.00592554,0.00854033,0.011731,0.0147697,0.0181222,0.0206046,0.0232316,0.025246,0.0264811,0.0272139,0.0286156,0.0294709,0.0297943,0.0313039,0.0306496,0.0311618,0.0301668,0.0312525,0.0317671,0.0317181,0.0302673,0.0299046,0.0288313,0.0274418,0.0267336,0.0258611,0.0236311,0.0215947,0.0189627,0.0177129,0.0164018,0.0146644,0.013368,0.0129048,0.0125422,0.0121452,0.0123191,0.0122089,0.0125593,0.0120569,0.0121746,0.0124392,0.0122726,0.0122334,0.0119491,0.0108537,0.00959653,0.00806736,0.00693274,0.00550404,0.00420277,0.00376657,0.00328625,0.00231826,0.00160269,0.00105621,0.000749882,0.000504823,0.00037004,0.000276917,0.000166641,0.000161739,0.000115178,3.67589e-05,2.69566e-05,2.94072e-05,1.2253e-05,1.2253e-05,4.90119e-06,4.90119e-06,2.4506e-06,1.2253e-05,2.4506e-06,2.4506e-06,0,0,2.4506e-06,0,2.4506e-06,0,0,0,0,0,0,0	
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
