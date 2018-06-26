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
		0.0331637,0.000176668,0.00029667,0.00100001,0.00130335,0.00178335,0.00522672,0.00037667,0.00105001,0.00114334,0.00205002,0.00414004,0.00393337,0.00436671,0.00632673,0.00885009,0.0119568,0.0149735,0.0186935,0.0209702,0.0233469,0.0232569,0.0255136,0.0247969,0.0260136,0.0266603,0.0271203,0.0282569,0.029797,0.028607,0.0288103,0.0282836,0.028697,0.0284536,0.0280403,0.0274503,0.0260703,0.0243069,0.0236669,0.0234369,0.0218569,0.0198069,0.0177902,0.0163968,0.0147468,0.0132335,0.0125801,0.0122735,0.0114368,0.0116401,0.0115901,0.0115768,0.0111968,0.0118501,0.0115034,0.0116868,0.0113634,0.0113668,0.0108468,0.0103101,0.00869675,0.00776008,0.00614339,0.00486005,0.00392337,0.00343337,0.00302336,0.00206335,0.00153002,0.00105001,0.00518672,0.000500005,0.00170668,0.000183335,0.000170002,0.000183335,0.00528672,3.6667e-05,4.00004e-05,2.00002e-05,1.33335e-05,0.00154668,1.33335e-05,3.33337e-06,3.33337e-06,0,3.33337e-06,0,0.00517005,0,0,0,3.33337e-06,0,0,0,0,0,0,2.66669e-05	
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
