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
		0.018281,0.00228991,0.00123454,0.00151107,0.00158564,0.000479524,0.000836837,0.00138782,0.00115272,0.000577915,0.00267933,0.00213456,0.00667399,0.00518881,0.00897425,0.00990845,0.0131201,0.0157311,0.0204735,0.0214833,0.023129,0.0243998,0.0257783,0.0272894,0.0257214,0.0279388,0.0287466,0.0292468,0.0283251,0.0284059,0.0294343,0.0285022,0.0295441,0.028409,0.0288989,0.0298279,0.0265012,0.0248669,0.0239265,0.0252853,0.0234739,0.0197019,0.0179589,0.0162448,0.0147306,0.0134464,0.0127307,0.0117375,0.0119021,0.0115345,0.0114578,0.0127121,0.0118079,0.0117758,0.0114879,0.0118234,0.0116153,0.0116847,0.0125702,0.0101425,0.0089349,0.00761853,0.00635396,0.00502931,0.00519502,0.00335667,0.00295586,0.00215838,0.00153075,0.00109369,0.000738447,0.000504381,0.000359384,0.000228887,0.000191603,0.00174203,9.63192e-05,4.86774e-05,8.38909e-05,1.3464e-05,2.48566e-05,9.32121e-06,5.38559e-05,0.000356277,2.07138e-06,0.000854444,2.07138e-06,6.21414e-06,0,3.10707e-06,4.14276e-06,0,1.03569e-06,1.03569e-06,5.17845e-06,0,1.03569e-06,0,0,2.07138e-05	
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
