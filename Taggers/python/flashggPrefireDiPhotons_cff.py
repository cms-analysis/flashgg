import FWCore.ParameterSet.Config as cms

from flashgg.Taggers.flashggTags_cff import UnpackedJetCollectionVInputTag

# prefire weight calculator
flashggPrefireDiPhotons = cms.EDProducer('FlashggPrefireDiPhotonProducer',
                               DiPhotonTag=cms.InputTag('flashggDifferentialPhoIdInputsCorrection'),
                               MuonTag=cms.InputTag('flashggSelectedMuons'),
                               inputTagJets= UnpackedJetCollectionVInputTag,
                               ECALFileName = cms.FileInPath("flashgg/Taggers/data/L1PrefiringMaps.root"),
                               dataeraEcal = cms.string(""),
                               MuonFileName = cms.FileInPath("flashgg/Taggers/data/L1MuonPrefiringParametriations.root"),
                               dataeraMuon = cms.string(""),
                               isECALRelevant = cms.bool(True),
                               applyToCentral = cms.bool(False)
)
