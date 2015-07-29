import FWCore.ParameterSet.Config as cms

# legacy VBF MVA
flashggVBFMVA = cms.EDProducer('FlashggVBFMVAProducer',
                               DiPhotonTag=cms.InputTag('flashggDiPhotons'),
                               #JetTag=cms.InputTag('selectedFlashggJets'),
                               inputTagJets= JetCollectionVInputTag,
                               UseLegacyMVA = cms.untracked.bool(True),
                               MinDijetMinv = cms.double(0.0),
                               vbfMVAweightfile = cms.FileInPath("flashgg/Taggers/data/TMVA_dijet_sherpa_scalewt50_2evenb_powheg200_maxdPhi_oct9_Gradient.weights.xml"),
		)

# Legacy DiPhoDiJet MVA
flashggVBFDiPhoDiJetMVA = cms.EDProducer('FlashggVBFDiPhoDiJetMVAProducer',
                                         DiPhotonTag=cms.InputTag('flashggDiPhotons'),
                                         VBFMVAResultTag=cms.InputTag('flashggVBFMVA'),
                                         MVAResultTag=cms.InputTag('flashggDiPhotonMVA'),
                                         UseLegacyMVA = cms.untracked.bool(True),
                                         vbfDiPhoDiJetMVAweightfile = cms.FileInPath("flashgg/Taggers/data/TMVA_vbf_dijet_dipho_evenbkg_scaledwt50_maxdPhi_Gradient.weights.xml"),
		)


# new test VBF MVA
flashggVBFMVANew = cms.EDProducer('FlashggVBFMVAProducer',
                                  DiPhotonTag = cms.InputTag('flashggDiPhotons'),
                                  #JetTag = cms.InputTag('selectedFlashggJets'),
                                  inputTagJets= JetCollectionVInputTag,
                                  UseLegacyMVA = cms.untracked.bool(False),
                                  MinDijetMinv = cms.double(0.0),
                                  vbfMVAweightfile = cms.FileInPath("flashgg/Taggers/data/Flashgg_VBF_BDT.weights.xml"),
                              )


# new test DiPhoDiJet MVA
flashggVBFDiPhoDiJetMVANew = cms.EDProducer('FlashggVBFDiPhoDiJetMVAProducer',
                                            DiPhotonTag=cms.InputTag('flashggDiPhotons'),
                                            VBFMVAResultTag=cms.InputTag('flashggVBFMVANew'),
                                            UseLegacyMVA = cms.untracked.bool(False),
                                            MVAResultTag=cms.InputTag('flashggDiPhotonMVA'),
                                            vbfDiPhoDiJetMVAweightfile = cms.FileInPath("flashgg/Taggers/data/Flashgg_DiPhoDiJet_BDT.weights.xml"),
                                        )
