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
		0.0515731,0.00143944,0.00106507,0.00049916,0.000937376,0.00109119,0.00151199,0.00161647,0.00342447,0.000815488,0.00161066,0.00286727,0.00507286,0.00861921,0.0100964,0.011916,0.0142435,0.0160805,0.0182077,0.0223229,0.0228017,0.0241019,0.0244124,0.0244995,0.0259041,0.0262668,0.0271026,0.0277991,0.0274509,0.0282519,0.0276976,0.0280923,0.0277556,0.0274277,0.0273493,0.0266818,0.0255094,0.0245604,0.0235795,0.0227959,0.0208951,0.0187823,0.0169917,0.0154536,0.0140374,0.0128476,0.0126792,0.0113646,0.0112804,0.0113762,0.0114604,0.0115619,0.0117041,0.0115619,0.0114023,0.0113791,0.0117245,0.0116113,0.0106536,0.0101109,0.00914449,0.00800977,0.00623369,0.00519765,0.00392943,0.00345059,0.0029137,0.00210982,0.00144234,0.0011144,0.000728425,0.00051367,0.000359859,0.000243776,0.000153811,0.000136398,0.000116084,6.67481e-05,5.22377e-05,1.74126e-05,5.80418e-06,0,1.45105e-05,1.45105e-05,0,5.80418e-06,5.80418e-06,1.74126e-05,4.35314e-05,0,0,0,0,2.90209e-06,0,0,0,0,2.90209e-06,2.32167e-05	
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
