import FWCore.ParameterSet.Config as cms

# This requires you to have done: git cms-merge-topic -u sethzenz:pileupjetid-for-flashgg
# process.load("RecoJets.JetProducers.PileupJetIDParams_cfi")

from RecoJets.JetProducers.PileupJetIDParams_cfi import full_53x

flashggJets = cms.EDProducer('FlashggJetProducer',
		DiPhotonTag=cms.untracked.InputTag('flashggDiPhotons'),
		VertexTag=cms.untracked.InputTag('offlineSlimmedPrimaryVertices'),
		JetTag=cms.untracked.InputTag('slimmedJets'),
		VertexCandidateMapTag = cms.InputTag("flashggVertexMapUnique"),
		PileupJetIdParameters=cms.PSet(full_53x) # from PileupJetIDParams_cfi
		)
