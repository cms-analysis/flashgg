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
		0.0071069,0.000146672,0.000243341,0.000616687,0.00101003,0.00163005,0.00116671,0.000426681,0.00197673,0.00158339,0.0030701,0.00452015,0.00534018,0.00654022,0.00857695,0.0118637,0.0163705,0.0180406,0.019914,0.0220741,0.0225674,0.0239875,0.0258475,0.0257775,0.0272009,0.0273042,0.0285376,0.0289443,0.0291443,0.0283709,0.029501,0.0288443,0.029091,0.0292643,0.0287743,0.0326944,0.0269842,0.0259309,0.0246042,0.0234974,0.0220507,0.0202807,0.0184739,0.0163672,0.0151772,0.0135938,0.0125671,0.0124637,0.0118504,0.0113937,0.0116171,0.0141138,0.0120671,0.0117137,0.0116437,0.0116604,0.0117004,0.011247,0.0110704,0.010127,0.0091303,0.0078736,0.00633354,0.00634688,0.00380013,0.00348012,0.0030901,0.00216007,0.00155339,0.00104337,0.000693356,0.000523351,0.000340011,0.000240008,0.00256009,0.000173339,9.33364e-05,5.00017e-05,3.66679e-05,3.33344e-05,2.00007e-05,3.33344e-06,1.66672e-05,7.33358e-05,1.00003e-05,6.66689e-06,3.33344e-06,3.33344e-06,3.33344e-06,0,0,0,6.66689e-06,0,0,3.33344e-06,0,0,0,3.0001e-05	
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
