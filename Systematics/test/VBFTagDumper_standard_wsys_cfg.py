#!/usr/bin/env cmsRun

#runOnZee = False
dumpJetSysTrees = True

if dumpJetSysTrees:
    dumpSysTreeMode = 2 #VBF
else:
    dumpSysTreeMode = 0

import FWCore.ParameterSet.Config as cms
import FWCore.Utilities.FileUtils as FileUtils
import FWCore.ParameterSet.VarParsing as VarParsing
import os
from flashgg.Systematics.SystematicDumperDefaultVariables import minimalVariables,minimalHistograms,minimalNonSignalVariables,systematicVariables

# ========================================================================
# SYSTEMATICS SECTION
process = cms.Process("FLASHggSyst")
process.load("FWCore.MessageService.MessageLogger_cfi")
process.load("Configuration.StandardSequences.GeometryDB_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff")

from Configuration.AlCa.GlobalTag import GlobalTag
if   os.environ["CMSSW_VERSION"].count("CMSSW_7_6"):
    process.GlobalTag.globaltag = '76X_mcRun2_asymptotic_v12'
elif os.environ["CMSSW_VERSION"].count("CMSSW_8_0"):
    process.GlobalTag.globaltag = '80X_mcRun2_asymptotic_2016_miniAODv2'
elif os.environ["CMSSW_VERSION"].count("CMSSW_7_4"):
    process.GlobalTag.globaltag = '74X_mcRun2_asymptotic_v4'
else:
    raise Exception,"Could not find a sensible CMSSW_VERSION for default globaltag"

process.maxEvents   = cms.untracked.PSet( input  = cms.untracked.int32( 50000 ) )
process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32( 1000 )

from flashgg.Systematics.SystematicsCustomize import *
jetSystematicsInputTags = createStandardSystematicsProducers(process)

process.load("flashgg.Taggers.flashggTagSequence_cfi")
process.load("flashgg.Taggers.flashggTagTester_cfi")

# PUJID  working points 
# cut values for 4X3 categories
# columns :  eta bins  : 4 categories
# rows    :  pt bins   :  3 

mva_wp = {
    "none"  : [
        [],[],[]
        #[-1, -1,  -1, -1],
        #[-1, -1,  -1, -1],
        #[-1, -1 , -1, -1]
    ],
    "tight" : [
        [0.26, -0.34, -0.24, -0.26],
        [0.62, -0.21, -0.07, -0.03],
        [0.87, 0.17 , 0.25 ,  0.33]
    ],
    "medium": [
        [-0.49, -0.53, -0.44, -0.42],
        [-0.06, -0.42, -0.30, -0.23],
        [ 0.56, -0.10,  0.01,  0.13]
    ],
    "loose" :[
        [-0.96, -0.64, -0.56, -0.54],
        [-0.92, -0.56, -0.44, -0.39],
        [-0.77, -0.31, -0.20, -0.03]
    ],
    "mixed" :[
        [-0.96, -0.34, -0.24, -0.26],
        [-0.92, -0.21, -0.07, -0.03],
        [-0.77, 0.17 , 0.25 ,  0.33]
        #[0.87, 0.17 , 0.25 ,  0.33]
    ],
    "forward_tight" : [
        [-1, -0.34, -0.24, -0.26],
        [-1, -0.21, -0.07, -0.03],
        [-1, 0.17 , 0.25 ,  0.33]
    ],
    "forward_medium": [
        [-1, -0.53, -0.44, -0.42],
        [-1, -0.42, -0.30, -0.23],
        [-1, -0.10,  0.01,  0.13]
    ],
    "forward_loose" :[
        [-1, -0.64, -0.56, -0.54],
        [-1, -0.56, -0.44, -0.39],
        [-1, -0.31, -0.20, -0.03]
    ]
}

# Keep an extra category as 'would go elsewhere instead', ignore preselection
process.flashggVBFTag.Boundaries             = cms.vdouble(-999.,0.62, 0.94)
process.flashggVBFTag.SetArbitraryNonGoldMC  = cms.bool(False)
process.flashggVBFTag.DropNonGoldData        = cms.bool(False)
process.flashggVBFTag.RequireVBFPreselection = cms.bool(False)

modifyTagSequenceForSystematics(process,jetSystematicsInputTags,dumpSysTreeMode)

systlabels    = [""]
phosystlabels = []
jetsystlabels = []
elesystlabels = []
musystlabels  = []

# import flashgg customization to check if we have signal or background
from flashgg.MetaData.JobConfig import customize
# Register forwardJetRMSCut to be used from customize
customize.options.register('forwardJetRMSCut',
                           0.03,
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

customize.parse()
print "customize.processId:",customize.processId

useEGMTools(process)

#== Only run systematics for signal events
from flashgg.Taggers.flashggTags_cff import UnpackedJetCollectionVInputTag

# load the correctors
process.load("JetMETCorrections.Configuration.JetCorrectors_cff")

if customize.processId == "Data":
    print "Data, so turn of all shifts and systematics, with some exceptions"
    variablesToUse = minimalNonSignalVariables

    process.flashggElectronSystematics.src = cms.InputTag("flashggElectrons")
    customizeSystematicsForData(process)
else:
    print "Background MC, so store mgg and central only"
    variablesToUse = minimalNonSignalVariables

    if dumpJetSysTrees:
        print "Running jet systematics and putting them in ntuples because doJetSystTrees is set"
        for direction in ["Up","Down"]:
            jetsystlabels.append("JEC%s01sigma" % direction)
            jetsystlabels.append("JER%s01sigma" % direction)
            jetsystlabels.append("RMSShift%s01sigma" % direction)
        systlabels += jetsystlabels
    else:
        print "Background MC, so store mgg and central only"
        customizeSystematicsForBackground(process)

print "Turning on central value for UnmatchedPUweight..."
for i in range(len(jetSystematicsInputTags)):
    prodname = 'flashggJetSystematics%i'%i
    vpset = getattr(process,prodname).SystMethods
    for pset in vpset:
        syst = pset.Label.value()
        if syst.count("UnmatchedPUWeight"):
            pset.ApplyCentralValue = True # default to false
#            pset.Debug = True
#            pset.OverallRange = "abs(eta)>2.5&&abs(eta)<4.7&&pt>20.&&pt<50." #uncomment to apply weights everywhere, not just pu: TESTING ONLY!!

process.flashggVBFTag.GetQCDWeights = True
        
print "--- Systematics  with independent collections ---"
print systlabels
print "-------------------------------------------------"
print "--- Variables to be dumped, including systematic weights ---"
print variablesToUse
print "------------------------------------------------------------"

# Use JetID                                                                                                                                                                     
process.flashggVBFMVA.UseJetID      = cms.bool(True)
process.flashggVBFMVA.JetIDLevel    = cms.string("Loose")                                                                                                  

process.flashggVBFTag.Boundaries  = cms.vdouble(-2)
process.systematicsTagSequences   = cms.Sequence()

process.flashggVBFMVA.rmsforwardCut = cms.double(customize.forwardJetRMSCut)
process.flashggVBFMVA.pujidWpPtBin1 = cms.vdouble(mva_wp[customize.pujidWP][0])
process.flashggVBFMVA.pujidWpPtBin2 = cms.vdouble(mva_wp[customize.pujidWP][1])
process.flashggVBFMVA.pujidWpPtBin3 = cms.vdouble(mva_wp[customize.pujidWP][2])
print '------------------------------------------------------------'
print ' PUJID Working point    ::' , customize.pujidWP
print '------------------------------------------------------------'
print ' PUJID wp cuts          ::' , mva_wp[customize.pujidWP]
print '------------------------------------------------------------'
print ' formward RMS Cut value ::' , customize.forwardJetRMSCut
print '------------------------------------------------------------'
print ' running on Zee         ::' , customize.runOnZee
print '------------------------------------------------------------'

cloneTagSequenceForEachSystematic(process,systlabels,phosystlabels,jetsystlabels,jetSystematicsInputTags,dumpSysTreeMode)

# ========================================================================
# Dumper section
from FWCore.ParameterSet.VarParsing import VarParsing
from flashgg.MetaData.samples_utils import SamplesManager

process.source = cms.Source ("PoolSource",
                             fileNames = cms.untracked.vstring(
        "root://eoscms.cern.ch//eos/cms/store/group/phys_higgs/cmshgg/sethzenz/flashgg/RunIISpring16DR80X-2_2_0-25ns_DYWithPDF/2_2_0/DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/RunIISpring16DR80X-2_2_0-25ns_DYWithPDF-2_2_0-v0-RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/160718_161741/0000/myMicroAODOutputFile_1.root"
   #                              "root://eoscms.cern.ch//eos/cms/store/group/phys_higgs/cmshgg/sethzenz/flashgg/RunIISpring16DR80X-2_1_2-25ns_ICHEP16/2_1_2/VBFHToGG_M-125_13TeV_powheg_pythia8/RunIISpring16DR80X-2_1_2-25ns_ICHEP16-2_1_2-v0-RunIISpring16MiniAODv1-PUSpring16RAWAODSIM_80X_mcRun2_asymptotic_2016_v3-v1/160628_143400/0000/myMicroAODOutputFile_5.root"
                                 #"/store/group/phys_higgs/cmshgg/ferriff/flashgg/RunIISpring16DR80X-2_0_0-25ns/2_0_0/VBFHToGG_M-120_13TeV_powheg_pythia8/RunIISpring16DR80X-2_0_0-25ns-2_0_0-v0-RunIISpring16MiniAODv1-PUSpring16RAWAODSIM_80X_mcRun2_asymptotic_2016_v3-v1/160524_093617/0000/myMicroAODOutputFile_1.root"
                                 #"root://eoscms.cern.ch//eos/cms/store/group/phys_higgs/cmshgg/ferriff/flashgg/RunIISpring16DR80X-2_0_0-25ns/2_0_0/DYToEE_NNPDF30_13TeV-powheg-pythia8/RunIISpring16DR80X-2_0_0-25ns-2_0_0-v0-RunIISpring16MiniAODv1-PUSpring16_80X_mcRun2_asymptotic_2016_v3-v1/160524_084452/0000/myMicroAODOutputFile_110.root"
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

if dumpJetSysTrees:
    process.vbfTagDumper.src = "flashggSystTagMerger"

# use custum TMVA weights
# process.flashggVBFMVA.vbfMVAweightfile = cms.FileInPath("flashgg/Taggers/data/TMVAClassification_dijetMVA_76x_17_02_15_BDTG.weights.xml")
# process.flashggVBFMVA.MVAMethod        = cms.string("BDTG")
# process.flashggDiPhotonMVA.diphotonMVAweightfile = cms.FileInPath("flashgg/Taggers/data/TMVAClassification_BDT_QCDeroded_v100_rereco.weights.xml")
#process.flashggVBFMVA.vbfMVAweightfile           = cms.FileInPath("flashgg/Taggers/data/TMVA_classification_dijet-mva-80x-ICHEP-12-07-2016-v03_BDTG.weights.xml")
#process.flashggVBFMVA.vbfDiPhoDiJetMVAweightfile = cms.FileInPath("flashgg/Taggers/data/TMVA_classification_combined-mva-80x-ICHEP-v02_BDTG.weights.xml")

# QCD Recovery 
# process.flashggVBFMVA.merge3rdJet   = cms.untracked.bool(False)
# process.flashggVBFMVA.thirdJetDRCut = cms.untracked.double(1.5)

# run on Drell-Yan 
if customize.runOnZee:
    process.flashggPreselectedDiPhotons.variables =  cms.vstring('pfPhoIso03', 
                                                                 'trkSumPtHollowConeDR03', 
                                                                 'full5x5_sigmaIetaIeta', 
                                                                 'full5x5_r9', 
                                                                 '1-passElectronVeto')

for syst in jetsystlabels:
    getattr(process,"flashggPreselectedDiPhotons%s"%syst).variables = process.flashggPreselectedDiPhotons.variables
# get the variable list
import flashgg.Taggers.VBFTagVariables as var
new_variables = [
    "n_jets               := VBFMVA.n_rec_jets",
    "dijet_jet1_RMS       := leading_rms",
    "dijet_jet2_RMS       := subLeading_rms",
    "dijet_jet1_QGL       := leading_QGL",
    "dijet_jet2_QGL       := subLeading_QGL",
    "dijet_jet1_pujid_mva := leading_pujidMVA()",
    "dijet_jet2_pujid_mva := subleading_pujidMVA()"
]

matching_photon = [
    "dijet_jet1_match := leadingJet_match",
    "dijet_jet2_match := subLeadingJet_match",
    "prompt_pho_1     := diPhoton.leadingPhoton.genMatchType()",
    "prompt_pho_2     := diPhoton.subLeadingPhoton.genMatchType()"
]

jet_syst_weights = [
    "UnmatchedPUWeightUp01sigma := weight(\"UnmatchedPUWeightUp01sigma\")",
    "centralObjectWeight := centralWeight",
    "UnmatchedPUWeightDown01sigma := weight(\"UnmatchedPUWeightDown01sigma\")" ,
    "alphaUp := alphaUp",
    "alphaDown := alphaDown",
]

for i in range(3):
    jet_syst_weights += ["scaleUp_%i := scaleUp(%i)" % (i,i),"scaleDown_%i := scaleDown(%i)" % (i,i)]

for i in range(60):
    jet_syst_weights +=["pdf_%i := pdf(%i)" % (i,i)]


#        float alphaUp() const { return alphaUp_; }
#        float alphaDown() const { return alphaDown_; }
#        float scaleUp(unsigned i) const { return scaleUp_[i]; }
#        float scaleDown(unsigned i) const { return scaleDown_[i]; }
#        float pdf(unsigned i) const { return pdf_[i]; }


all_variables = var.dipho_variables + var.dijet_variables +  new_variables + jet_syst_weights
if customize.processId != "Data":
    all_variables += var.truth_variables + matching_photon
    
cats = []
if dumpJetSysTrees:
    for syst in jetsystlabels:
        systcutstring = "hasSyst(\"%s\") "%syst
        cats += [("VBFDiJet_%s"%syst,systcutstring,0)]

cats += [("VBFDiJet","1",0)]
        

cfgTools.addCategories(process.vbfTagDumper,
                       cats,
                       variables  = all_variables,
                       histograms = []
)
process.vbfTagDumper.nameTemplate = "$PROCESS_$SQRTS_$CLASSNAME_$SUBCAT_$LABEL"
from HLTrigger.HLTfilters.hltHighLevel_cfi import hltHighLevel
if customize.runOnZee:
    if customize.processId == "Data":
        process.hltHighLevel = hltHighLevel.clone(HLTPaths = cms.vstring("HLT_Ele27_eta2p1_WPLoose_Gsf_v*") )
        #else:
        #process.hltHighLevel = hltHighLevel.clone(HLTPaths = cms.vstring("HLT_Ele27_eta2p1_WP75_Gsf_v*") )
else:
    process.hltHighLevel = hltHighLevel.clone(HLTPaths = cms.vstring("HLT_Diphoton30_18_R9Id_OR_IsoCaloId_AND_HE_R9Id_Mass90_v*"))
    
process.options      = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )

# ee bad supercluster filter on data
process.load('RecoMET.METFilters.eeBadScFilter_cfi')
process.eeBadScFilter.EERecHitSource = cms.InputTag("reducedEgamma","reducedEERecHits")

process.dataRequirements = cms.Sequence()
if customize.processId == "Data":
    process.flashggElectronSystematics.src = cms.InputTag("flashggSelectedElectrons") # this seems to cause problem only with data, Changed to flashggSelectedElectrons instead of flashggElectrons
    process.dataRequirements += process.hltHighLevel
    process.dataRequirements += process.eeBadScFilter

# Split WH and ZH
process.genFilter = cms.Sequence()
if (customize.processId.count("wh") or customize.processId.count("zh")) and not customize.processId.count("wzh"):
    process.load("flashgg/Systematics/VHFilter_cfi")
    process.genFilter += process.VHFilter
    process.VHFilter.chooseW = bool(customize.processId.count("wh"))
    process.VHFilter.chooseZ = bool(customize.processId.count("zh"))
    
process.p = cms.Path(process.dataRequirements
                     * process.genFilter
                     * process.flashggUpdatedIdMVADiPhotons
                     * process.flashggDiPhotonSystematics
                     * process.flashggMuonSystematics
                     * process.flashggElectronSystematics
                     * (process.flashggUnpackedJets
                        * process.ak4PFCHSL1FastL2L3CorrectorChain
                        * process.jetSystematicsSequence)
                     * (process.flashggTagSequence
                        + process.systematicsTagSequences)
                     * process.flashggSystTagMerger
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

# set default options if needed
customize.setDefault("maxEvents"  ,10000   )
customize.setDefault("targetLumi" ,1.00e+3)

#process.load("flashgg/Taggers/flashggTagTester_cfi")
#process.flashggTagTester.TagSorter = cms.InputTag("flashggSystTagMerger")
#process.flashggTagTester.ExpectMultiples = cms.untracked.bool(True)
#process.p += process.flashggTagTester 

# call the customization
customize(process)
