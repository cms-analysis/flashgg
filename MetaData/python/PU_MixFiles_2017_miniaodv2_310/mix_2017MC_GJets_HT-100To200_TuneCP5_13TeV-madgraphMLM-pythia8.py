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
		0.0236002,0.000753793,0.0013405,0.00157767,0.00141531,0.00153229,0.00167537,0.000863945,0.00171092,0.00145909,0.0024374,0.00347857,0.00435878,0.00635648,0.008578,0.0111487,0.0141087,0.0164026,0.019199,0.0216426,0.0234021,0.0244984,0.025371,0.0257651,0.0270296,0.0277048,0.0282318,0.028643,0.0287896,0.0285029,0.0284081,0.028635,0.0285441,0.0282381,0.0279959,0.0272475,0.0261147,0.0248192,0.0242657,0.0230453,0.0216681,0.0196812,0.0179994,0.0161314,0.0147353,0.0133975,0.0121355,0.0123413,0.0114353,0.0113096,0.0121696,0.0114834,0.011806,0.0121468,0.0119295,0.0118373,0.0120931,0.0113473,0.0110442,0.0104416,0.00897322,0.00778956,0.00641874,0.00554877,0.00457286,0.00341742,0.00327454,0.0025099,0.00168301,0.00129933,0.00079697,0.00073602,0.00109188,0.000383072,0.00035094,0.000499449,0.000160057,0.000324431,0.00016337,9.88054e-05,0.000652276,0.000255147,0.000499449,0.000318607,0.000209459,0.000572449,2.92199e-05,0.000340196,7.43049e-06,8.37436e-05,1.07441e-05,1.00412e-06,0.000120896,8.08317e-05,0.000292099,0.000210765,4.51854e-06,1.10453e-06,0.000156241,3.61483e-05	
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
