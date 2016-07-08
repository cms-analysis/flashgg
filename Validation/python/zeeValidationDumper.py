import FWCore.ParameterSet.Config as cms
import FWCore.Utilities.FileUtils as FileUtils
import FWCore.ParameterSet.VarParsing as VarParsing

from flashgg.MetaData.samples_utils import SamplesManager

process = cms.Process("zeeValidationDumper")

process.load("HLTrigger.HLTfilters.hltHighLevel_cfi")
process.hltFilter = process.hltHighLevel.clone()
process.hltFilter.throw = cms.bool(False)

from flashgg.MetaData.JobConfig import  JobConfig
customize = JobConfig(crossSections=["$CMSSW_BASE/src/flashgg/MetaData/data/cross_sections.json"])
customize.setDefault("maxEvents",10000)
customize.setDefault("targetLumi",2.6e+4)
customize.parse()

if ("data_single" in customize.processId):
    process.hltFilter.HLTPaths = cms.vstring("HLT_Ele23_WPLoose_Gsf_v*")
#    process.hltFilter.HLTPaths = cms.vstring("HLT_Ele22_eta2p1_WPLoose_Gsf_v*")
#elif ("data_double" in customize.processId):
#    process.hltFilter.HLTPaths = cms.vstring("HLT_Diphoton30_18_R9Id_OR_IsoCaloId_AND_HE_R9Id_DoublePixelSeedMatch_Mass70_v*")
#elif ("mc_single" in customize.processId):
#    process.hltFilter.HLTPaths = cms.vstring("HLT_Ele22_eta2p1_WP75_Gsf_v*")
#elif ("mc_double" in customize.processId):
#    process.hltFilter.HLTPaths = cms.vstring("HLT_Diphoton30_18_R9Id_OR_IsoCaloId_AND_HE_R9Id_DoublePixelSeedMatch_Mass70_v*")

process.load('RecoMET.METFilters.eeBadScFilter_cfi')
process.eeBadScFilter.EERecHitSource = cms.InputTag("reducedEgamma","reducedEERecHits") # Saved MicroAOD Collection (data only)
process.dataRequirements = cms.Sequence()
#process.dataRequirements += process.hltHighLevel
process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32( 10000 )

process.load("Configuration.StandardSequences.GeometryDB_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff")
process.RandomNumberGeneratorService = cms.Service("RandomNumberGeneratorService")

if ("data" in customize.processId):
    process.GlobalTag.globaltag = '80X_dataRun2_Prompt_v8'
else:
    process.GlobalTag.globaltag = '80X_mcRun2_asymptotic_2016_v3'
#process.GlobalTag.globaltag = '80X_mcRun2_asymptotic_2016_v3' #for interactive running
    
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(10000) )
    
process.source = cms.Source ("PoolSource",fileNames = cms.untracked.vstring('/store/group/phys_higgs/cmshgg/ferriff/flashgg/RunIISpring16DR80X-2_0_0-25ns/2_0_0/DYToEE_NNPDF30_13TeV-powheg-pythia8/RunIISpring16DR80X-2_0_0-25ns-2_0_0-v0-RunIISpring16MiniAODv1-PUSpring16_80X_mcRun2_asymptotic_2016_v3-v1/160524_084452/0000/myMicroAODOutputFile_1.root'
))

import flashgg.Taggers.dumperConfigTools as cfgTools
process.load("flashgg.Taggers.globalVariables_cff")
process.globalVariables.puReWeight = cms.bool(True)
if ("data" in customize.processId):
    process.globalVariables.puReWeight = cms.bool(False)
    process.dataRequirements += process.hltFilter
    process.dataRequirements += process.eeBadScFilter

process.load("flashgg.Taggers.diphotoMVAWithZeeDumper_cff")
process.DiPhotonWithZeeMVADumper.dumpHistos = False
process.DiPhotonWithZeeMVADumper.dumpTrees  =  True
process.DiPhotonWithZeeMVADumper.dumpGlobalVariables = cms.untracked.bool(True)
process.DiPhotonWithZeeMVADumper.globalVariables = process.globalVariables

#process.load("flashgg.Taggers.photonDumper_cfi")

process.load("flashgg.Taggers.diphotonDumper_cfi") 
process.diphotonDumper.src = cms.InputTag("flashggDiPhotonSystematics")
#process.diphotonDumper.src = cms.InputTag("flashggUpdatedIdMVADiPhotons")
#process.diphotonDumper.src = cms.InputTag("flashggDiPhotons")
process.diphotonDumper.dumpHistos = False
process.diphotonDumper.dumpTrees  =  True
process.diphotonDumper.dumpGlobalVariables = cms.untracked.bool(True)
process.diphotonDumper.globalVariables = process.globalVariables

process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string("histo.root"),
                                   closeFileFast = cms.untracked.bool(True)
                                   )

#DIPHOTON MVA
cfgTools.addCategories(process.DiPhotonWithZeeMVADumper,
                       [("All","1", 0),],
                       variables=["dipho_mva:=mvaValue"],
                       histograms=["dipho_mva>>dipho_mva(100,-1,1)",]
)
# split tree, histogram and datasets by process
process.DiPhotonWithZeeMVADumper.nameTemplate ="zeevalidation_$SQRTS_$LABEL_$SUBCAT"


cfgTools.addCategories(process.diphotonDumper,
                       ## categories definition
                       ## cuts are applied in cascade. Events getting to these categories have already failed the "Reject" selection
                       [("All",
"""
(abs(leadingPhoton.superCluster.eta) < 2.5 && abs(subLeadingPhoton.superCluster.eta) < 2.5 && leadingPhoton.hasPixelSeed()==1) &&
                                        (leadingPhoton.pt > 33) &&
                                        (leadingPhoton.hadronicOverEm < 0.1) &&
                                        ((leadingPhoton.full5x5_r9 > 0.5 && leadingPhoton.isEB) || (leadingPhoton.full5x5_r9 > 0.8 && leadingPhoton.isEE)) &&
                                        ((subLeadingPhoton.full5x5_r9 > 0.5 && subLeadingPhoton.isEB) || (subLeadingPhoton.full5x5_r9 > 0.8 && subLeadingPhoton.isEE)) &&
                                        ((leadingPhoton.isEB &&
                                        (leadingPhoton.full5x5_r9>0.85 ||
                                        (leadingPhoton.full5x5_sigmaIetaIeta < 0.015 && leadingPhoton.pfPhoIso03 < 4.0 && leadingPhoton.trkSumPtHollowConeDR03 < 6.0 ))) ||
                                        (leadingPhoton.isEE &&
                                        (leadingPhoton.full5x5_r9>0.9 ||
                                        (leadingPhoton.full5x5_sigmaIetaIeta < 0.035 && leadingPhoton.pfPhoIso03 < 4.0 && leadingPhoton.trkSumPtHollowConeDR03 < 6.0 )))) &&
                                        (leadingPhoton.pt > 14 && leadingPhoton.hadTowOverEm()<0.15 &&
                                        (leadingPhoton.r9()>0.8 || leadingPhoton.chargedHadronIso()<20 || leadingPhoton.chargedHadronIso()<0.3*leadingPhoton.pt())) &&

                         (abs(leadingPhoton.superCluster.eta) < 2.5 && abs(subLeadingPhoton.superCluster.eta) < 2.5 && subLeadingPhoton.hasPixelSeed()==1) &&
                                          (subLeadingPhoton.pt > 25) && 
                                          (subLeadingPhoton.hadronicOverEm < 0.1) &&
                                          ((leadingPhoton.full5x5_r9 > 0.5 && leadingPhoton.isEB) || (leadingPhoton.full5x5_r9 > 0.8 && leadingPhoton.isEE)) &&
                                          ((subLeadingPhoton.full5x5_r9 > 0.5 && subLeadingPhoton.isEB) || (subLeadingPhoton.full5x5_r9 > 0.8 && subLeadingPhoton.isEE)) &&
                                          (( subLeadingPhoton.isEB &&
                                          (subLeadingPhoton.full5x5_r9>0.85 ||
                                          (subLeadingPhoton.full5x5_sigmaIetaIeta < 0.015 && subLeadingPhoton.pfPhoIso03 < 4.0 && subLeadingPhoton.trkSumPtHollowConeDR03 < 6.0 ))) ||
                                          (subLeadingPhoton.isEE &&
                                          (subLeadingPhoton.full5x5_r9>0.9 ||
                                          (subLeadingPhoton.full5x5_sigmaIetaIeta < 0.035 && subLeadingPhoton.pfPhoIso03 < 6.0 && subLeadingPhoton.trkSumPtHollowConeDR03 < 6.0 )))) &&
                                          (subLeadingPhoton.pt > 14 && subLeadingPhoton.hadTowOverEm()<0.15 &&
                                          (subLeadingPhoton.r9()>0.8 || subLeadingPhoton.chargedHadronIso()<20 || subLeadingPhoton.chargedHadronIso()<0.3*subLeadingPhoton.pt()))

""", 0)
                        #("EB", "abs(superCluster.eta)<1.479", 0),
                        #("EE", "abs(superCluster.eta)>1.566",0)
                        ],
                       variables=["leadPhIso        := leadingPhoton.pfPhoIso03()",
                                  "leadNeuIso       := leadingPhoton.pfNeutIso03()",
                                  "leadsieie        := leadingPhoton.full5x5_sigmaIetaIeta",
                                  "leadcovieip      := leadingPhoton.sieip",
                                  "leadetawidth     := leadingPhoton.superCluster.etaWidth",
                                  "leadphiwidth     := leadingPhoton.superCluster.phiWidth",
                                  "leads4ratio      := leadingPhoton.s4",
                                  "leadr9           := leadingPhoton.r9",
                                  "leadfull5x5r9    := leadingPhoton.full5x5_r9",
                                  "leadPt           := leadingPhoton.et",
                                  "leadEta          := leadingPhoton.eta",
                                  "leadIDMVA        := subLeadingView.phoIdMvaWrtChosenVtx()",
                                  "leadChIsoRv      := leadingView.pfChIso03WrtChosenVtx()",
                                  "leadChIsoWv      := leadingPhoton.pfChgIsoWrtWorstVtx03",
                                  "leadESSigma      := leadingPhoton.esEffSigmaRR",
                                  "subleadPhIso     := subLeadingPhoton.pfPhoIso03()",
                                  "subLeadNeuIso    := subLeadingPhoton.pfNeutIso03()",
                                  "subleadsieie     := subLeadingPhoton.full5x5_sigmaIetaIeta",
                                  "subleadcovieip   := subLeadingPhoton.sieip",
                                  "subleadetawidth  := subLeadingPhoton.superCluster.etaWidth",
                                  "subleadphiwidth  := subLeadingPhoton.superCluster.phiWidth",
                                  "subleads4ratio   := subLeadingPhoton.s4",
                                  "subleadr9        := subLeadingPhoton.r9",
                                  "subleadfull5x5r9 := subLeadingPhoton.full5x5_r9",
                                  "subleadPt        := subLeadingPhoton.et",
                                  "subleadEta       := subLeadingPhoton.eta",
                                  "subIDMVA         := leadingView.phoIdMvaWrtChosenVtx()",
                                  "subleadChIsoRv   := subLeadingView.pfChIso03WrtChosenVtx()",
                                  "subleadChIsoWv   := subLeadingPhoton.pfChgIsoWrtWorstVtx03",
                                  "subleadESSigma   := subLeadingPhoton.esEffSigmaRR",
                                  "mass[120,60,120] := mass", 
                                  "scEta            := leadingPhoton.superCluster.eta",
                                  "sigmaEoE1        := leadingPhoton.sigEOverE",
                                  "sigmaEoE2        := subLeadingPhoton.sigEOverE",
                                  "sigmaMoM         := .5*sqrt(leadingPhoton.sigEOverE*leadingPhoton.sigEOverE+subLeadingPhoton.sigEOverE*subLeadingPhoton.sigEOverE)",
                                  "vtxProb          := vtxProbMVA",
                                  "cosdphi          := cos(leadingPhoton.phi-subLeadingPhoton.phi)",
                                  "dipho_pt         := pt",
                                  "nVtx             := nVert",
                                  ],
                       histograms=[]                                   
                       )
process.diphotonDumper.nameTemplate ="zeevalidation_$SQRTS_$LABEL_$SUBCAT"

#process.load("flashgg/Taggers/flashggDiPhotonMVA_cfi")
#process.flashggDiPhotonMVA.DiPhotonTag = cms.InputTag("flashggDiPhotons")
#process.flashggDiPhotonMVA.DiPhotonTag = cms.InputTag("flashggDiPhotonSystematics")

############################
#       Systematics        #
############################
## import systs. customize
from flashgg.Systematics.SystematicsCustomize import *
#customize.processId = 'Data' # for test

## apply shower shape corrections
doUpdatedIdMVADiPhotons = False # set to True for 76X (for 80X shower shape corrections not yet available)

## load syst producer
process.load("flashgg.Systematics.flashggDiPhotonSystematics_cfi")

if (doUpdatedIdMVADiPhotons):
    process.flashggDiPhotonSystematics.src = "flashggUpdatedIdMVADiPhotons"
else:
    process.flashggDiPhotonSystematics.src = "flashggDiPhotons"
print "input to flashggDiPhotonSystematics = ", process.flashggDiPhotonSystematics.src

## Or use the official  tool instead  ????????????????
useEGMTools(process)

## if data, apply only energy scale corrections, if MC apply only energy smearings
if ("data" in customize.processId):
    print 'data' 
    customizePhotonSystematicsForData(process)    # only central value, no syst. shifts 
else:
    print 'mc'
    customizePhotonSystematicsForMC(process)
    ##syst (1D) 
    vpset   = process.flashggDiPhotonSystematics.SystMethods
    newvpset = cms.VPSet()
    for pset in vpset:
        pset.NSigmas = cms.vint32() # no up/down syst shifts
        pset.ApplyCentralValue = cms.bool(False) # no central value
        if ( pset.Label.value().count("MCSmear") or pset.Label.value().count("SigmaEOverESmearing")):
            pset.ApplyCentralValue = cms.bool(True)
        newvpset+= [pset]
    process.flashggDiPhotonSystematics.SystMethods = newvpset        
    ##syst (2D) : smearings with EGMTool
    vpset2D   = process.flashggDiPhotonSystematics.SystMethods2D
    newvpset2D = cms.VPSet()
    for pset in vpset2D:
        pset.NSigmas = cms.PSet( firstVar = cms.vint32(), secondVar = cms.vint32() ) # only central value, no up/down syst shifts (2D case)
        if ( pset.Label.value().count("MCSmear") or pset.Label.value().count("SigmaEOverESmearing")):
            pset.ApplyCentralValue = cms.bool(True)
            newvpset2D+= [pset]
    process.flashggDiPhotonSystematics.SystMethods2D = newvpset2D       
############################
#    Systematics end       #
############################

process.load("flashgg.Taggers.flashggUpdatedIdMVADiPhotons_cfi")
#process.p = cms.Path(process.dataRequirements*process.flashggDiPhotonMVA*process.DiPhotonWithZeeMVADumper*process.diphotonDumper)*process.flashggDiPhotonSystematics
process.p = cms.Path(process.flashggUpdatedIdMVADiPhotons*process.flashggDiPhotonSystematics*process.dataRequirements*process.diphotonDumper)
#process.p = cms.Path(process.dataRequirements*process.diphotonDumper)


customize(process)
