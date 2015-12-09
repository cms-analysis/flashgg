import FWCore.ParameterSet.Config as cms

from flashgg.Taggers.flashggTags_cff import UnpackedJetCollectionVInputTag

# legacy VBF MVA
flashggVBFMVA = cms.EDProducer('FlashggVBFMVAProducer',
                               DiPhotonTag=cms.InputTag('flashggPreselectedDiPhotons'),
                               #JetTag=cms.InputTag('flashggSelectedJets'),
                               inputTagJets= UnpackedJetCollectionVInputTag,
                               UseLegacyMVA = cms.untracked.bool(True),
                               MinDijetMinv = cms.double(0.0),
                               #vbfMVAweightfile = cms.FileInPath("flashgg/Taggers/data/TMVA_dijet_sherpa_scalewt50_2evenb_powheg200_maxdPhi_oct9_Gradient.weights.xml"),
                               vbfMVAweightfile = cms.FileInPath("flashgg/Taggers/data/Flashgg_VBF_CHS_STD_BDTG.weights.xml"),
                               )

# Legacy DiPhoDiJet MVA
flashggVBFDiPhoDiJetMVA = cms.EDProducer('FlashggVBFDiPhoDiJetMVAProducer',
                                         DiPhotonTag=cms.InputTag('flashggPreselectedDiPhotons'),
                                         VBFMVAResultTag=cms.InputTag('flashggVBFMVA'),
                                         MVAResultTag=cms.InputTag('flashggDiPhotonMVA'),
                                         UseLegacyMVA = cms.untracked.bool(True),
                                         #vbfDiPhoDiJetMVAweightfile = cms.FileInPath("flashgg/Taggers/data/TMVA_vbf_dijet_dipho_evenbkg_scaledwt50_maxdPhi_Gradient.weights.xml"),
                                         vbfDiPhoDiJetMVAweightfile = cms.FileInPath("flashgg/Taggers/data/Flashgg_DiPhoDiJet_CHS_STD_BDTG.weights.xml"),
                                         )


# new test VBF MVA
flashggVBFMVANew = cms.EDProducer('FlashggVBFMVAProducer',
                                  DiPhotonTag = cms.InputTag('flashggPreselectedDiPhotons'),
                                  #JetTag = cms.InputTag('flashggSelectedJets'),
                                  inputTagJets= UnpackedJetCollectionVInputTag,
                                  UseLegacyMVA = cms.untracked.bool(False),
                                  MinDijetMinv = cms.double(0.0),
                                  vbfMVAweightfile = cms.FileInPath("flashgg/Taggers/data/Flashgg_VBF_BDT.weights.xml"),
                              )


# new test DiPhoDiJet MVA
flashggVBFDiPhoDiJetMVANew = cms.EDProducer('FlashggVBFDiPhoDiJetMVAProducer',
                                            DiPhotonTag=cms.InputTag('flashggPreselectedDiPhotons'),
                                            VBFMVAResultTag=cms.InputTag('flashggVBFMVANew'),
                                            UseLegacyMVA = cms.untracked.bool(False),
                                            MVAResultTag=cms.InputTag('flashggDiPhotonMVA'),
                                            vbfDiPhoDiJetMVAweightfile = cms.FileInPath("flashgg/Taggers/data/Flashgg_DiPhoDiJet_BDT.weights.xml"),
                                        )
