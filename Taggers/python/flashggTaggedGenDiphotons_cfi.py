import FWCore.ParameterSet.Config as cms

flashggTaggedGenDiphotons = cms.EDProducer('FlashggTaggedGenDiPhotonProducer',
                                           src=cms.InputTag("flashggGenDiPhotons"),
                                           tags=cms.InputTag("flashggTagSorter")
                                  )
