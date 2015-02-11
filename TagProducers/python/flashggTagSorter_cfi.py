import FWCore.ParameterSet.Config as cms

flashggTagSorter = cms.EDProducer('FlashggTagSorter',
                                  DiPhotonTag = cms.untracked.InputTag('flashggDiPhotons'),
                                  # Top of list is highest priority
                                  # Optionally can add category ranges if priority depends on category number
                                  TagPriorityRanges = cms.VPSet(cms.PSet(TagName = cms.InputTag('flashggVHtightTag')),
                                                                cms.PSet(TagName = cms.InputTag('flashggVHlooseTag')),
                                                                cms.PSet(TagName = cms.InputTag('flashggTTHleptonicTag')),
                                                                cms.PSet(TagName = cms.InputTag('flashggTTHhadronicTag')),
                                                                cms.PSet(TagName = cms.InputTag('flashggVBFTag'),
                                                                         MinCategory = cms.untracked.int32(0),
                                                                         MaxCategory = cms.untracked.int32(0)
                                                                         ),
                                                                cms.PSet(TagName = cms.InputTag('flashggUntaggedCategory'),
                                                                         MinCategory = cms.untracked.int32(0),
                                                                         MaxCategory = cms.untracked.int32(1)
                                                                         ),
                                                                cms.PSet(TagName = cms.InputTag('flashggVBFTag'),
                                                                         MinCategory = cms.untracked.int32(1),
                                                                         MaxCategory = cms.untracked.int32(2)
                                                                         ),
                                                                cms.PSet(TagName = cms.InputTag('flashggUntaggedCategory'),
                                                                         MinCategory = cms.untracked.int32(3),
                                                                         MaxCategory = cms.untracked.int32(4)
                                                                         )
                                                                ),
                                  massCutUpper=cms.untracked.double(180.),
                                  massCutLower=cms.untracked.double(100)
                                  )

#                                  TagVectorTag = cms.untracked.VInputTag(
#                                                                        cms.untracked.InputTag('flashggVHlooseTag'),
#                                                                        cms.untracked.InputTag('flashggVHtightTag'),
#                                                                        cms.untracked.InputTag('flashggTTHleptonicTag'),
#                                                                        cms.untracked.InputTag('flashggTTHhadronicTag'),
#                                                                        cms.untracked.InputTag('flashggVBFTag'),
#                                                                        cms.untracked.InputTag('flashggVHhadronicTag'),
#                                                                        cms.untracked.InputTag('flashggUntaggedCategory'),

