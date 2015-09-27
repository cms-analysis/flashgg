#!/usr/bin/env cmsRun

import FWCore.ParameterSet.Config as cms
import FWCore.Utilities.FileUtils as FileUtils
from flashgg.Systematics.SystematicDumperDefaultVariables import minimalVariables,minimalHistograms

# SYSTEMATICS SECTION

process = cms.Process("FLASHggSyst")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.load("Configuration.StandardSequences.GeometryDB_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.GlobalTag.globaltag = 'POSTLS170_V5::All'
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(100) )
process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32( 10 )

process.RandomNumberGeneratorService = cms.Service("RandomNumberGeneratorService",
                                                   flashggDiPhotonSystematics = cms.PSet(initialSeed = cms.untracked.uint32(664)),
                                                   flashggElectronSystematics = cms.PSet(initialSeed = cms.untracked.uint32(11)),
                                                   flashggMuonSystematics = cms.PSet(initialSeed = cms.untracked.uint32(13))
                                                  )

process.load("flashgg.Systematics.flashggDiPhotonSystematics_cfi")
process.load("flashgg.Systematics.flashggMuonSystematics_cfi")
process.load("flashgg.Systematics.flashggElectronSystematics_cfi")
process.load("flashgg/Taggers/flashggTagSequence_cfi")
process.load("flashgg/Taggers/flashggTagTester_cfi")

from PhysicsTools.PatAlgos.tools.helpers import cloneProcessingSnippet,massSearchReplaceAnyInputTag
massSearchReplaceAnyInputTag(process.flashggTagSequence,cms.InputTag("flashggDiPhotons"),cms.InputTag("flashggDiPhotonSystematics"))
massSearchReplaceAnyInputTag(process.flashggTagSequence,cms.InputTag("flashggSelectedElectrons"),cms.InputTag("flashggElectronSystematics"))
massSearchReplaceAnyInputTag(process.flashggTagSequence,cms.InputTag("flashggSelectedMuons"),cms.InputTag("flashggMuonSystematics"))

process.flashggSystTagMerger = cms.EDProducer("TagMerger",src=cms.VInputTag("flashggTagSorter"))

process.systematicsTagSequences = cms.Sequence()
systlabels = [""]
for r9 in ["HighR9","LowR9"]:
    for direction in ["Up","Down"]:
        systlabels.append("MCSmear%sEE%s01sigma" % (r9,direction))
        for var in ["Rho","Phi"]:
            systlabels.append("MCSmear%sEB%s%s01sigma" % (r9,var,direction))
        for region in ["EB","EE"]:
            systlabels.append("MCScale%s%s%s01sigma" % (r9,region,direction))

for systlabel in systlabels:
    if systlabel == "":
        continue
    newseq = cloneProcessingSnippet(process,process.flashggTagSequence,systlabel)
    massSearchReplaceAnyInputTag(newseq,cms.InputTag("flashggDiPhotonSystematics"),cms.InputTag("flashggDiPhotonSystematics",systlabel))
    for name in newseq.moduleNames():
        module = getattr(process,name)
        if hasattr(module,"SystLabel"):
            module.SystLabel = systlabel
    process.systematicsTagSequences += newseq
    process.flashggSystTagMerger.src.append(cms.InputTag("flashggTagSorter" + systlabel))

###### Dumper section

from FWCore.ParameterSet.VarParsing import VarParsing
from flashgg.MetaData.samples_utils import SamplesManager

## CMD LINE OPTIONS ##
#options = VarParsing('analysis')
#print options

# maxEvents is the max number of events processed of each file, not globally
#options.maxEvents = -1
#options.inputFiles = "file:myMicroAODOutputFile.root"
#options.outputFile = "ValidationTagsDump.root"
#options.parseArguments()

process.source = cms.Source ("PoolSource",
                             fileNames = cms.untracked.vstring("file:myMicroAODOutputFile.root"))

#if options.maxEvents > 0:
#    process.source.eventsToProcess = cms.untracked.VEventRange('1:1-1:'+str(options.maxEvents))

process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string("test.root"))

process.extraDumpers = cms.Sequence()
process.load("flashgg.Taggers.diphotonTagDumper_cfi") ##  import diphotonTagDumper 
import flashgg.Taggers.dumperConfigTools as cfgTools

process.tagsDumper.className = "DiPhotonTagDumper"
process.tagsDumper.src = "flashggSystTagMerger"
process.tagsDumper.processId = "test"
process.tagsDumper.dumpTrees = True
process.tagsDumper.dumpWorkspace = True
process.tagsDumper.dumpHistos = False
process.tagsDumper.quietRooFit = True
process.tagsDumper.nameTemplate = cms.untracked.string("$PROCESS_$SQRTS_$CLASSNAME_$SUBCAT_$LABEL")

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

definedSysts=set()
process.tagsDumper.classifierCfg.remap=cms.untracked.VPSet()
for tag in tagList: 
  tagName=tag[0]
  tagCats=tag[1]
  # remap return value of class-based classifier
  process.tagsDumper.classifierCfg.remap.append( cms.untracked.PSet( src=cms.untracked.string("flashgg%s"%tagName), dst=cms.untracked.string(tagName) ) )
  for systlabel in systlabels:
      if not systlabel in definedSysts:
          # the cut corresponding to the systematics can be defined just once
          cutstring = "hasSyst(\"%s\") "%(systlabel)
          definedSysts.add(systlabel)
      else:
          cutstring = None
      cfgTools.addCategory(process.tagsDumper,
                           systlabel,
                           classname=tagName,
                           cutbased=cutstring,
                           subcats=tagCats, 
                           variables=minimalVariables,
                           histograms=minimalHistograms
                           )

process.p = cms.Path((process.flashggDiPhotonSystematics+process.flashggMuonSystematics+process.flashggElectronSystematics)*
                     (process.flashggTagSequence+process.systematicsTagSequences)*
                     process.flashggSystTagMerger
                     * process.tagsDumper)



############################
## Dump the output Python ##
############################
processDumpFile = open('processDump.py', 'w')
print >> processDumpFile, process.dumpPython()



# import flashgg customization
from flashgg.MetaData.JobConfig import customize
# set default options if needed
customize.setDefault("maxEvents",-1)
customize.setDefault("targetLumi",20e+3)
# call the customization
customize(process)
