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
		2.81855e-05,1.05696e-05,1.76159e-05,3.34703e-05,5.6371e-05,0.000112742,9.68876e-05,0.000116265,0.000361127,0.000348795,0.000644743,0.0012384,0.00239577,0.00374515,0.00594538,0.00866528,0.0119612,0.0146071,0.0180176,0.0208308,0.0234697,0.0251503,0.0261368,0.0273963,0.028876,0.0291103,0.0303963,0.0310569,0.0305953,0.0306852,0.0313916,0.0309899,0.0305566,0.0311643,0.0310111,0.0295102,0.0288461,0.0274879,0.0263728,0.0257563,0.0236969,0.0214967,0.0196876,0.0177093,0.0162031,0.0143852,0.0135132,0.0129724,0.0127627,0.0124333,0.0124122,0.0124139,0.0122994,0.0124263,0.0123629,0.0124087,0.0123541,0.0119824,0.011778,0.01067,0.00966058,0.00824602,0.00677861,0.0053711,0.00429476,0.00367644,0.00310921,0.002394,0.00158543,0.00113447,0.000775101,0.000533763,0.000350557,0.000239577,0.000239577,0.000142689,0.000102172,6.34173e-05,4.05166e-05,4.05166e-05,1.93775e-05,1.23312e-05,1.05696e-05,5.28478e-06,1.76159e-06,1.76159e-06,5.28478e-06,1.76159e-06,1.76159e-06,0,0,0,0,0,0,0,0,0,0,0	
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
