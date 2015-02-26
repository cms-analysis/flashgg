import FWCore.ParameterSet.Config as cms
from flashgg.MicroAODProducers.flashggTkVtxMap_cfi import flashggVertexMapUnique,flashggVertexMapNonUnique
from flashgg.MicroAODProducers.flashggPhotons_cfi import flashggPhotons
from flashgg.MicroAODProducers.flashggDiPhotons_cfi import flashggDiPhotons
from flashgg.MicroAODProducers.flashggZeeDiPhotons_cfi import flashggZeeDiPhotons
from flashgg.MicroAODProducers.flashggJets_cfi import flashggJets
from flashgg.MicroAODProducers.flashggMicroAODGenSequence_cff import *

eventCount = cms.EDProducer("EventCountProducer")
weightsCount = cms.EDProducer("WeightsCountProducer",
                              generator=cms.InputTag("generator"),
                              pileupInfo=cms.InputTag("addPileupInfo"),
                              doObsPileup=cms.untracked.bool(True),
                              minObsPileup=cms.double(-0.5),
                              maxObsPileup=cms.double(100.5),
                              nbinsObsPileup=cms.int32(101),
                              )

flashggMicroAODZeeValidationSequence = cms.Sequence((eventCount+weightsCount
                                                     +flashggVertexMapUnique+flashggVertexMapNonUnique
                                                     +flashggMicroAODGenSequence
                                                     )
                                                    *flashggPhotons*flashggDiPhotons
                                                    *(flashggZeeDiPhotons)
                                                    )
