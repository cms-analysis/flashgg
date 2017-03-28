import FWCore.ParameterSet.Config as cms

flashggTagAndProbe = cms.EDProducer('FlashggTagAndProbeProducer',
                                    diphotonsSrc   = cms.InputTag('flashggDiPhotons'),
                                    maxDiphotons   = cms.int(-1),
                                    tagSelection   = cms.string(""),
                                    probeSelection = cms.string("")
)
