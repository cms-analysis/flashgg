import FWCore.ParameterSet.Config as cms

diPhotonSelector = cms.EDFilter("FLASHggDiPhotonSelector",
                                src = cms.InputTag("flashggDiPhotons"),
                                cut = cms.string("leadingPhoton().pt > 25. && subLeadingPhoton().pt > 15.")
                                )

diPhotonFilter = cms.EDFilter("CandViewCountFilter",
    src = cms.InputTag("diPhotonSelector"),
    minNumber = cms.uint32(1)
)
