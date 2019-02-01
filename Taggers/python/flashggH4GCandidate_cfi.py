import FWCore.ParameterSet.Config as cms
from h4gCandidateDumper_cfi import h4gCandidateDumper
from flashggH4GTag_cfi import flashggH4GTag


FlashggH4GCandidate = cms.EDProducer("FlashggH4GCandidateProducer",
                                     PhotonTag              = cms.InputTag('flashggRandomizedPhotons'),
                                     DiPhotonTag            = cms.InputTag('flashggDiPhotons'),
                                     VertexTag              = cms.InputTag('offlineSlimmedPrimaryVertices'),
                                     GenParticleTag         = cms.InputTag('flashggPrunedGenParticles')
                                     )
flashggH4GTagSequence = cms.Sequence( flashggH4GTag )
