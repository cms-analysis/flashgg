import FWCore.ParameterSet.Config as cms

flashggTagCandidateProducer = cms.EDProducer('FlashggTagCandidateProducer',
                                             TagSorter = cms.untracked.InputTag('flashggTagSorter'),
                                             triggerResults = cms.untracked.InputTag("TriggerResults"),
                                             pathNames = cms.untracked.vstring("HLT_Diphoton30_18_R9Id_OR_IsoCaloId_AND_HE_R9Id_Mass95_v1",
                                                                               "HLT_Diphoton30PV_18PV_R9Id_AND_IsoCaloId_AND_HE_R9Id_DoublePixelVeto_Mass55_v1",
                                                                               "HLT_Diphoton30EB_18EB_R9Id_OR_IsoCaloId_AND_HE_R9Id_DoublePixelVeto_Mass55_v1")
                             )
