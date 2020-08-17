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
		0.0186132,0.000399219,0.00124014,0.0013149,0.00118849,0.00138384,0.00113352,0.00121015,0.00231722,0.00178472,0.00229306,0.00312774,0.00435684,0.00657119,0.00870848,0.0110405,0.0137307,0.0163249,0.0192129,0.0217532,0.0235267,0.0244565,0.0253839,0.0260153,0.0268241,0.0280434,0.029164,0.0292138,0.0287353,0.0289874,0.0284372,0.0291111,0.0289168,0.0284458,0.0281134,0.0273704,0.0267777,0.0250286,0.0246389,0.02363,0.0219221,0.0196755,0.0179232,0.0163309,0.0146499,0.0138694,0.0124274,0.0119393,0.0115549,0.0119637,0.0119622,0.0120341,0.011665,0.012052,0.0116119,0.0119076,0.0116807,0.0117792,0.0111979,0.0104097,0.00903356,0.00768429,0.00668635,0.00526191,0.0042346,0.00341179,0.00292782,0.00248153,0.00167935,0.00139217,0.00102127,0.000805727,0.00072326,0.000235117,0.000494599,0.000481479,0.000421502,6.12261e-05,4.18587e-05,0.000995445,0.000616218,0.000109541,0.000330288,5.16465e-05,0.000327789,0.000326748,7.70533e-05,2.04087e-05,0.00094359,6.24756e-07,6.24756e-07,4.16504e-07,4.16504e-07,4.16504e-07,0,1.04126e-06,2.70728e-06,6.24756e-07,6.24756e-07,3.26956e-05	
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
