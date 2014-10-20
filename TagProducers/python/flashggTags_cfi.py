import FWCore.ParameterSet.Config as cms

flashggUntaggedCategory = cms.EDProducer("FlashggUntaggedCategoryProducer",
#                                         DiPhotonTag=cms.untracked.InputTag('flashggPreselectedDiPhotons'), # why doesn't this work?
                                         DiPhotonTag=cms.untracked.InputTag('flashggDiPhotons'),
                                         MVAResultTag=cms.untracked.InputTag('flashggDiPhotonMVA'),
                                         Boundaries=cms.untracked.vdouble(0.07,0.31,0.62,0.86,0.98)
                                         )

flashggVBFTag = cms.EDProducer("FlashggVBFTagProducer",
#                                         DiPhotonTag=cms.untracked.InputTag('flashggPreselectedDiPhotons'), # why doesn't this work?
                                         DiPhotonTag=cms.untracked.InputTag('flashggDiPhotons'),
                                         VBFDiPhoDiJetMVAResultTag=cms.untracked.InputTag('flashggVBFDiPhoDiJetMVA'),
                                         VBFMVAResultTag=cms.untracked.InputTag('flashggVBFMVA'),
                                         Boundaries=cms.untracked.vdouble(0.21,0.6,0.81)
                                         )
