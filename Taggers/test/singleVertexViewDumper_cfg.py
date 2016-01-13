#!/usr/bin/env cmsRun

import FWCore.ParameterSet.Config as cms
import FWCore.Utilities.FileUtils as FileUtils

process = cms.Process("Analysis")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.source = cms.Source("PoolSource",
                            fileNames=cms.untracked.vstring(
        )
)
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )
process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32( 1000 )

process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string("test.root")
)

from flashgg.MicroAOD.flashggPreselectedDiPhotons_cfi import flashggPreselectedDiPhotons
process.kinPreselDiPhotons = flashggPreselectedDiPhotons.clone(
cut=cms.string(
        "mass > 100"
        " && leadingPhoton.pt > 30 && subLeadingPhoton.pt > 30"
        " && abs(leadingPhoton.superCluster.eta)<2.5 && abs(subLeadingPhoton.superCluster.eta)<2.5 "
        " && ( abs(leadingPhoton.superCluster.eta)<1.4442 || abs(leadingPhoton.superCluster.eta)>1.566)"
        " && ( abs(subLeadingPhoton.superCluster.eta)<1.4442 || abs(subLeadingPhoton.superCluster.eta)>1.566)"
        ## " && leadingPhoton.genMatchType != subLeadingPhoton.genMatchType " ## selects only prompt-fake pairs
        )
                                                              )


process.flashggSingleVertexViews = cms.EDProducer("FlashggSingleVertexViewProducer",
                                          DiPhotonTag=cms.untracked.InputTag('kinPreselDiPhotons'),
                                          maxCandidates = cms.int32(1)
                                          )

process.load("flashgg.Taggers.vertexViewDumper_cfi") ##  import diphotonDumper 
import flashgg.Taggers.dumperConfigTools as cfgTools

process.vertexViewDumper.dumpTrees = True
process.vertexViewDumper.dumpWorkspace = False
process.vertexViewDumper.quietRooFit = True

# interestng categories 
cfgTools.addCategories(process.vertexViewDumper,
                       ## categories definition
                       ## cuts are applied in cascade. Events getting to these categories have already failed the "Reject" selection
                       [("trueVertexes","isClosestToGen",0),
                        ("puVertexes",  "!isClosestToGen",0),
                        ],
                       ## variables to be dumped in trees/datasets. Same variables for all categories
                       ## if different variables wanted for different categories, can add categorie one by one with cfgTools.addCategory
                       variables=["logSumPt2","ptBal","ptAsym","nConv","pullConv",
                                  "vx := pos.x", "vy := pos.y", "vx := pos.y", 
                                  "dZtoGen"
                                  ],
                       ## histograms to be plotted. 
                       ## the variables need to be defined first
                       histograms=["dZtoGen>>dZtoGen(100,-10.,10.)",
                                   "logSumPt2>>logSumPt2(100,-2,10)",
                                   ]
                       )



process.p1 = cms.Path(
    process.kinPreselDiPhotons*process.flashggSingleVertexViews*process.vertexViewDumper
    )



from flashgg.MetaData.JobConfig import customize
customize.setDefault("maxEvents",100)
customize(process)

