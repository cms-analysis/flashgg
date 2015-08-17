import FWCore.ParameterSet.Config as cms
import FWCore.Utilities.FileUtils as FileUtils

process = cms.Process("FLASHggDiphotonValidation")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.load("Configuration.StandardSequences.GeometryDB_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.GlobalTag.globaltag = 'POSTLS170_V5::All'

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32( -1 ) )

process.source = cms.Source("PoolSource",fileNames=cms.untracked.vstring("file:/afs/cern.ch/user/c/carrillo/flashgg/CMSSW_7_4_6_patch2/src/myMicroAODOutputFile.root"))

#process.TFileService = cms.Service("TFileService",fileName = cms.string("DiphotonValidationTree.root"))
process.flashggDiphotonPreselectionValidation = cms.EDAnalyzer('FlashggDiphotonPreselectionValidation',
    DiPhotonTag  = cms.InputTag('flashggDiPhotons'),
    DiPhotonTag1  = cms.InputTag('flashggPreselectedDiPhotons'),
    DiPhotonTag2  = cms.InputTag('flashggPreselectedDiPhotons')
)

process.p = cms.Path(
    process.flashggDiphotonPreselectionValidation
)
