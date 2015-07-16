import FWCore.ParameterSet.Config as cms

microAODDefaultOutputCommand = cms.untracked.vstring("drop *",
                                                     "keep *_flashgg*_*_*",
                                                     "drop *_flashggVertexMap*_*_*", 
                                                     "drop *_flashggDiPhotons_*_*", # keep preselected only
                                                     "drop patPackedCandidates_*_*_*", # for intermediate PFCHSLeg jet constituents
                                                     "drop *_flashggPrunedGenParticles_*_*",   
                                                     "keep recoGenParticles_flashggPrunedGenParticles_*_*", # this line, and preceding, drop unneded association object
                                                     "keep *_offlineSlimmedPrimaryVertices_*_*",
                                                     "keep *_reducedEgamma_reduced*Clusters_*",
                                                     "keep *_reducedEgamma_*PhotonCores_*",
                                                     "keep *_slimmedMETs_*_*",
                                                     "keep *_fixedGridRhoAll_*_*",
                                                     "keep *_offlineBeamSpot_*_*",
                                                     "keep *_TriggerResults_*_*",
                                                     "keep *_eventCount_*_*",
                                                     "keep *_weightsCount_*_*",
                                                     "keep *_generator_*_*",
                                                     "keep *_slimmedGenJets_*_*",
                                                     "keep *_flashggDiPhotons_*_*", # STILL NEEDED
                                                     "keep *_addPileupInfo_*_*", # Huge - a few Validation codes use this
                                                     "keep *GsfElectronCore*_*_*_*" # needed by at least one Tag
                                                     )

# Should be included for now for ongoing studies, but to be removed some day
# This string should be added to the one above if needed
microAODDebugOutputCommand = cms.untracked.vstring("keep *_reducedEgamma_*_*",
                                                   "keep recoGenParticles_prunedGenParticles_*_*", # MiniAOD important status non-1
                                                   "keep patPackedGenParticles_packedGenParticles_*_*", # MiniAOD status 1
                                                   "keep *_slimmedTaus_*_*", # no tag uses these
                                                   "keep *_slimmedMuons_*_*", ## flashggMuons exist insted
                                                   )
