import FWCore.ParameterSet.Config as cms

flashggDoubleHReweight = cms.EDProducer("FlashggDoubleHRewighter",
                                       targetNode = cms.int(-1) 
                                       )
