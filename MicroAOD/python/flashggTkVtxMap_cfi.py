import FWCore.ParameterSet.Config as cms

flashggVertexMapUnique = cms.EDProducer('FlashggDzVertexMapProducer',
                                        PFCandidatesTag=cms.InputTag('packedPFCandidates'),
                                        VertexTag=cms.InputTag('offlineSlimmedPrimaryVertices'),
                                        MaxAllowedDz=cms.double(0.2)
                                        )

flashggVertexMapNonUnique = cms.EDProducer('FlashggDzVertexMapProducer',
                                           PFCandidatesTag=cms.InputTag('packedPFCandidates'),
                                           VertexTag=cms.InputTag('offlineSlimmedPrimaryVertices'),
                                           MaxAllowedDz=cms.double(0.2), 
                                           UseEachTrackOnce=cms.untracked.bool(False)
                                           )

#flashggVertexMapForCHSOld = cms.EDProducer('FlashggDzVertexMapProducerForCHS',
#                                        PFCandidatesTag=cms.InputTag('packedPFCandidates'),
#                                        VertexTag=cms.InputTag('offlineSlimmedPrimaryVertices'),
#                                        MaxAllowedDz=cms.double(0.1)
#                                        )

flashggVertexMapForCHS = cms.EDProducer('FlashggVertexMapFromCandidateProducer', # https://twiki.cern.ch/twiki/bin/view/CMSPublic/WorkBookMiniAOD2015#PV_Assignment
                                                   PFCandidatesTag=cms.InputTag('packedPFCandidates'),
                                                   VertexTag=cms.InputTag('offlineSlimmedPrimaryVertices'),
                                                   FromPVCut=cms.uint32(0)) # "The definition normally used for isolation calculations is fromPV() > 1; 
                                                                            # the definition used for CHS subtraction in jets is fromPV() > 0."
                                                   
