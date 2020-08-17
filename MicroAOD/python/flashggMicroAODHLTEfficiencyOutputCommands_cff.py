import FWCore.ParameterSet.Config as cms

microAODHLTEfficiencyOutputCommand = cms.untracked.vstring("drop *",
                                                           "keep *_flashggHLTDiPhotons_*_*",
                                                           "keep *_flashggDiPhotonMVA_*_*",
                                                           "keep *_l1extraParticles_Isolated_*",
                                                           "keep *_l1extraParticles_NonIsolated_*",
                                                           "keep *_flashggElectrons_*_*",      
                                                           "keep *_offlineSlimmedPrimaryVertices_*_*",
                                                           "keep *_TriggerResults_*_HLT",
                                                           "keep *_eventCount_*_*",
                                                           "keep *_weightsCount_*_*",
                                                           "keep *_addPileupInfo_*_*",
                                                           "keep *_selectedPatTrigger_*_*",
                                                           "keep *_patTrigger_*_*",
                                                           )
