import FWCore.ParameterSet.Config as cms

from RecoJets.JetProducers.PileupJetIDParams_cfi import cutbased as pu_jetid

flashggJets = cms.EDProducer('FlashggJetProducer',
		DiPhotonTag=cms.untracked.InputTag('flashggDiPhotons'),
		VertexTag=cms.untracked.InputTag('offlineSlimmedPrimaryVertices'),
		JetTag=cms.untracked.InputTag('slimmedJets'),
		VertexCandidateMapTag = cms.InputTag("flashggVertexMapUnique"),
		PileupJetIdParameters=cms.PSet(pu_jetid),
                MinJetPt=cms.untracked.double(0.)             
		)
