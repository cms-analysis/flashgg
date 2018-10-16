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
		0.0236032,0.000879644,0.00121074,0.00147196,0.00139402,0.0013712,0.00123374,0.00111447,0.00211035,0.00163401,0.0022189,0.00323464,0.0047283,0.00644268,0.00870913,0.0112322,0.014018,0.016858,0.0193041,0.0215906,0.023353,0.024585,0.0254542,0.0260677,0.0267672,0.0276715,0.0284107,0.028803,0.0286912,0.0286026,0.0284849,0.0287281,0.0286968,0.0283698,0.0281356,0.0274446,0.0262694,0.0250536,0.0242308,0.0233729,0.0216364,0.0197774,0.0179543,0.0162115,0.0146734,0.0134581,0.0123474,0.0120044,0.0117358,0.0115204,0.0117716,0.0118045,0.0117306,0.0117895,0.0116984,0.0118697,0.0117207,0.0115046,0.0110263,0.0101738,0.00910796,0.0078607,0.0064469,0.00534815,0.00421417,0.00353097,0.00309267,0.00223379,0.00167679,0.00117395,0.000888267,0.000711134,0.00045593,0.000386477,0.000361388,0.000303124,0.000233967,0.000209459,0.000217716,0.000377129,0.000485505,0.000406004,0.000230496,0.000339245,0.000175622,0.000237108,0.00013369,0.000254814,0.000264524,0.000170964,7.38479e-05,5.7139e-05,9.42942e-06,3.1754e-05,3.13011e-05,0.000212024,9.05281e-06,1.8916e-05,3.27551e-05,3.3923e-05	
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
