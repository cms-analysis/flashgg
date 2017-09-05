import FWCore.ParameterSet.Config as cms

flashggGenLeptonsExtra = cms.EDProducer("FlashggGenLeptonExtraProducer",
                                        genLeptons = cms.InputTag("flashggGenLeptons"),
                                        genParticles = cms.InputTag("packedGenParticles"),
                                        isoConeSize = cms.double(0.3),
                                        dressingConeSize = cms.double(0.1),
 #                                      defaultType = cms.int32(0),  ## kUnknown
                                    )

