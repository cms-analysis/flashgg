#!/usr/bin/env cmsRun
import importlib

import FWCore.ParameterSet.Config as cms
import FWCore.Utilities.FileUtils as FileUtils
import FWCore.ParameterSet.VarParsing as VarParsing
from flashgg.MetaData.MetaConditionsReader import *


process = cms.Process("Analysis")
process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )
process.options.allowUnscheduled = cms.untracked.bool(True)

process.source = cms.Source ("PoolSource",
                             fileNames = cms.untracked.vstring(
                                 "/store/user/spigazzi/flashgg/Era2016_RR-07Aug17_v1/legacyRun2TestV1/DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/Era2016_RR-07Aug17_v1-legacyRun2TestV1-v0-RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3_ext2-v1/190228_142907/0000/myMicroAODOutputFile_610.root"
                             ))

process.TFileService = cms.Service("TFileService",
fileName = cms.string("test.root"))

process.load("FWCore.MessageService.MessageLogger_cfi")

process.load("Configuration.StandardSequences.GeometryDB_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff")

from flashgg.MetaData.JobConfig import customize
customize.options.register ("trigger",
                            "", # default value
                            VarParsing.VarParsing.multiplicity.singleton, # singleton or list
                            VarParsing.VarParsing.varType.string,          # string, int, or float
                            "trigger")


customize.setDefault("maxEvents",10000)
customize.setDefault("targetLumi",1.e+3)
customize.parse()
customize.metaConditions = MetaConditionsReader(customize.metaConditions)

from Configuration.AlCa.GlobalTag import GlobalTag

if customize.processId == "Data":
    process.GlobalTag.globaltag = str(customize.metaConditions['globalTags']['data'])
else:
    process.GlobalTag.globaltag = str(customize.metaConditions['globalTags']['MC'])

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )
process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32( 1000 )

process.load("flashgg.MicroAOD.flashggDiPhotons_cfi")
process.load("flashgg.Validation.FlashggTagAndProbeProducer_cfi")
process.load("flashgg.Validation.tagAndProbeDumper_cfi")  
process.load("flashgg.Systematics.flashggDiPhotonSystematics_cfi")
process.load("flashgg.Taggers.flashggUpdatedIdMVADiPhotons_cfi")

from flashgg.Validation.FlashggTagAndProbeProducer_cfi import flashggTagAndProbe

import flashgg.Taggers.dumperConfigTools as cfgTools
import flashgg.Validation.tagAndProbeDumperConfig as dumpCfg

from flashgg.Systematics.flashggDiPhotonSystematics_cfi import flashggDiPhotonSystematics

variables = dumpCfg.getDefaultConfig()
dumpCfg.addRegressionInput(variables)

matchTriggerPaths = []
dataTriggers = customize.options.trigger.split(",")
strippedNames = list(map(lambda x: x.rstrip("*"), dataTriggers))
matchTriggerPaths = "&& ".join( map(lambda x: "userInt('%s')" % x, strippedNames) )
print(matchTriggerPaths)
dumpBits = list(strippedNames)

from flashgg.MicroAOD.flashggLeptonSelectors_cff import flashggSelectedElectrons
process.flashggIdentifiedElectrons = flashggSelectedElectrons.clone( 
    src=cms.InputTag("flashggSelectedElectrons"),
    cut=cms.string("passTightId")
)


systModules = cms.VPSet(
        cms.PSet( PhotonMethodName = cms.string("FlashggPhotonHLTMatch"),
                  MethodName = cms.string("FlashggDiPhotonFromPhoton"),
                  Label = cms.string("hltMatch"),
                  NSigmas = cms.vint32(),
                  ApplyCentralValue = cms.bool(True),
                  trgBitsSrc = cms.InputTag("TriggerResults","","HLT"),
                  trgObjectsSrc = cms.InputTag("slimmedPatTrigger"),
                  pathNames = cms.vstring(dumpBits),
                  deltaRmax = cms.double(0.3),
                  ),
        cms.PSet( PhotonMethodName = cms.string("FlashggPhotonEleMatch"),
                  MethodName = cms.string("FlashggDiPhotonFromPhoton"),
                  Label = cms.string("eleMatch"),
                  NSigmas = cms.vint32(),
                  ApplyCentralValue = cms.bool(True),
                  electronsSrc = cms.InputTag("flashggIdentifiedElectrons"),
                  )
)
variables.extend( ["leadEleMatch    := leadingPhoton.hasUserCand('eleMatch')",
                   "subleadEleMatch := subLeadingPhoton.hasUserCand('eleMatch')"
               ] )

# Only EGM tools working atm. photonSmearBins and photonScaleUncertBins missing in flashggDiPhotonSystematics2018_cfi.py
# EGM not working due to missing NSigmas somewhere
sysmodule = importlib.import_module("flashgg.Systematics."+customize.metaConditions["flashggDiPhotonSystematics"])
systModules.append(sysmodule.MCScaleHighR9EB_EGM)
systModules.append(sysmodule.MCScaleLowR9EB_EGM)
systModules.append(sysmodule.MCScaleHighR9EE_EGM)
systModules.append(sysmodule.MCScaleLowR9EE_EGM)
systModules.append(sysmodule.MCScaleGain6EB_EGM)
systModules.append(sysmodule.MCScaleGain1EB_EGM)
systModules.append(sysmodule.MCSmearHighR9EE_EGM)
systModules.append(sysmodule.MCSmearLowR9EE_EGM)
systModules.append(sysmodule.MCSmearHighR9EB_EGM)
systModules.append(sysmodule.MCSmearLowR9EB_EGM)

process.flashggDiPhotonSystematics=flashggDiPhotonSystematics
process.flashggDiPhotonSystematics.SystMethods = systModules

process.flashggTagAndProbe = flashggTagAndProbe
process.flashggTagAndProbe.diphotonsSrc = cms.InputTag("flashggDiPhotonSystematics")
process.flashggTagAndProbe.tagSelection = "{} && pt > 40 && (?hasUserCand('eleMatch')?userCand('eleMatch').passTightId:0) && hasPixelSeed && egChargedHadronIso < 20 && egChargedHadronIso/pt < 0.3".format(matchTriggerPaths)
process.flashggTagAndProbe.probeSelection = "egChargedHadronIso < 20 && egChargedHadronIso/pt < 0.3"
process.flashggTagAndProbe.idSelection = cms.PSet( 
    rho = cms.InputTag("fixedGridRhoAll"),
    cut = cms.string("(full5x5_r9>0.8||egChargedHadronIso<20||egChargedHadronIso/pt<0.3) && egChargedHadronIso < 15"),
    variables = cms.vstring(),
    categories = cms.VPSet( cms.PSet(cut=cms.string("1"), selection=cms.VPSet( cms.VPSet() )) ) )

from flashgg.Validation.tagAndProbeDumper_cfi import tagAndProbeDumper
tagAndProbeDumper.dumpTrees = True
cfgTools.addCategories(tagAndProbeDumper,
                       [
                               ("Reject", "diPhoton.mass < 50 || diPhoton.mass > 130", -1),
                           ("All", "1", 0)
                           # ("EBHighR9", "abs(getProbe.superCluster.eta)<1.4442 && getProbe.full5x5_r9>0.94", 0),
                           # ("EBLowR9", "abs(getProbe.superCluster.eta)<1.4442 && getProbe.full5x5_r9<=0.94", 0),
                           # ("EEHighR9", "abs(getProbe.superCluster.eta)>1.566 && getProbe.full5x5_r9>0.94", 0),
                           # ("EELowR9", "abs(getProbe.superCluster.eta)>1.566 && getProbe.full5x5_r9<=0.94", 0)
                       ],
                       variables=dumpCfg.getTnPVariables(variables),
                       histograms=[]
                   )    
tnp_sequence = cms.Sequence(flashggTagAndProbe+tagAndProbeDumper)
process.p = cms.Path(process.flashggUpdatedIdMVADiPhotons*process.flashggIdentifiedElectrons*process.flashggDiPhotonSystematics*tnp_sequence)

customize(process)
