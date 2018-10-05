import FWCore.ParameterSet.Config as cms
from flashgg.MicroAOD.flashggJets_cfi import maxJetCollections

flashggPhotonJet = cms.EDProducer('FlashggPhotonJetProducer',
                                  JetTag=cms.InputTag('slimmedJets'),
                                  PhotonTag=cms.InputTag('flashggPhotons'),
                                  VertexTag=cms.InputTag('offlineSlimmedPrimaryVertices'),
                                  VertexSelectorName=cms.string("FlashggLegacyVertexSelector"),
                                  VertexCandidateMapTag=cms.InputTag("flashggVertexMapUnique"),
                                  ConversionTag=cms.InputTag("reducedEgamma","reducedConversions"),
                                  ConversionTagSingleLeg=cms.InputTag("reducedEgamma","reducedSingleLegConversions"),
                                  rhoTag = cms.InputTag( "fixedGridRhoFastjetAll" ),
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
                                  #vertexIdMVAweightfile = cms.FileInPath("flashgg/MicroAOD/data/TMVAClassification_BDTVtxId_SL_2015.xml"),
                                  #vertexProbMVAweightfile = cms.FileInPath("flashgg/MicroAOD/data/TMVAClassification_BDTVtxProb_SL_2015.xml"),

                                  useSingleLeg=cms.bool(True),
                                  useZerothVertexFromMicro = cms.bool(False),

                                  nVtxSaveInfo = cms.untracked.uint32(999),
                                  trackHighPurity = cms.bool(False),
                                  pureGeomConvMatching    = cms.bool(True),
                                  dRexclude=cms.double(0.05),

                                  era                     = cms.uint32(2017), ## default : 2017
                                  #diphoton vtx in 2017
                                  ##vtxId and vtxProb 2017 with single leg
                                  vertexIdMVAweightfile_17   = cms.FileInPath("flashgg/MicroAOD/data/TMVAClassification_BDTVtxId_SL_2017.xml"),
                                  vertexProbMVAweightfile_17 = cms.FileInPath("flashgg/MicroAOD/data/TMVAClassification_BDTVtxProb_SL_2017.xml"),
                                  ##reso in 2017:
                                  sigma1Pix_17               = cms.double( 0.00802885 ),
                                  sigma1Tib_17               = cms.double( 0.518602   ),
                                  sigma1Tob_17               = cms.double( 5.54238    ),
                                  sigma1PixFwd_17            = cms.double( 0.0343765  ),
                                  sigma1Tid_17               = cms.double( 0.327043   ),
                                  sigma1Tec_17               = cms.double( 1.22594    ),
                                  sigma2Pix_17               = cms.double( 0.0159439  ),
                                  sigma2Tib_17               = cms.double( 0.285931   ),
                                  sigma2Tob_17               = cms.double( 3.74779    ),
                                  sigma2PixFwd_17            = cms.double( 0.0914724  ),
                                  sigma2Tid_17               = cms.double( 0.342842   ),
                                  sigma2Tec_17               = cms.double( 0.875536   ),
                                  singlelegsigma1Pix_17      = cms.double( 0.00886973 ),
                                  singlelegsigma1Tib_17      = cms.double( 1.36891    ),
                                  singlelegsigma1Tob_17      = cms.double( 2.69513    ),
                                  singlelegsigma1PixFwd_17   = cms.double( 0.064971   ),
                                  singlelegsigma1Tid_17      = cms.double( 0.457494   ),
                                  singlelegsigma1Tec_17      = cms.double( 1.98512    ),
                                  singlelegsigma2Pix_17      = cms.double( 0.0218862  ),
                                  singlelegsigma2Tib_17      = cms.double( 0.581801   ),
                                  singlelegsigma2Tob_17      = cms.double( 0.42568    ),
                                  singlelegsigma2PixFwd_17   = cms.double( 0.139077   ),
                                  singlelegsigma2Tid_17      = cms.double( 0.395489   ),
                                  singlelegsigma2Tec_17      = cms.double( 0.958625   ),

                                  #diphoton vtx in 2016
                                  ##vtxId and vtxProb 2016 with single leg
                                  vertexIdMVAweightfile_16   = cms.FileInPath("flashgg/MicroAOD/data/TMVAClassification_BDTVtxId_SL_2016.xml"),
                                  vertexProbMVAweightfile_16 = cms.FileInPath("flashgg/MicroAOD/data/TMVAClassification_BDTVtxProb_SL_2016.xml"),
                                  ##reso in 2016:
                                  sigma1Pix_16               = cms.double( 0.0125255 ),
                                  sigma1Tib_16               = cms.double( 0.716301  ),
                                  sigma1Tob_16               = cms.double( 3.17615   ),
                                  sigma1PixFwd_16            = cms.double( 0.0581667 ),
                                  sigma1Tid_16               = cms.double( 0.38521   ),
                                  sigma1Tec_16               = cms.double( 1.67937   ),
                                  sigma2Pix_16               = cms.double( 0.0298574 ),
                                  sigma2Tib_16               = cms.double( 0.414393  ),
                                  sigma2Tob_16               = cms.double( 1.06805   ),
                                  sigma2PixFwd_16            = cms.double( 0.180419  ),
                                  sigma2Tid_16               = cms.double( 0.494722  ),
                                  sigma2Tec_16               = cms.double( 1.21941   ),
                                  singlelegsigma1Pix_16      = cms.double( 0.0178107 ),
                                  singlelegsigma1Tib_16      = cms.double( 1.3188    ),
                                  singlelegsigma1Tob_16      = cms.double( 2.23662   ),
                                  singlelegsigma1PixFwd_16   = cms.double( 0.152157  ),
                                  singlelegsigma1Tid_16      = cms.double( 0.702755  ),
                                  singlelegsigma1Tec_16      = cms.double( 2.46599   ),
                                  singlelegsigma2Pix_16      = cms.double( 0.0935307 ),
                                  singlelegsigma2Tib_16      = cms.double( 0.756568  ),
                                  singlelegsigma2Tob_16      = cms.double( 0.62143   ),
                                  singlelegsigma2PixFwd_16   = cms.double( 0.577081  ),
                                  singlelegsigma2Tid_16      = cms.double( 0.892751  ),
                                  singlelegsigma2Tec_16      = cms.double( 1.56638   ),

                                  MaxJetCollections = cms.uint32(maxJetCollections),

                                  #Photon jet cut
                                  minJetPt = cms.double(30.),
                                  maxJetEta = cms.double(2.5),
                                  minPhotonPt = cms.double(55.),
                                  #Photon loose Id cut https://twiki.cern.ch/twiki/bin/view/CMS/CutBasedPhotonIdentificationRun2#Working_points_for_94X_and_later
                                  minPhotEBHoE = cms.double(0.04596),
                                  minPhotEEHoE = cms.double(0.0590),
                                  minPhotEBsietaieta = cms.double(0.0106),
                                  minPhotEEsietaieta = cms.double(0.0272),
                                  PhotEBChgIsoParams = cms.vdouble(1.694),
                                  PhotEEChgIsoParams = cms.vdouble(2.089),
                                  PhotEBNeuIsoParams = cms.vdouble(24.032, 0.01512, 2.259e-05),
                                  PhotEENeuIsoParams = cms.vdouble(19.722, 0.0117, 2.3e-05),
                                  PhotEBPhoIsoParams = cms.vdouble(2.876, 0.004017),
                                  PhotEEPhoIsoParams = cms.vdouble(4.162, 0.0037),
                                  iphotIsolnAreaValN = cms.int32(7),
                                  photIsolnEAreaVal = cms.vdouble(1.0, 1.479, 2.0, 2.2, 2.3, 2.4, 999999.0),
                                  photIsolnEAreaChgHad = cms.vdouble(0.0112, 0.0108, 0.0106, 0.01002, 0.0098, 0.0089, 0.0087), #70% cont
                                  photIsolnEAreaNeuHad = cms.vdouble(0.0668, 0.1054, 0.0786, 0.0233, 0.0078, 0.0028, 0.0137), #90% cont
                                  photIsolnEAreaPhot = cms.vdouble(0.1113, 0.0953, 0.0619, 0.0837, 0.1070, 0.1212, 0.1466) #90% cont
                                  )
