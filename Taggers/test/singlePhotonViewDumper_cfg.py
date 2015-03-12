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


process.flashggSinglePhotonViews = cms.EDProducer("FlashggSinglePhotonViewProducer",
                                          DiPhotonTag=cms.untracked.InputTag('kinPreselDiPhotons'),
                                                  
                                          )

process.load("flashgg.Taggers.photonViewDumper_cfi") ##  import diphotonDumper 
import flashgg.Taggers.dumperConfigTools as cfgTools

process.photonViewDumper.src = "flashggSinglePhotonViews"
process.photonViewDumper.dumpTrees = True
process.photonViewDumper.dumpWorkspace = False
process.photonViewDumper.quietRooFit = True

## define categories and associated objects to dump
cfgTools.addCategory(process.photonViewDumper,
                     "Reject",
                     "abs(photon.superCluster.eta)>=1.4442&&abs(photon.superCluster.eta)<=1.566||abs(photon.superCluster.eta)>=2.5",
                     -1 ## if nSubcat is -1 do not store anythings
                     )

# interestng categories 
cfgTools.addCategories(process.photonViewDumper,
                       ## categories definition
                       ## cuts are applied in cascade. Events getting to these categories have already failed the "Reject" selection
                       [("promptPhotons","photon.genMatchType == 1",0),
                        ("fakePhotons",  "photon.genMatchType != 1",0),
                        ],
                       ## variables to be dumped in trees/datasets. Same variables for all categories
                       ## if different variables wanted for different categories, can add categorie one by one with cfgTools.addCategory
                       variables=["pt := photon.pt","energy := photon.energy","eta := photon.eta","phi := photon.phi","scEta:=photon.superCluster.eta",
                                  "r9 := photon.r9",
                                  "chgIsoWrtWorstVtx := photon.getpfChgIsoWrtWorstVtx03",
                                  "chgIsoWrtChosenVtx := pfChIso03WrtChosenVtx",
                                  "idMVA := phoIdMvaWrtChosenVtx",
                                  "genIso := photon.userFloat('genIso')", "eTrue := ? photon.hasMatchedGenPhoton ? photon.matchedGenPhoton.energy : 0"
                                  ],
                       ## histograms to be plotted. 
                       ## the variables need to be defined first
                       histograms=["r9>>r9(110,0,1.1)",
                                   "scEta>>scEta(100,-2.5,2.5)"
                                   ]
                       )



process.p1 = cms.Path(
    process.kinPreselDiPhotons*process.flashggSinglePhotonViews*process.photonViewDumper
    )



from flashgg.MetaData.JobConfig import customize
customize.setDefault("maxEvents",100)
customize(process)

