# y. haddad Imperial College
# this is based on the jet 74X recipe for jet re-clustering
# twiki source : https://twiki.cern.ch/twiki/bin/view/CMSPublic/WorkBookMiniAOD2015#Advanced_topics_re_clustering_ev

import FWCore.ParameterSet.Config as cms
from RecoJets.JetProducers.PileupJetIDParams_cfi import cutbased as pu_jetid
from PhysicsTools.PatAlgos.tools.jetTools import addJetCollection
from flashgg.MicroAOD.flashggJets_cfi import flashggBTag

def addFlashggPF(process):
  print "JET PRODUCER :: Flashgg PF producer ::"
  from RecoJets.JetProducers.ak4PFJets_cfi  import ak4PFJets
  
  process.ak4PFJets  = ak4PFJets.clone (src = 'packedPFCandidates', doAreaFastjet = True)

  ## cluster the jets
  addJetCollection(
    process,
    labelName      = 'AK4PF',
    jetSource      = cms.InputTag('ak4PFJets'),
    pvSource       = cms.InputTag('offlineSlimmedPrimaryVertices'),
    pfCandidates   = cms.InputTag('packedPFCandidates'),
    svSource       = cms.InputTag('slimmedSecondaryVertices'),
    btagDiscriminators = [ flashggBTag ],
    jetCorrections = ('AK4PFchs', ['L1FastJet', 'L2Relative', 'L3Absolute'], 'None'),
    genJetCollection = cms.InputTag('slimmedGenJets'),
    genParticles     = cms.InputTag('prunedGenParticles'),
    algo = 'AK', rParam = 0.4
  )
  
  #adjust PV used for Jet Corrections
  process.patJetCorrFactorsAK4PF.primaryVertices = "offlineSlimmedPrimaryVertices"
  
  
  



  
def addFlashggPFCHS0(process):
  print "JET PRODUCER :: Flashgg PFCHS producer ::"
  
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
  
  process.pfCHS0 = cms.EDFilter("CandPtrSelector", 
                                src = cms.InputTag("flashggCHSLegacyVertexCandidates"), 
                                cut = cms.string(""))
  
  # then remove the previously selected muons
  process.pfNoMuonCHS0      = cms.EDProducer("CandPtrProjector", 
                                             src  = cms.InputTag("pfCHS0"), 
                                             veto = cms.InputTag("selectedMuons"))
  # then remove the previously selected electrons
  process.pfNoElectronsCHS0 = cms.EDProducer("CandPtrProjector", 
                                             src  = cms.InputTag("pfNoMuonCHS0"), 
                                             veto = cms.InputTag("selectedElectrons"))
  
  #Import RECO jet producer for ak4 PF and GEN jet
  from RecoJets.JetProducers.ak4PFJets_cfi  import ak4PFJets
  process.ak4PFJetsCHS0 = ak4PFJets.clone ( src = 'pfNoElectronsCHS0', doAreaFastjet = True)
  
  # NOTE : these line are from the new Jet recipe 
  # The following is make patJets, but EI is done with the above
  process.load("Configuration.StandardSequences.MagneticField_cff")
  process.load("Configuration.Geometry.GeometryRecoDB_cff")
  process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
  
  
  
  # cluster the jets
  # NOTE: this is the 74X recipe for the jet clustering 
  addJetCollection(
    process,
    labelName      = 'AK4PFCHS0',
    jetSource      = cms.InputTag('ak4PFJetsCHS0'),
    pvSource       = cms.InputTag('offlineSlimmedPrimaryVertices'),
    pfCandidates   = cms.InputTag('packedPFCandidates'),
    svSource       = cms.InputTag('slimmedSecondaryVertices'),
    btagDiscriminators =  [ flashggBTag ],
    jetCorrections = ('AK4PFchs', ['L1FastJet', 'L2Relative', 'L3Absolute'], 'None'),
    
    genParticles     = cms.InputTag('prunedGenParticles'),
    genJetCollection = cms.InputTag('slimmedGenJets'),
    # jet param
    algo = 'AK', rParam = 0.4
    )
  
  #adjust PV used for Jet Corrections
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



def addQGTaggerPF(process):
  process.load('RecoJets.JetProducers.QGTagger_cfi')
  process.QGTaggerPF     =  process.QGTagger.clone( srcJets   = 'flashggJets' ,
                                                               jetsLabel = 'QGL_AK4PFchs')
  
  process.QGTaggerPF.jec              = cms.InputTag('')# keept empty, because are already corrected
  process.QGTaggerPF.systematicsLabel = cms.string('')# Produce systematic smearings (not yet available, keep empty)
  
def addQGTaggerPFCHS0(process):
  process.load('RecoJets.JetProducers.QGTagger_cfi')
  process.QGTaggerPFCHS0 =  process.QGTagger.clone( srcJets   = 'flashggJets' ,
                                                               jetsLabel = 'QGL_AK4PFchs')
  
  process.QGTaggerPFCHS0.jec              = cms.InputTag('')# keept empty, because are already corrected
  process.QGTaggerPFCHS0.systematicsLabel = cms.string('')# Produce systematic smearings (not yet available, keep empty)
