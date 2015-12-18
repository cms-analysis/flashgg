import FWCore.ParameterSet.Config as cms

flashggPrunedGenParticles = cms.EDProducer("GenParticlePruner",
                                    src = cms.InputTag("prunedGenParticles"),
                                    select = cms.vstring("drop  *  ", # this is the default
                                                         "keep++ abs(pdgId) = 6",
                                                         "drop abs(pdgId) > 25",
                                                         "keep status = 3",
                                                         "keep status = 23",
                                                         "keep status = 22",
                                                         "keep++ pdgId = 25"#
                                                         )
                                    )

