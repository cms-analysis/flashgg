import FWCore.ParameterSet.Config as cms
import FWCore.Utilities.FileUtils as FileUtils

process = cms.Process("FLASHggMicroAOD")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.load("Configuration.StandardSequences.GeometryDB_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff")
from Configuration.AlCa.GlobalTag import GlobalTag

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32( 1000) )
process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32( 1000 )

process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_mc')

# Fix because auto:run2_mc points to MCRUN2_74_V9::All
current_gt = process.GlobalTag.globaltag.value()
if current_gt.count("::All"):
    new_gt = current_gt.replace("::All","")
    print 'Removing "::All" from GlobalTag by hand for condDBv2: was %s, now %s' % (current_gt,new_gt)
    process.GlobalTag.globaltag = new_gt



process.source = cms.Source("PoolSource",
                            fileNames=cms.untracked.vstring(
                                "/store/mc/RunIISpring15DR74/GluGluHToGG_M-125_13TeV_powheg_pythia8/MINIAODSIM/Asympt50ns_MCRUN2_74_V9A-v1/30000/54ECB9A4-912E-E511-BB7D-002590A831CA.root"
                            )
)

process.MessageLogger.cerr.threshold = 'ERROR' 
# process.MessageLogger.suppressWarning.extend(['SimpleMemoryCheck','MemoryCheck'])

# This is a lightweight tool to digg further
#process.SimpleMemoryCheck = cms.Service("SimpleMemoryCheck",
#                                        ignoreTotal = cms.untracked.int32(1),
#                                        monitorPssAndPrivate = cms.untracked.bool(True)
#                                       )

process.load("flashgg/MicroAOD/flashggMicroAODSequence_cff")

from flashgg.MicroAOD.flashggMicroAODOutputCommands_cff import microAODDefaultOutputCommand
process.out = cms.OutputModule("PoolOutputModule", fileName = cms.untracked.string('myMicroAODOutputFile.root'),
                               outputCommands = microAODDefaultOutputCommand
                               )


process.TFileService = cms.Service("TFileService",fileName  = cms.string("JetValidationTest.root"))

# need to allow unscheduled processes otherwise reclustering function will fail
# this is because of the jet clustering tool, and we have to live with it for now.
process.options = cms.untracked.PSet(
    allowUnscheduled = cms.untracked.bool(True)
    )
# import function which takes care of reclustering the jets using legacy vertex		
from flashgg.MicroAOD.flashggJets_cfi import addFlashggPFCHSJets 
from flashgg.MicroAOD.flashggJets_cfi import addFlashggPuppiJets
from flashgg.MicroAOD.flashggJets_cfi import maxJetCollections
from flashgg.MicroAOD.flashggJets_cfi import PuppiJetCollectionVInputTag, JetCollectionVInputTag
# call the function, it takes care of everything else.


for vtx in range(0,maxJetCollections):
    addFlashggPFCHSJets (process = process,
                         vertexIndex =vtx,
                         doQGTagging = False,
                         label = '' + str(vtx))    
    addFlashggPuppiJets (process     = process,
                         vertexIndex = vtx,
                         debug       = False,
                         label = '' + str(vtx))
# run a standard puppi with the default seeting

#from flashgg.MicroAOD.flashggExtraJets_cfi import addStandardPuppiJets 
#addStandardPuppiJets(process, label = 'Test')
#stdPuppiVtag = cms.VInputTag(cms.InputTag('flashggSTDPUPPIJetsTest'))

# 
from flashgg.Taggers.flashggTags_cff import UnpackedJetCollectionVInputTag   

process.flashggJetTreeMakerPFCHS = cms.EDAnalyzer('FlashggJetValidationTreeMaker',
                                                  GenParticleTag = cms.untracked.InputTag('prunedGenParticles'),
                                                  DiPhotonTag    = cms.InputTag('flashggDiPhotons'),
                                                  inputTagJets   = JetCollectionVInputTag,
                                                  StringTag      = cms.string("PFCHS"))

process.flashggJetTreeMakerPUPPI = cms.EDAnalyzer('FlashggJetValidationTreeMaker',
                                                  GenParticleTag = cms.untracked.InputTag('prunedGenParticles'),
                                                  DiPhotonTag    = cms.InputTag('flashggDiPhotons'),
                                                  inputTagJets   = PuppiJetCollectionVInputTag,
                                                  StringTag      = cms.string("PUPPI"))

#process.flashggJetTreeMakerSTDPUPPI = cms.EDAnalyzer('FlashggJetValidationTreeMaker',
#                                                     GenParticleTag = cms.untracked.InputTag('prunedGenParticles'),
#                                                     DiPhotonTag    = cms.InputTag('flashggDiPhotons'),
#                                                     inputTagJets   = stdPuppiVtag,
#                                                     StringTag      = cms.string("PUPPI"))

process.p = cms.Path(process.flashggMicroAODSequence +
                     process.flashggJetTreeMakerPFCHS +
                     process.flashggJetTreeMakerPUPPI  )
process.e = cms.EndPath(process.out)



from flashgg.MicroAOD.MicroAODCustomize import customize
customize(process)

