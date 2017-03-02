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
    process.hltFilter.HLTPaths = cms.vstring("HLT_Ele27_WPTight_Gsf_v*")
#    process.hltFilter.HLTPaths = cms.vstring("HLT_Ele22_eta2p1_WPLoose_Gsf_v*")
#elif ("data_double" in customize.processId):
#    process.hltFilter.HLTPaths = cms.vstring("HLT_Diphoton30_18_R9Id_OR_IsoCaloId_AND_HE_R9Id_DoublePixelSeedMatch_Mass70_v*")
elif ("mc_single" in customize.processId):
    process.hltFilter.HLTPaths = cms.vstring("HLT_Ele27_WPTight_Gsf_v*")
#elif ("mc_double" in customize.processId):
#    process.hltFilter.HLTPaths = cms.vstring("HLT_Diphoton30_18_R9Id_OR_IsoCaloId_AND_HE_R9Id_DoublePixelSeedMatch_Mass70_v*")
# process.hltFilter.HLTPaths = cms.vstring("HLT_Ele27_WPTight_Gsf_v*")#for interactive running

process.load('RecoMET.METFilters.eeBadScFilter_cfi')
process.eeBadScFilter.EERecHitSource = cms.InputTag("reducedEgamma","reducedEERecHits") # Saved MicroAOD Collection (data only)
process.dataRequirements = cms.Sequence()
process.dataRequirements += process.hltFilter
process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32( 10000 )

process.load("Configuration.StandardSequences.GeometryDB_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff")
process.RandomNumberGeneratorService = cms.Service("RandomNumberGeneratorService")

if ("data" in customize.processId):
    process.GlobalTag.globaltag = '80X_dataRun2_2016SeptRepro_v3'
else:
    process.GlobalTag.globaltag = '80X_mcRun2_asymptotic_2016_TrancheIV_v6'

# process.GlobalTag.globaltag = '80X_mcRun2_asymptotic_2016_TrancheIV_v6' #for interactive running
    
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(10000) )
    
#process.source = cms.Source ("PoolSource",fileNames = cms.untracked.vstring('/store/group/phys_higgs/cmshgg/ferriff/flashgg/RunIISpring16DR80X-2_0_0-25ns/2_0_0/DYToEE_NNPDF30_13TeV-powheg-pythia8/RunIISpring16DR80X-2_0_0-25ns-2_0_0-v0-RunIISpring16MiniAODv1-PUSpring16_80X_mcRun2_asymptotic_2016_v3-v1/160524_084452/0000/myMicroAODOutputFile_1.root'
#))
process.source = cms.Source ("PoolSource",fileNames = cms.untracked.vstring('/store/group/phys_higgs/cmshgg/sethzenz/flashgg/RunIISummer16-2_4_1-25ns_Moriond17/2_4_1/DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/RunIISummer16-2_4_1-25ns_Moriond17-2_4_1-v0-RunIISummer16MiniAODv2-PUMoriond17_HCALDebug_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/170114_082421/0000/myMicroAODOutputFile_10.root'
))

import flashgg.Taggers.dumperConfigTools as cfgTools
process.load("flashgg.Taggers.globalVariables_cff")
process.globalVariables.puReWeight = cms.bool(True)
if ("data" in customize.processId):
    process.globalVariables.puReWeight = cms.bool(False)
#    process.dataRequirements += process.hltFilter
    process.dataRequirements += process.eeBadScFilter

process.load("flashgg.Taggers.flashggPreselectedDiPhotons_cfi")
process.flashggPreselectedDiPhotons.variables[-1] = "-(passElectronVeto - 1)"
process.flashggPreselectedDiPhotons.src = cms.InputTag("flashggDiPhotonSystematics")

process.load("flashgg.Taggers.flashggUpdatedIdMVADiPhotons_cfi")
process.flashggUpdatedIdMVADiPhotons.doNon5x5transformation = cms.bool(False)

process.load("flashgg/Taggers/flashggDiPhotonMVA_cfi")
process.flashggDiPhotonMVA.DiPhotonTag = cms.InputTag("flashggPreselectedDiPhotons")

process.load("flashgg.Taggers.diphotoMVAWithZeeDumper_cff")
#process.DiPhotonWithZeeMVADumper.src = cms.InputTag("flashggDiPhotonSystematics")
process.DiPhotonWithZeeMVADumper.dumpHistos = False
process.DiPhotonWithZeeMVADumper.dumpTrees  = True
process.DiPhotonWithZeeMVADumper.dumpGlobalVariables = cms.untracked.bool(True)
process.DiPhotonWithZeeMVADumper.globalVariables = process.globalVariables

#process.load("flashgg.Taggers.photonDumper_cfi")

process.load("flashgg.Taggers.diphotonDumper_cfi") 
process.load("flashgg.Taggers.flashggPreselectedDiPhotons_cfi")
process.diphotonDumper.rho = cms.InputTag("fixedGridRhoAll")
process.diphotonDumper.src = cms.InputTag("flashggPreselectedDiPhotons")
#process.diphotonDumper.src = cms.InputTag("flashggDiPhotonSystematics")
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
                       [("All","1", 0)
                        #("EB", "abs(superCluster.eta)<1.479", 0),
                        #("EE", "abs(superCluster.eta)>1.566",0)
                        ],
                       variables=["leadPhIso        := leadingPhoton.pfPhoIso03()",
                                  "leadPhIsoCorr    := leadingPhoton.pfPhoIso03Corr()",
                                  "leadNeuIso       := leadingPhoton.pfNeutIso03()",
                                  "leadfull5x5r9    := leadingPhoton.full5x5_r9",
                                  "leadfull5x5sieie := leadingPhoton.full5x5_sigmaIetaIeta",
                                  "leadfull5x5covieip := leadingPhoton.sieip",
                                  "leadfull5x5covipip := leadingPhoton.sipip",
                                  "leadetawidth     := leadingPhoton.superCluster.etaWidth",
                                  "leadphiwidth     := leadingPhoton.superCluster.phiWidth",
                                  "leads4ratio      := leadingPhoton.s4",
                                  "leadoldr9        := leadingPhoton.old_r9",
                                  "leadoldsieie     := leadingPhoton.sigmaIetaIeta",
                                  "leadoldcovieip   := leadingPhoton.showerShapeVariables.sigmaIetaIphi",
                                  "leadoldcovipip   := leadingPhoton.showerShapeVariables.sigmaIphiIphi",
#                                  "leadr9_5x5_by_3x3:= leadingPhoton.full5x5_r9/leadingPhoton.old_r9",
                                  "leadPt           := leadingPhoton.et",
                                  "leadEta          := leadingPhoton.eta",
                                  "leadIDMVA        := subLeadingView.phoIdMvaWrtChosenVtx()",
                                  "leadChIsoRv      := leadingView.pfChIso03WrtChosenVtx()",
                                  "leadChIsoWv      := leadingPhoton.pfChgIsoWrtWorstVtx03",
                                  "leadESSigma      := leadingPhoton.esEffSigmaRR",
                                  "leadScRawE       := leadingPhoton.superCluster.rawEnergy()",
                                  "leadesEnovSCRawEn:= leadingPhoton.superCluster.preshowerEnergy()/leadingPhoton.superCluster.rawEnergy()",
                                  "subleadPhIso     := subLeadingPhoton.pfPhoIso03()",
                                  "subleadPhIsoCorr := subLeadingPhoton.pfPhoIso03Corr()",
                                  "subleadNeuIso    := subLeadingPhoton.pfNeutIso03()",
                                  "subleadfull5x5r9 := subLeadingPhoton.full5x5_r9",
                                  "subleadfull5x5sieie   := subLeadingPhoton.full5x5_sigmaIetaIeta",
                                  "subleadfull5x5covieip := subLeadingPhoton.sieip",
                                  "subleadfull5x5covipip := subLeadingPhoton.sipip",
                                  "subleadetawidth  := subLeadingPhoton.superCluster.etaWidth",
                                  "subleadphiwidth  := subLeadingPhoton.superCluster.phiWidth",
                                  "subleads4ratio   := subLeadingPhoton.s4",
                                  "subleadoldr9     := subLeadingPhoton.old_r9",
                                  "subleadoldsieie  := subLeadingPhoton.sigmaIetaIeta",
                                  "subleadoldcovieip := subLeadingPhoton.showerShapeVariables.sigmaIetaIphi",
                                  "subleadoldcovipip := subLeadingPhoton.showerShapeVariables.sigmaIphiIphi",
#                                  "subleadr9_5x5_by_3x3:= subLeadingPhoton.full5x5_r9/subLeadingPhoton.old_r9",
                                  "subleadPt        := subLeadingPhoton.et",
                                  "subleadEta       := subLeadingPhoton.eta",
                                  "subIDMVA         := leadingView.phoIdMvaWrtChosenVtx()",
                                  "subleadChIsoRv   := subLeadingView.pfChIso03WrtChosenVtx()",
                                  "subleadChIsoWv   := subLeadingPhoton.pfChgIsoWrtWorstVtx03",
                                  "subleadESSigma   := subLeadingPhoton.esEffSigmaRR",
                                  "subleadScRawE    := subLeadingPhoton.superCluster.rawEnergy()",
                                  "subleadesEnovSCRawEn:= subLeadingPhoton.superCluster.preshowerEnergy()/subLeadingPhoton.superCluster.rawEnergy()",
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
doUpdatedIdMVADiPhotons = True # set to True for 80X corrections to be applied

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
process.p = cms.Path(process.flashggUpdatedIdMVADiPhotons*process.flashggDiPhotonSystematics*process.dataRequirements*process.flashggPreselectedDiPhotons*process.flashggDiPhotonMVA*process.DiPhotonWithZeeMVADumper*process.diphotonDumper)
#process.p = cms.Path(process.dataRequirements*process.diphotonDumper)


customize(process)
