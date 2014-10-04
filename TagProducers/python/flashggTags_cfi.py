import FWCore.ParameterSet.Config as cms

flashggUntaggedCategory = cms.EDProducer("UntaggedCategoryProducer",
                                         DiPhotonTag=cms.untracked.InputTag('flashggDiPhotons'),
                                         MVAResultTag=cms.untracked.InputTag('flashggDiPhotonMVA'),
                                         Boundaries=cms.untracked.vdouble(0.07,0.31,0.62,0.86,0.98)
                                         )

