import FWCore.ParameterSet.Config as cms
import FWCore.Utilities.FileUtils as FileUtils

process = cms.Process("FLASHggMicroAOD")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.load("PhysicsTools.PatAlgos.producersLayer1.patCandidates_cff")
process.load("Configuration.EventContent.EventContent_cff")

process.load("Configuration.StandardSequences.GeometryDB_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.GlobalTag.globaltag = 'POSTLS170_V5::All'

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(10000) )
process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32( 10000 )

process.source = cms.Source("PoolSource",
	#fileNames=cms.untracked.vstring("/store/cmst3/user/gpetrucc/miniAOD/v1/GluGluToHToGG_M-125_13TeV-powheg-pythia6_Flat20to50_PAT.root")
	fileNames=cms.untracked.vstring("/store/mc/Spring14miniaod/VBF_HToGG_M-125_13TeV-powheg-pythia6/MINIAODSIM/PU20bx25_POSTLS170_V5-v1/00000/CE926731-9607-E411-B0BA-001E67248A1B.root")
	)

process.source.inputCommands = cms.untracked.vstring("keep *",
			"drop *_MEtoEDMConverter_*_*")
from RecoJets.JetProducers.AnomalousCellParameters_cfi import *


PFJetParameters = cms.PSet(
					srcPVs = cms.InputTag(''),
					jetType = cms.string('PFJet'),
					doOutputJets = cms.bool(True),
					jetPtMin = cms.double(3.0),
					inputEMin = cms.double(0.0),
					inputEtMin = cms.double(0.0),
					doPVCorrection = cms.bool(False),
# pileup with offset correction
					doPUOffsetCorr = cms.bool(False),
# if pileup is false, these are not read:
					nSigmaPU = cms.double(1.0),
					radiusPU = cms.double(0.5),
# fastjet-style pileup
					doAreaFastjet = cms.bool( False),
					doRhoFastjet = cms.bool( False),
					doAreaDiskApprox = cms.bool( False),
					Active_Area_Repeats = cms.int32( 1),
					GhostArea = cms.double(0.01),
					Ghost_EtaMax = cms.double( 5.0),
					Rho_EtaMax = cms.double( 4.4),
					voronoiRfact = cms.double(-0.9),
					useDeterministicSeed= cms.bool( True ),
					minSeed = cms.uint32( 14327 )
	)

process.ak4PFJets = cms.EDProducer(
			"FastjetJetProducer",
			PFJetParameters,
			AnomalousCellParameters,
			UseOnlyOnePV = cms.bool(True),
			UseFlashggVertex = cms.bool(True),
		  DiPhotonTag = cms.untracked.InputTag('flashggDiPhotons'),
			src = cms.InputTag('packedPFCandidates'),
			jetAlgorithm = cms.string("AntiKt"),
			rParam = cms.double(0.4)
		)

process.load('PhysicsTools.PatAlgos.slimming.unpackedTracksAndVertices_cfi')

from PhysicsTools.PatAlgos.tools.jetTools import addJetCollection
addJetCollection(
		process,
		postfix   = "",
		labelName = 'AK4PF',
		jetSource = cms.InputTag('ak4PFJets'),
		trackSource = cms.InputTag('unpackedTracksAndVertices'), 
		pvSource = cms.InputTag('unpackedTracksAndVertices'), 
		jetCorrections = ('AK4PF', cms.vstring(['L1FastJet', 'L2Relative', 'L3Absolute']), 'Type-2'),
		btagDiscriminators = [      'combinedSecondaryVertexBJetTags'     ]
		,algo= 'AK', rParam = 0.4
		)

process.patJetGenJetMatchAK4PF.matched = "ak4GenJets"
process.patJetPartonMatchAK4PF.matched = "prunedGenParticles"
process.patJetPartons.particles = "prunedGenParticles"
process.patJetCorrFactorsAK4PF.primaryVertices = "offlineSlimmedPrimaryVertices"


process.load("flashgg/MicroAODProducers/flashggVertexMaps_cfi")
process.load("flashgg/MicroAODProducers/flashggPhotons_cfi")
process.load("flashgg/MicroAODProducers/flashggDiPhotons_cfi")
process.load("flashgg/MicroAODProducers/flashggPreselectedDiPhotons_cfi")
#process.load("flashgg/MicroAODProducers/flashggJets_cfi")
from RecoJets.JetProducers.PileupJetIDParams_cfi import full_53x

process.flashggJets = cms.EDProducer('FlashggJetProducer',
		DiPhotonTag=cms.untracked.InputTag('flashggDiPhotons'),
		VertexTag=cms.untracked.InputTag('offlineSlimmedPrimaryVertices'),
		JetTag=cms.untracked.InputTag('patJetsAK4PF'),
		VertexCandidateMapTag = cms.InputTag("flashggVertexMapUnique"),
		PileupJetIdParameters=cms.PSet(full_53x) # from PileupJetIDParams_cfi
		)

#Tag stuff
process.load("flashgg/TagProducers/flashggDiPhotonMVA_cfi")
process.load("flashgg/TagProducers/flashggVBFMVA_cfi")
process.load("flashgg/TagProducers/flashggVBFDiPhoDiJetMVA_cfi")
process.load("flashgg/TagProducers/flashggTags_cfi")

process.flashggTagSorter = cms.EDProducer('FlashggTagSorter',
		DiPhotonTag = cms.untracked.InputTag('flashggDiPhotons'),
		TagVectorTag = cms.untracked.VInputTag(
			cms.untracked.InputTag('flashggVBFTag'),
			cms.untracked.InputTag('flashggUntaggedCategory'),
			),
		massCutUpper=cms.untracked.double(180),
		massCutLower=cms.untracked.double(100)

		)


process.TFileService = cms.Service("TFileService",fileName = cms.string("flashggTreeWithTags.root"))
#process.flashggTreeMakerWithTags = cms.EDAnalyzer('FlashggFlashggTreeMakerWithTags',
#                                                          VertexTag=cms.untracked.InputTag('offlineSlimmedPrimaryVertices'),
#                                                          GenParticleTag=cms.untracked.InputTag('prunedGenParticles'),
#                                                          VertexCandidateMapTagDz=cms.InputTag('flashggVertexMapUnique'),
#                                                          VertexCandidateMapTagAOD = cms.InputTag('flashggVertexMapValidator'),
#                                                          JetTagDz = cms.InputTag("flashggJets"),
#																													DiPhotonTag = cms.untracked.InputTag('flashggDiPhotons'),
#																													METTag = cms.untracked.InputTag('slimmedMETs'),
#																													PileUpTag = cms.untracked.InputTag('addPileupInfo'),
#																													UntaggedTag = cms.untracked.InputTag('flashggUntaggedCategory'),
#																													VBFTag = cms.untracked.InputTag('flashggVBFTag'),
#																													rhoFixedGridCollection = cms.InputTag('fixedGridRhoAll'),
#                                                          )
process.flashggTreeMakerWithTagSorter = cms.EDAnalyzer('FlashggFlashggTreeMakerWithTagSorter',
		VertexTag=cms.untracked.InputTag('offlineSlimmedPrimaryVertices'),
		GenParticleTag=cms.untracked.InputTag('prunedGenParticles'),
		VertexCandidateMapTagDz=cms.InputTag('flashggVertexMapUnique'),
		VertexCandidateMapTagAOD = cms.InputTag('flashggVertexMapValidator'),
		JetTagDz = cms.InputTag("flashggJets"),
		DiPhotonTag = cms.untracked.InputTag('flashggDiPhotons'),
		METTag = cms.untracked.InputTag('slimmedMETs'),
		PileUpTag = cms.untracked.InputTag('addPileupInfo'),
		TagSorter = cms.untracked.InputTag('flashggTagSorter'),
		rhoFixedGridCollection = cms.InputTag('fixedGridRhoAll'),
		)



process.out = cms.OutputModule("PoolOutputModule", fileName = cms.untracked.string('myOutputFile.root'),
		#   outputCommands = cms.untracked.vstring("drop *",
			#                                         "keep *_flashgg*_*_*",
			#                                        "drop *_flashggVertexMap*_*_*",
			#                                       "keep *_offlineSlimmedPrimaryVertices_*_*",
			#                                      "keep *_reducedEgamma_reduced*Clusters_*",
			#                                     "keep *_reducedEgamma_*PhotonCores_*",
			#                                    "keep *_slimmedElectrons_*_*",
			#                                   "keep *_slimmedMuons_*_*",
			#                                  "keep *_slimmedMETs_*_*",
			#                                 "keep *_slimmedTaus_*_*",
			#                                "keep *_fixedGridRhoAll_*_*"
			#                              )
		outputCommands = cms.untracked.vstring("keep *",
			)
		)

process.commissioning = cms.EDAnalyzer('flashggCommissioning',
		PhotonTag=cms.untracked.InputTag('flashggPhotons'),
		DiPhotonTag = cms.untracked.InputTag('flashggDiPhotons'),
		VertexTag=cms.untracked.InputTag('offlineSlimmedPrimaryVertices')
		)

process.TFileService = cms.Service("TFileService",
		fileName = cms.string("regular_tree.root")
		)

#process.options = cms.untracked.PSet(
#        allowUnscheduled = cms.untracked.bool(True)
#				)

process.p = cms.Path(process.flashggVertexMapUnique*
		process.flashggVertexMapNonUnique*
		process.flashggPhotons*
		process.flashggDiPhotons*
		process.flashggPreselectedDiPhotons*
		#	process.puppi*
		process.ak4PFJets*
		process.patJetCorrFactorsAK4PF*
		process.patJetsAK4PF*
		(process.flashggDiPhotonMVA+process.flashggJets)* # These two could run in parallel, so use +
		process.flashggUntaggedCategory*
		(process.flashggVBFMVA)* # Needs to happen after Jets
		(process.flashggVBFDiPhoDiJetMVA)* # Needs to happen after VBF MVA and DiPho MVA
		(process.flashggVBFTag)* # Tag producers, once written, can run in parallel, so they go in here with +
		process.flashggTagSorter*
		process.flashggTreeMakerWithTagSorter
		#process.commissioning*
		)

process.e = cms.EndPath(process.out)
