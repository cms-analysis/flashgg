import FWCore.ParameterSet.Config as cms

flashggUntaggedCategory = cms.EDProducer("FlashggUntaggedCategoryProducer",
#                                         DiPhotonTag=cms.untracked.InputTag('flashggPreselectedDiPhotons'), # why doesn't this work?
                                         DiPhotonTag=cms.untracked.InputTag('flashggDiPhotons'),
                                         MVAResultTag=cms.untracked.InputTag('flashggDiPhotonMVA'),
                                         Boundaries=cms.untracked.vdouble(0.07,0.31,0.62,0.86,0.98)
                                         )

flashggVBFTag = cms.EDProducer("FlashggVBFTagProducer",
#                                         DiPhotonTag=cms.untracked.InputTag('flashggPreselectedDiPhotons'), # why doesn't this work?
                                         DiPhotonTag=cms.untracked.InputTag('flashggDiPhotons'),
                                         VBFDiPhoDiJetMVAResultTag=cms.untracked.InputTag('flashggVBFDiPhoDiJetMVA'),
                                         VBFMVAResultTag=cms.untracked.InputTag('flashggVBFMVA'),
                                         Boundaries=cms.untracked.vdouble(0.21,0.6,0.81)
                                         )
flashggTTHleptonicTag = cms.EDProducer("FlashggTTHleptonicTagProducer",
					DiPhotonTag=cms.untracked.InputTag('flashggDiPhotons'),
					TTHJetTag=cms.untracked.InputTag('flashggJets'),
					ElectronTag=cms.untracked.InputTag('slimmedElectron'),
					MuonTag=cms.untracked.InputTag('slimmedMuon'),
					leptonPtThreshold = cms.untracked.double(20),
					leadPhoOverMassThreshold = cms.untracked.double(0.5),
					subleadPhoOverMassThreshold = cms.untracked.double(0.25),
					MVAThreshold = cms.untracked.double(-0.6),
					deltaRLepPhoThreshold = cms.untracked.double(0.5),
					deltaRJetLepThreshold = cms.untracked.double(0.5),
					jetsNumberThreshold = cms.untracked.double(2.),
					bjetsNumberThreshold = cms.untracked.double(1.),
					jetPtThreshold = cms.untracked.double(30.),
					jetEtaThreshold= cms.untracked.double(2.4),
					bDiscriminator=cms.untracked.vdouble(0.244,0.679),
					#bDiscriminator=cms.untracked.vdouble(-1.,0.679),
					bTag = cms.untracked.string("combinedSecondaryVertexBJetTags"),
					muPFIsoSumRelThreshold = cms.untracked.double(0.2),
					deltaRMuonJetcountThreshold = cms.untracked.double(2.),
					PuIDCutoffThreshold = cms.untracked.double(0.8),
					PhoMVAThreshold = cms.untracked.double(-0.2)
				    )
