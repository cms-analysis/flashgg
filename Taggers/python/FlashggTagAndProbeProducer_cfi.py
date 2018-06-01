import FWCore.ParameterSet.Config as cms

flashggTagAndProbe = cms.EDProducer('FlashggTagAndProbeProducer',
                                    diphotonsSrc    = cms.InputTag('flashggDiPhotons'),
                                    genParticlesSrc = cms.InputTag('flashggPrunedGenParticles'),
                                    maxDiphotons    = cms.int32(-1),
                                    tagSelection    = cms.string("1"),
                                    probeSelection  = cms.string("1"),
                                    idSelection     = cms.PSet()
)
