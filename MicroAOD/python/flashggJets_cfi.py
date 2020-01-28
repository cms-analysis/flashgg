# this is based on the jet 74X recipe for jet re-clustering
# twiki source : https://twiki.cern.ch/twiki/bin/view/CMSPublic/WorkBookMiniAOD2015#Advanced_topics_re_clustering_ev

import FWCore.ParameterSet.Config as cms
#from RecoJets.JetProducers.PileupJetIDParams_cfi import cutbased_new as pu_jetid
from PhysicsTools.PatAlgos.tools.jetTools import addJetCollection
# from CondCore.DBCommon.CondDBSetup_cfi import *
from CondCore.CondDB.CondDB_cfi import *

import os

flashggBTag = 'pfCombinedInclusiveSecondaryVertexV2BJetTags'
flashggCMVABTag = 'pfCombinedMVAV2BJetTags'
flashggDeepCSVb = 'pfDeepCSVJetTags:probb' 
flashggDeepCSVbb = 'pfDeepCSVJetTags:probbb'
flashggDeepCSVc  = 'pfDeepCSVJetTags:probc'
flashggDeepCSVudsg = 'pfDeepCSVJetTags:probudsg'
flashggDeepCSV= 'pfDeepCSV'

# DeepJet discriminators
flashggDeepFlavourb = 'pfDeepFlavourJetTags:probb'
flashggDeepFlavourbb = 'pfDeepFlavourJetTags:probbb'
flashggDeepFlavourc  = 'pfDeepFlavourJetTags:probc'
flashggDeepFlavouruds = 'pfDeepFlavourJetTags:probuds'
flashggDeepFlavourg  = 'pfDeepFlavourJetTags:probg'
flashggDeepFlavourlepb = 'pfDeepFlavourJetTags:problepb'
flashggDeepJet = 'pfDeepJet'

maxJetCollections = 12
qgDatabaseVersion = 'AK4chs_94X'

def addFlashggPFCHSJets(process,
			DeepJet, 
                        isData,
                        vertexIndex = 0, 
                        #doQGTagging = True, 
                        label ='', 
                        debug = False):


  setattr(process,'pfCHSLeg' + label,cms.EDFilter("CandPtrSelector", src = cms.InputTag("packedPFCandidates"), cut = cms.string("fromPV(%i)"%vertexIndex)))
  #Import RECO jet producer for ak4 PF and GEN jet
  from RecoJets.JetProducers.ak4PFJets_cfi  import ak4PFJets
  setattr(process, 'ak4PFJetsCHSLeg' + label, ak4PFJets.clone ( src = 'pfCHSLeg' + label, doAreaFastjet = True))

  if isData:
    JECs = ['L1FastJet', 'L2Relative', 'L3Absolute','L2L3Residual']
  else:
    JECs = ['L1FastJet', 'L2Relative', 'L3Absolute']

  # NOTE: this is the 74X recipe for the jet clustering - has been confirmed to give reasonable results in 10X
  addJetCollection(
    process,
    postfix        = label,
    labelName      = 'AK4PFCHSLeg',
    jetSource      = cms.InputTag('ak4PFJetsCHSLeg' + label),
    pvSource       = cms.InputTag('offlineSlimmedPrimaryVertices'),
    pfCandidates   = cms.InputTag('packedPFCandidates'),
    svSource       = cms.InputTag('slimmedSecondaryVertices'),
    elSource       = cms.InputTag("slimmedElectrons"),
    muSource       = cms.InputTag("slimmedMuons"),
    runIVF         = True,
    btagDiscriminators = [ flashggBTag, flashggCMVABTag , flashggDeepCSVb, flashggDeepCSVbb, flashggDeepCSVc, flashggDeepCSVudsg ],
    jetCorrections = ('AK4PFchs', JECs, 'None'),
    genJetCollection = cms.InputTag('slimmedGenJets'),
    genParticles     = cms.InputTag('prunedGenParticles'),
    # jet param
    algo = 'AK', rParam = 0.4
    #btagInfos =  ['pfImpactParameterTagInfos', 'pfSecondaryVertexTagInfos', 'pfInclusiveSecondaryVertexFinderTagInfos', 'softPFMuonsTagInfos', 'softPFElectronsTagInfos'] #Extra Btagging Info
  )

  #Recalculate btagging info
  getattr( process, 'patJetsAK4PFCHSLeg' + label).addTagInfos = True
  
  #adjust PV used for Jet Corrections
  #process.patJetCorrFactorsAK4PFCHSLeg.primaryVertices = "offlineSlimmedPrimaryVertices"
  getattr(process, 'patJetCorrFactorsAK4PFCHSLeg' + label).primaryVertices = "offlineSlimmedPrimaryVertices"

  if DeepJet == "rerun":
    from PhysicsTools.PatAlgos.tools.jetTools import updateJetCollection
    updateJetCollection(
      process,
      jetSource = cms.InputTag("slimmedJets"),
      pvSource = cms.InputTag('offlineSlimmedPrimaryVertices'),
      svSource = cms.InputTag('slimmedSecondaryVertices'),
      btagDiscriminators = [flashggDeepFlavourb, flashggDeepFlavourbb, flashggDeepFlavourc, flashggDeepFlavouruds,flashggDeepFlavourlepb, flashggDeepFlavourg,],
      jetCorrections = ('AK4PFchs', JECs, 'None'),
     )
    miniaodJets="selectedUpdatedPatJets"
  else: miniaodJets="slimmedJets"

  if not hasattr(process,"QGPoolDBESSource"):
    process.QGPoolDBESSource = cms.ESSource("PoolDBESSource",
                                            #CondDB,
                                            toGet = cms.VPSet(),
                                            connect = cms.string('sqlite:QGL_'+qgDatabaseVersion+'.db') 
                                            )
    process.es_prefer_qg = cms.ESPrefer('PoolDBESSource','QGPoolDBESSource')
  
  for type in ['AK4PFchs']:#,'AK4PFchs_antib']:
    process.QGPoolDBESSource.toGet.extend(cms.VPSet(cms.PSet(
      record = cms.string('QGLikelihoodRcd'),
      tag    = cms.string('QGLikelihoodObject_v1_AK4'),
      label  = cms.untracked.string('QGL_'+type)
    )))
  
  from RecoJets.JetProducers.QGTagger_cfi import QGTagger
  setattr( process, 'QGTaggerPFCHS' + label,  
           QGTagger.clone( srcJets   = 'patJetsAK4PFCHSLeg' + label ,jetsLabel = 'QGL_AK4PFchs', vertexIndex = cms.uint32(vertexIndex),
                           srcVertexCollection = 'offlineSlimmedPrimaryVertices'))

  from RecoJets.JetProducers.PileupJetIDParams_cfi import full_81x_chs
  pujidparam = full_81x_chs
  
  flashggJets = cms.EDProducer('FlashggJetProducer',
                               DiPhotonTag = cms.InputTag('flashggDiPhotons'),
                               VertexTag   = cms.InputTag('offlineSlimmedPrimaryVertices'),
                               JetTag      = cms.InputTag('patJetsAK4PFCHSLeg' + label),
                               VertexCandidateMapTag = cms.InputTag("flashggVertexMapForCHS"),
                               qgVariablesInputTag   = cms.InputTag('QGTaggerPFCHS'+label, 'qgLikelihood'),
                               ComputeSimpleRMS = cms.bool(True),
                               PileupJetIdParameters = pujidparam,
                               rho     = cms.InputTag("fixedGridRhoFastjetAll"),
                               JetCollectionIndex = cms.uint32(vertexIndex),
                               Debug = cms.untracked.bool(debug),
                               DoPuJetID = cms.bool(True),
                               ComputeRegVars = cms.bool(True),
                               MinPtForEneSum = cms.double(0.),
                               MaxEtaForEneSum = cms.double(2.5),
                               NJetsForEneSum = cms.uint32(0),
                               MiniAodJetTag = cms.InputTag(miniaodJets)
                               )
  setattr( process, 'flashggPFCHSJets'+ label, flashggJets)

  # randomize Jets
  from flashgg.MicroAOD.flashggRandomizedJetProducer_cfi import flashggRandomizedJets
  flashggRandomizedPFCHSJets = flashggRandomizedJets.clone()
  flashggRandomizedPFCHSJets.src = "flashggPFCHSJets" + label
  setattr(process.RandomNumberGeneratorService, 'flashggRandomizedPFCHSJets' + label, cms.PSet(initialSeed = cms.untracked.uint32(36423784 + int(label))))
  setattr( process, 'flashggRandomizedPFCHSJets' + label, flashggRandomizedPFCHSJets )

  flashggSelectedJets = cms.EDFilter("FLASHggJetSelector",
                                     src = cms.InputTag( 'flashggRandomizedPFCHSJets' + label ),
                                     cut = cms.string("pt > 15.")
  )
  setattr( process, 'flashggSelectedPFCHSJets'+label, flashggSelectedJets )





  
def addFlashggPuppiJets(process,
                        vertexIndex = 0,
                        doQGTagging = True,
                        label       ='',
                        useLocalJEC = True,
                        dbfile      = 'flashgg/MetaData/data/PuppiJEC/PY8_RunIISpring15DR74_bx50_MC.db',
                        debug       = False):
    
  from CommonTools.PileupAlgos.flashggPuppi_cff          import flashggPuppi 
  from RecoJets.JetProducers.ak4PFJets_cfi               import ak4PFJets

  # fill the puppi parameters
  setattr(process, 'flashggPuppi' + label,
          flashggPuppi.clone( candName              = cms.InputTag('packedPFCandidates'),
                              vertexName            = cms.InputTag('offlineSlimmedPrimaryVertices'),
                              diPhotonTag           = cms.InputTag('flashggDiPhotons'),
                              VertexCandidateMapTag = cms.InputTag('flashggVertexMapForPUPPI'),
                              vertexIndex           = cms.uint32(vertexIndex),
                              debug                 = cms.untracked.bool(debug)
                            )
  )
  setattr ( process, 'ak4PFJetsPuppi' + label,
            ak4PFJets.clone ( src = cms.InputTag('flashggPuppi' + label), doAreaFastjet = True)
          )
  
  if useLocalJEC :
    print ':: using a local JEC dbfile for PUPPI :',
    print '\t -- ',  dbfile
    
    from flashgg.MicroAOD.flashggJetTools_cfi import loadLocalJECDBfile
    loadLocalJECDBfile(process,
                       dbfile = os.environ['CMSSW_BASE'] + '/src/' + dbfile,
                       tag    = 'JetCorrectorParametersCollection_PY8_RunIISpring15DR74_bx50_MC_AK4PUPPI',
                       label  = 'AK4PFPuppi')
    
  # do jet clustering
  addJetCollection(
    process,
    postfix            = label,
    labelName          = 'AK4PUPPI',
    jetSource          = cms.InputTag('ak4PFJetsPuppi' + label),
    pvSource           = cms.InputTag('offlineSlimmedPrimaryVertices'),
    pfCandidates       = cms.InputTag('packedPFCandidates'),
    svSource           = cms.InputTag('slimmedSecondaryVertices'),\
    elSource       = cms.InputTag("slimmedElectrons"),
    muSource       = cms.InputTag("slimmedMuons"),
    runIVF         = True,
    btagDiscriminators = [ flashggBTag, flashggCMVABTag , flashggDeepCSVb, flashggDeepCSVbb, flashggDeepCSVc, flashggDeepCSVudsg, ],
    jetCorrections     = ('AK4PFPuppi',['L1FastJet',  'L2Relative', 'L3Absolute'], 'None'),
    genJetCollection   = cms.InputTag('slimmedGenJets'),
    genParticles       = cms.InputTag('prunedGenParticles'),
    # jet param
    algo = 'AK', rParam = 0.4
  )

  getattr(process, 'patJetCorrFactorsAK4PUPPI' + label).primaryVertices = "offlineSlimmedPrimaryVertices"
  setattr( process,'flashggPUPPIJets'+ label,
           cms.EDProducer('FlashggJetProducer',
                          DiPhotonTag           = cms.InputTag('flashggDiPhotons'),
                          VertexTag             = cms.InputTag('offlineSlimmedPrimaryVertices'),
                          JetTag                = cms.InputTag('patJetsAK4PUPPI' + label),
                          VertexCandidateMapTag = cms.InputTag("flashggVertexMapForPUPPI"),
                          UsePuppi              = cms.untracked.bool(True),
                          ComputeSimpleRMS = cms.bool(True),
                          ComputeRegVars = cms.bool(False)
#                          PileupJetIdParameters = cms.PSet(pu_jetid)
                        ))

  # randomize Jets
  from flashgg.MicroAOD.flashggRandomizedJetProducer_cfi import flashggRandomizedJets
  flashggRandomizedPUPPIJets = flashggRandomizedJets.clone()
  flashggRandomizedPUPPIJets.src = "flashggPUPPIJets" + label
  setattr(process.RandomNumberGeneratorService, 'flashggRandomizedPUPPIJets' + label, cms.PSet(initialSeed = cms.untracked.uint32(36421523 + int(label))))
  setattr( process, 'flashggRandomizedPUPPIJets' + label, flashggRandomizedPUPPIJets )

  setattr( process, 'selectedFlashggPUPPIJets'+ label,
           cms.EDFilter("FLASHggJetSelector",
                        src = cms.InputTag( 'flashggRandomizedPUPPIJets'+ label ),
                        cut = cms.string("pt > 15.")
                      ))
  
  
  
import FWCore.ParameterSet.Config as cms

JetCollectionVInputTag      = cms.VInputTag()
PuppiJetCollectionVInputTag = cms.VInputTag()
for i in range(0,maxJetCollections):
  JetCollectionVInputTag.append(cms.InputTag('flashggSelectedPFCHSJets' + str(i)))
  PuppiJetCollectionVInputTag.append(cms.InputTag('selectedFlashggPUPPIJets' + str(i)))


  
flashggFinalJets = cms.EDProducer("FlashggVectorVectorJetCollector",
                                  inputTagJets= JetCollectionVInputTag
)

flashggFinalPuppiJets = cms.EDProducer("FlashggVectorVectorJetCollector",
                                  inputTagJets= PuppiJetCollectionVInputTag
)

flashggUnpackedJets = cms.EDProducer("FlashggVectorVectorJetUnpacker",
                                     JetsTag = cms.InputTag("flashggFinalJets"),
                                     NCollections = cms.uint32(maxJetCollections)
                                     )

UnpackedJetCollectionVInputTag = cms.VInputTag()
for i in range(0,maxJetCollections):
    UnpackedJetCollectionVInputTag.append(cms.InputTag('flashggUnpackedJets',str(i)))
