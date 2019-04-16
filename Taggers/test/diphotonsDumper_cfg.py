#!/usr/bin/env cmsRun

import FWCore.ParameterSet.Config as cms
import FWCore.Utilities.FileUtils as FileUtils
import os

process = cms.Process("Analysis")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.source = cms.Source("PoolSource",
                            fileNames=cms.untracked.vstring(
        "file:myMicroAODOutputFile.root"        
        )
)
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )
process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32( 1000 )

process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff")
process.load('Configuration.StandardSequences.GeometryRecoDB_cff') ## please double check
from Configuration.AlCa.GlobalTag import GlobalTag
if os.environ["CMSSW_VERSION"].count("CMSSW_7_6"):
    process.GlobalTag.globaltag = '76X_mcRun2_asymptotic_v12'
elif os.environ["CMSSW_VERSION"].count("CMSSW_7_4"):
    process.GlobalTag.globaltag = '74X_mcRun2_asymptotic_v4' 
elif os.environ["CMSSW_VERSION"].count("CMSSW_8_0"):
    process.GlobalTag.globaltag = '80X_mcRun2_asymptotic_2016_miniAODv2'
elif os.environ["CMSSW_VERSION"].count("CMSSW_9_4"):
     process.GlobalTag = GlobalTag(process.GlobalTag, '94X_mc2017_realistic_v12')
elif os.environ["CMSSW_VERSION"].count("CMSSW_10_1"):
    process.GlobalTag = GlobalTag(process.GlobalTag,'101X_dataRun2_Prompt_v11','')
else:
    raise Exception,"Could not find a sensible CMSSW_VERSION for default globaltag"
print "[INFO] Global Tag: ",process.GlobalTag.globaltag

process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string("test.root")
)

process.load("flashgg.Taggers.flashggUpdatedIdMVADiPhotons_cfi") 
from flashgg.Taggers.flashggPreselectedDiPhotons_cfi import flashggPreselectedDiPhotons
process.kinPreselDiPhotons = flashggPreselectedDiPhotons.clone(
cut=cms.string(
        "leadingPhoton.pt > 40 && subLeadingPhoton.pt > 30"
        " && abs(leadingPhoton.superCluster.eta)<2.5 && abs(subLeadingPhoton.superCluster.eta)<2.5 "
        " && ( abs(leadingPhoton.superCluster.eta)<1.4442 || abs(leadingPhoton.superCluster.eta)>1.566)"
        " && ( abs(subLeadingPhoton.superCluster.eta)<1.4442 || abs(subLeadingPhoton.superCluster.eta)>1.566)"
        )
                                                              )

process.load("flashgg.Taggers.diphotonDumper_cfi") ##  import diphotonDumper 
import flashgg.Taggers.dumperConfigTools as cfgTools

process.diphotonDumper.src = "kinPreselDiPhotons"

process.diphotonDumper.dumpTrees = True
process.diphotonDumper.dumpWorkspace = False
process.diphotonDumper.quietRooFit = True


# split tree, histogram and datasets by process
process.diphotonDumper.nameTemplate ="$PROCESS_$SQRTS_$LABEL_$SUBCAT"
## do not split by process
## process.diphotonDumper.nameTemplate = "minitree_$SQRTS_$LABEL_$SUBCAT"

## define categories and associated objects to dump
cfgTools.addCategory(process.diphotonDumper,
                     "Reject",
                     "abs(leadingPhoton.superCluster.eta)>=1.4442&&abs(leadingPhoton.superCluster.eta)<=1.566||abs(leadingPhoton.superCluster.eta)>=2.5"
                     "||abs(subLeadingPhoton.superCluster.eta)>=1.4442 && abs(subLeadingPhoton.superCluster.eta)<=1.566||abs(subLeadingPhoton.superCluster.eta)>=2.5",
                      -1 ## if nSubcat is -1 do not store anythings
                     )

# interestng categories 
cfgTools.addCategories(process.diphotonDumper,
                       ## categories definition
                       ## cuts are applied in cascade. Events getting to these categories have already failed the "Reject" selection
                       [("EBHighR9","max(abs(leadingPhoton.superCluster.eta),abs(leadingPhoton.superCluster.eta))<1.4442"
                         "&& min(leadingPhoton.full5x5_r9,subLeadingPhoton.full5x5_r9)>0.94",0), ## EB high R9
                        ("EBLowR9","max(abs(leadingPhoton.superCluster.eta),abs(leadingPhoton.superCluster.eta))<1.4442",0), ## remaining EB is low R9
                        ("EEHighR9","min(leadingPhoton.full5x5_r9,subLeadingPhoton.full5x5_r9)>0.94",0), ## then EE high R9
                        ("EELowR9","1",0), ## evereything elese is EE low R9
                        ],
                       ## variables to be dumped in trees/datasets. Same variables for all categories
                       ## if different variables wanted for different categories, can add categorie one by one with cfgTools.addCategory
                       variables=["CMS_hgg_mass[320,100,180]:=mass", 
                                  "leadPt                   :=leadingPhoton.pt",
                                  "subleadPt                :=subLeadingPhoton.pt",
                                  "minR9                    :=min(leadingPhoton.full5x5_r9,subLeadingPhoton.full5x5_r9)",
                                  "maxEta                   :=max(abs(leadingPhoton.superCluster.eta),abs(leadingPhoton.superCluster.eta))",
                                  "leadIDMVA                :=leadingView.phoIdMvaWrtChosenVtx",
                                  "subleadIDMVA             :=subLeadingView.phoIdMvaWrtChosenVtx",
                                  ],
                       ## histograms to be plotted. 
                       ## the variables need to be defined first
                       histograms=["CMS_hgg_mass>>mass(320,100,180)",
                                   "subleadPt:leadPt>>ptSubVsLead(180,20,200:180,20,200)",
                                   "minR9>>minR9(110,0,1.1)",
                                   "maxEta>>maxEta[0.,0.1,0.2,0.3,0.4,0.6,0.8,1.0,1.2,1.4442,1.566,1.7,1.8,2.,2.2,2.3,2.5]"
                                   ]
                       )


process.p1 = cms.Path(
    process.flashggUpdatedIdMVADiPhotons*process.kinPreselDiPhotons*process.diphotonDumper
    )



from flashgg.MetaData.JobConfig import customize
customize.setDefault("maxEvents",100)
customize(process)

