import FWCore.ParameterSet.Config as cms
import FWCore.Utilities.FileUtils as FileUtils

process = cms.Process("FLASHggTag")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.load("Configuration.StandardSequences.GeometryDB_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.GlobalTag.globaltag = 'POSTLS170_V5::All'
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(5000) )
#process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32( 100 )

# Uncomment the following if you notice you have a memory leak
# This is a lightweight tool to digg further
#process.SimpleMemoryCheck = cms.Service("SimpleMemoryCheck",
#                                        ignoreTotal = cms.untracked.int32(1),
#                                        monitorPssAndPrivate = cms.untracked.bool(True)
#                                       )

#process.source = cms.Source("PoolSource",fileNames=cms.untracked.vstring("/store/cmst3/user/gpetrucc/miniAOD/v1/GluGluToHToGG_M-125_13TeV-powheg-pythia6_Flat20to50_PAT.root"))

readFiles = cms.untracked.vstring()
secFiles = cms.untracked.vstring() 
process.source = cms.Source ("PoolSource",fileNames = readFiles, secondaryFileNames = secFiles)
#readFiles.extend( [
#       '/store/mc/Spring14miniaod/TTbarH_HToGG_M-125_13TeV_amcatnlo-pythia8-tauola/MINIAODSIM/PU20bx25_POSTLS170_V5-v1/00000/049C0F9C-E61E-E411-9388-D8D385AE8466.root',
#       '/store/mc/Spring14miniaod/TTbarH_HToGG_M-125_13TeV_amcatnlo-pythia8-tauola/MINIAODSIM/PU20bx25_POSTLS170_V5-v1/00000/2A906032-1729-E411-9AC8-D8D385AE888A.root',
#       '/store/mc/Spring14miniaod/TTbarH_HToGG_M-125_13TeV_amcatnlo-pythia8-tauola/MINIAODSIM/PU20bx25_POSTLS170_V5-v1/00000/628A7A57-1F1E-E411-B5D4-D8D385AF8ADC.root',
#       '/store/mc/Spring14miniaod/TTbarH_HToGG_M-125_13TeV_amcatnlo-pythia8-tauola/MINIAODSIM/PU20bx25_POSTLS170_V5-v1/00000/849507D8-201E-E411-B897-D8D385AF8ADC.root',
#       '/store/mc/Spring14miniaod/TTbarH_HToGG_M-125_13TeV_amcatnlo-pythia8-tauola/MINIAODSIM/PU20bx25_POSTLS170_V5-v1/00000/9CEA8583-FC28-E411-9D2E-D8D385AE8466.root' ] );

#readFiles.extend( [
#       '/store/mc/Spring14miniaod/WH_ZH_HToGG_M-125_13TeV_pythia6/MINIAODSIM/PU20bx25_POSTLS170_V5-v2/30000/06C055C2-2219-E411-B30E-02163E00A211.root',
#       '/store/mc/Spring14miniaod/WH_ZH_HToGG_M-125_13TeV_pythia6/MINIAODSIM/PU20bx25_POSTLS170_V5-v2/30000/9054DCC2-2219-E411-95F6-02163E00F057.root',
#       '/store/mc/Spring14miniaod/WH_ZH_HToGG_M-125_13TeV_pythia6/MINIAODSIM/PU20bx25_POSTLS170_V5-v2/30000/E0D066C6-2219-E411-BD9A-02163E00ECDF.root' ] )

readFiles.extend( [ 'file:myMicroAODOutputFile.root' ] ) # For default quick test, depend on output of simple_Producer_test.py


secFiles.extend( [
               ] )

#Tag stuff
process.load("flashgg/TagProducers/flashggDiPhotonMVA_cfi")
process.load("flashgg/TagProducers/flashggVBFMVA_cfi")
process.load("flashgg/TagProducers/flashggVBFDiPhoDiJetMVA_cfi")
process.load("flashgg/TagProducers/flashggTags_cfi")


from flashgg.TagProducers.flashggTagOutputCommands_cff import tagDefaultOutputCommand

process.out = cms.OutputModule("PoolOutputModule", fileName = cms.untracked.string('myTagOutputFile.root'),
                               outputCommands = tagDefaultOutputCommand			       
                               )


process.p = cms.Path((process.flashggDiPhotonMVA)*
                     (process.flashggVBFMVA)*
                     (process.flashggVBFDiPhoDiJetMVA)* # Needs to happen after VBF MVA and DiPho MVA
                     
                     # Tag producers, once written, can run in parallel, so they go in here with +
                     (process.flashggUntaggedCategory+process.flashggVBFTag+process.flashggTTHleptonicTag+process.flashggTTHhadronicTag+process.flashggVHlooseTag+process.flashggVHtightTag)
                    )

process.e = cms.EndPath(process.out)
