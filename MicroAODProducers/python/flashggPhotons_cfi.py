import FWCore.ParameterSet.Config as cms

flashggPhotons = cms.EDProducer('FlashggPhotonProducer',
                                PhotonTag = cms.untracked.InputTag('slimmedPhotons'),
                                reducedBarrelRecHitCollection = cms.InputTag('reducedEgamma','reducedEBRecHits'),
                                reducedEndcapRecHitCollection = cms.InputTag('reducedEgamma','reducedEERecHits'),
                                reducedPreshowerRecHitCollection = cms.InputTag('reducedEgamma','reducedESRecHits'),
                                VertexCandidateMapTag = cms.InputTag("flashggVertexMapNonUnique"),
                                rhoFixedGridCollection = cms.InputTag('fixedGridRhoAll'),
                                photonIdMVAweightfile_EB = cms.string("../data/2013FinalPaper_PhotonID_Barrel_BDT_TrainRangePT15_8TeV.weights.xml"),
                                photonIdMVAweightfile_EE = cms.string("../data/2013FinalPaper_PhotonID_Endcap_BDT_TrainRangePT15_8TeV.weights.xml")
                                )
