import FWCore.ParameterSet.Config as cms
import FWCore.Utilities.FileUtils as FileUtils

process = cms.Process("FLASHggMicroAODAndTag")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.load("Configuration.StandardSequences.GeometryDB_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.GlobalTag.globaltag = 'POSTLS170_V5::All'

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32( 1000) )
process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32( 1000 )

#process.source = cms.Source("PoolSource",fileNames=cms.untracked.vstring("/store/mc/Spring14miniaod/GluGluToHToGG_M-125_13TeV-powheg-pythia6/MINIAODSIM/PU20bx25_POSTLS170_V5-v2/00000/24926621-F11C-E411-AB9A-02163E008D0B.root"))
#process.source = cms.Source("PoolSource",fileNames=cms.untracked.vstring("/store/mc/Spring14miniaod/TTbarH_HToGG_M-125_13TeV_amcatnlo-pythia8-tauola/MINIAODSIM/PU20bx25_POSTLS170_V5-v1/00000/049C0F9C-E61E-E411-9388-D8D385AE8466.root"))                                                                                                                            
process.source = cms.Source("PoolSource",fileNames=cms.untracked.vstring("/store/mc/Spring14miniaod/WH_ZH_HToGG_M-125_13TeV_pythia6/MINIAODSIM/PU20bx25_POSTLS170_V5-v2/30000/E0D066C6-2219-E411-BD9A-02163E00ECDF.root"))

process.load("flashgg/MicroAODProducers/flashggMicroAODSequence_cff")
process.load("flashgg/TagProducers/flashggTagSequence_cfi")
process.load("flashgg/TagProducers/flashggTagTester_cfi")

from flashgg.MicroAODProducers.flashggMicroAODOutputCommands_cff import microAODDefaultOutputCommand,microAODDebugOutputCommand
from flashgg.TagProducers.flashggTagOutputCommands_cff import tagDefaultOutputCommand
process.out = cms.OutputModule("PoolOutputModule", fileName = cms.untracked.string('myMicroAODOutputFile.root'),
                               outputCommands = microAODDefaultOutputCommand+tagDefaultOutputCommand
                               )
process.out.outputCommands += microAODDebugOutputCommand # extra items for debugging, CURRENTLY REQUIRED

process.p = cms.Path(process.flashggMicroAODSequence*process.flashggTagSequence*process.flashggTagTester)
process.e = cms.EndPath(process.out)
