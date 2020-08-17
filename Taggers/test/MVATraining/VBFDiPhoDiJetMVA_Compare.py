import FWCore.ParameterSet.Config as cms
import FWCore.Utilities.FileUtils as FileUtils
import FWCore.ParameterSet.VarParsing as VarParsing
from flashgg.MetaData.samples_utils import SamplesManager

process = cms.Process("TestVBFDiPhoDiJetMVACompare")
process.load("FWCore.MessageService.MessageLogger_cfi")
process.load("Configuration.StandardSequences.GeometryDB_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_mc')
#process.GlobalTag.globaltag = 'POSTLS170_V5::All'

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )
process.source = cms.Source ("PoolSource",fileNames = cms.untracked.vstring())
#process.source = cms.Source ("PoolSource",fileNames = cms.untracked.vstring( "/store/group/phys_higgs/cmshgg/sethzenz/flashgg/HggPhys14/Phys14MicroAODV2/VBF_HToGG_M-125_13TeV-powheg-pythia6/HggPhys14-Phys14MicroAODV2-v0-Phys14DR-PU20bx25_PHYS14_25_V1-v1/150210_160130/0000/myMicroAODOutputFile_1.root"))

process.load("flashgg/Taggers/flashggTagSequence_cfi")
process.load("flashgg/Taggers/flashggTagTester_cfi")

from flashgg.Taggers.flashggTagOutputCommands_cff import tagDefaultOutputCommand

process.out = cms.OutputModule("PoolOutputModule",
	fileName = cms.untracked.string('myTagOutputFile.root'),
	outputCommands = tagDefaultOutputCommand
	)
process.TFileService = cms.Service("TFileService",
	fileName = cms.string("histo.root"),
	closeFileFast = cms.untracked.bool(True)
	)

import flashgg.Taggers.dumperConfigTools as cfgTools

process.load("flashgg/Taggers/VBFMVADumperNew_cff")

process.VBFMVADumperNew.dumpTrees = True
process.VBFMVADumperNew.dumpWorkspace = False
process.VBFMVADumperNew.quietRooFit = True

cfgTools.addCategories(process.VBFMVADumperNew,
	[## cuts are applied in cascade
	 ("GoodVBFNew","dijet_LeadJPt>=0",0)
	],
	variables=[
	"dijet_abs_dEta := dijet_abs_dEta ",
	"dijet_leadEta := dijet_leadEta ",
	"dijet_subleadEta := dijet_subleadEta ",
	"dijet_LeadJPt := dijet_LeadJPt ",
	"dijet_SubJPt := dijet_SubJPt ",
	"dijet_Zep := dijet_Zep ",
	"dijet_Mjj := dijet_Mjj ",
	"dipho_PToM := dipho_PToM ",
	"leadPho_PToM := leadPho_PToM ",
	"sublPho_PToM := sublPho_PToM ",
	"dijet_dPhi_trunc := dijet_dPhi_trunc ",
	"VBFMVAValue := VBFMVAValue ",
	],
	histograms=[
	"VBFMVAValue>>VBFMVAValue(100,-1,1)",
	]
	)

# split tree, histogram and datasets by process
process.VBFMVADumperNew.nameTemplate ="$PROCESS_$SQRTS_$LABEL_$SUBCAT"
process.load("flashgg/Taggers/VBFMVADumper_cff")
process.VBFMVADumper.dumpTrees = True
process.VBFMVADumper.dumpWorkspace = False
process.VBFMVADumper.quietRooFit = True

cfgTools.addCategories(process.VBFMVADumper,
	[## cuts are applied in cascade
	("GoodVBFLeg","dijet_LeadJPt>=0",0)
	],
	variables=[
	"dijet_abs_dEta := dijet_abs_dEta ",
	"dijet_leadEta := dijet_leadEta ",
	"dijet_subleadEta := dijet_subleadEta ",
	"dijet_LeadJPt := dijet_LeadJPt ",
	"dijet_SubJPt := dijet_SubJPt ",
	"dijet_Zep := dijet_Zep ",
	"dijet_Mjj := dijet_Mjj ",
	"dipho_PToM := dipho_PToM ",
	"leadPho_PToM := leadPho_PToM ",
	"sublPho_PToM := sublPho_PToM ",
	"dijet_dPhi_trunc := dijet_dPhi_trunc ",
	"VBFMVAValue := VBFMVAValue ",
	],
	histograms=[
	"VBFMVAValue>>VBFMVAValue(100,-1,1)",
	]
	)

# split tree, histogram and datasets by process
process.VBFMVADumper.nameTemplate ="$PROCESS_$SQRTS_$LABEL_$SUBCAT"
#############################################################

process.load("flashgg/Taggers/VBFDiPhoDiJetMVADumperNew_cff")

process.VBFDiPhoDiJetMVADumperNew.dumpTrees = True
process.VBFDiPhoDiJetMVADumperNew.dumpWorkspace = False
process.VBFDiPhoDiJetMVADumperNew.quietRooFit = True

cfgTools.addCategories(process.VBFDiPhoDiJetMVADumperNew,
	[## cuts are applied in cascade
	 ("GoodVBFDiPhoDiJetNew","dipho_PToM>=0",0)
	],
	variables=[
			"dijet_mva :=  dijet_mva",
			"dipho_mva :=  dipho_mva",
			"dipho_PToM :=  dipho_PToM",
			"vbfDiPhoDiJetMvaResult := vbfDiPhoDiJetMvaResult",
	],
	histograms=[
			"vbfDiPhoDiJetMvaResult>>VBFDiPhoDiJetMVAValue(100,-1,1)",
	]
	)

# split tree, histogram and datasets by process
process.VBFDiPhoDiJetMVADumperNew.nameTemplate ="$PROCESS_$SQRTS_$LABEL_$SUBCAT"
process.load("flashgg/Taggers/VBFDiPhoDiJetMVADumper_cff")
process.VBFDiPhoDiJetMVADumper.dumpTrees = True
process.VBFDiPhoDiJetMVADumper.dumpWorkspace = False
process.VBFDiPhoDiJetMVADumper.quietRooFit = True

cfgTools.addCategories(process.VBFDiPhoDiJetMVADumper,
	[## cuts are applied in cascade
	("GoodVBFDiPhoDiJetLeg","dipho_PToM>=0",0)
	],
	variables=[
			"dijet_mva :=  dijet_mva",
			"dipho_mva :=  dipho_mva",
			"dipho_PToM :=  dipho_PToM",
			"vbfDiPhoDiJetMvaResult := vbfDiPhoDiJetMvaResult",
	],
	histograms=[
			"vbfDiPhoDiJetMvaResult>>VBFDiPhoDiJetMVAValue(100,-1,1)",
	]
	)

# split tree, histogram and datasets by process
process.VBFDiPhoDiJetMVADumper.nameTemplate ="$PROCESS_$SQRTS_$LABEL_$SUBCAT"

# customization for job splitting, lumi weighting, etc.
from flashgg.MetaData.JobConfig import customize

customize.setDefault("maxEvents",-1)
customize.setDefault("targetLumi",1.e+4)
customize(process)

process.p = cms.Path(process.flashggTagSequence*
	process.VBFMVADumper*
	process.VBFMVADumperNew*
	process.VBFDiPhoDiJetMVADumper*
	process.VBFDiPhoDiJetMVADumperNew
	)

process.e = cms.EndPath(process.out)
