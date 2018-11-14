import FWCore.ParameterSet.Config as cms

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
# all_variables =  var.pho_variables + var.tp_variables
###---H4G ntuples (and workspaces eventually) creation
from flashgg.Taggers.h4gCandidateDumper_cfi import h4gCandidateDumper
process.h4gCandidateDumper = h4gCandidateDumper.clone()
process.h4gCandidateDumper.dumpTrees = True

cfgTools.addCategories(process.h4gCandidateDumper,
                        [
                            ("Reject", "", -1),
                            ("4photons", "is4Photons && h4gPho1.pt > 30 && h4gPho2.pt > 20 && h4gPho3.pt > 10 && h4gPho4.pt > 10 && abs(h4gPho1.eta) < 2.5  && abs(h4gPho2.eta) < 2.5  && abs(h4gPho3.eta) < 2.5  && abs(h4gPho4.eta) < 2.5", 0),
                            ("3photons", "is3Photons && h4gPho1.pt > 30 && h4gPho2.pt > 20 && h4gPho3.pt > 10 && abs(h4gPho1.eta) < 2.5  && abs(h4gPho2.eta) < 2.5  && abs(h4gPho3.eta) < 2.5", 0),
                            ("2photons", "is2Photons && h4gPho1.pt > 30 && h4gPho2.pt > 20 && abs(h4gPho1.eta) < 2.5  && abs(h4gPho2.eta) < 2.5", 0)
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
import FWCore.ParameterSet.VarParsing as VarParsing

# Require low mass diphoton triggers
from HLTrigger.HLTfilters.hltHighLevel_cfi import hltHighLevel
process.hltHighLevel= hltHighLevel.clone(HLTPaths = cms.vstring(#"HLT_Diphoton30_22_R9Id_OR_IsoCaloId_AND_HE_R9Id_Mass90_v*",
                                                               "HLT_Diphoton30PV_18PV_R9Id_AND_IsoCaloId_AND_HE_R9Id_DoublePixelVeto_Mass55_v*",
                                                               "HLT_Diphoton30EB_18EB_R9Id_OR_IsoCaloId_AND_HE_R9Id_DoublePixelVeto_Mass55_v*"
                                                                ))
process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )
process.load('RecoMET.METFilters.eeBadScFilter_cfi')
process.eeBadScFilter.EERecHitSource = cms.InputTag("reducedEgamma","reducedEERecHits") # Saved MicroAOD Collection (data only)
process.dataRequirements = cms.Sequence()
if customize.processId == "Data":
        process.dataRequirements += process.hltHighLevel
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

maxEvents = 5
if customize.maxEvents:
    maxEvents = int(customize.maxEvents)

customize.setDefault("targetLumi",1.00e+3)
if customize.inputFiles:
    inputFile = customize.inputFiles

if customize.outputFile:
    outputFile = customize.outputFile

process.path = cms.Path(process.FlashggH4GCandidate+process.h4gCandidateDumper)
#process.e = cms.EndPath(process.out)
customize(process)
