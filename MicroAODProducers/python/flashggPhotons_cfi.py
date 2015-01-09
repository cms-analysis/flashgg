import FWCore.ParameterSet.Config as cms

flashggPhotons = cms.EDProducer('FlashggPhotonProducer',
                                photonTag = cms.InputTag('slimmedPhotons'),
                                pfCandidatesTag = cms.InputTag("packedPFCandidates"),
                                reducedBarrelRecHitCollection = cms.InputTag('reducedEgamma','reducedEBRecHits'),
                                reducedEndcapRecHitCollection = cms.InputTag('reducedEgamma','reducedEERecHits'),
                                reducedPreshowerRecHitCollection = cms.InputTag('reducedEgamma','reducedESRecHits'),
                                
                                vertexTag = cms.InputTag("offlineSlimmedPrimaryVertices"),
                                vertexCandidateMapTag = cms.InputTag("flashggVertexMapNonUnique"),
                                rhoFixedGridCollection = cms.InputTag('fixedGridRhoAll'),
                                
                                photonIdMVAweightfile_EB = cms.FileInPath("flashgg/MicroAODProducers/data/2013FinalPaper_PhotonID_Barrel_BDT_TrainRangePT15_8TeV.weights.xml"),
                                photonIdMVAweightfile_EE = cms.FileInPath("flashgg/MicroAODProducers/data/2013FinalPaper_PhotonID_Endcap_BDT_TrainRangePT15_8TeV.weights.xml"),
#                                regressionWeightFile = cms.FileInPath("HiggsAnalysis/GBRLikelihoodEGTools/data/regweights_v8_8TeV_forest_ph.root"),

                                useNonZsLazyTools = cms.bool(False),
                                recomputeNonZsClusterShapes = cms.bool(True),
                                doOverlapRemovalForIsolation = cms.bool(True),
                                useVtx0ForNeutralIso = cms.bool(True),
                                extraCaloIsolations = cms.VPSet(),
                                
                                genPhotonTag = cms.InputTag("flashggGenPhotonsExtra"),
                                maxGenDeltaR = cms.double(0.1),
                                copyExtraGenInfo = cms.bool(False),
                                )
