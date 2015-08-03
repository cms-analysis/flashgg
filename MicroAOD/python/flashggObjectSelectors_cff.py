import FWCore.ParameterSet.Config as cms

selectedFlashggElectrons = cms.EDFilter("FLASHggElectronSelector",
    src = cms.InputTag("flashggElectrons"),
    cut = cms.string("pt > 9.")
)

selectedFlashggMuons = cms.EDFilter("FLASHggMuonSelector",
    src = cms.InputTag("flashggMuons"),
    cut = cms.string("pt > 9.")
)

selectedFlashggPhotons = cms.EDFilter("FLASHggPhotonSelector",
    src = cms.InputTag("flashggPhotons"),
#    cut = cms.string("pt > 14 && hadTowOverEm()<0.15") # same as MiniAOD, so not doing anything
    cut = cms.string("pt > 14 && hadTowOverEm()<0.15 && (r9()>0.8 || chargedHadronIso()<20 || chargedHadronIso()<0.3*pt())") # full clusters in MiniAOD for these only
)
