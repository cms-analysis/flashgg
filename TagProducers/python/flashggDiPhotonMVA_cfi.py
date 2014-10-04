import FWCore.ParameterSet.Config as cms

flashggDiPhotonMVA = cms.EDProducer('FlashggDiPhotonMVAProducer',
                                    # DiPhotonTag=cms.untracked.InputTag('flashggPreselectedDiPhotons'), # why doesn't this work?
                                    DiPhotonTag=cms.untracked.InputTag('flashggDiPhotons'),
                                    BeamSpotTag=cms.untracked.InputTag('offlineBeamSpot'),
                                    diphotonMVAweightfile = cms.FileInPath("flashgg/TagProducers/data/HggBambu_SMDipho_Oct29_rwgtptallsigevenbkg7TeV_BDTG.weights.xml")
                                            )
