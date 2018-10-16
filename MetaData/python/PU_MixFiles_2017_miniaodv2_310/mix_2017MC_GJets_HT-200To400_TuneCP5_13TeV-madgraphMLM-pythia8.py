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
		0.0257035,0.00109141,0.00140468,0.00195991,0.00189016,0.00182742,0.00171736,0.00164065,0.00240077,0.00216982,0.00247986,0.00362435,0.00507651,0.00683752,0.0094298,0.0117595,0.0140401,0.0167672,0.0189573,0.0214076,0.0230544,0.0244055,0.0251126,0.0256265,0.026488,0.0272422,0.0281115,0.0284037,0.0283073,0.0280953,0.0280121,0.0283611,0.0284279,0.0279571,0.0276759,0.0270251,0.0258893,0.0247984,0.0240135,0.0230354,0.0215191,0.0194492,0.017843,0.0162268,0.014446,0.0132697,0.0123486,0.0118356,0.0114719,0.0113852,0.0116773,0.0116669,0.0118329,0.0118031,0.0114581,0.0116809,0.0115468,0.0113596,0.0109173,0.0102401,0.00912402,0.00795968,0.00654615,0.00534457,0.00415735,0.00374314,0.0033585,0.00232799,0.00167437,0.00109351,0.000981682,0.00100229,0.000422894,0.000373964,0.000225177,0.000225932,0.00051358,0.000307447,0.000609552,0.000187521,0.000456611,0.000541848,0.000259703,0.000581823,0.000107194,0.000376931,0.000126723,0.000312572,0.000254092,0.000114584,1.58605e-05,0.000162112,2.91316e-06,1.59684e-05,0.000177919,7.06172e-05,2.10395e-06,2.64342e-06,8.09212e-07,3.11277e-05	
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
