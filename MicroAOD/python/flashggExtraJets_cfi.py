# y. haddad Imperial College
# this is based on the jet 74X recipe for jet re-clustering
# twiki source : https://twiki.cern.ch/twiki/bin/view/CMSPublic/WorkBookMiniAOD2015#Advanced_topics_re_clustering_ev

import FWCore.ParameterSet.Config as cms

#from RecoJets.JetProducers.PileupJetIDParams_cfi import cutbased_new as pu_jetid
from PhysicsTools.PatAlgos.tools.jetTools        import addJetCollection
from flashgg.MicroAOD.flashggJets_cfi            import flashggBTag
from flashgg.MicroAOD.flashggJets_cfi            import flashggCMVABTag
from flashgg.MicroAOD.flashggJets_cfi            import flashggDeepCSVb
from flashgg.MicroAOD.flashggJets_cfi            import flashggDeepCSVbb
from flashgg.MicroAOD.flashggJets_cfi            import flashggDeepCSVc
from flashgg.MicroAOD.flashggJets_cfi            import flashggDeepCSVudsg

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
    elSource       = cms.InputTag("slimmedElectrons"),
    muSource       = cms.InputTag("slimmedMuons"),
    runIVF         = True,
    btagDiscriminators = [ flashggBTag, flashggCMVABTag , flashggDeepCSVb, flashggDeepCSVbb, flashggDeepCSVc, flashggDeepCSVudsg, ],
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
#                                 PileupJetIdParameters=cms.PSet(pu_jetid),
  )
  setattr(process, 'flashggJetsPF' + label, flashggJetsPF)
  if doQGTagging:
    from RecoJets.JetProducers.QGTagger_cfi import QGTagger
    setattr(process, 'QGTaggerPF' + label, QGTagger.clone( srcJets   = 'flashggJetsPF' + label ,jetsLabel = 'QGL_AK4PF'))
    
  return getattr(process, 'flashggJetsPF' + label)  


def addStandardPuppiJets(process,
                         label       ='',
                         useLocalJEC = True,
                         dbfile      = 'flashgg/MetaData/data/PuppiJEC/PY8_RunIISpring15DR74_bx50_MC.db',
                         debug       = False):

  print  ':: Running a standard PUPPI : --'
  
  from CommonTools.PileupAlgos.Puppi_cff           import puppi 
  from RecoJets.JetProducers.ak4PFJets_cfi         import ak4PFJets
  from PhysicsTools.PatAlgos.tools.jetTools        import addJetCollection
#  from RecoJets.JetProducers.PileupJetIDParams_cfi import cutbased_new as pu_jetid
  from flashgg.MicroAOD.flashggJets_cfi            import flashggBTag
  
  setattr(process, 'flashggStdPuppi' + label,
          puppi.clone( candName    = cms.InputTag('packedPFCandidates'),
                       vertexName  = cms.InputTag('offlineSlimmedPrimaryVertices'),
                     ))
  
  setattr(process, 'ak4PFJetsStdPuppi' + label,
          ak4PFJets.clone ( src = cms.InputTag('flashggStdPuppi'),
                            doAreaFastjet = True
                          ))

  if useLocalJEC :
    print ':: using a local JEC dbfile for PUPPI :',
    print ' -- ',  dbfile
    
    from flashgg.MicroAOD.flashggJetTools_cfi import loadLocalJECDBfile
    loadLocalJECDBfile(process,
                       dbfile = os.environ['CMSSW_BASE'] + '/src/' + dbfile,
                       tag    = 'JetCorrectorParametersCollection_PY8_RunIISpring15DR74_bx50_MC_AK4PUPPI',
                       label  = 'AK4PFPuppi')
    
    
  addJetCollection(
    process,
    postfix            = 'std' + label,
    labelName          = 'AK4PUPPI',
    jetSource          = cms.InputTag('ak4PFJetsStdPuppi'),
    pvSource           = cms.InputTag('offlineSlimmedPrimaryVertices'),
    pfCandidates       = cms.InputTag('packedPFCandidates'),
    svSource           = cms.InputTag('slimmedSecondaryVertices'),
    elSource       = cms.InputTag("slimmedElectrons"),
    muSource       = cms.InputTag("slimmedMuons"),
    runIVF         = True,
    btagDiscriminators = [ flashggBTag, flashggCMVABTag , flashggDeepCSVb, flashggDeepCSVbb, flashggDeepCSVc, flashggDeepCSVudsg, ],
    jetCorrections     = ('AK4PFchs',['L1FastJet',  'L2Relative', 'L3Absolute'], 'None'),
    genJetCollection   = cms.InputTag('slimmedGenJets'),
    genParticles       = cms.InputTag('prunedGenParticles'),
    # jet param
    algo = 'AK', rParam = 0.4
  )
  
  getattr(process, 'patJetCorrFactorsAK4PUPPIstd' + label).primaryVertices = "offlineSlimmedPrimaryVertices"
  setattr(process, 'flashggStdPUPPIJets',
          cms.EDProducer('FlashggJetProducer',
                         DiPhotonTag           = cms.InputTag('flashggDiPhotons'),
                         VertexTag             = cms.InputTag('offlineSlimmedPrimaryVertices'),
                         JetTag                = cms.InputTag('patJetsAK4PUPPIstd' + label),
                         VertexCandidateMapTag = cms.InputTag("flashggVertexMapForPUPPI"),
                         UsePuppi              = cms.untracked.bool(True),
#                         PileupJetIdParameters = cms.PSet(pu_jetid))
                         )
          )
  setattr(process, 'selectedFlashggStdPUPPIJets' + label,
          cms.EDFilter("FLASHggJetSelector",
                       src = cms.InputTag( 'flashggStdPUPPIJets'),
                       cut = cms.string("pt > 15."))
          )


  
StandardPUPIJetVInputTag = cms.VInputTag()
StandardPUPIJetVInputTag.append(cms.InputTag('selectedFlashggStdPUPPIJets'))

PFJetVInputTag = cms.VInputTag()
PFJetVInputTag.append(cms.InputTag('flashggJetsPF'))

  
#-----------------------------------------------------------------------------------------------    
