#!/usr/bin/env cmsRun
import FWCore.ParameterSet.Config as cms
import FWCore.Utilities.FileUtils as FileUtils
from FWCore.ParameterSet.VarParsing import VarParsing
from flashgg.MetaData.samples_utils import SamplesManager
from flashgg.Systematics.SystematicDumperDefaultVariables import defaultVariables,defaultHistograms

## CMD LINE OPTIONS ##
options = VarParsing('analysis')
print options

processId = "tth"
targetMass = 120.
processId = "%s_%i" % (processId,int(targetMass))

# maxEvents is the max number of events processed of each file, not globally
options.maxEvents = -1
options.inputFiles = "file:myTagOutputFile_%s.root" % processId
options.outputFile = "ValidationTagsDump_%s.root" % processId
options.parseArguments()

## I/O SETUP ##
process = cms.Process("ValidationTagsDumper")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(-1))
process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32( 1 )
process.source = cms.Source ("PoolSource",
                             fileNames = cms.untracked.vstring(options.inputFiles))
process.flashggSystTagMerger = cms.EDProducer("TagMerger",src=cms.VInputTag("flashggTagSorter"))

if options.maxEvents > 0:
    process.source.eventsToProcess = cms.untracked.VEventRange('1:1-1:'+str(options.maxEvents))

process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string(options.outputFile))

process.extraDumpers = cms.Sequence()
systlabels = [""]
counter =0;
for r9 in ["HighR9","LowR9"]:
    for direction in ["Up","Down"]:
        systlabels.append("MCSmear%sEE%s01sigma" % (r9,direction))
        for var in ["Rho","Phi"]:
            systlabels.append("MCSmear%sEB%s%s01sigma" % (r9,var,direction))
        for region in ["EB","EE"]:
            systlabels.append("MCScale%s%s%s01sigma" % (r9,region,direction))

process.extraDumpers = cms.Sequence()
process.load("flashgg.Taggers.diphotonTagDumper_cfi") ##  import diphotonTagDumper 
import flashgg.Taggers.dumperConfigTools as cfgTools

process.diphotonDumper.className = "DiPhotonTagDumper"
process.diphotonDumper.src = "flashggSystTagMerger"
process.diphotonDumper.processId = processId
process.diphotonDumper.dumpTrees = True
process.diphotonDumper.dumpWorkspace = True
process.diphotonDumper.dumpHistos = True
process.diphotonDumper.quietRooFit = True
process.diphotonDumper.systLabel = ""

for systlabel in systlabels:
  cutstring = "hasSyst(\"%s\")"%systlabel
  #print "syst label ", systlabel

  cfgTools.addCategory(process.diphotonDumper,
                       "flashggUntaggedTag__%s"%systlabel,
                       cutbased=cutstring,
                       systLabel=systlabel,
                       subcats=5, 
                       variables = defaultVariables,
                       histograms = defaultHistograms
                       )
  
  cfgTools.addCategory(process.diphotonDumper,
                       "flashggVBFTag__%s"%systlabel,
                       cutbased=cutstring,
                       systLabel=systlabel,
                       subcats=3,
                       variables = defaultVariables + [
          "leadJetPt                :=leadingJet().pt",
          "subleadJetPt             :=subLeadingJet().pt",
          "VBFMVA                   :=VBFMVA().VBFMVAValue()",
          ],
                       histograms = defaultHistograms + [
          "subleadJetPt:leadJetPt>>JetptLeadvsSub(8,20,100:8,20,100)",
          "VBFMVA>>VBFMVA(50,0,1)"
          ]
                       )
  
  cfgTools.addCategory(process.diphotonDumper,
                       "flashggVHTightTag__%s"%systlabel,
                       cutbased=cutstring,
                       systLabel=systlabel,
                       subcats=0,
                       variables = defaultVariables,
                       histograms = defaultHistograms
                       )
  
  cfgTools.addCategory(process.diphotonDumper,
                       "flashggVHLooseTag__%s"%systlabel,
                       cutbased=cutstring,
                       systLabel=systlabel,
                       subcats=0,
                       variables = defaultVariables,
                       histograms = defaultHistograms
                       )
  
  cfgTools.addCategory(process.diphotonDumper,
                       "flashggVHHadronicTag__%s"%systlabel,
                       cutbased=cutstring,
                       systLabel=systlabel,
                       subcats=0,
                       variables = defaultVariables,
                       histograms = defaultHistograms
                       )
  
  cfgTools.addCategory(process.diphotonDumper,
                       "flashggTTHLeptonicTag__%s"%systlabel,
                       cutbased=cutstring,
                       systLabel=systlabel,
                       subcats=0,
                       variables = defaultVariables + [
          "centralWeight := centralWeight",
          "MuonWeightDown01sigma := weight(\"MuonWeightDown01sigma\")",
          "MuonWeightUp01sigma := weight(\"MuonWeightUp01sigma\")",
          "ElectronWeightDown01sigma := weight(\"ElectronWeightDown01sigma\")",
          "ElectronWeightUp01sigma := weight(\"ElectronWeightUp01sigma\")"
          ],
                       histograms = defaultHistograms
                       )
  
  cfgTools.addCategory(process.diphotonDumper,
                       "flashggTTHHadronicTag__%s"%systlabel,
                       cutbased=cutstring,
                       systLabel=systlabel,
                       subcats=0,
                       variables = defaultVariables,
                       histograms = defaultHistograms
                       )
  
process.p1 = cms.Path(
    process.diphotonDumper
    )

print process.p1
