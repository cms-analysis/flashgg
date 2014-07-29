import FWCore.ParameterSet.Config as cms

flashggDiPhotons = cms.EDProducer('FlashggDiPhotonProducer',
                                  PhotonTag=cms.untracked.InputTag('flashggPhotons'),
                                  VertexTag=cms.untracked.InputTag('offlineSlimmedPrimaryVertices'),
                                  VertexSelectorName=cms.string("FlashggLegacyVertexSelector"),
                                  VertexCandidateMapTag=cms.InputTag("flashggVertexMapUnique"),
				  ConversionTag=cms.untracked.InputTag("reducedEgamma","reducedConversions")
                                  )
