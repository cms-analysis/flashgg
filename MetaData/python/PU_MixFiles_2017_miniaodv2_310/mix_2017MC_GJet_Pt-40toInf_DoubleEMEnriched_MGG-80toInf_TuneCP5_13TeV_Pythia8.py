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
                3.35498e-05, 7.28102e-06, 1.35886e-05, 3.47958e-05, 5.93254e-05, 9.24729e-05, 0.00012126, 0.000128956, 0.000361806, 0.000362818, 0.000702625, 0.00134224, 0.00238063, 0.00388251, 0.00592804, 0.00857455, 0.0116362, 0.0148929, 0.0179643, 0.0208729, 0.0232647, 0.0249865, 0.0262338, 0.0272966, 0.0283354, 0.0294124, 0.030313, 0.0309948, 0.0308487, 0.0310496, 0.0309301, 0.0310789, 0.0310334, 0.031093, 0.0308082, 0.0299932, 0.0288579, 0.0272907, 0.0264461, 0.0255469, 0.0235967, 0.0215759, 0.0195451, 0.0177381, 0.0160444, 0.014614, 0.0134111, 0.0129762, 0.0125186, 0.0124327, 0.0123523, 0.0124, 0.0124238, 0.0124496, 0.0123667, 0.0124019, 0.0123217, 0.0122283, 0.0117049, 0.010839, 0.0096362, 0.00825809, 0.00685314, 0.00542299, 0.00421572, 0.00367936, 0.00315048, 0.00228499, 0.00162716, 0.001142, 0.000794423, 0.000534344, 0.000358172, 0.000242467, 0.000196354, 0.000157197, 0.000102544, 6.76966e-05, 4.40106e-05, 2.79041e-05, 1.79884e-05, 1.16808e-05, 7.35889e-06, 6.85273e-06, 4.73721e-06, 2.62169e-06, 2.36211e-06, 1.24595e-06, 7.91698e-07, 5.06167e-07, 2.72552e-07, 1.29786e-07, 3.24466e-07, 1.9468e-07, 3.89359e-08, 5.19146e-08, 0, 0, 0, 0
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
