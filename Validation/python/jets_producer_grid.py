import FWCore.ParameterSet.Config as cms
import FWCore.Utilities.FileUtils as FileUtils
 
process = cms.Process("FLASHggMicroAOD")
 
process.load("FWCore.MessageService.MessageLogger_cfi")
process.load("Configuration.StandardSequences.GeometryDB_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")

process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_mc')

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32( 100 ) )
process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32( 100 )

# PHYS14 Files
process.source = cms.Source("PoolSource",fileNames=cms.untracked.vstring(
    "/store/mc/Phys14DR/GluGluToHToGG_M-125_13TeV-powheg-pythia6/MINIAODSIM/PU20bx25_tsg_PHYS14_25_V1-v1/00000/3C2EFAB1-B16F-E411-AB34-7845C4FC39FB.root"
))

process.MessageLogger.cerr.threshold = 'ERROR' # can't get suppressWarning to work: disable all warnings for now

process.load("flashgg/MicroAOD/flashggMicroAODSequence_cff")
process.load("flashgg/MicroAOD/flashggMicroAODExtraJetsSequence_cff")

from flashgg.MicroAOD.flashggMicroAODOutputCommands_cff import microAODDefaultOutputCommand,microAODDebugOutputCommand
process.out = cms.OutputModule("PoolOutputModule",
                               fileName       = cms.untracked.string('myMicroAODOutputFile.root'),
                               outputCommands = microAODDefaultOutputCommand
                           )

process.out.outputCommands += microAODDebugOutputCommand # extra items for debugging, CURRENTLY REQUIRED
 
# need to allow unscheduled processes otherwise reclustering function will fail
# this is because of the jet clustering tool, and we have to live with it for now.
process.options = cms.untracked.PSet(
    allowUnscheduled = cms.untracked.bool(True)
)

# ++++++++++++ PF +++++++++++++++++++
# uses the vertex 0
from flashgg.MicroAOD.flashggExtraJets_cfi import addFlashggPF
addFlashggPF(process)
# +++++++++++++++++++++++++++++++++++

# ++++++++++++ PFCHS0 +++++++++++++++
# uses the vertex 0
from flashgg.MicroAOD.flashggExtraJets_cfi import addFlashggPFCHS0
addFlashggPFCHS0(process)
# +++++++++++++++++++++++++++++++++++

# ++++++++++++ PFCHSLeg +++++++++++++
# import function which takes care of reclustering the jets using legacy vertex     
from flashgg.MicroAOD.flashggJets_cfi import addFlashggPFCHSLegJets 
# call the function, it takes care of everything else.
addFlashggPFCHSLegJets(process)
# +++++++++++++++++++++++++++++++++++

process.p = cms.Path(process.flashggMicroAODSequence
                     + process.flashggMicroAODExtraJetsSequence
                 )

process.e = cms.EndPath(process.out)

from flashgg.MicroAOD.MicroAODCustomize import customize
customize(process)
