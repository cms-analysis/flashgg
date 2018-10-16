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
		0.0267718,0.000310483,0.00068495,0.0012734,0.00204331,0.000821311,0.00109718,0.000502437,0.0011769,0.0012241,0.00130487,0.00229401,0.00420515,0.00672048,0.00973091,0.0126039,0.0153039,0.0166192,0.0188576,0.0207132,0.0232086,0.0251009,0.0245858,0.0251155,0.0264739,0.0276036,0.0288644,0.0288833,0.0286871,0.0289766,0.0288109,0.0295672,0.0302815,0.0287952,0.0283976,0.0278784,0.0267047,0.0250589,0.0248785,0.0237194,0.0215681,0.0198803,0.0180888,0.0162343,0.0147081,0.0133738,0.0121623,0.0128913,0.0114386,0.0115246,0.0118497,0.0114816,0.0116494,0.0117879,0.0118162,0.011555,0.0116714,0.0113158,0.0107704,0.0100361,0.00915295,0.00766032,0.00631035,0.00522996,0.00397334,0.00341007,0.00294539,0.0022489,0.00153773,0.00106047,0.000709076,0.000502437,0.000315727,0.000229715,0.000177269,0.000154192,9.12568e-05,8.07675e-05,4.30061e-05,2.51743e-05,0.000588449,1.36361e-05,1.04893e-06,0.00163213,6.29357e-06,0.000204541,3.14678e-06,9.44035e-06,0,0,0,2.09786e-06,0,1.04893e-06,0,5.24464e-06,2.09786e-06,0,1.04893e-06,1.67829e-05	
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
