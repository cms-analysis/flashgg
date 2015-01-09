import FWCore.ParameterSet.Config as cms
from flashgg.MicroAODProducers.flashggPrunedGenParticles_cfi import flashggPrunedGenParticles
from flashgg.MicroAODProducers.flashggGenPhotons_cfi import flashggGenPhotons
from flashgg.MicroAODProducers.flashggGenPhotonsExtra_cfi import flashggGenPhotonsExtra


flashggMicroAODGenSequence = cms.Sequence(flashggPrunedGenParticles+flashggGenPhotons*flashggGenPhotonsExtra
                                        )
