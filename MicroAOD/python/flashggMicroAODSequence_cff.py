import FWCore.ParameterSet.Config as cms
from flashgg.MicroAOD.flashggTkVtxMap_cfi import flashggVertexMapUnique,flashggVertexMapNonUnique,flashggVertexMapForCHS,flashggVertexMapForPUPPI
from flashgg.MicroAOD.flashggPhotons_cfi import flashggPhotons
from flashgg.MicroAOD.flashggDiPhotons_cfi import flashggDiPhotons
from flashgg.MicroAOD.flashggPreselectedDiPhotons_cfi import flashggPreselectedDiPhotons
from flashgg.MicroAOD.flashggJets_cfi import flashggFinalJets,flashggFinalPuppiJets
from flashgg.MicroAOD.flashggElectrons_cfi import flashggElectrons
from flashgg.MicroAOD.flashggMuons_cfi import flashggMuons
from flashgg.MicroAOD.flashggFinalEGamma_cfi import flashggFinalEGamma

from flashgg.MicroAOD.flashggLeptonSelectors_cff import flashggSelectedMuons,flashggSelectedElectrons
from flashgg.MicroAOD.flashggMicroAODGenSequence_cff import *

from PhysicsTools.SelectorUtils.centralIDRegistry import central_id_registry
from RecoEgamma.ElectronIdentification.ElectronMVAValueMapProducer_cfi import *



eventCount = cms.EDProducer("EventCountProducer")
weightsCount = cms.EDProducer("WeightsCountProducer",
                              generator=cms.InputTag("generator"),
                              pileupInfo=cms.InputTag("addPileupInfo"),
                              doObsPileup=cms.untracked.bool(True),
                              minObsPileup=cms.double(-0.5),
                              maxObsPileup=cms.double(100.5),
                              nbinsObsPileup=cms.int32(101),
                              )

flashggMicroAODSequence = cms.Sequence(eventCount+weightsCount
                                       +flashggVertexMapUnique+flashggVertexMapNonUnique
                                       +electronMVAValueMapProducer*flashggElectrons*flashggSelectedElectrons
                                       +flashggMuons*flashggSelectedMuons
                                       +flashggMicroAODGenSequence
                                       +flashggPhotons*flashggDiPhotons*flashggPreselectedDiPhotons
                                       +flashggFinalEGamma
                                       +flashggVertexMapForCHS*flashggFinalJets
                                       +flashggVertexMapForPUPPI*flashggFinalPuppiJets
                                       )
