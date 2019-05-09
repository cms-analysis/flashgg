import FWCore.ParameterSet.Config as cms
import FWCore.Utilities.FileUtils as FileUtils

process = cms.Process("FLASHggDifferentialCorrectionsTest")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32( 1000 )

process.load("Configuration.StandardSequences.GeometryDB_cff")

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
process.flashggDifferentialPhoIdInputsCorrection.reRunRegression = True

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
                           'leadEnergy := leadingPhoton.p4.energy',
                           'leadInitialEnergy := leadingPhoton.energyAtStep("initial")',
                           'leadEnergy_corr := leadingPhoton.userFloat("afterDiffCorr_regr_E")',
                           'leadEnergy_uncorr := leadingPhoton.userFloat("reco_E")',
                           'leadPhoId := leadingView.phoIdMvaWrtChosenVtx',
                           'leadPt := leadingPhoton.pt',
                           'leadEta := leadingPhoton.superCluster().eta',
                           'leadPhi := leadingPhoton.phi',
                           'leadR9 := leadingPhoton.full5x5_r9',
                           'leadR9_uncorr := leadingPhoton.userFloat("uncorr_r9")',
                           'leadS4 := leadingPhoton.s4',
                           'leadS4_uncorr := leadingPhoton.userFloat("uncorr_s4")',
                           'leadSieie := leadingPhoton.full5x5_sigmaIetaIeta',
                           'leadSieie_uncorr := leadingPhoton.userFloat("uncorr_sieie")',
                           'leadSieip := leadingPhoton.sieip',
                           'leadSieip_uncorr := leadingPhoton.userFloat("uncorr_sieip")',
                           'leadEtaWidth := leadingPhoton.userFloat("etaWidth")',
                           'leadEtaWidth_uncorr := leadingPhoton.userFloat("uncorr_etaWidth")',
                           'leadPhiWidth := leadingPhoton.userFloat("phiWidth")',
                           'leadPhiWidth_uncorr := leadingPhoton.userFloat("uncorr_phiWidth")',
                           'subleadEnergy := subLeadingPhoton.p4.energy',
                           'subleadInitialEnergy := subLeadingPhoton.energyAtStep("initial")',
                           'subleadEnergy_corr := subLeadingPhoton.userFloat("afterDiffCorr_regr_E")',
                           'subleadEnergy_uncorr := subLeadingPhoton.userFloat("reco_E")',
                           'subleadPhoId := subLeadingView.phoIdMvaWrtChosenVtx',
                           'subleadPt := subLeadingPhoton.pt',
                           'subleadEta := subLeadingPhoton.superCluster().eta',
                           'subleadPhi := subLeadingPhoton.phi',
                           'subleadR9 := subLeadingPhoton.full5x5_r9',
                           'subleadR9_uncorr := subLeadingPhoton.userFloat("uncorr_r9")',
                           'subleadS4 := subLeadingPhoton.s4',
                           'subleadS4_uncorr := subLeadingPhoton.userFloat("uncorr_s4")',
                           'subleadSieie := subLeadingPhoton.full5x5_sigmaIetaIeta',
                           'subleadSieie_uncorr := subLeadingPhoton.userFloat("uncorr_sieie")',
                           'subleadSieip := subLeadingPhoton.sieip',
                           'subleadSieip_uncorr := subLeadingPhoton.userFloat("uncorr_sieip")',
                           'subleadEtaWidth := subLeadingPhoton.userFloat("etaWidth")',
                           'subleadEtaWidth_uncorr := subLeadingPhoton.userFloat("uncorr_etaWidth")',
                           'subleadPhiWidth := subLeadingPhoton.userFloat("phiWidth")',
                           'subleadPhiWidth_uncorr := subLeadingPhoton.userFloat("uncorr_phiWidth")',
                       ],
                       histograms=[]
)

process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string("test_diff_phoId_corrections.root"))

process.s = cms.Sequence(process.flashggDifferentialPhoIdInputsCorrection+process.dumper)
process.p = cms.Path(process.s)
