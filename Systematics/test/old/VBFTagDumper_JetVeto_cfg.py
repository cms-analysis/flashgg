#!/usr/bin/env cmsRun

import FWCore.ParameterSet.Config as cms
import FWCore.Utilities.FileUtils as FileUtils
import os
from flashgg.Systematics.SystematicDumperDefaultVariables import minimalVariables,minimalHistograms,minimalNonSignalVariables,systematicVariables

# SYSTEMATICS SECTION

doJetSystTrees = False # Set to 2 instead of True

process = cms.Process("FLASHggSyst")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.load("Configuration.StandardSequences.GeometryDB_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff")
from Configuration.AlCa.GlobalTag import GlobalTag
if os.environ["CMSSW_VERSION"].count("CMSSW_7_6"):
    process.GlobalTag.globaltag = '76X_mcRun2_asymptotic_v13' # keep updated for JEC
else:
    process.GlobalTag.globaltag = '74X_mcRun2_asymptotic_v4'
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(1000) )
process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32( 1000 )

from flashgg.Systematics.SystematicsCustomize import *
jetSystematicsInputTags = createStandardSystematicsProducers(process)

# Keep an extra category as 'would go elsewhere instead', ignore preselection
process.flashggVBFTag.Boundaries=cms.vdouble(-999.,0.553, 0.902, 0.957)
process.flashggVBFTag.SetArbitraryNonGoldMC = cms.bool(False)
process.flashggVBFTag.DropNonGoldData = cms.bool(False)
process.flashggVBFTag.RequireVBFPreselection = cms.bool(False)

modifyTagSequenceForSystematics(process,jetSystematicsInputTags,doJetSystTrees)

useEGMTools(process)

systlabels = [""]
phosystlabels = []
jetsystlabels = []
elesystlabels = []
musystlabels = []

# import flashgg customization to check if we have signal or background
from flashgg.MetaData.JobConfig import customize
customize.parse()
print "customize.processId:",customize.processId
# Only run systematics for signal events
if customize.processId.count("h_") or customize.processId.count("vbf_"): # convention: ggh vbf wzh (wh zh) tth
    raise Exception,"not really set up for signal or for shifted MCs right now"
elif customize.processId == "Data":
    print "Data, so turn of all shifts and systematics, with some exceptions"
    variablesToUse = minimalNonSignalVariables
    customizeSystematicsForData(process)
else:
    variablesToUse = minimalNonSignalVariables
    if doJetSystTrees:
        print "Running jet systematics and putting them in ntuples because doJetSystTrees is set"
        for direction in ["Up","Down"]:
            jetsystlabels.append("JEC%s01sigma" % direction)
            jetsystlabels.append("JER%s01sigma" % direction)
            jetsystlabels.append("RMSShift%s01sigma" % direction)
        systlabels += jetsystlabels
    else:
        print "Background MC, so store mgg and central only"
        customizeSystematicsForBackground(process)

print "--- Systematics  with independent collections ---"
print systlabels
print "-------------------------------------------------"
print "--- Variables to be dumped, including systematic weights ---"
print variablesToUse
print "------------------------------------------------------------"

cloneTagSequenceForEachSystematic(process,systlabels,phosystlabels,jetsystlabels,jetSystematicsInputTags,doJetSystTrees)

###### Dumper section

from FWCore.ParameterSet.VarParsing import VarParsing
from flashgg.MetaData.samples_utils import SamplesManager

process.source = cms.Source ("PoolSource",
                             fileNames = cms.untracked.vstring(
        #                             "file:myMicroAODOutputFile.root"
        #        "root://eoscms.cern.ch//eos/cms/store/group/phys_higgs/cmshgg/sethzenz/flashgg/RunIISpring15-ReMiniAOD-1_1_0-25ns/1_1_0/VBFHToGG_M-125_13TeV_powheg_pythia8/RunIISpring15-ReMiniAOD-1_1_0-25ns-1_1_0-v0-RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/160105_224017/0000/myMicroAODOutputFile_1.root"
#        "/store/group/phys_higgs/cmshgg/szenz/flashgg/RunIISpring15-ReReco74X-Rerun-1_1_0-25ns/1_2_0/DoubleEG/RunIISpring15-ReReco74X-Rerun-1_1_0-25ns-1_2_0-v0-Run2015D-04Dec2015-v2/160117_214114/0000/myMicroAODOutputFile_10.root"
#        "root://eoscms.cern.ch//eos/cms/store/group/phys_higgs/cmshgg/sethzenz/flashgg/RunIISpring15-ReMiniAOD-1_1_0-25ns/1_1_0/ttHJetToGG_M125_13TeV_amcatnloFXFX_madspin_pythia8/RunIISpring15-ReMiniAOD-1_1_0-25ns-1_1_0-v0-RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/160105_224456/0000/myMicroAODOutputFile_2.root"
        #"root://eoscms.cern.ch//eos/cms//store/group/phys_higgs/cmshgg/sethzenz/flashgg/RunIISpring15-ReMiniAOD-1_1_0-25ns/1_1_0/VHToGG_M120_13TeV_amcatnloFXFX_madspin_pythia8/RunIISpring15-ReMiniAOD-1_1_0-25ns-1_1_0-v0-RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/160105_224138/0000/myMicroAODOutputFile_1.root"
#        "root://eoscms.cern.ch//eos/cms//store/group/phys_higgs/cmshgg/ferriff/flashgg/RunIIFall15DR76-1_3_0-25ns_ext1/1_3_1/DYJetsToLL_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIIFall15DR76-1_3_0-25ns_ext1-1_3_1-v0-RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/160127_112132/0000/myMicroAODOutputFile_156.root"
#"root://eoscms.cern.ch//eos/cms//store/group/phys_higgs/cmshgg/ferriff/flashgg/RunIIFall15DR76-1_3_0-25ns_ext1/1_3_1/GluGluHToGG_M-125_13TeV_powheg_pythia8/RunIIFall15DR76-1_3_0-25ns_ext1-1_3_1-v0-RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/160130_032602/0000/myMicroAODOutputFile_1.root"
#                "/store/group/phys_higgs/cmshgg/ferriff/flashgg/RunIIFall15DR76-1_3_0-25ns_ext1/1_3_1/ttHJetToGG_M120_13TeV_amcatnloFXFX_madspin_pythia8/RunIIFall15DR76-1_3_0-25ns_ext1-1_3_1-v0-RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/160127_024939/0000/myMicroAODOutputFile_1.root"

))

for i in range(1,37):
    process.source.fileNames += ["root://eoscms.cern.ch//eos/cms//store/group/phys_higgs/cmshgg/sethzenz/flashgg/RunIISummer16-2_4_1-25ns_Moriond17/2_4_1/GluGluHToGG_M-125_13TeV_powheg_pythia8/RunIISummer16-2_4_1-25ns_Moriond17-2_4_1-v0-RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/170114_083716/0000/myMicroAODOutputFile_%i.root" % i]

process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string("test.root"))

import flashgg.Taggers.dumperConfigTools as cfgTools
from  flashgg.Taggers.tagsDumpers_cfi import createTagDumper

process.VBFTagDumper = createTagDumper("VBFTag")
process.VBFTagDumper.dumpTrees     = True
process.VBFTagDumper.dumpHistos    = True
process.VBFTagDumper.dumpWorkspace = False

if doJetSystTrees:
    process.VBFTagDumper.src = cms.InputTag("flashggSystTagMerger")

# DY
#process.flashggPreselectedDiPhotons.variables =  cms.vstring('pfPhoIso03', 
#                                                             'trkSumPtHollowConeDR03', 
#                                                             'full5x5_sigmaIetaIeta', 
#                                                             'full5x5_r9', 
#                                                             '1-passElectronVeto')
if doJetSystTrees:
    for syst in jetsystlabels:
        getattr(process,"flashggPreselectedDiPhotons%s"%syst).variables = process.flashggPreselectedDiPhotons.variables

# What we need
#  int category;
#  float weight,leadJPt, subleadJPt, MJJ, dEtaJJ, dPhiJJGammaGamma, zepp;
#  int leadGenJetMatching, subleadGenJetMatching;
#  float leadGenJetPt, subleadGenJetPt, MJJGen, dEtaJJGen, dPhiJJGammaGammaGen, zeppGen;


# get the variable list
import flashgg.Taggers.VBFTagVariables as var
new_variables = [
    "category := categoryNumber"
    ]
matching_photon = [
    "prompt_pho_1   := diPhoton.leadingPhoton.genMatchType()",
    "prompt_pho_2   := diPhoton.subLeadingPhoton.genMatchType()"
    ] 
all_variables = var.dipho_variables + var.dijet_variables + new_variables 
if customize.processId != "Data":
    all_variables += var.truth_variables + matching_photon

cats = []
if doJetSystTrees:
    for syst in jetsystlabels:
        systcutstring = "hasSyst(\"%s\") "%syst
        cats += [("VBF_%s"%syst,systcutstring,0)]

cats += [("VBF","1",0)]

cfgTools.addCategories(process.VBFTagDumper,
                       cats,
                       variables  = all_variables,
                       histograms = []
)

process.VBFTagDumper.nameTemplate = "$PROCESS_$SQRTS_$CLASSNAME_$SUBCAT_$LABEL"

customize.setDefault("maxEvents" ,  5000        ) # max-number of events
customize.setDefault("targetLumi",  1.00e+3  ) # define integrated lumi
customize(process)

from HLTrigger.HLTfilters.hltHighLevel_cfi import hltHighLevel
process.hltHighLevel = hltHighLevel.clone(HLTPaths = cms.vstring("HLT_Diphoton30_18_R9Id_OR_IsoCaloId_AND_HE_R9Id_Mass95_v*") )
#process.hltHighLevel = hltHighLevel.clone(HLTPaths = cms.vstring("HLT_Ele17_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v*") )
process.options      = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )

# ee bad supercluster filter on data
process.load('RecoMET.METFilters.eeBadScFilter_cfi')
process.eeBadScFilter.EERecHitSource = cms.InputTag("reducedEgamma","reducedEERecHits") # Saved MicroAOD Collection (data only)
process.dataRequirements = cms.Sequence()
if customize.processId == "Data":
        process.dataRequirements += process.hltHighLevel
        process.dataRequirements += process.eeBadScFilter

# Split WH and ZH
process.genFilter = cms.Sequence()
if (customize.processId.count("wh") or customize.processId.count("zh")) and not customize.processId.count("wzh"):
    process.load("flashgg/Systematics/VHFilter_cfi")
    process.genFilter += process.VHFilter
    process.VHFilter.chooseW = bool(customize.processId.count("wh"))
    process.VHFilter.chooseZ = bool(customize.processId.count("zh"))

process.p = cms.Path(process.dataRequirements*
                     process.genFilter*
                     process.flashggUpdatedIdMVADiPhotons*
                     process.flashggDiPhotonSystematics*
                     process.flashggMetSystematics*
                     process.flashggMuonSystematics*process.flashggElectronSystematics*
                     (process.flashggUnpackedJets*process.jetSystematicsSequence)*
                     (process.flashggTagSequence*process.systematicsTagSequences)
                     )

if doJetSystTrees:
    process.p += process.flashggSystTagMerger

process.p += process.VBFTagDumper

print "--- Dumping modules that take diphotons as input: ---"
mns = process.p.moduleNames()
for mn in mns:
    module = getattr(process,mn)
    if hasattr(module,"src") and type(module.src) == type(cms.InputTag("")) and module.src.value().count("DiPhoton"):
        print str(module),module.src
    elif hasattr(module,"DiPhotonTag"):
        print str(module),module.DiPhotonTag
print
printSystematicInfo(process)



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
#print process.dumpPython()
#processDumpFile = open('processDump.py', 'w')
#print >> processDumpFile, process.dumpPython()

# set default options if needed
customize.setDefault("maxEvents",1000000)
customize.setDefault("targetLumi",2.61e+3)
# call the customization
customize(process)
