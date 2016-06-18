# ================================================
#   MicroAOD and Jet Tree Producer
#   Y. Haddad 01/2015
# ================================================

import FWCore.ParameterSet.Config as cms
import FWCore.Utilities.FileUtils as FileUtils
from flashgg.MicroAOD.flashggJets_cfi import flashggBTag, maxJetCollections
process = cms.Process("FLASHggJetValidation")
process.load("FWCore.MessageService.MessageLogger_cfi")

# +++++ the number of processed events
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32( 50000 ) )
process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32( 100 )

# +++++ the source file
process.source = cms.Source("PoolSource",
                            fileNames=cms.untracked.vstring("/store/group/phys_higgs/cmshgg/sethzenz/flashgg/RunIISpring15-JetUpdate-BetaV7J1/Spring15BetaV7J1/VBFHToGG_M-125_13TeV_powheg_pythia8/RunIISpring15-JetUpdate-BetaV7J1-Spring15BetaV7J1-v0-RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/151127_155759/0000/myMicroAODOutputFile_1.root")) 

process.MessageLogger.cerr.threshold = 'ERROR'

process.TFileService = cms.Service("TFileService",fileName  = cms.string("jetValidationTrees_VBF_HToGG_test.root"))

# process.flashggJetValidationTreeMaker = cms.EDAnalyzer('FlashggJetValidationTreeMaker',
#                                                        GenParticleTag           = cms.untracked.InputTag('prunedGenParticles'),
#                                                        JetTagDz                 = cms.InputTag("flashggJets"),
#                                                        StringTag		= cms.string("PF"),
#                                                    )
#

#JetCollectionVInputTagPFCHS = cms.VInputTag()
#JetCollectionVInputTagPUPPI = cms.VInputTag()
#for i in range(0,5):
#    JetCollectionVInputTagPFCHS.append(cms.InputTag('flashggPFCHSJets' + str(i)))
    #JetCollectionVInputTagPUPPI.append(cms.InputTag('flashggPUPPIJets' + str(i)))

process.flashggUnpackedJets = cms.EDProducer("FlashggVectorVectorJetUnpacker",
                                             JetsTag = cms.InputTag("flashggFinalJets"),
                                             NCollections = cms.uint32(maxJetCollections)
                                     )

UnpackedJetCollectionVInputTag = cms.VInputTag()
for i in range(0,maxJetCollections):
    UnpackedJetCollectionVInputTag.append(cms.InputTag('flashggUnpackedJets',str(i)))
    
process.flashggJetValidationTreeMakerPFCHS = cms.EDAnalyzer('FlashggJetValidationTreeMaker',
                                                            GenParticleTag  = cms.untracked.InputTag('flashggPrunedGenParticles'),
                                                            DiPhotonTag     = cms.InputTag('flashggDiPhotons'),
                                                            inputTagJets    = UnpackedJetCollectionVInputTag,
                                                            StringTag	    = cms.string("PFCHS"),
                                                            )
#
#process.flashggJetValidationTreeMakerPUPPI = cms.EDAnalyzer('FlashggJetValidationTreeMaker',
#                                                            GenParticleTag = cms.untracked.InputTag('prunedGenParticles'),
#                                                            DiPhotonTag     = cms.InputTag('flashggDiPhotons'),
#                                                            inputTagJets   = JetCollectionVInputTagPUPPI,
#                                                            StringTag      = cms.string("PUPPI"),
#                                                        )

process.out = cms.OutputModule("PoolOutputModule", fileName = cms.untracked.string('testetstestst.root'),
                               outputCommands = cms.untracked.vstring())

process.options = cms.untracked.PSet( allowUnscheduled = cms.untracked.bool(True) )

process.p = cms.Path( 
    process.flashggUnpackedJets +  
    process.flashggJetValidationTreeMakerPFCHS 
    )
process.e = cms.EndPath(process.out)
