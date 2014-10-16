import FWCore.ParameterSet.Config as cms

flashggPrunedGenParticles = cms.EDProducer("GenParticlePruner",
                                    src = cms.InputTag("prunedGenParticles"),
                                    select = cms.vstring("drop  *  ", # this is the default
                                                         "keep++ pdgId = 25"#
                                                         #   "drop pdgId = {Z0} & status = 2"
                                                         )
                                    )
