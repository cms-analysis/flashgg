import FWCore.ParameterSet.Config as cms

flashggDiPhotonMVAold = cms.EDProducer('FlashggDiPhotonMVAProducer',
                                       DiPhotonTag=cms.InputTag('flashggPreselectedDiPhotons'), # why doesn't this work?
                                       #                                    DiPhotonTag=cms.InputTag('flashggDiPhotons'),
                                       BeamSpotTag=cms.InputTag('offlineBeamSpot'),
                                       BeamSpotSigma=cms.double(4.24),#value is average in data, set to <0 to use actual values from MC and data
                                       #                                    diphotonMVAweightfile = cms.FileInPath("flashgg/Taggers/data/TMVA_DiPhoton_BDT_betaV7.weights.xml"),
                                       diphotonMVAweightfile = cms.FileInPath("flashgg/Taggers/data/Flashgg_DiPhoton_80x.weights.xml"),
                                       VertexProbParamsConv=cms.vdouble(-0.049,-0.241,-0.505,-0.270),
                                       VertexProbParamsNoConv=cms.vdouble(-0.344,-0.091,-0.234,-0.186),
                                       Version=cms.string("old")
                                       )

flashggDiPhotonMVA = cms.EDProducer('FlashggDiPhotonMVAProducer',
                                    DiPhotonTag=cms.InputTag('flashggPreselectedDiPhotons'), # why doesn't this work?
                                    #                                    DiPhotonTag=cms.InputTag('flashggDiPhotons'),
                                    BeamSpotTag=cms.InputTag('offlineBeamSpot'),
                                    BeamSpotSigma=cms.double(3.7),#value is average in data, set to <0 to use actual values from MC and data
                                    diphotonMVAweightfile = cms.FileInPath("flashgg/Taggers/data/Flashgg_DiPhoton_80x.weights.xml"),
                                    # xml 2015                                    
#			            VertexProbParamsConv=cms.vdouble(-0.049,-0.241,-0.505,-0.270),
#                                   VertexProbParamsNoConv=cms.vdouble(-0.344,-0.091,-0.234,-0.186),
                                    # xml 2016 fix low nvtx
                                    VertexProbParamsConv=cms.vdouble(-0.045,-0.148,-0.328,-0.184),
                                    VertexProbParamsNoConv=cms.vdouble(-0.366,-0.126,-0.119,-0.091),
				    Version=cms.string("new")
                                    )

