import FWCore.ParameterSet.Config as cms

flashggTagSorter = cms.EDProducer('FlashggTagSorter',
                                  DiPhotonTag = cms.untracked.InputTag('flashggDiPhotons'),
                                  TagVectorTag = cms.untracked.VInputTag(
        								cms.untracked.InputTag('flashggVHlooseTag'),
                                                                        cms.untracked.InputTag('flashggVHtightTag'),
                                                                        cms.untracked.InputTag('flashggTTHleptonicTag'),
                                                                        cms.untracked.InputTag('flashggTTHhadronicTag'),
                                                                        cms.untracked.InputTag('flashggVBFTag'),
                                                                        cms.untracked.InputTag('flashggUntaggedCategory'),
                                                                        ),
                                  massCutUpper=cms.untracked.double(180.),
                                  massCutLower=cms.untracked.double(100)
                                  )
