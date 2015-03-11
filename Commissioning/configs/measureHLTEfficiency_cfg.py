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
                            #fileNames = cms.untracked.vstring("file:/afs/cern.ch/user/s/sani/work/CMSSW_7_0_7_patch1/src/flashgg/MicroAODProducers/test/myOutputFile.root")
                            fileNames = cms.untracked.vstring("file:/afs/cern.ch/user/s/sani/work/flashgg/src/flashgg/Commissioning/plugins/../../MicroAODProducers/test/myMicroAODOutputFile.root")
                            )

process.hltTest = cms.EDAnalyzer('HLTEfficiency',
                                 outputFileName  = cms.string("output.root"),
                                 diphoMVACut     = cms.double(0.05),                                
                                 diphotons       = cms.InputTag("flashggHLTDiPhotons"),
                                 diphoMVAResult  = cms.InputTag("flashggDiPhotonMVA"),
                                 electrons       = cms.InputTag("flashggElectrons"),
                                 tagFilterName   = cms.vstring("hltEle20WP60Ele8TrackIsoFilter", "hltEle25WP60SC4TrackIsoFilter"),
                                 probeFilterName = cms.vstring("hltEle20WP60Ele8EtUnseededFilter", "hltEle25WP60SC4EtUnseededFilter"),
                                 filterName      = cms.vstring("hltEG42Iso50T80LCaloId24b40eHE10R9Id65HcalIsoLastFilter", "hltEG42R9Id85HE10R9Id65R9IdLastFilter",
                                                               "hltEG22R9Id85HE10R9Id65R9IdUnseededLastFilter", "hltEG22Iso50T80LCaloId24b40eHE10R9Id65TrackIsoUnseededLastFilter",
                                                               ),
                                 l1Iso           = cms.InputTag("l1extraParticles:Isolated"),
                                 l1NonIso        = cms.InputTag("l1extraParticles:NonIsolated"),
                                 bits            = cms.InputTag('TriggerResults::HLT'),
                                 objects         = cms.InputTag('selectedPatTrigger'),
                                 prescales       = cms.InputTag('patTrigger'),
                                 )

process.p = cms.Path(process.hltTest)

