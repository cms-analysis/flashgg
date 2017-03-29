import FWCore.ParameterSet.Config as cms

PythiaFilter = cms.EDFilter("PythiaFilter",
                            genParticleTag = cms.InputTag( "flashggPrunedGenParticles" ),
                            packedGenParticleTag = cms.InputTag( "flashggGenPhotons" ),
                            #usePacked = cms.bool(False)
                            usePacked = cms.bool(True)
)
