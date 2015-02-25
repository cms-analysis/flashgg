import FWCore.ParameterSet.Config as cms
import FWCore.Utilities.FileUtils as FileUtils

process = cms.Process("FLASHggMicroAODAndTag")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.load("Configuration.StandardSequences.GeometryDB_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.GlobalTag.globaltag = 'POSTLS170_V5::All'

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32( -1) )
process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32( 1000 )

#process.source = cms.Source("PoolSource",fileNames=cms.untracked.vstring("/store/mc/Spring14miniaod/GluGluToHToGG_M-125_13TeV-powheg-pythia6/MINIAODSIM/PU20bx25_POSTLS170_V5-v2/00000/24926621-F11C-E411-AB9A-02163E008D0B.root"))
#process.source = cms.Source("PoolSource",fileNames=cms.untracked.vstring("/store/mc/Spring14miniaod/TTbarH_HToGG_M-125_13TeV_amcatnlo-pythia8-tauola/MINIAODSIM/PU20bx25_POSTLS170_V5-v1/00000/049C0F9C-E61E-E411-9388-D8D385AE8466.root"))                                                                                                                            
process.source = cms.Source("PoolSource",fileNames=cms.untracked.vstring("/store/group/phys_higgs/cmshgg/sethzenz/flashgg/HggPhys14/Phys14MicroAODV1/WH_ZH_HToGG_M-125_13TeV_pythia6/HggPhys14-Phys14MicroAODV1-v1-Phys14DR-PU20bx25_PHYS14_25_V1-v1/141219_110038/0000/myMicroAODOutputFile_1.root"))

process.load("flashgg/MicroAODProducers/flashggMicroAODSequence_cff")
process.load("flashgg/TagProducers/flashggTagSequence_cfi")
process.load("flashgg/TagProducers/flashggTagTester_cfi")

process.commissioning = cms.EDAnalyzer('FlashggFlashggTreeMakerWithTagSorter',
                                       VertexTag=cms.untracked.InputTag('offlineSlimmedPrimaryVertices'),
                                       VertexCandidateMapTagDz=cms.InputTag('flashggVertexMapUnique'),
                                       VertexCandidateMapTagAOD = cms.InputTag('flashggVertexMapValidator'),
                                       JetTagDz=cms.InputTag("flashggJets"),
                                       rhoFixedGridCollection=cms.InputTag('fixedGridRhoAll')
)

process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string("globeLikeTree.root")
)

#process.p = cms.Path(process.flashggMicroAODSequence*process.flashggTagSequence*process.flashggTagTester)
process.p = cms.Path(process.flashggTagSequence*process.commissioning)
