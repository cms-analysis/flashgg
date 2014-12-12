import FWCore.ParameterSet.Config as cms

process = cms.Process("FLASHggMicroAOD")
process.load("FWCore.MessageService.MessageLogger_cfi")


# Input source
process.source = cms.Source("PoolSource",
			#fileNames=cms.untracked.vstring("file:/afs/cern.ch/work/l/lcorpe/private/FLASHgg/CMSSW_7_0_7_patch1/src/flashgg/CE926731-9607-E411-B0BA-001E67248A1B.root")
			fileNames=cms.untracked.vstring("/store/mc/Spring14miniaod/VBF_HToGG_M-125_13TeV-powheg-pythia6/MINIAODSIM/PU20bx25_POSTLS170_V5-v1/00000/CE926731-9607-E411-B0BA-001E67248A1B.root")
		)

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(10) )
process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32( 1 )

process.load("flashgg/MicroAODProducers/flashggVertexMaps_cfi")
process.load("flashgg/MicroAODProducers/flashggPhotons_cfi")
process.load("flashgg/MicroAODProducers/flashggDiPhotons_cfi")
process.load("flashgg/MicroAODProducers/flashggPreselectedDiPhotons_cfi")
process.load("flashgg/MicroAODProducers/flashggElectrons_cfi")

#---------> BEGING  JET REPROCESSING <-------------------
#select isolated  muons and electrons collections
#tune the requirements to whatever ID and isolation you prefer 
process.selectedMuons = cms.EDFilter("CandPtrSelector", src = cms.InputTag("slimmedMuons"), cut = cms.string('''abs(eta)<2.5 && pt>10. &&
				(pfIsolationR04().sumChargedHadronPt+
				 max(0.,pfIsolationR04().sumNeutralHadronEt+
					 pfIsolationR04().sumPhotonEt-
					 0.50*pfIsolationR04().sumPUPt))/pt < 0.20 && 
				(isPFMuon && (isGlobalMuon || isTrackerMuon) )'''))
process.selectedElectrons = cms.EDFilter("CandPtrSelector", src = cms.InputTag("slimmedElectrons"), cut = cms.string('''abs(eta)<2.5 && pt>20. &&
				gsfTrack.isAvailable() &&
				gsfTrack.hitPattern().numberOfLostHits(\'MISSING_INNER_HITS\') < 2 &&
				(pfIsolationVariables().sumChargedHadronPt+
				 max(0.,pfIsolationVariables().sumNeutralHadronEt+
					 pfIsolationVariables().sumPhotonEt-
					 0.5*pfIsolationVariables().sumPUPt))/pt < 0.15'''))
	### Do "projections"
	# first select the packedCandidates passing the loose "fromPV()" requirement (equivalent to CHS definition used for Jets in Run I)
process.pfCHS = cms.EDFilter("CandPtrSelector", src = cms.InputTag("packedPFCandidates"), cut = cms.string("fromPV"))
	# then remove the previously selected muons
process.pfNoMuonCHS =  cms.EDProducer("CandPtrProjector", src = cms.InputTag("pfCHS"), veto = cms.InputTag("selectedMuons"))
	# then remove the previously selected electrons
process.pfNoElectronsCHS = cms.EDProducer("CandPtrProjector", src = cms.InputTag("pfNoMuonCHS"), veto =  cms.InputTag("selectedElectrons"))
	#Import RECO jet producer for ak4 PF and GEN jet
from RecoJets.JetProducers.ak4PFJets_cfi import ak4PFJets
from RecoJets.JetProducers.ak4GenJets_cfi import ak4GenJets
process.ak4PFJetsCHS = ak4PFJets.clone(src = 'pfNoElectronsCHS', doAreaFastjet = True)
process.ak4GenJets = ak4GenJets.clone(src = 'packedGenParticles')
	# The following is make patJets, but EI is done with the above
process.load("PhysicsTools.PatAlgos.producersLayer1.patCandidates_cff")
process.load("Configuration.EventContent.EventContent_cff")
process.load('Configuration.StandardSequences.Geometry_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.GlobalTag.globaltag = 'PLS170_V7AN1::All'
from PhysicsTools.PatAlgos.tools.jetTools import addJetCollection
addJetCollection(
		process,
		postfix   = "",
		labelName = 'AK4PFCHS',
		jetSource = cms.InputTag('ak4PFJetsCHS'),
		trackSource = cms.InputTag('unpackedTracksAndVertices'), 
		pvSource = cms.InputTag('unpackedTracksAndVertices'), 
		jetCorrections = ('AK4PFchs', cms.vstring(['L1FastJet', 'L2Relative', 'L3Absolute']), 'None'),
		btagDiscriminators = [      'combinedSecondaryVertexBJetTags'     ]
		,algo= 'AK', rParam = 0.4
		)
#adjust MC matching
process.patJetGenJetMatchAK4PFCHS.matched = "ak4GenJets"
process.patJetPartonMatchAK4PFCHS.matched = "prunedGenParticles"
process.patJetPartons.particles = "prunedGenParticles"
#adjust PV used for Jet Corrections
process.patJetCorrFactorsAK4PFCHS.primaryVertices = "offlineSlimmedPrimaryVertices"
# the following part is needed if you want to run b-tagging on the freshly made jets
# CAVEAT: it is not 100% the same b-tagging as in RECO, but performance plots are almost identical
# As tracks are not stored in miniAOD, and b-tag fwk for CMSSW < 72X does not accept candidates
# we need to recreate tracks and pv for btagging in standard reco format:
process.load('PhysicsTools.PatAlgos.slimming.unpackedTracksAndVertices_cfi')
process.combinedSecondaryVertex.trackMultiplicityMin = 1  #needed for CMSSW < 71X
#---------> END  JET REPROCESSING <-------------------


from RecoJets.JetProducers.PileupJetIDParams_cfi import full_53x

process.flashggJets = cms.EDProducer('FlashggJetProducer',
		DiPhotonTag=cms.untracked.InputTag('flashggDiPhotons'),
  	VertexTag=cms.untracked.InputTag('offlineSlimmedPrimaryVertices'),
		JetTag=cms.untracked.InputTag('patJetsAK4PFCHS'),
		VertexCandidateMapTag = cms.InputTag("flashggVertexMapUnique"),
		PileupJetIdParameters=cms.PSet(full_53x) # from PileupJetIDParams_cfi
		)


##Tag stuff
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



#new PAT default running is "unscheduled" so we just need to say in the outputCommands what we want to store
process.options = cms.untracked.PSet(
			allowUnscheduled = cms.untracked.bool(True)
			)

process.p = cms.Path(
		process.flashggTreeMakerWithTagSorter
		#process.commissioning*
		)
process.OUT = cms.OutputModule("PoolOutputModule",
			fileName = cms.untracked.string('test.root'),
			outputCommands = cms.untracked.vstring(['drop *','keep patJets_patJetsAK4PFCHS_*_*','keep *_*_*_PAT','keep flashgg*_*_*_*'])
			)
process.endpath= cms.EndPath(process.OUT)
