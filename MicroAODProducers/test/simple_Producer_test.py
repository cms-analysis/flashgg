import FWCore.ParameterSet.Config as cms

process = cms.Process("FLASHggMicroAOD")

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
                                        VertexTag=cms.untracked.InputTag('offlineSlimmedPrimaryVertices'),
                                        PFCandidatesTag=cms.untracked.InputTag('packedPFCandidates'),
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

# single photon preselection is mickmicking as much as possible
# what is documented in AN 2013/253 v8 page 41 table 18,
process.flashggPreselectedDiPhotons = cms.EDFilter("CandViewSelector",
                                                   src = cms.InputTag("flashggDiPhotons"),
                                                   cut = cms.string(" \
                                                                     ( \
                                                                      (leadingPhoton.r9 <= 0.9 \
                                                                            && ( ((leadingPhoton.isEB && leadingPhoton.hadronicOverEm < 0.075 && leadingPhoton.sigmaIetaIeta < 0.014) \
                                                                            || (leadingPhoton.isEE && leadingPhoton.hadronicOverEm < 0.075 && leadingPhoton.sigmaIetaIeta < 0.034)) \
                                                                            && (leadingPhoton.hcalTowerSumEtConeDR03 - 0.005 * leadingPhoton.pt < 4.0) \
                                                                            && (leadingPhoton.trkSumPtHollowConeDR03 - 0.002 * leadingPhoton.pt < 4.0) \
                                                                            )) \
                                                                       || (leadingPhoton.r9 > 0.9 \
                                                                            && ( ((leadingPhoton.isEB && leadingPhoton.hadronicOverEm < 0.082 && leadingPhoton.sigmaIetaIeta < 0.014) \
                                                                            || (leadingPhoton.isEE && leadingPhoton.hadronicOverEm < 0.075 && leadingPhoton.sigmaIetaIeta < 0.034)) \
                                                                            && (leadingPhoton.hcalTowerSumEtConeDR03 - 0.005 * leadingPhoton.pt < 50.0) \
                                                                            && (leadingPhoton.trkSumPtHollowConeDR03 - 0.002 * leadingPhoton.pt < 50.0) \
                                                                            )) \
                                                                     ) && ( \
                                                                      (subLeadingPhoton.r9 <= 0.9 \
                                                                            && ( ((subLeadingPhoton.isEB && subLeadingPhoton.hadronicOverEm < 0.075 && subLeadingPhoton.sigmaIetaIeta < 0.014) \
                                                                            || (subLeadingPhoton.isEE && subLeadingPhoton.hadronicOverEm < 0.075 && subLeadingPhoton.sigmaIetaIeta < 0.034)) \
                                                                            && (subLeadingPhoton.hcalTowerSumEtConeDR03 - 0.005 * subLeadingPhoton.pt < 4.0) \
                                                                            && (subLeadingPhoton.trkSumPtHollowConeDR03 - 0.002 * subLeadingPhoton.pt < 4.0) \
                                                                            )) \
                                                                       || (subLeadingPhoton.r9 > 0.9 \
                                                                            && ( ((subLeadingPhoton.isEB && subLeadingPhoton.hadronicOverEm < 0.082 && subLeadingPhoton.sigmaIetaIeta < 0.014) \
                                                                            || (subLeadingPhoton.isEE && subLeadingPhoton.hadronicOverEm < 0.075 && subLeadingPhoton.sigmaIetaIeta < 0.034)) \
                                                                            && (subLeadingPhoton.hcalTowerSumEtConeDR03 - 0.005 * subLeadingPhoton.pt < 50.0) \
                                                                            && (subLeadingPhoton.trkSumPtHollowConeDR03 - 0.002 * subLeadingPhoton.pt < 50.0) \
                                                                            )) \
                                                                     ) \
                                                                    ")
                                                  )
#                                                                            && (leadingPhoton.getChargedPFIso02(getVertex) < 4.0) \
# For some reason the cut parser outputs an exception
# ----- Begin Fatal Exception 25-Jun-2014 16:04:12 CEST-----------------------
# An exception of category 'Configuration' occurred while
#    [0] Constructing the EventProcessor
#    [1] Constructing module: class=CandViewSelector label='flashggPreselectedDiPhotons'
# Exception Message:
# Cut parser error:Missing close parenthesis. (char 942)
# ----- End Fatal Exception -------------------------------------------------
                                                   

process.out = cms.OutputModule("PoolOutputModule", fileName = cms.untracked.string('myOutputFile.root'),
                               outputCommands = cms.untracked.vstring("drop *","keep *_flashgg*_*_*","keep *_offlineSlimmedPrimaryVertices_*_*")
)

process.p = cms.Path( process.flashggPhotons
                     *process.flashggDiPhotons
                     *process.flashggPreselectedDiPhotons
                    )

process.e = cms.EndPath(process.out)
