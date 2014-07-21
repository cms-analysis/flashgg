import FWCore.ParameterSet.Config as cms

flashggDiPhotons = cms.EDProducer('FlashggDiPhotonProducer',
                                  PhotonTag=cms.untracked.InputTag('flashggPhotons'),
                                  VertexTag=cms.untracked.InputTag('offlineSlimmedPrimaryVertices'),
                                  VertexCandidateMapTag=cms.InputTag("flashggVertexMapUnique")
                                  )
