import FWCore.ParameterSet.Config as cms

flashggMuons = cms.EDProducer('FlashggMuonProducer',
		muonTag = cms.untracked.InputTag('slimmedMuons'),
		vertexTag = cms.untracked.InputTag('offlineSlimmedPrimaryVertices')
		)
