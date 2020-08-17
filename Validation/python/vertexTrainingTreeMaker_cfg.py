import FWCore.ParameterSet.Config as cms
import FWCore.Utilities.FileUtils as FileUtils

process = cms.Process("vertexTrainingTreeMaker")

process.load("FWCore.MessageService.MessageLogger_cfi")

# geometry and global tag:

process.load("Configuration.StandardSequences.GeometryDB_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.GlobalTag.globaltag = 'POSTLS170_V5::All'

#**************************************************************

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )


readFiles = cms.untracked.vstring()
secFiles = cms.untracked.vstring()
process.source = cms.Source ("PoolSource",fileNames = readFiles, secondaryFileNames = secFiles)

readFiles.extend( [
       '/store/mc/Phys14DR/GluGluToHToGG_M-125_13TeV-powheg-pythia6/MINIAODSIM/PU20bx25_tsg_PHYS14_25_V1-v1/00000/3C2EFAB1-B16F-E411-AB34-7845C4FC39FB.root',
       '/store/mc/Phys14DR/GluGluToHToGG_M-125_13TeV-powheg-pythia6/MINIAODSIM/PU20bx25_tsg_PHYS14_25_V1-v1/00000/7440A69A-4F70-E411-93D3-00A0D1EE273C.root',
       '/store/mc/Phys14DR/GluGluToHToGG_M-125_13TeV-powheg-pythia6/MINIAODSIM/PU20bx25_tsg_PHYS14_25_V1-v1/00000/E270C596-4F70-E411-9EB9-7845C4FC347F.root' ] );

secFiles.extend( [
               ] )

#**************************************************************

process.load("flashgg/MicroAOD/flashggMicroAODSequence_cff")

process.flashggDiPhotons.nVtxSaveInfo = cms.untracked.uint32(999) 

process.commissioning = cms.EDAnalyzer('vertexTrainingTreeMaker',
                                       PhotonTag=cms.untracked.InputTag('flashggPhotons'),
                                       DiPhotonTag = cms.InputTag('flashggDiPhotons'),
                                       VertexTag=cms.untracked.InputTag('offlineSlimmedPrimaryVertices'),
                                       VertexCandidateMapTagDz=cms.InputTag('flashggVertexMapUnique'),
                                       ConversionTag=cms.untracked.InputTag("reducedEgamma","reducedConversions"), 
                                       BeamSpotTag=cms.untracked.InputTag('offlineBeamSpot'),
)

#**************************************************************

process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string("test.root")
)

process.p = cms.Path(process.flashggMicroAODSequence*process.commissioning)
