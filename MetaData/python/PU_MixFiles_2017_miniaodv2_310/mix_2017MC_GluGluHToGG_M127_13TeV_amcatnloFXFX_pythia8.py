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
		2.95677e-05,3.94236e-06,1.37983e-05,3.74524e-05,5.12507e-05,0.000104473,0.000114329,0.000114329,0.000376496,0.000372553,0.000699769,0.00133252,0.00242455,0.00395222,0.00595888,0.00871065,0.0117128,0.0150283,0.0179614,0.021206,0.0235201,0.0249552,0.025994,0.028115,0.0286078,0.0293213,0.0297175,0.0308214,0.0309377,0.0307445,0.0307899,0.0307406,0.0314679,0.0311131,0.0302911,0.029893,0.0284796,0.0269421,0.0267174,0.025787,0.0241864,0.0213341,0.0197355,0.0177012,0.016071,0.014498,0.0134789,0.0128994,0.0122943,0.0123278,0.0122312,0.0125781,0.01231,0.0124421,0.0124046,0.0125663,0.0121602,0.012243,0.0116398,0.0111884,0.00947941,0.00808184,0.00667836,0.00542666,0.00434843,0.00363683,0.00333327,0.00232797,0.00159666,0.00115708,0.000804242,0.000522363,0.00031736,0.000226686,0.000165579,0.000173464,6.11066e-05,6.70202e-05,4.3366e-05,2.95677e-05,1.57695e-05,2.1683e-05,5.91354e-06,1.18271e-05,1.97118e-06,7.88473e-06,0,1.97118e-06,1.97118e-06,0,0,1.97118e-06,0,0,0,0,0,0,0,0	
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
