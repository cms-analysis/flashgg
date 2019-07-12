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
                             fileNames = cms.untracked.vstring("root://cms-xrd-global.cern.ch//store/user/spigazzi/flashgg/Era2017_RR-31Mar2018_v2/legacyRun2FullV1/DYJetsToLL_M-50_TuneCP5_13TeV-amcatnloFXFX-pythia8/Era2017_RR-31Mar2018_v2-legacyRun2FullV1-v0-RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/190606_094541/0000/myMicroAODOutputFile_851.root")
)

process.RandomNumberGeneratorService = cms.Service("RandomNumberGeneratorService",
    flashggDifferentialPhoIdInputsCorrection = cms.PSet(
        initialSeed = cms.untracked.uint32(90)
    )
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
                           'leadR9_uncorr := ? leadingPhoton.hasUserFloat("uncorr_r9")>0 ? leadingPhoton.userFloat("uncorr_r9") : -1.',
                           'leadS4 := leadingPhoton.s4',
                           'leadS4_uncorr := ? leadingPhoton.hasUserFloat("uncorr_s4")>0 ? leadingPhoton.userFloat("uncorr_s4") : -1',
                           'leadSieie := leadingPhoton.full5x5_sigmaIetaIeta',
                           'leadSieie_uncorr := ? leadingPhoton.hasUserFloat("uncorr_sieie")>0 ? leadingPhoton.userFloat("uncorr_sieie") : -1',
                           'leadSieip := leadingPhoton.sieip',
                           'leadSieip_uncorr := ? leadingPhoton.hasUserFloat("uncorr_sieip")>0 ? leadingPhoton.userFloat("uncorr_sieip") : -1',
                           'leadEtaWidth := ? leadingPhoton.hasUserFloat("etaWidth")>0 ? leadingPhoton.userFloat("etaWidth") : -1',
                           'leadEtaWidth_uncorr := ? leadingPhoton.hasUserFloat("uncorr_etaWidth")>0 ? leadingPhoton.userFloat("uncorr_etaWidth") : -1',
                           'leadPhiWidth := ? leadingPhoton.hasUserFloat("phiWidth")>0 ? leadingPhoton.userFloat("phiWidth") : -1',
                           'leadPhiWidth_uncorr := ? leadingPhoton.hasUserFloat("uncorr_phiWidth")>0 ? leadingPhoton.userFloat("uncorr_phiWidth") : -1',
                           'leadPhoIso := leadingPhoton.pfPhoIso03',
                           'leadPhoIso_uncorr := ? leadingPhoton.hasUserFloat("uncorr_pfPhoIso03")>0 ? leadingPhoton.userFloat("uncorr_pfPhoIso03") : -1',
                           'subleadEnergy := subLeadingPhoton.p4.energy',
                           'subleadInitialEnergy := subLeadingPhoton.energyAtStep("initial")',
                           'subleadEnergy_corr := subLeadingPhoton.userFloat("afterDiffCorr_regr_E")',
                           'subleadEnergy_uncorr := subLeadingPhoton.userFloat("reco_E")',
                           'subleadPhoId := subLeadingView.phoIdMvaWrtChosenVtx',
                           'subleadPt := subLeadingPhoton.pt',
                           'subleadEta := subLeadingPhoton.superCluster().eta',
                           'subleadPhi := subLeadingPhoton.phi',
                           'subleadR9 := subLeadingPhoton.full5x5_r9',
                           'subleadR9_uncorr := ? subLeadingPhoton.hasUserFloat("uncorr_r9")>0 ? subLeadingPhoton.userFloat("uncorr_r9") : -1',
                           'subleadS4 := subLeadingPhoton.s4',
                           'subleadS4_uncorr := ? subLeadingPhoton.hasUserFloat("uncorr_s4")>0 ? subLeadingPhoton.userFloat("uncorr_s4") : -1',
                           'subleadSieie := subLeadingPhoton.full5x5_sigmaIetaIeta',
                           'subleadSieie_uncorr := ? subLeadingPhoton.hasUserFloat("uncorr_sieie")>0 ? subLeadingPhoton.userFloat("uncorr_sieie") : -1',
                           'subleadSieip := subLeadingPhoton.sieip',
                           'subleadSieip_uncorr := ? subLeadingPhoton.hasUserFloat("uncorr_sieip")>0 ? subLeadingPhoton.userFloat("uncorr_sieip") : -1',
                           'subleadEtaWidth := ? subLeadingPhoton.hasUserFloat("etaWidth")>0 ? subLeadingPhoton.userFloat("etaWidth") : -1',
                           'subleadEtaWidth_uncorr := ? subLeadingPhoton.hasUserFloat("uncorr_etaWidth")>0 ? subLeadingPhoton.userFloat("uncorr_etaWidth") : -1',
                           'subleadPhiWidth := ? subLeadingPhoton.hasUserFloat("phiWidth")>0 ? subLeadingPhoton.userFloat("phiWidth") : -1',
                           'subleadPhiWidth_uncorr := ? subLeadingPhoton.hasUserFloat("uncorr_phiWidth")>0 ? subLeadingPhoton.userFloat("uncorr_phiWidth") : -1',
                           'subleadPhoIso := subLeadingPhoton.pfPhoIso03',
                           'subleadPhoIso_uncorr := ? subLeadingPhoton.hasUserFloat("uncorr_pfPhoIso03")>0 ? subLeadingPhoton.userFloat("uncorr_pfPhoIso03") : -1'
                       ],
                       histograms=[]
)

process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string("test_diff_phoId_corrections.root"))

process.s = cms.Sequence(process.flashggDifferentialPhoIdInputsCorrection+process.dumper)
process.p = cms.Path(process.s)
