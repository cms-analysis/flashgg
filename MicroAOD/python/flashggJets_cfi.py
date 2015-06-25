# this is based on the jet 74X recipe for jet re-clustering
# twiki source : https://twiki.cern.ch/twiki/bin/view/CMSPublic/WorkBookMiniAOD2015#Advanced_topics_re_clustering_ev

import FWCore.ParameterSet.Config as cms
from RecoJets.JetProducers.PileupJetIDParams_cfi import cutbased as pu_jetid
from PhysicsTools.PatAlgos.tools.jetTools import addJetCollection

flashggBTag = 'pfCombinedInclusiveSecondaryVertexV2BJetTags'

def addFlashggPFCHSLegJets(process):

  # NOTE : these lines are not longer needed
  # load various necessary plugins.
  ## process.load("PhysicsTools.PatAlgos.producersLayer1.patCandidates_cff")
  ## process.load('PhysicsTools.PatAlgos.slimming.unpackedTracksAndVertices_cfi')
  
  # leptons to remove as per default CHS workflow
  # select the isolated leptons : electrons + muons
  
  process.selectedMuons     = cms.EDFilter("CandPtrSelector", 
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
                                                            VertexTag = cms.untracked.InputTag('offlineSlimmedPrimaryVertices')
                                                          )
  
  process.pfCHSLeg = cms.EDFilter("CandPtrSelector", 
                                  src = cms.InputTag("flashggCHSLegacyVertexCandidates"), 
                                  cut = cms.string(""))
  
  # then remove the previously selected muons
  process.pfNoMuonCHSLeg      = cms.EDProducer("CandPtrProjector", 
                                               src  = cms.InputTag("pfCHSLeg"), 
                                               veto = cms.InputTag("selectedMuons"))
  # then remove the previously selected electrons
  process.pfNoElectronsCHSLeg = cms.EDProducer("CandPtrProjector", 
                                               src  = cms.InputTag("pfNoMuonCHSLeg"), 
                                               veto = cms.InputTag("selectedElectrons"))
  
  #Import RECO jet producer for ak4 PF and GEN jet
  from RecoJets.JetProducers.ak4PFJets_cfi  import ak4PFJets
  #from RecoJets.JetProducers.ak4GenJets_cfi import ak4GenJets
  process.ak4PFJetsCHSLeg = ak4PFJets.clone ( src = 'pfNoElectronsCHSLeg', doAreaFastjet = True)
  #process.ak4GenJetsLeg   = ak4GenJets.clone( src = 'packedGenParticles')
  
  # NOTE : these line are from the new Jet recipe 
  # The following is make patJets, but EI is done with the above
  process.load("Configuration.StandardSequences.MagneticField_cff")
  process.load("Configuration.Geometry.GeometryRecoDB_cff")
  process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
  
  # cluster the jets
  # NOTE: this is the 74X recipe for the jet clustering 
  addJetCollection(
    process,
    labelName      = 'AK4PFCHSLeg',
    jetSource      = cms.InputTag('ak4PFJetsCHSLeg'),
    pvSource       = cms.InputTag('offlineSlimmedPrimaryVertices'),
    pfCandidates   = cms.InputTag('packedPFCandidates'),
    svSource       = cms.InputTag('slimmedSecondaryVertices'),
    btagDiscriminators = [ flashggBTag ],
    jetCorrections = ('AK4PFchs', ['L1FastJet', 'L2Relative', 'L3Absolute'], 'None'),
    
    #genJetCollection = cms.InputTag('ak4GenJetsLeg'),
    genJetCollection = cms.InputTag('slimmedGenJets'),
    genParticles     = cms.InputTag('prunedGenParticles'),
    # jet param
    algo = 'AK', rParam = 0.4
  )
  
  #adjust PV used for Jet Corrections
  process.patJetCorrFactorsAK4PFCHSLeg.primaryVertices = "offlineSlimmedPrimaryVertices"
  
  # adjust MC matching
  # process.patJetGenJetMatchAK4PFCHSLeg.matched = "ak4GenJetsLeg"
  # process.patJetPartonMatchAK4PFCHSLeg.matched = "prunedGenParticles"
  # process.patJetPartons.particles = "prunedGenParticles"
    
  
# Flashgg Jet producer using the collection created with function above.
flashggJets = cms.EDProducer('FlashggJetProducer',
                             DiPhotonTag = cms.untracked.InputTag('flashggDiPhotons'),
                             VertexTag   = cms.untracked.InputTag('offlineSlimmedPrimaryVertices'),
                             JetTag      = cms.untracked.InputTag('patJetsAK4PFCHSLeg'),
                             VertexCandidateMapTag = cms.InputTag("flashggVertexMapForCHS"),
                             PileupJetIdParameters = cms.PSet(pu_jetid),
                             MinJetPt=cms.untracked.double(0.)             
)
  


def addQGTaggerPFCHSLeg(process):
  process.load('RecoJets.JetProducers.QGTagger_cfi')

  process.QGTaggerPFCHSLeg = process.QGTagger.clone( srcJets   = 'flashggJets' ,
                                                     jetsLabel = 'QGL_AK4PFchs')
  
  process.QGTaggerPFCHSLeg.jec              = cms.InputTag('')# keept empty, because are already corrected
  process.QGTaggerPFCHSLeg.systematicsLabel = cms.string('')# Produce systematic smearings (not yet available, keep empty)
  
