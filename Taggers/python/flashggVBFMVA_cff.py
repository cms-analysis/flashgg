import FWCore.ParameterSet.Config as cms

from flashgg.Taggers.flashggTags_cff import UnpackedJetCollectionVInputTag

pujidPtBin1_loose  = [-0.97, -0.68, -0.53, -0.47]
pujidPtBin2_loose  = [-0.89, -0.52, -0.38, -0.30]

pujidPtBin1_medium = [0.18, -0.55, -0.42, -0.36]
pujidPtBin2_medium = [0.61, -0.35, -0.23, -0.17]

pujidPtBin1_tight  = [0.69, -0.35, -0.26, -0.21]
pujidPtBin2_tight  = [0.86, -0.10, -0.05, -0.01]

# legacy VBF MVA
flashggVBFMVA = cms.EDProducer('FlashggVBFMVAProducer',
                               DiPhotonTag=cms.InputTag('flashggPreselectedDiPhotons'),
                               #JetTag=cms.InputTag('flashggSelectedJets'),
                               inputTagJets= UnpackedJetCollectionVInputTag,
                               #MVAMethod = cms.string("BDTG"),
                               MVAMethod = cms.string("Multi"),
                               UsePuJetID  = cms.bool(False),
                               UseJetID    = cms.bool(True),
                               merge3rdJet = cms.bool(False),
                               thirdJetDRCut = cms.double(1.8),
                               JetIDLevel    = cms.string("Tight2017"),
                               ## define the pujid working point 
                               pujidWpPtBin1 = cms.vdouble(pujidPtBin1_tight), ## WP for 20 < pT < 30 
                               pujidWpPtBin2 = cms.vdouble(pujidPtBin2_tight), ## WP for 30 < pT < 50
                               #UseLegacyMVA = cms.bool(True),
                               rmsforwardCut = cms.double(3.0), # default was 0.03 , running on loose pujid
                               MinDijetMinv  = cms.double(0.0),
                               DrJetPhoton   = cms.double(0.4), # Keep the same value for now, should be set later to 0.4
                               vbfMVAweightfile = cms.FileInPath("flashgg/Taggers/data/dijet-2017-10Jul.xml")
)

# Legacy DiPhoDiJet MVA
flashggVBFDiPhoDiJetMVA = cms.EDProducer('FlashggVBFDiPhoDiJetMVAProducer',
                                         DiPhotonTag=cms.InputTag('flashggPreselectedDiPhotons'),
                                         VBFMVAResultTag=cms.InputTag('flashggVBFMVA'),
                                         MVAResultTag=cms.InputTag('flashggDiPhotonMVA'),
                                         UseLegacyMVA = cms.bool(False),
                                         #vbfDiPhoDiJetMVAweightfile = cms.FileInPath("flashgg/Taggers/data/TMVA_vbf_dijet_dipho_evenbkg_scaledwt50_maxdPhi_Gradient.weights.xml"),
                                         #vbfDiPhoDiJetMVAweightfile = cms.FileInPath("flashgg/Taggers/data/TMVAClassification_combinedMVA_Jan2016_rmscut_BDTG.weights.xml"),
                                         #vbfDiPhoDiJetMVAweightfile = cms.FileInPath("flashgg/Taggers/data/TMVAClassification_combinedMVA_76x_25_02_15_0.03_BDTG.weights.xml"),
                                         vbfDiPhoDiJetMVAweightfile = cms.FileInPath("flashgg/Taggers/data/sklearn_combined_moriond17_v4.xml"),
                                         )
