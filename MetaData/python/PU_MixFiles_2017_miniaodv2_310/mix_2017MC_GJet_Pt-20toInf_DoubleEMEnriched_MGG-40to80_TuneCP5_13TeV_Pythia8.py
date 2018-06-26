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
		0.0237739,0.000720712,0.00138301,0.00159162,0.00141955,0.00118913,0.00114752,0.00116829,0.00175393,0.00144109,0.00220688,0.00331817,0.00465885,0.00619858,0.00858651,0.011008,0.0136731,0.0164982,0.0190711,0.0216787,0.0233185,0.0247208,0.0254773,0.0259224,0.0268456,0.0276721,0.0284425,0.0287237,0.0285276,0.0287408,0.0286181,0.028747,0.0288568,0.0285058,0.0281626,0.0274733,0.0264501,0.0251481,0.0243064,0.0234344,0.021779,0.0197344,0.0178403,0.016319,0.0147769,0.0134291,0.0123745,0.0120035,0.0118787,0.0115458,0.0117258,0.0118403,0.0119504,0.012081,0.0116833,0.0117625,0.011919,0.0115278,0.0110502,0.0101991,0.00913312,0.00790627,0.00653364,0.00526658,0.00433617,0.00344623,0.00301566,0.00231559,0.00169609,0.00117774,0.000886973,0.000679563,0.000586483,0.000272534,0.00033819,0.000203768,0.000346994,0.000124557,0.000186546,0.000328831,0.000346849,0.000499843,0.000254372,0.000413734,5.04836e-05,0.000286838,0.000276611,0.000405509,0.000114233,0.000122482,2.5085e-06,9.23805e-06,7.27225e-05,7.23607e-07,0.000136641,0.000104826,6.75125e-05,1.85726e-05,7.47727e-07,3.25382e-05	
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
