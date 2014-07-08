import FWCore.ParameterSet.Config as cms

flashggPhotons = cms.EDProducer('FlashggPhotonProducer',
                                PhotonTag = cms.untracked.InputTag('slimmedPhotons'),
                                PFCandidatesTag = cms.untracked.InputTag('packedPFCandidates'), 
#                                        PhotonPreselectorName=cms.string("FlashggNoPhotonPreselector"),
                                PhotonPreselectorName = cms.string("FlashggLegacyPhotonPreselector"), 
                                reducedBarrelRecHitCollection = cms.InputTag('reducedEgamma','reducedEBRecHits'),
                                reducedEndcapRecHitCollection = cms.InputTag('reducedEgamma','reducedEERecHits'),
                                reducedPreshowerRecHitCollection = cms.InputTag('reducedEgamma','reducedESRecHits'),
                                VertexCandidateMapTag = cms.InputTag("flashggVertexMapNonUnique"),
                                rhoFixedGridCollection = cms.InputTag('fixedGridRhoAll'),
                                photonIdMVAweightfile_EB = cms.string("/afs/cern.ch/work/f/favaro/private/miniAODforHgg/CMSSW_7_0_4/src/flashGGAnalyzers/flashggCommissioning/python/inputs/mvaDiscriminants/2013FinalPaper_PhotonID_Barrel_BDT_TrainRangePT15_8TeV.weights.xml"),
                                photonIdMVAweightfile_EE = cms.string("/afs/cern.ch/work/f/favaro/private/miniAODforHgg/CMSSW_7_0_4/src/flashGGAnalyzers/flashggCommissioning/python/inputs/mvaDiscriminants/2013FinalPaper_PhotonID_Endcap_BDT_TrainRangePT15_8TeV.weights.xml")
                                )
