import FWCore.ParameterSet.Config as cms
from flashgg.MicroAOD.flashggJets_cfi import maxJetCollections

flashggDiPhotons = cms.EDProducer('FlashggDiPhotonProducer',
                                  PhotonTag=cms.InputTag('flashggPhotons'),
                                  VertexTag=cms.InputTag('offlineSlimmedPrimaryVertices'),
                                  VertexSelectorName=cms.string("FlashggLegacyVertexSelector"),
                                  VertexCandidateMapTag=cms.InputTag("flashggVertexMapUnique"),
                                  ConversionTag=cms.InputTag("reducedEgamma","reducedConversions"),             
                                  ConversionTagSingleLeg=cms.InputTag("reducedEgamma","reducedSingleLegConversions"),
                                  beamSpotTag = cms.InputTag( "offlineBeamSpot" ),

                                  ##Parameters for Legacy Vertex Selector                                                
                                  #vtxId 2012
                                  #vertexIdMVAweightfile = cms.FileInPath("flashgg/MicroAOD/data/2013FinalPaper_VertexID_BDTCat_conversions.weights.xml"),
                                  #vxtProb2012
                                  #vertexProbMVAweightfile = cms.FileInPath("flashgg/MicroAOD/data/TMVAClassification_BDTvtxprob2012.weights.xml"),

                                  #vtxId and vtxProb 2015 no single leg
                                  #useSingleLeg=cms.untracked.bool(False),
                                  #vertexIdMVAweightfile = cms.FileInPath("flashgg/MicroAOD/data/TMVAClassification_BDTVtxId_noSL_2015.xml"),
                                  #vertexProbMVAweightfile = cms.FileInPath("flashgg/MicroAOD/data/TMVAClassification_BDTVtxProb_noSL_2015.xml"),

                                  #vtxId and vtxProb 2015 with single leg
                                  vertexIdMVAweightfile = cms.FileInPath("flashgg/MicroAOD/data/TMVAClassification_BDTVtxId_SL_2015.xml"),
                                  vertexProbMVAweightfile = cms.FileInPath("flashgg/MicroAOD/data/TMVAClassification_BDTVtxProb_SL_2015.xml"),
                                  useSingleLeg=cms.untracked.bool(True),

                                  dRexclude=cms.untracked.double(0.05),
                                  #new reso:
                                  sigma1Pix=cms.untracked.double(0.0125255),
                                  sigma1Tib=cms.untracked.double(0.716301), 
                                  sigma1Tob=cms.untracked.double(3.17615),
                                  sigma1PixFwd=cms.untracked.double(0.0581667),
                                  sigma1Tid=cms.untracked.double(0.38521),
                                  sigma1Tec=cms.untracked.double(1.67937),
                                  sigma2Pix=cms.untracked.double(0.0298574),
                                  sigma2Tib=cms.untracked.double(0.414393),
                                  sigma2Tob=cms.untracked.double(1.06805),
                                  sigma2PixFwd=cms.untracked.double(0.180419),
                                  sigma2Tid=cms.untracked.double(0.494722),
                                  sigma2Tec=cms.untracked.double(1.21941),
                                  singlelegsigma1Pix=cms.untracked.double(0.0178107), 
                                  singlelegsigma1Tib=cms.untracked.double(1.3188),
                                  singlelegsigma1Tob=cms.untracked.double(2.23662),
                                  singlelegsigma1PixFwd=cms.untracked.double(0.152157),
                                  singlelegsigma1Tid=cms.untracked.double(0.702755),
                                  singlelegsigma1Tec=cms.untracked.double(2.46599),
                                  singlelegsigma2Pix=cms.untracked.double(0.0935307),
                                  singlelegsigma2Tib=cms.untracked.double(0.756568),
                                  singlelegsigma2Tob=cms.untracked.double(0.62143),
                                  singlelegsigma2PixFwd=cms.untracked.double(0.577081),
                                  singlelegsigma2Tid=cms.untracked.double(0.892751),
                                  singlelegsigma2Tec=cms.untracked.double(1.56638),
                                  MaxJetCollections = cms.uint32(maxJetCollections)
                                  )
