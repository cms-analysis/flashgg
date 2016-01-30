import FWCore.ParameterSet.Config as cms

from flashgg.Taggers.flashggTags_cff import UnpackedJetCollectionVInputTag

# legacy VBF MVA
flashggVBFMVA = cms.EDProducer('FlashggVBFMVAProducer',
                               DiPhotonTag=cms.InputTag('flashggPreselectedDiPhotons'),
                               #JetTag=cms.InputTag('flashggSelectedJets'),
                               inputTagJets= UnpackedJetCollectionVInputTag,
                               MVAMethod = cms.string("BDT"),
                               UsePuJetID = cms.bool(False),
                               UseJetID = cms.bool(False),
                               merge3rdJet = cms.bool(False),
                               thirdJetDRCut = cms.double(1.8),
                               JetIDLevel = cms.string("Loose"),
                               #UseLegacyMVA = cms.bool(True),
                               rmsforwardCut = cms.double(0.03), # set to 1.0 to avoid pujid in this region
                               MinDijetMinv = cms.double(0.0),
                               #vbfMVAweightfile = cms.FileInPath("flashgg/Taggers/data/TMVA_dijet_sherpa_scalewt50_2evenb_powheg200_maxdPhi_oct9_Gradient.weights.xml"),
                               vbfMVAweightfile = cms.FileInPath("flashgg/Taggers/data/TMVAClassification_dijetMVA_Jan2016_rmscut_BDTG.weights.xml"),
                               )
# Legacy DiPhoDiJet MVA
flashggVBFDiPhoDiJetMVA = cms.EDProducer('FlashggVBFDiPhoDiJetMVAProducer',
                                         DiPhotonTag=cms.InputTag('flashggPreselectedDiPhotons'),
                                         VBFMVAResultTag=cms.InputTag('flashggVBFMVA'),
                                         MVAResultTag=cms.InputTag('flashggDiPhotonMVA'),
                                         UseLegacyMVA = cms.bool(False),
                                         #vbfDiPhoDiJetMVAweightfile = cms.FileInPath("flashgg/Taggers/data/TMVA_vbf_dijet_dipho_evenbkg_scaledwt50_maxdPhi_Gradient.weights.xml"),
                                         vbfDiPhoDiJetMVAweightfile = cms.FileInPath("flashgg/Taggers/data/TMVAClassification_combinedMVA_Jan2016_rmscut_BDTG.weights.xml"),
                                         )
