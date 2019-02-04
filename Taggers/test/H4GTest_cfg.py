import FWCore.ParameterSet.Config as cms
import os

from flashgg.Taggers.flashggH4GCandidate_cfi import FlashggH4GCandidate
from flashgg.Taggers.flashggPreselectedDiPhotons_LowMass_cfi import flashggPreselectedDiPhotonsLowMass
import flashgg.Taggers.dumperConfigTools as cfgTools

process = cms.Process("FLASHggH4GTest")


###---H4G candidates production
process.FlashggH4GCandidate = FlashggH4GCandidate.clone()
process.FlashggH4GCandidate.idSelection = cms.PSet(
        rho = flashggPreselectedDiPhotonsLowMass.rho,
        cut = flashggPreselectedDiPhotonsLowMass.cut,
        variables = flashggPreselectedDiPhotonsLowMass.variables,
        categories = flashggPreselectedDiPhotonsLowMass.categories
        )
###--- get the variables
import flashgg.Taggers.H4GTagVariables as var
all_variables = var.pho_variables + var.dipho_variables + var.tp_variables

from flashgg.Taggers.h4gCandidateDumper_cfi import h4gCandidateDumper
process.h4gCandidateDumper = h4gCandidateDumper.clone()
process.h4gCandidateDumper.dumpTrees = True
process.h4gCandidateDumper.dumpWorkspace = False

cfgTools.addCategories(process.h4gCandidateDumper,
                        [
                            ("Reject", "", -1),
                            ("4photons","phoVector.size() > 3", 0),
                            ("3photons","phoVector.size() == 3", 0),
                            ("2photons","phoVector.size() == 2", 0)
                        ],

                        variables = all_variables,
                        histograms=[]
                        )

process.source = cms.Source ("PoolSource",
                             fileNames = cms.untracked.vstring(
"root://eoscms.cern.ch//eos/cms/store/group/phys_higgs/HiggsExo/H4Gamma/MicroAOD/H4G_Jun7/v0/SUSYGluGluToHToAA_AToGG_M-60_TuneCUETP8M1_13TeV_pythia8/Test_jun7-R2S16MAODv2-PUM17_GT/170607_180035/0000/myMicroAODOutputFile_1.root"
))
process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string("test.root"),
                                   closeFileFast = cms.untracked.bool(True)
)
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )


from flashgg.MetaData.JobConfig import customize
customize.setDefault("maxEvents",-1)
# customize.setDefault("puTarget",'2.39e+05,8.38e+05,2.31e+06,3.12e+06,4.48e+06,6e+06,7e+06,1.29e+07,3.53e+07,7.87e+07,1.77e+08,3.6e+08,6.03e+08,8.77e+08,1.17e+09,1.49e+09,1.76e+09,1.94e+09,2.05e+09,2.1e+09,2.13e+09,2.15e+09,2.13e+09,2.06e+09,1.96e+09,1.84e+09,1.7e+09,1.55e+09,1.4e+09,1.24e+09,1.09e+09,9.37e+08,7.92e+08,6.57e+08,5.34e+08,4.27e+08,3.35e+08,2.58e+08,1.94e+08,1.42e+08,1.01e+08,6.9e+07,4.55e+07,2.88e+07,1.75e+07,1.02e+07,5.64e+06,2.99e+06,1.51e+06,7.32e+05,3.4e+05,1.53e+05,6.74e+04,3.05e+04,1.52e+04,8.98e+03,6.5e+03,5.43e+03,4.89e+03,4.52e+03,4.21e+03,3.91e+03,3.61e+03,3.32e+03,3.03e+03,2.75e+03,2.47e+03,2.21e+03,1.97e+03,1.74e+03,1.52e+03,1.32e+03,1.14e+03,983,839')


# import FWCore.ParameterSet.VarParsing as VarParsing
# customize(process)
# if customize.inputFiles:
#     inputFile = customize.inputFiles
#
# if customize.outputFile:
#     outputFile = customize.outputFile

# customize.parse()
# process.TFileService = cms.Service("TFileService",
#                                    fileName = cms.string("output.root"),
#                                    closeFileFast = cms.untracked.bool(True)
# )

# Require low mass diphoton triggers
from HLTrigger.HLTfilters.hltHighLevel_cfi import hltHighLevel
process.hltHighLevel= hltHighLevel.clone(HLTPaths = cms.vstring(
                                                              "HLT_Diphoton30PV_18PV_R9Id_AND_IsoCaloId_AND_HE_R9Id_DoublePixelVeto_Mass55_v*",
                                                              "HLT_Diphoton30EB_18EB_R9Id_OR_IsoCaloId_AND_HE_R9Id_DoublePixelVeto_Mass55_v*"
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

process.dataRequirements += process.hltHighLevel
if customize.processId == "Data":
   # process.dataRequirements += process.hltHighLevel
   process.dataRequirements += process.eeBadScFilter

# customize.register('puTarget',
#                    " ",
#                    VarParsing.VarParsing.multiplicity.singleton,
#                    VarParsing.VarParsing.varType.string,
#                    "puTarget")
###--- call the customization
# process.h4gCandidateDumper.puReWeight=cms.bool( bool(customize.PURW) )
# if customize.PURW == False:
# 	process.h4gCandidateDumper.puTarget = cms.vdouble()
process.path = cms.Path(process.dataRequirements*process.FlashggH4GCandidate*process.h4gCandidateDumper)
# process.path = cms.Path(process.FlashggH4GCandidate+process.h4gCandidateDumper)
#process.e = cms.EndPath(process.out)
customize(process)
