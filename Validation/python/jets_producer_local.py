import FWCore.ParameterSet.Config as cms
import FWCore.Utilities.FileUtils as FileUtils
 
process = cms.Process("FLASHggMicroAOD")
 
process.load("FWCore.MessageService.MessageLogger_cfi")
 
process.load("Configuration.StandardSequences.GeometryDB_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")

from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_mc')

process.maxEvents  = cms.untracked.PSet( input = cms.untracked.int32( 500 ) )
process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32( 100 )


jdebug=False

# PHYS14 Files
process.source = cms.Source("PoolSource",fileNames=cms.untracked.vstring(
    #"/store/mc/Phys14DR/DYToMuMu_M-50_Tune4C_13TeV-pythia8/MINIAODSIM/PU40bx25_tsg_castor_PHYS14_25_V1-v2/00000/622CAFBA-BD9A-E411-BE11-002481E14FFC.root",
    #"/store/mc/Phys14DR/DYToMuMu_M-50_Tune4C_13TeV-pythia8/MINIAODSIM/PU40bx25_tsg_castor_PHYS14_25_V1-v2/00000/FA4B46B9-8E9A-E411-A899-002590A3C954.root",
    #"/store/mc/Phys14DR/DYToMuMu_M-50_Tune4C_13TeV-pythia8/MINIAODSIM/PU40bx25_tsg_castor_PHYS14_25_V1-v2/10000/8607F88E-F799-E411-A180-0025B3E063F0.root",
    #"/store/mc/Phys14DR/DYToMuMu_M-50_Tune4C_13TeV-pythia8/MINIAODSIM/PU40bx25_tsg_castor_PHYS14_25_V1-v2/10000/F620A7C9-F799-E411-8DEF-002590A371AC.root"
    #"/store/mc/Spring14miniaod/VBF_HToGG_M-125_13TeV-powheg-pythia6/MINIAODSIM/141029_PU40bx50_PLS170_V6AN2-v1/10000/5C3A5675-7C72-E411-AC85-003048D436EA.root"
    "/store/mc/Phys14DR/GluGluToHToGG_M-125_13TeV-powheg-pythia6/MINIAODSIM/PU20bx25_tsg_PHYS14_25_V1-v1/00000/3C2EFAB1-B16F-E411-AB34-7845C4FC39FB.root"),
    skipEvents=cms.untracked.uint32(0)
                       
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
process.load("flashgg/MicroAOD/flashggMicroAODExtraJetsSequence_cff")
process.load("flashgg/Validation/JetTreeMaker_cff")

from flashgg.MicroAOD.flashggMicroAODOutputCommands_cff import microAODDefaultOutputCommand,microAODDebugOutputCommand
process.out = cms.OutputModule("PoolOutputModule",
                               fileName       = cms.untracked.string('myMicroAODOutputFile.root'),
                               outputCommands = microAODDefaultOutputCommand
                           )

process.out.outputCommands += microAODDebugOutputCommand # extra items for debugging, CURRENTLY REQUIRED

# need to allow unscheduled processes otherwise reclustering function will fail
# this is because of the jet clustering tool, and we have to live with it for now.
# NOTE: new PAT default running is "unscheduled" so we just need to say in the outputCommands what we want to store
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


# ++++++++++++ JetTreeMaker +++++++++
process.TFileService = cms.Service("TFileService",
                                   fileName  = cms.string("./workspace/GluGluToHToGG_M-125_13TeV_JetValTrees_NEW_0.root"))

process.flashggJetValidationTreeMakerPF = cms.EDAnalyzer('FlashggJetValidationTreeMaker',
                                                         GenParticleTag        = cms.untracked.InputTag('prunedGenParticles'),
                                                         JetTagDz              = cms.InputTag("flashggJetsPF"),
                                                         StringTag	       = cms.string("PF"),
                                                         VertexCandidateMapTag = cms.InputTag("flashggVertexMapUnique"),
                                                         debug                 = cms.untracked.bool(jdebug),
                                                     )

process.flashggJetValidationTreeMakerPFCHS0 = cms.EDAnalyzer('FlashggJetValidationTreeMaker',
                                                             GenParticleTag     = cms.untracked.InputTag('prunedGenParticles'),
                                                             JetTagDz           = cms.InputTag("flashggJetsPFCHS0"),
                                                             StringTag		= cms.string("PFCHS0"),
                                                             VertexCandidateMapTag = cms.InputTag("flashggVertexMapUnique"),
                                                             debug                 = cms.untracked.bool(jdebug),
                                                         )

process.flashggJetValidationTreeMakerPFCHSLeg = cms.EDAnalyzer('FlashggJetValidationTreeMaker',
                                                               GenParticleTag = cms.untracked.InputTag('prunedGenParticles'),
                                                               JetTagDz       = cms.InputTag("flashggJets"),                  
                                                               StringTag	= cms.string("PFCHSLeg"),
                                                               VertexCandidateMapTag = cms.InputTag("flashggVertexMapUnique"),
                                                               debug                 = cms.untracked.bool(jdebug),
                                                           )

# +++++++++++++++++++++++++++++++++
process.p = cms.Path(  process.flashggMicroAODSequence
                       + process.flashggMicroAODExtraJetsSequence
                       # tree producer ...
                       + process.flashggJetValidationTreeMakerPF 
                       + process.flashggJetValidationTreeMakerPFCHS0
                       + process.flashggJetValidationTreeMakerPFCHSLeg
                   )

process.e = cms.EndPath(process.out)

# Uncomment these lines to run the example commissioning module and send its output to root
# process.commissioning = cms.EDAnalyzer('flashggCommissioning',
#                                         PhotonTag   = cms.untracked.InputTag('flashggPhotons'),
#                                         DiPhotonTag = cms.untracked.InputTag('flashggDiPhotons'),
#                                         VertexTag   = cms.untracked.InputTag('offlineSlimmedPrimaryVertices')
#)
#process.TFileService = cms.Service("TFileService",
#                                   fileName = cms.string("commissioningTree.root")
#)
#process.p *= process.commissioning

from flashgg.MicroAOD.MicroAODCustomize import customize
customize(process)
