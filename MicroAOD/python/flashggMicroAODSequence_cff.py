import FWCore.ParameterSet.Config as cms
from flashgg.MicroAOD.flashggTkVtxMap_cfi import flashggVertexMapUnique,flashggVertexMapNonUnique,flashggVertexMapForCHS
from flashgg.MicroAOD.flashggPhotons_cfi import flashggPhotons
from flashgg.MicroAOD.flashggDiPhotons_cfi import flashggDiPhotons
from flashgg.MicroAOD.flashggPreselectedDiPhotons_cfi import flashggPreselectedDiPhotons
#from flashgg.MicroAOD.flashggJets_cfi import flashggJets
from flashgg.MicroAOD.flashggElectrons_cfi import flashggElectrons
from flashgg.MicroAOD.flashggMuons_cfi import flashggMuons
from flashgg.MicroAOD.flashggFinalEGamma_cfi import flashggFinalEGamma

from flashgg.MicroAOD.flashggObjectSelectors_cff import selectedFlashggMuons,selectedFlashggElectrons, selectedFlashggPhotons#, selecte
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
                                        +(flashggElectrons*selectedFlashggElectrons)
                                        +(flashggMuons*selectedFlashggMuons)
                                        +flashggMicroAODGenSequence
                                        )
                                       *flashggPhotons*selectedFlashggPhotons*flashggDiPhotons
                                       *(flashggPreselectedDiPhotons+flashggVertexMapForCHS)
                                       *flashggFinalEGamma
                                       )
