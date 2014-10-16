import FWCore.ParameterSet.Config as cms
import FWCore.Utilities.FileUtils as FileUtils

process = cms.Process("FLASHggMicroAOD")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.load("Configuration.StandardSequences.GeometryDB_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.GlobalTag.globaltag = 'POSTLS170_V5::All'

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32( 20 ) )
process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32( 1000 )

process.source = cms.Source("PoolSource",fileNames=cms.untracked.vstring("/store/cmst3/user/gpetrucc/miniAOD/v1/GluGluToHToGG_M-125_13TeV-powheg-pythia6_Flat20to50_PAT.root"))

process.flashggVertexMapUnique = cms.EDProducer('FlashggDzVertexMapProducer',
                                                PFCandidatesTag=cms.untracked.InputTag('packedPFCandidates'),
                                                VertexTag=cms.untracked.InputTag('offlineSlimmedPrimaryVertices'),
                                                BeamSpotTag=cms.untracked.InputTag('offlineBeamSpot'),
                                                MaxAllowedDz=cms.double(0.2) # in cm
                                                )

# Tracks will show up as associated to every vertex for which dZ < MaxAllowedDz
process.flashggVertexMapNonUnique = cms.EDProducer('FlashggDzVertexMapProducer',
                                                   PFCandidatesTag=cms.untracked.InputTag('packedPFCandidates'),
                                                   VertexTag=cms.untracked.InputTag('offlineSlimmedPrimaryVertices'),
                                                   MaxAllowedDz=cms.double(0.2), # in cm
                                                   UseEachTrackOnce=cms.untracked.bool(False)
                                                   )

process.load("flashgg/MicroAODProducers/flashggPhotons_cfi")
process.load("flashgg/MicroAODProducers/flashggDiPhotons_cfi")
process.load("flashgg/MicroAODProducers/flashggPreselectedDiPhotons_cfi")


process.TFileService = cms.Service("TFileService",fileName = cms.string("flashggTree.root"))
process.flashggTreeMaker = cms.EDAnalyzer('FlashggFlashggTreeMaker',
                                                          VertexTag=cms.untracked.InputTag('offlineSlimmedPrimaryVertices'),
                                                          GenParticleTag=cms.untracked.InputTag('prunedGenParticles'),
                                                          VertexCandidateMapTagDz=cms.InputTag('flashggVertexMapUnique'),
                                                          VertexCandidateMapTagAOD = cms.InputTag('flashggVertexMapValidator'),
                                                          JetTagDz = cms.InputTag("flashggJets"),
																													DiPhotonTag = cms.untracked.InputTag('flashggDiPhotons'),
																													METTag = cms.untracked.InputTag('slimmedMETs'),
																													PileUpTag = cms.untracked.InputTag('addPileupInfo'),
																													rhoFixedGridCollection = cms.InputTag('fixedGridRhoAll'),
                                     									     diphotonMVAweightfile = cms.FileInPath("flashgg/MicroAODProducers/data/HggBambu_SMDipho_Oct29_rwgtptallsigevenbkg7TeV_BDTG.weights.xml"),
                                     									     vbfMVAweightfile = cms.FileInPath("flashgg/MicroAODProducers/data/TMVA_dijet_sherpa_scalewt50_2evenb_powheg200_maxdPhi_oct9_Gradient.weights.xml"),
                                     									     vbfDiphotonMVAweightfile = cms.FileInPath("flashgg/MicroAODProducers/data/TMVA_vbf_dijet_dipho_evenbkg_scaledwt50_maxdPhi_Gradient.weights.xml"),
#                                                          JetTagReco = cms.InputTag("flashggJetsUsingRecoJets")
                                                          )
                                                 
# This requires you to have done: git cms-merge-topic -u sethzenz:pileupjetid-for-flashgg
process.load("RecoJets.JetProducers.PileupJetIDParams_cfi")

process.flashggJets = cms.EDProducer('FlashggJetProducer',
                                     DiPhotonTag=cms.untracked.InputTag('flashggDiPhotons'),
                                     VertexTag=cms.untracked.InputTag('offlineSlimmedPrimaryVertices'),
                                     JetTag=cms.untracked.InputTag('slimmedJets'),
                                     VertexCandidateMapTag = cms.InputTag("flashggVertexMapUnique"),
                                     PileupJetIdParameters=cms.PSet(process.full_53x) # from PileupJetIDParams_cfi
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
                                       DiPhotonTag = cms.untracked.InputTag('flashggDiPhotons'),
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
                     process.flashggJets*
                     process.commissioning*
										 process.flashggTreeMaker
                    )

process.e = cms.EndPath(process.out)
