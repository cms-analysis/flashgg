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
targetMass = 130.
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
process.diphotonDumper.systlabel = ""
process.diphotonDumper.nameTemplate = cms.untracked.string("$PROCESS_$SQRTS_$CLASSNAME_$SUBCAT_$SYST")

tagList=[
["UntaggedTag",5],
["VBFTag",3],
["VHTightTag",0],
["VHLooseTag",0],
["VHEtTag",0],
["VHHadronicTag",0],
["TTHHadronicTag",0],
["TTHLeptonicTag",0]
]

for tag in tagList: 
  tagName=tag[0]
  tagCats=tag[1]
  for systlabel in systlabels:
    cutstring = " isTag('%s') && hasSyst(\"%s\") "%(tagName,systlabel)
    cfgTools.addCategory(process.diphotonDumper,
                         "flashgg%s__%s"%(tagName,systlabel),
                         classname="flashgg%s"%tagName,
                         cutbased=cutstring,
                         systlabel=systlabel,
                         subcats=tagCats, 
                         variables = minimalVariables,
                         histograms = minimalHistograms
                         )

  
process.p1 = cms.Path(
    process.diphotonDumper
    )

print process.p1
