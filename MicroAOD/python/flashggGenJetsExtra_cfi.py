import FWCore.ParameterSet.Config as cms

flashggGenJetsExtra = cms.EDProducer("FlashggGenJetExtraProducer",
                                        genJets = cms.InputTag("slimmedGenJets"),
                                    )
