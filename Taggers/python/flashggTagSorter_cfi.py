import FWCore.ParameterSet.Config as cms
from flashgg.Taggers.flashggTags_cff import HTXSInputTags

flashggTagSorter = cms.EDProducer('FlashggTagSorter',
                                  DiPhotonTag = cms.InputTag('flashggPreselectedDiPhotons'),
                                  # Top of list is highest priority
                                  # Optionally can add category ranges if priority depends on category number
                                  TagPriorityRanges = cms.VPSet(
#        cms.PSet(TagName = cms.InputTag('flashggTTHDiLeptonTag')), 
        cms.PSet(TagName = cms.InputTag('flashggTTHLeptonicTag')), 
#        cms.PSet(TagName = cms.InputTag('flashggVHTightTag')),
#        cms.PSet(TagName = cms.InputTag('flashggVHLooseTag')),
        cms.PSet(TagName = cms.InputTag('flashggZHLeptonicTag')),
        cms.PSet(TagName = cms.InputTag('flashggWHLeptonicTag')),
        cms.PSet(TagName = cms.InputTag('flashggVHLeptonicLooseTag')),
        cms.PSet(TagName = cms.InputTag('flashggTTHHadronicTag')),   
        cms.PSet(TagName = cms.InputTag('flashggVBFTag')),     
#        cms.PSet(TagName = cms.InputTag('flashggVHEtTag')),
        cms.PSet(TagName = cms.InputTag('flashggVHMetTag')),
        cms.PSet(TagName = cms.InputTag('flashggVHHadronicTag')),
        cms.PSet(TagName = cms.InputTag('flashggUntagged'))
        ###                                                                 cms.PSet(TagName = cms.InputTag('flashggSigmaMoMpToMTag'))
        ),
                                  MassCutUpper=cms.double(180.),
                                  MassCutLower=cms.double(100),
                                  MinObjectWeightException = cms.double(0.1),
                                  MaxObjectWeightException = cms.double(10.),
                                  MinObjectWeightWarning = cms.double(0.4),
                                  MaxObjectWeightWarning = cms.double(2.5),
                                  StoreOtherTagInfo = cms.bool(False),
                                  BlindedSelectionPrintout = cms.bool(False),
                                  Debug = cms.untracked.bool(False),
                                  CreateNoTag = cms.bool(False),  # Placeholder for tracking rejected events
                                  HTXSTags = HTXSInputTags 
                                  )

