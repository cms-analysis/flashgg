import FWCore.ParameterSet.Config as cms

VHFilter  = cms.EDFilter("VHFilter",
#                           genParticleTag = cms.InputTag( "prunedGenParticles" ),
                         genParticleTag = cms.InputTag( "flashggPrunedGenParticles" ),
                         chooseW = cms.bool(False),
                         chooseZ = cms.bool(False)
)
