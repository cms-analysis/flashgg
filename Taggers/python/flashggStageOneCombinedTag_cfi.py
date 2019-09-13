import FWCore.ParameterSet.Config as cms
from flashgg.MicroAOD.flashggJets_cfi import UnpackedJetCollectionVInputTag

HTXSInputTags = cms.PSet(stage0cat = cms.InputTag("rivetProducerHTXS","stage0cat"), #2016
                         stage1cat = cms.InputTag("rivetProducerHTXS","stage1cat"), #2016
                         njets     = cms.InputTag("rivetProducerHTXS","njets"), #2016
                         pTH       = cms.InputTag("rivetProducerHTXS","pTH"), #2016
                         pTV       = cms.InputTag("rivetProducerHTXS","pTV"), #2016
                         ClassificationObj = cms.InputTag("rivetProducerHTXS","HiggsClassification") # 2017
                         )

flashggStageOneCombinedTag = cms.EDProducer("FlashggStageOneCombinedTagProducer",
                               DiPhotonTag=cms.InputTag('flashggPreselectedDiPhotons'),
                               SystLabel=cms.string(""),
                               MVAResultTag=cms.InputTag('flashggDiPhotonMVA'),
                               VBFMVAResultTag=cms.InputTag('flashggVBFMVA'),
                               GenParticleTag=cms.InputTag( "flashggPrunedGenParticles" ),
                               GenJetTag = cms.InputTag("slimmedGenJets"),
                               HTXSTags     = HTXSInputTags,
                               inputTagJets = UnpackedJetCollectionVInputTag
                               )
