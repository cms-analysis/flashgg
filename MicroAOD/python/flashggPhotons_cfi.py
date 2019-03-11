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
                                
                                photonIdMVAweightfile_EB = cms.FileInPath(""),
                                photonIdMVAweightfile_EE = cms.FileInPath(""),

                                is2017 = cms.bool(False),
                                effAreasConfigFile = cms.FileInPath(""),

                                ## For 2016 Legacy ReReco. For 2017 these variables are not needed
                                phoIsoPtScalingCoeff = cms.vdouble(0.0053,0.0034),
                                phoIsoCutoff = cms.double(2.5),
                                
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
