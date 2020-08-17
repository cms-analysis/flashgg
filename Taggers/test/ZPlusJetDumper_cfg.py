#!/usr/bin/env cmsRun

import FWCore.ParameterSet.Config as cms
import FWCore.Utilities.FileUtils as FileUtils
import FWCore.ParameterSet.VarParsing as VarParsing
import os
from flashgg.Systematics.SystematicDumperDefaultVariables import minimalVariables,minimalHistograms,minimalNonSignalVariables,systematicVariables

# SYSTEMATICS SECTION
process = cms.Process("FLASHggSyst")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.load("Configuration.StandardSequences.GeometryDB_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff")
from Configuration.AlCa.GlobalTag import GlobalTag
if os.environ["CMSSW_VERSION"].count("CMSSW_7_6"):
    process.GlobalTag.globaltag = '76X_mcRun2_asymptotic_v12'
elif os.environ["CMSSW_VERSION"].count("CMSSW_7_4"):
    process.GlobalTag.globaltag = '74X_mcRun2_asymptotic_v4'
elif os.environ["CMSSW_VERSION"].count("CMSSW_8_0"):
    process.GlobalTag.globaltag = '80X_mcRun2_asymptotic_2016_miniAODv2'
elif os.environ["CMSSW_VERSION"].count("CMSSW_8_0_25"):
    process.GlobalTag.globaltag = '80X_mcRun2_asymptotic_2016_TrancheIV_v6'    
else:
    raise Exception,"Could not find a sensible CMSSW_VERSION for default globaltag"
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(1000) )
process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32( 1000 )

# import flashgg customization to check if we have signal or background
from flashgg.MetaData.JobConfig import customize
customize.options.register('doJetSystTrees',
                           False,
                           VarParsing.VarParsing.multiplicity.singleton,
                           VarParsing.VarParsing.varType.string,
                           'doJetSystTrees')
customize.parse()
print "customize.processId:",customize.processId


from flashgg.Systematics.SystematicsCustomize import *
jetSystematicsInputTags = createStandardSystematicsProducers(process)
process.flashggTagSequence=cms.Sequence(process.flashggPreselectedDiPhotons*process.flashggDiPhotonMVA*process.flashggZPlusJetTag)
modifyTagSequenceForSystematics(process,jetSystematicsInputTags,customize.doJetSystTrees)

systlabels     = [""]
phosystlabels  = []
jetsystlabels_  = []
elesystlabels  = []
musystlabels   = []

useEGMTools(process)
# Only run systematics for signal events
if customize.processId.count("h_") or customize.processId.count("vbf_"): # convention: ggh vbf wzh (wh zh) tth
    raise Exception,"not really set up for signal or for shifted MCs right now"
elif customize.processId == "Data":
    print "Data, so turn of all shifts and systematics, with some exceptions"
    variablesToUse = minimalNonSignalVariables
    customizeSystematicsForData(process)
else:
    variablesToUse = minimalNonSignalVariables
    if customize.doJetSystTrees:
        print "Running jet systematics and putting them in ntuples because doJetSystTrees is set"
        for direction in ["Up","Down"]:
            jetsystlabels_.append("JEC%s01sigma" % direction)
            jetsystlabels_.append("JER%s01sigma" % direction)
            #jetsystlabels_.append("RMSShift%s01sigma" % direction) # no need for the RMS shift for Moriond17
        systlabels += jetsystlabels_
    else:
        print "Background MC, so store mgg and central only"
        customizeSystematicsForBackground(process)

print "--- Systematics  with independent collections ---"
print systlabels
print type(systlabels)
print "jetsystlabels_      :: ", jetsystlabels_
print "jetsystlabels_ type ::", type(jetsystlabels_)
print "-------------------------------------------------"
print "--- Variables to be dumped, including systematic weights ---"
print variablesToUse
print type(variablesToUse)
print "------------------------------------------------------------"

cloneTagSequenceForEachSystematic(process,systlabels=systlabels,
                                  phosystlabels=phosystlabels,
                                  jetsystlabels=jetsystlabels_,
                                  jetSystematicsInputTags=jetSystematicsInputTags,
                                  ZPlusJetMode=customize.doJetSystTrees)

###### Dumper section
print "jet tag :: ", process.jec.toGet[0].tag
print "process.jec.connect :: ", process.jec.connect
if customize.processId == "Data":
    process.jec.connect = cms.string('sqlite_file:%s/src/flashgg/Systematics/data/JEC/Spring16_23Sep2016AllV2_DATA.db' % environ['CMSSW_BASE'])
    process.jec.toGet[0].tag = cms.string('JetCorrectorParametersCollection_Spring16_23Sep2016AllV2_DATA_AK4PFchs')

from FWCore.ParameterSet.VarParsing import VarParsing
from flashgg.MetaData.samples_utils import SamplesManager

process.source = cms.Source ("PoolSource",
                             fileNames = cms.untracked.vstring(
                                 #"/store/group/phys_higgs/cmshgg/sethzenz/flashgg/RunIISummer16-2_4_1-25ns_Moriond17/2_4_1/DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/RunIISummer16-2_4_1-25ns_Moriond17-2_4_1-v0-RunIISummer16MiniAODv2-PUMoriond17_HCALDebug_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/170114_082421/0000/myMicroAODOutputFile_9.root"
                                 "/store/group/phys_higgs/cmshgg/ferriff/flashgg/RunIISpring16DR80X-2_3_0-25ns_Moriond17_MiniAODv2/2_3_0/DoubleEG/RunIISpring16DR80X-2_3_0-25ns_Moriond17_MiniAODv2-2_3_0-v0-Run2016D-23Sep2016-v1/161117_082833/0000/myMicroAODOutputFile_99.root"
                             )
)

process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string("test.root"))

import flashgg.Taggers.dumperConfigTools as cfgTools
from  flashgg.Taggers.tagsDumpers_cfi import createTagDumper

process.ZPlusJetTagDumper = createTagDumper("ZPlusJetTag")
process.ZPlusJetTagDumper.dumpTrees     = True
process.ZPlusJetTagDumper.dumpHistos    = True
process.ZPlusJetTagDumper.dumpWorkspace = False

if customize.doJetSystTrees:
    process.ZPlusJetTagDumper.src = cms.InputTag("flashggSystTagMerger")

# DY
process.flashggPreselectedDiPhotons.variables =  cms.vstring('pfPhoIso03', 
                                                             'trkSumPtHollowConeDR03', 
                                                             'full5x5_sigmaIetaIeta', 
                                                             'full5x5_r9', 
                                                             '1-passElectronVeto')
if customize.doJetSystTrees:
    for syst in jetsystlabels_:
        getattr(process,"flashggPreselectedDiPhotons%s"%syst).variables = process.flashggPreselectedDiPhotons.variables
print "after something ... ?? "
print "jetsystlabels_      :: ", jetsystlabels_
print "jetsystlabels_ type ::", type(jetsystlabels_)

# get the variable list
import flashgg.Taggers.VBFTagVariables as var
new_variables = [
    "n_jets                     := nJets",
    "jetPt                      := jetPt",
    "jetEta                     := jetEta",
    "jetPhi                     := jetPhi",
    "jet_HFHadronEnergyFraction := jet_HFHadronEnergyFraction",
    "jet_HFHadronEnergy         := jet_HFHadronEnergy",
    "jet_HFHadronMultiplicity   := jet_HFHadronMultiplicity",
    "jet_HFEMEnergyFraction     := jet_HFEMEnergyFraction",
    "jet_HFEMEnergy             := jet_HFEMEnergy",
    "jet_HFEMMultiplicity       := jet_HFEMMultiplicity",
    "jet_rms                    := jet_rms", 
    "jet_QGL                    := jet_QGL", 
    "jet_rawPt                  := jet_rawPt", 
    "zMass                      := zMass",
    "zPt                        := zPt",
    "zEta                       := zEta",
    "zPhi                       := zPhi",
    "deltaPhiZJet               := deltaPhiZJet",
    "smartIndex                 := smartIndex",
    "vtxZCoord                  := vtxZCoord",
    "jet_match      := jet_match"
    ]
matching_photon = [
    "prompt_pho_1   := diPhoton.leadingPhoton.genMatchType()",
    "prompt_pho_2   := diPhoton.subLeadingPhoton.genMatchType()"
    ] 
all_variables = var.dipho_variables + new_variables 
if customize.processId != "Data":
#    all_variables += var.truth_variables + matching_photon
    all_variables += matching_photon

cats = []
if customize.doJetSystTrees:
    for syst in jetsystlabels_:
        systcutstring = "hasSyst(\"%s\") "%syst
        cats += [("ZPlusJet_%s"%syst,systcutstring,0)]

cats += [("ZPlusJet","1",0)]

cfgTools.addCategories(process.ZPlusJetTagDumper,
                       cats,
                       variables  = all_variables,
                       histograms = []
)

process.ZPlusJetTagDumper.nameTemplate = "$PROCESS_$SQRTS_$CLASSNAME_$SUBCAT_$LABEL"

customize.setDefault("maxEvents" ,  5000        ) # max-number of events
customize.setDefault("targetLumi",  1.00e+3  ) # define integrated lumi
customize(process)

from HLTrigger.HLTfilters.hltHighLevel_cfi import hltHighLevel
#process.hltHighLevel = hltHighLevel.clone(HLTPaths = cms.vstring("HLT_Diphoton30_18_R9Id_OR_IsoCaloId_AND_HE_R9Id_Mass95_v*") )
#process.hltHighLevel = hltHighLevel.clone(HLTPaths = cms.vstring("HLT_Ele17_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v*") )
#process.hltHighLevel = hltHighLevel.clone(HLTPaths = cms.vstring("HLT_Ele27_eta2p1_WPLoose_Gsf_v*") )
#process.hltHighLevel = hltHighLevel.clone(HLTPaths = cms.vstring("HLT_Ele27WPTight_v*") )
#process.options      = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )

# Logic from Yacine's file
#from HLTrigger.HLTfilters.hltHighLevel_cfi import hltHighLevel
if customize.processId == "Data":
    process.hltHighLevel = hltHighLevel.clone(HLTPaths = cms.vstring(
        #"HLT_Ele27WPTight_v*"
        "HLT_Ele27_WPTight_Gsf_v*"
    ) )

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
                     process.flashggMuonSystematics*process.flashggElectronSystematics*
                     (process.flashggUnpackedJets*process.jetSystematicsSequence)*
                     (process.flashggTagSequence*process.systematicsTagSequences)
                     )

if customize.doJetSystTrees:
    process.p += process.flashggSystTagMerger

process.p += process.ZPlusJetTagDumper

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
customize.setDefault("maxEvents",100)
customize.setDefault("targetLumi",2.61e+3)
# call the customization
customize(process)
