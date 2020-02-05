import FWCore.ParameterSet.Config as cms

PromptFakeFilter  = cms.EDFilter("PromptFakeFilter",TagSorter = cms.InputTag('flashggTagSorter'))
                                 # These are deliberately missing so misconfigurations will crash
#                                 doPromptFake = cms.bool(False)
#                                 doFakeFake = cms.bool(False)
#                                 doBoth= cms.bool(True)
