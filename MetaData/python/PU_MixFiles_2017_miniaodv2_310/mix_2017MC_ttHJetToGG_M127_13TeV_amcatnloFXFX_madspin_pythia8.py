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
		3.22469e-05,9.67408e-06,1.28988e-05,4.1921e-05,4.51457e-05,7.41679e-05,0.000170909,0.000119314,0.000403087,0.000299896,0.000619141,0.00126086,0.00230243,0.00383739,0.00613014,0.0083842,0.0114186,0.0148529,0.0180357,0.0209412,0.0229534,0.025288,0.026278,0.0270229,0.0282161,0.0288417,0.0302251,0.030454,0.0312956,0.0315568,0.0310731,0.0306701,0.0309796,0.0310957,0.0310828,0.0296253,0.0292576,0.0277001,0.0261103,0.0257298,0.0234854,0.0215152,0.0196577,0.0180841,0.0155817,0.0144176,0.0134437,0.0132922,0.0123474,0.0123312,0.0125989,0.0128375,0.0125957,0.0124441,0.0123312,0.0123635,0.0124602,0.0125247,0.011725,0.0106834,0.00946448,0.0083842,0.00680088,0.00549488,0.00435011,0.00359876,0.00307313,0.0023508,0.00158655,0.00113187,0.000780376,0.000603018,0.00037084,0.000193482,0.000164459,0.000151561,0.000122538,8.3842e-05,2.25729e-05,4.1921e-05,2.25729e-05,9.67408e-06,6.44939e-06,6.44939e-06,3.22469e-06,3.22469e-06,6.44939e-06,0,0,3.22469e-06,0,0,0,0,0,0,0,0,0,0	
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
