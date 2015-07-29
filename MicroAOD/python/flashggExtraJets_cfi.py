# y. haddad Imperial College
# this is based on the jet 74X recipe for jet re-clustering
# twiki source : https://twiki.cern.ch/twiki/bin/view/CMSPublic/WorkBookMiniAOD2015#Advanced_topics_re_clustering_ev

import FWCore.ParameterSet.Config as cms

from RecoJets.JetProducers.PileupJetIDParams_cfi import cutbased_new as pu_jetid
from PhysicsTools.PatAlgos.tools.jetTools        import addJetCollection
from flashgg.MicroAOD.flashggJets_cfi            import flashggBTag

def addFlashggPF(process, doQGTagging =  True, label ='', debug = False):
  
  from RecoJets.JetProducers.ak4PFJets_cfi  import ak4PFJets
  # process.ak4PFJets  = ak4PFJets.clone (src = 'packedPFCandidates', doAreaFastjet = True)
  setattr(process, 'ak4PFJets' + label, ak4PFJets.clone (src = 'packedPFCandidates', doAreaFastjet = True))
  
  # cluster the jets
  addJetCollection(
    process,
    postfix            = label,
    labelName          = 'AK4PF' ,
    jetSource          = cms.InputTag('ak4PFJets'+label),
    pvSource           = cms.InputTag('offlineSlimmedPrimaryVertices'),
    pfCandidates       = cms.InputTag('packedPFCandidates'),
    svSource           = cms.InputTag('slimmedSecondaryVertices'),
    btagDiscriminators = [ flashggBTag ],
    jetCorrections     = ('AK4PFchs', ['L1FastJet', 'L2Relative', 'L3Absolute'], 'None'),
    genJetCollection   = cms.InputTag('slimmedGenJets'),
    genParticles       = cms.InputTag('prunedGenParticles'),
    algo = 'AK', rParam = 0.4
  )

  # adjust PV used for Jet Corrections
  # getattr(process, 'patJetCorrFactors'+_labelName+label)
  getattr(process, 'patJetCorrFactorsAK4PF' + label).primaryVertices = "offlineSlimmedPrimaryVertices"
  #process.patJetCorrFactorsAK4PF.primaryVertices = "offlineSlimmedPrimaryVertices"
  print ' --> patJetCorrFactorsAK4PF == ',  getattr(process, 'patJetCorrFactorsAK4PF' + label)
  print ' --> patJetsAK4PF           == ',  getattr(process, 'patJetsAK4PF' + label)
  flashggJetsPF = cms.EDProducer('FlashggJetProducer',
                                 DiPhotonTag=cms.InputTag('flashggDiPhotons'),
                                 VertexTag=cms.InputTag('offlineSlimmedPrimaryVertices'),
                                 JetTag=cms.InputTag('patJetsAK4PF' + label ),
                                 VertexCandidateMapTag = cms.InputTag("flashggVertexMapForCHS"),
                                 PileupJetIdParameters=cms.PSet(pu_jetid),
  )
  setattr(process, 'flashggJetsPF' + label, flashggJetsPF)
  if doQGTagging:
    from RecoJets.JetProducers.QGTagger_cfi import QGTagger
    setattr(process, 'QGTaggerPF' + label, QGTagger.clone( srcJets   = 'flashggJetsPF' + label ,jetsLabel = 'QGL_AK4PF'))
    
  return getattr(process, 'flashggJetsPF' + label)  


