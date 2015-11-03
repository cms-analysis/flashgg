import FWCore.ParameterSet.Config as cms
from flashgg.MicroAOD.flashggPrunedGenParticles_cfi import flashggPrunedGenParticles
from flashgg.MicroAOD.flashggGenPhotons_cfi import flashggGenPhotons
from flashgg.MicroAOD.flashggGenPhotonsExtra_cfi import flashggGenPhotonsExtra
from flashgg.MicroAOD.flashggPDFWeightObject_cfi import flashggPDFWeightObject

flashggMicroAODGenSequence = cms.Sequence(flashggPrunedGenParticles+flashggGenPhotons*flashggGenPhotonsExtra+flashggPDFWeightObject
                                        )
