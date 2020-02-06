import FWCore.ParameterSet.Config as cms

flashggMetFilters = cms.EDFilter("flashggMetFilters",
                                 filtersInputTag = cms.InputTag("TriggerResults", "", "RECO"),
                                 requiredFilterNames = cms.untracked.vstring(),
)
