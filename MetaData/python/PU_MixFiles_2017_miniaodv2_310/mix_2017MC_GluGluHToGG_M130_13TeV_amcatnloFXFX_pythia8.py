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
		0.0113639,0.000795101,0.0016338,0.000575047,0.000448412,0.00066639,0.00218393,0.000778493,0.00110027,0.000938343,0.00187876,0.00318248,0.00399004,0.00584804,0.00835167,0.010328,0.0115155,0.0145775,0.0183807,0.0202989,0.0228462,0.0242122,0.0261927,0.0263982,0.0283122,0.0282873,0.0288831,0.030492,0.0297322,0.0295744,0.0295744,0.0303239,0.0295931,0.0293627,0.029892,0.028578,0.0274258,0.0260183,0.0252792,0.0242246,0.0221486,0.0204526,0.0189246,0.016475,0.0151713,0.0140233,0.0124414,0.0126199,0.0119909,0.011721,0.0115445,0.0116795,0.0119431,0.0114449,0.0118103,0.0117459,0.0129624,0.0115902,0.0108532,0.010301,0.00920905,0.00776417,0.00648329,0.00779531,0.00402325,0.00344613,0.00315964,0.00219846,0.00160058,0.00103799,0.000745277,0.000514843,0.000340461,0.000217978,0.000168154,0.000354993,9.34191e-05,7.47353e-05,4.35956e-05,2.07598e-05,0.000124559,0.000288561,6.22794e-06,4.15196e-06,6.22794e-06,4.15196e-06,2.07598e-06,0.00044426,0.00293129,0,2.07598e-06,0,0,0,0.000950799,8.30392e-06,6.22794e-06,0,6.22794e-06,4.15196e-05	
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
