import FWCore.ParameterSet.Config as cms
from flashgg.MicroAOD.flashggTkVtxMap_cfi import flashggVertexMapUnique,flashggVertexMapNonUnique,flashggVertexMapForCHS
from flashgg.MicroAOD.flashggPhotons_cfi import flashggPhotons
from flashgg.MicroAOD.flashggDiPhotons_cfi import flashggDiPhotons
from flashgg.MicroAOD.flashggPreselectedDiPhotons_cfi import flashggPreselectedDiPhotons
from flashgg.MicroAOD.flashggJets_cfi import flashggJets
from flashgg.MicroAOD.flashggElectrons_cfi import flashggElectrons
from flashgg.MicroAOD.flashggDiMuons_cfi import flashggDiMuons   ##JTao
from flashgg.MicroAOD.flashggMuMuGamma_cfi import flashggMuMuGamma   ##JTao

from flashgg.MicroAOD.flashggMicroAODGenSequence_cff import *

eventCount = cms.EDProducer("EventCountProducer")
weightsCount = cms.EDProducer("WeightsCountProducer",
                              generator=cms.InputTag("generator"),
                              pileupInfo=cms.InputTag("addPileupInfo"),
                              doObsPileup=cms.untracked.bool(True),
                              minObsPileup=cms.double(-0.5),
                              maxObsPileup=cms.double(100.5),
                              nbinsObsPileup=cms.int32(101),
                              )

flashggMicroAODSequence = cms.Sequence((eventCount+weightsCount
                                        +flashggVertexMapUnique+flashggVertexMapNonUnique
                                        +flashggElectrons
                                        +flashggMicroAODGenSequence
                                        )
                                       *flashggPhotons*flashggDiPhotons
                                       *flashggDiMuons*flashggMuMuGamma      ##JTao
                                       *(flashggPreselectedDiPhotons+flashggVertexMapForCHS*flashggJets)
                                       )
