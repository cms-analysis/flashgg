import FWCore.ParameterSet.Config as cms
import FWCore.Utilities.FileUtils as FileUtils

process = cms.Process("FLASHggMicroAOD")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.load("Configuration.StandardSequences.GeometryDB_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.GlobalTag.globaltag = 'POSTLS170_V5::All'

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32( -1 ) )
process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32( 1000 )

process.source = cms.Source("PoolSource",fileNames=cms.untracked.vstring("/store/cmst3/user/gpetrucc/miniAOD/v1/GluGluToHToGG_M-125_13TeV-powheg-pythia6_Flat20to50_PAT.root"))


process.load("flashgg/MicroAOD/flashggTkVtxMap_cfi")
process.load("flashgg/MicroAOD/flashggPhotons_cfi")
process.load("flashgg/MicroAOD/flashggDiPhotons_cfi")
process.load("flashgg/MicroAOD/flashggPreselectedDiPhotons_cfi")
process.load("flashgg/MicroAOD/flashggJets_cfi")

#Tag stuff
process.load("flashgg/Taggers/flashggDiPhotonMVA_cfi")
process.load("flashgg/Taggers/flashggVBFMVA_cfi")
process.load("flashgg/Taggers/flashggVBFDiPhoDiJetMVA_cfi")
process.load("flashgg/Taggers/flashggTags_cfi")

process.TFileService = cms.Service("TFileService",fileName = cms.string("flashggTreeWithTags.root"))
process.flashggTreeMakerWithTags = cms.EDAnalyzer('FlashggFlashggTreeMakerWithTags',
                                                          VertexTag=cms.untracked.InputTag('offlineSlimmedPrimaryVertices'),
                                                          GenParticleTag=cms.untracked.InputTag('prunedGenParticles'),
                                                          VertexCandidateMapTagDz=cms.InputTag('flashggVertexMapUnique'),
                                                          VertexCandidateMapTagAOD = cms.InputTag('flashggVertexMapValidator'),
                                                          JetTagDz = cms.InputTag("flashggJets"),
																													DiPhotonTag = cms.InputTag('flashggDiPhotons'),
																													METTag = cms.untracked.InputTag('slimmedMETs'),
																													PileUpTag = cms.untracked.InputTag('addPileupInfo'),
																													UntaggedTag = cms.untracked.InputTag('flashggUntagged'),
																													VBFTag = cms.untracked.InputTag('flashggVBFTag'),
																													rhoFixedGridCollection = cms.InputTag('fixedGridRhoAll'),
                                                          )
                                                 


process.out = cms.OutputModule("PoolOutputModule", fileName = cms.untracked.string('myOutputFile.root'),
                               outputCommands = cms.untracked.vstring("drop *",
                                                                      "keep *_flashgg*_*_*",
                                                                      "drop *_flashggVertexMap*_*_*",
                                                                      "keep *_offlineSlimmedPrimaryVertices_*_*",
                                                                      "keep *_reducedEgamma_reduced*Clusters_*",
                                                                      "keep *_reducedEgamma_*PhotonCores_*",
                                                                      "keep *_slimmedElectrons_*_*",
                                                                      "keep *_slimmedMuons_*_*",
                                                                      "keep *_slimmedMETs_*_*",
                                                                      "keep *_slimmedTaus_*_*",
                                                                      "keep *_fixedGridRhoAll_*_*"
                                                                     )
                               )

process.commissioning = cms.EDAnalyzer('flashggCommissioning',
                                       PhotonTag=cms.untracked.InputTag('flashggPhotons'),
                                       DiPhotonTag = cms.InputTag('flashggDiPhotons'),
                                       VertexTag=cms.untracked.InputTag('offlineSlimmedPrimaryVertices')
)

process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string("tree.root")
)

process.p = cms.Path(process.flashggVertexMapUnique*
                     process.flashggVertexMapNonUnique*
                     process.flashggPhotons*
                     process.flashggDiPhotons*
                     process.flashggPreselectedDiPhotons*
                     (process.flashggDiPhotonMVA+process.flashggJets)* # These two could run in parallel, so use +
                     process.flashggUntagged*
										 (process.flashggVBFMVA)* # Needs to happen after Jets
                     (process.flashggVBFDiPhoDiJetMVA)* # Needs to happen after VBF MVA and DiPho MVA
                     (process.flashggVBFTag)* # Tag producers, once written, can run in parallel, so they go in here with +
                     #process.commissioning*
										 process.flashggTreeMakerWithTags
                    )

process.e = cms.EndPath(process.out)
