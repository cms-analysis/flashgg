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
                3.1718e-05, 0, 1.46391e-05, 3.1718e-05, 4.39172e-05, 8.53946e-05, 0.000104913, 0.000129312, 0.000373296, 0.00031962, 0.000714875, 0.00134435, 0.00245448, 0.00368661, 0.00602642, 0.00841259, 0.0114941, 0.014744, 0.0181427, 0.02089, 0.0235518, 0.0244643, 0.0261551, 0.0278118, 0.0288634, 0.0297076, 0.0302882, 0.0314252, 0.0306908, 0.0308982, 0.0313447, 0.0312227, 0.03123, 0.03123, 0.0304712, 0.0299637, 0.0287292, 0.0270091, 0.0264333, 0.0253402, 0.0231053, 0.0211803, 0.0195163, 0.0176254, 0.0162347, 0.0146635, 0.0131288, 0.0129092, 0.012492, 0.0124286, 0.0122993, 0.0126506, 0.0123944, 0.0124017, 0.0125603, 0.0126408, 0.0123261, 0.0122431, 0.0116307, 0.0107304, 0.00967399, 0.00839551, 0.00679497, 0.00532618, 0.00417946, 0.0035085, 0.00319132, 0.00229101, 0.00168349, 0.00126872, 0.000768551, 0.000556285, 0.000402575, 0.000241545, 0.000173229, 0.000182988, 9.5154e-05, 6.83157e-05, 5.85563e-05, 2.92781e-05, 2.92781e-05, 9.75938e-06, 4.87969e-06, 7.31954e-06, 9.75938e-06, 7.31954e-06, 0, 0, 2.43985e-06, 0, 0, 0, 2.43985e-06, 0, 0, 0, 0, 0, 0, 0
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
