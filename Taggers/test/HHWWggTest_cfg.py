import FWCore.ParameterSet.Config as cms  # imports our CMS-specific Python classes and functions
import os # python module for os dependent functionality 
from flashgg.Taggers.flashggHHWWggCandidate_cfi import FlashggHHWWggCandidate # flashggHHWWggCandidate parameters 
from flashgg.Taggers.flashggPreselectedDiPhotons_cfi import flashggPreselectedDiPhotons
import flashgg.Taggers.dumperConfigTools as cfgTools

process = cms.Process("FLASHggHHWWggTest") # Process name. Can't use HH_WWgg because '_' is a non-alphanumeric character
from flashgg.Taggers.flashggTags_cff import flashggUnpackedJets
process.flashggUnpackedJets = flashggUnpackedJets

from flashgg.Taggers.flashggDiPhotonMVA_cfi import flashggDiPhotonMVA
process.flashggDiPhotonMVA = flashggDiPhotonMVA 

from flashgg.Taggers.flashggUpdatedIdMVADiPhotons_cfi import flashggUpdatedIdMVADiPhotons
process.flashggUpdatedIdMVADiPhotons = flashggUpdatedIdMVADiPhotons

#process.flashggDiPhotonMVA.DiPhotonTag = "flashggDiPhotons" 
process.flashggDiPhotonMVA.DiPhotonTag = "flashggPreselectedDiPhotons"
###---HHWWgg candidates production
process.FlashggHHWWggCandidate = FlashggHHWWggCandidate.clone() # clone flashgg HHWWggCandidate parameters here 

process.FlashggHHWWggCandidate.idSelection = cms.PSet(
        # rho = flashggPreselectedDiPhotonsLowMass.rho,
        # cut = flashggPreselectedDiPhotonsLowMass.cut,
        # variables = flashggPreselectedDiPhotonsLowMass.variables,
        # categories = flashggPreselectedDiPhotonsLowMass.categories
        rho = flashggPreselectedDiPhotons.rho,
        cut = flashggPreselectedDiPhotons.cut, # diphoton preselection cuts. Become part of Idselector definition  
        variables = flashggPreselectedDiPhotons.variables,
        categories = flashggPreselectedDiPhotons.categories
        )

process.flashggPreselectedDiPhotons = flashggPreselectedDiPhotons
process.flashggPreselectedDiPhotons.src = "flashggDiPhotons"

# cut=cms.string(
#         "    (leadingPhoton.full5x5_r9>0.8||leadingPhoton.egChargedHadronIso<20||leadingPhoton.egChargedHadronIso/leadingPhoton.pt<0.3)"
#         " && (subLeadingPhoton.full5x5_r9>0.8||subLeadingPhoton.egChargedHadronIso<20||subLeadingPhoton.egChargedHadronIso/subLeadingPhoton.pt<0.3)"
#         " && (leadingPhoton.hadronicOverEm < 0.08 && subLeadingPhoton.hadronicOverEm < 0.08)"
#         " && (leadingPhoton.pt >35.0 && subLeadingPhoton.pt > 25.0)"
#         " && (abs(leadingPhoton.superCluster.eta) < 2.5 && abs(subLeadingPhoton.superCluster.eta) < 2.5)"
#         " && (abs(leadingPhoton.superCluster.eta) < 1.4442 || abs(leadingPhoton.superCluster.eta) > 1.566)"
#         " && (abs(subLeadingPhoton.superCluster.eta) < 1.4442 || abs(subLeadingPhoton.superCluster.eta) > 1.566)"
#         " && (leadPhotonId > -0.9 && subLeadPhotonId > -0.9)"
#         )
#                                                               )
# process.FlashggHHWWggCandidate.src = "PreselDiPhotons"

###--- get the variables
import flashgg.Taggers.HHWWggTagVariables as var # python file of lists of strings 
#all_variables = var.pho_variables + var.dipho_variables + var.tp_variables + var.abe_variables # add variable lists together 
all_variables = var.HHWWgg_variables # add variable lists together 

from flashgg.Taggers.HHWWggCandidateDumper_cfi import HHWWggCandidateDumper
process.HHWWggCandidateDumper = HHWWggCandidateDumper.clone() # clone parameters from HHWWggCandidateDumpConfig_cff (className, src, ...)
process.HHWWggCandidateDumper.dumpTrees = True # Needs to be set to true here. Default in _cff is false 
process.HHWWggCandidateDumper.dumpWorkspace = False

# Create histograms 

cfgTools.addCategories(process.HHWWggCandidateDumper,
                        [
                          ("All_HLT_Events","1",0),
                          #("All_HLT_Events","Cut_Results[0]",0), # All events that passed HLT 
                          #("Remaining","1",0), # Remaining events 
                            # cut 1
                            # rest of events 
                        ],

                        variables = all_variables, 
                        histograms=[]
                        )

# process.source = cms.Source ("PoolSource",
#                              fileNames = cms.untracked.vstring(
# "root://eoscms.cern.ch//eos/cms/store/group/phys_higgs/HiggsExo/HHWWggamma/MicroAOD/HHWWgg_Jun7/v0/SUSYGluGluToHToAA_AToGG_M-60_TuneCUETP8M1_13TeV_pythia8/Test_jun7-R2S16MAODv2-PUM17_GT/170607_180035/0000/myMicroAODOutputFile_1.root"
# ))

# input file. MicroAOD 

# process.source = cms.Source ("PoolSource",
#                              fileNames = cms.untracked.vstring(
# #"file:myMicroAODOutputFile.root" 
# "root://eoscms.cern.ch//eos/cms/store/group/phys_higgs/HiggsExo/H4Gamma/MicroAOD/H4G_Jun7/v0/SUSYGluGluToHToAA_AToGG_M-60_TuneCUETP8M1_13TeV_pythia8/Test_jun7-R2S16MAODv2-PUM17_GT/170607_180035/0000/myMicroAODOutputFile_1.root"
# ))

process.source = cms.Source ("PoolSource",
                             fileNames = cms.untracked.vstring(
#"file:/eos/cms/store/group/phys_higgs/cmshgg/sethzenz/flashgg/RunIISummer16-2_4_1-25ns_Moriond17_extra/2_4_1/GJets_HT-100To200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16-2_4_1-25ns_Moriond17_extra-2_4_1-v0-RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/171212_152732/0000/myMicroAODOutputFile_10.root"                               
#"file:myMicroAODOutputFile_testtest.root"     
#"root://eoscms.cern.ch//eos/cms/store/group/phys_higgs/cmshgg/ferriff/flashgg/RunIISpring16DR80X-2_3_0-25ns_Moriond17_MiniAODv2_p3/2_3_0/DoubleEG/RunIISpring16DR80X-2_3_0-25ns_Moriond17_MiniAODv2_p3-2_3_0-v0-Run2016G-23Sep2016-v1/170116_174903/0000/myMicroAODOutputFile_4.root"
#"root://eoscms.cern.ch//eos/cms/store/group/phys_higgs/cmshgg/sethzenz/flashgg/ReMiniAOD-03Feb2017-2_5_4/2_5_1/DoubleEG/ReMiniAOD-03Feb2017-2_5_4-2_5_1-v0-Run2016B-03Feb2017_ver2-v2/170310_111417/0000/myMicroAODOutputFile_101.root"
#"file:myMicroAODOutputFile_qqlnugg_8Apr2019.root" # SL X1250
#"file:myMicroAODOutputFile_lnulnugg_8Apr2019.root" # FL X1250
#"file:myMicroAODOutputFile_qqqqgg_8Apr2019.root" # FH  X1250
#"file:myMicroAODOutputFile_ggF_X250_WWgg_lnulnugg_crabtest_2.root" # FL X250
#"file:myMicroAODOutputFile_ggF_X250_WWgg_qqlnugg_crabtest_2.root" # SL X250
#"file:myMicroAODOutputFile_ggF_X250_WWgg_qqqqgg_crabtest.root" # FH X250
#"file:/eos/cms/store/group/phys_higgs/cmshgg/sethzenz/flashgg/RunIIFall17-3_1_0/3_1_0/GluGluHToGG_M-125_13TeV_powheg_pythia8/RunIIFall17-3_1_0-3_1_0-v0-RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/180609_081728/0000/myMicroAODOutputFile_4.root"
"root://cms-xrd-global.cern.ch//store/user/spigazzi/flashgg/Era2017_RR-31Mar2018_v1/legacyRun2TestV1/GluGluHToGG_M125_13TeV_amcatnloFXFX_pythia8/Era2017_RR-31Mar2018_v1-legacyRun2TestV1-v0-RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/190319_112256/0000/myMicroAODOutputFile_9.root"
))


process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string("test.root"),
                                   closeFileFast = cms.untracked.bool(True)
)
#process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(1000) )


from flashgg.MetaData.JobConfig import customize
#customize.setDefault("maxEvents",-1)
customize.setDefault("maxEvents",1000)


#to check which triggers are present ---remove if not needed
# process.TriggerAnalyzer = cms.EDAnalyzer("MiniAODTriggerAnalyzer",
# bits = cms.InputTag("TriggerResults","","HLT")
# )
# process.TriggerAnalyzerPath = cms.Path(process.TriggerAnalyzer)



# customize.setDefault("puTarget",'2.39e+05,8.38e+05,2.31e+06,3.12e+06,4.48e+06,6e+06,7e+06,1.29e+07,3.53e+07,7.87e+07,1.77e+08,3.6e+08,6.03e+08,8.77e+08,1.17e+09,1.49e+09,1.76e+09,1.94e+09,2.05e+09,2.1e+09,2.13e+09,2.15e+09,2.13e+09,2.06e+09,1.96e+09,1.84e+09,1.7e+09,1.55e+09,1.4e+09,1.24e+09,1.09e+09,9.37e+08,7.92e+08,6.57e+08,5.34e+08,4.27e+08,3.35e+08,2.58e+08,1.94e+08,1.42e+08,1.01e+08,6.9e+07,4.55e+07,2.88e+07,1.75e+07,1.02e+07,5.64e+06,2.99e+06,1.51e+06,7.32e+05,3.4e+05,1.53e+05,6.74e+04,3.05e+04,1.52e+04,8.98e+03,6.5e+03,5.43e+03,4.89e+03,4.52e+03,4.21e+03,3.91e+03,3.61e+03,3.32e+03,3.03e+03,2.75e+03,2.47e+03,2.21e+03,1.97e+03,1.74e+03,1.52e+03,1.32e+03,1.14e+03,983,839')


# import FWCore.ParameterSet.VarParsing as VarParsing
customize(process)
# if customize.inputFiles:
#     inputFile = customize.inputFiles
#
# if customize.outputFile:
#     outputFile = customize.outputFile

#customize.parse()
# process.TFileService = cms.Service("TFileService",
#                                    fileName = cms.string("output.root"),
#                                    closeFileFast = cms.untracked.bool(True)
# )

# Require low mass diphoton triggers
from HLTrigger.HLTfilters.hltHighLevel_cfi import hltHighLevel
process.hltHighLevel= hltHighLevel.clone(HLTPaths = cms.vstring(   
                                                                "HLT_Diphoton30_18_R9Id_OR_IsoCaloId_AND_HE_R9Id_Mass90_v*" # 2016 Hgg HLT path 
                                                                #"HLT_Diphoton30_18_R9Id_OR_IsoCaloId_AND_HE_R9Id_Mass90"
                                                              #"HLT_Diphoton30PV_18PV_R9Id_AND_IsoCaloId_AND_HE_R9Id_DoublePixelVeto_Mass55_v*",
                                                              #"HLT_Diphoton30EB_18EB_R9Id_OR_IsoCaloId_AND_HE_R9Id_DoublePixelVeto_Mass55_v*"
                                                               ))
process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )

#############   Geometry  ###############
process.load("Geometry.CMSCommonData.cmsIdealGeometryXML_cfi")
process.load("Geometry.CaloEventSetup.CaloGeometry_cfi")
process.load("Geometry.CaloEventSetup.CaloTopology_cfi")
process.load('RecoMET.METFilters.eeBadScFilter_cfi')
process.load("Configuration.Geometry.GeometryECALHCAL_cff")
process.eeBadScFilter.EERecHitSource = cms.InputTag("reducedEgamma","reducedEERecHits") # Saved MicroAOD Collection (data only)

process.dataRequirements = cms.Sequence()

print'customize.processId = ',customize.processId
if customize.processId == "Data":
   process.dataRequirements += process.hltHighLevel # HLT 
   process.dataRequirements += process.eeBadScFilter

# customize.register('puTarget',
#                    " ",
#                    VarParsing.VarParsing.multiplicity.singleton,
#                    VarParsing.VarParsing.varType.string,
#                    "puTarget")
###--- call the customization
# process.HHWWggCandidateDumper.puReWeight=cms.bool( bool(customize.PURW) )
# if customize.PURW == False:
# 	process.HHWWggCandidateDumper.puTarget = cms.vdouble()

#process.path = cms.Path(process.flashggDiPhotonMVA*process.flashggUnpackedJets*process.dataRequirements*process.FlashggHHWWggCandidate*process.HHWWggCandidateDumper)
process.path = cms.Path(process.flashggUpdatedIdMVADiPhotons
                        *process.flashggPreselectedDiPhotons
                        *process.flashggDiPhotonMVA
                        *process.flashggUnpackedJets
                        *process.dataRequirements
                        *process.FlashggHHWWggCandidate
                        *process.HHWWggCandidateDumper
                        )

# process.path = cms.Path(process.FlashggHHWWggCandidate+process.HHWWggCandidateDumper)
#process.e = cms.EndPath(process.out)

# customize(process) 
