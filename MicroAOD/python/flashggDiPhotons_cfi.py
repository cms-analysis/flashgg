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
                                  #vtxId 2012
                                  #vertexIdMVAweightfile = cms.FileInPath("flashgg/MicroAOD/data/2013FinalPaper_VertexID_BDTCat_conversions.weights.xml"),
                                  #vxtProb2012
                                  #vertexProbMVAweightfile = cms.FileInPath("flashgg/MicroAOD/data/TMVAClassification_BDTvtxprob2012.weights.xml"),

                                  #vtxId and vtxProb 2015 no single leg
                                  #useSingleLeg=cms.untracked.bool(False),
                                  #vertexIdMVAweightfile = cms.FileInPath("flashgg/MicroAOD/data/TMVAClassification_BDTVtxId_noSL_2015.xml"),
                                  #vertexProbMVAweightfile = cms.FileInPath("flashgg/MicroAOD/data/TMVAClassification_BDTVtxProb_noSL_2015.xml"),

                                  #vtxId and vtxProb 2015 with single leg
                                  #vertexIdMVAweightfile   = cms.FileInPath("flashgg/MicroAOD/data/TMVAClassification_BDTVtxId_SL_2015.xml"),
                                  #vertexProbMVAweightfile = cms.FileInPath("flashgg/MicroAOD/data/TMVAClassification_BDTVtxProb_SL_2015.xml"),

                                  useSingleLeg            = cms.bool(True),
                                  useZerothVertexFromMicro = cms.bool(False),

                                  nVtxSaveInfo            = cms.untracked.uint32(3),
                                  trackHighPurity         = cms.bool(False),
                                  pureGeomConvMatching    = cms.bool(True),
                                  dRexclude               = cms.double(0.05),

                                  is2017                  = cms.bool(True),
                                  #diphoton vtx in 2017
                                  ##vtxId and vtxProb 2017 with single leg
                                  vertexIdMVAweightfile_2017   = cms.FileInPath("flashgg/MicroAOD/data/TMVAClassification_BDTVtxId_SL_2017.xml"),
                                  vertexProbMVAweightfile_2017 = cms.FileInPath("flashgg/MicroAOD/data/TMVAClassification_BDTVtxProb_SL_2017.xml"),
                                  ##reso in 2017:
                                  sigma1Pix_2017               = cms.double( 0.00800379 ),
                                  sigma1Tib_2017               = cms.double( 0.502127   ),
                                  sigma1Tob_2017               = cms.double( 5.5138     ),
                                  sigma1PixFwd_2017            = cms.double( 0.0318172  ),
                                  sigma1Tid_2017               = cms.double( 0.325117   ),
                                  sigma1Tec_2017               = cms.double( 1.19907    ),
                                  sigma2Pix_2017               = cms.double( 0.0171381  ),
                                  sigma2Tib_2017               = cms.double( 0.282616   ),
                                  sigma2Tob_2017               = cms.double( 3.5737     ),
                                  sigma2PixFwd_2017            = cms.double( 0.0923745  ),
                                  sigma2Tid_2017               = cms.double( 0.355705   ),
                                  sigma2Tec_2017               = cms.double( 0.863342   ),
                                  singlelegsigma1Pix_2017      = cms.double( 0.00879849 ),
                                  singlelegsigma1Tib_2017      = cms.double( 1.37155    ),
                                  singlelegsigma1Tob_2017      = cms.double( 2.7242     ),
                                  singlelegsigma1PixFwd_2017   = cms.double( 0.0596455  ),
                                  singlelegsigma1Tid_2017      = cms.double( 0.479279   ),
                                  singlelegsigma1Tec_2017      = cms.double( 2.02211    ),
                                  singlelegsigma2Pix_2017      = cms.double( 0.0224474  ),
                                  singlelegsigma2Tib_2017      = cms.double( 0.594662   ),
                                  singlelegsigma2Tob_2017      = cms.double( 0.433137   ),
                                  singlelegsigma2PixFwd_2017   = cms.double( 0.137922   ),
                                  singlelegsigma2Tid_2017      = cms.double( 0.421378   ),
                                  singlelegsigma2Tec_2017      = cms.double( 0.977421   ),

                                  #diphoton vtx in 2016
                                  ##vtxId and vtxProb 2016 with single leg
                                  vertexIdMVAweightfile   = cms.FileInPath("flashgg/MicroAOD/data/TMVAClassification_BDTVtxId_SL_2016.xml"),
                                  vertexProbMVAweightfile = cms.FileInPath("flashgg/MicroAOD/data/TMVAClassification_BDTVtxProb_SL_2016.xml"),
                                  ##new reso:
                                  sigma1Pix               = cms.double( 0.0125255 ),
                                  sigma1Tib               = cms.double( 0.716301  ),
                                  sigma1Tob               = cms.double( 3.17615   ),
                                  sigma1PixFwd            = cms.double( 0.0581667 ),
                                  sigma1Tid               = cms.double( 0.38521   ),
                                  sigma1Tec               = cms.double( 1.67937   ),
                                  sigma2Pix               = cms.double( 0.0298574 ),
                                  sigma2Tib               = cms.double( 0.414393  ),
                                  sigma2Tob               = cms.double( 1.06805   ),
                                  sigma2PixFwd            = cms.double( 0.180419  ),
                                  sigma2Tid               = cms.double( 0.494722  ),
                                  sigma2Tec               = cms.double( 1.21941   ),
                                  singlelegsigma1Pix      = cms.double( 0.0178107 ),
                                  singlelegsigma1Tib      = cms.double( 1.3188    ),
                                  singlelegsigma1Tob      = cms.double( 2.23662   ),
                                  singlelegsigma1PixFwd   = cms.double( 0.152157  ),
                                  singlelegsigma1Tid      = cms.double( 0.702755  ),
                                  singlelegsigma1Tec      = cms.double( 2.46599   ),
                                  singlelegsigma2Pix      = cms.double( 0.0935307 ),
                                  singlelegsigma2Tib      = cms.double( 0.756568  ),
                                  singlelegsigma2Tob      = cms.double( 0.62143   ),
                                  singlelegsigma2PixFwd   = cms.double( 0.577081  ),
                                  singlelegsigma2Tid      = cms.double( 0.892751  ),
                                  singlelegsigma2Tec      = cms.double( 1.56638   ),

                                  MaxJetCollections       = cms.uint32(maxJetCollections)
                                  )
