import FWCore.ParameterSet.Config as cms

process = cms.Process("FLASHggTEST")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(1) )

process.source = cms.Source("PoolSource",fileNames=cms.untracked.vstring(
"file:/afs/cern.ch/work/s/sethzenz/public/Hgg_miniAOD_run0/miniAOD_0.root",
"file:/afs/cern.ch/work/s/sethzenz/public/Hgg_miniAOD_run0/miniAOD_1.root"
))

process.flashggPhotons = cms.EDProducer('FlashggPhotonProducer',
                                        PhotonTag=cms.untracked.InputTag('slimmedPhotons'),
                                        PhotonPreselectorName=cms.string("FlashggRun1LegacyPhotonPreselector")
                                        )
process.flashggDiPhotons = cms.EDProducer('FlashggDiPhotonProducer',
                                          PhotonTag=cms.untracked.InputTag('flashggPhotons'),
                                          VertexTag=cms.untracked.InputTag('offlineSlimmedPrimaryVertices'),
                                          VertexSelectorName=cms.string("FlashggZerothVertexSelector")
                                          )

process.out = cms.OutputModule("PoolOutputModule", fileName = cms.untracked.string('myOutputFile.root'),
                               outputCommands = cms.untracked.vstring("drop *","keep *_flashgg*_*_*","keep *_offlineSlimmedPrimaryVertices_*_*")
)

process.p = cms.Path(process.flashggPhotons*process.flashggDiPhotons)

process.e = cms.EndPath(process.out)
