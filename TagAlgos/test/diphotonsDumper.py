#!/usr/bin/env fwliteRun

import FWCore.ParameterSet.Config as cms

import FWCore.ParameterSet.VarParsing as VarParsing
from flashgg.MetaData.samples_utils import SamplesManager
        
process = cms.Process("FWLitePlots")

process.fwliteInput = cms.PSet(
    fileNames = cms.vstring(),
    maxEvents   = cms.int32(100),
    outputEvery = cms.uint32(200),
)


process.fwliteOutput = cms.PSet(
      fileName = cms.string("output.root")      ## mandatory
)

from flashgg.TagAlgos.diphotonDumpConfig_cff import diphotonDumpConfig
import flashgg.TagAlgos.dumperConfigTools as cfgTools

process.analyzer = diphotonDumpConfig
process.analyzer.dumpTrees = True
process.analyzer.dumpWorkspace = False
process.analyzer.quietRooFit = True
cfgTools.addCategory(process.analyzer,
                     "Reject", ## cuts are applied in cascade
                      "abs(leadingPhoton.superCluster.eta)>=1.4442&&abs(leadingPhoton.superCluster.eta)<=1.566||abs(leadingPhoton.superCluster.eta)>=2.5"
                      "||abs(subLeadingPhoton.superCluster.eta)>=1.4442 && abs(subLeadingPhoton.superCluster.eta)<=1.566||abs(subLeadingPhoton.superCluster.eta)>=2.5",
                      -1)
cfgTools.addCategories(process.analyzer,
                       [## cuts are applied in cascade
                        ("EBHighR9","max(abs(leadingPhoton.superCluster.eta),abs(leadingPhoton.superCluster.eta))<1.4442"
                         "&& min(leadingPhoton.r9,subLeadingPhoton.r9)>0.94",0),
                        ("EBLowR9","max(abs(leadingPhoton.superCluster.eta),abs(leadingPhoton.superCluster.eta))<1.4442",0),
                        ("EEHighR9","min(leadingPhoton.r9,subLeadingPhoton.r9)>0.94",0),
                        ("EELowR9","1",0),
                        ],
                       variables=["CMS_hgg_mass[320,100,180]:=mass", 
                                  "leadPt                   :=leadingPhoton.pt",
                                  "subleadPt                :=subLeadingPhoton.pt",
                                  "minR9                    :=min(leadingPhoton.r9,subLeadingPhoton.r9)",
                                  "maxEta                   :=max(abs(leadingPhoton.superCluster.eta),abs(leadingPhoton.superCluster.eta))"
                                  ],
                       histograms=["CMS_hgg_mass>>mass(320,100,180)",
                                   "subleadPt:leadPt>>ptSubVsLead(180,20,200:180,20,200)",
                                   "minR9>>minR9(110,0,1.1)",
                                   "maxEta>>maxEta[0.,0.1,0.2,0.3,0.4,0.6,0.8,1.0,1.2,1.4442,1.566,1.7,1.8,2.,2.2,2.3,2.5]"
                                   ]
                       )


# customization for job splitting, lumi weighting, etc.
from flashgg.MetaData.JobConfig import customize
customize.setDefault("maxEvents",500)
customize.setDefault("targetLumi",1.e+4)
customize(process)

