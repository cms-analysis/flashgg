import FWCore.ParameterSet.Config as cms
import FWCore.Utilities.FileUtils as FileUtils

process = cms.Process("HLTTest")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.load("Configuration.StandardSequences.GeometryDB_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.GlobalTag.globaltag = 'POSTLS170_V5::All'

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32( -1 ) )
process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32( 200 )

process.source = cms.Source("PoolSource",
                            #fileNames = cms.untracked.vstring("file:myMicroAODOutputFile.root")
                            #fileNames = cms.untracked.vstring("file:/afs/cern.ch/work/m/mplaner/myMicroAODOutputFile.root")
                            fileNames = cms.untracked.vstring("file:/afs/cern.ch/user/m/mplaner/CMSSW_7_4_6_patch2/src/flashgg/myMicroAODOutputFile.root")
                            )

process.hltTest = cms.EDAnalyzer('HLTEfficiency',
                                 outputFileName  = cms.string("output.root"),
                                 diphoMVACut     = cms.double(0.05),                                
                                 diphotons       = cms.InputTag("flashggPreselectedDiPhotons"),
                                 electrons       = cms.InputTag("flashggElectrons"),
                                                                  
                                 #updated paths for newest menu: 2015-06-09
                                 tagFilterName   = cms.vstring("hltEle25WP60Ele8TrackIsoFilter", "hltEle25WP60SC4TrackIsoFilter"),
                                 probeFilterName = cms.vstring("hltEle25WP60Ele8EtUnseededFilter", "hltEle25WP60SC4EtUnseededFilter"),
                                 filterName      = cms.vstring("hltEG30LIso60CaloId15b35eHE10R9Id50b80eEcalIsoLastFilter", "hltEG30LR9Id85b90eHE10R9Id50b80eR9IdLastFilter",
                                                               "hltEG18R9Id85b90eHE10R9Id50b80eR9UnseededLastFilter", "hltEG18Iso60CaloId15b35eHE10R9Id50b80eTrackIsoUnseededLastFilter"),
                                 l1Iso           = cms.InputTag("l1extraParticles:Isolated"),
                                 l1NonIso        = cms.InputTag("l1extraParticles:NonIsolated"),
                                 bits            = cms.InputTag('TriggerResults::HLT'),
                                 objects         = cms.InputTag('selectedPatTrigger'),
                                 prescales       = cms.InputTag('patTrigger'),
                                 )

process.p = cms.Path(process.hltTest)

