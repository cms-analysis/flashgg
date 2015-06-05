import FWCore.ParameterSet.Config as cms
import FWCore.Utilities.FileUtils as FileUtils

process = cms.Process("vertexProbTrainingTreeMaker")

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

#chnage with input produced with CMSSW 7_4 release
readFiles.extend( [
       '/store/mc/Phys14DR/GluGluToHToGG_M-125_13TeV-powheg-pythia6/MINIAODSIM/PU20bx25_tsg_PHYS14_25_V1-v1/00000/3C2EFAB1-B16F-E411-AB34-7845C4FC39FB.root',
       '/store/mc/Phys14DR/GluGluToHToGG_M-125_13TeV-powheg-pythia6/MINIAODSIM/PU20bx25_tsg_PHYS14_25_V1-v1/00000/7440A69A-4F70-E411-93D3-00A0D1EE273C.root',
       '/store/mc/Phys14DR/GluGluToHToGG_M-125_13TeV-powheg-pythia6/MINIAODSIM/PU20bx25_tsg_PHYS14_25_V1-v1/00000/E270C596-4F70-E411-9EB9-7845C4FC347F.root' ] );

secFiles.extend( [
               ] )


#**************************************************************


process.load("flashgg/MicroAOD/flashggMicroAODSequence_cff")
process.flashggDiPhotons.nVtxSaveInfo = cms.untracked.uint32(999) 
process.flashggDiPhotons.convProbCut = cms.untracked.double(1.0E-6) 
process.flashggDiPhotons.vertexIdMVAweightfile = cms.FileInPath("flashgg/MicroAOD/data/TMVAClassification_BDTVtxId_noSL_2015.xml")
process.flashggDiPhotons.vertexProbMVAweightfile = cms.FileInPath("flashgg/MicroAOD/data/TMVAClassification_BDTVtxProb_noSL_2015.xml")
process.flashggDiPhotons.useSingleLeg=cms.untracked.bool(False)

process.commissioning = cms.EDAnalyzer('vertexProbTrainingTreeMaker',
                                       PhotonTag=cms.untracked.InputTag('flashggPhotons'),
                                       DiPhotonTag = cms.untracked.InputTag('flashggPreselectedDiPhotons'),
                                       VertexTag=cms.untracked.InputTag('offlineSlimmedPrimaryVertices'),
                                       VertexCandidateMapTagDz=cms.InputTag('flashggVertexMapUnique'),
                                       ConversionTag=cms.untracked.InputTag("reducedEgamma","reducedConversions"), 
                                       BeamSpotTag=cms.untracked.InputTag('offlineBeamSpot'),
                                       evWeight = cms.untracked.double(1.0)
)

#**************************************************************

process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string("vtxProbTest.root")
)

process.p = cms.Path(process.flashggMicroAODSequence*process.commissioning)
