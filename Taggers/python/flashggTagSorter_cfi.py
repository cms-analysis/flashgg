import FWCore.ParameterSet.Config as cms

flashggTagSorter = cms.EDProducer('FlashggTagSorter',
                                  DiPhotonTag = cms.InputTag('flashggPreselectedDiPhotons'),
                                  # Top of list is highest priority
                                  # Optionally can add category ranges if priority depends on category number
                                  TagPriorityRanges = cms.VPSet(
                                                                 cms.PSet(TagName = cms.InputTag('flashggTTHLeptonicTag')), 
#                                                                cms.PSet(TagName = cms.InputTag('flashggVHTightTag')),
#                                                                cms.PSet(TagName = cms.InputTag('flashggVHLooseTag')),
#                                                                 cms.PSet(TagName = cms.InputTag('flashggVBFTag')),
#                                                                cms.PSet(TagName = cms.InputTag('flashggVHEtTag')),
                                                                 cms.PSet(TagName = cms.InputTag('flashggTTHHadronicTag')),   
                                                                 cms.PSet(TagName = cms.InputTag('flashggVBFTag')),     
#                                                                cms.PSet(TagName = cms.InputTag('flashggVHHadronicTag')),
                                                                 cms.PSet(TagName = cms.InputTag('flashggUntagged'))
                                                                ),
                                  MassCutUpper=cms.double(180.),
                                  MassCutLower=cms.double(100),
                                  MinAcceptableObjectWeight = cms.double(0.4),
                                  MaxAcceptableObjectWeight = cms.double(2.5),
                                  StoreOtherTagInfo = cms.bool(False),
                                  Debug = cms.untracked.bool(False)
                                  )

