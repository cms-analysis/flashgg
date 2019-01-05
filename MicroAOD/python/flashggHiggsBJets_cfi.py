import FWCore.ParameterSet.Config as cms

flashggHiggsBJets = cms.EDProducer('FlashggHiggsGenJetsSelector',
                                   src=cms.InputTag('flashggPrunedGenParticles'),
                                   jets=cms.InputTag('slimmedGenJets'),
                                   nus=cms.InputTag('flashggGenNeutrinos'),
                                   )

