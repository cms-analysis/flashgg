#!/usr/bin/env cmsRun

import FWCore.ParameterSet.Config as cms
import FWCore.Utilities.FileUtils as FileUtils

process = cms.Process("Analysis")

process.load("FWCore.MessageService.MessageLogger_cfi")

# process.source = cms.Source("PoolSource",
#                             fileNames=cms.untracked.vstring("/store/group/phys_higgs/cmshgg/sethzenz/flashgg/RunIIFall17-2_7_7/2_7_7/GJet_Pt-40toInf_DoubleEMEnriched_MGG-80toInf_TuneCP5_13TeV_Pythia8/RunIIFall17-2_7_7-2_7_7-v0-RunIIFall17MiniAOD-94X_mc2017_realistic_v10-v1/180117_155902/0000/myMicroAODOutputFile_1.root"
# ))
process.source = cms.Source("PoolSource",
                            fileNames=cms.untracked.vstring("root://cms-xrd-global.cern.ch//store/user/spigazzi/flashgg/Era2017_RR-31Mar2018_v2/legacyRun2FullV1/GJet_Pt-20to40_DoubleEMEnriched_MGG-80toInf_TuneCP5_13TeV_Pythia8/Era2017_RR-31Mar2018_v2-legacyRun2FullV1-v0-RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/190606_095939/0000/myMicroAODOutputFile_141.root"
))

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )
process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32( 1000 )
process.load("Configuration.StandardSequences.GeometryDB_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff")
process.GlobalTag.globaltag = '94X_mc2017_realistic_v10'

process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string("test.root")
)

process.load("flashgg.Taggers.flashggUpdatedIdMVADiPhotons_cfi") 
process.flashggUpdatedIdMVADiPhotons.reRunRegression = cms.bool(False)
process.flashggUpdatedIdMVADiPhotons.doNon5x5transformation = cms.bool(False)
process.flashggUpdatedIdMVADiPhotons.do5x5correction = cms.bool(False)
process.flashggUpdatedIdMVADiPhotons.doIsoCorrection = cms.bool(False)

from flashgg.Taggers.flashggPreselectedDiPhotons_cfi import flashggPreselectedDiPhotons
process.kinPreselDiPhotons = flashggPreselectedDiPhotons.clone(
src = cms.InputTag("flashggUpdatedIdMVADiPhotons"),
cut=cms.string(
        "mass > 95"
        " && leadingPhoton.pt > 18 && subLeadingPhoton.pt > 18"
        " && abs(leadingPhoton.superCluster.eta)<2.5 && abs(subLeadingPhoton.superCluster.eta)<2.5 "
        " && ( abs(leadingPhoton.superCluster.eta)<1.4442 || abs(leadingPhoton.superCluster.eta)>1.566)"
        " && ( abs(subLeadingPhoton.superCluster.eta)<1.4442 || abs(subLeadingPhoton.superCluster.eta)>1.566)"
        " && (leadingPhoton.pt > 14 && leadingPhoton.hadTowOverEm < 0.15 && (leadingPhoton.full5x5_r9>0.8 || leadingPhoton.chargedHadronIso<20 || leadingPhoton.chargedHadronIso<(0.3*leadingPhoton.pt)))"
        " && (subLeadingPhoton.pt > 14 && subLeadingPhoton.hadTowOverEm < 0.15 && (subLeadingPhoton.full5x5_r9>0.8 || subLeadingPhoton.chargedHadronIso<20 || subLeadingPhoton.chargedHadronIso<(0.3*subLeadingPhoton.pt)))"
        )
)


process.flashggSinglePhotonViews = cms.EDProducer("FlashggSinglePhotonViewProducer",
                                                  DiPhotonTag=cms.InputTag('kinPreselDiPhotons'),                                         
                                                  maxCandidates = cms.int32(1)
                                                  )

process.load("flashgg.Taggers.photonViewDumper_cfi") ##  import diphotonDumper 
import flashgg.Taggers.dumperConfigTools as cfgTools

process.photonViewDumper.src = "flashggSinglePhotonViews"
process.photonViewDumper.dumpTrees = True
process.photonViewDumper.dumpWorkspace = False
process.photonViewDumper.quietRooFit = True

## list of variables to be dumped in trees/datasets. Same variables for all categories
variables=["pt                     := photon.pt",
           "energy                 := photon.energy",
           "eta                    := photon.eta",
           "phi                    := photon.phi",
           "scEta                  := photon.superCluster.eta", 
           "scPhi                  := photon.superCluster.phi", 
           "SCRawE                 := photon.superCluster.rawEnergy",
           "etaWidth               := photon.superCluster.etaWidth",
           "phiWidth               := photon.superCluster.phiWidth",
           "covIphiIphi            := photon.sipip",
           "chgIsoWrtWorstVtx      := photon.pfChgIsoWrtWorstVtx03",
           "phoIso03               := photon.pfPhoIso03",
           "phoIsoCorr             := photon.pfPhoIso03Corr",
           "chgIsoWrtChosenVtx     := pfChIso03WrtChosenVtx",
           "hcalTowerSumEtConeDR03 := photon.hcalTowerSumEtConeDR03",
           "trkSumPtHollowConeDR03 := photon.trkSumPtHollowConeDR03",
           "hadTowOverEm           := photon.hadTowOverEm",
           "idMVA                  := phoIdMvaWrtChosenVtx",
           #"genIso                 := photon.userFloat('genIso')", 
           "eTrue                  := ? photon.hasMatchedGenPhoton ? photon.matchedGenPhoton.energy : 0",
           "sigmaIetaIeta          := photon.full5x5_sigmaIetaIeta",
           "r9                     := photon.full5x5_r9", 
           "esEffSigmaRR           := photon.esEffSigmaRR",
           "s4                     := photon.s4",
           "covIEtaIPhi            := photon.sieip",
           "esEnergy               := photon.superCluster.preshowerEnergy",
           "esEnergyOverRawE       := photon.superCluster.preshowerEnergy/photon.superCluster.rawEnergy"
           #"rho                    := global.rho",
           #"esEnergyPlane1         := photon.esEnergyPlane1",
           #"esEnergyPlane2         := photon.esEnergyPlane2",
           #"e1x3                   := photon.e1x3",
           #"e2x5max                := photon.e2x5max",
           #"e5x5                   := photon.e5x5"
           ]

## list of histograms to be plotted
histograms=["r9>>r9(110,0,1.1)",
            "scEta>>scEta(100,-2.5,2.5)"
            ]

## define categories and associated objects to dump
cfgTools.addCategory(process.photonViewDumper,
                     "Reject",
                     "abs(photon.superCluster.eta)>=1.4442&&abs(photon.superCluster.eta)<=1.566||abs(photon.superCluster.eta)>=2.5",
                     -1 ## if nSubcat is -1 do not store anythings
                     )

# interestng categories 
cfgTools.addCategories(process.photonViewDumper,
                       ## categories definition
                       ## cuts are applied in cascade. Events getting to these categories have already failed the "Reject" selection
                       [("promptPhotons","photon.genMatchType == 1",0),
                        ("fakePhotons",  "photon.genMatchType != 1",0),
                        ],
                       ## variables to be dumped in trees/datasets. Same variables for all categories
                       ## if different variables wanted for different categories, can add categorie one by one with cfgTools.addCategory
                       variables=variables,
                       ## histograms to be plotted. 
                       ## the variables need to be defined first
                       histograms=histograms,
                       )

process.p1 = cms.Path(process.flashggUpdatedIdMVADiPhotons*
                      process.kinPreselDiPhotons*
                      process.flashggSinglePhotonViews*
                      process.photonViewDumper
                      )


from flashgg.MetaData.JobConfig import customize
customize.setDefault("maxEvents",10000)
customize(process)

