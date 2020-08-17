import FWCore.ParameterSet.Config as cms
from flashgg.MicroAOD.flashggTkVtxMap_cfi import flashggVertexMapUnique,flashggVertexMapNonUnique
from flashgg.MicroAOD.flashggPhotons_cfi import flashggPhotons
from flashgg.MicroAOD.flashggPhotonJet_cfi import flashggPhotonJet
from flashgg.MicroAOD.flashggMicroAODGenSequence_cff import *

flashggMicroAODPhotonJetValidationSequence = cms.Sequence(#eventCount+weightsCount
    #                                   +
    flashggVertexMapUnique+flashggVertexMapNonUnique
    +flashggMicroAODGenSequence
                                       +flashggPhotons
                                       #+flashggFinalEGamma
                                       +flashggPhotonJet
                                       )

