import FWCore.ParameterSet.Config as cms
from flashgg.MicroAODProducers.flashggTkVtxMap_cfi import flashggVertexMapUnique,flashggVertexMapNonUnique
from flashgg.MicroAODProducers.flashggPhotons_cfi import flashggPhotons
from flashgg.MicroAODProducers.flashggDiPhotons_cfi import flashggDiPhotons
from flashgg.MicroAODProducers.flashggHLTDiPhotons_cfi import flashggHLTDiPhotons
from flashgg.MicroAODProducers.flashggElectrons_cfi import flashggElectrons
from flashgg.MicroAODProducers.flashggMicroAODGenSequence_cff import *#flashggMicroAODGenSequence

eventCount = cms.EDProducer("EventCountProducer")
weightsCount = cms.EDProducer("WeightsCountProducer",
                              generator=cms.InputTag("generator"),
                              pileupInfo=cms.InputTag("addPileupInfo"),
                              doObsPileup=cms.untracked.bool(True),
                              minObsPileup=cms.double(-0.5),
                              maxObsPileup=cms.double(100.5),
                              nbinsObsPileup=cms.int32(101),
                              )

flashggMicroAODHLTEfficiencySequence = cms.Sequence((eventCount+weightsCount
                                        +flashggVertexMapUnique+flashggVertexMapNonUnique+flashggElectrons
                                        )
                                       *flashggMicroAODGenSequence
                                       *flashggPhotons*flashggDiPhotons
                                       *(flashggHLTDiPhotons)
                                       )
#from flashgg.MicroAODProducers.flashggGenPhotonsExtra_cfi import flashggGenPhotonsExtra

#flashggMicroAODGenSequence = cms.Sequence(flashggPrunedGenParticles+flashggGenPhotons*flashggGenPhotonsExtra
