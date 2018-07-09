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
		0.00537005,2.00002e-05,2.00002e-05,7.00007e-05,0.000140001,0.000180002,0.000230002,0.000150002,0.000490005,0.000560006,0.000920009,0.00174002,0.00353004,0.00614006,0.00870009,0.0108501,0.0126801,0.0150802,0.0216802,0.0222302,0.0245102,0.0274103,0.0281703,0.0273603,0.0318803,0.0297103,0.0307003,0.0300903,0.0301003,0.0303203,0.0297703,0.0300303,0.0289903,0.0294103,0.0290603,0.0278303,0.0269103,0.0264303,0.0242102,0.0241802,0.0219902,0.0207402,0.0180502,0.0156002,0.0150402,0.0131501,0.0116601,0.0116201,0.0115201,0.0115901,0.0118201,0.0114801,0.0116301,0.0110501,0.0113201,0.0109901,0.0113101,0.0113401,0.0108601,0.00945009,0.00893009,0.00736007,0.00629006,0.00497005,0.00395004,0.00327003,0.00262003,0.00210002,0.00149001,0.00110001,0.000650007,0.000560006,0.000340003,0.000150002,0.000220002,0.000140001,9.00009e-05,4.00004e-05,6.00006e-05,2.00002e-05,2.00002e-05,2.00002e-05,1.00001e-05,0,0.0155102,0,1.00001e-05,0,0,0,0,0,0,0,0,0,0,0,0,1.00001e-05	
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
