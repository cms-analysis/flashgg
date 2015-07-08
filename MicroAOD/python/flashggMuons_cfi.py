import FWCore.ParameterSet.Config as cms

flashggMuons = cms.EDProducer('FlashggMuonProducer',
		muonTag = cms.InputTag('slimmedMuons'),
		)

