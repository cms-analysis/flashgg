import FWCore.ParameterSet.Config as cms
import FWCore.Utilities.FileUtils as FileUtils

process = cms.Process("METTest")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.load("Configuration.StandardSequences.GeometryDB_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
from Configuration.AlCa.GlobalTag import GlobalTag

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32( 100) )
process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32( 1000 )

process.GlobalTag = GlobalTag(process.GlobalTag,'80X_mcRun2_asymptotic_2016_miniAODv2')

process.source = cms.Source("PoolSource",
                            inputCommands = cms.untracked.vstring( 'keep *',
                                                                   ),
                            fileNames = cms.untracked.vstring("/store/group/phys_higgs/cmshgg/ferriff/flashgg/RunIISpring16DR80X-2_1_0-25ns_ICHEP16/2_1_0/GluGluHToGG_M-120_13TeV_powheg_pythia8/RunIISpring16DR80X-2_1_0-25ns_ICHEP16-2_1_0-v0-RunIISpring16MiniAODv1-PUSpring16RAWAODSIM_80X_mcRun2_asymptotic_2016_v3-v1/160618_074300/0000/myMicroAODOutputFile_1.root"),

)
process.metTest = cms.EDAnalyzer("metFilterTest",
                                 bits = cms.InputTag('TriggerResults::PAT')
                                 
                                 )
process.p = cms.Path(process.metTest)
