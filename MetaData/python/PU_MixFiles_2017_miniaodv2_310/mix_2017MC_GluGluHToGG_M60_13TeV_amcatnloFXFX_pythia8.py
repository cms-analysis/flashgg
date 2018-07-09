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
		0.0238352,0.000368759,0.000720756,0.00183374,0.00259807,0.00133759,0.000177675,0.00017097,0.000382168,0.000492796,0.000915192,0.00188737,0.00304729,0.00501177,0.00736847,0.00953074,0.0135971,0.0152264,0.0177373,0.0207879,0.0227759,0.0244185,0.0250722,0.0265372,0.0272379,0.0282737,0.0289945,0.0291051,0.0320116,0.0289811,0.0288906,0.0279754,0.0285352,0.0283877,0.0278379,0.0275195,0.025964,0.0247303,0.0248376,0.0234698,0.0214215,0.0236844,0.0202851,0.017325,0.0156957,0.0141235,0.0127155,0.0122797,0.0111633,0.0111432,0.0110527,0.0111667,0.0159069,0.0116897,0.0114717,0.0112874,0.0113142,0.0112237,0.0105364,0.0100906,0.00886027,0.00744222,0.00586997,0.00494472,0.00767353,0.00336911,0.00299365,0.00207175,0.00147168,0.000972182,0.000663766,0.000754279,0.000415692,0.000191084,0.000181027,0.000147504,0.000110628,9.05135e-05,0.00202817,0.00131747,1.67618e-05,1.00571e-05,0,3.35235e-06,3.35235e-06,6.7047e-06,0.00223937,0,0,0.000844793,0,0,0,0,0.00109287,0,1.34094e-05,0,0,4.02282e-05	
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
