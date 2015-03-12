
import FWCore.ParameterSet.Config as cms
import FWCore.Utilities.FileUtils as FileUtils

process = cms.Process("FLASHggMicroAOD")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.load("Configuration.StandardSequences.GeometryDB_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.GlobalTag.globaltag = 'POSTLS170_V5::All'

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32( 1000) )
process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32( 1000 )

process.source = cms.Source("PoolSource",fileNames=cms.untracked.vstring("/store/mc/Phys14DR/GluGluToHToGG_M-125_13TeV-powheg-pythia6/MINIAODSIM/PU20bx25_tsg_PHYS14_25_V1-v1/00000/3C2EFAB1-B16F-E411-AB34-7845C4FC39FB.root"))

process.MessageLogger.cerr.threshold = 'ERROR' # can't get suppressWarning to work: disable all warnings for now
# process.MessageLogger.suppressWarning.extend(['SimpleMemoryCheck','MemoryCheck']) # this would have been better...

# Uncomment the following if you notice you have a memory leak
# This is a lightweight tool to digg further
#process.SimpleMemoryCheck = cms.Service("SimpleMemoryCheck",
#                                        ignoreTotal = cms.untracked.int32(1),
#                                        monitorPssAndPrivate = cms.untracked.bool(True)
#                                       )

process.load("flashgg/MicroAOD/flashggMicroAODSequence_cff")

## extra calo isolations scan
process.flashggPhotons.extraCaloIsolations.extend([
    ## veto regions scan
        cms.PSet(overlapRemoval=cms.bool(False),
                 type=cms.int32(4), # photon, use str to enum
                 vetoRegions=cms.vdouble(0.3,0.,0.05,0.01,0.,0.,0.)
                 ),
        cms.PSet(overlapRemoval=cms.bool(False),
                 type=cms.int32(4), # photon, use str to enum
                 vetoRegions=cms.vdouble(0.3,0.,0.07,0.015,0.,0.,0.)
                 ),
        cms.PSet(overlapRemoval=cms.bool(False),
                 type=cms.int32(4), # photon, use str to enum
                 vetoRegions=cms.vdouble(0.3,0.,0.09,0.02,0.,0.,0.)
                 ),
        cms.PSet(overlapRemoval=cms.bool(False),
                 type=cms.int32(4), # photon, use str to enum
                 vetoRegions=cms.vdouble(0.3,0.,0.11,0.03,0.,0.,0.)
                 ),
        cms.PSet(overlapRemoval=cms.bool(False),
                 type=cms.int32(4), # photon, use str to enum
                 vetoRegions=cms.vdouble(0.3,0.,0.13,0.04,0.,0.,0.)
                 ),
        cms.PSet(overlapRemoval=cms.bool(False),
                 type=cms.int32(4), # photon, use str to enum
                 vetoRegions=cms.vdouble(0.3,0.,0.15,0.05,0.,0.,0.)
                 ),

    ## limited overlap removal scan
        cms.PSet(overlapRemoval=cms.bool(True),
                 type=cms.int32(4), # photon, use str to enum
                 vetoRegions=cms.vdouble(0.3,0.0,0.0,0.,0.,0.,0.)
                 ),
        cms.PSet(overlapRemoval=cms.bool(True),
                 type=cms.int32(4), # photon, use str to enum
                 vetoRegions=cms.vdouble(0.3,0.0,0.1,0.1,0.,0.,0.)
                 ),
        cms.PSet(overlapRemoval=cms.bool(True),
                 type=cms.int32(4), # photon, use str to enum
                 vetoRegions=cms.vdouble(0.3,0.0,0.2,0.2,0.,0.,0.)
                 ),
        cms.PSet(overlapRemoval=cms.bool(False),
                 type=cms.int32(4), # photon, use str to enum
                 vetoRegions=cms.vdouble(0.3,0.0,0.0,0.,0.,0.,0.)
                 ),
        cms.PSet(overlapRemoval=cms.bool(False),
                 type=cms.int32(4), # photon, use str to enum
                 vetoRegions=cms.vdouble(0.3,0.0,0.1,0.1,0.,0.,0.)
                 ),
        cms.PSet(overlapRemoval=cms.bool(False),
                 type=cms.int32(4), # photon, use str to enum
                 vetoRegions=cms.vdouble(0.3,0.0,0.2,0.2,0.,0.,0.)
                 ),
])


from flashgg.MicroAOD.flashggMicroAODOutputCommands_cff import microAODDefaultOutputCommand,microAODDebugOutputCommand
process.out = cms.OutputModule("PoolOutputModule", fileName = cms.untracked.string('myMicroAODOutputFile.root'),
                               outputCommands = microAODDefaultOutputCommand
                               )
process.out.outputCommands += microAODDebugOutputCommand # extra items for debugging, CURRENTLY REQUIRED

process.p = cms.Path(process.flashggMicroAODSequence)
process.e = cms.EndPath(process.out)

