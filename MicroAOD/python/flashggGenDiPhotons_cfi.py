import FWCore.ParameterSet.Config as cms

flashggGenDiPhotons = cms.EDProducer('FlashggGenDiPhotonProducer',
                                     src=cms.InputTag('flashggGenPhotonsExtra')
                                     )
