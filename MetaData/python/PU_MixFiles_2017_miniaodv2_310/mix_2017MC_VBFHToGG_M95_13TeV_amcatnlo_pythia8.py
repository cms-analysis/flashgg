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
		3.66667e-05,3.33333e-06,1.33333e-05,3.33333e-05,7.33333e-05,0.000106667,0.00013,0.0001,0.000386667,0.000373333,0.000766667,0.00134667,0.00229,0.00397667,0.00615667,0.00862333,0.0115267,0.015,0.0179667,0.0211267,0.0227567,0.02442,0.0261967,0.0275433,0.02759,0.0294267,0.03049,0.0312833,0.0303533,0.0310833,0.03083,0.0316667,0.0311733,0.0311767,0.03018,0.0298433,0.02921,0.0273533,0.02627,0.0256933,0.0232233,0.02153,0.01942,0.0180033,0.0162967,0.0148133,0.0137433,0.0131233,0.0125267,0.0124067,0.0124133,0.0122867,0.0122167,0.0125,0.01235,0.0122067,0.0124733,0.0123033,0.0117233,0.0108133,0.00956,0.00821,0.00692667,0.00539333,0.00424,0.00385,0.00319333,0.00223,0.00163667,0.00113,0.000853333,0.000503333,0.000396667,0.00026,0.00021,0.000156667,0.000126667,6.66667e-05,4.33333e-05,2e-05,1e-05,1e-05,6.66667e-06,3.33333e-06,6.66667e-06,3.33333e-06,0,0,0,0,3.33333e-06,0,0,0,0,0,0,0,0,0	
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
