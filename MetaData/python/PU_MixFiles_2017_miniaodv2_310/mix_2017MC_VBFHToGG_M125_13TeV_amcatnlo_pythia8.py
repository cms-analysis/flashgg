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
		0.0147694,0.00016725,0.000251394,0.00144655,0.00051785,0.00122165,0.000473701,0.000873645,0.00149382,0.00116815,0.0016995,0.00266353,0.00410852,0.00591606,0.00804148,0.0101783,0.0129016,0.0160637,0.0189044,0.0217523,0.0247586,0.0258577,0.026094,0.0272056,0.0271573,0.0294177,0.0291139,0.0299366,0.0293393,0.0289305,0.0292863,0.0289082,0.0289186,0.028864,0.0294079,0.0292323,0.02652,0.0252973,0.0254536,0.024553,0.0217845,0.0199505,0.0180515,0.0165,0.0152685,0.0134807,0.0123884,0.012686,0.0117885,0.0115958,0.0115579,0.0116929,0.0117563,0.0116597,0.0122679,0.0116836,0.0122264,0.0115314,0.0111844,0.0102407,0.00911925,0.00774126,0.00647547,0.00514059,0.00463988,0.00349198,0.00331798,0.00251082,0.00224229,0.00107621,0.000768725,0.000502787,0.000338654,0.000523564,0.000185948,0.00018439,9.03771e-05,0.000206725,4.62274e-05,0.000494477,2.59704e-05,0.00081703,7.27172e-06,9.34935e-06,9.55712e-05,6.2329e-06,3.11645e-06,0.00080768,0.000124658,0.000806641,1.55823e-06,1.03882e-06,1.55823e-06,5.19408e-07,0,5.19408e-07,4.67468e-06,5.19408e-07,0,2.75286e-05	
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
