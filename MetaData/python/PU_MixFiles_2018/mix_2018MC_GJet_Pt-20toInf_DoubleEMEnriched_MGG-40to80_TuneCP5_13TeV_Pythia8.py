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
                1.6469e-07,9.88142e-07,4.94071e-07,8.06983e-06,1.77866e-05,3.35968e-05,6.88406e-05,0.000139987,0.000263834,0.000479414,0.000844038,0.00138752,0.00216765,0.00316996,0.00451021,0.00603294,0.00782641,0.00976647,0.0118636,0.013947,0.0162212,0.0184649,0.0206205,0.022683,0.0247754,0.02678,0.0284256,0.0300736,0.0318621,0.033293,0.0345782,0.035384,0.036289,0.0367684,0.0368859,0.0370025,0.0365454,0.0360782,0.0351454,0.0341172,0.0326301,0.0311095,0.0293668,0.0276163,0.0257362,0.0236877,0.0215138,0.0195095,0.0174633,0.0155843,0.0137202,0.011805,0.0102255,0.00862483,0.00733234,0.00608432,0.00495998,0.00402306,0.0032169,0.00260935,0.00204018,0.00160128,0.00123122,0.000944334,0.000722002,0.00055336,0.000427042,0.00030303,0.00022612,0.00017276,0.000124506,9.96376e-05,5.87944e-05,4.57839e-05,2.93149e-05,2.58564e-05,1.41634e-05,1.51515e-05,7.41106e-06,3.95257e-06,3.95257e-06,1.97628e-06,1.6469e-06,1.31752e-06,9.88142e-07,3.29381e-07,3.29381e-07,8.23452e-07,1.6469e-07,1.6469e-07,1.6469e-07,3.29381e-07,3.29381e-07,0,0,0,0,1.6469e-07,0,0),
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
