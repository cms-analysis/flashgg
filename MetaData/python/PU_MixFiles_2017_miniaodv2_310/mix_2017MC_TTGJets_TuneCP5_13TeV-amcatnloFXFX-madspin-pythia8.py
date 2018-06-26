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
		0.0278596,0.00101682,0.00105943,0.0018753,0.00101271,0.00172541,0.0012807,0.000899583,0.00221835,0.00118531,0.00175158,0.00303812,0.00481393,0.00559195,0.0080982,0.0110275,0.013738,0.01685,0.0192542,0.0217038,0.023724,0.0250369,0.0259242,0.0264433,0.0267145,0.0272931,0.0283454,0.0285972,0.0285827,0.0284765,0.0281609,0.0285144,0.0285384,0.028188,0.0280614,0.0271653,0.0261463,0.025173,0.0240703,0.0229674,0.0224227,0.0196396,0.017542,0.0161205,0.0148354,0.013271,0.0120303,0.0118324,0.0112773,0.0117458,0.0114445,0.0123896,0.0118239,0.0118934,0.0117515,0.0115616,0.0120288,0.0113217,0.0109311,0.0101011,0.00896036,0.00794138,0.00627848,0.00541156,0.00455242,0.00373633,0.00294944,0.00287136,0.00154156,0.0010644,0.000747526,0.00057492,0.000507868,0.000218028,0.000696696,0.00022906,0.000284648,0.000431515,0.000599578,0.000405775,0.00013324,0.000385443,0.000752284,1.55735e-05,1.62224e-05,8.00303e-06,5.84005e-06,0.000138863,2.59558e-06,6.48894e-07,0.000130644,1.66549e-05,0.000240956,1.47083e-05,8.65192e-07,2.16298e-07,3.24447e-06,0,1.16801e-05,3.15795e-05	
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
