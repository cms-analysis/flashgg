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
		0.0182012,0.000307901,0.000535698,0.00428308,0.000773507,0.00116902,0.00201012,0.000996298,0.00191499,0.000888657,0.0045309,0.00312657,0.00447583,0.00738962,0.0103785,0.0124988,0.0146391,0.014937,0.0182312,0.0210124,0.0229599,0.0245169,0.0244869,0.0290003,0.0267323,0.027636,0.0285672,0.0286173,0.0292982,0.0292806,0.0329379,0.0290854,0.0280741,0.0289077,0.0280941,0.0266322,0.0279714,0.0247397,0.0240363,0.0229899,0.0216056,0.0192225,0.0178232,0.0159007,0.0138405,0.0131897,0.0118805,0.0113873,0.0110294,0.0109793,0.0119355,0.0116051,0.0116176,0.0115175,0.011137,0.0112722,0.0113047,0.0110694,0.0105663,0.0110544,0.00886405,0.00751479,0.00696407,0.00884903,0.00370983,0.0032993,0.00287625,0.00208271,0.00167218,0.00116151,0.0009212,0.000523181,0.000400522,0.000212777,0.00364224,0.000152699,0.00011515,4.25554e-05,5.00652e-05,2.50326e-05,3.25424e-05,1.0013e-05,7.50978e-06,7.50978e-06,5.00652e-06,7.50978e-06,5.00652e-06,0,1.25163e-05,0,0,0,0,0,0,0,0,0,0,3.00391e-05	
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
