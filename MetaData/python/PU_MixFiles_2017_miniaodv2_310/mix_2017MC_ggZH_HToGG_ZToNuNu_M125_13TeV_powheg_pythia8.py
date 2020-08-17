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
		0.0108768,0.000896676,0.000710007,0.000533339,0.000706674,0.00103334,0.00137335,0.000890009,0.00241336,0.00157335,0.00166002,0.0029467,0.00546339,0.00601339,0.00892009,0.00967343,0.0131901,0.0162268,0.0197269,0.0222102,0.0282569,0.0242936,0.0253503,0.0265503,0.0273136,0.0288336,0.0293303,0.0295803,0.0292703,0.0289203,0.0280869,0.0285936,0.0292936,0.0288936,0.0292936,0.0283136,0.0269036,0.0258203,0.0243402,0.0236836,0.0220902,0.0201969,0.0179768,0.0160535,0.0149868,0.0138335,0.0123268,0.0120968,0.0116901,0.0116434,0.0119001,0.0119435,0.0116001,0.0116634,0.0117101,0.0120268,0.0118735,0.0120701,0.0112634,0.0100501,0.00923676,0.00813675,0.00667673,0.00481671,0.00827675,0.0034767,0.00315003,0.00206335,0.00160668,0.00107001,0.000710007,0.000536672,0.00156335,0.000193335,0.000160002,0.000166668,9.00009e-05,9.00009e-05,4.66671e-05,1.66668e-05,1.33335e-05,2.00002e-05,1.33335e-05,1.66668e-05,2.00002e-05,0,3.33337e-06,0,1.66668e-05,0,0,0,0,0,0.000833342,0,6.66673e-06,0,0,1.66668e-05	
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
