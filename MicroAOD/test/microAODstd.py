import FWCore.ParameterSet.Config as cms
import FWCore.Utilities.FileUtils as FileUtils

process = cms.Process("FLASHggMicroAOD")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.load("Configuration.StandardSequences.GeometryDB_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")
#process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff") # gives deprecated message in 80X but still runs
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
from Configuration.AlCa.GlobalTag import GlobalTag

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32( -1 ) ) # max events 
process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32( 100 )

import os
### 2016
# process.GlobalTag = GlobalTag(process.GlobalTag, '80X_mcRun2_asymptotic_2016_TrancheIV_v7', '')
#process.source = cms.Source("PoolSource",
                             #fileNames=cms.untracked.vstring("/store/mc/RunIISummer16MiniAODv2/GluGluHToGG_M-125_13TeV_powheg_pythia8/MINIAODSIM/PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/60000/024E4FA3-8BBC-E611-8E3D-00266CFFBE88.root")
#)
#    process.GlobalTag = GlobalTag(process.GlobalTag,'80X_dataRun2_2016LegacyRepro_v4','')
#    process.source = cms.Source("PoolSource",fileNames=cms.untracked.vstring("/store/data/Run2016B/SingleElectron/MINIAOD/07Aug17_ver1-v1/110000/0248293E-578B-E711-A639-44A842CFC9D9.root"))
### 2017

# abe_files = [] 
# #file_path = '/store/user/atishelm/postGEN_Outputs/ggF_X1250_WWgg_qqmunugg_10000events_woPU_MINIAOD/190305_151213/0000/ggF_X1250_WWgg_qqmunugg_10000events_woPU_MINIAOD_'

# #file_path = '/store/user/atishelm/postGEN_Outputs/ggF_X1250_WWgg_qqenugg_10000events_woPU_MINIAOD/190225_135123/0000/ggF_X1250_WWgg_qqenugg_10000events_woPU_MINIAOD_' 
# #file_path = '/eos/cms/store/group/phys_higgs/resonant_HH/RunII/MicroAOD/HHWWggSignal/ggF_X1250_WWgg_lnulnugg/100000events_wPU_MINIAOD/190403_092057/0000/'
# #file_path = '/eos/cms/store/group/phys_higgs/resonant_HH/RunII/MicroAOD/HHWWggSignal/ggF_X1250_WWgg_qqlnugg/100000events_wPU_MINIAOD/190403_092149/0000/'
# # file_path = '/eos/cms/store/group/phys_higgs/resonant_HH/RunII/MicroAOD/HHWWggSignal/ggF_X250_WWgg_lnulnugg/100000events_wPU_MINIAOD/190620_020747/0000/'
# file_path = '/eos/cms/store/group/phys_higgs/cmshgg/atishelm/flashgg/GJetMINIAOD/RunIIFall17/'
# # file_path = '/eos/cms/store/group/phys_higgs/resonant_HH/RunII/MicroAOD/HHWWggSignal/ggF_X250_WWgg_qqqqgg/100000events_wPU_MINIAOD/190620_020707/0000/' # qqqq
# #for i in range(25):

# from os import listdir
# from os.path import isfile, join
# abe_files = [f for f in listdir(file_path) if isfile(join(file_path, f))]
# #print 'abe_files = ',abe_files 
# # Get all file names 
# # shortened_path = '/store/group/phys_higgs/resonant_HH/RunII/MicroAOD/HHWWggSignal/ggF_X250_WWgg_qqqqgg/100000events_wPU_MINIAOD/190620_020707/0000/' # qqqq
# shortened_path = '/store/group/phys_higgs/resonant_HH/RunII/MicroAOD/HHWWggSignal/ggF_X250_WWgg_qqlnugg/100000events_wPU_MINIAOD/190620_020629/0000/'
# # shortened_path = '/store/group/phys_higgs/resonant_HH/RunII/MicroAOD/HHWWggSignal/ggF_X250_WWgg_lnulnugg/100000events_wPU_MINIAOD/190620_020747/0000/'
# for i in range(len(abe_files)):
#     tmp_path = shortened_path 
#     tmp_path += abe_files[i]
#     #tmp_path = tmp_path[-8:]
#     abe_files[i] = tmp_path
#     if i == 3: break 
#print 'abe_files = ',abe_files 
# for i in range(1):
# # for i in range(3):
#     j = i + 1
#     this_path = file_path + str(j) + '.root'
#     #this_path += 
#     abe_files.append(this_path)


process.GlobalTag = GlobalTag(process.GlobalTag,'','')
process.source = cms.Source("PoolSource",
                            #fileNames=cms.untracked.vstring('/store/group/phys_higgs/resonant_HH/RunII/MicroAOD/HHWWggSignal/ggF_X1250_WWgg_lnulnugg/100000events_wPU_MINIAOD/190403_092057/0000/ggF_X1250_WWgg_lnulnugg_100000events_wPU_MINIAOD_3.root'))
                            # fileNames=cms.untracked.vstring('/store/group/phys_higgs/resonant_HH/RunII/MicroAOD/HHWWggSignal/ggF_X250_WWgg_lnulnugg/100000events_wPU_MINIAOD/190620_020747/0000/ggF_X250_WWgg_lnulnugg_100000events_wPU_MINIAOD_88.root'))
                            # fileNames=cms.untracked.vstring(abe_files))
                            fileNames=cms.untracked.vstring("/store/group/phys_higgs/cmshgg/atishelm/flashgg/GJetMINIAOD/RunIIFall17/98FC34F6-6ACC-E811-906B-509A4C83EFAB_output.root"))
                            #fileNames=cms.untracked.vstring(abe_files[0],abe_files[1]))
#    process.source = cms.Source("PoolSource",fileNames=cms.untracked.vstring('/store/mc/RunIISummer16MiniAODv2/GluGluHToGG_M-125_13TeV_powheg_pythia8/MINIAODSIM/PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/60000/024E4FA3-8BBC-E611-8E3D-00266CFFBE88.root'))
    #process.source = cms.Source("PoolSource",fileNames=cms.untracked.vstring('root://eoscms.cern.ch//eos/cms/store/mc/RunIIFall17MiniAOD/GJet_Pt-20to40_DoubleEMEnriched_MGG-80toInf_TuneCP5_13TeV_Pythia8/MINIAODSIM/94X_mc2017_realistic_v10-v1/40000/4A2ACB0A-1BD9-E711-AF54-141877410316.root'))
#    process.source = cms.Source("PoolSource",fileNames=cms.untracked.vstring('root://eoscms.cern.ch//eos/cms/store/mc/RunIIFall17MiniAOD/GluGluToHHTo2B2G_node_SM_13TeV-madgraph/MINIAODSIM/94X_mc2017_realistic_v10-v1/00000/2E0E165D-8E05-E811-909C-FA163E80AE1F.root'))
#    process.source = cms.Source("PoolSource",fileNames=cms.untracked.vstring('file:/afs/cern.ch/user/s/sethzenz/work/public/GluGluHToGG_M125_13TeV_amcatnloFXFX_pythia8_94X_mc2017_realistic_v10-v1.root'))
#    process.source = cms.Source("PoolSource",fileNames=cms.untracked.vstring("/store/mc/RunIIFall17MiniAODv2/THQ_ctcvcp_HToGG_M125_13TeV-madgraph-pythia8_TuneCP5/MINIAODSIM/PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/90000/6E58A5DD-BF43-E811-8946-0023AEEEB538.root"))

### 2018
     # process.GlobalTag = GlobalTag(process.GlobalTag,'100X_upgrade2018_realistic_v10','')
     # process.source = cms.Source("PoolSource",fileNames=cms.untracked.vstring("/store/data/Run2018B/DoubleMuon/MINIAOD/PromptReco-v1/000/317/080/00000/4E78B565-8464-E811-BF54-02163E01A0FC.root"))
    #process.source = cms.Source("PoolSource",fileNames=cms.untracked.vstring("/store/data/Run2018D/EGamma/MINIAOD/PromptReco-v2/000/322/106/00000/9A1C4C91-1EB3-E811-A238-02163E0150CE.root"))
    #process.source = cms.Source("PoolSource",fileNames=cms.untracked.vstring("/store/data/Run2018D/DoubleMuon/MINIAOD/PromptReco-v2/000/320/673/00000/0A83E8DF-EB97-E811-AE18-FA163E192E5D.root"))
    # process.source = cms.Source("PoolSource",fileNames=cms.untracked.vstring("/store/mc/RunIISpring18MiniAOD/DYJetsToLL_M-50_TuneCP5_13TeV-madgraphMLM-pythia8/MINIAODSIM/100X_upgrade2018_realistic_v10-v2/10000/F24C5C06-FF47-E811-9C2F-FA163EC3CAD1.root"))

process.RandomNumberGeneratorService = cms.Service("RandomNumberGeneratorService")
process.RandomNumberGeneratorService.flashggRandomizedPhotons = cms.PSet(
          initialSeed = cms.untracked.uint32(16253245)
        )

#process.GlobalTag = GlobalTag(process.GlobalTag,'92X_upgrade2017_realistic_v10','')
#process.source.fileNames=cms.untracked.vstring("/store/mc/RunIISummer17MiniAOD/DYJetsToLL_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/MINIAODSIM/92X_upgrade2017_realistic_v10_ext1-v2/10000/00F9D855-E293-E711-B625-02163E014200.root")

# 2017 Data

# Legacy ReReco
#process.source = cms.Source("PoolSource",fileNames=cms.untracked.vstring("/store/data/Run2016B/SingleElectron/MINIAOD/18Apr2017_ver1-v1/120000/40167FB6-6237-E711-934A-001E67E69E05.root"))

#Moriond17 MC
#process.source = cms.Source("PoolSource",fileNames=cms.untracked.vstring("/store/mc/RunIISummer16MiniAODv2/GluGluHToGG_M-125_13TeV_powheg_pythia8/MINIAODSIM/PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/60000/024E4FA3-8BBC-E611-8E3D-00266CFFBE88.root"))
#80x reminiAOD data
#process.source = cms.Source("PoolSource",fileNames=cms.untracked.vstring("/store/data/Run2016G/DoubleEG/MINIAOD/03Feb2017-v1/100000/002F14FF-D0EA-E611-952E-008CFA197AF4.root"))

#80x signal
#process.source = cms.Source("PoolSource",fileNames=cms.untracked.vstring("/store/mc/RunIISpring16MiniAODv2/GluGluHToGG_M125_13TeV_amcatnloFXFX_pythia8/MINIAODSIM/PUSpring16RAWAODSIM_reHLT_80X_mcRun2_asymptotic_v14_ext2-v1/10000/6A31A211-063B-E611-98EC-001E67F8F727.root")) # ggH 125 miniAODv2
#process.source = cms.Source("PoolSource",fileNames=cms.untracked.vstring("/store/mc/RunIISpring16MiniAODv2/ttHJetToGG_M125_13TeV_amcatnloFXFX_madspin_pythia8_v2/MINIAODSIM/PUSpring16RAWAODSIM_reHLT_80X_mcRun2_asymptotic_v14-v1/80000/267A1DB4-3D3B-E611-9AD2-003048C559C4.root")) # ttH 125 miniAODv2

#80x data
#process.source = cms.Source("PoolSource",fileNames=cms.untracked.vstring("/store/data/Run2016B/DoubleEG/MINIAOD/PromptReco-v2/000/273/158/00000/1E5ABF54-E019-E611-AAED-02163E01293F.root")) # /DoubleEG/Run2016B-PromptReco-v2/MINIAOD

process.MessageLogger.cerr.threshold = 'ERROR' # can't get suppressWarning to work: disable all warnings for now
# process.MessageLogger.suppressWarning.extend(['SimpleMemoryCheck','MemoryCheck']) # this would have been better...

# Uncomment the following if you notice you have a memory leak
# This is a lightweight tool to digg further
#process.SimpleMemoryCheck = cms.Service("SimpleMemoryCheck",
#                                        ignoreTotal = cms.untracked.int32(1),
#                                        monitorPssAndPrivate = cms.untracked.bool(True)
#                                       )

process.load("flashgg/MicroAOD/flashggMicroAODSequence_cff")

# NEEDED FOR ANYTHING PRIOR TO reMiniAOD
#process.weightsCount.pileupInfo = "addPileupInfo"

from flashgg.MicroAOD.flashggMicroAODOutputCommands_cff import microAODDefaultOutputCommand
process.out = cms.OutputModule("PoolOutputModule", fileName = cms.untracked.string('myMicroAODOutputFile.root'),
                               outputCommands = microAODDefaultOutputCommand
                               )

# # For PhotonID study 
# process.out.outputCommands.extend([
#                                              'keep *Ecal*_*_*_*',
#                                              'keep *_*Ecal*_*_*',
#                                              'keep *ECAL*_*_*_*',  
#                                              'keep *_*ECAL*_*_*', 
#                                              'keep *ecal*_*_*_*',      
#                                              'keep *_*ecal*_*_*',  
#                                              'keep *EGamma*_*_*_*', 
#                                              'keep *_*EGamma*_*_*'])

# All jets are now handled in MicroAODCustomize.py
# Switch from PFCHS to PUPPI with puppi=1 argument (both if puppi=2)

process.p = cms.Path(process.flashggMicroAODSequence)
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

if "DY" in customize.datasetName or "SingleElectron" in customize.datasetName or "DoubleEG" in customize.datasetName or "EGamma" in customize.datasetName:
    customize.customizeHLT(process)

#open('dump.py', 'w').write(process.dumpPython())
