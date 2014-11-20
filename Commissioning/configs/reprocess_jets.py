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

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(100) )
process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32( 1 )

process.source = cms.Source("PoolSource",
			#fileNames=cms.untracked.vstring("/store/cmst3/user/gpetrucc/miniAOD/v1/GluGluToHToGG_M-125_13TeV-powheg-pythia6_Flat20to50_PAT.root")
			#fileNames=cms.untracked.vstring("/store/mc/Spring14miniaod/VBF_HToGG_M-125_13TeV-powheg-pythia6/MINIAODSIM/PU20bx25_POSTLS170_V5-v1/00000/CE926731-9607-E411-B0BA-001E67248A1B.root")
			fileNames=cms.untracked.vstring("file:../../CE926731-9607-E411-B0BA-001E67248A1B.root")
			)

process.source.inputCommands = cms.untracked.vstring("keep *",
			"drop *_MEtoEDMConverter_*_*")


process.load("flashgg/MicroAODProducers/flashggVertexMaps_cfi")
process.load("flashgg/MicroAODProducers/flashggPhotons_cfi")
process.load("flashgg/MicroAODProducers/flashggDiPhotons_cfi")
process.load("flashgg/MicroAODProducers/flashggPreselectedDiPhotons_cfi")
			
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

process.ak4PFJetsCHS = cms.EDProducer(
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


from PhysicsTools.PatAlgos.tools.jetTools import addJetCollection
from PhysicsTools.PatAlgos.tools.jetTools import switchJetCollection

addJetCollection(
			process,
			labelName = 'AK4PFCHS',
			jetSource = cms.InputTag('ak4PFJetsCHS'),
			algo = 'ak4',
			rParam = 0.4,
			jetCorrections = ('AK4PFchs', cms.vstring(['L1FastJet', 'L2Relative', 'L3Absolute']), 'None'),
			trackSource = cms.InputTag('unpackedTracksAndVertices'),
			pvSource = cms.InputTag('unpackedTracksAndVertices'),
			btagDiscriminators = ['combinedSecondaryVertexBJetTags'],
			)

#adjust MC matching
process.patJetGenJetMatchAK4PFCHS.matched = "ak4GenJets"
process.patJetPartonMatchAK4PFCHS.matched = "prunedGenParticles"
process.patJetPartons.particles = "prunedGenParticles"

process.patJetCorrFactorsAK4PFCHS.primaryVertices = "offlineSlimmedPrimaryVertices"

# As tracks are not stored in miniAOD, and b-tag fwk for CMSSW < 72X does not accept candidates
# we need to recreate tracks and pv for btagging in standard reco format:
process.load('PhysicsTools.PatAlgos.slimming.unpackedTracksAndVertices_cfi')
process.combinedSecondaryVertex.trackMultiplicityMin = 1  #needed for CMSSW < 71X


process.options = cms.untracked.PSet(
        allowUnscheduled = cms.untracked.bool(True)
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


#process.options = cms.untracked.PSet(
#        allowUnscheduled = cms.untracked.bool(True)
#				)

#process.p = cms.Path(process.flashggVertexMapUnique*
#		process.flashggVertexMapNonUnique*
#		process.flashggPhotons*
#		process.flashggDiPhotons*
#		process.flashggPreselectedDiPhotons*
		#	process.puppi*
#		process.ak4PFJets
#		process.patJetCorrFactorsAK4PF*
#		process.patJetsAK4PF*
#		(process.flashggDiPhotonMVA+process.flashggJets)* # These two could run in parallel, so use +
#		process.flashggUntaggedCategory*
#		(process.flashggVBFMVA)* # Needs to happen after Jets
#		(process.flashggVBFDiPhoDiJetMVA)* # Needs to happen after VBF MVA and DiPho MVA
#		(process.flashggVBFTag)* # Tag producers, once written, can run in parallel, so they go in here with +
#		process.flashggTagSorter*
#		process.flashggTreeMakerWithTagSorter
		#process.commissioning*
	#	)
#process.endpath= cms.EndPath(process.OUT)
process.e = cms.EndPath(process.out)
