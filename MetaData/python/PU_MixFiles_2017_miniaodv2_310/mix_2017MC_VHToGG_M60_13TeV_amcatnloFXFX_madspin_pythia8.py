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
		0.0296462,0.000840439,0.001041,0.00186393,0.00116197,0.000593719,0.000646247,0.000580985,0.00112536,0.00134025,0.00234782,0.00339041,0.00466858,0.00759738,0.00853491,0.011599,0.0134805,0.016968,0.0202358,0.0225709,0.0235212,0.0245128,0.0256939,0.0254249,0.0257146,0.026555,0.0274273,0.0280544,0.0280512,0.0281022,0.0284428,0.0280019,0.0277759,0.0282056,0.0282757,0.0265614,0.0261937,0.0244635,0.0238506,0.0232044,0.0224754,0.0204682,0.0173341,0.0159015,0.0163997,0.0131892,0.0118219,0.0115879,0.0112759,0.0110865,0.0115735,0.0126225,0.0113603,0.0116308,0.0114621,0.0128979,0.0114239,0.0113523,0.0109225,0.0101649,0.00894558,0.00768333,0.00642108,0.00500284,0.00743661,0.00330445,0.00297656,0.00214407,0.00149942,0.00102508,0.000732201,0.000756077,0.00164427,0.000230802,0.000211702,0.000149624,8.91375e-05,6.52614e-05,5.09357e-05,3.50183e-05,1.91009e-05,9.55044e-06,3.18348e-06,7.9587e-06,7.9587e-06,3.18348e-06,0,1.59174e-06,0,0.00246242,0,0,0,0,0,0.00183528,0,3.18348e-06,0,2.54679e-05	
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
