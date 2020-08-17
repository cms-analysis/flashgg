import FWCore.ParameterSet.Config as cms

microAODZeeValidationOutputCommand = cms.untracked.vstring("drop *",
                                                           "keep *_reducedEgamma_reduced*Clusters_*",
                                                           "keep *_flashggZeeDiPhotons_*_*",
                                                           "keep *_flashggPhotons_*_*", 
                                                           "keep *_flashggDiPhotonMVA_*_*",
                                                           "drop *_flashggPrunedGenParticles_*_*",   
                                                           "keep recoGenParticles_flashggPrunedGenParticles_*_*", # this line, and preceding, drop unneded association object
                                                           "keep *_offlineSlimmedPrimaryVertices_*_*",
                                                           "keep *_reducedEgamma_*PhotonCores_*",
                                                           "keep *_fixedGridRhoAll_*_*",
                                                           "keep *_offlineBeamSpot_*_*",
                                                           "keep *_eventCount_*_*",
                                                           "keep *_weightsCount_*_*",
                                                           "keep *_generator_*_*",
                                                           "keep *_addPileupInfo_*_*",
                                                           )
