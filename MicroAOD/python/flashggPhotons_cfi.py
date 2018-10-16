import FWCore.ParameterSet.Config as cms

flashggPhotons = cms.EDProducer('FlashggPhotonProducer',
                                photonTag = cms.InputTag('slimmedPhotons'),
                                pfCandidatesTag = cms.InputTag("packedPFCandidates"),
                                reducedBarrelRecHitCollection = cms.InputTag('reducedEgamma','reducedEBRecHits'),
                                reducedEndcapRecHitCollection = cms.InputTag('reducedEgamma','reducedEERecHits'),
                                reducedPreshowerRecHitCollection = cms.InputTag('reducedEgamma','reducedESRecHits'),
                                
                                vertexTag = cms.InputTag("offlineSlimmedPrimaryVertices"),
                                vertexCandidateMapTag = cms.InputTag("flashggVertexMapNonUnique"),
                                rhoFixedGridCollection = cms.InputTag('fixedGridRhoAll'),
                                
                                photonIdMVAweightfile_EB = cms.FileInPath("flashgg/MicroAOD/data/MVAweights_80X_barrel_ICHEPvtx.xml"),
                                photonIdMVAweightfile_EE = cms.FileInPath("flashgg/MicroAOD/data/MVAweights_80X_endcap_ICHEPvtx.xml"),

                                useNewPhoId = cms.bool(True),
                                is2017 = cms.bool(True),

                                ## For 2016 Legacy ReReco
                                # effAreasConfigFile = cms.FileInPath("RecoEgamma/PhotonIdentification/data/Spring16/effAreaPhotons_cone03_pfPhotons_90percentBased.txt"),
                                
                                ## For 2017 ReReco
                                effAreasConfigFile = cms.FileInPath("RecoEgamma/PhotonIdentification/data/Fall17/effAreaPhotons_cone03_pfPhotons_90percentBased_TrueVtx.txt"),
                                ## For 2016 Legacy ReReco. For 2017 these variables are not needed
                                phoIsoPtScalingCoeff = cms.vdouble(0.0053,0.0034),
                                phoIsoCutoff = cms.double(2.5),

                                ## For 2016 Legacy rereco
                                photonIdMVAweightfile_EB_new = cms.FileInPath("flashgg/MicroAOD/data/HggPhoId_barrel_Moriond2017_wRhoRew.weights.xml"),
                                photonIdMVAweightfile_EE_new = cms.FileInPath("flashgg/MicroAOD/data/HggPhoId_endcap_Moriond2017_wRhoRew.weights.xml"),
                                
                                ## For 2017 ReReco 
                                photonIdMVAweightfile_EB_2017 = cms.FileInPath("flashgg/MicroAOD/data/HggPhoId_94X_barrel_BDT_woisocorr.weights.xml"),
                                photonIdMVAweightfile_EE_2017 = cms.FileInPath("flashgg/MicroAOD/data/HggPhoId_94X_endcap_BDT_woisocorr.weights.xml"),

                                useNonZsLazyTools = cms.bool(True),
                                recomputeNonZsClusterShapes = cms.bool(False),
                                addRechitFlags = cms.bool(True),
                                doOverlapRemovalForIsolation = cms.bool(True),
                                useVtx0ForNeutralIso = cms.bool(True),
                                extraCaloIsolations = cms.VPSet(),
                                extraIsolations = cms.VPSet(),

                                genPhotonTag = cms.InputTag("flashggGenPhotonsExtra"),
                                maxGenDeltaR = cms.double(0.1),
                                copyExtraGenInfo = cms.bool(True),

				convTag = cms.InputTag('reducedEgamma','reducedConversions'),
				beamSpotTag = cms.InputTag('offlineBeamSpot'),
				elecTag = cms.InputTag("slimmedElectrons"),

                                egmMvaValuesMap = cms.InputTag("photonMVAValueMapProducer:PhotonMVAEstimatorRun2Spring16NonTrigV1Values") 
                              )
