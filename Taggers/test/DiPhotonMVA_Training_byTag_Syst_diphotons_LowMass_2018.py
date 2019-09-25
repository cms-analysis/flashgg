#!/usr/bin/env cmsRun

import FWCore.ParameterSet.Config as cms
import FWCore.Utilities.FileUtils as FileUtils

from flashgg.Systematics.SystematicDumperDefaultVariables import minimalVariables,minimalHistograms,minimalNonSignalVariables,systematicVariables
import FWCore.ParameterSet.VarParsing as VarParsing
import FWCore.ParameterSet.Options as options
from flashgg.MetaData.samples_utils import SamplesManager


from flashgg.MetaData.JobConfig import customize
customize.setDefault("metaConditions",'$CMSSW_BASE/src/flashgg/MetaData/data/MetaConditions/Era2018_RR-17Sep2018_v1.json')

process = cms.Process("DiPhotonMVATrainig")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.load("Configuration.StandardSequences.GeometryDB_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag.globaltag = '102X_upgrade2018_realistic_v15'
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )
process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32(1000)
process.source = cms.Source ("PoolSource",fileNames = cms.untracked.vstring( "root://cms-xrd-global.cern.ch//store/user/spigazzi/flashgg/Era2018_RR-17Sep2018_v2/legacyRun2FullV2/GluGluHToGG_M-125_13TeV_powheg_pythia8/Era2018_RR-17Sep2018_v2-legacyRun2FullV2-v0-RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/190710_090337/0000/myMicroAODOutputFile_13.root"))



from flashgg.Systematics.SystematicsCustomize import *

process.load("flashgg/Taggers/flashggTagSequence_cfi")
process.load("flashgg/Taggers/flashggPreselectedDiPhotons_cfi")
process.load("flashgg/Systematics/flashggDiPhotonSystematics2018_cfi")
#process.load("flashgg/MetaData/data/MetaConditions/Era2018_RR-17Sep2018_v1.json")

from flashgg.Taggers.flashggDifferentialPhoIdInputsCorrection_cfi import flashggDifferentialPhoIdInputsCorrection

useEGMTools(process)

jetSystematicsInputTags = createStandardSystematicsProducers(process)
modifyTagSequenceForSystematics(process,jetSystematicsInputTags)

systlabels = [""]
phosystlabels = []
jetsystlabels = []


from HLTrigger.HLTfilters.hltHighLevel_cfi import hltHighLevel
##trigger for 2016
# STD
#process.hltHighLevel= hltHighLevel.clone(HLTPaths = cms.vstring("HLT_Diphoton30_18_R9Id_OR_IsoCaloId_AND_HE_R9Id_Mass90_v*") )
# LOW MASS
#process.hltHighLevel= hltHighLevel.clone(HLTPaths = cms.vstring( "HLT_Diphoton30PV_18PV_R9Id_AND_IsoCaloId_AND_HE_R9Id_DoublePixelVeto_Mass55_v*",
#                                                                 "HLT_Diphoton30EB_18EB_R9Id_OR_IsoCaloId_AND_HE_R9Id_DoublePixelVeto_Mass55_v*")
#                                                               )
##trigger for 2017
# LOW MASS
process.hltHighLevel= hltHighLevel.clone(HLTPaths = cms.vstring("HLT_Diphoton30PV_18PV_R9Id_AND_IsoCaloId_AND_HE_R9Id_PixelVeto_Mass55_v*") )
                                                                

from flashgg.Taggers.flashggTagOutputCommands_cff import tagDefaultOutputCommand


process.TFileService = cms.Service("TFileService",
			fileName = cms.string("histo.root"),
			closeFileFast = cms.untracked.bool(True)
			)


process.flashggUntagged.Boundaries=cms.vdouble(-2)

# customization for job splitting, lumi weighting, etc.
#from flashgg.MetaData.JobConfig import customize
customize.setDefault("maxEvents",-1)
#customize.setDefault("processIndex",5)
#customize.setDefault("targetLumi", 41529.186840973)
#customize.setDefault("targetLumi",1.e+3)
customize.setDefault("targetLumi",1.0)

#customize.setDefault("metaConditions",'$CMSSW_BASE/src/flashgg/MetaData/data/MetaConditions/Era2018_RR-17Sep2018_v1.json')

# OLD PU Target from Vittorio
#customize.setDefault("puTarget", ' 2.51e+05,1.15e+06,2.47e+06,3.72e+06,5.19e+06,6.79e+06,8.67e+06,2.31e+07,5.89e+07,1.38e+08,3.12e+08,5.71e+08,8.76e+08,1.21e+09,1.56e+09,1.87e+09,2.08e+09,2.19e+09,2.24e+09,2.28e+09,2.29e+09,2.24e+09,2.15e+09,2.03e+09,1.88e+09,1.71e+09,1.54e+09,1.36e+09,1.19e+09,1.01e+09,8.48e+08,6.94e+08,5.57e+08,4.38e+08,3.37e+08,2.53e+08,1.85e+08,1.31e+08,8.96e+07,5.87e+07,3.68e+07,2.2e+07,1.25e+07,6.75e+06,3.46e+06,1.68e+06,7.79e+05,3.44e+05,1.46e+05,6.13e+04,2.68e+04,1.33e+04,8.25e+03,6.30e+03,5.45e+03,4.97e+03,4.59e+03,4.25e+03,3.92e+03,3.58e+03,3.25e+03,2.93e+03,2.62e+03,2.33e+03,2.05e+03,1.79e+03,1.56e+03,1.34e+03,1.14e+03,969,815,680,563,463,378')
# NEW 2016 PU Target from flashgg/Systematics/test/puTarget_moriond17_69200.txt
#customize.setDefault("puTarget", ' 2.39e+05,8.38e+05,2.31e+06,3.12e+06,4.48e+06,6.00e+06,7.00e+06,1.29e+07,3.53e+07,7.87e+07,1.77e+08,3.60e+08,6.03e+08,8.77e+08,1.17e+09,1.49e+09,1.76e+09,1.94e+09,2.05e+09,2.10e+09,2.13e+09,2.15e+09,2.13e+09,2.06e+09,1.96e+09,1.84e+09,1.70e+09,1.55e+09,1.40e+09,1.24e+09,1.09e+09,9.37e+08,7.92e+08,6.57e+08,5.34e+08,4.27e+08,3.35e+08,2.58e+08,1.94e+08,1.42e+08,1.01e+08,6.9e+07,4.55e+07,2.88e+07,1.75e+07,1.02e+07,5.64e+06,2.99e+06,1.51e+06,7.32e+05,3.40e+05,1.53e+05,6.74e+04,3.05e+04,1.52e+04,8.98e+03,6.50e+03,5.43e+03,4.89e+03,4.52e+03,4.21e+03,3.91e+03,3.61e+03,3.32e+03,3.03e+03,2.75e+03,2.47e+03,2.21e+03,1.97e+03,1.74e+03,1.52e+03,1.32e+03,1.14e+03,983,839')
# 2017 PU Target
customize.setDefault("puTarget", '258796,1.08435e+06,2.01193e+06,3.77861e+06,4.05589e+06,5.87787e+06,6.4516e+06,6.83344e+06,9.25297e+06,2.18232e+07,4.37034e+07,8.26532e+07,1.31504e+08,1.8857e+08,2.66488e+08,3.75273e+08,5.23538e+08,6.97846e+08,8.71195e+08,1.03118e+09,1.17165e+09,1.28523e+09,1.3722e+09,1.43974e+09,1.49771e+09,1.55261e+09,1.59971e+09,1.6293e+09,1.63492e+09,1.61469e+09,1.57114e+09,1.50879e+09,1.43073e+09,1.33931e+09,1.23908e+09,1.13676e+09,1.03828e+09,9.47166e+08,8.657e+08,7.96707e+08,7.43839e+08,7.10458e+08,6.98228e+08,7.06139e+08,7.29876e+08,7.61635e+08,7.90699e+08,8.05265e+08,7.95322e+08,7.55363e+08,6.8604e+08,5.94397e+08,4.9105e+08,3.8729e+08,2.92423e+08,2.12189e+08,1.48643e+08,1.01017e+08,6.69271e+07,4.34303e+07,2.77229e+07,1.74762e+07,1.09192e+07,6.78555e+06,4.20834e+06,2.61362e+06,1.63076e+06,1.02515e+06,650709,417555,270919,177583,117412,78141.3,52235.7,34999.9,23462.5,15711.1,10495.3,6986.61,4630.6,3053.36,2001.73,1303.98,843.625,541.805,345.281,218.261,136.808,85.0054,52.3447,31.9367,19.3024,11.5548,6.84977,4.02065,2.33655,1.34421,0.76548,0.431463')




#customize.options.register('diphoxml',
#                           'flashgg/Taggers/data/Flashgg_DiPhoton_ICHEP.weights.xml',
#                           VarParsing.VarParsing.multiplicity.singleton,
#                           VarParsing.VarParsing.varType.string,
#                           'diphoxml'
#                           )

customize.options.register('diphoxml',
                          'flashgg/Taggers/data/Flashgg_DiPhoton_Moriond17.weights.xml',
                           VarParsing.VarParsing.multiplicity.singleton,
                           VarParsing.VarParsing.varType.string,
                           'diphoxml'
                           )


customize.options.register('runOnZ',
                           '',
                           VarParsing.VarParsing.multiplicity.singleton,
                           VarParsing.VarParsing.varType.string,
                           'runOnZ'
                           )
customize.runOnZ = ''
customize.parse()


#FIXME ?
#if customize.processType == 'data':
#    process.GlobalTag.globaltag = '80X_dataRun2_Prompt_v8'
#else:
#    process.GlobalTag.globaltag = '94X_mc2017_realistic_v12'


process.flashggDiPhotonMVA.diphotonMVAweightfile = customize.diphoxml

# STD
#minmass=100.
#maxmass=180.
# LOW
#minmass=65.
#maxmass=120.
# LARGE
minmass=65.
maxmass=180.

if customize.runOnZ:
    minmass=70.
    maxmass=120.



process.flashggDiPhotonSystematics.src = "flashggDifferentialPhoIdInputsCorrection" 

###secondpart pfrom microAODtoWS
if customize.processType != 'data':
    variablesToUse = minimalVariables
    for direction in ["Up","Down"]:
#    for direction in ["Down"]:
        phosystlabels.append("MvaShift%s01sigma" % direction)
#        phosystlabels.append("MvaShift%s02sigma" % direction)
#        phosystlabels.append("MvaShift%s03sigma" % direction)
#        phosystlabels.append("MvaShift%s04sigma" % direction)
        phosystlabels.append("SigmaEOverEShift%s01sigma" % direction)
    systlabels += phosystlabels
    customizeSystematicsForSignal(process)

elif customize.processType == 'data':
    variablesToUse = minimalVariables
    customizeSystematicsForData(process)


if customize.runOnZ != '':
    process.flashggPreselectedDiPhotons.variables[-1] = "-(passElectronVeto - 1)"
    if customize.runOnZ == 'single' and customize.processType == 'data':
        process.hltHighLevel.HLTPaths = ["HLT_Ele35_WPLoose_Gsf_v*"]

# STD
#process.flashggTagSorter.MassCutLower=100.
#process.flashggTagSorter.MassCutUpper=180.
# LOW
#process.flashggTagSorter.MassCutLower=65.
#process.flashggTagSorter.MassCutUpper=120.
# LARGE
process.flashggTagSorter.MassCutLower=65.
process.flashggTagSorter.MassCutUpper=180.

metsystlabels = []

cloneTagSequenceForEachSystematic(process,systlabels,phosystlabels,metsystlabels,jetsystlabels,jetSystematicsInputTags)


import flashgg.Taggers.dumperConfigTools as cfgTools
#from flashgg.Taggers.tagsDumpers_cfi import createTagDumper
# ## FIXME switch to preselected diphotons
#process.flashggDiPhotonMVANew.DiPhotonTag = "flashggPreselectedDiPhotons"

process.extraDumpers = cms.Sequence()
process.load("flashgg.Taggers.diphotonTagDumper_cfi") ##  import diphotonTagDumper 
process.tagsDumper.className = "DiPhotonTagDumper"
process.tagsDumper.src = "flashggSystTagMerger"
#process.tagsDumper.src = "flashggTagSystematics"
process.tagsDumper.processId = "test"
process.tagsDumper.dumpTrees = True
process.tagsDumper.dumpWorkspace = False
process.tagsDumper.dumpHistos = False
process.tagsDumper.quietRooFit = True
process.tagsDumper.nameTemplate = cms.untracked.string("$PROCESS_$SQRTS_$CLASSNAME_$SUBCAT_$LABEL")

#from flashgg.Taggers.dumperConfigTools import flashggUntagged
process.flashggUntagged.Boundaries = cms.vdouble(-1.,1.)
tagList=[
["UntaggedTag",0]
]


variablesToUse=[
    "leadptom         := diPhotonMVA.leadptom  ",
    "subleadptom      := diPhotonMVA.subleadptom ",
    "leadmva          := diPhotonMVA.leadmva ",
    "subleadmva       := diPhotonMVA.subleadmva    ",
    "leadeta          := diPhotonMVA.leadeta     ",
    "subleadeta       := diPhotonMVA.subleadeta",
    "sigmarv          := diPhotonMVA.sigmarv",
    "sigmarvDecorr    := diPhotonMVA.decorrSigmarv",
    "sigmawv          := diPhotonMVA.sigmawv",
    "CosPhi           := diPhotonMVA.CosPhi",
    "vtxprob          := diPhotonMVA.vtxprob",
    "result           := diPhotonMVA.result",
    "mass             := diPhoton.mass",
    "pt               := diPhoton.pt",
    "leadR9           := diPhoton.leadingPhoton().old_r9()",
    "subleadR9        := diPhoton.subLeadingPhoton().old_r9()",
    "leadfull5x5R9    := diPhoton.leadingPhoton().full5x5_r9()",
    "subleadfull5x5R9 := diPhoton.subLeadingPhoton().full5x5_r9()",
    "leadSCeta        := diPhoton.leadingPhoton().superCluster().eta()",
    "subleadSCeta     := diPhoton.subLeadingPhoton().superCluster().eta()",
    "leadMatchType    := diPhoton.leadingPhoton().genMatchType()",
    "subleadMatchType := diPhoton.subLeadingPhoton().genMatchType()",
    "leadSigEOverE    := diPhoton.leadingPhoton().sigEOverE()",
    "subleadSigEOverE := diPhoton.subLeadingPhoton().sigEOverE()",
    "dz               := ?!tagTruth().isNull()?abs(tagTruth().genPV().z-diPhoton().vtx().z):-99.",  
#    "MvaLinearSystDown01sigma[1,-999999.,999999.] := weight(\"MvaLinearSystDown01sigma\")",
#    "MvaLinearSystUp01sigma[1,-999999.,999999.] := weight(\"MvaLinearSystUp01sigma\")"
    ]

systematicVariables=[
    "leadptom         := diPhotonMVA.leadptom  ",
    "subleadptom      := diPhotonMVA.subleadptom ",
    "leadmva          := diPhotonMVA.leadmva ",
    "subleadmva       := diPhotonMVA.subleadmva    ",
    "leadeta          := diPhotonMVA.leadeta     ",
    "subleadeta       := diPhotonMVA.subleadeta",
    "sigmarv          := diPhotonMVA.sigmarv",
    "sigmarvDecorr    := diPhotonMVA.decorrSigmarv",
    "sigmawv          := diPhotonMVA.sigmawv",
    "CosPhi           := diPhotonMVA.CosPhi",
    "vtxprob          := diPhotonMVA.vtxprob",
    "result           := diPhotonMVA.result",
    "mass             := diPhoton.mass",
    "pt               := diPhoton.pt",
    "leadR9           := diPhoton.leadingPhoton().old_r9()",
    "subleadR9        := diPhoton.subLeadingPhoton().old_r9()",
    "leadfull5x5R9    := diPhoton.leadingPhoton().full5x5_r9()",
    "subleadfull5x5R9 := diPhoton.subLeadingPhoton().full5x5_r9()",
    "leadSCeta        := diPhoton.leadingPhoton().superCluster().eta()",
    "subleadSCeta     := diPhoton.subLeadingPhoton().superCluster().eta()",
    "leadMatchType    := diPhoton.leadingPhoton().genMatchType()",
    "subleadMatchType := diPhoton.subLeadingPhoton().genMatchType()",
    "leadSigEOverE    := diPhoton.leadingPhoton().sigEOverE()",
    "subleadSigEOverE := diPhoton.subLeadingPhoton().sigEOverE()",
    "dz               := ?!tagTruth().isNull()?abs(tagTruth().genPV().z-diPhoton().vtx().z):-99.",  
#    "MvaLinearSystDown01sigma[1,-999999.,999999.] := weight(\"MvaLinearSystDown01sigma\")",
#    "MvaLinearSystUp01sigma[1,-999999.,999999.] := weight(\"MvaLinearSystUp01sigma\")"
    ]

minimalHistograms = []


definedSysts=set()
process.tagsDumper.classifierCfg.remap=cms.untracked.VPSet()
#cfgTools.addCategory(process.tagDumper, "Reject",  "diPhoton.mass< %f || diPhoton.mass> %f" %(minmass, maxmass),-1 ## if nSubcat is -1 do not store anythings)
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
        
      cfgTools.addCategory(process.tagsDumper,
                           systlabel,
                           classname=tagName,
                           cutbased=cutstring,
                           subcats=tagCats, 
                           variables=currentVariables,
                           histograms=minimalHistograms,
                           #binnedOnly=isBinnedOnly,
                           #dumpPdfWeights=dumpPdfWeights,
                           #nPdfWeights=nPdfWeights,
                           #nAlphaSWeights=nAlphaSWeights,
                           #nScaleWeights=nScaleWeights
                           )



process.tagsDumper.splitLumiWeight=cms.untracked.bool(True)




process.options = cms.untracked.PSet( allowUnscheduled = cms.untracked.bool(True) )

if customize.processType != 'data':#and customize.runOnZ != 'single':
        process.p = cms.Path(
             process.flashggDifferentialPhoIdInputsCorrection
           * process.flashggDiPhotonSystematics
           * process.flashggMetSystematics
           * process.flashggMuonSystematics
           * process.flashggElectronSystematics
           * process.flashggUnpackedJets
           * process.jetSystematicsSequence
           * process.flashggTagSequence
           * process.systematicsTagSequences
           * process.flashggSystTagMerger
           * process.tagsDumper
           )
else:
    process.p = cms.Path(
             process.hltHighLevel
           * process.flashggDifferentialPhoIdInputsCorrection
           * process.flashggDiPhotonSystematics
           * process.flashggMetSystematics
           * process.flashggMuonSystematics
           * process.flashggElectronSystematics
           * process.flashggUnpackedJets
           * process.jetSystematicsSequence
           * process.flashggTagSequence
           * process.systematicsTagSequences
           * process.flashggSystTagMerger
           * process.tagsDumper
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


customize(process)

