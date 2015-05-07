import FWCore.ParameterSet.Config as cms

from RecoJets.JetProducers.PileupJetIDParams_cfi import cutbased as pu_jetid
from PhysicsTools.PatAlgos.tools.jetTools import addJetCollection
#from Configuration.Geometry.GeometryAll_cff import *

# define a function to add in the jet collection, as the reclustering need to know about the process
# but we obviously don't want all this stuff clogging up python configs. 
def addFlashggPFCHSLegJets(process):
  #process.load("Configuration.StandardSequences.Geometry_cff")
  #process.myPrefer = cms.ESPrefer("CaloSubdetectorGeometry" [,"ZDC"
  #                   [,ZdcGeometryFromDBEP = cms.vstring("<C++ data type>[/<data label>]"[,...])]
  
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
  
  process.flashggCHSLegacyVertexCandidates = cms.EDProducer('FlashggCHSLegacyVertexCandidateProducer',
                                                            PFCandidatesTag       = cms.untracked.InputTag('packedPFCandidates'),
                                                            DiPhotonTag           = cms.untracked.InputTag('flashggDiPhotons'),
                                                            VertexCandidateMapTag = cms.InputTag("flashggVertexMapForCHS"),
                                                            VertexTag=cms.untracked.InputTag('offlineSlimmedPrimaryVertices')
                                                          )
  process.pfCHSLeg = cms.EDFilter("CandPtrSelector", 
                                  src = cms.InputTag("flashggCHSLegacyVertexCandidates"), 
                                  cut = cms.string(""))
  # then remove the previously selected muons
  process.pfNoMuonCHSLeg =  cms.EDProducer("CandPtrProjector", 
                                           src  = cms.InputTag("pfCHSLeg"), 
                                           veto = cms.InputTag("selectedMuons"))
  # then remove the previously selected electrons
  process.pfNoElectronsCHSLeg = cms.EDProducer("CandPtrProjector", 
                                               src  = cms.InputTag("pfNoMuonCHSLeg"), 
                                               veto = cms.InputTag("selectedElectrons"))
  #Import RECO jet producer for ak4 PF and GEN jet
  from RecoJets.JetProducers.ak4PFJets_cfi  import ak4PFJets
  from RecoJets.JetProducers.ak4GenJets_cfi import ak4GenJets
  process.ak4PFJetsCHSLeg = ak4PFJets.clone ( src = 'pfNoElectronsCHSLeg', doAreaFastjet = True)
  process.ak4GenJetsLeg   = ak4GenJets.clone( src = 'packedGenParticles')
  
  # cluster the jets
  addJetCollection(
    process,
    postfix   = "",
    labelName = 'AK4PFCHSLeg',
    pfCandidates = cms.InputTag('packedPFCandidates'),
    jetSource = cms.InputTag('ak4PFJetsCHSLeg'),
    #trackSource = cms.InputTag('unpackedTracksAndVertices'), 
    pvSource = cms.InputTag('unpackedTracksAndVertices'), 
    jetCorrections = ('AK4PFchs', cms.vstring(['L1FastJet', 'L2Relative', 'L3Absolute']), 'None'),
    #btagDiscriminators = [      'combinedSecondaryVertexBJetTags'     ]
    btagDiscriminators = [      'pfJetProbabilityBJetTags'     ]
    ,algo= 'AK', rParam = 0.4
  )
  # adjust MC matching
  process.patJetGenJetMatchAK4PFCHSLeg.matched = "ak4GenJetsLeg"
  process.patJetPartonMatchAK4PFCHSLeg.matched = "prunedGenParticles"
  process.patJetPartons.particles = "prunedGenParticles"
  # adjust PV collection used for Jet Corrections
  process.patJetCorrFactorsAK4PFCHSLeg.primaryVertices = "offlineSlimmedPrimaryVertices"


# Flashgg Jet producer using the collection created with function above.
flashggJets = cms.EDProducer('FlashggJetProducer',
                             DiPhotonTag=cms.untracked.InputTag('flashggDiPhotons'),
                             VertexTag=cms.untracked.InputTag('offlineSlimmedPrimaryVertices'),
                             JetTag=cms.untracked.InputTag('patJetsAK4PFCHSLeg'),
                             VertexCandidateMapTag = cms.InputTag("flashggVertexMapForCHS"),
                             PileupJetIdParameters=cms.PSet(pu_jetid),
                             MinJetPt=cms.untracked.double(0.)             
)
  

