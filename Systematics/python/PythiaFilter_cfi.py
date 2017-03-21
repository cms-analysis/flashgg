import FWCore.ParameterSet.Config as cms

PythiaFilter = cms.EDFilter("PythiaFilter",
#                           genParticleTag = cms.InputTag( "prunedGenParticles" ),
                         genParticleTag = cms.InputTag( "flashggPrunedGenParticles" )
)
