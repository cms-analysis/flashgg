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
		0.0226929,0.000498067,0.00133163,0.00153102,0.00125374,0.00143973,0.00109445,0.0012009,0.00197609,0.00166418,0.00220037,0.00318304,0.00467476,0.00597595,0.0083019,0.0107593,0.0141093,0.0166169,0.0188757,0.0216873,0.0232566,0.0245951,0.025513,0.0263095,0.0267636,0.0278722,0.0285985,0.029049,0.0286807,0.0288464,0.0285359,0.0290436,0.0290044,0.0285869,0.0283587,0.0274849,0.0262845,0.024992,0.0242712,0.0236139,0.0215618,0.0199564,0.0180079,0.0161736,0.0146798,0.0135519,0.0124005,0.0120756,0.0116,0.0118596,0.0119949,0.0116436,0.0117973,0.0117734,0.0116572,0.0120128,0.0116908,0.0117896,0.0110235,0.0102943,0.0090561,0.00800006,0.00634804,0.00529212,0.00434022,0.00352872,0.0031116,0.00218895,0.00161684,0.00109337,0.000779937,0.000760365,0.000537267,0.0002649,0.000295785,0.000353709,0.000315188,6.26189e-05,3.99924e-05,0.000353596,0.000440312,0.000526463,0.000382502,3.69944e-05,0.000183954,0.000314509,0.000138361,0.000580936,0.000179655,0.00027791,3.95964e-07,1.60083e-05,8.28697e-05,3.95964e-07,8.90354e-05,7.18393e-05,2.54549e-06,6.78796e-07,1.73093e-05,4.55359e-05	
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
