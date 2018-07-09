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
		0.0259764,0.000471385,0.000871289,0.00170394,0.000855834,0.0011263,0.00150882,0.00161121,0.00376721,0.00161121,0.00251148,0.00332094,0.00392177,0.006667,0.00873414,0.0102275,0.0128626,0.0157392,0.0187588,0.0213321,0.0231674,0.0249042,0.0253311,0.027605,0.0265289,0.0272766,0.028484,0.0286811,0.0285226,0.0294422,0.0282541,0.0291138,0.0281169,0.0298827,0.0283488,0.0269713,0.0260073,0.0247516,0.0236349,0.0228776,0.0211544,0.0192495,0.0176364,0.0164985,0.0144217,0.0150264,0.0120358,0.0137204,0.0113306,0.0136547,0.013419,0.0110563,0.0116977,0.011574,0.011008,0.0113847,0.0113577,0.0111606,0.0107433,0.00973293,0.00875925,0.00746488,0.00609709,0.00489545,0.0038696,0.00344652,0.00287661,0.00211544,0.00148563,0.00107607,0.000693554,0.000479112,0.000359334,0.000220237,0.000168076,0.00015262,9.85271e-05,6.37528e-05,4.82976e-05,0.00408018,0.000164212,0.000135233,6.76166e-05,5.21614e-05,0.00114562,2.70467e-05,2.31829e-05,1.73871e-05,0.000496499,5.79571e-06,3.86381e-06,5.79571e-06,5.79571e-06,1.9319e-06,0,0,1.9319e-06,0,0,5.21614e-05	
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
