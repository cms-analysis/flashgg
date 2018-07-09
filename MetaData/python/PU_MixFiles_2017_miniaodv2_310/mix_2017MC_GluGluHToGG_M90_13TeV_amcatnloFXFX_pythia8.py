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
		0.040375,0.000289982,0.000606939,0.00149712,0.00164548,0.000155107,0.00266716,0.00126783,0.00144654,0.00138247,0.00261995,0.00297063,0.00456216,0.00650099,0.008659,0.0114644,0.0142833,0.0181509,0.0203729,0.0218633,0.0233705,0.0242506,0.0262501,0.0265131,0.0276967,0.0274944,0.0276225,0.0281013,0.0278889,0.0297164,0.0297569,0.0279698,0.026911,0.0276865,0.0267896,0.0264828,0.0264996,0.0241124,0.023121,0.0221398,0.0206595,0.0193378,0.0175203,0.0155174,0.0143541,0.0181846,0.0119702,0.0112081,0.0109687,0.0109822,0.0109283,0.011134,0.0111744,0.0113801,0.0113262,0.011687,0.0115622,0.0112183,0.0109215,0.00966382,0.00910746,0.00746873,0.00652123,0.00513875,0.00376977,0.00328759,0.00299086,0.00201302,0.00138584,0.00102168,0.00076879,0.000472064,0.000360792,0.000185454,0.000219173,0.000141619,6.40658e-05,4.38345e-05,6.40658e-05,3.70907e-05,1.01157e-05,1.01157e-05,6.74377e-06,0,1.01157e-05,0,3.37189e-06,0,1.34875e-05,0,3.37189e-06,0,0,0,0,3.37189e-06,6.74377e-06,0,0,1.68594e-05	
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
