import FWCore.ParameterSet.Config as cms



flashggMets = cms.EDProducer('FlashggMetProducer',
                             verbose = cms.untracked.bool(False),
                             metTag = cms.InputTag('slimmedMETs'),
                             )

