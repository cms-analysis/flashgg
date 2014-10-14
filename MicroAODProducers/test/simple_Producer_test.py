import FWCore.ParameterSet.Config as cms
import FWCore.Utilities.FileUtils as FileUtils

process = cms.Process("FLASHggMicroAOD")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.load("Configuration.StandardSequences.GeometryDB_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.GlobalTag.globaltag = 'POSTLS170_V5::All'

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32( 100 ) )
process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32( 100 )

# Uncomment the following if you notice you have a memory leak
# This is a lightweight tool to digg further
#process.SimpleMemoryCheck = cms.Service("SimpleMemoryCheck",
#                                        ignoreTotal = cms.untracked.int32(1),
#                                        monitorPssAndPrivate = cms.untracked.bool(True)
#                                       )

process.source = cms.Source("PoolSource",fileNames=cms.untracked.vstring("/store/cmst3/user/gpetrucc/miniAOD/v1/GluGluToHToGG_M-125_13TeV-powheg-pythia6_Flat20to50_PAT.root"))

process.load("flashgg/MicroAODProducers/flashggVertexMaps_cfi")
process.load("flashgg/MicroAODProducers/flashggPhotons_cfi")
process.load("flashgg/MicroAODProducers/flashggDiPhotons_cfi")
process.load("flashgg/MicroAODProducers/flashggPreselectedDiPhotons_cfi")
process.load("flashgg/MicroAODProducers/flashggJets_cfi")
process.load("flashgg/MicroAODProducers/flashggPrunedGenParticles_cfi")

from flashgg.MicroAODProducers.flashggMicroAODOutputCommands_cff import microAODDefaultOutputCommand

process.out = cms.OutputModule("PoolOutputModule", fileName = cms.untracked.string('myOutputFile.root'),
                               outputCommands = microAODDefaultOutputCommand
                               )

process.commissioning = cms.EDAnalyzer('flashggCommissioning',
                                       PhotonTag=cms.untracked.InputTag('flashggPhotons'),
                                       DiPhotonTag = cms.untracked.InputTag('flashggDiPhotons'),
                                       VertexTag=cms.untracked.InputTag('offlineSlimmedPrimaryVertices')
)

process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string("tree.root")
)

process.p = cms.Path(process.flashggVertexMapUnique*
                     process.flashggVertexMapNonUnique*
                     process.flashggPhotons*
                     process.flashggDiPhotons*
                     process.flashggPreselectedDiPhotons*
                     process.flashggPrunedGenParticles*
                     process.flashggJets*
                     process.commissioning
                    )

process.e = cms.EndPath(process.out)
