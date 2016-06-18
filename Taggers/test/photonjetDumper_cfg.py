#!/usr/bin/env cmsRun

import FWCore.ParameterSet.Config as cms
import FWCore.Utilities.FileUtils as FileUtils

process = cms.Process("Analysis")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.source = cms.Source("PoolSource",
                            fileNames=cms.untracked.vstring(
        "file:myMicroAODOutputFile.root"        
        )
)
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(5000) )
process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32( 1000 )

process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string("test.root")
)



process.load("flashgg.Taggers.photonjetDumper_cfi") ##  import photonjetDumper 
import flashgg.Taggers.dumperConfigTools as cfgTools

process.photonjetDumper.dumpTrees = True
process.photonjetDumper.dumpWorkspace = False
process.photonjetDumper.quietRooFit = True

# split tree, histogram and datasets by process
#process.photonjetDumper.nameTemplate ="$PROCESS_$SQRTS_$LABEL_$SUBCAT"
## do not split by process
process.photonjetDumper.nameTemplate = "minitree_$SQRTS_$LABEL_$SUBCAT"

# interestng categories 
cfgTools.addCategories(process.photonjetDumper,
                       ## categories definition
                       ## cuts are applied in cascade. Events getting to these categories have already failed the "Reject" selection
                       [("all","1", 0)
                        ],
                       ## variables to be dumped in trees/datasets. Same variables for all categories
                       ## if different variables wanted for different categories, can add categorie one by one with cfgTools.addCategory
                       variables=["photonPt                 :=photon.pt",
                                  "jetPt                    :=jet.pt",
                                  "photonEta                :=photon.eta",
                                  "jetEta                   :=jet.eta",
                                  "logSumPt2","ptBal","ptAsym","nConv",
                                  "dZfromRecoPV", 
                                  "photonjetPt"
                                  ],
                       ## histograms to be plotted. 
                       ## the variables need to be defined first
                       histograms=[                                   
                                   ]
                       )





process.p1 = cms.Path(
    process.photonjetDumper
    )



#from flashgg.MetaData.JobConfig import customize
#customize.setDefault("maxEvents",100)
#customize(process)

