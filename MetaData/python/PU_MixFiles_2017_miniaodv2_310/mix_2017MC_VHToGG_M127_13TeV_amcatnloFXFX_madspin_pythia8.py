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
		2.43699e-05,3.04624e-06,2.13237e-05,3.04624e-05,5.78785e-05,0.000103572,0.000106618,0.000106618,0.000466074,0.000365548,0.000779836,0.00135253,0.00236083,0.00403017,0.00601327,0.0084533,0.0116458,0.0148504,0.0179819,0.02108,0.0230021,0.0249669,0.0262616,0.0270658,0.0283178,0.0289484,0.030371,0.031035,0.0309102,0.0313732,0.0315925,0.0312544,0.0312026,0.0309772,0.0303771,0.0300633,0.0288661,0.0271359,0.0259966,0.0260697,0.0234195,0.0221979,0.0192126,0.0176682,0.0161115,0.0144605,0.0138116,0.0130409,0.0122154,0.0123312,0.0121606,0.0121636,0.0125779,0.0125018,0.0125139,0.0121971,0.0119961,0.0120661,0.0118103,0.0109695,0.00963524,0.00852032,0.006918,0.00563554,0.00397534,0.00345443,0.00320464,0.00226335,0.00169675,0.00116062,0.00072805,0.000523953,0.000426473,0.000216283,0.000179728,0.000152312,6.3971e-05,7.31097e-05,4.26473e-05,2.74161e-05,2.13237e-05,3.04624e-06,9.13871e-06,9.13871e-06,9.13871e-06,0,3.04624e-06,0,0,0,0,0,0,0,0,0,0,0,0,0	
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
