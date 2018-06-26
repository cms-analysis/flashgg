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
		4.09122e-05,2.04561e-05,3.21453e-05,4.09122e-05,7.01352e-05,8.76691e-05,0.000119814,0.000149037,0.000376977,0.000379899,0.000675052,0.00125075,0.00234369,0.00386913,0.00581538,0.00828473,0.0116395,0.0152778,0.0178114,0.0207746,0.0232323,0.0247431,0.0259121,0.0270868,0.0285187,0.0291032,0.0302078,0.0313505,0.0307368,0.0308741,0.0312745,0.0310553,0.0313183,0.0309676,0.0303539,0.0305468,0.02896,0.0276508,0.0264322,0.025275,0.0237817,0.0215666,0.0197022,0.0181241,0.0159382,0.0148278,0.0133754,0.0127997,0.0124139,0.0121597,0.0123496,0.0122883,0.0123906,0.0125016,0.0126214,0.0123175,0.0123175,0.0125104,0.0115665,0.0107044,0.00967866,0.00822628,0.00694047,0.00550562,0.00410876,0.00377854,0.00320577,0.00244012,0.00152252,0.00113385,0.000756876,0.000575693,0.000347754,0.00022794,0.000201639,0.000166571,8.18245e-05,6.42906e-05,2.33784e-05,3.21453e-05,1.75338e-05,2.63007e-05,5.8446e-06,5.8446e-06,2.9223e-06,2.9223e-06,0,0,0,0,2.9223e-06,0,0,0,0,0,0,0,0,0	
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
