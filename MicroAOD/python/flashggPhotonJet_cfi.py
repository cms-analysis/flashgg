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

                                  #vtxId and vtxProb 2016 with single leg
                                  vertexIdMVAweightfile   = cms.FileInPath("flashgg/MicroAOD/data/TMVAClassification_BDTVtxId_SL_2016.xml"),
                                  vertexProbMVAweightfile = cms.FileInPath("flashgg/MicroAOD/data/TMVAClassification_BDTVtxProb_SL_2016.xml"),

                                  useSingleLeg=cms.bool(True),
					
                                  minJetPt = cms.double(30.),
                                  maxJetEta = cms.double(2.5),
				  minPhotonPt = cms.double(55.),
				  minPhotEBHoE = cms.double(0.05), #medium from https://twiki.cern.ch/twiki/bin/view/CMS/CutBasedPhotonIdentificationRun2#Recommended_Working_points_for_2
				  minPhotEEHoE = cms.double(0.05),
				  minPhotEBsietaieta = cms.double(0.0102),
				  minPhotEEsietaieta = cms.double(0.0268),
				  minPhotEBChgIso = cms.double(1.37),
                                  minPhotEEChgIso = cms.double(1.10),
                                  minPhotEBNeuIso = cms.double(1.06),
                                  minPhotEENeuIso = cms.double(2.69),
                                  minPhotEBPhoIso = cms.double(0.28),
                                  minPhotEEPhoIso = cms.double(0.39),
				  iphotIsolnAreaValN = cms.int32(7),#https://indico.cern.ch/event/370513/contribution/1/attachments/1183744/1715032/SP15_25_4th_final.pdf
				  photIsolnEAreaVal = cms.vdouble(1.0, 1.479, 2.0, 2.2, 2.3, 2.4, 999999.0),
				  photIsolnEAreaChgHad = cms.vdouble(0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0),
                                  photIsolnEAreaNeuHad = cms.vdouble(0.0599, 0.0819, 0.0696, 0.0360, 0.0360, 0.0462, 0.0656),
				  photIsolnEAreaPhot = cms.vdouble(0.1271, 0.1101, 0.0756, 0.1175, 0.1498, 0.1857, 0.2183),
                                  
				  trackHighPurity = cms.bool(False),
                                  pureGeomConvMatching    = cms.bool(True),
                                  dRexclude=cms.double(0.05),
                                  #new reso:
                                  sigma1Pix=cms.double(0.0125255),
                                  sigma1Tib=cms.double(0.716301), 
                                  sigma1Tob=cms.double(3.17615),
                                  sigma1PixFwd=cms.double(0.0581667),
                                  sigma1Tid=cms.double(0.38521),
                                  sigma1Tec=cms.double(1.67937),
                                  sigma2Pix=cms.double(0.0298574),
                                  sigma2Tib=cms.double(0.414393),
                                  sigma2Tob=cms.double(1.06805),
                                  sigma2PixFwd=cms.double(0.180419),
                                  sigma2Tid=cms.double(0.494722),
                                  sigma2Tec=cms.double(1.21941),
                                  singlelegsigma1Pix=cms.double(0.0178107), 
                                  singlelegsigma1Tib=cms.double(1.3188),
                                  singlelegsigma1Tob=cms.double(2.23662),
                                  singlelegsigma1PixFwd=cms.double(0.152157),
                                  singlelegsigma1Tid=cms.double(0.702755),
                                  singlelegsigma1Tec=cms.double(2.46599),
                                  singlelegsigma2Pix=cms.double(0.0935307),
                                  singlelegsigma2Tib=cms.double(0.756568),
                                  singlelegsigma2Tob=cms.double(0.62143),
                                  singlelegsigma2PixFwd=cms.double(0.577081),
                                  singlelegsigma2Tid=cms.double(0.892751),
                                  singlelegsigma2Tec=cms.double(1.56638),
                                  MaxJetCollections = cms.uint32(maxJetCollections)
                                  )
