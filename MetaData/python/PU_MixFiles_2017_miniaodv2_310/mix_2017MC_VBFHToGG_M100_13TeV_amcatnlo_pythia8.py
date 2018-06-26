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
		0.0322102,0.000103334,0.000230002,0.00056667,0.00106667,0.00052667,0.000373336,0.0055267,0.00266668,0.00132668,0.00223668,0.00365002,0.00363002,0.00639671,0.00740005,0.00954006,0.0127801,0.0168034,0.0195135,0.0248435,0.0229735,0.0288302,0.0245868,0.0262168,0.0311669,0.0261202,0.0332736,0.0280135,0.0274902,0.0268202,0.0272268,0.0270835,0.0269302,0.0269435,0.0269735,0.0276569,0.0253568,0.0231335,0.0231835,0.0218268,0.0200968,0.0184635,0.0166734,0.0150934,0.0137368,0.0123667,0.0117067,0.0110434,0.0107634,0.0104001,0.0111701,0.0114967,0.0115234,0.0122001,0.0117567,0.0112401,0.0112134,0.0112534,0.0109334,0.00984007,0.00902673,0.00727672,0.00605337,0.0049767,0.00901006,0.00323335,0.00292335,0.00195668,0.00148668,0.000966673,0.000823339,0.00564337,0.000346669,0.000206668,0.000193335,0.000163334,0.000110001,6.33338e-05,0.00233335,0.00053667,2.66668e-05,3.33336e-06,3.33336e-06,0.000100001,3.33336e-06,0,0.00517003,4.00003e-05,0.00108667,0,0,0,0,0,3.33336e-06,0,3.33336e-06,0,4.33336e-05,1.66668e-05	
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
