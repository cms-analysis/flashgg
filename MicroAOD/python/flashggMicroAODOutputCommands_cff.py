import FWCore.ParameterSet.Config as cms

microAODDefaultOutputCommand = cms.untracked.vstring("drop *",
                                                     "keep *_flashgg*_*_*",
                                                     "drop *_flashggVertexMap*_*_*", 
                                                     ## this part drop all the tools used to build puppi jets
                                                     "drop *_flashggPuppi*_*_*",
                                                     "drop *_flashggPhotons_*_*", # Only keep the copies with random numbers added
                                                     #
                                                     ""
                                                     "drop patPackedCandidates_*_*_*", # for intermediate PFCHSLeg jet constituents
                                                     "drop *_flashggPrunedGenParticles_*_*",   
                                                     "keep recoGenParticles_flashggPrunedGenParticles_*_*", # this line, and preceding, drop unneded association object
                                                     "keep recoVertexs_offlineSlimmedPrimaryVertices_*_*", # leave out floatedmValueMap_offlineSlimmedPrimaryVertices__PAT
                                                     "keep *_reducedEgamma_reducedSuperClusters_*",
                                                     "keep *CaloClusters_reducedEgamma_*_*",
                                                     "keep *_reducedEgamma_*PhotonCores_*",
                                                     "keep *_*Rho*_*_*",
                                                     "keep *_offlineBeamSpot_*_*",
                                                     "keep *_TriggerResults_*_*",
                                                     "keep *_eventCount_*_*",
                                                     "keep *CaloClusters_reducedEgamma_*_*",
                                                     "keep *_weightsCount_*_*",
                                                     "keep *_generator_*_*",
                                                     "keep *_slimmedGenJets_*_*",
                                                     "keep *_flashggDiPhotons_*_*", # STILL NEEDED
                                                     "keep *_slimmedAddPileupInfo_*_*", # Was huge in old MiniAod - hopefully better now
                                                     "keep *GsfElectronCore*_reducedEgamma_*_*", # needed by at least one Tag
                                                     
                                                     "keep *_flashggSelected*_*_*",
                                                     "keep *_flashggMets*_*_*",
                                                     # Drop intermediate collections in favor of selected/final collections
                                                     "drop *_flashgg*Jet*_*_*",
                                                     "drop *_flashggMuons_*_*",
                                                     "drop *_flashggElectrons_*_*",

                                                     "keep *_flashggFinalJets_*_*",
                                                     "keep *_flashggFinalPuppiJets_*_*",
						     "drop floatedmValueMap_electronMVAValueMapProducer_*_*",
						     "drop intedmValueMap_electronMVAValueMapProducer_*_*",
						     "drop floatedmValueMap_photonMVAValueMapProducer_*_*",
                                                     "keep *_selectedPatTrigger_*_*",
                                                     "keep *_particleFlowEGammaGSFixed_dupECALClusters_*",
                                                     "keep *_ecalMultiAndGSGlobalRecHitEB_hitsNotReplaced_*",
#                                                     "keep *_slimmedJets_*_*"
                                                     "keep *_flashggGenJetsExtra_*_*"
                                                     )

# Should be included for now for ongoing studies, but to be removed some day
# This string should be added to the one above if needed
microAODDebugOutputCommand = cms.untracked.vstring("keep *_reducedEgamma_*_*",
                                                   "keep recoGenParticles_prunedGenParticles_*_*", # MiniAOD important status non-1
                                                   "keep patPackedGenParticles_packedGenParticles_*_*", # MiniAOD status 1
                                                   "keep *_slimmedTaus_*_*", # no tag uses these
                                                   "keep *_slimmedMuons_*_*", 
                                                   "keep *_slimmedElectrons_*_*",
                                                   "keep *_flashgg*Jet*_*_*",
                                                   "keep *_slimmedJets_*_*"
                                                   )

#output needed for HLT efficiency study
microAODHLTOutputCommand = cms.untracked.vstring("keep *_l1extraParticles_Isolated_*",
                                                 "keep *_l1extraParticles_NonIsolated_*",
                                                 "keep *_selectedPatTrigger_*_*",
                                                 "keep *_caloStage2Digis_EGamma_*",
                                                 "keep *_TriggerResults_*_HLT",
                                                 "keep *_slimmedPatTrigger_*_*",
                                                 "keep *_patTrigger_*_*"
                                                 )
