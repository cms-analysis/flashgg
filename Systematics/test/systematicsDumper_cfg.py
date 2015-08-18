#!/usr/bin/env cmsRun
import FWCore.ParameterSet.Config as cms
import FWCore.Utilities.FileUtils as FileUtils
from FWCore.ParameterSet.VarParsing import VarParsing

from flashgg.MetaData.samples_utils import SamplesManager

## CMD LINE OPTIONS ##
options = VarParsing('analysis')
print options

processId = "tth"
targetMass = 120.
processId = "%s_%i" % (processId,int(targetMass))

# maxEvents is the max number of events processed of each file, not globally
options.maxEvents = -1
options.inputFiles = "file:myTagOutputFile_%s.root" % processId
options.outputFile = "ValidationTagsDump_%s.root" % processId
options.parseArguments()

## I/O SETUP ##
process = cms.Process("ValidationTagsDumper")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(-1))
process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32( 1 )
process.source = cms.Source ("PoolSource",
                             fileNames = cms.untracked.vstring(options.inputFiles))
process.flashggSystTagMerger = cms.EDProducer("TagMerger",src=cms.VInputTag("flashggTagSorter"))

if options.maxEvents > 0:
    process.source.eventsToProcess = cms.untracked.VEventRange('1:1-1:'+str(options.maxEvents))

process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string(options.outputFile))

process.extraDumpers = cms.Sequence()
systlabels = [""]
counter =0;
for r9 in ["HighR9","LowR9"]:
    for direction in ["Up","Down"]:
        systlabels.append("MCSmear%sEE%s01sigma" % (r9,direction))
        for var in ["Rho","Phi"]:
            systlabels.append("MCSmear%sEB%s%s01sigma" % (r9,var,direction))
        for region in ["EB","EE"]:
            systlabels.append("MCScale%s%s%s01sigma" % (r9,region,direction))

process.extraDumpers = cms.Sequence()
process.load("flashgg.Taggers.diphotonTagDumper_cfi") ##  import diphotonTagDumper 
import flashgg.Taggers.dumperConfigTools as cfgTools

process.diphotonDumper.className = "DiPhotonTagDumper"
process.diphotonDumper.src = "flashggSystTagMerger"
process.diphotonDumper.processId = processId
process.diphotonDumper.dumpTrees = True
process.diphotonDumper.dumpWorkspace = True
process.diphotonDumper.dumpHistos = True
process.diphotonDumper.quietRooFit = True
process.diphotonDumper.systLabel = ""

for systlabel in systlabels:
  cutstring = "hasSyst(\"%s\")"%systlabel
  #print "syst label ", systlabel

  cfgTools.addCategory(process.diphotonDumper,
                      "flashggUntaggedTag__%s"%systlabel,
                      cutbased=cutstring,
											systLabel=systlabel,
                            subcats=5, 
                                    variables=["CMS_hgg_mass[160,100,180]:=diPhoton().mass", 
                                    "leadPt                   :=diPhoton().leadingPhoton.pt",
                                    "subleadPt                :=diPhoton().subLeadingPhoton.pt",
                                    "diphoMVA                 :=diPhotonMVA().result",    
                                    "maxEta                   :=max(abs(diPhoton().leadingPhoton.superCluster.eta),abs(diPhoton().leadingPhoton.superCluster.eta))",
                                    "genZ           :=tagTruth().genPV().z",
                                    "vtxZ           :=diPhoton().vtx().z",
                                    "dZ             :=abs(tagTruth().genPV().z-diPhoton().vtx().z)"
  
                                    ],
                         histograms=["CMS_hgg_mass>>mass(160,100,180)",
                                     "subleadPt:leadPt>>ptLeadvsSub(180,20,200:180,20,200)",
                                     "diphoMVA>>diphoMVA(50,0,1)",
                                     "maxEta>>maxEta[0.,0.1,0.2,0.3,0.4,0.6,0.8,1.0,1.2,1.4442,1.566,1.7,1.8,2.,2.2,2.3,2.5]"
                                     ]
                      )
  
  cfgTools.addCategory(process.diphotonDumper,
                      "flashggVBFTag__%s"%systlabel,
                      cutbased=cutstring,
											systLabel=systlabel,
                            subcats=3,
                                    variables=["CMS_hgg_mass[160,100,180]:=diPhoton().mass", 
                                    "leadPt                   :=diPhoton().leadingPhoton.pt",
                                    "subleadPt                :=diPhoton().subLeadingPhoton.pt",
                                               "diphoMVA                 :=diPhotonMVA().result",    
                                    "maxEta                   :=max(abs(diPhoton().leadingPhoton.superCluster.eta),abs(diPhoton().leadingPhoton.superCluster.eta))",
                                                 "leadJetPt                :=leadingJet().pt",
                                                 "subleadJetPt             :=subLeadingJet().pt",
                                                 "VBFMVA                   :=VBFMVA().VBFMVAValue()",
                                               "genZ           :=tagTruth().genPV().z",
                                               "vtxZ           :=diPhoton().vtx().z",
                                               "dZ            :=abs(tagTruth().genPV().z-diPhoton().vtx().z)",
                                    ],
                         histograms=["CMS_hgg_mass>>mass(160,100,180)",
                                     "subleadPt:leadPt>>ptLeadvsSub(180,20,200:180,20,200)",
                                     "diphoMVA>>diphoMVA(50,0,1)",
                                     "maxEta>>maxEta[0.,0.1,0.2,0.3,0.4,0.6,0.8,1.0,1.2,1.4442,1.566,1.7,1.8,2.,2.2,2.3,2.5]",
                                        "subleadJetPt:leadJetPt>>JetptLeadvsSub(8,20,100:8,20,100)",
                                     "VBFMVA>>VBFMVA(50,0,1)"
                                     ]
                      )
  
  cfgTools.addCategory(process.diphotonDumper,
                      "flashggVHTightTag__%s"%systlabel,
                      cutbased=cutstring,
											systLabel=systlabel,
                            subcats=0,
                                      variables=["CMS_hgg_mass[160,100,180]:=diPhoton().mass", 
                                    "leadPt                   :=diPhoton().leadingPhoton.pt",
                                    "subleadPt                :=diPhoton().subLeadingPhoton.pt",
                                                 "diphoMVA                 :=diPhotonMVA().result",    
                                    "maxEta                   :=max(abs(diPhoton().leadingPhoton.superCluster.eta),abs(diPhoton().leadingPhoton.superCluster.eta))",
                                                   "nMuons                   :=muons().size()",
                                                   "nElectrons               :=electrons().size()",
                                                   "nJets                    :=jets().size()",
                                               "genZ           :=tagTruth().genPV().z",
                                               "vtxZ           :=diPhoton().vtx().z",
                                               "dZ            :=abs(tagTruth().genPV().z-diPhoton().vtx().z)",
                                    ],
                         histograms=["CMS_hgg_mass>>mass(160,100,180)",
                                     "subleadPt:leadPt>>ptLeadvsSub(180,20,200:180,20,200)",
                                     "diphoMVA>>diphoMVA(50,0,1)",
                                     "maxEta>>maxEta[0.,0.1,0.2,0.3,0.4,0.6,0.8,1.0,1.2,1.4442,1.566,1.7,1.8,2.,2.2,2.3,2.5]",
                                     "nMuons:nElectrons>>nElectronsVsMuons(2,0,2:2,0,2)",
                                     "nJets>>nJets(5,0,5)"
                                     ]
                         )
  
  cfgTools.addCategory(process.diphotonDumper,
                      "flashggVHLooseTag__%s"%systlabel,
                      cutbased=cutstring,
											systLabel=systlabel,
                            subcats=0,
                                      variables=["CMS_hgg_mass[160,100,180]:=diPhoton().mass", 
                                    "leadPt                   :=diPhoton().leadingPhoton.pt",
                                    "subleadPt                :=diPhoton().subLeadingPhoton.pt",
                                                 "diphoMVA                 :=diPhotonMVA().result",    
                                    "maxEta                   :=max(abs(diPhoton().leadingPhoton.superCluster.eta),abs(diPhoton().leadingPhoton.superCluster.eta))",
                                                   "nMuons                   :=muons().size()",
                                                   "nElectrons               :=electrons().size()",
                                                   "nJets                    :=jets().size()",
                                               "genZ           :=tagTruth().genPV().z",
                                               "vtxZ           :=diPhoton().vtx().z",
                                               "dZ            :=abs(tagTruth().genPV().z-diPhoton().vtx().z)",
                                    ],
                         histograms=["CMS_hgg_mass>>mass(160,100,180)",
                                     "subleadPt:leadPt>>ptLeadvsSub(180,20,200:180,20,200)",
                                     "diphoMVA>>diphoMVA(50,0,1)",
                                     "maxEta>>maxEta[0.,0.1,0.2,0.3,0.4,0.6,0.8,1.0,1.2,1.4442,1.566,1.7,1.8,2.,2.2,2.3,2.5]",
                                     "nMuons:nElectrons>>nElectronsVsMuons(2,0,2:2,0,2)",
                                     "nJets>>nJets(5,0,5)"
                                     ]
                         )
  
  
  cfgTools.addCategory(process.diphotonDumper,
                      "flashggVHHadronicTag__%s"%systlabel,
                      cutbased=cutstring,
											systLabel=systlabel,
                            subcats=0,
                                    variables=["CMS_hgg_mass[160,100,180]:=diPhoton().mass", 
                                    "leadPt                   :=diPhoton().leadingPhoton.pt",
                                    "subleadPt                :=diPhoton().subLeadingPhoton.pt",
                                               "diphoMVA                 :=diPhotonMVA().result",    
                                    "maxEta                   :=max(abs(diPhoton().leadingPhoton.superCluster.eta),abs(diPhoton().leadingPhoton.superCluster.eta))",
                                                 "leadJetPt                :=leadingJet().pt",
                                                 "subleadJetPt             :=subLeadingJet().pt",
                                               "genZ           :=tagTruth().genPV().z",
                                               "vtxZ           :=diPhoton().vtx().z",
                                               "dZ            :=abs(tagTruth().genPV().z-diPhoton().vtx().z)",
                                    ],
                         ## histograms to be plotted. 
                         ## the variables need to be defined first
                         histograms=["CMS_hgg_mass>>mass(160,100,180)",
                                     "subleadPt:leadPt>>ptLeadvsSub(180,20,200:180,20,200)",
                                     "diphoMVA>>diphoMVA(50,0,1)",
                                     "maxEta>>maxEta[0.,0.1,0.2,0.3,0.4,0.6,0.8,1.0,1.2,1.4442,1.566,1.7,1.8,2.,2.2,2.3,2.5]",
                                        "subleadJetPt:leadJetPt>>JetptLeadvsSub(8,20,100:18,20,200)",
                                     ]
                      )
  
  cfgTools.addCategory(process.diphotonDumper,
                      "flashggTTHLeptonicTag__%s"%systlabel,
                      cutbased=cutstring,
											systLabel=systlabel,
                            subcats=0,
                                   variables=["CMS_hgg_mass[160,100,180]:=diPhoton().mass", 
                                    "leadPt                   :=diPhoton().leadingPhoton.pt",
                                    "subleadPt                :=diPhoton().subLeadingPhoton.pt",
                                              "diphoMVA                 :=diPhotonMVA().result",    
                                    "maxEta                   :=max(abs(diPhoton().leadingPhoton.superCluster.eta),abs(diPhoton().leadingPhoton.superCluster.eta))",
                                                "nMuons                   :=getMuons().size()",
                                                "nElectrons               :=getElectrons().size()",
                                                "nJets                    :=getJets().size()",
                                                "nBJets                   :=getBJets().size()",
                                               "genZ           :=tagTruth().genPV().z",
                                               "vtxZ           :=diPhoton().vtx().z",
                                               "dZ            :=abs(tagTruth().genPV().z-diPhoton().vtx().z)",
                                              "centralWeight := centralWeight"
  #                                            "MuonWeightDown01sigma : weight('MuonWeightDown01sigma')",
  #                                            "MuonWeightUp01sigma : weight('MuonWeightUp01sigma')",
  #                                            "ElectronWeightDown01sigma : weight('ElectronWeightDown01sigma')",
  #                                            "ElectronWeightUp01sigma : weight('ElectronWeightUp01sigma')",
                                    ],
                         ## histograms to be plotted. 
                         ## the variables need to be defined first
                         histograms=["CMS_hgg_mass>>mass(160,100,180)",
                                     "subleadPt:leadPt>>ptLeadvsSub(180,20,200:180,20,200)",
                                     "diphoMVA>>diphoMVA(50,0,1)",
                                     "maxEta>>maxEta[0.,0.1,0.2,0.3,0.4,0.6,0.8,1.0,1.2,1.4442,1.566,1.7,1.8,2.,2.2,2.3,2.5]",
                                     "nMuons:nElectrons>>nElectronsVsMuons(2,0,2:2,0,2)",
                                     "nJets>>nJets(5,0,5)",
                                     "nBJets>>nBJets(5,0,5)"
                                     ]
                      )
  
  cfgTools.addCategory(process.diphotonDumper,
                      "flashggTTHHadronicTag__%s"%systlabel,
                      cutbased=cutstring,
											systLabel=systlabel,
                            subcats=0,
                                     variables=["CMS_hgg_mass[160,100,180]:=diPhoton().mass", 
                                    "leadPt                   :=diPhoton().leadingPhoton.pt",
                                    "subleadPt                :=diPhoton().subLeadingPhoton.pt",
                                                "diphoMVA                 :=diPhotonMVA().result",    
                                    "maxEta                   :=max(abs(diPhoton().leadingPhoton.superCluster.eta),abs(diPhoton().leadingPhoton.superCluster.eta))",
                                                  "nJets                    :=getJetVector().size()",
                                                  "nBJets                   :=getBJetVector().size()",
                                               "genZ           :=tagTruth().genPV().z",
                                               "vtxZ           :=diPhoton().vtx().z",
                                               "dZ            :=abs(tagTruth().genPV().z-diPhoton().vtx().z)",
  
                                    ],
                         ## histograms to be plotted. 
                         ## the variables need to be defined first
                         histograms=["CMS_hgg_mass>>mass(160,100,180)",
                                     "subleadPt:leadPt>>ptLeadvsSub(180,20,200:180,20,200)",
                                     "diphoMVA>>diphoMVA(50,0,1)",
                                     "maxEta>>maxEta[0.,0.1,0.2,0.3,0.4,0.6,0.8,1.0,1.2,1.4442,1.566,1.7,1.8,2.,2.2,2.3,2.5]",
                                     "nJets>>nJets(5,0,5)",
                                     "nBJets>>nBJets(5,0,5)"
                                     ]
                      )

process.p1 = cms.Path(
    process.diphotonDumper
   # + process.extraDumpers
    )

print process.p1
