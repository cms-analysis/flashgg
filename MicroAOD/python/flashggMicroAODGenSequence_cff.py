import FWCore.ParameterSet.Config as cms
from flashgg.MicroAOD.flashggPrunedGenParticles_cfi import flashggPrunedGenParticles
from flashgg.MicroAOD.flashggGenPhotons_cfi import flashggGenPhotons
from flashgg.MicroAOD.flashggGenNeutrinos_cfi import flashggGenNeutrinos
from flashgg.MicroAOD.flashggGenPhotonsExtra_cfi import flashggGenPhotonsExtra


flashggMicroAODGenSequence = cms.Sequence(flashggPrunedGenParticles+flashggGenPhotons*flashggGenPhotonsExtra+flashggGenNeutrinos
                                        )
