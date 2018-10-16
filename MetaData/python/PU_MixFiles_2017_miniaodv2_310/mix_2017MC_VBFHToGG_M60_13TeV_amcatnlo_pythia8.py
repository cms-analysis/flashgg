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
		0.00495373,6.6672e-05,0.00012001,0.000113342,0.000206683,0.000340027,0.00054671,0.00144012,0.00216017,0.00196682,0.00670054,0.00227352,0.00375363,0.00617383,0.00960077,0.012841,0.0171814,0.0181348,0.0192815,0.0223685,0.0310558,0.0288823,0.0271022,0.0272888,0.0266621,0.0265221,0.0271955,0.0276222,0.0272488,0.0275155,0.0271488,0.0273955,0.0275555,0.0276489,0.0265955,0.0268421,0.0253954,0.0240619,0.0226618,0.0226551,0.0207483,0.0192482,0.016948,0.0159546,0.0144878,0.0127344,0.0116009,0.0171414,0.0112409,0.0171747,0.0118543,0.0118409,0.0114943,0.0118009,0.0113876,0.0153412,0.0116876,0.0116943,0.0116076,0.00989412,0.00939408,0.00819399,0.00656719,0.00532043,0.00388698,0.00346028,0.00334027,0.00274022,0.00143345,0.00163346,0.000753394,0.000506707,0.000313358,0.000246686,0.000193349,0.000140011,0.000106675,8.66736e-05,4.66704e-05,3.3336e-05,3.3336e-05,0.0103408,2.00016e-05,0,1.33344e-05,6.6672e-06,0,0,0,0,0,0,0,0,0,0,0,0,0,2.66688e-05	
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
