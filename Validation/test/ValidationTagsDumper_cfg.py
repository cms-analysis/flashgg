#!/usr/bin/env cmsRun

import FWCore.ParameterSet.Config as cms
import FWCore.Utilities.FileUtils as FileUtils
from FWCore.ParameterSet.VarParsing import VarParsing

from flashgg.MetaData.samples_utils import SamplesManager

## CMD LINE OPTIONS ##
options = VarParsing('analysis')

# maxEvents is the max number of events processed of each file, not globally
options.maxEvents = -1
options.inputFiles = "../../myTagOutputFile.root"
options.outputFile = "ValidationTagsDump.root"
options.parseArguments()

## I/O SETUP ##
process = cms.Process("ValidationTagsDumper")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(-1))
process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32( 1 )
process.source = cms.Source ("PoolSource",
                             fileNames = cms.untracked.vstring(options.inputFiles),
                             eventsToProcess = cms.untracked.VEventRange('1:1-1:'+str(options.maxEvents)))

process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string(options.outputFile))

## TAGS DUMPERS ##
from flashgg.Taggers.tagsDumpers_cfi import *

process.untaggedDumper = createTagDumper("Untagged")
process.vbfTagDumper = createTagDumper("VBFTag")
process.tthLeptonicTagDumper = createTagDumper("TTHLeptonicTag")
process.tthHadronicTagDumper = createTagDumper("TTHHadronicTag")
process.vhLooseTagDumper = createTagDumper("VHLooseTag")
process.vhTightTagDumper = createTagDumper("VHTightTag")
process.vhHadronicTagDumper = createTagDumper("VHHadronicTag")

process.untaggedDumper.dumpTrees = True
process.vbfTagDumper.dumpTrees = True
process.tthLeptonicTagDumper.dumpTrees = True
process.tthHadronicTagDumper.dumpTrees = True
process.vhLooseTagDumper.dumpTrees = True
process.vhTightTagDumper.dumpTrees = True
process.vhHadronicTagDumper.dumpTrees = True

## define categories and associated objects to dump
import flashgg.Taggers.dumperConfigTools as cfgTools

dipho_variables=["dipho_sumpt      := diPhoton.sumPt",
                 "dipho_cosphi     := abs(cos(diPhoton.leadingPhoton.phi - diPhoton.subLeadingPhoton.phi))",
                 "mass             := diPhoton.mass",
                 "leadPt           := diPhoton.leadingPhoton.pt",
                 "leadEt           := diPhoton.leadingPhoton.et",
                 "leadEta          := diPhoton.leadingPhoton.eta",
                 "leadPhi          := diPhoton.leadingPhoton.phi",
                 "lead_sieie       := diPhoton.leadingPhoton.sigmaIetaIeta",
                 "lead_hoe         := diPhoton.leadingPhoton.hadronicOverEm",
                 "lead_sigmaEoE    := diPhoton.leadingPhoton.sigEOverE",
                 "lead_ptoM        := diPhoton.leadingPhoton.pt/diPhoton.mass",
                 "leadR9           := diPhoton.leadingPhoton.r9",
                 "subleadPt        := diPhoton.subLeadingPhoton.pt",
                 "subleadEt        := diPhoton.subLeadingPhoton.et",
                 "subleadEta       := diPhoton.subLeadingPhoton.eta",
                 "subleadPhi       := diPhoton.subLeadingPhoton.phi",
                 "sublead_sieie    := diPhoton.subLeadingPhoton.sigmaIetaIeta",
                 "sublead_hoe      := diPhoton.subLeadingPhoton.hadronicOverEm",
                 "sublead_sigmaEoE := diPhoton.subLeadingPhoton.sigEOverE",
                 "sublead_ptoM     := diPhoton.subLeadingPhoton.pt/diPhoton.mass",
                 "subleadR9        := diPhoton.subLeadingPhoton.r9",
                 "leadIDMVA        := diPhoton.leadingView.phoIdMvaWrtChosenVtx",
                 "subleadIDMVA     := diPhoton.subLeadingView.phoIdMvaWrtChosenVtx",]

cfgTools.addCategories(process.untaggedDumper,
                       ## categories definition
                       [("all","1",0)
                    ],                       
                       ## variables to be dumped in trees/datasets. Same variables for all categories
                       variables=dipho_variables,

                       ## histograms
                       histograms=[]
)
                       

## VBF ##
cfgTools.addCategories(process.vbfTagDumper,
                       ## categories definition
                       [("all","1",0)
                    ],                       
                       ## variables to be dumped in trees/datasets. Same variables for all categories
                       variables=dipho_variables+                       
                       ["dRphoLeadJ    := min(deltaR(leadingJet.eta, leadingJet.phi, diPhoton.leadingPhoton.eta, diPhoton.leadingPhoton.phi), deltaR(leadingJet.eta, leadingJet.phi, diPhoton.subLeadingPhoton.eta, diPhoton.subLeadingPhoton.phi))",
                        "dRphoSubleadJ := min(deltaR(subLeadingJet.eta, subLeadingJet.phi, diPhoton.leadingPhoton.eta, diPhoton.leadingPhoton.phi), deltaR(subLeadingJet.eta, subLeadingJet.phi, diPhoton.subLeadingPhoton.eta, diPhoton.subLeadingPhoton.phi))",
                        "leadJPt       := leadingJet.pt",
                        "leadJEta      := leadingJet.eta",
                        "subleadJPt    := subLeadingJet.pt",
                        "subleadJEta   := leadingJet.eta",
                        "Mjj           := sqrt((leadingJet.energy+subLeadingJet.energy)^2-(leadingJet.px+subLeadingJet.px)^2-(leadingJet.py+subLeadingJet.py)^2-(leadingJet.pz+subLeadingJet.pz)^2)",
                        "dijet_dEta    := abs(leadingJet.eta - subLeadingJet.eta)",
                        "dijet_dPhi    := deltaPhi(leadingJet.phi, subLeadingJet.phi)",  
                        "dijet_Zep     := VBFMVA.dijet_Zep",
                        "dijet_MVA     := VBFMVA.VBFMVAValue",
                        "vbfcat        := categoryNumber",
                    ],
                       ## histograms
                       histograms=[]
)

## TTH leptonic ##
cfgTools.addCategories(process.tthLeptonicTagDumper,
                       ## categories definition
                       [("all","1",0)
                    ],                       
                       ## variables to be dumped in trees/datasets. Same variables for all categories
                       variables=dipho_variables+
                       ["n_ele    := electrons.size",
                        "ele1_pt  := ?(electrons.size>0)? electrons.at(0).pt : -1",
                        "ele2_pt  := ?(electrons.size>1)? electrons.at(1).pt : -1",
                        "n_muons  := muons.size",
                        # "muon1_pt := ?(muons.size>0)? muons.at(0).pt : -1",
                        # "muon2_pt := ?(muons.size>1)? muons.at(1).pt : -1",
                        "n_bjets  := bJets.size",
                        "n_jets   := jets.size",
                        "bjet1_pt := bJets.at(0).pt",
                        "bjet2_pt := ?bJets.size>1? bJets.at(1).pt : -1",
                        "jet1_pt  := ?(jets.size>0)? jets.at(0).pt : -1",
                        "jet2_pt  := ?(jets.size>1)? jets.at(1).pt : -1",
                        "Mjj      := ?(jets.size>1)?"
                        +"sqrt((jets.at(0).energy+jets.at(1).energy)^2-(jets.at(0).px+jets.at(1).px)^2-(jets.at(0).py+jets.at(1).py)^2-(jets.at(0).pz+jets.at(1).pz)^2)"
                        +": -1",
                    ],
                       ## histograms
                       histograms=[]
)

## TTH hadronic ##
cfgTools.addCategories(process.tthHadronicTagDumper,
                       ## categories definition
                       [("all","1",0)
                    ],                       
                       ## variables to be dumped in trees/datasets. Same variables for all categories
                       variables=dipho_variables+
                       ["n_bjets  := nBMedium",
                        "n_jets   := jetVector.size",
                        "bjet1_pt := bJetVector.at(0).pt",
                        "bjet2_pt := ?nBMedium>1? bJetVector.at(1).pt : -1",
                        "jet1_pt  := ?(jetVector.size>0)? jetVector.at(0).pt : -1",
                        "jet2_pt  := ?(jetVector.size>1)? jetVector.at(1).pt : -1",
                        "jet3_pt  := ?(jetVector.size>2)? jetVector.at(2).pt : -1",
                        "jet4_pt  := ?(jetVector.size>3)? jetVector.at(3).pt : -1"
                    ],
                       ## histograms
                       histograms=[]
)

## VH Loose ##
cfgTools.addCategories(process.vhLooseTagDumper,
                       ## categories definition
                       [("all","1",0)
                    ],                       
                       ## variables to be dumped in trees/datasets. Same variables for all categories
                       variables=dipho_variables+
                       ["met      := ?met.size>0? met.at(0).NoShift : -1",
                        "n_ele    := electrons.size",
                        "ele1_pt  := ?(electrons.size>0)? electrons.at(0).pt : -1",
                        "ele2_pt  := ?(electrons.size>1)? electrons.at(1).pt : -1",
                        "n_muons  := muons.size",
                        # "muon1_pt := ?(muons.size>0)? muons.at(0).pt : -1",
                        # "muon2_pt := ?(muons.size>1)? muons.at(1).pt : -1",
                        "n_jets   := jets.size",
                        "jet1_pt  := ?(jets.size>0)? jets.at(0).pt : -1",
                        "jet2_pt  := ?(jets.size>1)? jets.at(1).pt : -1",
                        "Mjj      := ?(jets.size>1)?"
                        +"sqrt((jets.at(0).energy+jets.at(1).energy)^2-(jets.at(0).px+jets.at(1).px)^2-(jets.at(0).py+jets.at(1).py)^2-(jets.at(0).pz+jets.at(1).pz)^2)"
                        +": -1",
                    ],
                       ## histograms
                       histograms=[]
)

## VH Tight ##
cfgTools.addCategories(process.vhTightTagDumper,
                       ## categories definition
                       [("all","1",0)
                    ],                       
                       ## variables to be dumped in trees/datasets. Same variables for all categories
                       variables=dipho_variables+
                       ["met      := ?met.size>0? met.at(0).NoShift : -1",
                        "n_ele    := electrons.size",
                        "ele1_pt  := ?(electrons.size>0)? electrons.at(0).pt : -1",
                        "ele2_pt  := ?(electrons.size>1)? electrons.at(1).pt : -1",
                        "n_muons  := muons.size",
                        # "muon1_pt := ?(muons.size>0)? muons.at(0).pt : -1",
                        # "muon2_pt := ?(muons.size>1)? muons.at(1).pt : -1",
                        "n_jets   := jets.size",
                        "jet1_pt  := ?(jets.size>0)? jets.at(0).pt : -1",
                        "jet2_pt  := ?(jets.size>1)? jets.at(1).pt : -1",
                        "Mjj      := ?(jets.size>1)?"
                        +"sqrt((jets.at(0).energy+jets.at(1).energy)^2-(jets.at(0).px+jets.at(1).px)^2-(jets.at(0).py+jets.at(1).py)^2-(jets.at(0).pz+jets.at(1).pz)^2)"
                        +": -1",
                    ],
                       ## histograms
                       histograms=[]
)

## VH hadronic ##
cfgTools.addCategories(process.vhHadronicTagDumper,
                       ## categories definition
                       [("all","1",0)
                    ],                       
                       ## variables to be dumped in trees/datasets. Same variables for all categories
                       variables=dipho_variables+
                       ["dRphoLeadJ    := min(deltaR(leadingJet.eta, leadingJet.phi, diPhoton.leadingPhoton.eta, diPhoton.leadingPhoton.phi), deltaR(leadingJet.eta, leadingJet.phi, diPhoton.subLeadingPhoton.eta, diPhoton.subLeadingPhoton.phi))",
                        "dRphoSubleadJ := min(deltaR(subLeadingJet.eta, subLeadingJet.phi, diPhoton.leadingPhoton.eta, diPhoton.leadingPhoton.phi), deltaR(subLeadingJet.eta, subLeadingJet.phi, diPhoton.subLeadingPhoton.eta, diPhoton.subLeadingPhoton.phi))",
                        "leadJPt       := leadingJet.pt",
                        "leadJEta      := leadingJet.eta",
                        "subleadJPt    := subLeadingJet.pt",
                        "subleadJEta   := leadingJet.eta",
                        "Mjj           := sqrt((leadingJet.energy+subLeadingJet.energy)^2-(leadingJet.px+subLeadingJet.px)^2-(leadingJet.py+subLeadingJet.py)^2-(leadingJet.pz+subLeadingJet.pz)^2)",
                    ],
                       ## histograms
                       histograms=[]
)

process.p1 = cms.Path(process.untaggedDumper
                      +process.vbfTagDumper
                      +process.tthLeptonicTagDumper
                      +process.tthHadronicTagDumper
                      +process.vhLooseTagDumper                      
                      +process.vhTightTagDumper
                      +process.vhHadronicTagDumper)

