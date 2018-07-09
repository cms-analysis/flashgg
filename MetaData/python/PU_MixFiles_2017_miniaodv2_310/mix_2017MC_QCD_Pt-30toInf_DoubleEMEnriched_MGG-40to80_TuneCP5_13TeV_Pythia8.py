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
		0.0202797,0.000574306,0.00115841,0.00123214,0.00135728,0.00141826,0.00108894,0.00100848,0.00197998,0.00166498,0.00221208,0.00300029,0.0045969,0.00632978,0.00863749,0.0117192,0.0140115,0.0167799,0.0191492,0.0215483,0.023485,0.0246989,0.0254878,0.0262497,0.0270132,0.0278727,0.0284324,0.0289522,0.0287013,0.0286459,0.0286016,0.0289649,0.0289638,0.0285237,0.0284682,0.0276032,0.0264606,0.0250466,0.0244017,0.0234521,0.0216397,0.0198411,0.0180169,0.0164735,0.0147437,0.0135967,0.012308,0.0121087,0.0117471,0.0114363,0.0119246,0.0118057,0.0117314,0.0118432,0.011768,0.0120028,0.0117911,0.0115275,0.0111057,0.0102056,0.00903973,0.00787694,0.00650651,0.0052836,0.00433997,0.00357555,0.00306517,0.00229965,0.00158476,0.00115684,0.000840672,0.000812133,0.000538227,0.000287795,0.000324349,0.000276355,0.000334409,0.000179536,0.000258376,0.000398765,0.000514896,0.000301565,0.000203271,0.000257662,0.000138106,0.000356694,9.54639e-05,0.000403165,0.000287367,0.000321662,5.21318e-05,0.000197849,0.000110851,1.13919e-05,4.13582e-05,0.000296143,2.94906e-06,8.51422e-06,7.61048e-07,2.92528e-05	
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
