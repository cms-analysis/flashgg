import FWCore.ParameterSet.Config as cms

# This requires you to have done: git cms-merge-topic -u sethzenz:pileupjetid-for-flashgg
# process.load("RecoJets.JetProducers.PileupJetIDParams_cfi")

## from RecoJets.JetProducers.PileupJetIDParams_cfi import full_53x as pu_jetid
### TMVA compatiblity issue. Running with full_53x gives the following error:
### --- <FATAL> LowPtTK                  : Option NegWeightTreatment does not have predefined value: "ignorenegweights"

from RecoJets.JetProducers.PileupJetIDParams_cfi import cutbased as pu_jetid

flashggJets = cms.EDProducer('FlashggJetProducer',
		DiPhotonTag=cms.untracked.InputTag('flashggDiPhotons'),
		VertexTag=cms.untracked.InputTag('offlineSlimmedPrimaryVertices'),
		JetTag=cms.untracked.InputTag('slimmedJets'),
		VertexCandidateMapTag = cms.InputTag("flashggVertexMapUnique"),
		PileupJetIdParameters=cms.PSet(pu_jetid) # from PileupJetIDParams_cfi
		)
