import FWCore.ParameterSet.Config as cms
import FWCore.Utilities.FileUtils as FileUtils

import FWCore.ParameterSet.VarParsing as VarParsing
from flashgg.MetaData.samples_utils import SamplesManager

process = cms.Process("DiPhotonMVATrainig")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.load("Configuration.StandardSequences.GeometryDB_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_mc')
#process.GlobalTag.globaltag = 'POSTLS170_V5::All'
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )
process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32(1)
process.source = cms.Source ("PoolSource",fileNames = cms.untracked.vstring())
#process.source = cms.Source ("PoolSource",fileNames = cms.untracked.vstring( "/store/group/phys_higgs/cmshgg/sethzenz/flashgg/HggPhys14/Phys14MicroAODV2/VBF_HToGG_M-125_13TeV-powheg-pythia6/HggPhys14-Phys14MicroAODV2-v0-Phys14DR-PU20bx25_PHYS14_25_V1-v1/150210_160130/0000/myMicroAODOutputFile_1.root"))


process.load("flashgg/Taggers/flashggTagSequence_cfi")
process.load("flashgg/Taggers/flashggTagTester_cfi")


from flashgg.Taggers.flashggTagOutputCommands_cff import tagDefaultOutputCommand

process.out = cms.OutputModule("PoolOutputModule", fileName = cms.untracked.string('myMicroAODOutputFile.root'),
                               outputCommands =tagDefaultOutputCommand
                               )

import flashgg.Taggers.dumperConfigTools as cfgTools
process.load("flashgg/Taggers/DiPhotonMVADumperNew_cff")
process.TFileService = cms.Service("TFileService",
			fileName = cms.string("histo.root"),
			closeFileFast = cms.untracked.bool(True)
			)
process.DiPhotonMVADumperNew.dumpTrees = True
process.DiPhotonMVADumperNew.dumpWorkspace = False
process.DiPhotonMVADumperNew.quietRooFit = True

cfgTools.addCategories(process.DiPhotonMVADumperNew,
			[## cuts are applied in cascade
			("All","1",0),
			],
			variables=[
			"leadptom   :=  leadptom  ",
			"subleadptom    :=  subleadptom ",
			"leadmva :=  leadmva ",
			"subleadmva    :=  subleadmva    ",
			"leadeta     :=  leadeta     ",
			"subleadeta  := subleadeta",
			"sigmarv  := sigmarv",
     			"sigmawv  := sigmawv",
			"CosPhi := CosPhi",
			"vtxprob := vtxprob",
			"result := result",
			],
			histograms=[
			"result>>diphoMVAValue(100,-1,1)",
			]
			)
# split tree, histogram and datasets by process
process.DiPhotonMVADumperNew.nameTemplate ="$PROCESS_$SQRTS_$LABEL_$SUBCAT"


# customization for job splitting, lumi weighting, etc.
from flashgg.MetaData.JobConfig import customize
customize.setDefault("maxEvents",-1)
customize.setDefault("targetLumi",1.e+4)
customize(process)
process.p = cms.Path( process.flashggTagSequence
			*process.DiPhotonMVADumperNew
			)

process.e = cms.EndPath(process.out)
