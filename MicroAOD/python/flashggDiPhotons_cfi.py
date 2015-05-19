import FWCore.ParameterSet.Config as cms

#-rw-r--r--. 1 sethzenz zh 1.1M Sep 22 09:36 TMVAClassification_BDTvtxprob2012.weights.xml
#-rw-r--r--. 1 sethzenz zh 4.2M Sep 22 09:36 HggBambu_SMDipho_Oct29_rwgtptallsigevenbkg7TeV_BDTG.weights.xml


flashggDiPhotons = cms.EDProducer('FlashggDiPhotonProducer',
                                  PhotonTag=cms.untracked.InputTag('flashggPhotons'),
                                  VertexTag=cms.untracked.InputTag('offlineSlimmedPrimaryVertices'),
                                  VertexSelectorName=cms.string("FlashggLegacyVertexSelector"),
                                  VertexCandidateMapTag=cms.InputTag("flashggVertexMapUnique"),
                                  ConversionTag=cms.untracked.InputTag("reducedEgamma","reducedConversions"),             
                                  ConversionTagSingleLeg=cms.untracked.InputTag("reducedEgamma","reducedSingleLegConversions"),

                                  ##Parameters for Legacy Vertex Selector                                                
                                  #old
                                  #vertexIdMVAweightfile = cms.FileInPath("flashgg/MicroAOD/data/2013FinalPaper_VertexID_BDTCat_conversions.weights.xml"),
                                  #new xml, new reso, no single leg conv : useSingleLeg = false
                                  #useSingleLeg=cms.untracked.bool(False),
                                  #vertexIdMVAweightfile = cms.FileInPath("flashgg/MicroAOD/data/TMVAClassification_BDTVtxId_allModes_NewReso_noSingleLeg.weights.xml"),
                                  #new xml, new reso, with single leg conv : useSingleLeg = true
                                  useSingleLeg=cms.untracked.bool(True),
                                  vertexIdMVAweightfile = cms.FileInPath("flashgg/MicroAOD/data/TMVAClassification_BDTVtxId_allModes_NewReso_withSingleLeg.weights.xml"), 
                                  vertexProbMVAweightfile = cms.FileInPath("flashgg/MicroAOD/data/TMVAClassification_BDTvtxprob2012.weights.xml"),
                                  dRexclude=cms.untracked.double(0.05),
                                  #useSingleLeg=cms.untracked.bool(True),
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
                                  singlelegsigma2Tec=cms.untracked.double(1.56638)
                                  )
