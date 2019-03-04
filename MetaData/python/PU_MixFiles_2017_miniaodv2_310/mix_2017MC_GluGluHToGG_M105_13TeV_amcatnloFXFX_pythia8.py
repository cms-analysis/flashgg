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
               4.15313e-05, 0, 1.66125e-05, 1.66125e-05, 3.32251e-05, 6.64501e-05, 9.1369e-05, 0.0001329, 0.000332251, 0.000307332, 0.000838933, 0.00127086, 0.00240051, 0.00399532, 0.00600543, 0.0083478, 0.0108231, 0.0149098, 0.0176342, 0.0206245, 0.022701, 0.0248607, 0.0263558, 0.0270867, 0.0282164, 0.030077, 0.0300022, 0.0303843, 0.0312565, 0.0306667, 0.0313977, 0.0316469, 0.0309575, 0.0311568, 0.030999, 0.0295454, 0.0285071, 0.0274273, 0.0267213, 0.0250102, 0.0240051, 0.0219452, 0.020043, 0.018415, 0.0157653, 0.014943, 0.0137136, 0.0131737, 0.0121105, 0.0119444, 0.0119693, 0.0125591, 0.0126504, 0.0127917, 0.012393, 0.0123597, 0.0122268, 0.0121853, 0.0116205, 0.0111719, 0.00974325, 0.00844747, 0.00672808, 0.00568979, 0.00408668, 0.00339726, 0.00318961, 0.00242543, 0.00171109, 0.00108812, 0.000847239, 0.000581439, 0.000340557, 0.000257494, 0.000166125, 0.0001329, 9.96752e-05, 9.96752e-05, 4.98376e-05, 1.66125e-05, 2.49188e-05, 0, 8.30627e-06, 0, 0, 0, 1.66125e-05, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
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
