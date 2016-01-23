import FWCore.ParameterSet.Config as cms

flashggDiPhotonMVAold = cms.EDProducer('FlashggDiPhotonMVAProducer',
                                    DiPhotonTag=cms.InputTag('flashggPreselectedDiPhotons'), # why doesn't this work?
#                                    DiPhotonTag=cms.InputTag('flashggDiPhotons'),
                                    BeamSpotTag=cms.InputTag('offlineBeamSpot'),
#                                    diphotonMVAweightfile = cms.FileInPath("flashgg/Taggers/data/TMVA_DiPhoton_BDT_betaV7.weights.xml"),
                                    diphotonMVAweightfile = cms.FileInPath("flashgg/Taggers/data/Flashgg_DiPhoton_Rereco.weights.xml"),
                                    VertexProbParamsConv=cms.vdouble(-0.049,-0.241,-0.505,-0.270),
                                    VertexProbParamsNoConv=cms.vdouble(-0.344,-0.091,-0.234,-0.186),
				    Version=cms.string("old")
                                    )

flashggDiPhotonMVA = cms.EDProducer('FlashggDiPhotonMVAProducer',
                                    DiPhotonTag=cms.InputTag('flashggPreselectedDiPhotons'), # why doesn't this work?
#                                    DiPhotonTag=cms.InputTag('flashggDiPhotons'),
                                    BeamSpotTag=cms.InputTag('offlineBeamSpot'),
                                    diphotonMVAweightfile = cms.FileInPath("flashgg/Taggers/data/Flashgg_DiPhoton_Rereco.weights.xml"),
			            VertexProbParamsConv=cms.vdouble(-0.049,-0.241,-0.505,-0.270),
                                    VertexProbParamsNoConv=cms.vdouble(-0.344,-0.091,-0.234,-0.186),
				    Version=cms.string("new")
                                    )

