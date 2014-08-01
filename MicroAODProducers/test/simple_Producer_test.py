import FWCore.ParameterSet.Config as cms
import FWCore.Utilities.FileUtils as FileUtils

process = cms.Process("FLASHggMicroAOD")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.load("Configuration.StandardSequences.GeometryDB_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.GlobalTag.globaltag = 'POSTLS170_V5::All'

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.source = cms.Source("PoolSource",fileNames=cms.untracked.vstring(
        "file:/afs/cern.ch/work/s/sethzenz/public/Hgg_miniAOD_run0/miniAOD_1.root",
        "file:/afs/cern.ch/work/s/sethzenz/public/Hgg_miniAOD_run0/miniAOD_2.root",
        "file:/afs/cern.ch/work/s/sethzenz/public/Hgg_miniAOD_run0/miniAOD_3.root",
        "file:/afs/cern.ch/work/s/sethzenz/public/Hgg_miniAOD_run0/miniAOD_4.root",
        "file:/afs/cern.ch/work/s/sethzenz/public/Hgg_miniAOD_run0/miniAOD_5.root",
        "file:/afs/cern.ch/work/s/sethzenz/public/Hgg_miniAOD_run0/miniAOD_6.root",
        "file:/afs/cern.ch/work/s/sethzenz/public/Hgg_miniAOD_run0/miniAOD_7.root",
        "file:/afs/cern.ch/work/s/sethzenz/public/Hgg_miniAOD_run0/miniAOD_8.root",
        "file:/afs/cern.ch/work/s/sethzenz/public/Hgg_miniAOD_run0/miniAOD_9.root",
        "file:/afs/cern.ch/work/s/sethzenz/public/Hgg_miniAOD_run0/miniAOD_10.root",
        "file:/afs/cern.ch/work/s/sethzenz/public/Hgg_miniAOD_run0/miniAOD_11.root",
        "file:/afs/cern.ch/work/s/sethzenz/public/Hgg_miniAOD_run0/miniAOD_12.root",
        "file:/afs/cern.ch/work/s/sethzenz/public/Hgg_miniAOD_run0/miniAOD_13.root",
        "file:/afs/cern.ch/work/s/sethzenz/public/Hgg_miniAOD_run0/miniAOD_14.root",
        "file:/afs/cern.ch/work/s/sethzenz/public/Hgg_miniAOD_run0/miniAOD_15.root",
        "file:/afs/cern.ch/work/s/sethzenz/public/Hgg_miniAOD_run0/miniAOD_16.root",
        "file:/afs/cern.ch/work/s/sethzenz/public/Hgg_miniAOD_run0/miniAOD_17.root",
        "file:/afs/cern.ch/work/s/sethzenz/public/Hgg_miniAOD_run0/miniAOD_18.root",
        "file:/afs/cern.ch/work/s/sethzenz/public/Hgg_miniAOD_run0/miniAOD_19.root"
        ))

# Each track associated only to the closest vertex (or none if dZ >= MaxAllowedDz for all vertices)
process.flashggVertexMapUnique = cms.EDProducer('FlashggDzVertexMapProducer',
                                                PFCandidatesTag=cms.untracked.InputTag('packedPFCandidates'),
                                                VertexTag=cms.untracked.InputTag('offlineSlimmedPrimaryVertices'),
                                                MaxAllowedDz=cms.double(0.2) # in cm
                                                )

# Tracks will show up as associated to every vertex for which dZ < MaxAllowedDz
process.flashggVertexMapNonUnique = cms.EDProducer('FlashggDzVertexMapProducer',
                                                   PFCandidatesTag=cms.untracked.InputTag('packedPFCandidates'),
                                                   VertexTag=cms.untracked.InputTag('offlineSlimmedPrimaryVertices'),
                                                   MaxAllowedDz=cms.double(0.2), # in cm
                                                   UseEachTrackOnce=cms.untracked.bool(False)
                                                   )

process.load("flashgg/MicroAODProducers/flashggPhotons_cfi")
process.load("flashgg/MicroAODProducers/flashggDiPhotons_cfi")
process.load("flashgg/MicroAODProducers/flashggPreselectedDiPhotons_cfi")
                                                 

process.out = cms.OutputModule("PoolOutputModule", fileName = cms.untracked.string('myOutputFile.root'),
                               outputCommands = cms.untracked.vstring("drop *",
                                                                      "keep *_flashgg*_*_*",
                                                                      "drop *_flashggVertexMap*_*_*",
                                                                      "keep *_offlineSlimmedPrimaryVertices_*_*")
)

process.p = cms.Path(process.flashggVertexMapUnique*
                     process.flashggVertexMapNonUnique*
                     process.flashggPhotons*
                     process.flashggDiPhotons*
                     process.flashggPreselectedDiPhotons
                    )

process.e = cms.EndPath(process.out)
