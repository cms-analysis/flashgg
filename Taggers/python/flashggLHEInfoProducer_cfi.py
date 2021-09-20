import FWCore.ParameterSet.Config as cms

flashggLHEInfos = cms.EDProducer("FlashggLHEInfoDumper",
                                 lheInfo = cms.VInputTag(cms.InputTag("externalLHEProducer"), 
                                                         cms.InputTag("source"))
                                 )
