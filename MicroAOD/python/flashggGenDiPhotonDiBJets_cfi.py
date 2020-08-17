import FWCore.ParameterSet.Config as cms

flashggGenDiPhotonDiBJets = cms.EDProducer('FlashggGenDiPhotonDiJetProducer',
                                     src=cms.InputTag('flashggGenPhotonsExtra'),
                                     jets=cms.InputTag('flashggHiggsBJets')
                                     )

