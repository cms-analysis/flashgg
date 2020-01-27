import FWCore.ParameterSet.Config as cms
from flashgg.MicroAOD.flashggJets_cfi import UnpackedJetCollectionVInputTag

flashggStageOneCombinedTag = cms.EDProducer("FlashggStageOneCombinedTagProducer",
                               DiPhotonTag=cms.InputTag('flashggPreselectedDiPhotons'),
                               SystLabel=cms.string(""),
                               MVAResultTag=cms.InputTag('flashggDiPhotonMVA'),
                               VBFMVAResultTag=cms.InputTag('flashggVBFMVA'),
                               inputTagJets = UnpackedJetCollectionVInputTag,
                               rawDiphoBounds = cms.vdouble(),
                               rawDijetBounds = cms.vdouble()
                               )
