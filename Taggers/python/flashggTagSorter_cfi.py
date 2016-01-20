import FWCore.ParameterSet.Config as cms

flashggTagSorter = cms.EDProducer('FlashggTagSorter',
                                  DiPhotonTag = cms.InputTag('flashggPreselectedDiPhotons'),
                                  # Top of list is highest priority
                                  # Optionally can add category ranges if priority depends on category number
                                  TagPriorityRanges = cms.VPSet(
                                                                cms.PSet(TagName = cms.InputTag('flashggTTHLeptonicTag')), #JM put this first
                                                                cms.PSet(TagName = cms.InputTag('flashggTTHHadronicTag')), #JM put this first also for the time beeing
#                                                                cms.PSet(TagName = cms.InputTag('flashggVHTightTag')),
#                                                                cms.PSet(TagName = cms.InputTag('flashggVHLooseTag')),
#                                                                cms.PSet(TagName = cms.InputTag('flashggVHEtTag')),
#                                                               cms.PSet(TagName = cms.InputTag('flashggVBFTag'),
#                                                                         MinCategory = cms.untracked.int32(0),
#                                                                         MaxCategory = cms.untracked.int32(2)
#                                                                         ),
#                                                                cms.PSet(TagName = cms.InputTag('flashggVHHadronicTag')),
                                                                cms.PSet(TagName = cms.InputTag('flashggUntagged'),
                                                                         MinCategory = cms.untracked.int32(0),
                                                                         MaxCategory = cms.untracked.int32(4)
                                                                         )
                                                                ),
                                  massCutUpper=cms.untracked.double(180.),
                                  massCutLower=cms.untracked.double(100)
                                  )

#                                  TagVectorTag = cms.untracked.VInputTag(
#                                                                        cms.untracked.InputTag('flashggVHLooseTag'),
#                                                                        cms.untracked.InputTag('flashggVHTightTag'),
#                                                                        cms.untracked.InputTag('flashggTTHLeptonicTag'),
#                                                                        cms.untracked.InputTag('flashggTTHHadronicTag'),
#                                                                        cms.untracked.InputTag('flashggVBFTag'),
#                                                                        cms.untracked.InputTag('flashggVHHadronicTag'),
#                                                                        cms.untracked.InputTag('flashggUntagged'),

