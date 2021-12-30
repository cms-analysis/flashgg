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
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load("Configuration.StandardSequences.GeometryDB_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff")
process.maxEvents   = cms.untracked.PSet( input  = cms.untracked.int32( 10 ) )
process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32( 1 )

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

customize.options.register('doSystematics',
                           False,
                           VarParsing.VarParsing.multiplicity.singleton,
                           VarParsing.VarParsing.varType.bool,
                           'doSystematics')

customize.options.register('ignoreNegR9',
                           True,
                           VarParsing.VarParsing.multiplicity.singleton,
                           VarParsing.VarParsing.varType.bool,
                           'ignoreNegR9')

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
#jetSystematicsInputTags = None
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

process.flashggTagSorter.AddTruthInfo = cms.bool(False)

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

    if customize.doSystematics:
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
'/store/user/spigazzi/flashgg/Era2016_RR-17Jul2018_v2/legacyRun2FullV1/GluGluHToGG_M-125_13TeV_powheg_pythia8/Era2016_RR-17Jul2018_v2-legacyRun2FullV1-v0-RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v1/191111_133402/0000/myMicroAODOutputFile_10.root'
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
# MVA method
process.flashggVBFMVA.MVAMethod = cms.string("DNNMulti")
process.flashggVBFMVA.vbfDNNpbfile = cms.FileInPath("flashgg/Taggers/data/vbfdnn_3classes_smAndCPoddAndL1_2021-09-08.pb")
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

signal_processes = ["ggh_", "vbf_", "wzh_", "wh_", "zh_", "bbh_", "thq_", "thw_",
                    "tth_", "HHTo2B2G", "GluGluHToGG", "VBFHToGG", "VHToGG", "ttHToGG", "Acceptance"]

is_signal = reduce(lambda y, z: y or z, map(
    lambda x: customize.processId.count(x), signal_processes))

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
    "centralObjectWeight  := centralWeight",
]

jet_vars = [
    "gghMVA_n_rec_jets         := GluGluHMVA.n_rec_jets",
    "gghMVA_Mjj                := GluGluHMVA.dijet_Mjj",
    "gghMVA_leadEta            := GluGluHMVA.dijet_leadEta",
    "gghMVA_subleadEta         := GluGluHMVA.dijet_subleadEta",
    "gghMVA_subsubleadEta      := GluGluHMVA.dijet_subsubleadEta",
    "gghMVA_leadPhi            := GluGluHMVA.dijet_leadPhi",
    "gghMVA_subleadPhi         := GluGluHMVA.dijet_subleadPhi",
    "gghMVA_subsubleadPhi      := GluGluHMVA.dijet_subsubleadPhi",
    "gghMVA_leadJPt            := GluGluHMVA.dijet_leadJPt",
    "gghMVA_SubleadJPt         := GluGluHMVA.dijet_SubleadJPt",
    "gghMVA_SubsubleadJPt      := GluGluHMVA.dijet_SubsubleadJPt",
    "gghMVA_leadJEn            := GluGluHMVA.dijet_leadJEn",
    "gghMVA_SubleadJEn         := GluGluHMVA.dijet_SubleadJEn",
    "gghMVA_SubsubleadJEn      := GluGluHMVA.dijet_SubsubleadJEn",
    "gghMVA_leadPUMVA          := GluGluHMVA.dijet_leadPUMVA",
    "gghMVA_subleadPUMVA       := GluGluHMVA.dijet_subleadPUMVA",
    "gghMVA_subsubleadPUMVA    := GluGluHMVA.dijet_subsubleadPUMVA",
    "gghMVA_leadDeltaPhi       := GluGluHMVA.dijet_leadDeltaPhi",
    "gghMVA_subleadDeltaPhi    := GluGluHMVA.dijet_subleadDeltaPhi",
    "gghMVA_subsubleadDeltaPhi := GluGluHMVA.dijet_subsubleadDeltaPhi",
    "gghMVA_leadDeltaEta       := GluGluHMVA.dijet_leadDeltaEta",
    "gghMVA_subleadDeltaEta    := GluGluHMVA.dijet_subleadDeltaEta",
    "gghMVA_subsubleadDeltaEta := GluGluHMVA.dijet_subsubleadDeltaEta",
]

ggH_mva_probs = [
    "ggHMVAResult_prob_0J_PTH_0_10                := GluGluHMVA.ggHMVAResult_prob_0J_PTH_0_10()",
    "ggHMVAResult_prob_0J_PTH_GT10                := GluGluHMVA.ggHMVAResult_prob_0J_PTH_GT10()",
    "ggHMVAResult_prob_1J_PTH_0_60                := GluGluHMVA.ggHMVAResult_prob_1J_PTH_0_60()",
    "ggHMVAResult_prob_1J_PTH_60_120              := GluGluHMVA.ggHMVAResult_prob_1J_PTH_60_120()",
    "ggHMVAResult_prob_1J_PTH_120_200             := GluGluHMVA.ggHMVAResult_prob_1J_PTH_120_200()", 
    "ggHMVAResult_prob_GE2J_MJJ_0_350_PTH_0_60    := GluGluHMVA.ggHMVAResult_prob_GE2J_MJJ_0_350_PTH_0_60()",
    "ggHMVAResult_prob_GE3J_MJJ_0_350_PTH_60_120  := GluGluHMVA.ggHMVAResult_prob_GE2J_MJJ_0_350_PTH_60_120()", 
    "ggHMVAResult_prob_GE2J_MJJ_0_350_PTH_120_200 := GluGluHMVA.ggHMVAResult_prob_GE2J_MJJ_0_350_PTH_120_200()",
    "ggHMVAResult_prob_PTH_GT200                  := GluGluHMVA.ggHMVAResult_prob_PTH_GT200()",
]

VBF_mva_probs = [
    "vbfMvaResult_prob_bkg := VBFMVA.vbfMvaResult_prob_bkg()",
    "vbfMvaResult_prob_ggH := VBFMVA.vbfMvaResult_prob_ggH()",
    "vbfMvaResult_prob_VBF := VBFMVA.vbfMvaResult_prob_VBF()",
    "vbfDnnResult_prob_bkg := VBFMVA.vbfDnnResult_prob_bkg()",
    "vbfDnnResult_prob_ggH := VBFMVA.vbfDnnResult_prob_ggH()",
    "vbfDnnResult_prob_VBF := VBFMVA.vbfDnnResult_prob_VBF()",
]

vh_mva_inputs = [
    "dijet_minDRJetPho    :=  VBFMVA.dijet_minDRJetPho",
    "dijet_centrality_gg  :=  VBFMVA.dijet_centrality_gg",
    "dijet_centrality_j3  :=  VBFMVA.dijet_centrality_j3",
    "dijet_centrality_g   :=  VBFMVA.dijet_centrality_g ",
    "cosThetaStar         :=  VHhadMVA.cosThetaStar",
]

vh_had_probs  = [
    "VH_had_mvascore      := VHhadMVA.VHhadMVAValue()",
]

matching_photon = [
    "dijet_jet1_match := leadingJet_match",
    "dijet_jet2_match := subLeadingJet_match",
    "prompt_pho_1 := diPhoton.leadingPhoton.genMatchType()",
    "prompt_pho_2 := diPhoton.subLeadingPhoton.genMatchType()"
]

if customize.doSystematics:
    cloneTagSequenceForEachSystematic(process,
                                      systlabels=systlabels,
                                      phosystlabels=phosystlabels,
                                      jetsystlabels=jetsystlabels,
                                      jetSystematicsInputTags=jetSystematicsInputTags,
                                      ZPlusJetMode=2)

all_variables = var.dipho_variables + var.dijet_variables + new_variables + jet_vars + ggH_mva_probs + VBF_mva_probs + vh_mva_inputs + vh_had_probs

if customize.processId != "Data":
    all_variables += matching_photon# + jet_syst_weights
    all_variables += var.stxs_truth_variables

cats = []

if customize.doSystematics and customize.processId != "Data" :
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
if customize.processId != "Data":
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

### MELA Generator Stuff ###
melaTable = cms.EDProducer("MELAGenMatrixElementTableProducer",
   lheInfo = cms.VInputTag(cms.InputTag("externalLHEProducer"), cms.InputTag("source")),
   name = cms.string("MELA"),
   year = cms.int32(2018),
   sqrts = cms.double(13.0),
   mH = cms.double(125.0),
   #candVVmode = cms.string("undecayed"),
   #decayVVmode = cms.int32(-1),
   #melaSetup = cms.string("ZH_NLO"),
   mode = cms.string("Decay_gammagamma"),
   normalize = cms.bool(True),
   matrixElements = cms.vstring(),
   #   "Name:SM Couplings:ghz1=1,0;ghw1=1,0",
   #   "Name:SM2 Couplings:ghz1=2,0;ghw1=2,0",
   #   "Name:ghz2a Couplings:ghz1=1,0;ghw1=1,0;ghz2=0.01,0",
   #   "Name:ghz2b Couplings:ghz1=1,0;ghw1=1,0;ghz2=-0.01,0", 
   #   "Name:ghzgs2a Couplings:ghz1=1,0;ghw1=1,0;ghzgs2=0.01,0",
   #   "Name:ghzgs2b Couplings:ghz1=1,0;ghw1=1,0;ghzgs2=-0.01,0",
   #   "Name:ghw2a Couplings:ghz1=1,0;ghw1=1,0;ghw2=0.01,0",
   #   "Name:ghw2b Couplings:ghz1=1,0;ghw1=1,0;ghw2=-0.01,0", 
)

from MelaAnalytics.GenericMEComputer.couplingUtils import *

test = 'test'

bases = "any"
if   "SMEFT"    in test: bases = "Warsaw"
elif "HELatNLO" in test: bases = "HELatNLO"
elif "HEL"      in test: bases = "HEL"


# ==================================================================================
# ================================== HIGGS BASIS ===================================
# ==================================================================================

Higgs_SCAN = byhand_hig_strings([
          ('SM',make_free_higlike(dict())), 
          ('cZA_up', make_free_higlike(dict(cZA=-0.1))),
          ('cZA_dn', make_free_higlike(dict(cZA=+0.1))),
          ('cAA_up', make_free_higlike(dict(cAA=-0.1))),
          ('cAA_dn', make_free_higlike(dict(cAA=+0.1))),
          ('cZZ_up', make_free_higlike(dict(cZZ=0.01))),
          ('cZZ_dn', make_free_higlike(dict(cZZ=-0.01))),
          ('cWW_up', make_free_higlike(dict(cWW=0.01))),
          ('cWW_dn', make_free_higlike(dict(cWW=-0.01))),
          #('cWBox_up', make_free_higlike(dict(cWBox=0.001))),
          #('cWBox_dn', make_free_higlike(dict(cWBox=-0.001))), ])
          ], FlipZG=True)

if bases in ("any", "Higgs"):
    print "Higgs SCAN\n", "\n".join(Higgs_SCAN)

process.melaGenMatrixElementHiggsTable = melaTable.clone(name = "MEWeight_Higgs", matrixElements = Higgs_SCAN)
process.tablesHiggs = cms.Sequence(process.melaGenMatrixElementHiggsTable)

# ==================================================================================
# ================================== SILH HEL BASIS ================================
# ==================================================================================
HEL_points = [
             ('SM',hel_to_higgs(dict())), 
             ('H_up', hel_to_higgs(dict(cH=0.1))),
             ('H_dn', hel_to_higgs(dict(cH=-0.1))),
             ('WWmB_up', hel_to_higgs(dict(cWW=+0.01,cB=-0.01))),
             ('WWmB_dn', hel_to_higgs(dict(cWW=-0.01,cB=+0.01))),
             ('HW_up', hel_to_higgs(dict(cHW=+0.01))),
             ('HW_dn', hel_to_higgs(dict(cHW=-0.01))),
             ('HB_up', hel_to_higgs(dict(cHB=+0.01))),
             ('HB_dn', hel_to_higgs(dict(cHB=-0.01))),
             ('NoBox_up', hel_to_higgs(dict(cHW=+0.01,cHB=-0.01,cWW=-0.01,cB=+0.01))),
             ('NoBox_dn', hel_to_higgs(dict(cHW=-0.01,cHB=+0.01,cWW=+0.01,cB=-0.01))),
             ('A_up', hel_to_higgs(dict(cA=+0.01))),
             ('A_dn', hel_to_higgs(dict(cA=-0.01))),
             ('NoZA_up', hel_to_higgs(dict(cHW=+0.01,cHB=-0.01,cWW=-0.01,cB=+0.01,cA=-0.010703646430953565))),
             ('NoZA_dn', hel_to_higgs(dict(cHW=-0.01,cHB=+0.01,cWW=+0.01,cB=-0.01,cA=+0.010703646430953565))),
             ('NoZZ_up', hel_to_higgs(dict(cHW=+0.01,cHB=-0.01,cWW=-0.01,cB=+0.01,cA=+0.02441992590563888))),
             ('NoZZ_dn', hel_to_higgs(dict(cHW=-0.01,cHB=+0.01,cWW=+0.01,cB=-0.01,cA=-0.02441992590563888))),
            #('WWmBSmall_up', hel_to_higgs(dict(cWW=+0.001,cB=-0.001))),
            #('WWmBSmall_dn', hel_to_higgs(dict(cWW=-0.001,cB=+0.001))),
            #('HWSmall_up', hel_to_higgs(dict(cHW=+0.001))),
            #('HWSmall_dn', hel_to_higgs(dict(cHW=-0.001))),
            #('HBSmall_up', hel_to_higgs(dict(cHB=+0.001))),
            #('HBSmall_dn', hel_to_higgs(dict(cHB=-0.001))),
            #('NoBoxSmall_up', hel_to_higgs(dict(cHW=+0.001,cHB=-0.001,cWW=-0.001,cB=+0.001))),
            #('NoBoxSmall_dn', hel_to_higgs(dict(cHW=-0.001,cHB=+0.001,cWW=+0.001,cB=-0.001))),
            #('ASmall_up', hel_to_higgs(dict(cA=+0.001))),
            #('ASmall_dn', hel_to_higgs(dict(cA=-0.001))),
            #('NoZASmall_up', hel_to_higgs(dict(cHW=+0.001,cHB=-0.001,cWW=-0.001,cB=+0.001,cA=-0.0010703646430953565))),
            #('NoZASmall_dn', hel_to_higgs(dict(cHW=-0.001,cHB=+0.001,cWW=+0.001,cB=-0.001,cA=+0.0010703646430953565))),
            #('NoZZSmall_up', hel_to_higgs(dict(cHW=+0.001,cHB=-0.001,cWW=-0.001,cB=+0.001,cA=+0.002441992590563888))),
            #('NoZZSmall_dn', hel_to_higgs(dict(cHW=-0.001,cHB=+0.001,cWW=+0.001,cB=-0.001,cA=-0.002441992590563888))),
             ]

#HEL_SCAN = byhand_hig_strings(HEL_points, FlipZG=False)
#HELEff_SCAN = byhand_hig_strings(HEL_points, FlipZG=False, SMLoop=True)
HELFlip_SCAN = byhand_hig_strings(HEL_points, FlipZG=True)
HELFlipEff_SCAN = byhand_hig_strings(HEL_points, FlipZG=True, SMLoop=True)
HELNoG_SCAN = byhand_hig_strings(HEL_points, noGamma=True)

if bases in ("any", "HEL"):
    #print "HEL SCAN\n", "\n".join(HEL_SCAN)
    #print "HEL SCAN with Eff SM Loops\n", "\n".join(HELEff_SCAN)
    print "HEL SCAN flipped \n", "\n".join(HELFlip_SCAN)
    print "HEL SCAN flipped with Eff SM Loops\n", "\n".join(HELFlipEff_SCAN)
    print "HEL SCAN with no gamma\n", "\n".join(HELNoG_SCAN)

#process.melaGenMatrixElementHELTable = melaTable.clone( name = "MEWeight_HEL", matrixElements = HEL_SCAN)
#process.melaGenMatrixElementHELEffTable = melaTable.clone( name = "MEWeight_HELEff", matrixElements = HELEff_SCAN)
process.melaGenMatrixElementHELFlipTable = melaTable.clone( name = "MEWeight_HELFlip", matrixElements = HELFlip_SCAN)
process.melaGenMatrixElementHELFlipEffTable = melaTable.clone( name = "MEWeight_HELFlipEff", matrixElements = HELFlipEff_SCAN)
process.melaGenMatrixElementHELNoGTable = melaTable.clone( name = "MEWeight_HELNoG", matrixElements = HELNoG_SCAN)
process.tablesHEL = cms.Sequence(
      #  process.melaGenMatrixElementHELTable 
      #+ process.melaGenMatrixElementHELEffTable 
        process.melaGenMatrixElementHELFlipTable 
      + process.melaGenMatrixElementHELFlipEffTable 
      + process.melaGenMatrixElementHELNoGTable 
)


# ==================================================================================
# ================================== SILH HELatNLO BASIS ===========================
# ==================================================================================
HELatNLO_points = [
        ('SM',(dict())), 
        ('HW_up', dict(cHW=+0.1)),
        ('HW_dn', dict(cHW=-0.1)),
        ('HB_up', dict(cHB=+0.1)),
        ('HB_dn', dict(cHB=-0.1)),
        ('B_up',  dict(cB=+0.1)),
        ('B_dn',  dict(cB=-0.1)),
        ('BB_up', dict(cBB=+0.1)),
        ('BB_dn', dict(cBB=-0.1)),
        ('WW_up', dict(cWW=+0.1)),
        ('WW_dn', dict(cWW=-0.1)),
        ('halfWWpB_up', dict(cWW=+0.05,cB=+0.1)),
        ('halfWWpB_dn', dict(cWW=-0.05,cB=-0.1)),
        ('halfWWmB_up', dict(cWW=+0.05,cB=-0.1)),
        ('halfWWmB_dn', dict(cWW=-0.05,cB=+0.1)),

        ('WWphalfB_up', dict(cWW=+0.1,cB=+0.05)), # no ABox (only ZBox, WBox)
        ('WWphalfB_dn', dict(cWW=-0.1,cB=-0.05)),
        ('WWmhalfB_up', dict(cWW=+0.1,cB=-0.05)),
        ('WWmhalfB_dn', dict(cWW=-0.1,cB=+0.05)),

        ('HW1_up', dict(cHW=+1.)),
        ('HW1_dn', dict(cHW=-1.)),
        ('HB1_up', dict(cHB=+1.)),
        ('HB1_dn', dict(cHB=-1.)),
        ('B1_up',  dict(cB=+1.)),
        ('B1_dn',  dict(cB=-1.)),
        ('BB1_up', dict(cBB=+1.)),
        ('BB1_dn', dict(cBB=-1.)),
        ('WW1_up', dict(cWW=+1.)),
        ('WW1_dn', dict(cWW=-1.)),
        ('WWphalfB1_up', dict(cWW=+1.,cB=+0.5)), # no ABox (only ZBox, WBox)
        ('WWphalfB1_dn', dict(cWW=-1.,cB=-0.5)),
        ('WWmhalfB1_up', dict(cWW=+1.,cB=-0.5)),
        ('WWmhalfB1_dn', dict(cWW=-1.,cB=+0.5)),

        ('HWmWW1_up', dict(cHW=+1,cWW=-1)),  # cWW w/o cWBox
        ('HWmWW1_dn', dict(cHW=-1,cWW=+1)),  # cWW w/o cWBox

        ('WWm1701B_up', dict(cWW=+1,cB=-1.701)),   # ABox-like (tiny cZBox)
        ('WWm1701B_dn', dict(cWW=-1,cB=+1.701)),

        ('WWm16057B_up', dict(cWW=+0.1,cB=-0.16057)),   # ABox-like (tiny cZBox)
        ('WWm16057B_dn', dict(cWW=-0.1,cB=+0.16057)),
        ('WWm16057B1_up', dict(cWW=+1,cB=-1.6057)),   # ABox-like (tiny cZBox)
        ('WWm16057B1_dn', dict(cWW=-1,cB=+1.6057)),
]

HELatNLO_points_fixBox   = [ (k, helatnlo_to_higgs(v, fixBoxes=True )) for (k,v) in HELatNLO_points ]
#HELatNLO_points_noFixBox = [ (k, helatnlo_to_higgs(v, fixBoxes=False)) for (k,v) in HELatNLO_points ]

#HELatNLO_NoG_noFixBox_SCAN = byhand_hig_strings(HELatNLO_points_noFixBox, noGamma=True) # Bad for Wbox in WH
HELatNLO_NoG_fixBox_SCAN = byhand_hig_strings(HELatNLO_points_fixBox, noGamma=True)
#HELatNLO_NoFlip_fixBox_SCAN = byhand_hig_strings(HELatNLO_points_fixBox, FlipZG=False)
HELatNLO_FlipZG_fixBox_SCAN = byhand_hig_strings(HELatNLO_points_fixBox, FlipZG=True)
#HELatNLO_NoFlip_noFixBox_SCAN = byhand_hig_strings(HELatNLO_points_noFixBox, FlipZG=False)
#HELatNLO_FlipZG_noFixBox_SCAN = byhand_hig_strings(HELatNLO_points_noFixBox, FlipZG=True)
HELatNLO_FlipZGEff_fixBox_SCAN = byhand_hig_strings(HELatNLO_points_fixBox, FlipZG=True, SMLoop=True)
#HEL_SCAN = byhand_hig_strings(HEL_points, FlipZG=False)
#HELEff_SCAN = byhand_hig_strings(HEL_points, FlipZG=False, SMLoop=True)
#HELFlip_SCAN = byhand_hig_strings(HEL_points, FlipZG=True)
#HELFlipEff_SCAN = byhand_hig_strings(HEL_points, FlipZG=True, SMLoop=True)

if bases in ("any", "HELatNLO"):
    #print "HELatNLO SCAN (Boxes as per YR4), no gamma\n", "\n".join(HELatNLO_NoG_noFixBox_SCAN)  # BAD for cWbox in WH

    print "HELatNLO SCAN (Boxes as per ArXiv), no gamma\n", "\n".join(HELatNLO_NoG_fixBox_SCAN)
    #print "HELatNLO SCAN (Boxes as per ArXiv), as is\n", "\n".join(HELatNLO_NoFlip_fixBox_SCAN)
    #print "HELatNLO SCAN (Boxes as per YR4), as is\n", "\n".join(HELatNLO_NoFlip_noFixBox_SCAN)
    print "HELatNLO SCAN (Boxes as per ArXiv), flip Zgamma\n", "\n".join(HELatNLO_FlipZG_fixBox_SCAN)
    #print "HELatNLO SCAN (Boxes as per YR4), flip Zgamma\n", "\n".join(HELatNLO_FlipZG_noFixBox_SCAN)
    print "HELatNLO SCAN (Boxes as per ArXiv), flip Zgamma, with Eff SM Loops\n", "\n".join(HELatNLO_FlipZGEff_fixBox_SCAN)
    #print "HELatNLO SCAN with no gamma\n", "\n".join(HELNoG_SCAN)


process.melaGenMatrixElementHELatNLONoGFixTable   = melaTable.clone( name = "MEWeight_HELatNLO_NoG_Fix", matrixElements = HELatNLO_NoG_fixBox_SCAN)
#process.melaGenMatrixElementHELatNLONoGNoFixTable = melaTable.clone( name = "MEWeight_HELatNLO_NoG_NoFix", matrixElements = HELatNLO_NoG_noFixBox_SCAN) # BAD for cWbox in WH
#process.melaGenMatrixElementHELatNLONoFlipFixTable   = melaTable.clone( name = "MEWeight_HELatNLO_NoFlip_Fix", matrixElements = HELatNLO_NoFlip_fixBox_SCAN)
process.melaGenMatrixElementHELatNLOFlipZGFixTable   = melaTable.clone( name = "MEWeight_HELatNLO_FlipZG_Fix", matrixElements = HELatNLO_FlipZG_fixBox_SCAN)
#process.melaGenMatrixElementHELatNLONoFlipNoFixTable   = melaTable.clone( name = "MEWeight_HELatNLO_NoFlip_NoFix", matrixElements = HELatNLO_NoFlip_noFixBox_SCAN)
#process.melaGenMatrixElementHELatNLOFlipZGNoFixTable   = melaTable.clone( name = "MEWeight_HELatNLO_FlipZG_NoFix", matrixElements = HELatNLO_FlipZG_noFixBox_SCAN)
process.melaGenMatrixElementHELatNLOFlipZGEffFixTable   = melaTable.clone( name = "MEWeight_HELatNLO_FlipZGEff_Fix", matrixElements = HELatNLO_FlipZGEff_fixBox_SCAN)
#process.melaGenMatrixElementHELatNLOEffTable = melaTable.clone( name = "MEWeight_HELEff", matrixElements = HELEff_SCAN)
#process.melaGenMatrixElementHELatNLOFlipTable = melaTable.clone( name = "MEWeight_HELFlip", matrixElements = HELFlip_SCAN)
#process.melaGenMatrixElementHELatNLOFlipEffTable = melaTable.clone( name = "MEWeight_HELFlipEff", matrixElements = HELFlipEff_SCAN)
#process.melaGenMatrixElementHELatNLONoGTable = melaTable.clone( name = "MEWeight_HELNoG", matrixElements = HELNoG_SCAN)
process.tablesHELatNLO = cms.Sequence(
    process.melaGenMatrixElementHELatNLONoGFixTable  +
    #process.melaGenMatrixElementHELatNLONoGNoFixTable +
    #process.melaGenMatrixElementHELatNLONoFlipFixTable +
    process.melaGenMatrixElementHELatNLOFlipZGFixTable +
    #process.melaGenMatrixElementHELatNLONoFlipNoFixTable +
    #process.melaGenMatrixElementHELatNLOFlipZGNoFixTable +
    process.melaGenMatrixElementHELatNLOFlipZGEffFixTable 
)


# ==================================================================================
# ================================== WARSAW BASIS ==================================
# ==================================================================================
War_points = [
             ('SM',warsaw_smeftsim_MW_to_higgs(dict())), 
             ('HBox_up', warsaw_smeftsim_MW_to_higgs(dict(cHbox=+1))),
             ('HBox_dn', warsaw_smeftsim_MW_to_higgs(dict(cHbox=-1))),
             ('HWHB_up', warsaw_smeftsim_MW_to_higgs(dict(cHW=+0.1,cHB=+0.1))),
             ('HWHB_dn', warsaw_smeftsim_MW_to_higgs(dict(cHW=-0.1,cHB=-0.1))),
             ('HW_up', warsaw_smeftsim_MW_to_higgs(dict(cHW=+0.1))),
             ('HW_dn', warsaw_smeftsim_MW_to_higgs(dict(cHW=-0.1))),
             ('HB_up', warsaw_smeftsim_MW_to_higgs(dict(cHB=+0.1))),
             ('HB_dn', warsaw_smeftsim_MW_to_higgs(dict(cHB=-0.1))),
             ('NoZZ_up', warsaw_smeftsim_MW_to_higgs(dict(cHW=+0.469333471898234, cHB=-1.6326252710332367))),
             ('NoZZ_dn', warsaw_smeftsim_MW_to_higgs(dict(cHW=-0.469333471898234, cHB=+1.6326252710332367))),
             ('HWtil_up', warsaw_smeftsim_MW_to_higgs(dict(cHWtil=+1))),
             ('HWtil_dn', warsaw_smeftsim_MW_to_higgs(dict(cHWtil=-1))),
             ('HWtil_half', warsaw_smeftsim_MW_to_higgs(dict(cHWtil=+0.5))),
             ('HBtil_up', warsaw_smeftsim_MW_to_higgs(dict(cHBtil=+1))),
             ('HBtil_dn', warsaw_smeftsim_MW_to_higgs(dict(cHBtil=-1))),
             ('HBtil_half', warsaw_smeftsim_MW_to_higgs(dict(cHBtil=+0.5))),
             ('HWBtil_up', warsaw_smeftsim_MW_to_higgs(dict(cHWBtil=+1))),
             ('HWBtil_dn', warsaw_smeftsim_MW_to_higgs(dict(cHWBtil=-1))),
             ('HWBtil_half', warsaw_smeftsim_MW_to_higgs(dict(cHWBtil=+0.5))),
             ]
War_points += [
             ('HWHBBig_up',warsaw_smeftsim_MW_to_higgs(dict(cHW=+0.5,cHB=+0.5))),
             ('HWHBBig_dn', warsaw_smeftsim_MW_to_higgs(dict(cHW=-0.5,cHB=-0.5))),
             ('HWBig_up', warsaw_smeftsim_MW_to_higgs(dict(cHW=+0.5))),
             ('HWBig_dn', warsaw_smeftsim_MW_to_higgs(dict(cHW=-0.5))),
             ('HBBig_up', warsaw_smeftsim_MW_to_higgs(dict(cHB=+0.5))),
             ('HBBig_dn', warsaw_smeftsim_MW_to_higgs(dict(cHB=-0.5))),
             ('HWHBSmall_up', warsaw_smeftsim_MW_to_higgs(dict(cHW=+0.02,cHB=+0.02))),
             ('HWHBSmall_dn', warsaw_smeftsim_MW_to_higgs(dict(cHW=-0.02,cHB=-0.02))),
             ('HWSmall_up', warsaw_smeftsim_MW_to_higgs(dict(cHW=+0.02))),
             ('HWSmall_dn', warsaw_smeftsim_MW_to_higgs(dict(cHW=-0.02))),
             ('HBSmall_up', warsaw_smeftsim_MW_to_higgs(dict(cHB=+0.02))),
             ('HBSmall_dn', warsaw_smeftsim_MW_to_higgs(dict(cHB=-0.02))),
             ('NoZZSmall_up', warsaw_smeftsim_MW_to_higgs(dict(cHW=+0.2*0.469333471898234, cHB=-0.2*1.6326252710332367))),
             ('NoZZSmall_dn', warsaw_smeftsim_MW_to_higgs(dict(cHW=-0.2*0.469333471898234, cHB=+0.2*1.6326252710332367))),
]

#War_SCAN = byhand_hig_strings(War_points)
#WarEff_SCAN = byhand_hig_strings(War_points, SMLoop=True)
WarFlip_SCAN = byhand_hig_strings(War_points, FlipZG=True)
WarFlipEff_SCAN = byhand_hig_strings(War_points, FlipZG=True, SMLoop=True)
#WarFlipEffF1_SCAN = byhand_hig_strings(War_points, FlipZG=True, SMLoop="flipGG")
#WarFlipEffF2_SCAN = byhand_hig_strings(War_points, FlipZG=True, SMLoop="flipZG")
#WarFlipGG_SCAN = byhand_hig_strings(War_points, FlipZG=True, FlipGG=True)
#WarFlipGGEff_SCAN = byhand_hig_strings(War_points, FlipZG=True, FlipGG=True, SMLoop=True)
WarNoG_SCAN = byhand_hig_strings(War_points, noGamma=True)

if bases in ("any", "Warsaw"):
    print "Warsaw SCAN\n", "\n".join(WarFlip_SCAN)
    print "Warsaw SCAN with Eff SM Loops\n", "\n".join(WarFlipEff_SCAN)
    #print "Warsaw SCAN (GG flip)\n", "\n".join(WarFlipGG_SCAN)
    #print "Warsaw SCAN (GG flip) with Eff SM Loops\n", "\n".join(WarFlipGGEff_SCAN)
    print "Warsaw SCAN with no gammas\n", "\n".join(WarNoG_SCAN)

#process.melaGenMatrixElementWarTable = melaTable.clone( name = "MEWeight_War", matrixElements = War_SCAN)
#process.melaGenMatrixElementWarEffTable = melaTable.clone( name = "MEWeight_WarEff", matrixElements = WarEff_SCAN)
process.melaGenMatrixElementWarFlipTable = melaTable.clone( name = "MEWeight_WarFlip", matrixElements = WarFlip_SCAN)
process.melaGenMatrixElementWarFlipEffTable = melaTable.clone( name = "MEWeight_WarFlipEff", matrixElements = WarFlipEff_SCAN)
#process.melaGenMatrixElementWarFlipEffF1Table = melaTable.clone( name = "MEWeight_WarFlipEffF1", matrixElements = WarFlipEffF1_SCAN)
#process.melaGenMatrixElementWarFlipEffF2Table = melaTable.clone( name = "MEWeight_WarFlipEffF2", matrixElements = WarFlipEffF2_SCAN)
#process.melaGenMatrixElementWarFlipGGTable = melaTable.clone( name = "MEWeight_WarFlipGG", matrixElements = WarFlipGG_SCAN)
#process.melaGenMatrixElementWarFlipGGEffTable = melaTable.clone( name = "MEWeight_WarFlipGGEff", matrixElements = WarFlipGGEff_SCAN)
process.melaGenMatrixElementWarNoGTable = melaTable.clone( name = "MEWeight_WarNoG", matrixElements = WarNoG_SCAN)
process.tablesWarsaw = cms.Sequence(
      #  process.melaGenMatrixElementWarTable
      #  process.melaGenMatrixElementWarEffTable +
        process.melaGenMatrixElementWarFlipTable 
      + process.melaGenMatrixElementWarFlipEffTable 
      #+ process.melaGenMatrixElementWarFlipEffF1Table 
      #+ process.melaGenMatrixElementWarFlipEffF2Table 
      #+ process.melaGenMatrixElementWarFlipGGTable 
      #+ process.melaGenMatrixElementWarFlipGGEffTable 
      + process.melaGenMatrixElementWarNoGTable 
)


# ==================================================================================
# ================================== COMMON ========================================
# ==================================================================================

if bases == "Warsaw":
    process.tables = cms.Sequence( process.tablesWarsaw )
elif bases == "Higgs":
    process.tables = cms.Sequence( process.tablesHiggs )
elif bases == "HEL":
    process.tables = cms.Sequence( process.tablesHEL )
elif bases == "HELatNLO":
    process.tables = cms.Sequence( process.tablesHELatNLO )
elif bases == "any":
    process.tables = cms.Sequence( process.tablesWarsaw + process.tablesHiggs + process.tablesHELatNLO )
else:
    raise RuntimeError("Unsupported bases %r" % bases)

# # Decay
# if "Decay" in melaTable.mode.value():
#     process.higgsDecayTable = cms.EDProducer("HiggsDecayLevelVariablesProducer",
#            genParticles = 'genParticles'
#     )
#     process.tables += process.higgsDecayTable
#     #process.maxEvents.input = 20



# Save the LHE information to make a-posteriori re-weighting
process.lheInfosSeq = cms.Sequence()
if customize.processId != "Data":
    if is_signal: 
        print '-------------------------------------------------------------'
        print ' Running on signal, so adding the sequence to store LHE info '
        customize.options.useParentDataset = True
        process.load("PhysicsTools.NanoAOD.nano_cff")
        process.lheInfosSeq += process.genParticleSequence
        process.lheInfosSeq += process.particleLevelSequence
        process.lheInfosSeq += process.lheInfoTable
        process.vbfTagDumper.globalVariables.dumpLHEInfo = True 
        print '-------------------------------------------------------------'

    melaTables=True
    if melaTables:
        process.lheInfosSeq += process.tables


process.p = cms.Path(process.dataRequirements
                     * process.genFilter
                     * process.lheInfosSeq
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
if customize.doSystematics:
    printSystematicInfo(process)

## rerun rivet for stage 1p1 info
#if customize.useParentDataset and not customize.processId == "Data":
#    runRivetSequence(process, customize.metaConditions, customize.processId)

# call the customization
customize(process)
