import FWCore.ParameterSet.Config as cms
import FWCore.Utilities.FileUtils as FileUtils

process = cms.Process("FLASHggAODtoMicroAOD")

####### STANDARD CONFIG IMPORTS
# import of standard configuration (from AOD->miniAOD)
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('SimGeneral.MixingModule.mixNoPU_cfi')
process.load('Configuration.StandardSequences.PATMC_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')

# import of standard configuration (from miniAOD->microAOD)
process.load("FWCore.MessageService.MessageLogger_cfi")
process.load("Configuration.StandardSequences.GeometryDB_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")

####### GLOBAL TAG
#global tag (microAOD->miniAOD)
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'PLS170_V7AN1::All', '')

#global tag (miniAOD->microAOD)
#process.GlobalTag.globaltag = 'POSTLS170_V5::All'

####### EVENT NUMBER 
#event number
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(1000) )

####### INPUT SOURCE
# Input source (from AOD->miniAOD)
process.source = cms.Source("PoolSource",
    secondaryFileNames = cms.untracked.vstring(),
    fileNames = cms.untracked.vstring(	
		'/store/relval/CMSSW_7_0_0/RelValH130GGgluonfusion_13/GEN-SIM-RECO/PU50ns_POSTLS170_V4-v2/00000/E698F634-CD98-E311-B38F-02163E00EA9F.root',
		)
)

process.options = cms.untracked.PSet(
    allowUnscheduled = cms.untracked.bool(True)
)
# Input source (from miniAOD->microAOD)
#process.source = cms.Source("PoolSource",fileNames=cms.untracked.vstring("file:../../../miniAOD-prod_PAT.root"))

####### PRODUCTION INFO
# Production Info
process.configurationMetadata = cms.untracked.PSet(
    version = cms.untracked.string('$Revision: 1.19 $'),
    annotation = cms.untracked.string('miniAOD-prod nevts:1'),
    name = cms.untracked.string('Applications')
)

####### PROCESSES
### AOD->miniAOD processes

from PhysicsTools.PatAlgos.slimming.miniAOD_tools import miniAOD_customizeAllMC 
process = miniAOD_customizeAllMC(process)

### miniAOD->microAOD processes
# Each track associated only to the closest vertex (or none if dZ >= MaxAllowedDz for all vertices)
process.flashggVertexMapUnique = cms.EDProducer('FlashggDzVertexMapProducer',
                                                PFCandidatesTag=cms.untracked.InputTag('packedPFCandidates'),
                                                VertexTag=cms.InputTag('offlineSlimmedPrimaryVertices'),
                                                MaxAllowedDz=cms.double(0.2) # in cm
                                                )

# Tracks will show up as associated to every vertex for which dZ < MaxAllowedDz
process.flashggVertexMapNonUnique = cms.EDProducer('FlashggDzVertexMapProducer',
                                                   PFCandidatesTag=cms.untracked.InputTag('packedPFCandidates'),
                                                   VertexTag=cms.InputTag('offlineSlimmedPrimaryVertices'),
                                                   MaxAllowedDz=cms.double(0.2), # in cm
                                                   UseEachTrackOnce=cms.untracked.bool(False)
                                                   )
### NEW to help validate, first sprout of vertex association based on AOD
process.flashggVertexMapValidator = cms.EDProducer('FlashggVertexMapValidator',
                                                PFCandidatesTag=cms.InputTag('packedPFCandidates'),
                                                VertexTag=cms.InputTag('offlineSlimmedPrimaryVertices'),
                                                VertexTagAOD=cms.InputTag('offlinePrimaryVertices'),
                                                UseMiniAODTrackVertexAssociation=cms.bool(True),
                                                DoTextDebug=cms.untracked.bool(False)
                                                )

process.load("flashgg/MicroAODProducers/flashggPhotons_cfi")
process.load("flashgg/MicroAODProducers/flashggDiPhotons_cfi")
process.load("flashgg/MicroAODProducers/flashggPreselectedDiPhotons_cfi")

### TO MAKE A QUICK TREE COMPARING THE VERTEX MAPS
process.TFileService = cms.Service("TFileService",fileName = cms.string("VertexValidationTree.root"))
process.flashggVertexValidationTreeMaker = cms.EDAnalyzer('FlashggVertexValidationTreeMaker',
                                                          VertexTag=cms.untracked.InputTag('offlineSlimmedPrimaryVertices'),
                                                          GenParticleTag=cms.untracked.InputTag('prunedGenParticles'),
                                                          VertexCandidateMapTagDz=cms.InputTag('flashggVertexMapUnique'),
                                                          VertexCandidateMapTagAOD = cms.InputTag('flashggVertexMapValidator'),
                                                          JetTagDz = cms.InputTag("flashggJets"),
                                                          JetTagRecoBasedMap = cms.InputTag("flashggJetsUsingRecoBasedVertexMap"),
																													DiPhotonTag = cms.InputTag('flashggDiPhotons')
#                                                          JetTagReco = cms.InputTag("flashggJetsUsingRecoJets")
                                                          )
                 
# This requires you to have done: git cms-merge-topic -u sethzenz:pileupjetid-for-flashgg                                                                                          
process.load("RecoJets.JetProducers.PileupJetIDParams_cfi")

process.flashggJets = cms.EDProducer('FlashggJetProducer',
                                     DiPhotonTag=cms.InputTag('flashggDiPhotons'),
#                                     VertexTag=cms.untracked.InputTag('offlineSlimmedPrimaryVertices'),
                                     JetTag=cms.untracked.InputTag('slimmedJets'),
                                     VertexCandidateMapTag = cms.InputTag("flashggVertexMapUnique"),
                                     PileupJetIdParameters=cms.PSet(process.full_5x_chs) # from PileupJetIDParams_cfi                                                                
                                     )

process.flashggJetsUsingRecoBasedVertexMap = cms.EDProducer('FlashggJetProducer',
                                                           DiPhotonTag=cms.InputTag('flashggDiPhotons'),
#                                                           VertexTag=cms.untracked.InputTag('offlineSlimmedPrimaryVertices'),
                                                           JetTag=cms.untracked.InputTag('slimmedJets'),
                                                           VertexCandidateMapTag = cms.InputTag("flashggVertexMapValidator"),
                                                           PileupJetIdParameters=cms.PSet(process.full_5x_chs) # from PileupJetIDParams_cfi
                                                           )

####### OUTPUT								 
#Output definition (AOD->miniAOD)
#process.MINIAODSIMoutput = cms.OutputModule("PoolOutputModule",
#    compressionLevel = cms.untracked.int32(4),
#    compressionAlgorithm = cms.untracked.string('LZMA'),
#    eventAutoFlushCompressedSize = cms.untracked.int32(15728640),
#    outputCommands = process.MINIAODSIMEventContent.outputCommands,
#    fileName = cms.untracked.string('miniAOD-prod_PAT.root'),
#    dataset = cms.untracked.PSet(
#        filterName = cms.untracked.string(''),
#        dataTier = cms.untracked.string('')
#    ),
#    dropMetaData = cms.untracked.string('ALL'),
#    fastCloning = cms.untracked.bool(False),
#    overrideInputFileSplitLevels = cms.untracked.bool(True)
#)

#Output definition (miniAOD->microAOD)
process.out = cms.OutputModule("PoolOutputModule", fileName = cms.untracked.string('myAOD_to_microAOD.root'),
                               outputCommands = cms.untracked.vstring("drop *",
                                                                      "keep *_flashgg*_*_*",
                                                                      "drop *_flashggVertexMap*_*_*",
                                                                      "keep *_offlineSlimmedPrimaryVertices_*_*",
                                                                      "keep *_reducedEgamma_reduced*Clusters_*",
                                                                      "keep *_reducedEgamma_*PhotonCores_*"
                                                                      )
)




####### PATH
# paths from AOD->miniAOD
process.Flag_trackingFailureFilter = cms.Path(process.goodVertices+process.trackingFailureFilter)
process.Flag_goodVertices = cms.Path(process.goodVertices)
process.Flag_CSCTightHaloFilter = cms.Path(process.CSCTightHaloFilter)
process.Flag_trkPOGFilters = cms.Path(process.trkPOGFilters)
process.Flag_trkPOG_logErrorTooManyClusters = cms.Path(~process.logErrorTooManyClusters)
process.Flag_EcalDeadCellTriggerPrimitiveFilter = cms.Path(process.EcalDeadCellTriggerPrimitiveFilter)
process.Flag_ecalLaserCorrFilter = cms.Path(process.ecalLaserCorrFilter)
process.Flag_trkPOG_manystripclus53X = cms.Path(~process.manystripclus53X)
process.Flag_eeBadScFilter = cms.Path(process.eeBadScFilter)
process.Flag_METFilters = cms.Path(process.metFilters)
process.Flag_HBHENoiseFilter = cms.Path(process.HBHENoiseFilter)
process.Flag_trkPOG_toomanystripclus53X = cms.Path(~process.toomanystripclus53X)
process.Flag_hcalLaserEventFilter = cms.Path(process.hcalLaserEventFilter)

# paths from miniAOD->microAOD
process.p = cms.Path(process.flashggVertexMapValidator*
                     process.flashggVertexMapUnique*
                     process.flashggVertexMapNonUnique*
                     process.flashggPhotons*
                     process.flashggDiPhotons*
                     process.flashggPreselectedDiPhotons*
                     process.flashggJets*
                     process.flashggJetsUsingRecoBasedVertexMap*
                     process.flashggVertexValidationTreeMaker
                    )

# endpaths from AOD->miniAOD
#process.endjob_step = cms.EndPath(process.endOfProcess)
#process.MINIAODSIMoutput_step = cms.EndPath(process.MINIAODSIMoutput)

# endpaths from miniAOD->microAOD
process.e = cms.EndPath(process.out)
