import FWCore.ParameterSet.Config as cms

flashggPrunedGenParticles = cms.EDProducer("GenParticlePruner",
                                    src = cms.InputTag("prunedGenParticles"),
                                    select = cms.vstring("drop  *  ", # this is the default
                                                         "keep++ abs(pdgId) = 6",
                                                         "keep++ pdgId = 23", #save Z descendants
                                                         "keep++ abs(pdgId) = 24", #save W descendants
                                                         "drop abs(pdgId) > 25",
                                                         "keep status = 3",
                                                         "keep status = 23",
                                                         "keep status = 22",
                                                         "keep++ pdgId = 25",
                                                         "++keep pdgId == 22 && status == 1 && (pt > 10 || isPromptFinalState())", # keep gamma above 10 GeV (or all prompt) and its parents
                                                         "keep++ abs(pdgId) = 5"# keep b quarks
                                                         )
                                    )

