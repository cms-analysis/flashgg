import FWCore.ParameterSet.Config as cms

flashggGenLeptons = cms.EDFilter("PackedGenParticleSelector",
                                 src = cms.InputTag("packedGenParticles"),
                                 cut = cms.string("( abs(pdgId) == 13 || abs(pdgId) == 11 ) && pt > 5.")
                                 )

