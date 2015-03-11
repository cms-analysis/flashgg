import FWCore.ParameterSet.Config as cms

#-rw-r--r--. 1 sethzenz zh 1.1M Sep 22 09:36 TMVAClassification_BDTvtxprob2012.weights.xml
#-rw-r--r--. 1 sethzenz zh 4.2M Sep 22 09:36 HggBambu_SMDipho_Oct29_rwgtptallsigevenbkg7TeV_BDTG.weights.xml


flashggDiPhotons = cms.EDProducer('FlashggDiPhotonProducer',
                                  PhotonTag=cms.untracked.InputTag('flashggPhotons'),
                                  VertexTag=cms.untracked.InputTag('offlineSlimmedPrimaryVertices'),
                                  VertexSelectorName=cms.string("FlashggLegacyVertexSelector"),
                                  VertexCandidateMapTag=cms.InputTag("flashggVertexMapUnique"),
                                  ConversionTag=cms.untracked.InputTag("reducedEgamma","reducedConversions"),             

                                  ##Parameters for Legacy Vertex Selector                                                
                                  vertexIdMVAweightfile = cms.FileInPath("flashgg/MicroAODProducers/data/2013FinalPaper_VertexID_BDTCat_conversions.weights.xml"),
                                  vertexProbMVAweightfile = cms.FileInPath("flashgg/MicroAODProducers/data/TMVAClassification_BDTvtxprob2012.weights.xml"),
                                  dRexclude=cms.untracked.double(0.05),
                                  sigma1Pix=cms.untracked.double(0.011),                     
                                  sigma1Tib=cms.untracked.double(0.492),                     
                                  sigma1Tob=cms.untracked.double(4.398),                     
                                  sigma1PixFwd=cms.untracked.double(0.054),                  
                                  sigma1Tid=cms.untracked.double(0.320),                     
                                  sigma1Tec=cms.untracked.double(1.092),            
                                  sigma2Pix=cms.untracked.double(0.022),                     
                                  sigma2Tib=cms.untracked.double(0.297),                     
                                  sigma2Tob=cms.untracked.double(1.728),                     
                                  sigma2PixFwd=cms.untracked.double(0.150),                  
                                  sigma2Tid=cms.untracked.double(0.393),                     
                                  sigma2Tec=cms.untracked.double(1.008),                     
                                  singlelegsigma1Pix=cms.untracked.double(0.009),            
                                  singlelegsigma1Tib=cms.untracked.double(1.163),            
                                  singlelegsigma1Tob=cms.untracked.double(2.130),            
                                  singlelegsigma1PixFwd=cms.untracked.double(0.071),
                                  singlelegsigma1Tid=cms.untracked.double(0.384),            
                                  singlelegsigma1Tec=cms.untracked.double(1.923),            
                                  singlelegsigma2Pix=cms.untracked.double(0.054),            
                                  singlelegsigma2Tib=cms.untracked.double(0.597),            
                                  singlelegsigma2Tob=cms.untracked.double(0.480),        
                                  singlelegsigma2PixFwd=cms.untracked.double(0.276),
                                  singlelegsigma2Tid=cms.untracked.double(0.497),
                                  singlelegsigma2Tec=cms.untracked.double(1.046)
                                  )
