import FWCore.ParameterSet.Config as cms

HardProcessFinalStateFilter = cms.EDFilter("HardProcessFinalStateFilter",
                                           genParticleTag = cms.InputTag( "flashggPrunedGenParticles" ),
                                           packedGenParticleTag = cms.InputTag( "flashggGenPhotons" ),
                                           genInfoTag = cms.InputTag("generator"),
                                           usePacked = cms.bool(False),
                                           debug = cms.bool(False)
)
