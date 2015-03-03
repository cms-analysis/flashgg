import FWCore.ParameterSet.Config as cms
import FWCore.Utilities.FileUtils as FileUtils

process = cms.Process("FLASHggMicroAOD")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.load("Configuration.StandardSequences.GeometryDB_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.GlobalTag.globaltag = 'POSTLS170_V5::All'

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(900) )
process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32( 1 )

readFiles = cms.untracked.vstring()
secFiles = cms.untracked.vstring() 
process.source = cms.Source ("PoolSource",fileNames = readFiles, secondaryFileNames = secFiles, skipEvents=cms.untracked.uint32(0))
#readFiles.extend( [
#       '/store/mc/Phys14DR/WH_ZH_HToGG_M-125_13TeV_pythia6/MINIAODSIM/PU20bx25_PHYS14_25_V1-v1/00000/24B70163-5769-E411-93CA-002590200A28.root',
#       '/store/mc/Phys14DR/WH_ZH_HToGG_M-125_13TeV_pythia6/MINIAODSIM/PU20bx25_PHYS14_25_V1-v1/00000/E6E81FBF-5769-E411-AD3B-001E6739801B.root' ] );

readFiles.extend( [
       '/store/mc/Phys14DR/TTbarH_HToGG_M-125_13TeV_amcatnlo-pythia8-tauola/MINIAODSIM/PU20bx25_PHYS14_25_V1-v1/00000/9606B5EA-F370-E411-9DD1-002590200978.root',
       '/store/mc/Phys14DR/TTbarH_HToGG_M-125_13TeV_amcatnlo-pythia8-tauola/MINIAODSIM/PU20bx25_PHYS14_25_V1-v1/00000/C4B23A70-E170-E411-B90A-001E67398683.root',
       '/store/mc/Phys14DR/TTbarH_HToGG_M-125_13TeV_amcatnlo-pythia8-tauola/MINIAODSIM/PU20bx25_PHYS14_25_V1-v1/10000/7CD4EFBC-9C70-E411-94A0-002590200828.root',
       '/store/mc/Phys14DR/TTbarH_HToGG_M-125_13TeV_amcatnlo-pythia8-tauola/MINIAODSIM/PU20bx25_PHYS14_25_V1-v1/10000/A45B04BD-9C70-E411-933C-002590200A94.root' ] );

#readFiles.extend( [
#       '/store/mc/Phys14DR/GluGluToHToGG_M-125_13TeV-powheg-pythia6/MINIAODSIM/AVE30BX50_tsg_PHYS14_ST_V1-v3/40000/52640FE8-CFA0-E411-BD06-7845C4FC3C02.root',
#       '/store/mc/Phys14DR/GluGluToHToGG_M-125_13TeV-powheg-pythia6/MINIAODSIM/AVE30BX50_tsg_PHYS14_ST_V1-v3/40000/6C9E8EE6-D0A0-E411-8F91-7845C4FC3A94.root',
#       '/store/mc/Phys14DR/GluGluToHToGG_M-125_13TeV-powheg-pythia6/MINIAODSIM/AVE30BX50_tsg_PHYS14_ST_V1-v3/40000/D483920B-92A0-E411-85DE-848F69FD294F.root' ] );
secFiles.extend( [
               ] )

#process.source = cms.Source("PoolSource",fileNames=cms.untracked.vstring("/store/mc/Spring14miniaod/GluGluToHToGG_M-125_13TeV-powheg-pythia6/MINIAODSIM/PU20bx25_POSTLS170_V5-v2/00000/24926621-F11C-E411-AB9A-02163E008D0B.root"),  skipEvents=cms.untracked.uint32(0))
#process.source = cms.Source("PoolSource",fileNames=cms.untracked.vstring("/store/mc/Spring14miniaod/TTbarH_HToGG_M-125_13TeV_amcatnlo-pythia8-tauola/MINIAODSIM/PU20bx25_POSTLS170_V5-v1/00000/049C0F9C-E61E-E411-9388-D8D385AE8466.root"))                                                                                                                            
##  process.source = cms.Source("PoolSource",fileNames=cms.untracked.vstring("/store/mc/Spring14miniaod/WH_ZH_HToGG_M-125_13TeV_pythia6/MINIAODSIM/PU20bx25_POSTLS170_V5-v2/30000/E0D066C6-2219-E411-BD9A-02163E00ECDF.root"))

#process.source = cms.Source("PoolSource",fileNames=cms.untracked.vstring("/store/mc/Phys14DR/GluGluToHToGG_M-125_13TeV-powheg-pythia6/MINIAODSIM/PU20bx25_tsg_PHYS14_25_V1-v1/00000/3C2EFAB1-B16F-E411-AB34-7845C4FC39FB.root"),
# 			    skipEvents=cms.untracked.uint32(0)

#)
### process.source = cms.Source("PoolSource",fileNames=cms.untracked.vstring("/store/mc/Phys14DR/GJet_Pt40_doubleEMEnriched_TuneZ2star_13TeV-pythia6/MINIAODSIM/PU20bx25_PHYS14_25_V1-v1/00000/101611CC-026E-E411-B8D7-00266CFFBF88.root"))
## process.source = cms.Source("PoolSource",fileNames=cms.untracked.vstring("/store/mc/Phys14DR/GJets_HT-100to200_Tune4C_13TeV-madgraph-tauola/MINIAODSIM/PU20bx25_PHYS14_25_V1-v1/00000/00D67F78-2873-E411-B3BB-0025907DC9C0.root"))

process.MessageLogger.cerr.threshold = 'ERROR' # can't get suppressWarning to work: disable all warnings for now
# process.MessageLogger.suppressWarning.extend(['SimpleMemoryCheck','MemoryCheck']) # this would have been better...

# Uncomment the following if you notice you have a memory leak
# This is a lightweight tool to digg further
#process.SimpleMemoryCheck = cms.Service("SimpleMemoryCheck",
#                                        ignoreTotal = cms.untracked.int32(1),
#                                        monitorPssAndPrivate = cms.untracked.bool(True)
#                                       )

process.load("flashgg/MicroAODProducers/flashggMicroAODSequence_cff")

from flashgg.MicroAODProducers.flashggMicroAODOutputCommands_cff import microAODDefaultOutputCommand,microAODDebugOutputCommand
process.out = cms.OutputModule("PoolOutputModule", fileName = cms.untracked.string('myMicroAOD_test.root'),
                                outputCommands = microAODDefaultOutputCommand
                               )
process.out.outputCommands += microAODDebugOutputCommand # extra items for debugging, CURRENTLY REQUIRED

process.p = cms.Path(process.flashggMicroAODSequence)
process.e = cms.EndPath(process.out)

# Uncomment these lines to run the example commissioning module and send its output to root
#process.commissioning = cms.EDAnalyzer('flashggCommissioning',
#                                       PhotonTag=cms.untracked.InputTag('flashggPhotons'),
#                                       DiPhotonTag = cms.untracked.InputTag('flashggDiPhotons'),
#                                       VertexTag=cms.untracked.InputTag('offlineSlimmedPrimaryVertices')
#)
#process.TFileService = cms.Service("TFileService",
#                                   fileName = cms.string("commissioningTree.root")
#)
#process.p *= process.commissioning
