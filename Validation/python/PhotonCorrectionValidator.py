import FWCore.ParameterSet.Config as cms
import FWCore.Utilities.FileUtils as FileUtils

process = cms.Process("FLASHggPhotonCorrectionValidation")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.load("Configuration.StandardSequences.GeometryDB_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.GlobalTag.globaltag = 'POSTLS170_V5::All'

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32( -1 ) )

process.source = cms.Source("PoolSource",fileNames=cms.untracked.vstring("file:myOutputFile_Legacy.root"))

process.TFileService = cms.Service("TFileService",fileName = cms.string("PhotonCorrectionValidationTree_ZerothVtx.root"))
process.flashggPhotonCorrectionValidationTreeMaker = cms.EDAnalyzer('FlashggPhotonCorrectionValidationTreeMaker',
                                                                    DiPhotonTag = cms.InputTag('flashggDiPhotons')
                                                                   )

process.p = cms.Path(
                     process.flashggPhotonCorrectionValidationTreeMaker
                    )
