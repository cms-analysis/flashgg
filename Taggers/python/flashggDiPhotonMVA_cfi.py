import FWCore.ParameterSet.Config as cms

flashggDiPhotonMVA = cms.EDProducer('FlashggDiPhotonMVAProducer',
                                    DiPhotonTag=cms.InputTag('flashggPreselectedDiPhotons'),
                                    BeamSpotTag=cms.InputTag('offlineBeamSpot'),
                                    BeamSpotSigma=cms.double(3.7),#value is average in data, set to <0 to use actual values from MC and data
                                    diphotonMVAweightfile = cms.FileInPath("flashgg/Taggers/data/Flashgg_DiPhoton_Moriond17.weights.xml"),
                                    VertexProbParamsConv=cms.vdouble(-0.045,-0.148,-0.328,-0.184),
                                    VertexProbParamsNoConv=cms.vdouble(-0.366,-0.126,-0.119,-0.091),
                                    doSigmaMdecorr = cms.bool(True),
                                    sigmaMdecorrFile = cms.FileInPath("flashgg/Taggers/data/diphoMVA_sigmaMoMdecorr_split_Mgg40_180.root"),
                                    Version=cms.string("new")
)
