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
		0.0297339,0.00562487,0.00601002,0.000548978,0.000425386,0.00059784,0.00084215,0.000942748,0.00247471,0.00209532,0.00380261,0.00447805,0.00398369,0.00652451,0.00991898,0.0123391,0.0148397,0.0172224,0.0197776,0.0210652,0.022807,0.0237527,0.0240803,0.0268223,0.02498,0.025756,0.0270437,0.0280008,0.027607,0.0275553,0.0285469,0.032887,0.026854,0.0271126,0.0270149,0.0261584,0.0251467,0.0240746,0.0243045,0.0218011,0.020407,0.0184669,0.0172023,0.0153944,0.0136382,0.0124196,0.0142821,0.0120085,0.0107899,0.0134974,0.01106,0.0111175,0.0116291,0.012799,0.0112095,0.0149977,0.0110658,0.0110054,0.0105168,0.00970916,0.00850198,0.00759947,0.00583469,0.00490631,0.00362728,0.00335998,0.00308405,0.00213843,0.00155496,0.000982987,0.000649576,0.000497242,0.000356405,0.000201196,0.000183951,0.000137963,8.91012e-05,6.03589e-05,6.32331e-05,0.00119568,0.0016958,8.6227e-06,1.14969e-05,5.74846e-06,0,0,2.87423e-06,0,0,0,2.87423e-06,2.87423e-06,0,0,0,0.00445219,2.87423e-06,0,0,2.01196e-05	
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
