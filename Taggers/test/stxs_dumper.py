#!/usr/bin/env cmsRun

import FWCore.ParameterSet.Config as cms
import FWCore.Utilities.FileUtils as FileUtils
import FWCore.ParameterSet.VarParsing as VarParsing
import os
from flashgg.Systematics.SystematicDumperDefaultVariables import minimalVariables,minimalHistograms,minimalNonSignalVariables,systematicVariables
from flashgg.MetaData.MetaConditionsReader import *

# ========================================================================
# SYSTEMATICS SECTION
process = cms.Process("FLASHggSyst")
process.load("FWCore.MessageService.MessageLogger_cfi")
process.load("Configuration.StandardSequences.GeometryDB_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff")
process.maxEvents   = cms.untracked.PSet( input  = cms.untracked.int32( 10 ) )
process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32( 1000 )

systlabels    = [""]
phosystlabels = []
jetsystlabels = []
elesystlabels = []
musystlabels  = []

# import flashgg customization to check if we have signal or background
from flashgg.MetaData.JobConfig import customize
# Register forwardJetRMSCut to be used from customize
customize.options.register('forwardJetRMSCut',
                           3.00,
                           VarParsing.VarParsing.multiplicity.singleton,
                           VarParsing.VarParsing.varType.float,
                           'forwardJetRMSCut')

customize.options.register('pujidWP',
                           'tight', 
                           VarParsing.VarParsing.multiplicity.singleton,
                           VarParsing.VarParsing.varType.string,
                           'pujidWP')

customize.options.register('runOnZee',
                           False,
                           VarParsing.VarParsing.multiplicity.singleton,
                           VarParsing.VarParsing.varType.bool,
                           'runOnZee')

customize.options.register('analysisType',
                           'mainAnalysis',
                           VarParsing.VarParsing.multiplicity.singleton,
                           VarParsing.VarParsing.varType.string,
                           'analysisType'
                           )

customize.options.register('dumpJetSysTrees',
                           False,
                           VarParsing.VarParsing.multiplicity.singleton,
                           VarParsing.VarParsing.varType.bool,
                           'dumpJetSysTrees')
# set default options if needed
customize.setDefault("maxEvents",-1)
customize.setDefault("targetLumi",1.00e+3)
customize.parse()
customize.metaConditions = MetaConditionsReader(customize.metaConditions)

### Global Tag
from Configuration.AlCa.GlobalTag import GlobalTag
if customize.processId == "Data": process.GlobalTag.globaltag = str(customize.metaConditions['globalTags']['data'])
else: process.GlobalTag.globaltag = str(customize.metaConditions['globalTags']['MC'])

#Systematics customize
from flashgg.Systematics.SystematicsCustomize import *
jetSystematicsInputTags = createStandardSystematicsProducers(process,customize)
modifyTagSequenceForSystematics(process,jetSystematicsInputTags,2)

#Using standard tools
useEGMTools(process)

# Load tag sequence
#process.load("flashgg.Taggers.flashggTagSequence_cfi") #FIXME testing this
process.flashggTagSequence.remove(process.flashggUntagged)
process.flashggTagSequence.remove(process.flashggTTHDiLeptonTag)
process.flashggTagSequence.remove(process.flashggTTHLeptonicTag)
process.flashggTagSequence.remove(process.flashggTHQLeptonicTag)
process.flashggTagSequence.remove(process.flashggTTHHadronicTag)
process.flashggTagSequence.remove(process.flashggVHMetTag)
process.flashggTagSequence.remove(process.flashggWHLeptonicTag)
process.flashggTagSequence.remove(process.flashggZHLeptonicTag)
process.flashggTagSequence.remove(process.flashggVHLeptonicLooseTag)
process.flashggTagSequence.remove(process.flashggVHHadronicTag)

#change the tag sorter
process.flashggTagSorter.TagPriorityRanges = cms.VPSet(
    cms.PSet(TagName = cms.InputTag('flashggVBFTag'))
)

#set the prefiring correctly 
applyL1Prefiring = customizeForL1Prefiring(process, customize.metaConditions, customize.processId)

from flashgg.Taggers.flashggVBFMVA_cff import pujidPtBin1_loose, pujidPtBin2_loose, pujidPtBin1_medium, pujidPtBin2_medium, pujidPtBin1_tight, pujidPtBin2_tight 

mva_wp = {
    "none"  : [
        [],
        [],
        []
    ],
    "tight" : [
        pujidPtBin1_tight,
        pujidPtBin2_tight
    ],
    "medium": [
        pujidPtBin1_medium,
        pujidPtBin2_medium
    ],
    "loose" : [
        pujidPtBin1_loose,
        pujidPtBin2_loose
    ]
}

#== Only run systematics for signal events
from flashgg.Taggers.flashggTags_cff import UnpackedJetCollectionVInputTag

# load the correctors
process.load("JetMETCorrections.Configuration.JetCorrectors_cff")

if customize.processId == "Data":
    print "Data, so turn of all shifts and systematics, with some exceptions"
    variablesToUse = minimalNonSignalVariables
    customizeSystematicsForData(process)
else:
    print "Background MC, so store mgg and central only"
    variablesToUse = minimalNonSignalVariables

    if customize.dumpJetSysTrees:
        print "Running jet systematics and putting them in ntuples because doJetSystTrees is set"
        for direction in ["Up","Down"]:
            jetsystlabels.append("JEC%s01sigma" % direction)
            jetsystlabels.append("JER%s01sigma" % direction)
            jetsystlabels.append("PUJIDShift%s01sigma" % direction)
        systlabels += jetsystlabels
        for direction in ["Up","Down"]:
            phosystlabels += ["MvaShift%s01sigma" % direction,
                           "SigmaEOverEShift%s01sigma" % direction
                           ]
        systlabels += phosystlabels
    else:
        print "Background MC, so store mgg and central only"
        customizeSystematicsForBackground(process)

print "--- Turning on central value for UnmatchedPUweight---"
for i in range(len(jetSystematicsInputTags)):
    prodname = 'flashggJetSystematics%i'%i
    vpset = getattr(process,prodname).SystMethods
    for pset in vpset:
        syst = pset.Label.value()
        if syst.count("UnmatchedPUWeight"):
            pset.ApplyCentralValue = False # default to false
            pset.Debug = False

#process.flashggVBFTag.GetQCDWeights = True
#process.flashggVBFTag.GetJetVetoWeights = True
        
print "--- Systematics  with independent collections ---"
print systlabels
print "-------------------------------------------------"
print "--- Variables to be dumped, including systematic weights ---"
print variablesToUse
print "------------------------------------------------------------"

# ========================================================================
# Dumper section
from FWCore.ParameterSet.VarParsing import VarParsing
from flashgg.MetaData.samples_utils import SamplesManager

process.source = cms.Source ("PoolSource",
                             fileNames = cms.untracked.vstring(
'root://xrootd-cms.infn.it//store/user/spigazzi/flashgg/Era2017_RR-31Mar2018_v2/legacyRun2FullV1/GluGluHToGG_M125_13TeV_amcatnloFXFX_pythia8/Era2017_RR-31Mar2018_v2-legacyRun2FullV1-v0-RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/190703_101705/0000/myMicroAODOutputFile_15.root'
                             )
)

process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string("test.root"))

import flashgg.Taggers.dumperConfigTools as cfgTools
from   flashgg.Taggers.tagsDumpers_cfi   import createTagDumper

process.vbfTagDumper = createTagDumper("VBFTag")
process.vbfTagDumper.dumpTrees     = True
process.vbfTagDumper.dumpHistos    = True
process.vbfTagDumper.dumpWorkspace = False
process.vbfTagDumper.src = "flashggSystTagMerger"

# OPTIONS FOR VBF DUMPER
# Use JetID
process.flashggVBFMVA.UseJetID      = cms.bool(True) #fixme
process.flashggVBFMVA.JetIDLevel    = cms.string("Tight2017") #cms.string("Loose")
process.flashggVBFMVA.DrJetPhoton   = cms.double(0.4) # this is the right number
# Relax all selection on VBF tag
process.flashggVBFTag.Boundaries             = cms.vdouble(-2.0,2.0)
process.flashggVBFTag.SetArbitraryNonGoldMC  = cms.bool(False)
process.flashggVBFTag.DropNonGoldData        = cms.bool(False)
process.flashggVBFTag.RequireVBFPreselection = cms.bool(False)
#OPtions for MVA
process.flashggVBFMVA.rmsforwardCut = cms.double(customize.forwardJetRMSCut)
process.flashggVBFMVA.pujidWpPtBin1 = cms.vdouble(mva_wp[customize.pujidWP][0])
process.flashggVBFMVA.pujidWpPtBin2 = cms.vdouble(mva_wp[customize.pujidWP][1])
# Print to user
print '------------------------------------------------------------'
print ' PUJID Working point    ::' , customize.pujidWP
print '------------------------------------------------------------'
print ' PUJID wp cuts          ::' , mva_wp[customize.pujidWP]
print '------------------------------------------------------------'
print ' formward RMS Cut value ::' , customize.forwardJetRMSCut
print '------------------------------------------------------------'
print ' running on Zee         ::' , customize.runOnZee
print '------------------------------------------------------------'


# run on Drell-Yan
if customize.runOnZee:
    process.flashggPreselectedDiPhotons.variables =  cms.vstring('pfPhoIso03',
                                                                 'trkSumPtHollowConeDR03',
                                                                 'full5x5_sigmaIetaIeta',
                                                                 'full5x5_r9',
                                                                 '1-passElectronVeto')

# GET THE VARIABLES TO USE IN THE DUMPER
import flashgg.Taggers.VBFTagVariables as var
new_variables = [
    "n_jet_30             := VBFMVA.n_rec_jets",
    "dijet_jet1_RMS       := leading_rms",
    "dijet_jet2_RMS       := subLeading_rms",
    "dijet_jet1_QGL       := leading_QGL",
    "dijet_jet2_QGL       := subLeading_QGL",
    "dijet_jet1_pujid_mva := leading_pujidMVA()",
    "dijet_jet2_pujid_mva := subleading_pujidMVA()",
    "dipho_pt             := diPhoton.pt",
    "dijet_pt             := VBFMVA.dijet_pt",
]

matching_photon = [
    "dijet_jet1_match := leadingJet_match",
    "dijet_jet2_match := subLeadingJet_match",
    "prompt_pho_1 := diPhoton.leadingPhoton.genMatchType()",
    "prompt_pho_2 := diPhoton.subLeadingPhoton.genMatchType()"
]

cloneTagSequenceForEachSystematic(process,
                                  systlabels=systlabels,
                                  phosystlabels=phosystlabels,
                                  jetsystlabels=jetsystlabels,
                                  jetSystematicsInputTags=jetSystematicsInputTags,
                                  ZPlusJetMode=2)

all_variables = var.dipho_variables + var.dijet_variables + new_variables

if customize.processId != "Data":
    all_variables += matching_photon# + jet_syst_weights
    all_variables += var.stxs_truth_variables

cats = []

if customize.dumpJetSysTrees and customize.processId != "Data" :
    for syst in (jetsystlabels+phosystlabels):
        systcutstring = "hasSyst(\"%s\") "%syst
        cats += [
            ("GeneralDipho_%s"%syst,"%s"%systcutstring,0)]#,
cats += [
    ("GeneralDipho","1",0)
]

cfgTools.addCategories(process.vbfTagDumper,
                       cats,
                       variables  = all_variables,
                       histograms = []
)
process.vbfTagDumper.nameTemplate = "$PROCESS_$SQRTS_$CLASSNAME_$SUBCAT_$LABEL"

filterHLTrigger(process, customize) #FIXME this might need adapting for our purposes

process.options      = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )

# ee bad supercluster filter on data
process.load('RecoMET.METFilters.eeBadScFilter_cfi')
process.eeBadScFilter.EERecHitSource = cms.InputTag("reducedEgamma","reducedEERecHits")

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

# Split out prompt-fake or fake-fake
process.finalFilter = cms.Sequence()
if (customize.processId.count("qcd") or customize.processId.count("gjet")) and customize.processId.count("fake"):
    process.load("flashgg/Systematics/PromptFakeFilter_cfi")
    process.finalFilter += process.PromptFakeFilter
    if (customize.processId.count("promptfake")):
        process.PromptFakeFilter.doPromptFake = cms.bool(True)
        process.PromptFakeFilter.doFakeFake = cms.bool(False)
        process.PromptFakeFilter.doBoth = cms.bool(False)
    elif (customize.processId.count("fakefake")):
        process.PromptFakeFilter.doPromptFake = cms.bool(False)
        process.PromptFakeFilter.doFakeFake = cms.bool(True)
        process.PromptFakeFilter.doBoth = cms.bool(False)
    elif (customize.processId.count("anyfake")):
        process.PromptFakeFilter.doPromptFake = cms.bool(False)
        process.PromptFakeFilter.doFakeFake = cms.bool(False)
        process.PromptFakeFilter.doBoth = cms.bool(True)
    else:
        raise Exception,"Mis-configuration of python for prompt-fake filter"

process.p = cms.Path(process.dataRequirements
                     * process.genFilter
                     #* process.flashggUpdatedIdMVADiPhotons #replaced by version below now...
                     * process.flashggDifferentialPhoIdInputsCorrection
                     * process.flashggDiPhotonSystematics
                     * process.flashggMetSystematics
                     * process.flashggMuonSystematics
                     * process.flashggElectronSystematics
                     * (process.flashggUnpackedJets
                        * process.ak4PFCHSL1FastL2L3CorrectorChain
                        * process.jetSystematicsSequence)
                     * (process.flashggTagSequence
                        + process.systematicsTagSequences)
                     * process.flashggSystTagMerger
                     * process.finalFilter
                     * process.vbfTagDumper
                     )

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

## rerun rivet for stage 1p1 info
if customize.useParentDataset:
    runRivetSequence(process, customize.metaConditions, customize.processId)

# call the customization
customize(process)
