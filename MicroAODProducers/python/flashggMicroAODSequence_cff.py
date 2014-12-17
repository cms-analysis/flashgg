import FWCore.ParameterSet.Config as cms
from flashgg.MicroAODProducers.flashggVertexMaps_cff import flashggVertexMapUnique,flashggVertexMapNonUnique
from flashgg.MicroAODProducers.flashggPhotons_cfi import flashggPhotons
from flashgg.MicroAODProducers.flashggDiPhotons_cfi import flashggDiPhotons
from flashgg.MicroAODProducers.flashggPreselectedDiPhotons_cfi import flashggPreselectedDiPhotons
from flashgg.MicroAODProducers.flashggJets_cfi import flashggJets
from flashgg.MicroAODProducers.flashggPrunedGenParticles_cfi import flashggPrunedGenParticles
from flashgg.MicroAODProducers.flashggElectrons_cfi import flashggElectrons

eventCount = cms.EDProducer("EventCountProducer")
weightsCount = cms.EDProducer("WeightsCountProducer",
                              generator=cms.InputTag("generator"),
                              pileupInfo=cms.InputTag("addPileupInfo"),
                              doObsPileup=cms.untracked.bool(True),
                              minObsPileup=cms.double(-0.5),
                              maxObsPileup=cms.double(100.5),
                              nbinsObsPileup=cms.int32(101),
                              )



flashggMicroAODSequence = cms.Sequence((eventCount+weightsCount+flashggVertexMapUnique+flashggVertexMapNonUnique+flashggPrunedGenParticles+flashggElectrons)*
                                       flashggPhotons*
                                       flashggDiPhotons*
                                       (flashggPreselectedDiPhotons+flashggJets)
                                       )
