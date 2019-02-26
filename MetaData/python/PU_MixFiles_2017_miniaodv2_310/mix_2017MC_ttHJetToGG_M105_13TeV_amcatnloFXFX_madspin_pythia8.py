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
                4.1277e-05, 4.85611e-06, 1.21403e-05, 2.67086e-05, 4.85611e-05, 9.71223e-05, 0.000128687, 0.000148111, 0.000378777, 0.000354496, 0.000735701, 0.00125045, 0.00228237, 0.00411798, 0.0059366, 0.00845449, 0.0115745, 0.0147286, 0.0179798, 0.0207162, 0.0229694, 0.0251255, 0.0263396, 0.0273351, 0.0279979, 0.0292775, 0.0304745, 0.030982, 0.0303143, 0.0314409, 0.0311204, 0.0312467, 0.0315599, 0.0312224, 0.0303167, 0.029734, 0.0283281, 0.0277236, 0.0266552, 0.0256743, 0.0238265, 0.0216388, 0.0191039, 0.0176423, 0.0164671, 0.0144712, 0.0134247, 0.0129245, 0.0121573, 0.0122665, 0.0126113, 0.0123491, 0.0124122, 0.0122665, 0.0123637, 0.0124729, 0.0125263, 0.0122204, 0.0118246, 0.0111521, 0.00979478, 0.00835251, 0.00670144, 0.00547041, 0.00424667, 0.00373921, 0.00305207, 0.00228966, 0.00164865, 0.00118489, 0.000803687, 0.000534172, 0.00034964, 0.000223381, 0.000196673, 0.000172392, 0.000106834, 6.07014e-05, 3.88489e-05, 1.94245e-05, 1.45683e-05, 7.28417e-06, 4.85611e-06, 2.42806e-06, 0, 2.42806e-06, 2.42806e-06, 2.42806e-06, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
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
