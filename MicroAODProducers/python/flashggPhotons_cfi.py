import FWCore.ParameterSet.Config as cms
import os

flashggPhotons = cms.EDProducer('FlashggPhotonProducer',
                                PhotonTag = cms.untracked.InputTag('slimmedPhotons'),
                                reducedBarrelRecHitCollection = cms.InputTag('reducedEgamma','reducedEBRecHits'),
                                reducedEndcapRecHitCollection = cms.InputTag('reducedEgamma','reducedEERecHits'),
                                reducedPreshowerRecHitCollection = cms.InputTag('reducedEgamma','reducedESRecHits'),
                                VertexCandidateMapTag = cms.InputTag("flashggVertexMapNonUnique"),
                                rhoFixedGridCollection = cms.InputTag('fixedGridRhoAll'),
                                photonIdMVAweightfile_EB = cms.string( ("%s/%s") % ( os.getenv('CMSSW_BASE', '.') , "src/flashgg/MicroAODProducers/data/2013FinalPaper_PhotonID_Barrel_BDT_TrainRangePT15_8TeV.weights.xml") ),
                                photonIdMVAweightfile_EE = cms.string( ("%s/%s") % ( os.getenv('CMSSW_BASE', '.') , "src/flashgg/MicroAODProducers/data/2013FinalPaper_PhotonID_Endcap_BDT_TrainRangePT15_8TeV.weights.xml") ),
                                regressionWeightFile = cms.string( ("%s/%s") % ( os.getenv('CMSSW_BASE','.') , "src/HiggsAnalysis/GBRLikelihoodEGTools/data/regweights_v8_8TeV_forest_ph.root") ),
                                )
