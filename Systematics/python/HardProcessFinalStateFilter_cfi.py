import FWCore.ParameterSet.Config as cms

HardProcessFinalStateFilter = cms.EDFilter("HardProcessFinalStateFilter",
                                           genParticleTag = cms.InputTag( "flashggPrunedGenParticles" ),
                                           genInfoTag = cms.InputTag("generator")
)
