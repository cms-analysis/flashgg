import FWCore.ParameterSet.Config as cms

flashggGenNeutrinos = cms.EDFilter("PackedGenParticleSelector",
                                    src = cms.InputTag("packedGenParticles"),
                                    cut = cms.string("( abs(pdgId) == 12 || abs(pdgId) == 14  || abs(pdgId) == 16 )")
                                    )

