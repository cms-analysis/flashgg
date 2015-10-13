import FWCore.ParameterSet.Config as cms
import FWCore.Utilities.FileUtils as FileUtils

process = cms.Process("FLASHggMicroAOD")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.load("Configuration.StandardSequences.GeometryDB_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")
#process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
#process.GlobalTag.globaltag = 'PLS170_V7AN1::All'
#process.GlobalTag.globaltag = 'auto:run2_mc'
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff")
from Configuration.AlCa.GlobalTag import GlobalTag

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32( 1000) )
process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32( 1000 )

process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_mc')

# Fix because auto:run2_mc points to MCRUN2_74_V9::All
current_gt = process.GlobalTag.globaltag.value()
if current_gt.count("::All"):
    new_gt = current_gt.replace("::All","")
    print 'Removing "::All" from GlobalTag by hand for condDBv2: was %s, now %s' % (current_gt,new_gt)
    process.GlobalTag.globaltag = new_gt


# 2012 data
#process.GlobalTag = GlobalTag(process.GlobalTag, 'GR_R_74_V8A::All')
#process.source = cms.Source("PoolSource",fileNames=cms.untracked.vstring(
#        "/store/relval/CMSSW_7_4_0_pre9/DoubleElectron/MINIAOD/GR_R_74_V8A_RelVal_zEl2012D-v1/00000/5A04EF0A-29D4-E411-BB12-003048FFCC2C.root",
#        "/store/relval/CMSSW_7_4_0_pre9/DoubleElectron/MINIAOD/GR_R_74_V8A_RelVal_zEl2012D-v1/00000/A6D4F50A-29D4-E411-98A2-002618943838.root",
#        "/store/relval/CMSSW_7_4_0_pre9/DoubleElectron/MINIAOD/GR_R_74_V8A_RelVal_zEl2012D-v1/00000/DE947AB8-FED3-E411-995F-0025905AA9F0.root"
#        ))

# PHYS14 Files
#process.source = cms.Source("PoolSource",fileNames=cms.untracked.vstring(
#"/store/mc/Phys14DR/DYToMuMu_M-50_Tune4C_13TeV-pythia8/MINIAODSIM/PU40bx25_tsg_castor_PHYS14_25_V1-v2/00000/622CAFBA-BD9A-E411-BE11-002481E14FFC.root",
#"/store/mc/Phys14DR/DYToMuMu_M-50_Tune4C_13TeV-pythia8/MINIAODSIM/PU40bx25_tsg_castor_PHYS14_25_V1-v2/00000/FA4B46B9-8E9A-E411-A899-002590A3C954.root",
#"/store/mc/Phys14DR/DYToMuMu_M-50_Tune4C_13TeV-pythia8/MINIAODSIM/PU40bx25_tsg_castor_PHYS14_25_V1-v2/10000/8607F88E-F799-E411-A180-0025B3E063F0.root",
#"/store/mc/Phys14DR/DYToMuMu_M-50_Tune4C_13TeV-pythia8/MINIAODSIM/PU40bx25_tsg_castor_PHYS14_25_V1-v2/10000/F620A7C9-F799-E411-8DEF-002590A371AC.root"
#))
#process.source = cms.Source("PoolSource",fileNames=cms.untracked.vstring("/store/mc/Phys14DR/GluGluToHToGG_M-125_13TeV-powheg-pythia6/MINIAODSIM/PU20bx25_tsg_PHYS14_25_V1-v1/00000/3C2EFAB1-B16F-E411-AB34-7845C4FC39FB.root"))
#process.source = cms.Source("PoolSource",fileNames=cms.untracked.vstring("/store/mc/Phys14DR/VBF_HToGG_M-125_13TeV-powheg-pythia6/MINIAODSIM/PU20bx25_PHYS14_25_V1-v1/00000/4A8E0BD1-026C-E411-8760-00266CFFA418.root"))
#process.source = cms.Source("PoolSource",fileNames=cms.untracked.vstring("/store/mc/Phys14DR/WH_ZH_HToGG_M-125_13TeV_pythia6/MINIAODSIM/PU20bx25_PHYS14_25_V1-v1/00000/24B70163-5769-E411-93CA-002590200A28.root"))
### process.source = cms.Source("PoolSource",fileNames=cms.untracked.vstring("/store/mc/Phys14DR/GJet_Pt40_doubleEMEnriched_TuneZ2star_13TeV-pythia6/MINIAODSIM/PU20bx25_PHYS14_25_V1-v1/00000/101611CC-026E-E411-B8D7-00266CFFBF88.root"))
## process.source = cms.Source("PoolSource",fileNames=cms.untracked.vstring("/store/mc/Phys14DR/GJets_HT-100to200_Tune4C_13TeV-madgraph-tauola/MINIAODSIM/PU20bx25_PHYS14_25_V1-v1/00000/00D67F78-2873-E411-B3BB-0025907DC9C0.root"))

# 740 RelVal
#process.source = cms.Source("PoolSource",fileNames=cms.untracked.vstring("/store/relval/CMSSW_7_4_0_pre9_ROOT6/RelValH130GGgluonfusion_13/MINIAODSIM/MCRUN2_74_V7-v1/00000/0A35F6D-DAD1-E411-A8CC-0026189438CC.root"))
#process.source = cms.Source("PoolSource",fileNames=cms.untracked.vstring("/store/relval/CMSSW_7_4_0_pre9/RelValH130GGgluonfusion_13/MINIAODSIM/PU25ns_MCRUN2_74_V7-v1/00000/5ABC049C-4CD4-E411-B28A-0025905A613C.root",
#                                                                         "/store/relval/CMSSW_7_4_0_pre9/RelValH130GGgluonfusion_13/MINIAODSIM/PU25ns_MCRUN2_74_V7-v1/00000/C65FAFAA-4CD4-E411-9026-0025905A607E.root"))

# Spring15
#process.source = cms.Source("PoolSource",fileNames=cms.untracked.vstring("/store/mc/RunIISpring15DR74/ttHJetToGG_M125_13TeV_amcatnloFXFX_madspin_pythia8/MINIAODSIM/Asympt50ns_MCRUN2_74_V9A-v1/70000/0232BC3C-01FF-E411-8779-0025907B4FC2.root"))
#process.source = cms.Source("PoolSource",fileNames=cms.untracked.vstring("/store/mc/RunIISpring15DR74/GluGluHToGG_M-125_13TeV_powheg_pythia8/MINIAODSIM/Asympt50ns_MCRUN2_74_V9A-v1/30000/54ECB9A4-912E-E511-BB7D-002590A831CA.root"))
process.source = cms.Source("PoolSource",fileNames=cms.untracked.vstring("file:/afs/cern.ch/work/s/sethzenz/public/GluGluHToGG_M-125_13TeV_powheg_pythia8_MINIAODSIM_Asympt50ns_MCRUN2_74_V9A-v1_54ECB9A4-912E-E511-BB7D-002590A831CA.root"))
process.MessageLogger.cerr.threshold = 'ERROR' # can't get suppressWarning to work: disable all warnings for now
# process.MessageLogger.suppressWarning.extend(['SimpleMemoryCheck','MemoryCheck']) # this would have been better...

# Uncomment the following if you notice you have a memory leak
# This is a lightweight tool to digg further
#process.SimpleMemoryCheck = cms.Service("SimpleMemoryCheck",
#                                        ignoreTotal = cms.untracked.int32(1),
#                                        monitorPssAndPrivate = cms.untracked.bool(True)
#                                       )

#process.load("flashgg/MicroAOD/flashggPDFWeightObject_cfi")
process.load("flashgg/MicroAOD/flashggMicroAODSequence_cff")

from flashgg.MicroAOD.flashggMicroAODOutputCommands_cff import microAODDefaultOutputCommand
process.out = cms.OutputModule("PoolOutputModule", fileName = cms.untracked.string('myMicroAODOutputFile.root'),
                               outputCommands = microAODDefaultOutputCommand
                               )

# All jets are now handled in MicroAODCustomize.py
# Switch from PFCHS to PUPPI with puppi=1 argument (both if puppi=2)

process.p = cms.Path(process.flashggMicroAODSequence)
#process.p = cms.Path(process.flashggPDFWeightObject*process.flashggMicroAODSequence)
process.e = cms.EndPath(process.out)

# Uncomment these lines to run the example commissioning module and send its output to root
#process.commissioning = cms.EDAnalyzer('flashggCommissioning',
#                                       PhotonTag=cms.untracked.InputTag('flashggPhotons'),
#                                       DiPhotonTag = cms.untracked.InputTag('flashggDiPhotons'),
#                                       VertexTag=cms.untracked.InputTag('offlineSlimmedPrimaryVertices')
#)
#process.TFileService = cms.Service("TFileService",
#                                   fileName = cms.string("commissioningTree.root")
#)
#process.p *= process.commissioning


from flashgg.MicroAOD.MicroAODCustomize import customize
customize(process)

if "DY" in customize.datasetName or "SingleElectron" in customize.datasetName or "DoubleEG" in customize.datasetName:
  customize.customizeHLT(process)
