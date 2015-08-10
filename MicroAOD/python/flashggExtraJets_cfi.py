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






def addStandardPuppiJets(process,
                         label ='',
                         debug = False):

  print ':: standard puppi ... '
  from CommonTools.PileupAlgos.Puppi_cff      import puppi
  from RecoJets.JetProducers.ak4PFJets_cfi    import ak4PFJets
  # fill the puppi parameters
  setattr(process, 'Puppi' + label,
          puppi.clone( candName              = cms.InputTag('packedPFCandidates'),
                       vertexName            = cms.InputTag('offlineSlimmedPrimaryVertices')
                     )
        )
  setattr ( process, 'ak4PFJetsPuppi' + label,
            ak4PFJets.clone ( src = cms.InputTag('Puppi' + label), doAreaFastjet = True)
          )

  #===================================================
  # local BD reader
  # def addPoolDBESSource(process,moduleName,record,tag,label='',connect='sqlite_file:'):
  import os
  dbfile  = os.environ['CMSSW_BASE'] + '/src/flashgg/MetaData/data/PuppiJEC/PY8_RunIISpring15DR74_bx50_MC.db'
  print ':: dbfile == ', dbfile
  
  process.load("CondCore.DBCommon.CondDBCommon_cfi")
  from CondCore.DBCommon.CondDBSetup_cfi import *
  process.jec = cms.ESSource("PoolDBESSource",
                     DBParameters = cms.PSet(
                       messageLevel = cms.untracked.int32(0)
                     ),
                     timetype = cms.string('runnumber'),
                     toGet = cms.VPSet(cms.PSet(
                       record = cms.string('JetCorrectionsRecord'),
                       tag    = cms.string('JetCorrectorParametersCollection_PY8_RunIISpring15DR74_bx50_MC_AK4PUPPI'),
                       #tag    = cms.string('JetCorrectorParametersCollection_Summer12_V3_MC_AK5PF'),
                       label  = cms.untracked.string('AK4PFPuppi')
                     )),
                     connect = cms.string('sqlite_file:%s' % dbfile)
  )
  #authenticationMethod = cms.untracked.uint32(0))
  #if authPath: calibDB.DBParameters.authenticationPath = authPath
  #if connect.find('oracle:') != -1: calibDB.DBParameters.authenticationPath = '/afs/cern.ch/cms/DB/conddb'
  process.es_prefer_jec  = cms.ESPrefer('PoolDBESSource','jec')
    
  
  #===================================================
  # do jet clustering
  addJetCollection(
    process,
    postfix            = label,
    labelName          = 'AK4PUPPI',
    jetSource          = cms.InputTag('ak4PFJetsPuppi' + label),
    pvSource           = cms.InputTag('offlineSlimmedPrimaryVertices'),
    pfCandidates       = cms.InputTag('packedPFCandidates'),
    svSource           = cms.InputTag('slimmedSecondaryVertices'),
    btagDiscriminators = [ flashggBTag ],
    jetCorrections     = ('AK4PFPuppi',['L1FastJet',  'L2Relative', 'L3Absolute'], 'None'),
    genJetCollection   = cms.InputTag('slimmedGenJets'),
    genParticles       = cms.InputTag('prunedGenParticles'),
    # jet param
    algo = 'AK', rParam = 0.4
  )
  
  getattr(process, 'patJetCorrFactorsAK4PUPPI' + label).primaryVertices = "offlineSlimmedPrimaryVertices"
  setattr( process,'flashghSTDPUPPIJets'+ label,
           cms.EDProducer('FlashggJetProducer',
                          DiPhotonTag           = cms.InputTag('flashggDiPhotons'),
                          VertexTag             = cms.InputTag('offlineSlimmedPrimaryVertices'),
                          JetTag                = cms.InputTag('patJetsAK4PUPPI' + label),
                          VertexCandidateMapTag = cms.InputTag("flashggVertexMapForCHS"),
                          UsePuppi              = cms.untracked.bool(True),
                          PileupJetIdParameters = cms.PSet(pu_jetid)
                        ))
  setattr( process, 'selectedFlashggSTDPUPPIJets'+ label,
           cms.EDFilter("FLASHggJetSelector",
                        src = cms.InputTag( 'flashggSTDPUPPIJets'+ label ),
                        cut = cms.string("pt > 15.")
                      ))
