import FWCore.ParameterSet.Config as cms

tagDefaultOutputCommand = cms.untracked.vstring("drop *",
                                                     "keep *_flashgg*_*_*",
                                                     "drop *_flashggVertexMap*_*_*", 
                                                     "keep *_offlineSlimmedPrimaryVertices_*_*",
                                                     "keep *_reducedEgamma_reduced*Clusters_*",
                                                     "keep *_reducedEgamma_*PhotonCores_*",
                                                     "keep *_slimmedElectrons_*_*",
                                                     "keep *_slimmedMuons_*_*",
                                                     "keep *_slimmedMETs_*_*",
                                                     "keep *_slimmedTaus_*_*",
                                                     "keep *_fixedGridRhoAll_*_*",
                                                     "keep *_offlineBeamSpot_*_*",
                                                     "keep *_TriggerResults_*_*",
                                                     )
