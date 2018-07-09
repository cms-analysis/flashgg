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
		4.53854e-05,7.89311e-06,1.18397e-05,4.14388e-05,6.70914e-05,9.27441e-05,0.000112477,0.000153916,0.000345324,0.00037295,0.000745899,0.0015135,0.00242911,0.00397418,0.00584288,0.00859954,0.0116562,0.0147956,0.0181186,0.0206582,0.0233873,0.0248416,0.0260354,0.0274601,0.0283284,0.0288612,0.029978,0.0309568,0.0311423,0.0315271,0.031312,0.031458,0.030866,0.0310969,0.0305957,0.030132,0.0283915,0.0269767,0.0267221,0.0254178,0.0238767,0.021416,0.0192868,0.0176786,0.0161868,0.0145628,0.0133571,0.0131775,0.0125126,0.0123882,0.0125481,0.0123626,0.0126191,0.0125362,0.0123843,0.0125816,0.0122915,0.0122383,0.0116621,0.0109655,0.00949147,0.00809439,0.00658285,0.00544625,0.00429583,0.00374331,0.00314935,0.00223967,0.00164769,0.00113661,0.000736033,0.000580144,0.000319671,0.000183515,0.000177595,0.000153916,0.000112477,8.48509e-05,3.94656e-05,2.17061e-05,1.57862e-05,1.38129e-05,7.89311e-06,5.91983e-06,5.91983e-06,1.97328e-06,5.91983e-06,0,0,0,0,0,1.97328e-06,0,0,0,0,0,0,0	
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
