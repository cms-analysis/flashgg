import FWCore.ParameterSet.Config as cms

flashggSlimmedPhotonAndDiPhoton = cms.EDProducer("FlashggPhotonCollectionFromDiPhotons",
                                                 DiPhotonTag=cms.InputTag("flashggPreselectedDiPhotons"))
