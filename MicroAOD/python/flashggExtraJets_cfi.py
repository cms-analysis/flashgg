import FWCore.ParameterSet.Config as cms

from RecoJets.JetProducers.PileupJetIDParams_cfi import cutbased as pu_jetid
from PhysicsTools.PatAlgos.tools.jetTools import addJetCollection

def addFlashggPF(process):
  print "JET PRODUCER :: Flashgg PF producer +++++ "
  from RecoJets.JetProducers.ak4PFJets_cfi  import ak4PFJets
  from RecoJets.JetProducers.ak4GenJets_cfi import ak4GenJets

  process.ak4PFJets  = ak4PFJets.clone (src = 'packedPFCandidates', doAreaFastjet = True)
  process.ak4GenJets = ak4GenJets.clone(src = 'packedGenParticles')
  ## cluster the jets
  addJetCollection(
    process,
    postfix      = "",
    labelName    = 'AK4PF',
    pfCandidates = cms.InputTag('packedPFCandidates'),
    jetSource    = cms.InputTag('ak4PFJets'),
    pvSource     = cms.InputTag('unpackedTracksAndVertices'), 
    jetCorrections     = ('AK4PFchs', cms.vstring(['L1FastJet', 'L2Relative', 'L3Absolute']), 'None'),
    btagDiscriminators = [      'pfJetProbabilityBJetTags'     ] ,algo= 'AK', rParam = 0.4
  )
  #
  ## adjust MC matching
  process.patJetGenJetMatchAK4PF.matched = "ak4GenJets"
  process.patJetPartonMatchAK4PF.matched = "prunedGenParticles"
  process.patJetCorrFactorsAK4PF.primaryVertices = "offlineSlimmedPrimaryVertices"
  process.patJetPartons.particles        = "prunedGenParticles"

def addFlashggPFCHS0(process):
  print "JET PRODUCER :: Flashgg PFCHS0 producer +++++ "
  # load various necessary plugins.
  process.load("PhysicsTools.PatAlgos.producersLayer1.patCandidates_cff")
  process.load('PhysicsTools.PatAlgos.slimming.unpackedTracksAndVertices_cfi')
  # leptons to remove as per default CHS workflow
  process.selectedMuons = cms.EDFilter("CandPtrSelector", 
                                       src = cms.InputTag("slimmedMuons"), 
                                       cut = cms.string('''abs(eta)<2.5 && pt>10. &&
                                       (pfIsolationR04().sumChargedHadronPt+
                                       max(0.,pfIsolationR04().sumNeutralHadronEt+
                                       pfIsolationR04().sumPhotonEt-
                                       0.50*pfIsolationR04().sumPUPt))/pt < 0.20 && 
                                       (isPFMuon && (isGlobalMuon || isTrackerMuon) )'''))
  process.selectedElectrons = cms.EDFilter("CandPtrSelector", 
                                           src = cms.InputTag("slimmedElectrons"), 
                                           cut = cms.string('''abs(eta)<2.5 && pt>20. &&
                                           gsfTrack.isAvailable() &&
                                           gsfTrack.hitPattern().numberOfLostHits(\'MISSING_INNER_HITS\') < 2 &&
                                           (pfIsolationVariables().sumChargedHadronPt+
                                           max(0.,pfIsolationVariables().sumNeutralHadronEt+
                                           pfIsolationVariables().sumPhotonEt-
                                           0.5*pfIsolationVariables().sumPUPt))/pt < 0.15'''))

  # Simple producer which just removes the Candidates which
  # don't come from the legacy vertex according to the Flashgg Vertex Map
  process.pfCHS0       = cms.EDFilter("CandPtrSelector",
                                      src = cms.InputTag("packedPFCandidates"),
                                      cut = cms.string("fromPV"))
  
  process.pfNoMuonCHS0 = cms.EDProducer("CandPtrProjector",
                                        src = cms.InputTag("pfCHS0"),
                                        veto = cms.InputTag("selectedMuons"))
  
  process.pfNoElectronsCHS0 = cms.EDProducer("CandPtrProjector",
                                             src = cms.InputTag("pfNoMuonCHS0"),
                                             veto =  cms.InputTag("selectedElectrons"))

  #Import RECO jet producer for ak4 PF and GEN jet
  from RecoJets.JetProducers.ak4PFJets_cfi  import ak4PFJets
  from RecoJets.JetProducers.ak4GenJets_cfi import ak4GenJets
  process.ak4PFJetsCHS0  = ak4PFJets.clone ( src = 'pfNoElectronsCHS0', doAreaFastjet = True)
  process.ak4GenJetsCHS0 = ak4GenJets.clone( src = 'packedGenParticles')
  
  # cluster the jets
  addJetCollection(
    process,
    postfix   = "",
    labelName = 'AK4PFCHS0',
    pfCandidates = cms.InputTag('packedPFCandidates'),
    jetSource = cms.InputTag('ak4PFJetsCHS0'),
    #trackSource = cms.InputTag('unpackedTracksAndVertices'), 
    pvSource = cms.InputTag('unpackedTracksAndVertices'), 
    jetCorrections = ('AK4PFchs', cms.vstring(['L1FastJet', 'L2Relative', 'L3Absolute']), 'None'),
    #btagDiscriminators = [      'combinedSecondaryVertexBJetTags'     ]
    btagDiscriminators = [      'pfJetProbabilityBJetTags'     ]
    ,algo= 'AK', rParam = 0.4
  )
  # adjust MC matching
  process.patJetGenJetMatchAK4PFCHS0.matched = "ak4GenJetsLeg"
  process.patJetPartonMatchAK4PFCHS0.matched = "prunedGenParticles"
  process.patJetPartons.particles            = "prunedGenParticles"
  # adjust PV collection used for Jet Corrections
  process.patJetCorrFactorsAK4PFCHS0.primaryVertices = "offlineSlimmedPrimaryVertices"
  






  
flashggJetsPF     = cms.EDProducer('FlashggJetProducer',
                                   DiPhotonTag=cms.untracked.InputTag('flashggDiPhotons'),
                                   VertexTag=cms.untracked.InputTag('offlineSlimmedPrimaryVertices'),
                                   JetTag=cms.untracked.InputTag('patJetsAK4PF'),
                                   VertexCandidateMapTag = cms.InputTag("flashggVertexMapForCHS"),
                                   PileupJetIdParameters=cms.PSet(pu_jetid),
                                   MinJetPt=cms.untracked.double(0.)             
                                 )

flashggJetsPFCHS0 = cms.EDProducer('FlashggJetProducer',
                                   DiPhotonTag=cms.untracked.InputTag('flashggDiPhotons'),
                                   VertexTag=cms.untracked.InputTag('offlineSlimmedPrimaryVertices'),
                                   JetTag=cms.untracked.InputTag('patJetsAK4PFCHS0'),
                                   VertexCandidateMapTag = cms.InputTag("flashggVertexMapForCHS"),
                                   PileupJetIdParameters=cms.PSet(pu_jetid),
                                   MinJetPt=cms.untracked.double(0.)             
                                 )

