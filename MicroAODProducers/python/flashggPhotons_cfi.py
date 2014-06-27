import FWCore.ParameterSet.Config as cms

flashggPhotons = cms.EDProducer('FlashggPhotonProducer',
                                PhotonTag=cms.untracked.InputTag('slimmedPhotons'),
                                PFCandidatesTag=cms.untracked.InputTag('packedPFCandidates'), 
#                                        PhotonPreselectorName=cms.string("FlashggNoPhotonPreselector"),
                                PhotonPreselectorName=cms.string("FlashggLegacyPhotonPreselector"), 
                                reducedBarrelRecHitCollection=cms.InputTag('reducedEgamma','reducedEBRecHits'),
                                reducedEndcapRecHitCollection=cms.InputTag('reducedEgamma','reducedEERecHits'),
                                reducedPreshowerRecHitCollection=cms.InputTag('reducedEgamma','reducedESRecHits')
                                )
