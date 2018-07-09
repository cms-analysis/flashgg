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
		0.0147571,0.00010667,0.000186672,0.00026334,0.000423345,0.000500013,0.000460012,0.000543348,0.00103336,0.000843356,0.00201339,0.00388677,0.00546015,0.00716686,0.012427,0.0118536,0.0139137,0.0193638,0.0191538,0.0239373,0.0237806,0.0237073,0.0249507,0.0247573,0.0261374,0.0274874,0.0278007,0.0285474,0.0281941,0.0291241,0.0283341,0.0286274,0.0280507,0.0282708,0.0285108,0.0276307,0.0261574,0.0255007,0.0248273,0.0240973,0.0244573,0.0195305,0.0182705,0.0161671,0.0148937,0.013347,0.012177,0.0120037,0.0162771,0.011827,0.011917,0.011257,0.0123203,0.013847,0.0116503,0.0144137,0.0112836,0.011177,0.011117,0.0104569,0.00888357,0.00838022,0.00656684,0.00504347,0.00399011,0.00342676,0.00300008,0.00197005,0.00165338,0.00103003,0.000790021,0.000530014,0.000316675,0.000276674,0.000563348,0.000160004,0.000120003,7.33353e-05,5.00013e-05,0.00188672,1.66671e-05,1.66671e-05,3.33342e-06,0,0,0,0,0,6.66684e-06,0,0,0,1.66671e-05,0,0,2.3334e-05,6.66684e-06,0,0,2.00005e-05	
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
