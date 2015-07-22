import FWCore.ParameterSet.Config as cms

flashggFinalEGamma = cms.EDProducer("FlashggEGammaMinimizer",
                                    DiPhotonTag=cms.InputTag("flashggPreselectedDiPhotons"))
