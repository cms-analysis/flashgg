import FWCore.ParameterSet.Config as cms
import FWCore.Utilities.FileUtils as FileUtils

process = cms.Process("FLASHggMicroAOD")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.load("Configuration.StandardSequences.GeometryDB_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.GlobalTag.globaltag = 'POSTLS170_V5::All'

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )
process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32( 100 )

process.source = cms.Source("PoolSource",
                            fileNames=cms.untracked.vstring(
#"file:/afs/cern.ch/work/s/sethzenz/public/Hgg_miniAOD_run0/miniAOD_3.root",
"file:/afs/cern.ch/work/s/sethzenz/public/Hgg_miniAOD_run0/miniAOD_0.root",
"file:/afs/cern.ch/work/s/sethzenz/public/Hgg_miniAOD_run0/miniAOD_10.root",
"file:/afs/cern.ch/work/s/sethzenz/public/Hgg_miniAOD_run0/miniAOD_11.root",
"file:/afs/cern.ch/work/s/sethzenz/public/Hgg_miniAOD_run0/miniAOD_12.root",
"file:/afs/cern.ch/work/s/sethzenz/public/Hgg_miniAOD_run0/miniAOD_13.root",
"file:/afs/cern.ch/work/s/sethzenz/public/Hgg_miniAOD_run0/miniAOD_14.root",
"file:/afs/cern.ch/work/s/sethzenz/public/Hgg_miniAOD_run0/miniAOD_15.root",
"file:/afs/cern.ch/work/s/sethzenz/public/Hgg_miniAOD_run0/miniAOD_16.root",
"file:/afs/cern.ch/work/s/sethzenz/public/Hgg_miniAOD_run0/miniAOD_17.root",
"file:/afs/cern.ch/work/s/sethzenz/public/Hgg_miniAOD_run0/miniAOD_18.root",
"file:/afs/cern.ch/work/s/sethzenz/public/Hgg_miniAOD_run0/miniAOD_19.root"
#"file:/afs/cern.ch/work/s/sethzenz/public/Hgg_miniAOD_run0/miniAOD_1.root",
#"file:/afs/cern.ch/work/s/sethzenz/public/Hgg_miniAOD_run0/miniAOD_20.root",
#"file:/afs/cern.ch/work/s/sethzenz/public/Hgg_miniAOD_run0/miniAOD_21.root",
#"file:/afs/cern.ch/work/s/sethzenz/public/Hgg_miniAOD_run0/miniAOD_22.root",
#"file:/afs/cern.ch/work/s/sethzenz/public/Hgg_miniAOD_run0/miniAOD_23.root",
#"file:/afs/cern.ch/work/s/sethzenz/public/Hgg_miniAOD_run0/miniAOD_24.root",
#"file:/afs/cern.ch/work/s/sethzenz/public/Hgg_miniAOD_run0/miniAOD_25.root",
#"file:/afs/cern.ch/work/s/sethzenz/public/Hgg_miniAOD_run0/miniAOD_26.root",
#"file:/afs/cern.ch/work/s/sethzenz/public/Hgg_miniAOD_run0/miniAOD_27.root",
#"file:/afs/cern.ch/work/s/sethzenz/public/Hgg_miniAOD_run0/miniAOD_28.root",
#"file:/afs/cern.ch/work/s/sethzenz/public/Hgg_miniAOD_run0/miniAOD_29.root",
#"file:/afs/cern.ch/work/s/sethzenz/public/Hgg_miniAOD_run0/miniAOD_2.root",
#"file:/afs/cern.ch/work/s/sethzenz/public/Hgg_miniAOD_run0/miniAOD_30.root",
#"file:/afs/cern.ch/work/s/sethzenz/public/Hgg_miniAOD_run0/miniAOD_31.root",
#"file:/afs/cern.ch/work/s/sethzenz/public/Hgg_miniAOD_run0/miniAOD_32.root",
#"file:/afs/cern.ch/work/s/sethzenz/public/Hgg_miniAOD_run0/miniAOD_33.root",
#"file:/afs/cern.ch/work/s/sethzenz/public/Hgg_miniAOD_run0/miniAOD_34.root",
#"file:/afs/cern.ch/work/s/sethzenz/public/Hgg_miniAOD_run0/miniAOD_35.root",
#"file:/afs/cern.ch/work/s/sethzenz/public/Hgg_miniAOD_run0/miniAOD_36.root",
#"file:/afs/cern.ch/work/s/sethzenz/public/Hgg_miniAOD_run0/miniAOD_37.root",
#"file:/afs/cern.ch/work/s/sethzenz/public/Hgg_miniAOD_run0/miniAOD_38.root",
#"file:/afs/cern.ch/work/s/sethzenz/public/Hgg_miniAOD_run0/miniAOD_39.root",
#"file:/afs/cern.ch/work/s/sethzenz/public/Hgg_miniAOD_run0/miniAOD_3.root",
#"file:/afs/cern.ch/work/s/sethzenz/public/Hgg_miniAOD_run0/miniAOD_40.root",
#"file:/afs/cern.ch/work/s/sethzenz/public/Hgg_miniAOD_run0/miniAOD_41.root",
#"file:/afs/cern.ch/work/s/sethzenz/public/Hgg_miniAOD_run0/miniAOD_42.root",
#"file:/afs/cern.ch/work/s/sethzenz/public/Hgg_miniAOD_run0/miniAOD_43.root",
#"file:/afs/cern.ch/work/s/sethzenz/public/Hgg_miniAOD_run0/miniAOD_44.root",
#"file:/afs/cern.ch/work/s/sethzenz/public/Hgg_miniAOD_run0/miniAOD_45.root",
#"file:/afs/cern.ch/work/s/sethzenz/public/Hgg_miniAOD_run0/miniAOD_46.root",
#"file:/afs/cern.ch/work/s/sethzenz/public/Hgg_miniAOD_run0/miniAOD_47.root",
#"file:/afs/cern.ch/work/s/sethzenz/public/Hgg_miniAOD_run0/miniAOD_48.root",
#"file:/afs/cern.ch/work/s/sethzenz/public/Hgg_miniAOD_run0/miniAOD_49.root",
#"file:/afs/cern.ch/work/s/sethzenz/public/Hgg_miniAOD_run0/miniAOD_4.root",
#"file:/afs/cern.ch/work/s/sethzenz/public/Hgg_miniAOD_run0/miniAOD_50.root",
#"file:/afs/cern.ch/work/s/sethzenz/public/Hgg_miniAOD_run0/miniAOD_51.root",
#"file:/afs/cern.ch/work/s/sethzenz/public/Hgg_miniAOD_run0/miniAOD_52.root",
#"file:/afs/cern.ch/work/s/sethzenz/public/Hgg_miniAOD_run0/miniAOD_53.root",
#"file:/afs/cern.ch/work/s/sethzenz/public/Hgg_miniAOD_run0/miniAOD_54.root",
#"file:/afs/cern.ch/work/s/sethzenz/public/Hgg_miniAOD_run0/miniAOD_55.root",
#"file:/afs/cern.ch/work/s/sethzenz/public/Hgg_miniAOD_run0/miniAOD_56.root",
#"file:/afs/cern.ch/work/s/sethzenz/public/Hgg_miniAOD_run0/miniAOD_57.root",
#"file:/afs/cern.ch/work/s/sethzenz/public/Hgg_miniAOD_run0/miniAOD_58.root",
#"file:/afs/cern.ch/work/s/sethzenz/public/Hgg_miniAOD_run0/miniAOD_59.root",
#"file:/afs/cern.ch/work/s/sethzenz/public/Hgg_miniAOD_run0/miniAOD_5.root",
#"file:/afs/cern.ch/work/s/sethzenz/public/Hgg_miniAOD_run0/miniAOD_60.root",
#"file:/afs/cern.ch/work/s/sethzenz/public/Hgg_miniAOD_run0/miniAOD_61.root",
#"file:/afs/cern.ch/work/s/sethzenz/public/Hgg_miniAOD_run0/miniAOD_62.root",
#"file:/afs/cern.ch/work/s/sethzenz/public/Hgg_miniAOD_run0/miniAOD_63.root",
#"file:/afs/cern.ch/work/s/sethzenz/public/Hgg_miniAOD_run0/miniAOD_64.root",
#"file:/afs/cern.ch/work/s/sethzenz/public/Hgg_miniAOD_run0/miniAOD_65.root",
#"file:/afs/cern.ch/work/s/sethzenz/public/Hgg_miniAOD_run0/miniAOD_66.root",
#"file:/afs/cern.ch/work/s/sethzenz/public/Hgg_miniAOD_run0/miniAOD_67.root",
#"file:/afs/cern.ch/work/s/sethzenz/public/Hgg_miniAOD_run0/miniAOD_68.root",
#"file:/afs/cern.ch/work/s/sethzenz/public/Hgg_miniAOD_run0/miniAOD_69.root",
#"file:/afs/cern.ch/work/s/sethzenz/public/Hgg_miniAOD_run0/miniAOD_6.root",
#"file:/afs/cern.ch/work/s/sethzenz/public/Hgg_miniAOD_run0/miniAOD_70.root",
#"file:/afs/cern.ch/work/s/sethzenz/public/Hgg_miniAOD_run0/miniAOD_71.root",
#"file:/afs/cern.ch/work/s/sethzenz/public/Hgg_miniAOD_run0/miniAOD_72.root",
#"file:/afs/cern.ch/work/s/sethzenz/public/Hgg_miniAOD_run0/miniAOD_73.root",
#"file:/afs/cern.ch/work/s/sethzenz/public/Hgg_miniAOD_run0/miniAOD_74.root",
#"file:/afs/cern.ch/work/s/sethzenz/public/Hgg_miniAOD_run0/miniAOD_75.root",
#"file:/afs/cern.ch/work/s/sethzenz/public/Hgg_miniAOD_run0/miniAOD_76.root",
#"file:/afs/cern.ch/work/s/sethzenz/public/Hgg_miniAOD_run0/miniAOD_77.root",
#"file:/afs/cern.ch/work/s/sethzenz/public/Hgg_miniAOD_run0/miniAOD_78.root",
#"file:/afs/cern.ch/work/s/sethzenz/public/Hgg_miniAOD_run0/miniAOD_79.root",
#"file:/afs/cern.ch/work/s/sethzenz/public/Hgg_miniAOD_run0/miniAOD_7.root",
#"file:/afs/cern.ch/work/s/sethzenz/public/Hgg_miniAOD_run0/miniAOD_80.root",
#"file:/afs/cern.ch/work/s/sethzenz/public/Hgg_miniAOD_run0/miniAOD_81.root",
#"file:/afs/cern.ch/work/s/sethzenz/public/Hgg_miniAOD_run0/miniAOD_82.root",
#"file:/afs/cern.ch/work/s/sethzenz/public/Hgg_miniAOD_run0/miniAOD_83.root",
#"file:/afs/cern.ch/work/s/sethzenz/public/Hgg_miniAOD_run0/miniAOD_84.root",
#"file:/afs/cern.ch/work/s/sethzenz/public/Hgg_miniAOD_run0/miniAOD_85.root",
#"file:/afs/cern.ch/work/s/sethzenz/public/Hgg_miniAOD_run0/miniAOD_86.root",
#"file:/afs/cern.ch/work/s/sethzenz/public/Hgg_miniAOD_run0/miniAOD_87.root",
#"file:/afs/cern.ch/work/s/sethzenz/public/Hgg_miniAOD_run0/miniAOD_88.root",
#"file:/afs/cern.ch/work/s/sethzenz/public/Hgg_miniAOD_run0/miniAOD_89.root",
#"file:/afs/cern.ch/work/s/sethzenz/public/Hgg_miniAOD_run0/miniAOD_8.root",
#"file:/afs/cern.ch/work/s/sethzenz/public/Hgg_miniAOD_run0/miniAOD_90.root",
#"file:/afs/cern.ch/work/s/sethzenz/public/Hgg_miniAOD_run0/miniAOD_91.root",
#"file:/afs/cern.ch/work/s/sethzenz/public/Hgg_miniAOD_run0/miniAOD_92.root",
#"file:/afs/cern.ch/work/s/sethzenz/public/Hgg_miniAOD_run0/miniAOD_93.root",
#"file:/afs/cern.ch/work/s/sethzenz/public/Hgg_miniAOD_run0/miniAOD_94.root",
#"file:/afs/cern.ch/work/s/sethzenz/public/Hgg_miniAOD_run0/miniAOD_95.root",
#"file:/afs/cern.ch/work/s/sethzenz/public/Hgg_miniAOD_run0/miniAOD_96.root",
#"file:/afs/cern.ch/work/s/sethzenz/public/Hgg_miniAOD_run0/miniAOD_97.root",
#"file:/afs/cern.ch/work/s/sethzenz/public/Hgg_miniAOD_run0/miniAOD_98.root",
#"file:/afs/cern.ch/work/s/sethzenz/public/Hgg_miniAOD_run0/miniAOD_99.root",
#"file:/afs/cern.ch/work/s/sethzenz/public/Hgg_miniAOD_run0/miniAOD_9.root"
                            ))

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
process.load("flashgg/MicroAODProducers/flashggPreselectedDiPhotons_cfi")
                                                 

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
