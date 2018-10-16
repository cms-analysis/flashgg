import FWCore.ParameterSet.Config as cms

from flashgg.Taggers.flashggTags_cff import UnpackedJetCollectionVInputTag
import flashgg.Taggers.PUJID_wps as pujid

# legacy VBF MVA
flashggVBFMVA = cms.EDProducer('FlashggVBFMVAProducer',
                               DiPhotonTag=cms.InputTag('flashggPreselectedDiPhotons'),
                               #JetTag=cms.InputTag('flashggSelectedJets'),
                               inputTagJets= UnpackedJetCollectionVInputTag,
                               MVAMethod = cms.string("BDTG"),
                               UsePuJetID  = cms.bool(False),
                               UseJetID    = cms.bool(True),
                               merge3rdJet = cms.bool(False),
                               thirdJetDRCut = cms.double(1.8),
                               JetIDLevel    = cms.string("Tight2017"),
                               # changes loose to another working point, or comment if you want to disable pujid
                               pujidWpPtBin1 = cms.vdouble([0.69, -0.35, -0.26, -0.21]), # cms.vdouble(pujid.loose[0]),
                               pujidWpPtBin2 = cms.vdouble([0.86, -0.1 , -0.05, -0.01]), # cms.vdouble(pujid.loose[1]),
                               pujidWpPtBin3 = cms.vdouble([0.95,  0.28,  0.31,  0.28]), # cms.vdouble(pujid.loose[2]), 
                               #UseLegacyMVA = cms.bool(True),
                               rmsforwardCut = cms.double(3.0), # default was 0.03 , running on loose pujid
                               MinDijetMinv  = cms.double(0.0),
                               DrJetPhoton   = cms.double(0.4), # Keep the same value for now, should be set later to 0.4
                               #vbfMVAweightfile = cms.FileInPath("flashgg/Taggers/data/TMVA_dijet_sherpa_scalewt50_2evenb_powheg200_maxdPhi_oct9_Gradient.weights.xml"),
                               #vbfMVAweightfile = cms.FileInPath("flashgg/Taggers/data/TMVAClassification_dijetMVA_Jan2016_rmscut_BDTG.weights.xml"),
                               #vbfMVAweightfile = cms.FileInPath("flashgg/Taggers/data/TMVAClassification_dijetMVA_76x_24_02_15_BDTG.weights.xml"),
                               #vbfMVAweightfile = cms.FileInPath("flashgg/Taggers/data/TMVA_classification_dijet-mva-80x-ICHEP-v04.weights.xml"),
#                               vbfMVAweightfile = cms.FileInPath("flashgg/Taggers/data/sklearn_training_moriond17_v8.xml"),
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
