import FWCore.ParameterSet.Config as cms

flashggVertexMapUnique = cms.EDProducer('FlashggDzVertexMapProducer',
                                        PFCandidatesTag=cms.untracked.InputTag('packedPFCandidates'),
                                        VertexTag=cms.untracked.InputTag('offlineSlimmedPrimaryVertices'),
                                        MaxAllowedDz=cms.double(0.2)
                                        )

flashggVertexMapNonUnique = cms.EDProducer('FlashggDzVertexMapProducer',
                                           PFCandidatesTag=cms.untracked.InputTag('packedPFCandidates'),
                                           VertexTag=cms.untracked.InputTag('offlineSlimmedPrimaryVertices'),
                                           MaxAllowedDz=cms.double(0.2), 
                                           UseEachTrackOnce=cms.untracked.bool(False)
                                           )

flashggVertexMapForCHS = cms.EDProducer('FlashggDzVertexMapProducerForCHS',
                                        PFCandidatesTag=cms.untracked.InputTag('packedPFCandidates'),
                                        VertexTag=cms.untracked.InputTag('offlineSlimmedPrimaryVertices'),
                                        MaxAllowedDz=cms.double(0.1)
                                        )
