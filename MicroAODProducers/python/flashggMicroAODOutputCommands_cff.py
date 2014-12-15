import FWCore.ParameterSet.Config as cms

microAODDefaultOutputCommand = cms.untracked.vstring("drop *",
                                                     "keep *_flashgg*_*_*",
                                                     "drop *_flashggVertexMap*_*_*", 
                                                     "drop *_flashggDiPhotons_*_*", # keep preselected only
                                                     "keep *_offlineSlimmedPrimaryVertices_*_*",
                                                     "keep *_reducedEgamma_reduced*Clusters_*",
                                                     "keep *_reducedEgamma_*PhotonCores_*",
                                                     "keep *_slimmedMuons_*_*",
                                                     "keep *_slimmedMETs_*_*",
                                                     "keep *_slimmedTaus_*_*",
                                                     "keep *_fixedGridRhoAll_*_*",
                                                     "keep *_offlineBeamSpot_*_*",
                                                     "keep *_TriggerResults_*_*",
                                                     "keep *_eventCount_*_*",
                                                     "keep *_packedGenParticles_*_*"
                                                     )

# Should be included for now for ongoing studies, but to be removed some day
# This string should be added to the one above if needed
microAODDebugOutputCommand = cms.untracked.vstring("keep *_reducedEgamma_*_*",
                                                   "keep *_prunedGenParticles_*_*",
                                                   "keep *_flashggDiPhotons_*_*" # STILL NEEDED
                                                   )
