import FWCore.ParameterSet.Config as cms

process = cms.Process("FLASHggTEST")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(10) )

process.source = cms.Source("PoolSource",fileNames=cms.untracked.vstring("file:/afs/cern.ch/work/g/gpetrucc/micro/70x/CMSSW_7_0_4/src/miniProd/WH_ZH_HToGG_M-125_13TeV_pythia6_PAT.root"))

process.flashggPhotons = cms.EDProducer('FlashggPhotonProducer',
                                        PhotonTag=cms.untracked.InputTag('slimmedPhotons')
                                        )
process.flashggDiPhotons = cms.EDProducer('FlashggDiPhotonProducer',
                                          PhotonTag=cms.untracked.InputTag('flashggPhotons'),
                                          VertexTag=cms.untracked.InputTag('offlineSlimmedPrimaryVertices'),
#                                         VertexSelectorName=cms.string("FlashggZerothVertexSelector"),
                                          VertexSelectorName=cms.string("FlashggLegacyVertexSelector")
                                          )

process.out = cms.OutputModule("PoolOutputModule", fileName = cms.untracked.string('myOutputFile.root'),
                               outputCommands = cms.untracked.vstring("drop *","keep *_flashgg*_*_*","keep *_offlineSlimmedPrimaryVertices_*_*")
)

process.p = cms.Path(process.flashggPhotons*process.flashggDiPhotons)

process.e = cms.EndPath(process.out)
