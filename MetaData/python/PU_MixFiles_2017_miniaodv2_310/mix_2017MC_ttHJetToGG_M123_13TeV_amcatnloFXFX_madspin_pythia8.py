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
		2.25662e-05,0,1.2895e-05,3.8685e-05,6.12512e-05,7.41462e-05,0.000122502,0.000116055,0.000303032,0.000370731,0.000673763,0.00137009,0.0024565,0.00390073,0.00581886,0.00861708,0.0118086,0.0145907,0.0183206,0.0209318,0.0232819,0.0250936,0.0257577,0.0271826,0.0287333,0.0293264,0.0303838,0.0308448,0.0307159,0.0311801,0.0305998,0.030574,0.0315218,0.0307513,0.0309254,0.0303838,0.028946,0.0277339,0.0260479,0.0259866,0.0237042,0.0213767,0.019623,0.0174856,0.0159962,0.0147132,0.0131045,0.013356,0.0123921,0.0120923,0.0122793,0.0126726,0.0124211,0.0126951,0.012492,0.0126403,0.0118956,0.0119408,0.0117054,0.0110575,0.00962289,0.00853648,0.00668283,0.00547392,0.00414252,0.00378146,0.00294006,0.0021857,0.00157964,0.00108963,0.000735014,0.000628631,0.000415863,0.000290137,0.00023211,0.000122502,0.000116055,7.09224e-05,2.25662e-05,3.22375e-05,3.54612e-05,9.67124e-06,1.2895e-05,1.2895e-05,3.22375e-06,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0	
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
