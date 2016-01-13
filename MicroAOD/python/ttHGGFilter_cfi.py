import FWCore.ParameterSet.Config as cms

ttHGGFilter  = cms.EDFilter("ttHGGFilter",
                           genParticleTag = cms.InputTag( "prunedGenParticles" )
                           #genParticleTag = cms.InputTag( "flashggPrunedGenParticles" )
)
