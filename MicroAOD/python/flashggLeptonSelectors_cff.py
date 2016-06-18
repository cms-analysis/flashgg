import FWCore.ParameterSet.Config as cms

flashggSelectedElectrons = cms.EDFilter("FLASHggElectronSelector",
    src = cms.InputTag("flashggElectrons"),
    cut = cms.string("pt > 9.")
)

flashggSelectedMuons = cms.EDFilter("FLASHggMuonSelector",
    src = cms.InputTag("flashggMuons"),
    cut = cms.string("pt > 9.")
)
