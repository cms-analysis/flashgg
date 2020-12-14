from importlib import import_module
# import importlib 
import FWCore.ParameterSet.Config as cms  # imports our CMS-specific Python classes and functions
import os # python module for os dependent functionality 
from flashgg.Taggers.flashggHHWWggCandidate_cfi import FlashggHHWWggCandidate # cut parameters 
from flashgg.Taggers.flashggPreselectedDiPhotons_cfi import flashggPreselectedDiPhotons
import flashgg.Taggers.dumperConfigTools as cfgTools 
from flashgg.MetaData.MetaConditionsReader import *

process = cms.Process("FLASHggHHWWggTest") # Process name. Can't use HH_WWgg because '_' is a non-alphanumeric character
from flashgg.Taggers.flashggTags_cff import flashggUnpackedJets
process.flashggUnpackedJets = flashggUnpackedJets

from flashgg.Taggers.flashggDiPhotonMVA_cfi import flashggDiPhotonMVA
process.flashggDiPhotonMVA = flashggDiPhotonMVA 

from flashgg.Taggers.flashggUpdatedIdMVADiPhotons_cfi import flashggUpdatedIdMVADiPhotons
process.flashggUpdatedIdMVADiPhotons = flashggUpdatedIdMVADiPhotons
# process.flashggUpdatedIdMVADiPhotons.DiPhotonTag = "flashggPreselectedDiPhotons"

from flashgg.Systematics.flashggDiPhotonSystematics_cfi import flashggDiPhotonSystematics

#process.flashggDiPhotonMVA.DiPhotonTag = "flashggDiPhotons" 
process.flashggDiPhotonMVA.DiPhotonTag = "flashggPreselectedDiPhotons"
###---HHWWgg candidates production
process.FlashggHHWWggCandidate = FlashggHHWWggCandidate.clone() # clone flashgg HHWWggCandidate parameters here 

process.FlashggHHWWggCandidate.idSelection = cms.PSet(
        rho = flashggPreselectedDiPhotons.rho,
        cut = flashggPreselectedDiPhotons.cut, # diphoton preselection cuts. Become part of Idselector definition  
        variables = flashggPreselectedDiPhotons.variables,
        categories = flashggPreselectedDiPhotons.categories
        )

process.flashggPreselectedDiPhotons = flashggPreselectedDiPhotons

###--- get the variables
import flashgg.Taggers.HHWWggTagVariables as var # python file of lists of strings 
#all_variables = var.pho_variables + var.dipho_variables + var.tp_variables + var.abe_variables # add variable lists together 
# all_variables = var.HHWWgg_variables # add variable lists together 
Fit_Variables = var.Fit_Variables
Reco_Variables = var.Reco_Variables
RECO_GEN_Variables = var.RECO_GEN_Variables

from flashgg.Taggers.HHWWggCandidateDumper_cfi import HHWWggCandidateDumper
process.HHWWggCandidateDumper = HHWWggCandidateDumper.clone() # clone parameters from HHWWggCandidateDumpConfig_cff (className, src, ...)
process.HHWWggCandidateDumper.dumpTrees = True # Trees 
process.HHWWggCandidateDumper.dumpWorkspace = True # Workspace 

# If signal, if data 

# Create histograms 

cfgTools.addCategories(process.HHWWggCandidateDumper,
                        [
                          # Signal Categories
                          # ("SL","(CMS_hgg_mass!=-99) && (CMS_hgg_mass>=100) && (CMS_hgg_mass<=180)",0), # for background model 
                          ("SL","(CMS_hgg_mass!=-99) && (CMS_hgg_mass>=115) && (CMS_hgg_mass<=135)",0), # for signal model 
                          # ("SL","1",0), # All events # Not working for some reason 
                          
                          # Data
                          # ("All_HLT_Events","1",0), # All events that passed HLT 

                        ],

                        # variables = all_variables, 
                        variables = Reco_Variables,
                        # variables = Fit_Variables,
                        # variables = RECO_GEN_Variables, 
                        histograms=[]
                        )

# Input file 
process.source = cms.Source ("PoolSource",
                             fileNames = cms.untracked.vstring(

# One data microaod 
# "root://cms-xrd-global.cern.ch//store/user/spigazzi/flashgg/Era2017_RR-31Mar2018_v2/legacyRun2FullV1/DoubleEG/Era2017_RR-31Mar2018_v2-legacyRun2FullV1-v0-Run2017C-31Mar2018-v1/190606_095024/0000/myMicroAODOutputFile_333.root"

# "root://cms-xrd-global.cern.ch//store/group/phys_higgs/cmshgg/atishelm/flashgg/28OctTest/RunIIFall18-4_0_0-75-g71c3c6e9/ggF_X250_WWgg_qqlnugg/RunIIFall17MiniAOD-94X_mc2017_realistic_v11_wPU_MICROAOD-5f646ecd4e1c7a39ab0ed099ff55ceb9/191029_142936/0000/myMicroAODOutputFile_3.root"
#"root://cms-xrd-global.cern.ch//store/group/phys_higgs/cmshgg/atishelm/flashgg/HHWWgg_v1/94X_mc2017-RunIIFall18/ggF_X250_WWgg_qqlnugg/HHWWgg_v1-94X_mc2017-RunIIFall18-v0-atishelm-100000events_wPU_MINIAOD-5f646ecd4e1c7a39ab0ed099ff55ceb9/191205_120702/0000/myMicroAODOutputFile_162.root"
# "file:/eos/cms/store/group/phys_higgs/cmshgg/atishelm/flashgg/HHWWgg_v1/94X_mc2017-RunIIFall18/ggF_X250_WWgg_qqlnugg/HHWWgg_v1-94X_mc2017-RunIIFall18-v0-atishelm-100000events_wPU_MINIAOD-5f646ecd4e1c7a39ab0ed099ff55ceb9/191205_120702/0000/myMicroAODOutputFile_162.root",

# "file:/eos/cms/store/group/phys_higgs/cmshgg/atishelm/flashgg/HHWWgg_v2-2/94X_mc2017-RunIIFall18/ggF_X250_WWgg_qqlnugg/HHWWgg_v2-2-94X_mc2017-RunIIFall18-v0-atishelm-100000events_wPU_MINIAOD-5f646ecd4e1c7a39ab0ed099ff55ceb9/191216_220038/0000/myMicroAODOutputFile_9.root",
"file:root://cms-xrd-global.cern.ch//store/user/spigazzi/flashgg/Era2017_RR-31Mar2018_v2/legacyRun2FullV1/ttHJetToGG_M125_13TeV_amcatnloFXFX_madspin_pythia8/Era2017_RR-31Mar2018_v2-legacyRun2FullV1-v0-RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/190703_133141/0000/myMicroAODOutputFile_9.root", # ttH
## X250                       
#"file:/eos/user/a/atishelm/ntuples/MicroAOD/ggF_X250_WWgg_qqlnu.root" # SL      
# "file:/eos/user/a/atishelm/ntuples/MicroAOD/ggF_X250_WWgg_lnulnu.root" # FL      
# "file:/eos/user/a/atishelm/ntuples/MicroAOD/ggF_X250_WWgg_qqqq.root" # FH

## X1250
# "file:/eos/user/a/atishelm/ntuples/MicroAOD/ggF_X1250_WWgg_qqlnu.root" # SL 
# "file:/eos/user/a/atishelm/ntuples/MicroAOD/ggF_X1250_WWgg_lnulnu.root" # FL
# "file:/eos/user/a/atishelm/ntuples/MicroAOD/ggF_X1250_WWgg_qqqq.root" # FH 
))


process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string("test.root"),
                                   closeFileFast = cms.untracked.bool(True)
)
#process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(1000) )


from flashgg.MetaData.JobConfig import customize
#customize.setDefault("maxEvents",-1)
customize.setDefault("maxEvents",1000)

# customize.setDefault("puTarget",'2.39e+05,8.38e+05,2.31e+06,3.12e+06,4.48e+06,6e+06,7e+06,1.29e+07,3.53e+07,7.87e+07,1.77e+08,3.6e+08,6.03e+08,8.77e+08,1.17e+09,1.49e+09,1.76e+09,1.94e+09,2.05e+09,2.1e+09,2.13e+09,2.15e+09,2.13e+09,2.06e+09,1.96e+09,1.84e+09,1.7e+09,1.55e+09,1.4e+09,1.24e+09,1.09e+09,9.37e+08,7.92e+08,6.57e+08,5.34e+08,4.27e+08,3.35e+08,2.58e+08,1.94e+08,1.42e+08,1.01e+08,6.9e+07,4.55e+07,2.88e+07,1.75e+07,1.02e+07,5.64e+06,2.99e+06,1.51e+06,7.32e+05,3.4e+05,1.53e+05,6.74e+04,3.05e+04,1.52e+04,8.98e+03,6.5e+03,5.43e+03,4.89e+03,4.52e+03,4.21e+03,3.91e+03,3.61e+03,3.32e+03,3.03e+03,2.75e+03,2.47e+03,2.21e+03,1.97e+03,1.74e+03,1.52e+03,1.32e+03,1.14e+03,983,839')

# import FWCore.ParameterSet.VarParsing as VarParsing
customize(process)
# print'customize.metaConditions = ',customize.metaConditions
customize.metaConditions = MetaConditionsReader(customize.metaConditions)

# if customize.processId == "Data":
#     process.GlobalTag.globaltag = str(
#         customize.metaConditions['globalTags']['data'])
# else:
#     process.GlobalTag.globaltag = str(
#         customize.metaConditions['globalTags']['MC'])

# if customize.inputFiles:
#     inputFile = customize.inputFiles
#
# if customize.outputFile:
#     outputFile = customize.outputFile

#customize.parse()
# process.TFileService = cms.Service("TFileService",
#                                    fileName = cms.string("output.root"),
#                                    closeFileFast = cms.untracked.bool(True)
# )

# Require diphoton triggers
from HLTrigger.HLTfilters.hltHighLevel_cfi import hltHighLevel
process.hltHighLevel= hltHighLevel.clone(HLTPaths = cms.vstring(   
                                                                "HLT_Diphoton30_22_R9Id_OR_IsoCaloId_AND_HE_R9Id_Mass90*" # 2017 Hgg HLT path 
                                                                # "HLT_Diphoton30_18_R9Id_OR_IsoCaloId_AND_HE_R9Id_Mass90_v*" # 2016 Hgg HLT path 
                                                                #"HLT_Diphoton30_18_R9Id_OR_IsoCaloId_AND_HE_R9Id_Mass90"
                                                                # "HLT_Diphoton30_18_PVrealAND_R9Id_AND_IsoCaloId_AND_HE_R9Id_PixelVeto_Mass55*",
                                                                # "HLT_Diphoton30_18_PVrealAND_R9Id_AND_IsoCaloId_AND_HE_R9Id_NoPixelVeto_Mass55*"
                                                               ))


process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )


# #to check which triggers are present ---remove if not needed
# process.TriggerAnalyzer = cms.EDAnalyzer("MiniAODTriggerAnalyzer",
# bits = cms.InputTag("TriggerResults","","HLT")
# )
# process.TriggerAnalyzerPath = cms.Path(process.TriggerAnalyzer)

#############   Geometry  ###############
process.load("Geometry.CMSCommonData.cmsIdealGeometryXML_cfi")
process.load("Geometry.CaloEventSetup.CaloGeometry_cfi")
process.load("Geometry.CaloEventSetup.CaloTopology_cfi")
process.load('RecoMET.METFilters.eeBadScFilter_cfi')
process.load("Configuration.Geometry.GeometryECALHCAL_cff")
process.eeBadScFilter.EERecHitSource = cms.InputTag("reducedEgamma","reducedEERecHits") # Saved MicroAOD Collection (data only)

process.dataRequirements = cms.Sequence()

# print'customize.processId = ',customize.processId
if customize.processId == "Data":
   process.dataRequirements += process.hltHighLevel # HLT 
   process.dataRequirements += process.eeBadScFilter

# Do scale and smearing corrections

# process.load("flashgg.Systematics.flashggDiPhotonSystematics_cfi")
# process.load("flashgg.Systematics."+customize.metaConditions["flashggDiPhotonSystematics"])

sysmodule = import_module(
    "flashgg.Systematics."+customize.metaConditions["flashggDiPhotonSystematics"])
systModules2D = cms.VPSet()
systModules = cms.VPSet()

if customize.processId == "Data":
    print'Data'
    systModules.append(sysmodule.MCScaleHighR9EB_EGM)
    systModules.append(sysmodule.MCScaleLowR9EB_EGM)
    systModules.append(sysmodule.MCScaleHighR9EE_EGM)
    systModules.append(sysmodule.MCScaleLowR9EE_EGM)
    # systModules.append(sysmodule.MCScaleGain6EB_EGM)
    # systModules.append(sysmodule.MCScaleGain1EB_EGM)

    for module in systModules:
        module.ApplyCentralValue = cms.bool(True)

else:
    print'Not Data'
    systModules.append(sysmodule.MCScaleHighR9EB_EGM)
    systModules.append(sysmodule.MCScaleLowR9EB_EGM)
    systModules.append(sysmodule.MCScaleHighR9EE_EGM)
    systModules.append(sysmodule.MCScaleLowR9EE_EGM)

    systModules2D.append(sysmodule.MCSmearHighR9EE_EGM)
    systModules2D.append(sysmodule.MCSmearLowR9EE_EGM)
    systModules2D.append(sysmodule.MCSmearHighR9EB_EGM)
    systModules2D.append(sysmodule.MCSmearLowR9EB_EGM)

    for module in systModules:
        module.ApplyCentralValue = cms.bool(False)

process.flashggDiPhotonSystematics = flashggDiPhotonSystematics
process.flashggDiPhotonSystematics.src = "flashggPreselectedDiPhotons"
process.flashggDiPhotonSystematics.SystMethods = systModules
process.flashggDiPhotonSystematics.SystMethods2D = systModules2D

# process.FlashggHHWWggCandidate.DiPhotonTag = "flashggDiPhotonSystematics"

# customize.register('puTarget',
#                    " ",
#                    VarParsing.VarParsing.multiplicity.singleton,
#                    VarParsing.VarParsing.varType.string,
#                    "puTarget")
###--- call the customization
# process.HHWWggCandidateDumper.puReWeight=cms.bool( bool(customize.PURW) )
# if customize.PURW == False:
# 	process.HHWWggCandidateDumper.puTarget = cms.vdouble()

# Options 

zero_vtx = 1 # Choose to require zeroeth vertex for all diphotons or not 
sands = 1 # Apply scaling and smearing 


# process.FlashggHHWWggCandidate.MVAResultTag = cms.InputTag('flashggUpdatedIdMVADiPhotons')

if zero_vtx:
  from flashgg.MicroAOD.flashggDiPhotons_cfi import flashggDiPhotons
  process.flashggDiPhotonsVtx0 = flashggDiPhotons.clone(useZerothVertexFromMicro = cms.bool(True), whichVertex=cms.uint32(0),
                                                        vertexProbMVAweightfile = "flashgg/MicroAOD/data/TMVAClassification_BDTVtxId_SL_2016.xml", # why not flashgg/MicroAOD/data/TMVAClassification_BDTVtxProb_SL_2016.xml ? 
                                                        vertexIdMVAweightfile = "flashgg/MicroAOD/data/TMVAClassification_BDTVtxId_SL_2016.xml" 
  )

  process.flashggPreselectedDiPhotons.src = "flashggDiPhotonsVtx0" # Only use zeroth vertex diphotons, order by pt 


  if sands:
    process.FlashggHHWWggCandidate.DiPhotonTag = cms.InputTag('flashggDiPhotonSystematics') # use diphotons with scaling and smearing
    process.path = cms.Path(process.flashggDiPhotonsVtx0
                          *process.flashggPreselectedDiPhotons
                          *process.flashggDiPhotonMVA
                          *process.flashggUnpackedJets
                          *process.dataRequirements
                          *process.flashggDiPhotonSystematics
                          *process.FlashggHHWWggCandidate
                          *process.HHWWggCandidateDumper
                          )
  else:
    print'zero vertex and no scaling and smearing'
    process.FlashggHHWWggCandidate.DiPhotonTag = cms.InputTag('flashggPreselectedDiPhotons') # use diphotons without scaling and smearing 
    process.path = cms.Path(process.flashggDiPhotonsVtx0
                          *process.flashggPreselectedDiPhotons
                          *process.flashggDiPhotonMVA
                          *process.flashggUnpackedJets
                          *process.dataRequirements
                          *process.flashggDiPhotonSystematics
                          *process.FlashggHHWWggCandidate
                          *process.HHWWggCandidateDumper
                          )

else:
  if sands:
    process.FlashggHHWWggCandidate.DiPhotonTag = cms.InputTag('flashggDiPhotonSystematics') # use diphotons with scaling and smearing
    process.flashggPreselectedDiPhotons.src = "flashggDiPhotons" # don't require 0th vertex 
    process.path = cms.Path(process.dataRequirements
                            *process.flashggPreselectedDiPhotons
                            *process.flashggDiPhotonMVA
                            *process.flashggUnpackedJets
                            *process.flashggDiPhotonSystematics
                            *process.FlashggHHWWggCandidate
                            *process.HHWWggCandidateDumper
                            )
  else:
    process.FlashggHHWWggCandidate.DiPhotonTag = cms.InputTag('flashggPreselectedDiPhotons') # use diphotons without scaling and smearing 
    process.flashggPreselectedDiPhotons.src = "flashggDiPhotons" # don't require 0th vertex 
    process.path = cms.Path(process.dataRequirements
                            *process.flashggPreselectedDiPhotons
                            *process.flashggDiPhotonMVA
                            *process.flashggUnpackedJets
                            *process.FlashggHHWWggCandidate
                            *process.HHWWggCandidateDumper
                            )
