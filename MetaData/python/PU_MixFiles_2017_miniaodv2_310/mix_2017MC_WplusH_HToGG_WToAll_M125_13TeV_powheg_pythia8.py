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
		0.00367179,0.00144779,4.38725e-05,0.000151866,0.000209238,0.000401602,0.000519721,0.000465724,0.00148492,0.00142417,0.00277409,0.0026526,0.00457962,0.006699,0.00927398,0.0101413,0.0140966,0.0166986,0.0187032,0.0218249,0.0239915,0.0249635,0.0243729,0.0260502,0.0271065,0.0278422,0.0332857,0.0285239,0.0284631,0.0309538,0.0285711,0.0280818,0.0295903,0.0293473,0.031055,0.0278422,0.0263201,0.0244235,0.0240725,0.0237755,0.0215617,0.019935,0.0191487,0.0171609,0.0152575,0.0188449,0.013398,0.0120886,0.0116026,0.0113056,0.0108736,0.0115115,0.0115958,0.0112381,0.0116127,0.0116127,0.0112685,0.0114237,0.010661,0.0100198,0.00906474,0.00766082,0.00648976,0.00502172,0.00398228,0.00343218,0.00298333,0.00204513,0.00159291,0.00107994,0.00289221,0.000502847,0.00563593,0.000199114,0.000212613,0.000172115,0.000111369,0.00266272,1.6874e-05,2.36237e-05,1.6874e-05,0.00522758,1.01244e-05,6.74962e-06,6.74962e-06,3.37481e-06,0,0.0008842,0,0,0,0,0,0,0,0,3.37481e-06,0,0,4.04977e-05	
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
