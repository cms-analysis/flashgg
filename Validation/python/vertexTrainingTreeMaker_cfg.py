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


process.source = cms.Source("PoolSource",fileNames=cms.untracked.vstring("/store/cmst3/user/gpetrucc/miniAOD/v1/GluGluToHToGG_M-125_13TeV-powheg-pythia6_Flat20to50_PAT_big.root"))

#**************************************************************

process.load("flashgg/MicroAODProducers/flashggPhotons_cfi")
process.load("flashgg/MicroAODProducers/flashggDiPhotons_cfi")
process.load("flashgg/MicroAODProducers/flashggTkVtxMap_cfi")

process.flashggDiPhotons.nVtxSaveInfo = cms.untracked.uint32(999) 

process.commissioning = cms.EDAnalyzer('vertexTrainingTreeMaker',
                                       PhotonTag=cms.untracked.InputTag('flashggPhotons'),
                                       DiPhotonTag = cms.untracked.InputTag('flashggDiPhotons'),
                                       VertexTag=cms.untracked.InputTag('offlineSlimmedPrimaryVertices'),
                                       VertexCandidateMapTagDz=cms.InputTag('flashggVertexMapUnique'),
                                       ConversionTag=cms.untracked.InputTag("reducedEgamma","reducedConversions"), 
                                       BeamSpotTag=cms.untracked.InputTag('offlineBeamSpot'),
)

#**************************************************************

process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string("VertexMVATraining.root")
)

process.p = cms.Path(process.flashggVertexMapUnique*process.flashggVertexMapNonUnique*process.flashggPhotons*process.flashggDiPhotons*process.commissioning)

