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
		0.017184,0.000179297,0.000404129,0.000973325,0.00166205,0.00119531,0.00101317,0.000335825,0.00366277,0.00225971,0.00187835,0.00314481,0.00540736,0.00927504,0.013268,0.0128097,0.0155191,0.0163672,0.0177589,0.0197938,0.0216664,0.0228589,0.0246234,0.0254886,0.025648,0.0271364,0.0273413,0.0278764,0.0279418,0.0273043,0.0322791,0.0278308,0.0275291,0.0272702,0.0295896,0.0289521,0.0258842,0.0240741,0.0237611,0.0228333,0.0215839,0.0197027,0.0174032,0.0161025,0.0146113,0.0163928,0.01166,0.0115177,0.0110851,0.0113156,0.0115974,0.0115547,0.0114266,0.0115575,0.0111733,0.013046,0.0153284,0.0111192,0.0107521,0.0112445,0.00899614,0.00788621,0.0065429,0.0049207,0.0040726,0.0033611,0.00287444,0.00206049,0.00164498,0.00101032,0.0007428,0.00052366,0.000321596,0.00461902,0.000202064,0.000150837,9.96092e-05,7.96874e-05,5.40736e-05,2.56138e-05,0.000472432,1.42299e-05,1.13839e-05,2.84598e-06,1.13839e-05,0,5.69196e-06,2.84598e-06,0,0.00711779,2.84598e-06,0,0,0,0,0,2.84598e-06,0,0,1.13839e-05	
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
