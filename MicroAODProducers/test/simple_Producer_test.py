import FWCore.ParameterSet.Config as cms
import FWCore.Utilities.FileUtils as FileUtils

process = cms.Process("FLASHggMicroAOD")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.load("Configuration.StandardSequences.GeometryDB_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.GlobalTag.globaltag = 'POSTLS170_V5::All'

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(100) )

process.source = cms.Source("PoolSource",fileNames=cms.untracked.vstring("file:/afs/cern.ch/work/s/sethzenz/public/Hgg_miniAOD_run0/miniAOD_3.root"))

# Each track associated only to the closest vertex (or none if dZ >= MaxAllowedDz for all vertices)
process.flashggVertexMapUnique = cms.EDProducer('FlashggDzVertexMapProducer',
                                                PFCandidatesTag=cms.untracked.InputTag('packedPFCandidates'),
                                                VertexTag=cms.untracked.InputTag('offlineSlimmedPrimaryVertices'),
                                                MaxAllowedDz=cms.double(0.2) # in cm
                                                )

# Tracks will show up as associated to every vertex for which dZ < MaxAllowedDz
process.flashggVertexMapNonUnique = cms.EDProducer('FlashggDzVertexMapProducer',
                                                   PFCandidatesTag=cms.untracked.InputTag('packedPFCandidates'),
                                                   VertexTag=cms.untracked.InputTag('offlineSlimmedPrimaryVertices'),
                                                   MaxAllowedDz=cms.double(0.2), # in cm
                                                   UseEachTrackOnce=cms.untracked.bool(False)
                                                   )

process.load("flashgg/MicroAODProducers/flashggPhotons_cfi")
process.load("flashgg/MicroAODProducers/flashggDiPhotons_cfi")

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
# Charged PF iso is still left to implement && (leadingPhoton.getChargedPFIso02(getVertex) < 4.0) \
                                                  

process.out = cms.OutputModule("PoolOutputModule", fileName = cms.untracked.string('myOutputFile.root'),
                               outputCommands = cms.untracked.vstring("drop *",
                                                                      "keep *_flashgg*_*_*",
                                                                      "drop *_flashggVertexMap*_*_*",
                                                                      "keep *_offlineSlimmedPrimaryVertices_*_*")
)

process.p = cms.Path(process.flashggVertexMapUnique*
                     process.flashggVertexMapNonUnique*
                     process.flashggPhotons*
                     process.flashggDiPhotons*
                     process.flashggPreselectedDiPhotons
                    )

process.e = cms.EndPath(process.out)
