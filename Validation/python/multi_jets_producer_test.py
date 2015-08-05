import FWCore.ParameterSet.Config as cms
import FWCore.Utilities.FileUtils as FileUtils
 
process = cms.Process("FLASHggMicroAOD")
 
process.load("FWCore.MessageService.MessageLogger_cfi")
 
process.load("Configuration.StandardSequences.GeometryDB_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")

from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_mc')

process.maxEvents  = cms.untracked.PSet( input = cms.untracked.int32( 100 ) )
process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32( 100 )


jdebug=True

# PHYS14 Files
process.source = cms.Source("PoolSource",fileNames=cms.untracked.vstring("/store/mc/RunIISpring15DR74/VBFHToGG_M-125_13TeV_powheg_pythia8/MINIAODSIM/Asympt50ns_MCRUN2_74_V9A-v1/50000/049AAFAA-CA2D-E511-93E8-02163E00F402.root"),
                            skipEvents=cms.untracked.uint32(3)
                            
)

process.MessageLogger.cerr.threshold = 'ERROR' # can't get suppressWarning to work: disable all warnings for now
# process.MessageLogger.suppressWarning.extend(['SimpleMemoryCheck','MemoryCheck']) # this would have been better...
# Uncomment the following if you notice you have a memory leak
# This is a lightweight tool to digg further

process.SimpleMemoryCheck = cms.Service("SimpleMemoryCheck",
                                        ignoreTotal = cms.untracked.int32(1),
                                        monitorPssAndPrivate = cms.untracked.bool(True)
                                    )

process.load("flashgg/MicroAOD/flashggMicroAODSequence_cff")
#process.load("flashgg/MicroAOD/flashggMicroAODExtraJetsSequence_cff")
#process.load("flashgg/MicroAOD/flashggMicroAODMultiJetsSequence_cff")

from flashgg.MicroAOD.flashggMicroAODOutputCommands_cff import microAODDefaultOutputCommand,microAODDebugOutputCommand
process.out = cms.OutputModule("PoolOutputModule",
                               fileName       = cms.untracked.string('workspace/myMicroAODOutputFile.root'),
                               outputCommands = microAODDefaultOutputCommand
                           )

process.out.outputCommands += microAODDebugOutputCommand # extra items for debugging, CURRENTLY REQUIRED

# need to allow unscheduled processes otherwise reclustering function will fail
# this is because of the jet clustering tool, and we have to live with it for now.
# NOTE: new PAT default running is "unscheduled" so we just need to say in the outputCommands what we want to store

process.options = cms.untracked.PSet(
    allowUnscheduled = cms.untracked.bool(True)
)
process.TFileService = cms.Service("TFileService",
                                   fileName  = cms.string("./workspace/test.root"))



from flashgg.MicroAOD.flashggExtraJets_cfi import addFlashggPF
from flashgg.MicroAOD.flashggJets_cfi      import addFlashggPFCHSJets, addFlashggPuppiJets

print ':: process ==', process
addFlashggPF           (process = process, doQGTagging = True, label = '')


for vtx in range(0,5):
    # chs
#addFlashggPFCHSJets (process = process,
#                     vertexIndex =vtx,
#                     doQGTagging = True,
#                     debug       = True,
#                     label = '' + str(vtx))
# puppi 
    addFlashggPuppiJets (process     = process,
                         #'out'
                         vertexIndex = vtx,
                         label = '' + str(vtx)) 
    
    
#newseq = cloneProcessingSnippet(process,process.currentMicroAODExtraJetsSequence,'flashggJetsPF')
#currentMicroAODExtraJetsSequence = cms.Sequence( )# addFlashggPF + addFlashggPFCHS0)

# This producer is the default in flashgg Dijet MVA 
#process.flashggJetValidationTreeMakerPFCHSLeg = cms.EDAnalyzer('FlashggJetValidationTreeMaker',
#                                                               GenParticleTag        = cms.untracked.InputTag('prunedGenParticles'),
#                                                               JetTagDz              = cms.InputTag("flashggJets"),                  
#                                                               StringTag	     = cms.string  ("PFCHSLeg"),
#                                                               VertexCandidateMapTag = cms.InputTag("flashggVertexMapUnique"),
#                                                               qgVariablesInputTag   = cms.InputTag('QGTaggerPFCHSLeg'),
#                                                               DiPhotonTag           = cms.InputTag("flashggDiPhotons"),
#                                                               debug                 = cms.untracked.bool(jdebug),
#                                                           )
#


process.p = cms.Path(  process.flashggMicroAODSequence )

process.e = cms.EndPath(process.out)

from flashgg.MicroAOD.MicroAODCustomize import customize
customize(process)
