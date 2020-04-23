import FWCore.ParameterSet.Config as cms

HTXSInputTags = cms.PSet(stage0bin = cms.InputTag("rivetProducerHTXS","stage0bin"), #2016
                         stage1bin = cms.InputTag("rivetProducerHTXS","stage1bin"), #2016
                         njets     = cms.InputTag("rivetProducerHTXS","njets"), #2016
                         pTH       = cms.InputTag("rivetProducerHTXS","pTH"), #2016
                         pTV       = cms.InputTag("rivetProducerHTXS","pTV"), #2016
                         ClassificationObj = cms.InputTag("rivetProducerHTXS","HiggsClassification") # 2017
                         )

flashggTagSorter = cms.EDProducer('FlashggTagSorter',
                                  DiPhotonTag = cms.InputTag('flashggPreselectedDiPhotons'),
                                  # Top of list is highest priority
                                  # Optionally can add category ranges if priority depends on category number
                                  TagPriorityRanges = cms.VPSet(
#        cms.PSet(TagName = cms.InputTag('flashggTTHDiLeptonTag')), 
        cms.PSet(TagName = cms.InputTag('flashggTTHLeptonicTag')), 
	cms.PSet(TagName = cms.InputTag('flashggTHQLeptonicTag')),
        cms.PSet(TagName = cms.InputTag('flashggZHLeptonicTag')),
        cms.PSet(TagName = cms.InputTag('flashggWHLeptonicTag')),
        cms.PSet(TagName = cms.InputTag('flashggTTHHadronicTag')),   
#        cms.PSet(TagName = cms.InputTag('flashggDoubleHTag')), # hint priority for double H tag w/ ttH veto
        cms.PSet(TagName = cms.InputTag('flashggVBFTag')),     
        cms.PSet(TagName = cms.InputTag('flashggVHMetTag')),
        cms.PSet(TagName = cms.InputTag('flashggVHHadronicTag')),
        cms.PSet(TagName = cms.InputTag('flashggUntagged'))
        ###                                                                 cms.PSet(TagName = cms.InputTag('flashggSigmaMoMpToMTag'))
        ),
                                  MassCutUpper=cms.double(180.),
                                  MassCutLower=cms.double(100),
                                  MinObjectWeightException = cms.double(0.0),
                                  MaxObjectWeightException = cms.double(10.),
                                  MinObjectWeightWarning = cms.double(0.4),
                                  MaxObjectWeightWarning = cms.double(2.5),
                                  StoreOtherTagInfo = cms.bool(False),
                                  BlindedSelectionPrintout = cms.bool(False),
                                  Debug = cms.untracked.bool(False),
                                  CreateNoTag = cms.bool(False),  # Placeholder for tracking rejected events
                                  HTXSTags = HTXSInputTags,
                                  GenParticleTag=cms.InputTag( "flashggPrunedGenParticles" ),
                                  isGluonFusion = cms.bool(False),
                                  NNLOPSWeightFile = cms.FileInPath("flashgg/Taggers/data/NNLOPS_reweight.root"),
                                  applyNNLOPSweight = cms.bool(False),
                                  SetHTXSinfo = cms.bool(True),
                                  AddTruthInfo= cms.bool(True)
                                  )

