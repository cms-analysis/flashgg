import FWCore.ParameterSet.Config as cms


process = cms.Process("FLASHggMicroAOD")




flashggMets = cms.EDProducer('FlashggMetProducer',
                             verbose = cms.untracked.bool(False),
                             metTag = cms.InputTag('slimmedMETs'),
                             )

