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
process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32(1000)
process.source = cms.Source ("PoolSource",fileNames = cms.untracked.vstring("/store/group/phys_higgs/cmshgg/sethzenz/flashgg/RunIISpring15-25ns/Spring15BetaV5/GluGluHToGG_M-120_13TeV_powheg_pythia8/RunIISpring15-25ns-Spring15BetaV5-v0-RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/150922_093229/0000/myMicroAODOutputFile_1.root"))
##process.source = cms.Source ("PoolSource",fileNames = cms.untracked.vstring("/store/group/phys_higgs/cmshgg/sethzenz/flashgg/RunIISpring15-25ns/Spring15BetaV5/GluGluHToGG_M-120_13TeV_powheg_pythia8/RunIISpring15-25ns-Spring15BetaV5-v0-RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/150922_093229/0000/myMicroAODOutputFile_1.root"))
#process.source = cms.Source ("PoolSource",fileNames = cms.untracked.vstring( "/store/group/phys_higgs/cmshgg/sethzenz/flashgg/HggPhys14/Phys14MicroAODV2/VBF_HToGG_M-125_13TeV-powheg-pythia6/HggPhys14-Phys14MicroAODV2-v0-Phys14DR-PU20bx25_PHYS14_25_V1-v1/150210_160130/0000/myMicroAODOutputFile_1.root"))


process.load("flashgg/Taggers/flashggTagSequence_cfi")
## process.load("flashgg/Taggers/flashggTagTester_cfi")


from flashgg.Taggers.flashggTagOutputCommands_cff import tagDefaultOutputCommand


process.TFileService = cms.Service("TFileService",
			fileName = cms.string("histo.root"),
			closeFileFast = cms.untracked.bool(True)
			)


process.flashggUntagged.Boundaries=cms.untracked.vdouble(-2)

import flashgg.Taggers.dumperConfigTools as cfgTools
from flashgg.Taggers.tagsDumpers_cfi import createTagDumper
# ## FIXME switch to preselected diphotons
#process.flashggDiPhotonMVANew.DiPhotonTag = "flashggPreselectedDiPhotons"

process.tagDumper = createTagDumper("UntaggedTag")
process.tagDumper.src = "flashggUntagged"

process.tagDumper.splitLumiWeight=cms.untracked.bool(True)
#process.tagDumper.throwOnUnclassified= False
process.tagDumper.dumpTrees = True
process.tagDumper.dumpWorkspace = False
process.tagDumper.quietRooFit = True

cfgTools.addCategory(process.tagDumper, "Reject",  "diPhoton.mass<100 || diPhoton.mass>180",
-1 ## if nSubcat is -1 do not store anythings
)

cfgTools.addCategories(process.tagDumper,
			[## cuts are applied in cascade
			("All","1",0),
			],
			variables=[
			"leadptom         := diPhotonMVA.leadptom  ",
			"subleadptom      := diPhotonMVA.subleadptom ",
			"leadmva          := diPhotonMVA.leadmva ",
			"subleadmva       := diPhotonMVA.subleadmva    ",
			"leadeta          := diPhotonMVA.leadeta     ",
			"subleadeta       := diPhotonMVA.subleadeta",
			"sigmarv          := diPhotonMVA.sigmarv",
     			"sigmawv          := diPhotonMVA.sigmawv",
			"CosPhi           := diPhotonMVA.CosPhi",
			"vtxprob          := diPhotonMVA.vtxprob",
			"result           := diPhotonMVA.result",
			"mass             := diPhoton.mass",
			"pt               := diPhoton.pt",
                        "dz               := abs(tagTruth().genPV().z-diPhoton().vtx().z)",
                        "leadMatchType    := diPhoton.leadingPhoton().genMatchType()",
                        "subleadMatchType := diPhoton.subLeadingPhoton().genMatchType()",
                        "leadptgen := ?diPhoton.leadingPhoton().hasMatchedGenPhoton()?diPhoton.leadingPhoton().matchedGenPhoton().pt():0",
                        "subleadptgen := ?diPhoton.subLeadingPhoton().hasMatchedGenPhoton()?diPhoton.subLeadingPhoton().matchedGenPhoton().pt():0",
                        "massgen := diPhoton.genP4().mass()"
			],
			histograms=[
			"result>>diphoMVAValue(100,-1,1)",
			]
			)
# split tree, histogram and datasets by process
process.tagDumper.nameTemplate ="$PROCESS_$SQRTS_$LABEL"

process.options = cms.untracked.PSet( allowUnscheduled = cms.untracked.bool(True) )

process.p = cms.Path( process.tagDumper )


# customization for job splitting, lumi weighting, etc.
from flashgg.MetaData.JobConfig import customize
customize.setDefault("maxEvents",-1)
customize.setDefault("targetLumi",1.e+4)
customize(process)
