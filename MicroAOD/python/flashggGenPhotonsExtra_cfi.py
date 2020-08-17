import FWCore.ParameterSet.Config as cms

flashggGenPhotonsExtra = cms.EDProducer("FlashggGenPhotonExtraProducer",
                                      genPhotons = cms.InputTag("flashggGenPhotons"),
                                      genParticles = cms.InputTag("packedGenParticles"),
                                      isoConeSize = cms.double(0.3),
                                      epsilon0 = cms.double(1.0), ## for Frixione isolation
                                      n0 = cms.double(1.0),
                                      defaultType = cms.int32(0),  ## kUnknown
                                    )

