import FWCore.ParameterSet.Config as cms
process = cms.Process("FLASHggMicroAOD")

process.load("PhysicsTools.PatAlgos.producersLayer1.patCandidates_cff")
process.load("Configuration.EventContent.EventContent_cff")
process.load('Configuration.StandardSequences.Geometry_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.GlobalTag.globaltag = 'PLS170_V7AN1::All'

process.load("FWCore.MessageService.MessageLogger_cfi")


# Input source
process.source = cms.Source("PoolSource",
                            #fileNames=cms.untracked.vstring("file:/afs/cern.ch/work/y/yhaddad/VBFHgg_Phys14/VBF_HToGG_M-125_13TeV-powheg-pythia6/MINIAODSIM/PU20bx25_PHYS14_25_V1-v1/32180B1A-166C-E411-ACC7-00266CFFA120.root"),
                            fileNames=cms.untracked.vstring("file:/afs/cern.ch/work/l/lcorpe/public/VBFHgg_Phys14/32180B1A-166C-E411-ACC7-00266CFFA120.root"),
                            skipEvents=cms.untracked.uint32(0)
                            )
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32( 10) )
process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32( 1 )

process.load("flashgg/MicroAODProducers/flashggMicroAODSequence_cff")
process.load("flashgg/JetValidation/JetEnergyCorr_cff")

from RecoJets.JetProducers.ak4PFJets_cfi  import ak4PFJets
from RecoJets.JetProducers.ak4GenJets_cfi import ak4GenJets
from PhysicsTools.PatAlgos.tools.jetTools import addJetCollection


#-------------------> BEGIN PF Jets <----------------
#Import RECO jet producer for ak4 PF and GEN jet



process.ak4PFJets = ak4PFJets.clone(src = 'packedPFCandidates', doAreaFastjet = True)
process.ak4GenJets = ak4GenJets.clone(src = 'packedGenParticles')
addJetCollection(
		process,
		postfix   = "",
		labelName = 'AK4PF',
		jetSource = cms.InputTag('ak4PFJets'),
		trackSource = cms.InputTag('unpackedTracksAndVertices'), 
		pvSource = cms.InputTag('unpackedTracksAndVertices'), 
		jetCorrections = ('AK4PF', cms.vstring(['L1FastJet', 'L2Relative', 'L3Absolute']), 'None'),
		btagDiscriminators = [      'combinedSecondaryVertexBJetTags'     ]
		,algo= 'AK', rParam = 0.4
		)
#adjust MC matching
process.patJetGenJetMatchAK4PF.matched = "ak4GenJets"
process.patJetPartonMatchAK4PF.matched = "prunedGenParticles"
process.patJetPartons.particles = "prunedGenParticles"
#adjust PV used for Jet Corrections
process.patJetCorrFactorsAK4PF.primaryVertices = "offlineSlimmedPrimaryVertices"
#---------> END  PF JET REPROCESSING <-------------------

#select isolated  muons and electrons collections
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

#---------> BEGIN PFCHS 0 REPROCESSING <-------------------
# first select the packedCandidates passing the loose "fromPV()" requirement (equivalent to CHS definition used for Jets in Run I)
process.pfCHS0 = cms.EDFilter("CandPtrSelector", src = cms.InputTag("packedPFCandidates"), cut = cms.string("fromPV"))
# then remove the previously selected muons
process.pfNoMuonCHS0 =  cms.EDProducer("CandPtrProjector", src = cms.InputTag("pfCHS0"), veto = cms.InputTag("selectedMuons"))
# then remove the previously selected electrons
process.pfNoElectronsCHS0 = cms.EDProducer("CandPtrProjector", src = cms.InputTag("pfNoMuonCHS0"), veto =  cms.InputTag("selectedElectrons"))
process.ak4PFJetsCHS0 = ak4PFJets.clone(src = 'pfNoElectronsCHS0', doAreaFastjet = True)
process.ak4GenJets0 = ak4GenJets.clone(src = 'packedGenParticles')
addJetCollection(
		process,
		postfix   = "",
		labelName = 'AK4PFCHS0',
		jetSource = cms.InputTag('ak4PFJetsCHS0'),
		trackSource = cms.InputTag('unpackedTracksAndVertices'), 
		pvSource = cms.InputTag('unpackedTracksAndVertices'), 
		jetCorrections = ('AK4PFchs', cms.vstring(['L1FastJet', 'L2Relative', 'L3Absolute']), 'None'),
		btagDiscriminators = [      'combinedSecondaryVertexBJetTags'     ]
		,algo= 'AK', rParam = 0.4
		)
#adjust MC matching
process.patJetGenJetMatchAK4PFCHS0.matched = "ak4GenJets"
process.patJetPartonMatchAK4PFCHS0.matched = "prunedGenParticles"
process.patJetPartons.particles = "prunedGenParticles"
#adjust PV used for Jet Corrections
process.patJetCorrFactorsAK4PFCHS0.primaryVertices = "offlineSlimmedPrimaryVertices"
process.load('PhysicsTools.PatAlgos.slimming.unpackedTracksAndVertices_cfi')
process.combinedSecondaryVertex.trackMultiplicityMin = 1  #needed for CMSSW < 71X
#---------> END  PFCHS 0 REPROCESSING <-------------------

#---------> PFCHS Legacy VERTEX <-------------------
process.flashggCHSLegacyVertexCandidates = cms.EDProducer('FlashggCHSLegacyVertexCandidateProducer',
		PFCandidatesTag=cms.untracked.InputTag('packedPFCandidates'),
		DiPhotonTag=cms.untracked.InputTag('flashggDiPhotons'),
		VertexCandidateMapTag = cms.InputTag("flashggVertexMapUnique"),
		VertexTag=cms.untracked.InputTag('offlineSlimmedPrimaryVertices')
		)
process.ak4PFJetsCHSLeg = ak4PFJets.clone(src = 'pfNoElectronsCHSLeg', doAreaFastjet = True)
# first select the packedCandidates passing the loose "fromPV()" requirement (equivalent to CHS definition used for Jets in Run I)
process.pfCHSLeg = cms.EDFilter("CandPtrSelector", src = cms.InputTag("flashggCHSLegacyVertexCandidates"), cut = cms.string(""))
# then remove the previously selected muons
process.pfNoMuonCHSLeg =  cms.EDProducer("CandPtrProjector", src = cms.InputTag("pfCHSLeg"), veto = cms.InputTag("selectedMuons"))
# then remove the previously selected electrons
process.pfNoElectronsCHSLeg = cms.EDProducer("CandPtrProjector", src = cms.InputTag("pfNoMuonCHSLeg"), veto =  cms.InputTag("selectedElectrons"))
#Import RECO jet producer for ak4 PF and GEN jet
addJetCollection(
		process,
		postfix   = "",
		labelName = 'AK4PFCHSLeg',
		jetSource = cms.InputTag('ak4PFJetsCHSLeg'),
		trackSource = cms.InputTag('unpackedTracksAndVertices'), 
		pvSource = cms.InputTag('unpackedTracksAndVertices'), 
		jetCorrections = ('AK4PFchs', cms.vstring(['L1FastJet', 'L2Relative', 'L3Absolute']), 'None'),
		btagDiscriminators = [      'combinedSecondaryVertexBJetTags'     ]
		,algo= 'AK', rParam = 0.4
		)
#adjust MC matching
process.patJetGenJetMatchAK4PFCHSLeg.matched = "ak4GenJetsLeg"
process.patJetPartonMatchAK4PFCHSLeg.matched = "prunedGenParticles"
process.patJetPartons.particles = "prunedGenParticles"
#adjust PV used for Jet Corrections
process.patJetCorrFactorsAK4PFCHSLeg.primaryVertices = "offlineSlimmedPrimaryVertices"
#---------> END PFCHS Leg VERTEX <-------------------

puppiCentral = cms.VPSet(
		cms.PSet(
			algoId           = cms.untracked.int32(5),  #0 is default Puppi
			useCharged       = cms.untracked.bool(True),
			applyLowPUCorr   = cms.untracked.bool(True),
			combOpt          = cms.untracked.int32(0),
			cone             = cms.untracked.double(0.3),
			rmsPtMin         = cms.untracked.double(0.1),
			rmsScaleFactor   = cms.untracked.double(1.0)
			)
		)
puppiForward = cms.VPSet(
		cms.PSet(
			algoId         = cms.untracked.int32(5),  #0 is default Puppi
			useCharged     = cms.untracked.bool(False),
			applyLowPUCorr = cms.untracked.bool(True),
			combOpt        = cms.untracked.int32(0),
			cone           = cms.untracked.double(0.3),
			rmsPtMin       = cms.untracked.double(0.5),
			rmsScaleFactor = cms.untracked.double(1.0)
			)
		)

#u----------------> PUPPI 0 < ---------------------
process.puppi0 = cms.EDProducer("PuppiProducer",
		PuppiName      = cms.untracked.string("Puppi"),
		#UseDeltaZCut   = cms.untracked.bool  (True), #remvoing this cut
		UseDeltaZCut   = cms.untracked.bool  (False), #remvoing this cut
		DeltaZCut      = cms.untracked.double(0.2),
		#candName       = cms.untracked.string('particleFlow'),
		#vertexName     = cms.untracked.string('offlinePrimaryVertices'),
		UseFlashggVertex = cms.untracked.bool(False), #CONTROLS Legacy Vertex (True)vs 0th Vtx (False).Default is False. 
		candName      = cms.untracked.string('packedPFCandidates'),
		vertexName     = cms.untracked.string('offlineSlimmedPrimaryVertices'),
		VertexCandidateMapTag = cms.InputTag("flashggVertexMapUnique"),
		DiPhotonTag   = cms.untracked.InputTag('flashggDiPhotons'),
		VertexTag=cms.untracked.InputTag('offlineSlimmedPrimaryVertices'),
		applyCHS       = cms.untracked.bool  (True),
		useExp         = cms.untracked.bool  (False),
		MinPuppiWeight = cms.untracked.double(0.01),
		algos          = cms.VPSet( 
			cms.PSet( 
				etaMin = cms.untracked.double(0.),
				etaMax = cms.untracked.double( 2.5),
				ptMin  = cms.untracked.double(0.),
				MinNeutralPt   = cms.untracked.double(0.2),
				MinNeutralPtSlope   = cms.untracked.double(0.02),
				puppiAlgos = puppiCentral
				),
			cms.PSet( 
				etaMin = cms.untracked.double(2.5),
				etaMax = cms.untracked.double(3.0),
				ptMin  = cms.untracked.double(0.0),
				MinNeutralPt        = cms.untracked.double(1.0),
				MinNeutralPtSlope   = cms.untracked.double(0.005),
				puppiAlgos = puppiForward
				),
			cms.PSet( 
				etaMin = cms.untracked.double(3.0),
				etaMax = cms.untracked.double(10.0),
				ptMin  = cms.untracked.double(0.0),
				MinNeutralPt        = cms.untracked.double(1.5),
				MinNeutralPtSlope   = cms.untracked.double(0.005),
				puppiAlgos = puppiForward
				)

			)
			)
process.ak4PFJetsPUPPI0 = ak4PFJets.clone(src = cms.InputTag('puppi0','Puppi'), doAreaFastjet = True)
addJetCollection(
		process,
		postfix   = "",
		labelName = 'AK4PFPUPPI0',
		jetSource = cms.InputTag('ak4PFJetsPUPPI0'),
		trackSource = cms.InputTag('unpackedTracksAndVertices'), 
		pvSource = cms.InputTag('unpackedTracksAndVertices'), 
		jetCorrections = ('AK4PFchs', cms.vstring(['L1FastJet', 'L2Relative', 'L3Absolute']), 'None'),
		btagDiscriminators = [      'combinedSecondaryVertexBJetTags'     ]
		,algo= 'AK', rParam = 0.4
		)
#adjust MC matching
process.patJetGenJetMatchAK4PFPUPPI0.matched = "ak4GenJets"
process.patJetPartonMatchAK4PFPUPPI0.matched = "prunedGenParticles"
process.patJetPartons.particles = "prunedGenParticles"
#adjust PV used for Jet Corrections
process.patJetCorrFactorsAK4PFPUPPI0.primaryVertices = "offlineSlimmedPrimaryVertices"
#---------> END  PUPPI0  REPROCESSING <-------------------


#u----------------> PUPPI Leg < ---------------------
process.puppiLeg = cms.EDProducer("PuppiProducer",
					PuppiName      = cms.untracked.string("Puppi"),
					#UseDeltaZCut   = cms.untracked.bool  (True), #Removing this cut, as it seems to have a bug.
					UseDeltaZCut   = cms.untracked.bool  (False), 
					DeltaZCut      = cms.untracked.double(0.2),
					#candName       = cms.untracked.string('particleFlow'),
					#vertexName     = cms.untracked.string('offlinePrimaryVertices'),
					UseFlashggVertex = cms.untracked.bool(True), #CONTROLS Legacy Vertex (True)vs 0th Vtx (False).Default is False. 
					candName      = cms.untracked.string('packedPFCandidates'),
					vertexName     = cms.untracked.string('offlineSlimmedPrimaryVertices'),
					VertexCandidateMapTag = cms.InputTag("flashggVertexMapUnique"),
					DiPhotonTag   = cms.untracked.InputTag('flashggDiPhotons'),
					VertexTag=cms.untracked.InputTag('offlineSlimmedPrimaryVertices'),
					applyCHS       = cms.untracked.bool  (True),
					useExp         = cms.untracked.bool  (False),
					MinPuppiWeight = cms.untracked.double(0.01),
					algos          = cms.VPSet( 
						cms.PSet( 
							etaMin = cms.untracked.double(0.),
							etaMax = cms.untracked.double( 2.5),
							ptMin  = cms.untracked.double(0.),
							MinNeutralPt   = cms.untracked.double(0.2),
							MinNeutralPtSlope   = cms.untracked.double(0.02),
							puppiAlgos = puppiCentral
							),
						cms.PSet( 
							etaMin = cms.untracked.double(2.5),
							etaMax = cms.untracked.double(3.0),
							ptMin  = cms.untracked.double(0.0),
							MinNeutralPt        = cms.untracked.double(1.0),
							MinNeutralPtSlope   = cms.untracked.double(0.005),
							puppiAlgos = puppiForward
							),
						cms.PSet( 
							etaMin = cms.untracked.double(3.0),
							etaMax = cms.untracked.double(10.0),
							ptMin  = cms.untracked.double(0.0),
							MinNeutralPt        = cms.untracked.double(1.5),
							MinNeutralPtSlope   = cms.untracked.double(0.005),
							puppiAlgos = puppiForward
							)

						)
						)

process.ak4PFJetsPUPPILeg = ak4PFJets.clone(src = cms.InputTag('puppiLeg','Puppi'), doAreaFastjet = True)
addJetCollection(
		process,
		postfix   = "",
		labelName = 'AK4PFPUPPILeg',
		jetSource = cms.InputTag('ak4PFJetsPUPPILeg'),
		trackSource = cms.InputTag('unpackedTracksAndVertices'), 
		pvSource = cms.InputTag('unpackedTracksAndVertices'), 
		jetCorrections = ('AK4PFchs', cms.vstring(['L1FastJet', 'L2Relative', 'L3Absolute']), 'None'),
		btagDiscriminators = [      'combinedSecondaryVertexBJetTags'     ]
		,algo= 'AK', rParam = 0.4
		)
#adjust MC matching
process.patJetGenJetMatchAK4PFPUPPILeg.matched = "ak4GenJets"
process.patJetPartonMatchAK4PFPUPPILeg.matched = "prunedGenParticles"

process.patJetCorrFactorsAK4PFPUPPILeg.primaryVertices = "offlineSlimmedPrimaryVertices"
#---------> END  PUPPI Leg  REPROCESSING <-------------------
from RecoJets.JetProducers.PileupJetIDParams_cfi import full_53x

process.flashggJets = cms.EDProducer('FlashggJetProducer',
		DiPhotonTag=cms.untracked.InputTag('flashggDiPhotons'),
		VertexTag=cms.untracked.InputTag('offlineSlimmedPrimaryVertices'),
		JetTag=cms.untracked.InputTag('patJetsAK4PF'),
		VertexCandidateMapTag = cms.InputTag("flashggVertexMapUnique"),
		PileupJetIdParameters=cms.PSet(full_53x) # from PileupJetIDParams_cfi
		)

process.flashggJetsPFCHS0 = cms.EDProducer('FlashggJetProducer',
		DiPhotonTag=cms.untracked.InputTag('flashggDiPhotons'),
		VertexTag=cms.untracked.InputTag('offlineSlimmedPrimaryVertices'),
		JetTag=cms.untracked.InputTag('patJetsAK4PFCHS0'),
		VertexCandidateMapTag = cms.InputTag("flashggVertexMapUnique"),
		PileupJetIdParameters=cms.PSet(full_53x) # from PileupJetIDParams_cfi
		)

process.flashggJetsPFCHSLeg = cms.EDProducer('FlashggJetProducer',
		DiPhotonTag=cms.untracked.InputTag('flashggDiPhotons'),
		VertexTag=cms.untracked.InputTag('offlineSlimmedPrimaryVertices'),
		JetTag=cms.untracked.InputTag('patJetsAK4PFCHSLeg'),
		VertexCandidateMapTag = cms.InputTag("flashggVertexMapUnique"),
		PileupJetIdParameters=cms.PSet(full_53x) # from PileupJetIDParams_cfi
		)
process.flashggJetsPUPPI0 = cms.EDProducer('FlashggJetProducer',
		DiPhotonTag=cms.untracked.InputTag('flashggDiPhotons'),
		VertexTag=cms.untracked.InputTag('offlineSlimmedPrimaryVertices'),
		JetTag=cms.untracked.InputTag('patJetsAK4PFPUPPI0'),
		VertexCandidateMapTag = cms.InputTag("flashggVertexMapUnique"),
		UsePuppi = cms.untracked.bool(True),
		PileupJetIdParameters=cms.PSet(full_53x) # from PileupJetIDParams_cfi
		)
process.flashggJetsPUPPILeg = cms.EDProducer('FlashggJetProducer',
		DiPhotonTag=cms.untracked.InputTag('flashggDiPhotons'),
		VertexTag=cms.untracked.InputTag('offlineSlimmedPrimaryVertices'),
		JetTag=cms.untracked.InputTag('patJetsAK4PFPUPPILeg'),
		VertexCandidateMapTag = cms.InputTag("flashggVertexMapUnique"),
		UsePuppi = cms.untracked.bool(True),
		PileupJetIdParameters=cms.PSet(full_53x) # from PileupJetIDParams_cfi
		)
##Tag stuff
process.load("flashgg/TagProducers/flashggDiPhotonMVA_cfi")
process.load("flashgg/TagProducers/flashggVBFMVA_cff")
process.load("flashgg/TagProducers/flashggTags_cff")

process.flashggTagSorter = cms.EDProducer('FlashggTagSorter',
		DiPhotonTag = cms.untracked.InputTag('flashggDiPhotons'),
		TagVectorTag = cms.untracked.VInputTag(cms.untracked.InputTag('flashggVBFTag'),
			cms.untracked.InputTag('flashggUntaggedCategory'),
			),
		massCutUpper=cms.untracked.double(180),
		massCutLower=cms.untracked.double(100)
		)

process.TFileService = cms.Service("TFileService",fileName  = cms.string("jetValidationCollection_ggH_JEC_small.root"))

process.flashggPFCollAnalyzer = cms.EDAnalyzer('FlashggFlashggPFCollAnalyzer',
		CollTagPF         = cms.InputTag("packedPFCandidates"),
		CollTagPFPFCHS0   = cms.InputTag("pfNoElectronsCHS0"),
		CollTagPFPFCHSLeg = cms.InputTag("pfNoElectronsCHSLeg"),
		CollTagPFPUPPI0   = cms.InputTag('puppi0'   ,'Puppi'),
		CollTagPFPUPPILeg = cms.InputTag('puppiLeg' ,'Puppi')
		)

process.flashggJetValidationTreeMaker = cms.EDAnalyzer('FlashggJetValidationTreeMaker',
		GenParticleTag           = cms.untracked.InputTag('prunedGenParticles'),
		JetTagDz                 = cms.InputTag("flashggJets"),
		DiPhotonTag=cms.untracked.InputTag('flashggDiPhotons'),
		VertexTag=cms.untracked.InputTag('offlineSlimmedPrimaryVertices'),
		StringTag		= cms.string("PF"),
		)

process.flashggJetValidationTreeMakerPFCHS0 = cms.EDAnalyzer('FlashggJetValidationTreeMaker',
		GenParticleTag     = cms.untracked.InputTag('prunedGenParticles'),
		JetTagDz           = cms.InputTag("flashggJetsPFCHS0"),
		DiPhotonTag=cms.untracked.InputTag('flashggDiPhotons'),
		VertexTag=cms.untracked.InputTag('offlineSlimmedPrimaryVertices'),
		StringTag		= cms.string("PFCHS0"),
		)

process.flashggJetValidationTreeMakerPFCHSLeg = cms.EDAnalyzer('FlashggJetValidationTreeMaker',
		GenParticleTag           = cms.untracked.InputTag('prunedGenParticles'),
		JetTagDz                 = cms.InputTag("flashggJetsPFCHSLeg"),
		DiPhotonTag=cms.untracked.InputTag('flashggDiPhotons'),
		VertexTag=cms.untracked.InputTag('offlineSlimmedPrimaryVertices'),
		StringTag		= cms.string("PFCHSLeg"),
		)

process.flashggJetValidationTreeMakerPUPPI0 = cms.EDAnalyzer('FlashggJetValidationTreeMaker',
		GenParticleTag           = cms.untracked.InputTag('prunedGenParticles'),
		JetTagDz                 = cms.InputTag("flashggJetsPUPPI0"),
		DiPhotonTag=cms.untracked.InputTag('flashggDiPhotons'),
		VertexTag=cms.untracked.InputTag('offlineSlimmedPrimaryVertices'),
		StringTag	       	      = cms.string("PUPPI0"),
		)
process.flashggJetValidationTreeMakerPUPPILeg = cms.EDAnalyzer('FlashggJetValidationTreeMaker',
		GenParticleTag           = cms.untracked.InputTag('prunedGenParticles'),
		JetTagDz                 = cms.InputTag("flashggJetsPUPPILeg"),
		DiPhotonTag=cms.untracked.InputTag('flashggDiPhotons'),
		VertexTag=cms.untracked.InputTag('offlineSlimmedPrimaryVertices'),
		StringTag		= cms.string("PUPPILeg"),
		)
#new PAT default running is "unscheduled" so we just need to say in the outputCommands what we want to store
process.options = cms.untracked.PSet(
			allowUnscheduled = cms.untracked.bool(True)
			)

process.p = cms.Path(
		#	process.flashggPFCollAnalyzer + 
			process.flashggJetValidationTreeMaker +
			process.flashggJetValidationTreeMakerPFCHS0 +
			process.flashggJetValidationTreeMakerPFCHSLeg +
			process.flashggJetValidationTreeMakerPUPPI0 +
			process.flashggJetValidationTreeMakerPUPPILeg 
		#process.commissioning*
			)

process.OUT = cms.OutputModule("PoolOutputModule",
                               fileName = cms.untracked.string('testv2.root'),
                               outputCommands = cms.untracked.vstring(['drop *','keep patJets_patJetsAK4PFCHS_*_*','keep *_*_*_PAT' ,'keep flashgg*_*_*_*'])
                               )
process.endpath= cms.EndPath(process.OUT)

