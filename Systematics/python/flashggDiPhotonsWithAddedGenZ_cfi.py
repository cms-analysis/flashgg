import FWCore.ParameterSet.Config as cms

flashggDiPhotonsWithAddedGenZ = cms.EDProducer('FlashggDiPhotonGenZProducer',
                                               DiPhotonTag=cms.InputTag('flashggRandomizedPerPhotonDiPhotons'),
                                               GenParticleTag=cms.InputTag( "flashggPrunedGenParticles" ))
