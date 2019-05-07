#!/usr/bin/env cmsRun

import FWCore.ParameterSet.Config as cms
import FWCore.Utilities.FileUtils as FileUtils
import FWCore.ParameterSet.VarParsing as VarParsing

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
from Configuration.AlCa.GlobalTag import GlobalTag

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )
process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32( 1000 )

from flashgg.MetaData.JobConfig import customize

customize.options.register ('trigger',
                            "", # default value
                            VarParsing.VarParsing.multiplicity.singleton, # singleton or list
                            VarParsing.VarParsing.varType.string,          # string, int, or float
                            "trigger")

customize.setDefault("maxEvents",10000)
customize.setDefault("targetLumi",1.e+3)

process.load("flashgg.Validation.FlashggTagAndProbeProducer_cfi")
process.load("flashgg.Validation.tagAndProbeDumper_cfi")    

from flashgg.Validation.FlashggTagAndProbeProducer_cfi import flashggTagAndProbe

import flashgg.Taggers.dumperConfigTools as cfgTools
import flashgg.Validation.tagAndProbeDumperConfig as dumpCfg

variables = dumpCfg.getDefaultConfig()
dumpCfg.addRegressionInput(variables)


eleSource="flashggSelectedElectrons"

from flashgg.MicroAOD.flashggLeptonSelectors_cff import flashggSelectedElectrons
process.flashggIdentifiedElectrons = flashggSelectedElectrons.clone( 
    src=cms.InputTag(eleSource),
    cut=cms.string("passTightId")
)

variables.extend( ["leadEleMatch    := leadingPhoton.hasUserCand('eleMatch')",
                       "subleadEleMatch := subLeadingPhoton.hasUserCand('eleMatch')"
                   ] )
    # store cut-based IDs
for eid in "Loose", "Medium", "Tight":
    variables.extend( [
        "leadEleIs%s    := ?leadingPhoton.hasUserCand('eleMatch')?leadingPhoton.userCand('eleMatch').pass%sId:0"       % (eid,eid),
        "subleadEleIs%s := ?subLeadingPhoton.hasUserCand('eleMatch')?subLeadingPhoton.userCand('eleMatch').pass%sId:0" % (eid,eid)
    ])

matchTriggerPaths = []
dataTriggers = customize.options.trigger.split(",")
strippedNames = list(map(lambda x: x.rstrip("*"), dataTriggers))
matchTriggerPaths = "&& ".join( map(lambda x: "userInt('%s')" % x, strippedNames) )
print(matchTriggerPaths)

process.flashggTagAndProbe = flashggTagAndProbe
process.flashggTagAndProbe.diphotonsSrc = "kinDiPhotons"
process.flashggTagAndProbe.tagSelection = "%s && pt > 40 && (?hasUserCand('eleMatch')?userCand('eleMatch').passTightId:0) && hasPixelSeed && egChargedHadronIso < 20 && egChargedHadronIso/pt < 0.3" % matchTriggerPaths 
process.flashggTagAndProbe.probeSelection = "hasPixelSeed && egChargedHadronIso < 20 && egChargedHadronIso/pt < 0.3"
# process.flashggTagAndProbe.idSelection = cms.PSet(
#     rho = highMassCiCPhotonsV2.rho,
#     cut = highMassCiCPhotonsV2.cut,        
#     variables = highMassCiCPhotonsV2.variables,
#     categories = highMassCiCPhotonsV2.categories
# ) #This only adds flags to the ntuples to test a particular selection, does not affect the tag or probe selection

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
process.p = cms.Path(tnp_sequence)

customize(process)
