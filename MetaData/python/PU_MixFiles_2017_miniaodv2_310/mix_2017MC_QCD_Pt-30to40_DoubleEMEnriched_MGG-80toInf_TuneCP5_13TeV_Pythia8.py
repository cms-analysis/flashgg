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
		0.0216722,0.000899022,0.00113421,0.00141663,0.00109917,0.00137662,0.00125125,0.000910133,0.00179886,0.00175878,0.00213569,0.00308986,0.00497148,0.00688037,0.00912608,0.0115996,0.0140502,0.0170546,0.0193372,0.0217373,0.0234344,0.0246782,0.0252289,0.0262667,0.0270188,0.0276168,0.0286189,0.0286646,0.0284597,0.0283253,0.028467,0.0285636,0.0285885,0.0285886,0.0284576,0.027558,0.0265749,0.0252316,0.0240882,0.0232499,0.0216961,0.0200737,0.0177901,0.0165598,0.0146867,0.0133759,0.0124721,0.0118987,0.011918,0.0115637,0.0116364,0.0119304,0.0116655,0.0119106,0.0115299,0.0119503,0.0116921,0.011718,0.0110517,0.0101531,0.00922465,0.00783692,0.00646923,0.00514517,0.00442278,0.00353848,0.00318475,0.00222083,0.00176205,0.00111185,0.000722599,0.000685787,0.000345825,0.000262112,0.000364503,0.000195647,0.000240366,9.94596e-05,0.000160608,0.000461304,0.000272133,0.000316444,7.51912e-05,0.000350665,0.000216984,0.000177514,0.000513318,4.17198e-05,0.000455442,0.000151678,1.83376e-05,5.41949e-05,0.000123864,3.40848e-07,0.000105663,0.000379092,2.59045e-06,1.29522e-06,1.02255e-06,3.53119e-05	
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
