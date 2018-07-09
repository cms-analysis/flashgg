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
		0.0533822,0.00199001,0.00595361,0.00347759,0.00197359,0.00302442,0.0036582,0.00222645,0.00342833,0.00120517,0.00108367,0.00214764,0.00346774,0.00498488,0.00727372,0.0106462,0.0136575,0.0181695,0.0205864,0.0207145,0.025834,0.0234729,0.0239983,0.0252298,0.0258964,0.0273544,0.0270753,0.0271968,0.0269341,0.0271673,0.0266812,0.0264612,0.0262938,0.0268027,0.0262051,0.0258077,0.0249474,0.0235846,0.0226421,0.0220773,0.0205043,0.018511,0.0166195,0.0152765,0.0160317,0.0123571,0.0116379,0.0112045,0.0112636,0.0106889,0.0112308,0.0114705,0.01106,0.0113523,0.0109352,0.0114967,0.0111487,0.0112308,0.0105608,0.00982527,0.00878757,0.00762837,0.00637723,0.00500786,0.00374687,0.00336266,0.00274201,0.00223302,0.00154341,0.000942464,0.000732298,0.000571389,0.000295546,0.00021345,0.000203598,0.000164192,0.000105083,7.22446e-05,4.59738e-05,2.29869e-05,0.00510638,1.97031e-05,3.28385e-06,0.000436751,0,0,0,0,0,0.000459738,0,0,0,0,6.56769e-06,6.56769e-06,1.64192e-05,0,0,2.62708e-05	
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
