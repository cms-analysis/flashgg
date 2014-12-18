import FWCore.ParameterSet.Config as cms

weightProducer = cms.EDProducer('WeightProducer',
                                lumiWeight = cms.double(1.),
                                generator=cms.InputTag("generator"),
                                pileupInfo=cms.InputTag("addPileupInfo"),
                                )
