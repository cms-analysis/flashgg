import FWCore.ParameterSet.Config as cms
import FWCore.Utilities.FileUtils as FileUtils

process = cms.Process("flashggCommissioning")

process.load("FWCore.MessageService.MessageLogger_cfi")

# geometry and global tag:

process.load("Configuration.StandardSequences.GeometryDB_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.GlobalTag.globaltag = 'POSTLS170_V5::All'

#**************************************************************

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(500) )

inputlist = FileUtils.loadListFromFile('fileList_ggToH_125_13TeV.txt')
readFiles = cms.untracked.vstring( *inputlist)
process.source = cms.Source("PoolSource", fileNames = readFiles)

#**************************************************************

process.load("flashgg/MicroAOD/flashggPhotons_cfi")
process.load("flashgg/MicroAOD/flashggDiPhotons_cfi")
process.load("flashgg/MicroAOD/flashggTkVtxMap_cfi")

process.commissioning = cms.EDAnalyzer('flashggCommissioning',
                                       PhotonTag=cms.untracked.InputTag('flashggPhotons'),
                                       DiPhotonTag = cms.InputTag('flashggDiPhotons'),
                                       VertexTag=cms.untracked.InputTag('offlineSlimmedPrimaryVertices')
)

#**************************************************************

process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string("tree.root")
)

process.out = cms.OutputModule("PoolOutputModule", fileName = cms.untracked.string('test_microAOD_ggToH_125_13TeV.root'),
                               outputCommands = cms.untracked.vstring("drop *","keep *_flashgg*_*_*","keep *_offlineSlimmedPrimaryVertices_*_*")
)


process.p = cms.Path(process.flashggVertexMapUnique*process.flashggVertexMapNonUnique*process.flashggPhotons*process.flashggDiPhotons*process.commissioning)
process.e = cms.EndPath(process.out)
