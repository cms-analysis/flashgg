import FWCore.ParameterSet.Config as cms

flashggDiPhotonMVAold = cms.EDProducer('FlashggDiPhotonMVAProducer',
                                       DiPhotonTag=cms.InputTag('flashggPreselectedDiPhotons'), # why doesn't this work?
                                       #                                    DiPhotonTag=cms.InputTag('flashggDiPhotons'),
                                       BeamSpotTag=cms.InputTag('offlineBeamSpot'),
                                       BeamSpotSigma=cms.double(4.24),#value is average in data, set to <0 to use actual values from MC and data
                                       #                                    diphotonMVAweightfile = cms.FileInPath("flashgg/Taggers/data/TMVA_DiPhoton_BDT_betaV7.weights.xml"),
                                       diphotonMVAweightfile = cms.FileInPath("flashgg/Taggers/data/Flashgg_DiPhotonMVA_76x_transfS4R9EW.weights.xml"),
                                       VertexProbParamsConv=cms.vdouble(-0.049,-0.241,-0.505,-0.270),
                                       VertexProbParamsNoConv=cms.vdouble(-0.344,-0.091,-0.234,-0.186),
                                       Version=cms.string("old")
                                       )

flashggDiPhotonMVA = cms.EDProducer('FlashggDiPhotonMVAProducer',
                                    DiPhotonTag=cms.InputTag('flashggPreselectedDiPhotons'), # why doesn't this work?
                                    #                                    DiPhotonTag=cms.InputTag('flashggDiPhotons'),
                                    BeamSpotTag=cms.InputTag('offlineBeamSpot'),
                                    BeamSpotSigma=cms.double(4.24),#value is average in data, set to <0 to use actual values from MC and data
                                    diphotonMVAweightfile = cms.FileInPath("flashgg/Taggers/data/Flashgg_DiPhotonMVA_76x_transfS4R9EW.weights.xml"),
                                    # xml 2015                                    
#			            VertexProbParamsConv=cms.vdouble(-0.049,-0.241,-0.505,-0.270),
#                                   VertexProbParamsNoConv=cms.vdouble(-0.344,-0.091,-0.234,-0.186),
                                    # xml 2016
                                    VertexProbParamsConv=cms.vdouble(-0.092,-0.205,-0.310,-0.154),
                                    VertexProbParamsNoConv=cms.vdouble(-0.351,-0.121,-0.145,-0.122),
				    Version=cms.string("new")
                                    )


