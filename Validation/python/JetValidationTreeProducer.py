# ================================================
#   MicroAOD and Jet Tree Producer
#   Y. Haddad 01/2015
# ================================================

import FWCore.ParameterSet.Config as cms
import FWCore.Utilities.FileUtils as FileUtils

process = cms.Process("FLASHggJetValidation")
process.load("FWCore.MessageService.MessageLogger_cfi")

# +++++ the number of processed events
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32( 1000 ) )
process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32( 100 )

# +++++ the source file
process.source = cms.Source("PoolSource",
                            fileNames=cms.untracked.vstring("file:/afs/cern.ch/work/y/yhaddad/myMicroAODOutputFile.root")) 

process.MessageLogger.cerr.threshold = 'ERROR'

process.TFileService = cms.Service("TFileService",fileName  = cms.string("jetValidationTrees_VBF_HToGG_test.root"))

# process.flashggJetValidationTreeMaker = cms.EDAnalyzer('FlashggJetValidationTreeMaker',
#                                                        GenParticleTag           = cms.untracked.InputTag('prunedGenParticles'),
#                                                        JetTagDz                 = cms.InputTag("flashggJets"),
#                                                        StringTag		= cms.string("PF"),
#                                                    )
#

JetCollectionVInputTagPFCHS = cms.VInputTag()
JetCollectionVInputTagPUPPI = cms.VInputTag()
for i in range(0,5):
    JetCollectionVInputTagPFCHS.append(cms.InputTag('flashggPFCHSJets' + str(i)))
    JetCollectionVInputTagPUPPI.append(cms.InputTag('flashggPUPPIJets' + str(i)))
    
process.flashggJetValidationTreeMakerPFCHS = cms.EDAnalyzer('FlashggJetValidationTreeMaker',
                                                            GenParticleTag  = cms.untracked.InputTag('prunedGenParticles'),
                                                            #JetTagDz        = cms.InputTag("flashggJetsPFCHS0"),
                                                            DiPhotonTag     = cms.InputTag('flashggDiPhotons'),
                                                            #VertexCandidateMapTag = cms.InputTag('')
                                                            inputTagJets    = JetCollectionVInputTagPFCHS,
                                                            StringTag	    = cms.string("PFCHS"),
                                                        )

process.flashggJetValidationTreeMakerPUPPI = cms.EDAnalyzer('FlashggJetValidationTreeMaker',
                                                            GenParticleTag = cms.untracked.InputTag('prunedGenParticles'),
                                                            DiPhotonTag     = cms.InputTag('flashggDiPhotons'),
                                                            inputTagJets   = JetCollectionVInputTagPUPPI,
                                                            StringTag      = cms.string("PUPPI"),
                                                        )

process.out = cms.OutputModule("PoolOutputModule", fileName = cms.untracked.string('testetstestst.root'),
                               outputCommands = cms.untracked.vstring())

process.options = cms.untracked.PSet(
    allowUnscheduled = cms.untracked.bool(True)
)

process.p = cms.Path(
    process.flashggJetValidationTreeMakerPFCHS +
    process.flashggJetValidationTreeMakerPUPPI 
)
process.e = cms.EndPath(process.out)
