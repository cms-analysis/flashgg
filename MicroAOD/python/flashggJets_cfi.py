# this is based on the jet 74X recipe for jet re-clustering
# twiki source : https://twiki.cern.ch/twiki/bin/view/CMSPublic/WorkBookMiniAOD2015#Advanced_topics_re_clustering_ev

import FWCore.ParameterSet.Config as cms
from RecoJets.JetProducers.PileupJetIDParams_cfi import cutbased_new as pu_jetid
from PhysicsTools.PatAlgos.tools.jetTools import addJetCollection

flashggBTag = 'pfCombinedInclusiveSecondaryVertexV2BJetTags'


def addFlashggPFCHSLegJets(process, vertexIndex_ = 1, doQGTagging = True, label ='', postfix='postfix'):
  postfix += label 
  
  setattr(process, 'selectedMuons' + postfix, cms.EDFilter("CandPtrSelector", 
                                                           src = cms.InputTag("slimmedMuons"), 
                                                           cut = cms.string('''abs(eta)<2.5 && pt>10. &&
                                                           (pfIsolationR04().sumChargedHadronPt+
                                                           max(0.,pfIsolationR04().sumNeutralHadronEt+
                                                           pfIsolationR04().sumPhotonEt-
                                                           0.50*pfIsolationR04().sumPUPt))/pt < 0.20 && 
                                                           (isPFMuon && (isGlobalMuon || isTrackerMuon) )''')))
  
  setattr(process, 'selectedElectrons' + postfix, cms.EDFilter("CandPtrSelector", 
                                                               src = cms.InputTag("slimmedElectrons"), 
                                                               cut = cms.string('''abs(eta)<2.5 && pt>20. &&
                                                               gsfTrack.isAvailable() &&
                                                               gsfTrack.hitPattern().numberOfLostHits(\'MISSING_INNER_HITS\') < 2 &&
                                                               (pfIsolationVariables().sumChargedHadronPt+
                                                               max(0.,pfIsolationVariables().sumNeutralHadronEt+
                                                               pfIsolationVariables().sumPhotonEt-
                                                               0.5*pfIsolationVariables().sumPUPt))/pt < 0.15''')))
  
  # Simple producer which just removes the Candidates which
  # don't come from the legacy vertex according to the Flashgg Vertex Map
  setattr(process,'flashggCHSLegacyVertexCandidates' + postfix,
          cms.EDProducer('FlashggMultiCHSLegacyVertexCandidateProducer',
                         PFCandidatesTag       = cms.InputTag('packedPFCandidates'),
                         DiPhotonTag           = cms.InputTag('flashggDiPhotons'),
                         VertexCandidateMapTag = cms.InputTag("flashggVertexMapForCHS"),
                         VertexTag             = cms.InputTag('offlineSlimmedPrimaryVertices'),
                         vertexIndex           = cms.untracked.int(vertexIndex_)
                       )
  )
  setattr(process, 'pfCHSLeg' + postfix, cms.EDFilter("CandPtrSelector", 
                                                      src = cms.InputTag('flashggCHSLegacyVertexCandidates' + postfix), 
                                                      cut = cms.string('')))
  
  # then remove the previously selected muons
  setattr(process, 'pfNoMuonCHSLeg' + postfix,  cms.EDProducer("CandPtrProjector", 
                                                               src  = cms.InputTag("pfCHSLeg" + postfix), 
                                                               veto = cms.InputTag("selectedMuons" + postfix)))
  # then remove the previously selected electrons
  setattr(process, 'pfNoElectronsCHSLeg' + postfix,  cms.EDProducer("CandPtrProjector", 
                                                                    src  = cms.InputTag("pfNoMuonCHSLeg" + postfix), 
                                                                    veto = cms.InputTag("selectedElectrons" + postfix)))
  
  #Import RECO jet producer for ak4 PF and GEN jet
  from RecoJets.JetProducers.ak4PFJets_cfi  import ak4PFJets
  #from RecoJets.JetProducers.ak4GenJets_cfi import ak4GenJets
  setattr(process, 'ak4PFJetsCHSLeg' + postfix, ak4PFJets.clone ( src = 'pfNoElectronsCHSLeg' + postfix, doAreaFastjet = True))
  #process.ak4GenJetsLeg   = ak4GenJets.clone( src = 'packedGenParticles')
  
  # NOTE: this is the 74X recipe for the jet clustering
  addJetCollection(
    process,
    postfix        = postfix,
    labelName      = 'AK4PFCHSLeg',
    jetSource      = cms.InputTag('ak4PFJetsCHSLeg' + postfix),
    pvSource       = cms.InputTag('offlineSlimmedPrimaryVertices'),
    pfCandidates   = cms.InputTag('packedPFCandidates'),
    svSource       = cms.InputTag('slimmedSecondaryVertices'),
    btagDiscriminators = [ flashggBTag ],
    jetCorrections = ('AK4PFchs', ['L1FastJet', 'L2Relative', 'L3Absolute'], 'None'),
    genJetCollection = cms.InputTag('slimmedGenJets'),
    genParticles     = cms.InputTag('prunedGenParticles'),
    # jet param
    algo = 'AK', rParam = 0.4
  )
  
  #adjust PV used for Jet Corrections
  #process.patJetCorrFactorsAK4PFCHSLeg.primaryVertices = "offlineSlimmedPrimaryVertices"
  getattr(process, 'patJetCorrFactorsAK4PFCHSLeg' + postfix).primaryVertices = "offlineSlimmedPrimaryVertices"
  # Flashgg Jet producer using the collection created with function above.
  flashggJets = cms.EDProducer('FlashggJetProducer',
                               DiPhotonTag = cms.InputTag('flashggDiPhotons'),
                               VertexTag   = cms.InputTag('offlineSlimmedPrimaryVertices'),
                               JetTag      = cms.InputTag('patJetsAK4PFCHSLeg' + postfix),
                               VertexCandidateMapTag = cms.InputTag("flashggVertexMapForCHS"),
                               PileupJetIdParameters = cms.PSet(pu_jetid)
  )
  setattr( process, 'flashggJets' + postfix, flashggJets)
  #setattr( process, 'selectedFlashggPFCHSLegJet' + postfix, cms.EDFilter("FLASHggJetSelector",
  #                                                                       src = cms.InputTag("flashggJets" +  postfix),
  #                                                                       cut = cms.string("pt > 15.")))  
  
  if doQGTagging:
    from RecoJets.JetProducers.QGTagger_cfi import QGTagger
    setattr( process, 'QGTaggerPFCHSLeg' + postfix,  QGTagger.clone( srcJets   = 'flashggJets' ,jetsLabel = 'ak4PFJetsCHS'))
    
  return getattr( process, 'flashggJets' + postfix)



