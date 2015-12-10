#!/usr/bin/env cmsRun

import FWCore.ParameterSet.Config as cms
import FWCore.Utilities.FileUtils as FileUtils
from flashgg.Systematics.SystematicDumperDefaultVariables import minimalVariables,minimalHistograms,minimalNonSignalVariables,systematicVariables

# SYSTEMATICS SECTION

process = cms.Process("FLASHggSyst")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.load("Configuration.StandardSequences.GeometryDB_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff")
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag.globaltag = '74X_mcRun2_asymptotic_v4' # keep updated for JEC
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(100) )
process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32( 10 )

process.RandomNumberGeneratorService = cms.Service("RandomNumberGeneratorService",
                                                   flashggDiPhotonSystematics = cms.PSet(initialSeed = cms.untracked.uint32(664)),
                                                   flashggElectronSystematics = cms.PSet(initialSeed = cms.untracked.uint32(11)),
                                                   flashggMuonSystematics = cms.PSet(initialSeed = cms.untracked.uint32(13)),
                                                   flashggTagSystematics = cms.PSet(initialSeed = cms.untracked.uint32(999))
                                                  )

process.load("flashgg.Systematics.flashggDiPhotonSystematics_cfi")
process.load("flashgg.Systematics.flashggMuonSystematics_cfi")
process.load("flashgg.Systematics.flashggElectronSystematics_cfi")

from flashgg.Taggers.flashggTags_cff import UnpackedJetCollectionVInputTag
from flashgg.Systematics.flashggJetSystematics_cfi import createJetSystematics
jetSystematicsInputTags = createJetSystematics(process,UnpackedJetCollectionVInputTag)

process.load("flashgg/Taggers/flashggTagSequence_cfi")

# UnpackedJets has to go in front of the systematics producers, and only needs to be run once
# We put it back later
process.flashggTagSequence.remove(process.flashggUnpackedJets)

from PhysicsTools.PatAlgos.tools.helpers import cloneProcessingSnippet,massSearchReplaceAnyInputTag
massSearchReplaceAnyInputTag(process.flashggTagSequence,cms.InputTag("flashggDiPhotons"),cms.InputTag("flashggDiPhotonSystematics"))
massSearchReplaceAnyInputTag(process.flashggTagSequence,cms.InputTag("flashggSelectedElectrons"),cms.InputTag("flashggElectronSystematics"))
massSearchReplaceAnyInputTag(process.flashggTagSequence,cms.InputTag("flashggSelectedMuons"),cms.InputTag("flashggMuonSystematics"))
for i in range(len(UnpackedJetCollectionVInputTag)):
    massSearchReplaceAnyInputTag(process.flashggTagSequence,UnpackedJetCollectionVInputTag[i],jetSystematicsInputTags[i])

process.flashggSystTagMerger = cms.EDProducer("TagMerger",src=cms.VInputTag("flashggTagSorter"))
#process.load("flashgg.Systematics.flashggTagSystematics_cfi")

process.systematicsTagSequences = cms.Sequence()
systlabels = [""]
phosystlabels = []
jetsystlabels = []

# import flashgg customization to check if we have signal or background
from flashgg.MetaData.JobConfig import customize
customize.parse()
print "customize.processId:",customize.processId
# Only run systematics for signal events
if customize.processId.count("h_") or customize.processId.count("vbf_"): # convention: ggh vbf wzh tth
    print "Signal MC, so adding systematics and dZ"
    variablesToUse = minimalVariables
    for direction in ["Up","Down"]:
        phosystlabels.append("MvaShift%s01sigma" % direction)
        phosystlabels.append("SigmaEOverEShift%s01sigma" % direction)
        jetsystlabels.append("JEC%s01sigma" % direction)
        jetsystlabels.append("JER%s01sigma" % direction)
        variablesToUse.append("LooseMvaSF%s01sigma[1,-999999.,999999.] := weight(\"LooseMvaSF%s01sigma\")" % (direction,direction))
        variablesToUse.append("PreselSF%s01sigma[1,-999999.,999999.] := weight(\"PreselSF%s01sigma\")" % (direction,direction))
#        variablesToUse.append("FracRVWeight%s01sigma[1,-999999.,999999.] := weight(\"FracRVWeight%s01sigma\")" % (direction,direction))
        for r9 in ["HighR9","LowR9"]:
#            phosystlabels.append("MCSmear%sEE%s01sigma" % (r9,direction))
#            for var in ["Rho","Phi"]:
#                phosystlabels.append("MCSmear%sEB%s%s01sigma" % (r9,var,direction))
            for region in ["EB","EE"]:
                phosystlabels.append("MCSmear%s%s%s01sigma" % (r9,region,direction))
                phosystlabels.append("MCScale%s%s%s01sigma" % (r9,region,direction))
#                variablesToUse.append("LooseMvaSF%s%s%s01sigma[1,-999999.,999999.] := weight(\"LooseMvaSF%s%s%s01sigma\")" % (r9,region,direction,r9,region,direction))
#                variablesToUse.append("PreselSF%s%s%s01sigma[1,-999999.,999999.] := weight(\"PreselSF%s%s%s01sigma\")" % (r9,region,direction,r9,region,direction))
    systlabels += phosystlabels
    systlabels += jetsystlabels
else:
    print "Data or background MC, so store mgg and central only"
    variablesToUse = minimalNonSignalVariables

print "--- Systematics  with independent collections ---"
print systlabels
print "-------------------------------------------------"
print "--- Variables to be dumped, including systematic weights ---"
print variablesToUse
print "------------------------------------------------------------"

for systlabel in systlabels:
    if systlabel == "":
        continue
    newseq = cloneProcessingSnippet(process,process.flashggTagSequence,systlabel)
    if systlabel in phosystlabels:
        massSearchReplaceAnyInputTag(newseq,cms.InputTag("flashggDiPhotonSystematics"),cms.InputTag("flashggDiPhotonSystematics",systlabel))
    if systlabel in jetsystlabels:    
        for i in range(len(jetSystematicsInputTags)):
            massSearchReplaceAnyInputTag(newseq,jetSystematicsInputTags[i],cms.InputTag(jetSystematicsInputTags[i].moduleLabel,systlabel))
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
#process.source = cms.Source("PoolSource",fileNames = cms.untracked.vstring("/store/group/phys_higgs/cmshgg/sethzenz/flashgg/RunIISpring15-FinalPrompt-BetaV7-25ns/Spring15#BetaV7/DoubleEG/RunIISpring15-FinalPrompt-BetaV7-25ns-Spring15BetaV7-v0-Run2015D-PromptReco-v4/151124_234634/0000/myMicroAODOutputFile_1.root"))


#if options.maxEvents > 0:
#    process.source.eventsToProcess = cms.untracked.VEventRange('1:1-1:'+str(options.maxEvents))

process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string("test.root"))

process.extraDumpers = cms.Sequence()
process.load("flashgg.Taggers.diphotonTagDumper_cfi") ##  import diphotonTagDumper 
import flashgg.Taggers.dumperConfigTools as cfgTools

process.tagsDumper.className = "DiPhotonTagDumper"
process.tagsDumper.src = "flashggSystTagMerger"
#process.tagsDumper.src = "flashggTagSystematics"
process.tagsDumper.processId = "test"
process.tagsDumper.dumpTrees = False
process.tagsDumper.dumpWorkspace = True
process.tagsDumper.dumpHistos = False
process.tagsDumper.quietRooFit = True
process.tagsDumper.nameTemplate = cms.untracked.string("$PROCESS_$SQRTS_$CLASSNAME_$SUBCAT_$LABEL")

tagList=[
["UntaggedTag",5],
["VBFTag",2],
#["VHTightTag",0],
#["VHLooseTag",0],
#["VHEtTag",0],
#["VHHadronicTag",0],
#["TTHHadronicTag",0],
#["TTHLeptonicTag",0]
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
      if systlabel == "":
          currentVariables = variablesToUse
      else:
          currentVariables = systematicVariables
      
      isBinnedOnly = (systlabel !=  "")
      dumpPdfWeights = False #(systlabel ==  "")
      nPdfWeights = 0
      
      cfgTools.addCategory(process.tagsDumper,
                           systlabel,
                           classname=tagName,
                           cutbased=cutstring,
                           subcats=tagCats, 
                           variables=currentVariables,
                           histograms=minimalHistograms,
                           binnedOnly=isBinnedOnly,
                           dumpPdfWeights=dumpPdfWeights,
                           nPdfWeights=nPdfWeights
                           )

process.p = cms.Path((process.flashggDiPhotonSystematics+process.flashggMuonSystematics+process.flashggElectronSystematics)*
                     (process.flashggUnpackedJets*process.jetSystematicsSequence)*
                     (process.flashggTagSequence+process.systematicsTagSequences)*
                     process.flashggSystTagMerger*
#                     process.flashggTagSystematics*
                     process.tagsDumper)

################################
## Dump merged tags to screen ##
################################

#process.load("flashgg/Taggers/flashggTagTester_cfi")
#process.flashggTagTester.TagSorter = cms.InputTag("flashggTagSystematics")
#process.flashggTagTester.TagSorter = cms.InputTag("flashggSystTagMerger")
#process.flashggTagTester.ExpectMultiples = cms.untracked.bool(True)
#process.p += process.flashggTagTester

##############
## Dump EDM ##
##############

#process.out = cms.OutputModule("PoolOutputModule", fileName = cms.untracked.string('CustomizeWillChangeThisAnyway.root'),
#                               outputCommands = cms.untracked.vstring('keep *') # dump everything! small tests only!
#                               )
#process.e = cms.EndPath(process.out)

############################
## Dump the output Python ##
############################
#processDumpFile = open('processDump.py', 'w')
#print >> processDumpFile, process.dumpPython()

# set default options if needed
customize.setDefault("maxEvents",-1)
customize.setDefault("targetLumi",2.46e+3)
# call the customization
customize(process)
