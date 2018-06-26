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
		0.0511837,0.000545965,0.000840259,0.00320171,0.00149075,0.00390091,0.00107569,0.000962035,0.00335495,0.00113658,0.00152627,0.00201845,0.00406632,0.00557331,0.00790533,0.0104048,0.0130047,0.0154616,0.0182726,0.0205224,0.022552,0.0240681,0.0245816,0.0252829,0.0260825,0.0271562,0.0276068,0.0280756,0.027623,0.0277651,0.0279295,0.0276504,0.0281598,0.0275946,0.0272678,0.0299946,0.0257365,0.0242782,0.0234887,0.02303,0.0208613,0.0192691,0.0173014,0.0156148,0.014054,0.0128961,0.0122852,0.011587,0.010833,0.0108939,0.0109518,0.0110969,0.0112846,0.0111832,0.0121756,0.0111669,0.0112156,0.0109416,0.0105986,0.00981824,0.00868876,0.00743954,0.0065394,0.00487208,0.00382886,0.00326057,0.00296222,0.00204585,0.00149278,0.00102901,0.000672816,0.000495225,0.000738778,0.000211079,0.000192813,0.00015628,0.000123806,0.000100466,5.27699e-05,0.00028719,2.84145e-05,0.00157802,8.11844e-06,6.08883e-06,8.11844e-06,1.0148e-06,1.0148e-06,0.00314894,4.05922e-06,0,3.04441e-06,0.00156077,0,0,1.11629e-05,0,3.04441e-06,4.05922e-06,0,4.26218e-05	
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
