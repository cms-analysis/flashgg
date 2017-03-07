import FWCore.ParameterSet.Config as cms

CentralHiggsFilter  = cms.EDFilter("CentralHiggsFilter",
                         genParticleTag = cms.InputTag( "flashggPrunedGenParticles" ),
)
