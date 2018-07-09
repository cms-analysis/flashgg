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
		0.0086355,0.000369232,0.00034851,0.000924963,0.000416328,0.00124898,0.00132057,0.000623549,0.00162575,0.00152779,0.00305746,0.00347568,0.00432717,0.00669892,0.00916674,0.0115856,0.0129137,0.0147222,0.0176704,0.0211913,0.0229338,0.0248139,0.0254205,0.0267825,0.0273759,0.0277112,0.0287925,0.0296572,0.0307687,0.0293068,0.029369,0.0290921,0.0296026,0.0291505,0.0290826,0.0286098,0.0279373,0.0256917,0.0256258,0.0238004,0.0219504,0.0197351,0.0183768,0.0170468,0.0153438,0.0139084,0.0124484,0.0126763,0.0153269,0.0123052,0.0122336,0.0121206,0.012162,0.0118512,0.0120452,0.0119511,0.0116308,0.0116873,0.0110619,0.0104157,0.0090556,0.00776705,0.00639374,0.00529358,0.00406155,0.00353219,0.00291806,0.00223234,0.0016427,0.0010606,0.000695135,0.000487913,0.000310833,0.000222293,0.00022606,0.000133752,0.000114914,9.60756e-05,0.000231712,0.000214757,9.41918e-06,1.1303e-05,1.88384e-06,0.00163894,1.88384e-06,3.76767e-06,0,1.88384e-06,0.00195542,0,0,1.88384e-06,0,0,0,0,3.76767e-06,3.76767e-06,0,2.2606e-05	
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
