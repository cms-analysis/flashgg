import FWCore.ParameterSet.Config as cms
from flashgg.MicroAOD.flashggJets_cfi import maxJetCollections

flashggDiPhotons = cms.EDProducer('FlashggDiPhotonProducer',
                                  PhotonTag              = cms.InputTag('flashggRandomizedPhotons'),
                                  VertexTag              = cms.InputTag('offlineSlimmedPrimaryVertices'),
                                  VertexSelectorName     = cms.string("FlashggLegacyVertexSelector"),
                                  VertexCandidateMapTag  = cms.InputTag("flashggVertexMapUnique"),
                                  ConversionTag          = cms.InputTag("reducedEgamma","reducedConversions"),
                                  ConversionTagSingleLeg = cms.InputTag("reducedEgamma","reducedSingleLegConversions"),
                                  beamSpotTag            = cms.InputTag( "offlineBeamSpot" ),
                                  GenParticleTag         = cms.InputTag( "flashggPrunedGenParticles" ),

                                  ##Parameters for Legacy Vertex Selector                                                
                                  vertexIdMVAweightfile   = cms.FileInPath(""),
                                  vertexProbMVAweightfile = cms.FileInPath(""),

                                  useSingleLeg            = cms.bool(True),
                                  useZerothVertexFromMicro = cms.bool(False),
                                #   whichVertex              = cms.   uint32(0),

                                  nVtxSaveInfo            = cms.untracked.uint32(3),
                                  trackHighPurity         = cms.bool(False),
                                  pureGeomConvMatching    = cms.bool(True),
                                  dRexclude               = cms.double(0.05),
                                  #new reso:
                                  sigma1Pix               = cms.double(0.0125255),
                                  sigma1Tib               = cms.double(0.716301),
                                  sigma1Tob               = cms.double(3.17615),
                                  sigma1PixFwd            = cms.double(0.0581667),
                                  sigma1Tid               = cms.double(0.38521),
                                  sigma1Tec               = cms.double(1.67937),
                                  sigma2Pix               = cms.double(0.0298574),
                                  sigma2Tib               = cms.double(0.414393),
                                  sigma2Tob               = cms.double(1.06805),
                                  sigma2PixFwd            = cms.double(0.180419),
                                  sigma2Tid               = cms.double(0.494722),
                                  sigma2Tec               = cms.double(1.21941),
                                  singlelegsigma1Pix      = cms.double(0.0178107),
                                  singlelegsigma1Tib      = cms.double(1.3188),
                                  singlelegsigma1Tob      = cms.double(2.23662),
                                  singlelegsigma1PixFwd   = cms.double(0.152157),
                                  singlelegsigma1Tid      = cms.double(0.702755),
                                  singlelegsigma1Tec      = cms.double(2.46599),
                                  singlelegsigma2Pix      = cms.double(0.0935307),
                                  singlelegsigma2Tib      = cms.double(0.756568),
                                  singlelegsigma2Tob      = cms.double(0.62143),
                                  singlelegsigma2PixFwd   = cms.double(0.577081),
                                  singlelegsigma2Tid      = cms.double(0.892751),
                                  singlelegsigma2Tec      = cms.double(1.56638),
                                  MaxJetCollections       = cms.uint32(maxJetCollections)
                              )

# flashggDiPhotonsLite = cms.EDProducer('FlashggDiPhotonProducer',
#                                       PhotonTag              = cms.InputTag('flashggRandomizedPhotons'),
#                                       VertexTag              = cms.InputTag('offlineSlimmedPrimaryVertices'),
#                                       VertexSelectorName     = cms.string("FlashggLegacyVertexSelector"),
#                                       VertexCandidateMapTag  = cms.InputTag("flashggVertexMapUnique"),
#                                       ConversionTag          = cms.InputTag("reducedEgamma","reducedConversions"),
#                                       ConversionTagSingleLeg = cms.InputTag("reducedEgamma","reducedSingleLegConversions"),
#                                       beamSpotTag            = cms.InputTag( "offlineBeamSpot" ),
#                                       GenParticleTag         = cms.InputTag( "flashggPrunedGenParticles" ),
#                                       MaxJetCollections      = cms.uint32(maxJetCollections),
#                                       reProcess              = cms.bool(True)
#                                       )