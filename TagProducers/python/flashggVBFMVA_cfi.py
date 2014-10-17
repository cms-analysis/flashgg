import FWCore.ParameterSet.Config as cms

flashggVBFMVA = cms.EDProducer('FlashggVBFMVAProducer',
#                                    DiPhotonTag=cms.untracked.InputTag('flashggPreselectedDiPhotons'), # why doesn't this work?
                                    DiPhotonTag=cms.untracked.InputTag('flashggDiPhotons'),
                                    JetTag=cms.untracked.InputTag('flashggJets'),
                                    vbfMVAweightfile = cms.FileInPath("flashgg/MicroAODProducers/data/TMVA_dijet_sherpa_scalewt50_2evenb_powheg200_maxdPhi_oct9_Gradient.weights.xml"),
                                    )
flashggVBFDiPhoDiJetMVA = cms.EDProducer('FlashggVBFDiPhoDiJetMVAProducer',
#                                    DiPhotonTag=cms.untracked.InputTag('flashggPreselectedDiPhotons'), # why doesn't this work?
                                    DiPhotonTag=cms.untracked.InputTag('flashggDiPhotons'),
                                     VBFMVAResultTag=cms.untracked.InputTag('flashggVBFMVA'),
                                     MVAResultTag=cms.untracked.InputTag('flashggDiPhotonMVA'),
                                     vbfDiPhoDiJetMVAweightfile = cms.FileInPath("flashgg/MicroAODProducers/data/TMVA_vbf_dijet_dipho_evenbkg_scaledwt50_maxdPhi_Gradient.weights.xml"),
                                    )
