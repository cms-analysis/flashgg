import FWCore.ParameterSet.Config as cms
#from flashgg.MicroAOD.flashggJets_cfi import maxJetCollections

flashggPhotonJet = cms.EDProducer('FlashggPhotonJetProducer',
                                  JetTag=cms.InputTag('slimmedJets'),
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
                                  useSingleLeg=cms.bool(True),

                                  minJetPt = cms.double(30.),
                                  minPhotonPt = cms.double(30.),
                                  ###FF#trackHighPurity = cms.bool(False),
                                  ###FF#dRexclude=cms.double(0.05),
                                  ###FF##new reso:
                                  ###FF#sigma1Pix=cms.double(0.0125255),
                                  ###FF#sigma1Tib=cms.double(0.716301), 
                                  ###FF#sigma1Tob=cms.double(3.17615),
                                  ###FF#sigma1PixFwd=cms.double(0.0581667),
                                  ###FF#sigma1Tid=cms.double(0.38521),
                                  ###FF#sigma1Tec=cms.double(1.67937),
                                  ###FF#sigma2Pix=cms.double(0.0298574),
                                  ###FF#sigma2Tib=cms.double(0.414393),
                                  ###FF#sigma2Tob=cms.double(1.06805),
                                  ###FF#sigma2PixFwd=cms.double(0.180419),
                                  ###FF#sigma2Tid=cms.double(0.494722),
                                  ###FF#sigma2Tec=cms.double(1.21941),
                                  ###FF#singlelegsigma1Pix=cms.double(0.0178107), 
                                  ###FF#singlelegsigma1Tib=cms.double(1.3188),
                                  ###FF#singlelegsigma1Tob=cms.double(2.23662),
                                  ###FF#singlelegsigma1PixFwd=cms.double(0.152157),
                                  ###FF#singlelegsigma1Tid=cms.double(0.702755),
                                  ###FF#singlelegsigma1Tec=cms.double(2.46599),
                                  ###FF#singlelegsigma2Pix=cms.double(0.0935307),
                                  ###FF#singlelegsigma2Tib=cms.double(0.756568),
                                  ###FF#singlelegsigma2Tob=cms.double(0.62143),
                                  ###FF#singlelegsigma2PixFwd=cms.double(0.577081),
                                  ###FF#singlelegsigma2Tid=cms.double(0.892751),
                                  ###FF#singlelegsigma2Tec=cms.double(1.56638),
                                  #MaxJetCollections = cms.uint32(maxJetCollections)
                                  )
