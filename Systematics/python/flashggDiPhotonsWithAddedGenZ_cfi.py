import FWCore.ParameterSet.Config as cms

# This producer is temporary to add new information ( vertex 3-position ) to Spring15BetaV7
# In future MicroAOD this will be stored by the ordinary DiPhotonProducer, so this module will be obsolete

flashggDiPhotonsWithAddedGenZ = cms.EDProducer('FlashggDiPhotonGenZProducer',
                                               DiPhotonTag=cms.InputTag('flashggRandomizedPerPhotonDiPhotons'),
                                               GenParticleTag=cms.InputTag( "flashggPrunedGenParticles" ))
