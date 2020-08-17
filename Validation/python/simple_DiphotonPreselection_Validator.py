import FWCore.ParameterSet.Config as cms
import FWCore.Utilities.FileUtils as FileUtils

process = cms.Process("FLASHggDiphotonValidation")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.load("Configuration.StandardSequences.GeometryDB_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.GlobalTag.globaltag = 'POSTLS170_V5::All'

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32( -1 ) )
process.MessageLogger = cms.Service("MessageLogger",
    destinations                = cms.untracked.vstring('preselectionEfficiencies', 'cerr'),
    categories                  = cms.untracked.vstring('EfficienciesInfo', 'FwkReport'),
    preselectionEfficiencies    = cms.untracked.PSet(
        threshold           = cms.untracked.string('INFO'),
        INFO                =  cms.untracked.PSet( limit = cms.untracked.int32(0) ),
        WARNING             = cms.untracked.PSet( limit = cms.untracked.int32(0) ),
        EfficienciesInfo    = cms.untracked.PSet( limit = cms.untracked.int32(-1) ),
    ),
    cerr                        = cms.untracked.PSet(
        EfficienciesInfo    = cms.untracked.PSet( limit = cms.untracked.int32(0) ),
        FwkReport           = cms.untracked.PSet( reportEvery = cms.untracked.int32(1000) )
    ),
)

process.source = cms.Source("PoolSource",fileNames=cms.untracked.vstring("file:myOutputFile.root"))

process.TFileService = cms.Service("TFileService",fileName = cms.string("DiphotonValidationTree.root"))
process.flashggDiphotonPreselectionValidationTreeMaker = cms.EDAnalyzer('FlashggDiphotonPreselectionValidationTreeMaker',
    DiPhotonTag  = cms.InputTag('flashggDiPhotons')
                                                          )

process.p = cms.Path(
    process.flashggDiphotonPreselectionValidationTreeMaker
)
