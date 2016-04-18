# ================================================
#   Both jet tree and VBF tree at the same time
#   Y. Haddad 01/2015, S Zenz 04/2016
# ================================================

import FWCore.ParameterSet.Config as cms
import FWCore.Utilities.FileUtils as FileUtils

process = cms.Process("FLASHggJetValidation")

# +++++ the number of processed events
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32( 250 ) )
process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32( 100 )
process.MessageLogger.cerr.threshold = 'ERROR'

# +++++ the source file
process.source = cms.Source("PoolSource",
                            fileNames=cms.untracked.vstring("root://eoscms.cern.ch//eos/cms//store/group/phys_higgs/cmshgg/ferriff/flashgg/RunIIFall15DR76-1_3_0-25ns_ext1/1_3_1/VBFHToGG_M-120_13TeV_powheg_pythia8/RunIIFall15DR76-1_3_0-25ns_ext1-1_3_1-v0-RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/160210_045711/0000/myMicroAODOutputFile_1.root"))

process.TFileService = cms.Service("TFileService",fileName  = cms.string("jetValidationTrees_VBF_HToGG_test.root"))

process.load("flashgg/Taggers/flashggTagSequence_cfi")

from flashgg.Taggers.flashggTags_cff import UnpackedJetCollectionVInputTag
process.flashggJetValidationTreeMakerPFCHS = cms.EDAnalyzer('FlashggJetValidationTreeMaker',
                                                            GenParticleTag  = cms.untracked.InputTag('flashggPrunedGenParticles'),
                                                            DiPhotonTag     = cms.InputTag('flashggPreselectedDiPhotons'),
                                                            inputTagJets    = UnpackedJetCollectionVInputTag,
                                                            StringTag	    = cms.string("PFCHS"),
                                                            )

import flashgg.Taggers.dumperConfigTools as cfgTools
from  flashgg.Taggers.tagsDumpers_cfi import createTagDumper

process.VBFTagDumper = createTagDumper("VBFTag")
process.VBFTagDumper.dumpTrees     = True
process.VBFTagDumper.dumpHistos    = False
process.VBFTagDumper.dumpWorkspace = False

import flashgg.Taggers.VBFTagVariables as var
new_variables = [
    "category := categoryNumber"
    ]
matching_photon = [
    "prompt_pho_1   := diPhoton.leadingPhoton.genMatchType()",
    "prompt_pho_2   := diPhoton.subLeadingPhoton.genMatchType()"
    ]
all_variables = var.dipho_variables + var.dijet_variables + new_variables
all_variables += var.truth_variables + matching_photon

cats = []
cats += [("VBF","1",0)]

cfgTools.addCategories(process.VBFTagDumper,
                       cats,
                       variables  = all_variables,
                              histograms = []
)

process.VBFTagDumper.nameTemplate = "$PROCESS_$SQRTS_$CLASSNAME_$SUBCAT_$LABEL"


process.options = cms.untracked.PSet( allowUnscheduled = cms.untracked.bool(True) )

process.p = cms.Path( 
    process.flashggTagSequence*
    process.flashggJetValidationTreeMakerPFCHS *
    process.VBFTagDumper
    )

