import FWCore.ParameterSet.Config as cms

process = cms.Process("FLASHggTEST")

process.load("FWCore.MessageService.MessageLogger_cfi")
#process.load("Geometry.CaloEventSetup.CaloTopology_cfi")
#process.load("Geometry.CMSCommonData.cmsIdealGeometryXML_cfi")
#process.load("Geometry.CaloEventSetup.CaloGeometry_cfi")
#process.load("CalibCalorimetry.EcalLaserCorrection.ecalLaserCorrectionService_cfi")
#process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")


process.load("Configuration.StandardSequences.GeometryDB_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.GlobalTag.globaltag = 'POSTLS170_V5::All'
#process.GlobalTag.toGet = cms.VPSet(
#    cms.PSet(record = cms.string("EcalIntercalibConstantsRcd"),
#             tag = cms.string("EcalIntercalibConstants_Bon_V20101105"),
#             connect = cms.untracked.string("frontier://FrontierProd/CMS_COND_31X_ECAL")
#             )
#    )

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(10) )

process.source = cms.Source("PoolSource",fileNames=cms.untracked.vstring("file:/afs/cern.ch/work/s/sethzenz/public/Hgg_miniAOD_run0/miniAOD_3.root"))

process.flashggPhotons = cms.EDProducer('FlashggPhotonProducer',
                                        PhotonTag=cms.untracked.InputTag('slimmedPhotons'),
                                        PFCandidatesTag=cms.untracked.InputTag('packedPFCandidates'), # Needed to compute ChargedPFIso for Run1 Legacy preselection
#                                        PhotonPreselectorName=cms.string("FlashggNoPhotonPreselector"),
                                        PhotonPreselectorName=cms.string("FlashggLegacyPhotonPreselector"),
                                        reducedBarrelRecHitCollection=cms.InputTag('reducedEgamma','reducedEBRecHits'),
                                        reducedEndcapRecHitCollection=cms.InputTag('reducedEgamma','reducedEERecHits'),
                                        reducedPreshowerRecHitCollection=cms.InputTag('reducedEgamma','reducedESRecHits')
                                        )
process.flashggDiPhotons = cms.EDProducer('FlashggDiPhotonProducer',
                                          PhotonTag=cms.untracked.InputTag('flashggPhotons'),
                                          VertexTag=cms.untracked.InputTag('offlineSlimmedPrimaryVertices'),
#                                         VertexSelectorName=cms.string("FlashggZerothVertexSelector"),
                                          VertexSelectorName=cms.string("FlashggLegacyVertexSelector")
                                          )

process.out = cms.OutputModule("PoolOutputModule", fileName = cms.untracked.string('myOutputFile.root'),
                               outputCommands = cms.untracked.vstring("drop *","keep *_flashgg*_*_*","keep *_offlineSlimmedPrimaryVertices_*_*")
)

process.p = cms.Path(process.flashggPhotons*process.flashggDiPhotons)

process.e = cms.EndPath(process.out)
