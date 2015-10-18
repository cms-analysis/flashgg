import FWCore.ParameterSet.Config as cms

microAODPhotonJetValidationOutputCommand = cms.untracked.vstring("drop *",
                                                                 "keep *_flashgg*_*_*",
                                                                 "drop *_flashggVertexMap*_*_*", 
                                                                 "drop *_flashggDiPhotons_*_*", # keep preselected only
                                                                 ## this part drop all the tools used to build puppi jets
                                                                 "drop *_flashggPuppi*_*_*",
                                                                 #
                                                                 ""
                                                                 "drop patPackedCandidates_*_*_*", # for intermediate PFCHSLeg jet constituents
                                                                 "drop *_flashggPrunedGenParticles_*_*",   
                                                                 "keep recoGenParticles_flashggPrunedGenParticles_*_*", # this line, and preceding, drop unneded association object
                                                                 "keep recoVertexs_offlineSlimmedPrimaryVertices_*_*", # leave out floatedmValueMap_offlineSlimmedPrimaryVertices__PAT
                                                                 "keep *_reducedEgamma_reducedSuperClusters_*",
                                                                 "keep *_reducedEgamma_*PhotonCores_*",
                                                                 "keep *_slimmedMETs_*_*",
                                                                 "keep *_fixedGridRhoAll_*_*",
                                                                 "keep *_offlineBeamSpot_*_*",
                                                                 "keep *_TriggerResults_*_*",
                                                                 "keep *_eventCount_*_*",
                                                                 "keep *_weightsCount_*_*",
                                                                 "keep *_generator_*_*",
                                                                 "keep *_slimmedGenJets_*_*",
                                                                 "keep *_addPileupInfo_*_*", # Huge - a few Validation codes use this
                                                                 
                                                                 
                                                                 )
