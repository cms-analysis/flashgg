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
		3.9521e-05,7.9042e-06,3.9521e-06,3.35929e-05,5.92815e-05,0.000114611,0.000116587,0.00015018,0.000440659,0.000365569,0.000760779,0.00134371,0.00239893,0.00379006,0.00599138,0.00857211,0.0116725,0.014848,0.0176718,0.0207228,0.0234458,0.0250563,0.0259653,0.0273999,0.0284057,0.0290677,0.0299984,0.0311781,0.0305221,0.0308936,0.031022,0.0317077,0.031354,0.0313184,0.0307948,0.0299767,0.0290045,0.0272399,0.0267518,0.0256215,0.0231218,0.0215113,0.0190728,0.0179109,0.0162471,0.0145003,0.0134707,0.0128799,0.0124669,0.0124353,0.0124965,0.012457,0.0124274,0.0125084,0.012455,0.0122693,0.0123859,0.0122179,0.0118899,0.0107734,0.0094633,0.00827175,0.00681145,0.00552306,0.00424258,0.00362803,0.00314982,0.00227839,0.00159072,0.00116389,0.000756827,0.000563174,0.000383354,0.000191677,0.000191677,0.000154132,9.48504e-05,7.11378e-05,4.54492e-05,2.76647e-05,1.58084e-05,1.97605e-05,9.88025e-06,1.38324e-05,3.9521e-06,0,0,5.92815e-06,1.97605e-06,0,0,0,0,0,0,0,0,0,0,0	
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
