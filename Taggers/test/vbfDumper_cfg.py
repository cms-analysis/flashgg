#!/usr/bin/env cmsRun

import FWCore.ParameterSet.Config as cms
import FWCore.Utilities.FileUtils as FileUtils
from FWCore.ParameterSet.VarParsing import VarParsing

from flashgg.MetaData.samples_utils import SamplesManager

## CMD LINE OPTIONS ##
options = VarParsing('analysis')
print options

# maxEvents is the max number of events processed of each file, not globally
options.maxEvents = -1
options.inputFiles = "file:myTagOutputFile.root" 
options.outputFile = "VBFTagsDump.root" 
options.parseArguments()

## I/O SETUP ##
process = cms.Process("VBFTagsDumper")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(-1))
process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32( 1 )
process.source = cms.Source ("PoolSource",
                             fileNames = cms.untracked.vstring(options.inputFiles))

if options.maxEvents > 0:
    process.source.eventsToProcess = cms.untracked.VEventRange('1:1-1:'+str(options.maxEvents))

process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string(options.outputFile))

from flashgg.Taggers.tagsDumpers_cfi import createTagDumper
import flashgg.Taggers.dumperConfigTools as cfgTools

process.vbfTagDumper = createTagDumper("VBFTag")
process.vbfTagDumper.dumpTrees = True
process.vbfTagDumper.dumpHistos = True

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
                        "genZ           :=tagTruth().genPV().z",
                        "vtxZ           :=diPhoton().vtx().z",
                        "dZ            :=abs(tagTruth().genPV().z-diPhoton().vtx().z)",
                        "leadQuarkPt                  := tagTruth().pt_Q1",
                        "matchingGenJetToLeadPt    := tagTruth().pt_genPartMatchingToJ1"
                        ],
                       histograms=["CMS_hgg_mass>>mass(160,100,180)",
                                   "subleadPt:leadPt>>ptLeadvsSub(180,20,200:180,20,200)",
                                   "diphoMVA>>diphoMVA(50,0,1)",
                                   "maxEta>>maxEta[0.,0.1,0.2,0.3,0.4,0.6,0.8,1.0,1.2,1.4442,1.566,1.7,1.8,2.,2.2,2.3,2.5]",
                                   "subleadJetPt:leadJetPt>>JetptLeadvsSub(8,20,100:8,20,100)",
                                   "VBFMVA>>VBFMVA(50,0,1)"
                                   ]
                       )



process.p1 = cms.Path(
    process.vbfTagDumper
    )

print process.p1
