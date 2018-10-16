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
		0.0192159,0.000173568,0.00175237,0.000203608,0.000393865,0.000640865,0.000961298,0.000650879,0.00201272,0.00197266,0.00349806,0.0050635,0.00467965,0.00697608,0.00991005,0.0110983,0.0148033,0.0180844,0.019019,0.0213655,0.0229276,0.0256646,0.0249704,0.0259951,0.026409,0.0275973,0.027674,0.0279043,0.0280412,0.0282515,0.0285285,0.0284718,0.0281881,0.0281747,0.0279811,0.0277708,0.0262688,0.0249337,0.0237754,0.0252474,0.0215257,0.019493,0.0195297,0.0161752,0.0150403,0.0134482,0.0123667,0.0120129,0.0112519,0.0116958,0.0144595,0.0117358,0.0121464,0.0115189,0.0120129,0.0118126,0.0115189,0.0117859,0.0111951,0.0100335,0.00922579,0.00749345,0.00638862,0.00533053,0.00397537,0.00352142,0.00294731,0.00222968,0.00159883,0.00122165,0.000747676,0.000480649,0.000317095,0.000193595,0.000160216,0.000186919,0.000263689,5.34054e-05,4.00541e-05,0.00113487,0.000133514,3.67162e-05,0.000487325,1.33514e-05,3.33784e-06,1.00135e-05,0.00149869,1.00135e-05,0.00118493,0.00078773,0.000267027,3.33784e-06,0,6.67568e-06,3.33784e-06,0,6.67568e-06,3.33784e-06,0,2.33649e-05	
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
