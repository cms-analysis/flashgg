import FWCore.ParameterSet.Config as cms

PromptFakeFilter  = cms.EDFilter("PromptFakeFilter",TagSorter = cms.InputTag('flashggTagSorter'))
                                 # These are deliberately missing so misconfigurations will crash
#                                 doPromptFake = cms.bool(True)
#                                 doFakeFakse = cms.bool(False)
