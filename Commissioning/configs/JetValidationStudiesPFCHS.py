import FWCore.ParameterSet.Config as cms

process = cms.Process("FLASHggMicroAOD")
process.load("FWCore.MessageService.MessageLogger_cfi")


# Input source
process.source = cms.Source("PoolSource",
                            #fileNames=cms.untracked.vstring("file:/afs/cern.ch/work/l/lcorpe/private/FLASHgg/CMSSW_7_0_7_patch1/src/flashgg/CE926731-9607-E411-B0BA-001E67248A1B.root")
                            fileNames=cms.untracked.vstring("/store/mc/Spring14miniaod/VBF_HToGG_M-125_13TeV-powheg-pythia6/MINIAODSIM/PU20bx25_POSTLS170_V5-v1/00000/CE926731-9607-E411-B0BA-001E67248A1B.root"),
                            #fileNames=cms.untracked.vstring("root://xrootd.unl.edu//store/mc/Spring14miniaod/VBF_HToGG_M-125_13TeV-powheg-pythia6/MINIAODSIM/141029_PU40bx50_PLS170_V6AN2-v1/10000/5C3A5675-7C72-E411-AC85-003048D436EA.root"),
                            #fileNames=cms.untracked.vstring("root://cms-xrd-global.cern.ch//store/mc/Spring14miniaod/VBF_HToGG_M-125_13TeV-powheg-pythia6/MINIAODSIM/141029_PU40bx50_PLS170_V6AN2-v1/10000/5C3A5675-7C72-E411-AC85-003048D436EA.root"),
                            #skipEvents=cms.untracked.uint32(13000)
                            )

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32( 5000 ) )
process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32( 1 )

process.load("flashgg/MicroAODProducers/flashggVertexMaps_cfi")
process.flashggVertexMapUnique02=process.flashggVertexMapUnique.clone(MaxAllowedDz=0.2)
process.flashggVertexMapUnique05=process.flashggVertexMapUnique.clone(MaxAllowedDz=0.5)
process.flashggVertexMapUnique1=process.flashggVertexMapUnique.clone(MaxAllowedDz=1)
process.flashggVertexMapUnique2=process.flashggVertexMapUnique.clone(MaxAllowedDz=2)
process.load("flashgg/MicroAODProducers/flashggPhotons_cfi")
process.load("flashgg/MicroAODProducers/flashggDiPhotons_cfi")
process.load("flashgg/MicroAODProducers/flashggPreselectedDiPhotons_cfi")
process.load("flashgg/MicroAODProducers/flashggElectrons_cfi")

#-------------------> BEGIN PF Jets <----------------
#Import RECO jet producer for ak4 PF and GEN jet

from RecoJets.JetProducers.ak4PFJets_cfi  import ak4PFJets
from RecoJets.JetProducers.ak4GenJets_cfi import ak4GenJets
process.ak4PFJets = ak4PFJets.clone(src = 'packedPFCandidates', doAreaFastjet = True)
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
# the following part is needed if you want to run b-tagging on the freshly made jets
# CAVEAT: it is not 100% the same b-tagging as in RECO, but performance plots are almost identical
# As tracks are not stored in miniAOD, and b-tag fwk for CMSSW < 72X does not accept candidates
# we need to recreate tracks and pv for btagging in standard reco format:
process.load('PhysicsTools.PatAlgos.slimming.unpackedTracksAndVertices_cfi')
process.combinedSecondaryVertex.trackMultiplicityMin = 1  #needed for CMSSW < 71X
#---------> END  PF JET REPROCESSING <-------------------

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

#---------> BEGIN PFCHS0fv REPROCESSING <-------------------
#select isolated  muons and electrons collections
#tune the requirements to whatever ID and isolation you prefer 
# first select the packedCandidates passing the loose "fromPV()" requirement (equivalent to CHS definition used for Jets in Run I)
#process.pfCHS0 = cms.EDFilter("CandPtrSelector", src = cms.InputTag("packedPFCandidates"), cut = cms.string("fromPV"))
process.pfCHS0fpv1 = cms.EDFilter("CandPtrSelector", src = cms.InputTag("packedPFCandidates"), cut = cms.string("fromPV>0"))
# then remove the previously selected muons
process.pfNoMuonCHS0fpv1 =  cms.EDProducer("CandPtrProjector", src = cms.InputTag("pfCHS0fpv1"), veto = cms.InputTag("selectedMuons"))
# then remove the previously selected electrons
process.pfNoElectronsCHS0fpv1 = cms.EDProducer("CandPtrProjector", src = cms.InputTag("pfNoMuonCHS0fpv1"), veto =  cms.InputTag("selectedElectrons"))
	#Import RECO jet producer for ak4 PF and GEN jet
from RecoJets.JetProducers.ak4PFJets_cfi import ak4PFJets
from RecoJets.JetProducers.ak4GenJets_cfi import ak4GenJets
process.ak4PFJetsCHS0fpv1 = ak4PFJets.clone(src = 'pfNoElectronsCHS0fpv1', doAreaFastjet = True)
process.ak4GenJets0 = ak4GenJets.clone(src = 'packedGenParticles')
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
    labelName = 'AK4PFCHS0fpv1',
    jetSource = cms.InputTag('ak4PFJetsCHS0fpv1'),
    trackSource = cms.InputTag('unpackedTracksAndVertices'), 
    pvSource = cms.InputTag('unpackedTracksAndVertices'), 
    jetCorrections = ('AK4PFchs', cms.vstring(['L1FastJet', 'L2Relative', 'L3Absolute']), 'None'),
    btagDiscriminators = [      'combinedSecondaryVertexBJetTags'     ]
    ,algo= 'AK', rParam = 0.4
    )
#adjust MC matching
process.patJetGenJetMatchAK4PFCHS0fpv1.matched = "ak4GenJets"
process.patJetPartonMatchAK4PFCHS0fpv1.matched = "prunedGenParticles"
process.patJetPartons.particles = "prunedGenParticles"
#adjust PV used for Jet Corrections
process.patJetCorrFactorsAK4PFCHS0fpv1.primaryVertices = "offlineSlimmedPrimaryVertices"
# the following part is needed if you want to run b-tagging on the freshly made jets
# CAVEAT: it is not 100% the same b-tagging as in RECO, but performance plots are almost identical
# As tracks are not stored in miniAOD, and b-tag fwk for CMSSW < 72X does not accept candidates
# we need to recreate tracks and pv for btagging in standard reco format:
process.load('PhysicsTools.PatAlgos.slimming.unpackedTracksAndVertices_cfi')
process.combinedSecondaryVertex.trackMultiplicityMin = 1  #needed for CMSSW < 71X
#---------> END  PFCHS 0 REPROCESSINGfpv1 <-------------------
#---------> BEGIN PFCHS0fv1 REPROCESSING <-------------------
#select isolated  muons and electrons collections
#tune the requirements to whatever ID and isolation you prefer 
# first select the packedCandidates passing the loose "fromPV()" requirement (equivalent to CHS definition used for Jets in Run I)
#process.pfCHS0 = cms.EDFilter("CandPtrSelector", src = cms.InputTag("packedPFCandidates"), cut = cms.string("fromPV"))
process.pfCHS0fpv2 = cms.EDFilter("CandPtrSelector", src = cms.InputTag("packedPFCandidates"), cut = cms.string("fromPV>1"))
# then remove the previously selected muons
process.pfNoMuonCHS0fpv2 =  cms.EDProducer("CandPtrProjector", src = cms.InputTag("pfCHS0fpv2"), veto = cms.InputTag("selectedMuons"))
# then remove the previously selected electrons
process.pfNoElectronsCHS0fpv2 = cms.EDProducer("CandPtrProjector", src = cms.InputTag("pfNoMuonCHS0fpv2"), veto =  cms.InputTag("selectedElectrons"))
	#Import RECO jet producer for ak4 PF and GEN jet
from RecoJets.JetProducers.ak4PFJets_cfi import ak4PFJets
from RecoJets.JetProducers.ak4GenJets_cfi import ak4GenJets
process.ak4PFJetsCHS0fpv2 = ak4PFJets.clone(src = 'pfNoElectronsCHS0fpv2', doAreaFastjet = True)
process.ak4GenJets0 = ak4GenJets.clone(src = 'packedGenParticles')
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
    labelName = 'AK4PFCHS0fpv2',
    jetSource = cms.InputTag('ak4PFJetsCHS0fpv2'),
    trackSource = cms.InputTag('unpackedTracksAndVertices'), 
    pvSource = cms.InputTag('unpackedTracksAndVertices'), 
    jetCorrections = ('AK4PFchs', cms.vstring(['L1FastJet', 'L2Relative', 'L3Absolute']), 'None'),
    btagDiscriminators = [      'combinedSecondaryVertexBJetTags'     ]
    ,algo= 'AK', rParam = 0.4
    )
#adjust MC matching
process.patJetGenJetMatchAK4PFCHS0fpv2.matched = "ak4GenJets"
process.patJetPartonMatchAK4PFCHS0fpv2.matched = "prunedGenParticles"
process.patJetPartons.particles = "prunedGenParticles"
#adjust PV used for Jet Corrections
process.patJetCorrFactorsAK4PFCHS0fpv2.primaryVertices = "offlineSlimmedPrimaryVertices"
# the following part is needed if you want to run b-tagging on the freshly made jets
# CAVEAT: it is not 100% the same b-tagging as in RECO, but performance plots are almost identical
# As tracks are not stored in miniAOD, and b-tag fwk for CMSSW < 72X does not accept candidates
# we need to recreate tracks and pv for btagging in standard reco format:
process.load('PhysicsTools.PatAlgos.slimming.unpackedTracksAndVertices_cfi')
process.combinedSecondaryVertex.trackMultiplicityMin = 1  #needed for CMSSW < 71X
#---------> END  PFCHS 0 REPROCESSINGfpv2 <-------------------
#---------> BEGIN PFCHS0fv1 REPROCESSING <-------------------
#select isolated  muons and electrons collections
#tune the requirements to whatever ID and isolation you prefer 
# first select the packedCandidates passing the loose "fromPV()" requirement (equivalent to CHS definition used for Jets in Run I)
#process.pfCHS0 = cms.EDFilter("CandPtrSelector", src = cms.InputTag("packedPFCandidates"), cut = cms.string("fromPV"))
process.pfCHS0fpv3 = cms.EDFilter("CandPtrSelector", src = cms.InputTag("packedPFCandidates"), cut = cms.string("fromPV>2"))
# then remove the previously selected muons
process.pfNoMuonCHS0fpv3 =  cms.EDProducer("CandPtrProjector", src = cms.InputTag("pfCHS0fpv3"), veto = cms.InputTag("selectedMuons"))
# then remove the previously selected electrons
process.pfNoElectronsCHS0fpv3 = cms.EDProducer("CandPtrProjector", src = cms.InputTag("pfNoMuonCHS0fpv3"), veto =  cms.InputTag("selectedElectrons"))
	#Import RECO jet producer for ak4 PF and GEN jet
from RecoJets.JetProducers.ak4PFJets_cfi import ak4PFJets
from RecoJets.JetProducers.ak4GenJets_cfi import ak4GenJets
process.ak4PFJetsCHS0fpv3 = ak4PFJets.clone(src = 'pfNoElectronsCHS0fpv3', doAreaFastjet = True)
process.ak4GenJets0 = ak4GenJets.clone(src = 'packedGenParticles')
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
    labelName = 'AK4PFCHS0fpv3',
    jetSource = cms.InputTag('ak4PFJetsCHS0fpv3'),
    trackSource = cms.InputTag('unpackedTracksAndVertices'), 
    pvSource = cms.InputTag('unpackedTracksAndVertices'), 
    jetCorrections = ('AK4PFchs', cms.vstring(['L1FastJet', 'L2Relative', 'L3Absolute']), 'None'),
    btagDiscriminators = [      'combinedSecondaryVertexBJetTags'     ]
    ,algo= 'AK', rParam = 0.4
    )
#adjust MC matching
process.patJetGenJetMatchAK4PFCHS0fpv3.matched = "ak4GenJets"
process.patJetPartonMatchAK4PFCHS0fpv3.matched = "prunedGenParticles"
process.patJetPartons.particles = "prunedGenParticles"
#adjust PV used for Jet Corrections
process.patJetCorrFactorsAK4PFCHS0fpv3.primaryVertices = "offlineSlimmedPrimaryVertices"
# the following part is needed if you want to run b-tagging on the freshly made jets
# CAVEAT: it is not 100% the same b-tagging as in RECO, but performance plots are almost identical
# As tracks are not stored in miniAOD, and b-tag fwk for CMSSW < 72X does not accept candidates
# we need to recreate tracks and pv for btagging in standard reco format:
process.load('PhysicsTools.PatAlgos.slimming.unpackedTracksAndVertices_cfi')
process.combinedSecondaryVertex.trackMultiplicityMin = 1  #needed for CMSSW < 71X
#---------> END  PFCHS 0 REPROCESSINGfpv3 <-------------------

#---------> BEGIN PFCHSf0dz02 REPROCESSING <-------------------
process.flashggCHS0thVertexCandidatesf0dz02 = cms.EDProducer('FlashggCHSLegacyVertexCandidateProducer',
                                                          PFCandidatesTag=cms.untracked.InputTag('packedPFCandidates'),
                                                          DiPhotonTag=cms.untracked.InputTag('flashggDiPhotons'),
                                                          VertexCandidateMapTag = cms.InputTag("flashggVertexMapUnique02"),
                                           								UseZeroth = cms.untracked.bool(True),
                                                          VertexTag=cms.untracked.InputTag('offlineSlimmedPrimaryVertices')
                                                          )
#select isolated  muons and electrons collections
#tune the requirements to whatever ID and isolation you prefer 
# first select the packedCandidates passing the loose "fromPV()" requirement (equivalent to CHS definition used for Jets in Run I)
#process.pfCHS0 = cms.EDFilter("CandPtrSelector", src = cms.InputTag("packedPFCandidates"), cut = cms.string("fromPV"))
process.pfCHS0f0dz02 = cms.EDFilter("CandPtrSelector", src = cms.InputTag("flashggCHS0thVertexCandidatesf0dz02"), cut = cms.string(""))
# then remove the previously selected muons
process.pfNoMuonCHS0f0dz02 =  cms.EDProducer("CandPtrProjector", src = cms.InputTag("pfCHS0f0dz02"), veto = cms.InputTag("selectedMuons"))
# then remove the previously selected electrons
process.pfNoElectronsCHS0f0dz02 = cms.EDProducer("CandPtrProjector", src = cms.InputTag("pfNoMuonCHS0f0dz02"), veto =  cms.InputTag("selectedElectrons"))
	#Import RECO jet producer for ak4 PF and GEN jet
from RecoJets.JetProducers.ak4PFJets_cfi import ak4PFJets
from RecoJets.JetProducers.ak4GenJets_cfi import ak4GenJets
process.ak4PFJetsCHS0f0dz02 = ak4PFJets.clone(src = 'pfNoElectronsCHS0f0dz02', doAreaFastjet = True)
process.ak4GenJets0 = ak4GenJets.clone(src = 'packedGenParticles')
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
    labelName = 'AK4PFCHS0f0dz02',
    jetSource = cms.InputTag('ak4PFJetsCHS0f0dz02'),
    trackSource = cms.InputTag('unpackedTracksAndVertices'), 
    pvSource = cms.InputTag('unpackedTracksAndVertices'), 
    jetCorrections = ('AK4PFchs', cms.vstring(['L1FastJet', 'L2Relative', 'L3Absolute']), 'None'),
    btagDiscriminators = [      'combinedSecondaryVertexBJetTags'     ]
    ,algo= 'AK', rParam = 0.4
    )
#adjust MC matching
process.patJetGenJetMatchAK4PFCHS0f0dz02.matched = "ak4GenJets"
process.patJetPartonMatchAK4PFCHS0f0dz02.matched = "prunedGenParticles"
process.patJetPartons.particles = "prunedGenParticles"
#adjust PV used for Jet Corrections
process.patJetCorrFactorsAK4PFCHS0f0dz02.primaryVertices = "offlineSlimmedPrimaryVertices"
# the following part is needed if you want to run b-tagging on the freshly made jets
# CAVEAT: it is not 100% the same b-tagging as in RECO, but performance plots are almost identical
# As tracks are not stored in miniAOD, and b-tag fwk for CMSSW < 72X does not accept candidates
# we need to recreate tracks and pv for btagging in standard reco format:
process.load('PhysicsTools.PatAlgos.slimming.unpackedTracksAndVertices_cfi')
process.combinedSecondaryVertex.trackMultiplicityMin = 1  #needed for CMSSW < 71X
#---------> END  PFCHS 0 REPROCESSINGf0dz02 <-------------------

#---------> BEGIN PFCHSf0dz05 REPROCESSING <-------------------
process.flashggCHS0thVertexCandidatesf0dz05 = cms.EDProducer('FlashggCHSLegacyVertexCandidateProducer',
                                                          PFCandidatesTag=cms.untracked.InputTag('packedPFCandidates'),
                                                          DiPhotonTag=cms.untracked.InputTag('flashggDiPhotons'),
                                                          VertexCandidateMapTag = cms.InputTag("flashggVertexMapUnique05"),
                                           								UseZeroth = cms.untracked.bool(True),
                                                          VertexTag=cms.untracked.InputTag('offlineSlimmedPrimaryVertices')
                                                          )
#select isolated  muons and electrons collections
#tune the requirements to whatever ID and isolation you prefer 
# first select the packedCandidates passing the loose "fromPV()" requirement (equivalent to CHS definition used for Jets in Run I)
#process.pfCHS0 = cms.EDFilter("CandPtrSelector", src = cms.InputTag("packedPFCandidates"), cut = cms.string("fromPV"))
process.pfCHS0f0dz05 = cms.EDFilter("CandPtrSelector", src = cms.InputTag("flashggCHS0thVertexCandidatesf0dz05"), cut = cms.string(""))
# then remove the previously selected muons
process.pfNoMuonCHS0f0dz05 =  cms.EDProducer("CandPtrProjector", src = cms.InputTag("pfCHS0f0dz05"), veto = cms.InputTag("selectedMuons"))
# then remove the previously selected electrons
process.pfNoElectronsCHS0f0dz05 = cms.EDProducer("CandPtrProjector", src = cms.InputTag("pfNoMuonCHS0f0dz05"), veto =  cms.InputTag("selectedElectrons"))
	#Import RECO jet producer for ak4 PF and GEN jet
from RecoJets.JetProducers.ak4PFJets_cfi import ak4PFJets
from RecoJets.JetProducers.ak4GenJets_cfi import ak4GenJets
process.ak4PFJetsCHS0f0dz05 = ak4PFJets.clone(src = 'pfNoElectronsCHS0f0dz05', doAreaFastjet = True)
process.ak4GenJets0 = ak4GenJets.clone(src = 'packedGenParticles')
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
    labelName = 'AK4PFCHS0f0dz05',
    jetSource = cms.InputTag('ak4PFJetsCHS0f0dz05'),
    trackSource = cms.InputTag('unpackedTracksAndVertices'), 
    pvSource = cms.InputTag('unpackedTracksAndVertices'), 
    jetCorrections = ('AK4PFchs', cms.vstring(['L1FastJet', 'L2Relative', 'L3Absolute']), 'None'),
    btagDiscriminators = [      'combinedSecondaryVertexBJetTags'     ]
    ,algo= 'AK', rParam = 0.4
    )
#adjust MC matching
process.patJetGenJetMatchAK4PFCHS0f0dz05.matched = "ak4GenJets"
process.patJetPartonMatchAK4PFCHS0f0dz05.matched = "prunedGenParticles"
process.patJetPartons.particles = "prunedGenParticles"
#adjust PV used for Jet Corrections
process.patJetCorrFactorsAK4PFCHS0f0dz05.primaryVertices = "offlineSlimmedPrimaryVertices"
# the following part is needed if you want to run b-tagging on the freshly made jets
# CAVEAT: it is not 100% the same b-tagging as in RECO, but performance plots are almost identical
# As tracks are not stored in miniAOD, and b-tag fwk for CMSSW < 72X does not accept candidates
# we need to recreate tracks and pv for btagging in standard reco format:
process.load('PhysicsTools.PatAlgos.slimming.unpackedTracksAndVertices_cfi')
process.combinedSecondaryVertex.trackMultiplicityMin = 1  #needed for CMSSW < 71X
#---------> END  PFCHS 0f0dz05 REPROCESSING <-------------------

#---------> BEGIN PFCHSf0dz1 REPROCESSING <-------------------
process.flashggCHS0thVertexCandidatesf0dz1 = cms.EDProducer('FlashggCHSLegacyVertexCandidateProducer',
                                                          PFCandidatesTag=cms.untracked.InputTag('packedPFCandidates'),
                                                          DiPhotonTag=cms.untracked.InputTag('flashggDiPhotons'),
                                                          VertexCandidateMapTag = cms.InputTag("flashggVertexMapUnique1"),
                                           								UseZeroth = cms.untracked.bool(True),
                                                          VertexTag=cms.untracked.InputTag('offlineSlimmedPrimaryVertices')
                                                          )
#select isolated  muons and electrons collections
#tune the requirements to whatever ID and isolation you prefer 
# first select the packedCandidates passing the loose "fromPV()" requirement (equivalent to CHS definition used for Jets in Run I)
#process.pfCHS0 = cms.EDFilter("CandPtrSelector", src = cms.InputTag("packedPFCandidates"), cut = cms.string("fromPV"))
process.pfCHS0f0dz1 = cms.EDFilter("CandPtrSelector", src = cms.InputTag("flashggCHS0thVertexCandidatesf0dz1"), cut = cms.string(""))
# then remove the previously selected muons
process.pfNoMuonCHS0f0dz1 =  cms.EDProducer("CandPtrProjector", src = cms.InputTag("pfCHS0f0dz1"), veto = cms.InputTag("selectedMuons"))
# then remove the previously selected electrons
process.pfNoElectronsCHS0f0dz1 = cms.EDProducer("CandPtrProjector", src = cms.InputTag("pfNoMuonCHS0f0dz1"), veto =  cms.InputTag("selectedElectrons"))
	#Import RECO jet producer for ak4 PF and GEN jet
from RecoJets.JetProducers.ak4PFJets_cfi import ak4PFJets
from RecoJets.JetProducers.ak4GenJets_cfi import ak4GenJets
process.ak4PFJetsCHS0f0dz1 = ak4PFJets.clone(src = 'pfNoElectronsCHS0f0dz1', doAreaFastjet = True)
process.ak4GenJets0 = ak4GenJets.clone(src = 'packedGenParticles')
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
    labelName = 'AK4PFCHS0f0dz1',
    jetSource = cms.InputTag('ak4PFJetsCHS0f0dz1'),
    trackSource = cms.InputTag('unpackedTracksAndVertices'), 
    pvSource = cms.InputTag('unpackedTracksAndVertices'), 
    jetCorrections = ('AK4PFchs', cms.vstring(['L1FastJet', 'L2Relative', 'L3Absolute']), 'None'),
    btagDiscriminators = [      'combinedSecondaryVertexBJetTags'     ]
    ,algo= 'AK', rParam = 0.4
    )
#adjust MC matching
process.patJetGenJetMatchAK4PFCHS0f0dz1.matched = "ak4GenJets"
process.patJetPartonMatchAK4PFCHS0f0dz1.matched = "prunedGenParticles"
process.patJetPartons.particles = "prunedGenParticles"
#adjust PV used for Jet Corrections
process.patJetCorrFactorsAK4PFCHS0f0dz1.primaryVertices = "offlineSlimmedPrimaryVertices"
# the following part is needed if you want to run b-tagging on the freshly made jets
# CAVEAT: it is not 100% the same b-tagging as in RECO, but performance plots are almost identical
# As tracks are not stored in miniAOD, and b-tag fwk for CMSSW < 72X does not accept candidates
# we need to recreate tracks and pv for btagging in standard reco format:
process.load('PhysicsTools.PatAlgos.slimming.unpackedTracksAndVertices_cfi')
process.combinedSecondaryVertex.trackMultiplicityMin = 1  #needed for CMSSW < 71X
#---------> END  PFCHS 0f0dz1 REPROCESSING <-------------------


#---------> BEGIN PFCHSf0dz2 REPROCESSING <-------------------
process.flashggCHS0thVertexCandidatesf0dz2 = cms.EDProducer('FlashggCHSLegacyVertexCandidateProducer',
                                                          PFCandidatesTag=cms.untracked.InputTag('packedPFCandidates'),
                                                          DiPhotonTag=cms.untracked.InputTag('flashggDiPhotons'),
                                                          VertexCandidateMapTag = cms.InputTag("flashggVertexMapUnique2"),
                                           								UseZeroth = cms.untracked.bool(True),
                                                          VertexTag=cms.untracked.InputTag('offlineSlimmedPrimaryVertices')
                                                          )
#select isolated  muons and electrons collections
#tune the requirements to whatever ID and isolation you prefer 
# first select the packedCandidates passing the loose "fromPV()" requirement (equivalent to CHS definition used for Jets in Run I)
#process.pfCHS0 = cms.EDFilter("CandPtrSelector", src = cms.InputTag("packedPFCandidates"), cut = cms.string("fromPV"))
process.pfCHS0f0dz2 = cms.EDFilter("CandPtrSelector", src = cms.InputTag("flashggCHS0thVertexCandidatesf0dz2"), cut = cms.string(""))
# then remove the previously selected muons
process.pfNoMuonCHS0f0dz2 =  cms.EDProducer("CandPtrProjector", src = cms.InputTag("pfCHS0f0dz2"), veto = cms.InputTag("selectedMuons"))
# then remove the previously selected electrons
process.pfNoElectronsCHS0f0dz2 = cms.EDProducer("CandPtrProjector", src = cms.InputTag("pfNoMuonCHS0f0dz2"), veto =  cms.InputTag("selectedElectrons"))
	#Import RECO jet producer for ak4 PF and GEN jet
from RecoJets.JetProducers.ak4PFJets_cfi import ak4PFJets
from RecoJets.JetProducers.ak4GenJets_cfi import ak4GenJets
process.ak4PFJetsCHS0f0dz2 = ak4PFJets.clone(src = 'pfNoElectronsCHS0f0dz2', doAreaFastjet = True)
process.ak4GenJets0 = ak4GenJets.clone(src = 'packedGenParticles')
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
    labelName = 'AK4PFCHS0f0dz2',
    jetSource = cms.InputTag('ak4PFJetsCHS0f0dz2'),
    trackSource = cms.InputTag('unpackedTracksAndVertices'), 
    pvSource = cms.InputTag('unpackedTracksAndVertices'), 
    jetCorrections = ('AK4PFchs', cms.vstring(['L1FastJet', 'L2Relative', 'L3Absolute']), 'None'),
    btagDiscriminators = [      'combinedSecondaryVertexBJetTags'     ]
    ,algo= 'AK', rParam = 0.4
    )
#adjust MC matching
process.patJetGenJetMatchAK4PFCHS0f0dz2.matched = "ak4GenJets"
process.patJetPartonMatchAK4PFCHS0f0dz2.matched = "prunedGenParticles"
process.patJetPartons.particles = "prunedGenParticles"
#adjust PV used for Jet Corrections
process.patJetCorrFactorsAK4PFCHS0f0dz2.primaryVertices = "offlineSlimmedPrimaryVertices"
# the following part is needed if you want to run b-tagging on the freshly made jets
# CAVEAT: it is not 100% the same b-tagging as in RECO, but performance plots are almost identical
# As tracks are not stored in miniAOD, and b-tag fwk for CMSSW < 72X does not accept candidates
# we need to recreate tracks and pv for btagging in standard reco format:
process.load('PhysicsTools.PatAlgos.slimming.unpackedTracksAndVertices_cfi')
process.combinedSecondaryVertex.trackMultiplicityMin = 1  #needed for CMSSW < 71X
#---------> END  PFCHS 0f0dz2 REPROCESSING <-------------------




from RecoJets.JetProducers.PileupJetIDParams_cfi import full_53x

process.flashggJets = cms.EDProducer('FlashggJetProducer',
                                     DiPhotonTag=cms.untracked.InputTag('flashggDiPhotons'),
                                     VertexTag=cms.untracked.InputTag('offlineSlimmedPrimaryVertices'),
                                     JetTag=cms.untracked.InputTag('patJetsAK4PF'),
                                     VertexCandidateMapTag = cms.InputTag("flashggVertexMapUnique"),
                                     PileupJetIdParameters=cms.PSet(full_53x) # from PileupJetIDParams_cfi
                                     )

process.flashggJetsPFCHS0f0dz02 = cms.EDProducer('FlashggJetProducer',
                                           DiPhotonTag=cms.untracked.InputTag('flashggDiPhotons'),
                                           VertexTag=cms.untracked.InputTag('offlineSlimmedPrimaryVertices'),
                                           JetTag=cms.untracked.InputTag('patJetsAK4PFCHS0f0dz02'),
                                           VertexCandidateMapTag = cms.InputTag("flashggVertexMapUnique"),
                                           PileupJetIdParameters=cms.PSet(full_53x) # from PileupJetIDParams_cfi
                                           )
process.flashggJetsPFCHS0f0dz05 = cms.EDProducer('FlashggJetProducer',
                                           DiPhotonTag=cms.untracked.InputTag('flashggDiPhotons'),
                                           VertexTag=cms.untracked.InputTag('offlineSlimmedPrimaryVertices'),
                                           JetTag=cms.untracked.InputTag('patJetsAK4PFCHS0f0dz05'),
                                           VertexCandidateMapTag = cms.InputTag("flashggVertexMapUnique"),
                                           PileupJetIdParameters=cms.PSet(full_53x) # from PileupJetIDParams_cfi
                                           )
process.flashggJetsPFCHS0f0dz1 = cms.EDProducer('FlashggJetProducer',
                                           DiPhotonTag=cms.untracked.InputTag('flashggDiPhotons'),
                                           VertexTag=cms.untracked.InputTag('offlineSlimmedPrimaryVertices'),
                                           JetTag=cms.untracked.InputTag('patJetsAK4PFCHS0f0dz1'),
                                           VertexCandidateMapTag = cms.InputTag("flashggVertexMapUnique"),
                                           PileupJetIdParameters=cms.PSet(full_53x) # from PileupJetIDParams_cfi
                                           )
process.flashggJetsPFCHS0f0dz2 = cms.EDProducer('FlashggJetProducer',
                                           DiPhotonTag=cms.untracked.InputTag('flashggDiPhotons'),
                                           VertexTag=cms.untracked.InputTag('offlineSlimmedPrimaryVertices'),
                                           JetTag=cms.untracked.InputTag('patJetsAK4PFCHS0f0dz2'),
                                           VertexCandidateMapTag = cms.InputTag("flashggVertexMapUnique"),
                                           PileupJetIdParameters=cms.PSet(full_53x) # from PileupJetIDParams_cfi
                                           )

process.flashggJetsPFCHS0fpv1 = cms.EDProducer('FlashggJetProducer',
                                           DiPhotonTag=cms.untracked.InputTag('flashggDiPhotons'),
                                           VertexTag=cms.untracked.InputTag('offlineSlimmedPrimaryVertices'),
                                           JetTag=cms.untracked.InputTag('patJetsAK4PFCHS0fpv1'),
                                           VertexCandidateMapTag = cms.InputTag("flashggVertexMapUnique"),
                                           PileupJetIdParameters=cms.PSet(full_53x) # from PileupJetIDParams_cfi
                                           )
process.flashggJetsPFCHS0fpv2 = cms.EDProducer('FlashggJetProducer',
                                           DiPhotonTag=cms.untracked.InputTag('flashggDiPhotons'),
                                           VertexTag=cms.untracked.InputTag('offlineSlimmedPrimaryVertices'),
                                           JetTag=cms.untracked.InputTag('patJetsAK4PFCHS0fpv2'),
                                           VertexCandidateMapTag = cms.InputTag("flashggVertexMapUnique"),
                                           PileupJetIdParameters=cms.PSet(full_53x) # from PileupJetIDParams_cfi
                                           )
process.flashggJetsPFCHS0fpv3 = cms.EDProducer('FlashggJetProducer',
                                           DiPhotonTag=cms.untracked.InputTag('flashggDiPhotons'),
                                           VertexTag=cms.untracked.InputTag('offlineSlimmedPrimaryVertices'),
                                           JetTag=cms.untracked.InputTag('patJetsAK4PFCHS0fpv3'),
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
                                          TagVectorTag = cms.untracked.VInputTag(cms.untracked.InputTag('flashggVBFTag'),
                                                                                 cms.untracked.InputTag('flashggUntaggedCategory'),
                                                                                 ),
                                          massCutUpper=cms.untracked.double(180),
                                          massCutLower=cms.untracked.double(100)
                                          )

#process.TFileService = cms.Service("TFileService",fileName = cms.string("jetValidationCollection_PU40bx50.root"))
process.TFileService = cms.Service("TFileService",fileName = cms.string("jetValidationCollection_PU20bx25.root"))
#process.TFileService = cms.Service("TFileService",fileName = cms.string("Test.root"))
process.flashggPFCollAnalyzer = cms.EDAnalyzer('FlashggFlashggPFCollAnalyzer',
                                               #VertexTag=cms.untracked.InputTag('offlineSlimmedPrimaryVertices'),
                                               #GenParticleTag=cms.untracked.InputTag('prunedGenParticles'),
                                               #VertexCandidateMapTagDz=cms.InputTag('flashggVertexMapUnique'),
                                               #VertexCandidateMapTagAOD = cms.InputTag('flashggVertexMapValidator'),
                                               CollTagPF         = cms.InputTag("packedPFCandidates"),
                                               CollTagPFPFCHS0   = cms.InputTag("pfNoElectronsCHS0"),
                                               CollTagPFPFCHSLeg = cms.InputTag("pfNoElectronsCHSLeg"),
                                               CollTagPFPUPPI0   = cms.InputTag('puppi0'   ,'Puppi'),
                                               CollTagPFPUPPILeg = cms.InputTag('puppiLeg' ,'Puppi')
                                               #DiPhotonTag = cms.untracked.InputTag('flashggDiPhotons'),
                                               #METTag = cms.untracked.InputTag('slimmedMETs'),
                                               #PileUpTag = cms.untracked.InputTag('addPileupInfo'),
                                               #TagSorter = cms.untracked.InputTag('flashggTagSorter'),
                                               #rhoFixedGridCollection = cms.InputTag('fixedGridRhoAll'),
                                               )


#process.TFileService = cms.Service("TFileService",fileName = cms.string("JetValidationTree.root"))
process.flashggJetValidationTreeMaker = cms.EDAnalyzer('FlashggJetValidationTreeMaker',
                                                       GenParticleTag           = cms.untracked.InputTag('prunedGenParticles'),
                                                       JetTagDz                 = cms.InputTag("flashggJets"),
																											 StringTag								= cms.string("PF"),
                                                       )

process.flashggJetValidationTreeMakerPFCHS0f0dz02 = cms.EDAnalyzer('FlashggJetValidationTreeMaker',
                                                       GenParticleTag           = cms.untracked.InputTag('prunedGenParticles'),
                                                       JetTagDz                 = cms.InputTag("flashggJetsPFCHS0f0dz02"),
																											 StringTag								= cms.string("PFCHS0f0dz02"),
                                                       )
process.flashggJetValidationTreeMakerPFCHS0f0dz05 = cms.EDAnalyzer('FlashggJetValidationTreeMaker',
                                                       GenParticleTag           = cms.untracked.InputTag('prunedGenParticles'),
                                                       JetTagDz                 = cms.InputTag("flashggJetsPFCHS0f0dz05"),
																											 StringTag								= cms.string("PFCHS0f0dz05"),
                                                       )
process.flashggJetValidationTreeMakerPFCHS0f0dz1 = cms.EDAnalyzer('FlashggJetValidationTreeMaker',
                                                       GenParticleTag           = cms.untracked.InputTag('prunedGenParticles'),
                                                       JetTagDz                 = cms.InputTag("flashggJetsPFCHS0f0dz1"),
																											 StringTag								= cms.string("PFCHS0f0dz1"),
                                                       )
process.flashggJetValidationTreeMakerPFCHS0f0dz2 = cms.EDAnalyzer('FlashggJetValidationTreeMaker',
                                                       GenParticleTag           = cms.untracked.InputTag('prunedGenParticles'),
                                                       JetTagDz                 = cms.InputTag("flashggJetsPFCHS0f0dz2"),
																											 StringTag								= cms.string("PFCHS0f0dz2"),
                                                       )
process.flashggJetValidationTreeMakerPFCHS0fpv1 = cms.EDAnalyzer('FlashggJetValidationTreeMaker',
                                                       GenParticleTag           = cms.untracked.InputTag('prunedGenParticles'),
                                                       JetTagDz                 = cms.InputTag("flashggJetsPFCHS0fpv1"),
																											 StringTag								= cms.string("PFCHS0fpv1"),
                                                       )
process.flashggJetValidationTreeMakerPFCHS0fpv2 = cms.EDAnalyzer('FlashggJetValidationTreeMaker',
                                                       GenParticleTag           = cms.untracked.InputTag('prunedGenParticles'),
                                                       JetTagDz                 = cms.InputTag("flashggJetsPFCHS0fpv2"),
																											 StringTag								= cms.string("PFCHS0fpv2"),
                                                       )
process.flashggJetValidationTreeMakerPFCHS0fpv3 = cms.EDAnalyzer('FlashggJetValidationTreeMaker',
                                                       GenParticleTag           = cms.untracked.InputTag('prunedGenParticles'),
                                                       JetTagDz                 = cms.InputTag("flashggJetsPFCHS0fpv3"),
																											 StringTag								= cms.string("PFCHS0fpv3"),
                                                       )
#new PAT default running is "unscheduled" so we just need to say in the outputCommands what we want to store
process.options = cms.untracked.PSet(
    allowUnscheduled = cms.untracked.bool(True)
    )

process.p = cms.Path(
    #process.flashggPFCollAnalyzer + 
    process.flashggJetValidationTreeMaker +
    process.flashggJetValidationTreeMakerPFCHS0f0dz02 +
    process.flashggJetValidationTreeMakerPFCHS0f0dz05 +
    process.flashggJetValidationTreeMakerPFCHS0f0dz1 +
    process.flashggJetValidationTreeMakerPFCHS0f0dz2+ 
    process.flashggJetValidationTreeMakerPFCHS0fpv1+ 
    process.flashggJetValidationTreeMakerPFCHS0fpv2+ 
    process.flashggJetValidationTreeMakerPFCHS0fpv3 
    #process.commissioning*
    )
process.OUT = cms.OutputModule("PoolOutputModule",
                               fileName = cms.untracked.string('test25.root'),
                               outputCommands = cms.untracked.vstring(['drop *','keep patJets_patJetsAK4PFCHS_*_*','keep *_*_*_PAT' ,'keep flashgg*_*_*_*'])
                               )
process.endpath= cms.EndPath(process.OUT)
