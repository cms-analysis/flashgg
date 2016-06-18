import FWCore.ParameterSet.Config as cms

flashggGenPhotons = cms.EDFilter("PackedGenParticleSelector",
                                    src = cms.InputTag("packedGenParticles"),
                                   cut = cms.string("pdgId == 22 && pt > 5.")
                                    )

