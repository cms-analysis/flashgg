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
		0.0346829,0.000486225,0.000808017,0.00133137,0.000523355,0.000809785,0.0012359,0.00374305,0.00100604,0.000956537,0.00196965,0.00241521,0.00400826,0.00648889,0.00806426,0.0130609,0.0136886,0.016369,0.0202765,0.0222037,0.0229551,0.0247073,0.0254004,0.0262721,0.0270518,0.02798,0.027805,0.0283725,0.0278704,0.0284503,0.0307842,0.0281268,0.0279058,0.0279234,0.0269952,0.0291328,0.0258672,0.0241044,0.0251422,0.0257381,0.0211499,0.0194066,0.0175589,0.0158262,0.0145125,0.0131422,0.0118179,0.0120336,0.0111213,0.0115421,0.01104,0.0113282,0.0113087,0.0114767,0.0115739,0.0114696,0.0112963,0.0113635,0.0107641,0.00990307,0.00894477,0.00766644,0.00628556,0.00488523,0.00367586,0.00346015,0.00284486,0.00216945,0.00147989,0.00108384,0.000677179,0.00046324,0.000344778,0.000259909,0.000198026,0.000146751,0.000104317,5.48108e-05,4.77384e-05,2.47533e-05,0.00275822,1.41447e-05,8.84045e-06,7.07236e-06,5.30427e-06,1.76809e-06,0,0,1.06085e-05,0,0,0,0,0,0,0,3.53618e-06,1.76809e-06,0,2.29852e-05	
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
