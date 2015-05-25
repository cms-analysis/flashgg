#!/usr/bin/env fwliteRun

import FWCore.ParameterSet.Config as cms

import FWCore.ParameterSet.VarParsing as VarParsing
from flashgg.MetaData.samples_utils import SamplesManager
        
process = cms.Process("FWLitePlots")

process.source = cms.Source("PoolSource",
                            fileNames=cms.untracked.vstring("file:myMicroAODOutputFile.root")
)

process.fwliteInput = cms.PSet(
    fileNames = cms.vstring("file:myMicroAODOutputFile.root"),
    maxEvents   = cms.int32(100),
    outputEvery = cms.uint32(10),
)


process.fwliteOutput = cms.PSet(
      fileName = cms.string("output.root")      ## mandatory
)

from flashgg.Taggers.photonDumpConfig_cff import photonDumpConfig
import flashgg.Taggers.dumperConfigTools as cfgTools


## Dumper
process.analyzer = photonDumpConfig
process.analyzer.dumpTrees = True
process.analyzer.dumpWorkspace = False
process.analyzer.quietRooFit = True
# split tree, histogram and datasets by process
## process.analyzer.nameTemplate ="$PROCESS_$SQRTS_$LABEL_$SUBCAT"
## do not split by process
## process.analyzer.nameTemplate = "minitree_$SQRTS_$LABEL_$SUBCAT"

process.analyzer.nameTemplate ="$PROCESS_$LABEL"

## define categories and associated objects to dump
cfgTools.addCategory(process.analyzer,
                     "Reject",
                     "abs(superCluster.eta)>=1.4442&&abs(superCluster.eta)<=1.566||abs(superCluster.eta)>=2.5",
                      -1 ## if nSubcat is -1 do not store anythings
                     )

# interestng categories 
cfgTools.addCategories(process.analyzer,
                       ## categories definition
                       ## cuts are applied in cascade. Events getting to these categories have already failed the "Reject" selection
                       [("promptPhotons","genMatchType == 1",0),
                        ("fakePhotons",  "genMatchType != 1",0),
                        ],
                       ## variables to be dumped in trees/datasets. Same variables for all categories
                       ## if different variables wanted for different categories, can add categorie one by one with cfgTools.addCategory
                       variables=["pt","energy","eta","phi","scEta:=superCluster.eta",
                                  "r9","chgIsoWrtWorstVtx := getpfChgIsoWrtWorstVtx03",
                                  "genIso := userFloat('genIso')", "eTrue := ? hasMatchedGenPhoton ? matchedGenPhoton.energy : 0"
                                  ],
                       ## histograms to be plotted. 
                       ## the variables need to be defined first
                       histograms=["r9>>r9(110,0,1.1)",
                                   "scEta>>scEta(100,-2.5,2.5)"
                                   ]
                       )


# customization for job splitting, lumi weighting, etc.
from flashgg.MetaData.JobConfig import customize
customize.setDefault("maxEvents",500)
customize.setDefault("targetLumi",1.e+4)
customize(process)


## print process.analyzer

