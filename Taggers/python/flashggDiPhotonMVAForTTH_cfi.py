import FWCore.ParameterSet.Config as cms

flashggDiPhotonMVAForTTH = cms.EDProducer('FlashggDiPhotonMVAForTTHProducer',
                                    DiPhotonTag=cms.InputTag('flashggPreselectedDiPhotons'),
                                    diphotonMVAweightfile = cms.FileInPath("flashgg/Taggers/data/TMVAClassification_BDT__diPho__ttH_powheg_vs_bkg__5000Trees__sigmaEoE__Moriond18_0jets.json.weights.xml")
)
