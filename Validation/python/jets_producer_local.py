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
process.source = cms.Source("PoolSource",fileNames=cms.untracked.vstring(
    "/store/mc/RunIISpring15DR74/DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/MINIAODSIM/Asympt50ns_MCRUN2_74_V9A-v2/00000/00C4781D-6B08-E511-8A0A-0025905A6084.root"),
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

from flashgg.MicroAOD.flashggMicroAODOutputCommands_cff import microAODDefaultOutputCommand,microAODDebugOutputCommand
process.out = cms.OutputModule("PoolOutputModule",
                               fileName       = cms.untracked.string('./workspace/myMicroAODOutputFile.root'),
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
# uses the vertex leg
from flashgg.MicroAOD.flashggExtraJets_cfi import addFlashggPF , addQGTaggerPF
# call the function, it takes care of everything else.
addFlashggPF(process)
# add QG tagger
addQGTaggerPF(process)
# +++++++++++++++++++++++++++++++++++

# ++++++++++++ PFCHS0 +++++++++++++++
# uses the vertex 0
from flashgg.MicroAOD.flashggExtraJets_cfi import addFlashggPFCHS0 , addQGTaggerPFCHS0
# call the function, it takes care of everything else.
addFlashggPFCHS0(process)
# add QG tagger
addQGTaggerPFCHS0(process)
# +++++++++++++++++++++++++++++++++++

# ++++++++++++ PFCHSLeg +++++++++++++
# import function which takes care of reclustering the jets using legacy vertex     
from flashgg.MicroAOD.flashggJets_cfi import addFlashggPFCHSLegJets , addQGTaggerPFCHSLeg
# call the function, it takes care of everything else.
addFlashggPFCHSLegJets(process)
# add QG tagger
addQGTaggerPFCHSLeg(process)
# +++++++++++++++++++++++++++++++++++

# ++++++++++++ QG Tagger Test ++++++++++++
#process.load('RecoJets.JetProducers.QGTagger_cfi')
#
## Could be reco::PFJetCollection or pat::JetCollection (both AOD and miniAOD)
#process.QGTagger.srcJets     = cms.InputTag('flashggJetsPFCHS0')
#process.QGTagger.jetsLabel   = cms.string('QGL_AK4PFchs') 
### for all the options :  https://twiki.cern.ch/twiki/bin/viewauth/CMS/QGDataBaseVersion
#
#process.QGTaggerPF       = process.QGTagger.clone( srcJets = 'flashggJetsPF'      ,jetsLabel = 'QGL_AK4PF' );
#process.QGTaggerPFCHS0   = process.QGTagger.clone( srcJets = 'flashggJetsPFCHS0'  ,jetsLabel = 'ak4PFJetsCHS' );
#process.QGTaggerPFCHSLeg = process.QGTagger.clone( srcJets = 'flashggJetsPFCHSLeg',jetsLabel = 'ak4PFJetsCHS' );
## +++++++++++++++++++++++++++++++++++

## ++++++++++++ JetTreeMaker +++++++++
process.TFileService = cms.Service("TFileService",
                                   fileName  = cms.string("./workspace/GluGluToHToGG_M-125_13TeV_JetValTrees_NEW_0.root"))

process.flashggJetValidationTreeMakerPF = cms.EDAnalyzer('FlashggJetValidationTreeMaker',
                                                         GenParticleTag        = cms.untracked.InputTag('prunedGenParticles'),
                                                         JetTagDz              = cms.InputTag("flashggJetsPF"),
                                                         StringTag	       = cms.string("PF"),
                                                         VertexCandidateMapTag = cms.InputTag("flashggVertexMapUnique"),
                                                         qgVariablesInputTag   = cms.InputTag('QGTaggerPF'),
                                                         DiPhotonTag           = cms.InputTag("flashggDiPhotons"),
                                                         debug                 = cms.untracked.bool(jdebug),
                                                     )

process.flashggJetValidationTreeMakerPFCHS0 = cms.EDAnalyzer('FlashggJetValidationTreeMaker',
                                                             GenParticleTag        = cms.untracked.InputTag('prunedGenParticles'),
                                                             JetTagDz              = cms.InputTag("flashggJetsPFCHS0"),
                                                             StringTag		   = cms.string("PFCHS0"),
                                                             VertexCandidateMapTag = cms.InputTag("flashggVertexMapUnique"),
                                                             qgVariablesInputTag   = cms.InputTag('QGTaggerPFCHS0'),
                                                             DiPhotonTag           = cms.InputTag("flashggDiPhotons"),
                                                             debug                 = cms.untracked.bool(jdebug),
                                                         )
# This producer is the default in flashgg Dijet MVA 
process.flashggJetValidationTreeMakerPFCHSLeg = cms.EDAnalyzer('FlashggJetValidationTreeMaker',
                                                               GenParticleTag        = cms.untracked.InputTag('prunedGenParticles'),
                                                               JetTagDz              = cms.InputTag("flashggJets"),                  
                                                               StringTag	     = cms.string("PFCHSLeg"),
                                                               VertexCandidateMapTag = cms.InputTag("flashggVertexMapUnique"),
                                                               qgVariablesInputTag   = cms.InputTag('QGTaggerPFCHSLeg'),
                                                               DiPhotonTag           = cms.InputTag("flashggDiPhotons"),
                                                               debug                 = cms.untracked.bool(jdebug),
                                                           )

# +++++++++++++++++++++++++++++++++
process.p = cms.Path(  process.flashggMicroAODSequence
                       + process.flashggMicroAODExtraJetsSequence
                       # tree producer ...
                       + process.flashggJetValidationTreeMakerPF 
                       #+ process.flashggJetValidationTreeMakerPFCHS0
                       #+ process.flashggJetValidationTreeMakerPFCHSLeg
                   )

process.e = cms.EndPath(process.out)

# Uncomment these lines to run the example commissioning module and send its output to root
# process.commissioning = cms.EDAnalyzer('flashggCommissioning',
#                                         PhotonTag   = cms.untracked.InputTag('flashggPhotons'),
#                                         DiPhotonTag = cms.InputTag('flashggDiPhotons'),
#                                         VertexTag   = cms.untracked.InputTag('offlineSlimmedPrimaryVertices')
#)
#process.TFileService = cms.Service("TFileService",
#                                   fileName = cms.string("commissioningTree.root")
#)
#process.p *= process.commissioning

from flashgg.MicroAOD.MicroAODCustomize import customize
customize(process)
