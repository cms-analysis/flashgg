import FWCore.ParameterSet.Config as cms

flashggDiPhotonMVA = cms.EDProducer('FlashggDiPhotonMVAProducer',
#                                    DiPhotonTag=cms.untracked.InputTag('flashggPreselectedDiPhotons'), # why doesn't this work?
                                    DiPhotonTag=cms.InputTag('flashggDiPhotons'),
                                    BeamSpotTag=cms.untracked.InputTag('offlineBeamSpot'),
                                    diphotonMVAweightfile = cms.FileInPath("flashgg/Taggers/data/HggBambu_SMDipho_Oct29_rwgtptallsigevenbkg7TeV_BDTG.weights.xml"),
                                    VertexProbParamsConv=cms.vdouble(-0.049,-0.241,-0.505,-0.270),
                                    VertexProbParamsNoConv=cms.vdouble(-0.344,-0.091,-0.234,-0.186)
                                    )
