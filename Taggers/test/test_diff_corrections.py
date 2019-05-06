import FWCore.ParameterSet.Config as cms
import FWCore.Utilities.FileUtils as FileUtils

process = cms.Process("FLASHggDifferentialCorrectionsTest")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32( 1000 )

process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_mc')
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(10000) )

process.source = cms.Source ("PoolSource",
                             #fileNames = cms.untracked.vstring("root://cms-xrd-global.cern.ch//store/user/spigazzi/flashgg/Era2017_RR-31Mar2018_v1/legacyRun2TestV1/GluGluHToGG_M125_13TeV_amcatnloFXFX_pythia8/Era2017_RR-31Mar2018_v1-legacyRun2TestV1-v0-RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/190319_112256/0000/myMicroAODOutputFile_9.root")
                             fileNames = cms.untracked.vstring("root://cms-xrd-global.cern.ch//store/user/spigazzi/flashgg/Era2017_RR-31Mar2018_v1/legacyRun2TestV1/DYJetsToLL_M-50_TuneCP5_13TeV-amcatnloFXFX-pythia8/Era2017_RR-31Mar2018_v1-legacyRun2TestV1-v0-RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/190319_111412/0000/myMicroAODOutputFile_68.root")
)

from flashgg.Taggers.flashggDifferentialPhoIdInputsCorrection_cfi import *
process.flashggDifferentialPhoIdInputsCorrection = flashggDifferentialPhoIdInputsCorrection.clone()

from flashgg.Taggers.diphotonDumper_cfi import diphotonDumper
import flashgg.Taggers.dumperConfigTools as cfgTools
process.dumper = diphotonDumper
process.dumper.dumpTrees = True
process.dumper.src = "flashggDifferentialPhoIdInputsCorrection"
process.dumper.nameTemplate = "test_$LABEL"
cfgTools.addCategories(process.dumper,
                       [("All", "1", 0)],
                       variables=[
                           'mass',
                           'leadPt := leadingPhoton.pt',
                           'leadEta := leadingPhoton.superCluster().eta',
                           'leadPhi := phi',
                           'leadR9 := leadingPhoton.full5x5_r9',
                           'leadR9_correction := leadingPhoton.userFloat("r9_correction")',
                           'leadR9_uncorr := leadingPhoton.userFloat("uncorr_r9")',
                           'leadS4 := leadingPhoton.s4',
                           'leadS4_correction := leadingPhoton.userFloat("s4_correction")',
                           'leadS4_uncorr := leadingPhoton.userFloat("uncorr_s4")',
                           'leadSieie := leadingPhoton.full5x5_sigmaIetaIeta',
                           'leadSieie_correction := leadingPhoton.userFloat("sieie_correction")',
                           'leadSieie_uncorr := leadingPhoton.userFloat("uncorr_sieie")',
                           'leadSieip := leadingPhoton.sieip',
                           'leadSieip_correction := leadingPhoton.userFloat("sieip_correction")',
                           'leadSieip_uncorr := leadingPhoton.userFloat("uncorr_sieip")',
                           'leadEtaWidth := leadingPhoton.userFloat("etaWidth")',
                           'leadEtaWidth_correction := leadingPhoton.userFloat("etaWidth_correction")',
                           'leadEtaWidth_uncorr := leadingPhoton.userFloat("uncorr_etaWidth")',
                           'leadPhiWidth := leadingPhoton.userFloat("phiWidth")',
                           'leadPhiWidth_correction := leadingPhoton.userFloat("phiWidth_correction")',
                           'leadPhiWidth_uncorr := leadingPhoton.userFloat("uncorr_phiWidth")',
                           'subleadPt := subLeadingPhoton.pt',
                           'subleadEta := subLeadingPhoton.superCluster().eta',
                           'subleadPhi := phi',
                           'subleadR9 := subLeadingPhoton.full5x5_r9',
                           'subleadR9_correction := subLeadingPhoton.userFloat("r9_correction")',
                           'subleadR9_uncorr := subLeadingPhoton.userFloat("uncorr_r9")',
                           'subleadS4 := subLeadingPhoton.s4',
                           'subleadS4_correction := subLeadingPhoton.userFloat("s4_correction")',
                           'subleadS4_uncorr := subLeadingPhoton.userFloat("uncorr_s4")',
                           'subleadSieie := subLeadingPhoton.full5x5_sigmaIetaIeta',
                           'subleadSieie_correction := subLeadingPhoton.userFloat("sieie_correction")',
                           'subleadSieie_uncorr := subLeadingPhoton.userFloat("uncorr_sieie")',
                           'subleadSieip := subLeadingPhoton.sieip',
                           'subleadSieip_correction := subLeadingPhoton.userFloat("sieip_correction")',
                           'subleadSieip_uncorr := subLeadingPhoton.userFloat("uncorr_sieip")',
                           'subleadEtaWidth := subLeadingPhoton.userFloat("etaWidth")',
                           'subleadEtaWidth_correction := subLeadingPhoton.userFloat("etaWidth_correction")',
                           'subleadEtaWidth_uncorr := subLeadingPhoton.userFloat("uncorr_etaWidth")',
                           'subleadPhiWidth := subLeadingPhoton.userFloat("phiWidth")',
                           'subleadPhiWidth_correction := subLeadingPhoton.userFloat("phiWidth_correction")',
                           'subleadPhiWidth_uncorr := subLeadingPhoton.userFloat("uncorr_phiWidth")',
                       ],
                       histograms=[]
)

process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string("test_diff_phoId_corrections.root"))

process.s = cms.Sequence(process.flashggDifferentialPhoIdInputsCorrection+process.dumper)
process.p = cms.Path(process.s)
