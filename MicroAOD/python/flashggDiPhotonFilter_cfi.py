import FWCore.ParameterSet.Config as cms

diPhotonFilter = cms.EDFilter("CandViewCountFilter",
    src = cms.InputTag("flashggDiPhotons"),
    minNumber = cms.uint32(1)
)
