import FWCore.ParameterSet.Config as cms

flashggMetFilters = cms.EDFilter("flashggMetFilters",
                                 filtersInputTag = cms.InputTag("TriggerResults", "", ""),
                                 requiredFilterNames = cms.untracked.vstring(),
)
