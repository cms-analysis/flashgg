#!/usr/bin/env cmsRun

import FWCore.ParameterSet.Config as cms
import FWCore.Utilities.FileUtils as FileUtils
import FWCore.ParameterSet.VarParsing as VarParsing
from flashgg.Systematics.SystematicDumperDefaultVariables import minimalVariables,minimalHistograms,minimalNonSignalVariables,systematicVariables
from flashgg.Systematics.SystematicDumperDefaultVariables import minimalVariablesHTXS,systematicVariablesHTXS
import os

# SYSTEMATICS SECTION
dropVBFInNonGold = False  # for 2015 only!

process = cms.Process("FLASHggSyst")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.load("Configuration.StandardSequences.GeometryDB_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff")
from Configuration.AlCa.GlobalTag import GlobalTag
if os.environ["CMSSW_VERSION"].count("CMSSW_7_6"):
    process.GlobalTag.globaltag = '76X_mcRun2_asymptotic_v12'
elif os.environ["CMSSW_VERSION"].count("CMSSW_7_4"):
    process.GlobalTag.globaltag = '74X_mcRun2_asymptotic_v4' 
elif os.environ["CMSSW_VERSION"].count("CMSSW_8_0"):
    process.GlobalTag.globaltag = '80X_mcRun2_asymptotic_2016_miniAODv2'
elif os.environ["CMSSW_VERSION"].count("CMSSW_9_4"):
    process.GlobalTag.globaltag = '94X_mc2017_realistic_v12'
else:
    raise Exception,"Could not find a sensible CMSSW_VERSION for default globaltag"

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )
process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32( 1000 )

MUON_ID = "Tight" #["Tight", "Medium" , "Loose", "Soft", "HighPt", "MediumPrompt", "TrkHighPt"]
MUON_ISO = "LooseRel" #{ LooseID : ["LooseRel"],MediumID:["LooseRel", "TightRel"] , TrkHighPtID:["LooseRelTk", "TightRelTk"], TightIDandIPCut:["LooseRel", "TightRel"], HighPtIDandIPCut:["LooseRelTk", "TightRelTk"] }
from flashgg.Systematics.SystematicsCustomize import *
jetSystematicsInputTags = createStandardSystematicsProducers(process , MUON_ID=MUON_ID , MUON_ISO=MUON_ISO)
if dropVBFInNonGold:
    process.flashggVBFTag.SetArbitraryNonGoldMC = True
    process.flashggVBFTag.DropNonGoldData = True
modifyTagSequenceForSystematics(process,jetSystematicsInputTags)


systlabels = [""]
phosystlabels = []
metsystlabels = []
jetsystlabels = []
elesystlabels = []
musystlabels = []

from flashgg.MetaData.JobConfig import customize
customize.options.register('tthTagsOnly',
                           True,
                           VarParsing.VarParsing.multiplicity.singleton,
                           VarParsing.VarParsing.varType.bool,
                           'tthTagsOnly'
                           )
customize.options.register('doHTXS',
                           False,
                           VarParsing.VarParsing.multiplicity.singleton,
                           VarParsing.VarParsing.varType.bool,
                           'doHTXS'
                           )
customize.options.register('doMuFilter',
                           True,
                           VarParsing.VarParsing.multiplicity.singleton,
                           VarParsing.VarParsing.varType.bool,
                           'doMuFilter'
                           )
from flashgg.MetaData.JobConfig import customize
customize.options.register('doFiducial',
                           False,
                           VarParsing.VarParsing.multiplicity.singleton,
                           VarParsing.VarParsing.varType.bool,
                           'doFiducial'
                           )
customize.options.register('acceptance',
                           'NONE',
                           VarParsing.VarParsing.multiplicity.singleton,
                           VarParsing.VarParsing.varType.string,
                           'acceptance'
                           )
customize.options.register('doSystematics',
                           #True,
                           False,#for tth analysis
                           VarParsing.VarParsing.multiplicity.singleton,
                           VarParsing.VarParsing.varType.bool,
                           'doSystematics'
                           )
customize.options.register('doPdfWeights',
                           True,
                           VarParsing.VarParsing.multiplicity.singleton,
                           VarParsing.VarParsing.varType.bool,
                           'doPdfWeights'
                           )
customize.options.register('dumpTrees',
                           True,
                           VarParsing.VarParsing.multiplicity.singleton,
                           VarParsing.VarParsing.varType.bool,
                           'dumpTrees'
                           )
customize.options.register('dumpWorkspace',
                           True,
                           VarParsing.VarParsing.multiplicity.singleton,
                           VarParsing.VarParsing.varType.bool,
                           'dumpWorkspace'
                           )

customize.options.register('verboseTagDump',
                           False,
                           VarParsing.VarParsing.multiplicity.singleton,
                           VarParsing.VarParsing.varType.bool,
                           'verboseTagDump'
                           )

print "Printing defaults"
print 'doFiducial '+str(customize.doFiducial)
print 'acceptance '+str(customize.acceptance)
print 'tthTagsOnly '+str(customize.tthTagsOnly)
# import flashgg customization to check if we have signal or background
from flashgg.MetaData.JobConfig import customize
customize.parse()
print "Printing options"
print 'doFiducial '+str(customize.doFiducial)
print 'acceptance '+str(customize.acceptance)
print 'tthTagsOnly '+str(customize.tthTagsOnly)
print 'doMuFilter '+str(customize.doMuFilter)

if customize.doFiducial:
    import flashgg.Systematics.fiducialCrossSectionsCustomize as fc
    fc.leadCut = 1./3.
    fc.subLeadCut = 1./4.
    fc.isoCut = 10.
    fc.etaCut = 2.5
    matchCut = "leadingPhoton.hasMatchedGenPhoton() && subLeadingPhoton.hasMatchedGenPhoton()"
    phoIDcut = '(leadingView().phoIdMvaWrtChosenVtx() >0.320 && subLeadingView().phoIdMvaWrtChosenVtx() >0.320)'
    accCut   = fc.getAccRecoCut()
    
    print process.flashggPreselectedDiPhotons.cut

    if customize.acceptance == 'IN':
        process.flashggPreselectedDiPhotons.cut = cms.string(str(process.flashggPreselectedDiPhotons.cut)[12:-2] +' && '+ str(matchCut)+ ' && '+ str(phoIDcut) +' && ' + str(accCut))

    if customize.acceptance == 'OUT':
        process.flashggPreselectedDiPhotons.cut = cms.string(str(process.flashggPreselectedDiPhotons.cut)[12:-2] +' && '+ str(matchCut)+ ' && '+ str(phoIDcut) +' && !' + str(accCut))
        
    if customize.acceptance == 'NONE':
        process.flashggPreselectedDiPhotons.cut = cms.string(str(process.flashggPreselectedDiPhotons.cut)[12:-2] +' && '+ str(phoIDcut))
    print "Here we print the preslection cut"
    print process.flashggPreselectedDiPhotons.cut

process.load("flashgg/Taggers/flashggTagSequence_cfi")
process.load("flashgg/MicroAOD/flashggDiPhotons_cfi")

if customize.tthTagsOnly:
    process.flashggDiPhotons.whichVertex = cms.uint32(0)
    process.flashggDiPhotons.useZerothVertexFromMicro = cms.bool(True)

print 'here we print the tag sequence before'
print process.flashggTagSequence

if customize.doFiducial:
    from PhysicsTools.PatAlgos.tools.helpers import cloneProcessingSnippet,massSearchReplaceAnyInputTag
    process.flashggTagSequence.remove(process.flashggVBFTag)
    process.flashggTagSequence.remove(process.flashggTTHLeptonicTag)
    process.flashggTagSequence.remove(process.flashggTTHHadronicTag)
    #haven't tested VH tags with fiducial cross-section measurement yet
    process.flashggTagSequence.remove(process.flashggVHEtTag)
    process.flashggTagSequence.remove(process.flashggVHLooseTag)
    process.flashggTagSequence.remove(process.flashggVHTightTag)
    process.flashggTagSequence.remove(process.flashggVHMetTag)
    process.flashggTagSequence.remove(process.flashggWHLeptonicTag)
    process.flashggTagSequence.remove(process.flashggZHLeptonicTag)
    process.flashggTagSequence.remove(process.flashggVHLeptonicLooseTag)
    process.flashggTagSequence.remove(process.flashggVHHadronicTag)
    process.flashggTagSequence.replace(process.flashggUntagged, process.flashggSigmaMoMpToMTag)


#for ttH analysis
if customize.tthTagsOnly:
    process.flashggTagSequence.remove(process.flashggVBFTag)
    process.flashggTagSequence.remove(process.flashggVHMetTag)
    process.flashggTagSequence.remove(process.flashggWHLeptonicTag)
    process.flashggTagSequence.remove(process.flashggZHLeptonicTag)
    process.flashggTagSequence.remove(process.flashggVHLeptonicLooseTag)
    process.flashggTagSequence.remove(process.flashggVHHadronicTag)
#    process.flashggTagSequence.remove(process.flashggUntagged) # JM keep untagged in trees
    process.flashggTagSequence.remove(process.flashggVBFMVA)
    process.flashggTagSequence.remove(process.flashggVBFDiPhoDiJetMVA)

    
print 'here we print the tag sequence after'
print process.flashggTagSequence

if customize.doFiducial:
    print 'we do fiducial and we change tagsorter'
    process.flashggTagSorter.TagPriorityRanges = cms.VPSet(     cms.PSet(TagName = cms.InputTag('flashggSigmaMoMpToMTag')) )


# Relax ttH hadronic settings to get all remaining event inside ttHHadronic tag

#process.flashggTTHHadronicTag.PhoMVAThreshold = cms.double(-0.9)
#process.flashggTTHHadronicTag.leadPhoOverMassTTHHMVAThreshold = cms.double(0.33333)
#process.flashggTTHHadronicTag.MVATTHHMVAThreshold = cms.double(-1.0)  # minimum for untag is -0.405
#process.flashggTTHHadronicTag.jetsNumberTTHHMVAThreshold = cms.int32(2)
#process.flashggTTHHadronicTag.bjetsNumberTTHHMVAThreshold = cms.int32(0)
#process.flashggTTHHadronicTag.bjetsLooseNumberTTHHMVAThreshold = cms.int32(0)  
#process.flashggTTHHadronicTag.tthHadMVAThresholdMin = cms.double(0.65)
#process.flashggTTHHadronicTag.tthHadMVAThresholdMax = cms.double(999.0)
#process.flashggTTHHadronicTag.tthMVAweightfile = cms.FileInPath("flashgg/Taggers/data/TMVAClassification_tth_hadronic_2017Data_17vars_v0.weights.xml")
#process.flashggTTHHadronicTag.tthMVAweightfile = cms.FileInPath("flashgg/Taggers/data/TMVAClassification_tth_hadronic_2017Data_19vars_v0.weights.xml")
#process.flashggTTHHadronicTag.secondMaxBTagTTHHMVAThreshold  = cms.double(0.0)

#process.flashggTTHHadronicTag.leptonPtThreshold = cms.double(500) # FIXME REMOVE THIS test whether all events in untag are 0 leptons events
process.flashggTTHLeptonicTag.MVAThreshold = cms.double(-1.0)
process.flashggTTHLeptonicTag.leptonPtThreshold = cms.double(15)
process.flashggTTHLeptonicTag.jetsNumberThreshold = cms.double(0.)
process.flashggTTHLeptonicTag.bjetsNumberThreshold = cms.double(0.)
process.flashggTTHLeptonicTag.bLoosejetsNumberThreshold = cms.double(0.)


process.flashggUntagged.Boundaries     = cms.vdouble(-1.)

if customize.tthTagsOnly:
    process.flashggTagSorter.TagPriorityRanges = cms.VPSet(     
        cms.PSet(TagName = cms.InputTag('flashggTTHLeptonicTag')),
        cms.PSet(TagName = cms.InputTag('flashggTTHHadronicTag')),
        cms.PSet(TagName = cms.InputTag('flashggUntagged')) 
        )

    print "customize.processId:",customize.processId
    print "Removing FracRV from syst"
    
    newvpset = cms.VPSet()
    for pset in process.flashggDiPhotonSystematics.SystMethods:
        if not pset.Label.value().count("FracRVWeight")and not pset.Label.value().count("FracRVNvtxWeight") :
            print  pset.Label.value()
            newvpset += [pset]

            process.flashggDiPhotonSystematics.SystMethods = newvpset


# Or use the official tool instead
useEGMTools(process)

#for ttH analysis
dipho_variables =   [
			"nVtxTree:=diPhoton().nVert",
			"diPhoMass:=diPhoton().mass", 
			"diPhoMVA:=diPhotonMVA().result",
			"diPhoPt:=diPhoton().pt", 
                        "diPhoPtoM:=diPhoton().pt / diPhoton().mass", 
			"diPhoEta:=diPhoton().eta",
			"diPhoY:=diPhoton().rapidity",
			"diPhoPhi:=diPhoton().phi",  
                        "diPhoVtxProbMVA:=diPhoton().vtxProbMVA", 
                        "diPhoSigmaRV := diPhotonMVA().sigmarv",
                        "diPhoDecorrSigmaRV := diPhotonMVA().decorrSigmarv",
                        "diPhoSigmaWV := diPhotonMVA().sigmawv",
                        "diPhoCosPhi :=  diPhotonMVA().CosPhi",
			"pho1_pt:=diPhoton().leadingPhoton.pt",
                        "pho2_pt:=diPhoton().subLeadingPhoton.pt",
                        "pho1_sigmaEOverE:=diPhoton().leadingPhoton.sigEOverE()",
                        "pho2_sigmaEOverE:=diPhoton().subLeadingPhoton.sigEOverE()",
                        "pho1_ptoM:=diPhoton().leadingPhoton.pt / diPhoton().mass",
			"pho2_ptoM:=diPhoton().subLeadingPhoton.pt / diPhoton().mass",
			"pho1_eta:=diPhoton().leadingPhoton.eta",
			"pho2_eta:=diPhoton().subLeadingPhoton.eta",
			"pho1_phi:=diPhoton().leadingPhoton.phi",
                        "pho2_phi:=diPhoton().subLeadingPhoton.phi",
			"pho1_isEB:=diPhoton().leadingPhoton.isEB",
			"pho2_isEB:=diPhoton().subLeadingPhoton.isEB",
			"pho1_sceta:=diPhoton().leadingPhoton.superCluster.eta",
			"pho2_sceta:=diPhoton().subLeadingPhoton.superCluster.eta",
			"pho1_scphi:=diPhoton().leadingPhoton.superCluster.phi",
			"pho2_scphi:=diPhoton().subLeadingPhoton.superCluster.phi",
			"pho1_idmva:=diPhoton.leadingView.phoIdMvaWrtChosenVtx",
                 	"pho2_idmva:=diPhoton.subLeadingView.phoIdMvaWrtChosenVtx",
			"maxPhoID:=max(diPhoton.leadingView.phoIdMvaWrtChosenVtx,diPhoton.subLeadingView.phoIdMvaWrtChosenVtx)",
			"minPhoID:=min(diPhoton.leadingView.phoIdMvaWrtChosenVtx,diPhoton.subLeadingView.phoIdMvaWrtChosenVtx)",                 
			"pho1_r9:=diPhoton().leadingPhoton.full5x5_r9",
			"pho2_r9:=diPhoton().subLeadingPhoton.full5x5_r9",
                        "pho1_genmatch := ?(diPhoton().leadingPhoton.hasGenMatchType)? diPhoton().leadingPhoton.genMatchType : -999",
                        "pho2_genmatch := ?(diPhoton().subLeadingPhoton.hasGenMatchType)? diPhoton().subLeadingPhoton.genMatchType : -999",
			"vtxZ:=diPhoton().vtx().z",
                        "vtxI:=diPhoton().vertexIndex",
                       # "dZ:=(tagTruth().genPV().z-diPhoton().vtx().z)",
#                        SigmaEOverESmearing
			#"weight_standardWeight:=weight",
			"weight_totalWeight:=weight(\"totalWeight\")",
			"weight_lumiWeight:=weight(\"lumiWeightCentral\")",
			"weight_PreselSF:=weight(\"PreselSFCentral\")",
			"weight_electronVetoSF:=weight(\"electronVetoSFCentral\")",
			"weight_TriggerWeight:=weight(\"TriggerWeightCentral\")",
#			"weight_FracRVWeight:=weight(\"FracRVWeightCentral\")",
			"weight_LooseMvaSF:=weight(\"LooseMvaSFCentral\")",
			"weight_ElectronWeight:=weight(\"ElectronWeightCentral\")",
			"weight_MuonWeight:=weight(\"MuonWeightCentral\")",
			"weight_JetBTagWeight:=weight(\"JetBTagWeightCentral\")",
			"weight_JetBTagCutWeight:=weight(\"JetBTagCutWeightCentral\")",
			"weight_JetBTagReshapeWeight:=weight(\"JetBTagReshapeWeightCentral\")",
			"weight_UnmatchedPUWeight:=weight(\"UnmatchedPUWeightCentral\")",
			"weight_MvaLinearSyst:=weight(\"MvaLinearSystCentral\")",
			"weight_puweight:=weight(\"puweightCentral\")",

			#"n_ele    := ?(electrons)? electrons.size : -1",
	       	     ]

ttHLeptonic_variables = [
                        "nElec:=electrons.size",
			"elecPt_1:= ?(electrons.size>0)? electrons.at(0).pt : -999",
			"elecEta_1:= ?(electrons.size>0)? electrons.at(0).eta : -999",
			"elecPhi_1:= ?(electrons.size>0)? electrons.at(0).phi : -999",
			"elecScEta_1:= ?(electrons.size>0)? electrons.at(0).superCluster.eta : -999",
			"elecScPhi_1:= ?(electrons.size>0)? electrons.at(0).superCluster.phi : -999",
			"dRPhoLeadElec_1:= ?(electrons.size>0)? deltaR(diPhoton().leadingPhoton.eta, diPhoton().leadingPhoton.phi, electrons.at(0).eta, electrons.at(0).phi) : -999",
			"dRPhoSubLeadElec_1:= ?(electrons.size>0)? deltaR(diPhoton().subLeadingPhoton.eta, diPhoton().subLeadingPhoton.phi, electrons.at(0).eta, electrons.at(0).phi) : -999",
#			"elecIsSelMVA80_1:= ?(electrons.size>0)? electrons.at(0).passMVATightId : -999",
#			"elecIsSelMVA90_1:= ?(electrons.size>0)? electrons.at(0).passMVAMediumId : -999",
#			"elecIsSelCutLoose_1:= ?(electrons.size>0)? electrons.at(0).passLooseId : -999",
#			"elecIsSelCutMedium_1:= ?(electrons.size>0)? electrons.at(0).passMediumId : -999",
#			"elecMiniIso_1:= ?(electrons.size>0)? electrons.at(0).fggMiniIsoSumRel : -999",
			
			#"elecPt_2:= ?(electrons.size>1)? electrons.at(1).pt : -999",
                        #"elecEta_2:= ?(electrons.size>1)? electrons.at(1).eta : -999",
                        #"elecPhi_2:= ?(electrons.size>1)? electrons.at(1).phi : -999",
                        #"elecScEta_2:= ?(electrons.size>1)? electrons.at(1).superCluster.eta : -999",
                        #"elecScPhi_2:= ?(electrons.size>1)? electrons.at(1).superCluster.phi : -999",
                        #"dRPhoLeadElec_2:= ?(electrons.size>1)? deltaR(diPhoton().leadingPhoton.eta, diPhoton().leadingPhoton.phi, electrons.at(1).eta, electrons.at(1).phi) : -999",
                        #"dRPhoSubLeadElec_2:= ?(electrons.size>1)? deltaR(diPhoton().subLeadingPhoton.eta, diPhoton().subLeadingPhoton.phi, electrons.at(1).eta, electrons.at(1).phi) : -999",
                        #"elecIsSelMVA80_2:= ?(electrons.size>1)? electrons.at(1).passMVATightId : -999",
                        #"elecIsSelMVA90_2:= ?(electrons.size>1)? electrons.at(1).passMVAMediumId : -999",
                        #"elecIsSelCutLoose_2:= ?(electrons.size>1)? electrons.at(1).passLooseId : -999",
                        #"elecIsSelCutMedium_2:= ?(electrons.size>1)? electrons.at(1).passMediumId : -999",
                        #"elecMiniIso_2:= ?(electrons.size>1)? electrons.at(1).fggMiniIsoSumRel : -999",

			#"elecPt_3:= ?(electrons.size>2)? electrons.at(2).pt : -999",
                        #"elecEta_3:= ?(electrons.size>2)? electrons.at(2).eta : -999",
                        #"elecPhi_3:= ?(electrons.size>2)? electrons.at(2).phi : -999",
                        #"elecScEta_3:= ?(electrons.size>2)? electrons.at(2).superCluster.eta : -999",
                        #"elecScPhi_3:= ?(electrons.size>2)? electrons.at(2).superCluster.phi : -999",
                        #"dRPhoLeadElec_3:= ?(electrons.size>2)? deltaR(diPhoton().leadingPhoton.eta, diPhoton().leadingPhoton.phi, electrons.at(2).eta, electrons.at(2).phi) : -999",
                        #"dRPhoSubLeadElec_3:= ?(electrons.size>2)? deltaR(diPhoton().subLeadingPhoton.eta, diPhoton().subLeadingPhoton.phi, electrons.at(2).eta, electrons.at(2).phi) : -999",
                        #"elecIsSelMVA80_3:= ?(electrons.size>2)? electrons.at(2).passMVATightId : -999",
                        #"elecIsSelMVA90_3:= ?(electrons.size>2)? electrons.at(2).passMVAMediumId : -999",
                        #"elecIsSelCutLoose_3:= ?(electrons.size>2)? electrons.at(2).passLooseId : -999",
                        #"elecIsSelCutMedium_3:= ?(electrons.size>2)? electrons.at(2).passMediumId : -999",
                        #"elecMiniIso_3:= ?(electrons.size>2)? electrons.at(2).fggMiniIsoSumRel : -999",
			
			#---

			"nMuons:=muons.size",
                        #"nMuonsTTH:=muons.size",
                        #"nMuonsTTHLep:=muons.size",
                        "muPt_1:= ?(muons.size>0)? muons.at(0).pt : -999",
                        "muEta_1:= ?(muons.size>0)? muons.at(0).eta : -999",
                        "muPhi_1:= ?(muons.size>0)? muons.at(0).phi : -999",
                        "dRPhoLeadMuon_1:= ?(muons.size>0)? deltaR(diPhoton().leadingPhoton.eta, diPhoton().leadingPhoton.phi, muons.at(0).eta, muons.at(0).phi) : -999",
                        "dRPhoSubLeadMuon_1:= ?(muons.size>0)? deltaR(diPhoton().subLeadingPhoton.eta, diPhoton().subLeadingPhoton.phi, muons.at(0).eta, muons.at(0).phi) : -999",
 #                       "isTightMu_1:= ?(muons.size>0)? 1 : 0",
 #                       "muPFIsoSumRel_1:= ?(muons.size>0)? muons.at(0).fggPFIsoSumRelR04 : -999",
 #                       "muMiniIso_1:= ?(muons.size>0)? muons.at(0).fggMiniIsoSumRel : -999",
			#"muMiniIsoCh_1:= ?(muons.size>0)? muons.at(0).fggMiniIsoCharged : -999",
			#"muMiniIsoNe_1:= ?(muons.size>0)? muons.at(0).fggMiniIsoNeutrals : -999",
                        #"muMiniIsoPh_1:= ?(muons.size>0)? muons.at(0).fggMiniIsoPhotons : -999",
                        #"muMiniIsoPh2_1:= ?(muons.size>0)? muons.at(0).fggMiniIsoPhotons2 : -999",
                        #"muMiniIsoPU_1:= ?(muons.size>0)? muons.at(0).fggMiniIsoPUCharged : -999",

                        #"muPt_2:= ?(muons.size>1)? muons.at(1).pt : -999",
                        #"muEta_2:= ?(muons.size>1)? muons.at(1).eta : -999",
                        #"muPhi_2:= ?(muons.size>1)? muons.at(1).phi : -999",
                        #"dRPhoLeadMuon_2:= ?(muons.size>1)? deltaR(diPhoton().leadingPhoton.eta, diPhoton().leadingPhoton.phi, muons.at(1).eta, muons.at(1).phi) : -999",
                        #"dRPhoSubLeadMuon_2:= ?(muons.size>1)? deltaR(diPhoton().subLeadingPhoton.eta, diPhoton().subLeadingPhoton.phi, muons.at(1).eta, muons.at(1).phi) : -999",
                        #"isTightMu_2:= ?(muons.size>1)? 1 : 0",
                        #"muPFIsoSumRel_2:= ?(muons.size>1)? muons.at(1).fggPFIsoSumRelR04 : -999",
                        #"muMiniIso_2:= ?(muons.size>1)? muons.at(1).fggMiniIsoSumRel : -999",

                        #"muPt_3:= ?(muons.size>2)? muons.at(2).pt : -999",
                        #"muEta_3:= ?(muons.size>2)? muons.at(2).eta : -999",
                        #"muPhi_3:= ?(muons.size>2)? muons.at(2).phi : -999",
                        #"dRPhoLeadMuon_3:= ?(muons.size>2)? deltaR(diPhoton().leadingPhoton.eta, diPhoton().leadingPhoton.phi, muons.at(2).eta, muons.at(2).phi) : -999",
                        #"dRPhoSubLeadMuon_3:= ?(muons.size>2)? deltaR(diPhoton().subLeadingPhoton.eta, diPhoton().subLeadingPhoton.phi, muons.at(2).eta, muons.at(2).phi) : -999",
                        #"isTightMu_3:= ?(muons.size>2)? 1 : 0",
                        #"muPFIsoSumRel_3:= ?(muons.size>2)? muons.at(2).fggPFIsoSumRelR04 : -999",
                        #"muMiniIso_3:= ?(muons.size>2)? muons.at(2).fggMiniIsoSumRel : -999",

                        #---   

                        "nJets:= jets.size",
                        "nJetsBm:= bJets.size"
                        
                     ]

ttHHadronic_variables = [
                        "nJets:= nJet",
                        "nJetsBm:= nBMedium",
			"nJetsBl:= nBLoose",
			"nJetsBt:= nBTight",
			"leadJetPt:=leadJetPt",
			"subLeadJetPt:=subLeadJetPt",
			"sumJetPt:=sumJetPt",
			"maxBTagVal:=maxBTagVal",
			"secondMaxBTagVal:=secondMaxBTagVal",
			"thirdMaxBTagVal:=thirdMaxBTagVal",
			"fourthMaxBTagVal:=fourthMaxBTagVal",
			"tthMvaRes:=tthMvaRes",
                        "jetPt_1:= ?(jetVector.size>0)? jetVector.at(0).pt : -999",
                        "jetEta_1:= ?(jetVector.size>0)? jetVector.at(0).eta : -999",
                        "jetPhi_1:= ?(jetVector.size>0)? jetVector.at(0).phi : -999",
                        "mindRPhoLeadJet:= ?(jetVector.size>1)? min(deltaR(diPhoton().leadingPhoton.eta, diPhoton().leadingPhoton.phi,  jetVector.at(0).eta,  jetVector.at(0).phi),deltaR(diPhoton().leadingPhoton.eta, diPhoton().leadingPhoton.phi, jetVector.at(1).eta, jetVector.at(1).phi)) : -999 ",
                        "maxdRPhoLeadJet:= ?(jetVector.size>1)? max(deltaR(diPhoton().leadingPhoton.eta, diPhoton().leadingPhoton.phi,  jetVector.at(0).eta,  jetVector.at(0).phi),deltaR(diPhoton().leadingPhoton.eta, diPhoton().leadingPhoton.phi, jetVector.at(1).eta, jetVector.at(1).phi)) : -999 ",                        
                        "dRPhoLeadJet_1:= ?(jetVector.size>0)? deltaR(diPhoton().leadingPhoton.eta, diPhoton().leadingPhoton.phi, jetVector.at(0).eta, jetVector.at(0).phi) : -999",
                        "dRPhoSubLeadJet_1:= ?(jetVector.size>0)? deltaR(diPhoton().subLeadingPhoton.eta, diPhoton().subLeadingPhoton.phi, jetVector.at(0).eta, jetVector.at(0).phi) : -999",
                        "btag_1:=  ?(jetVector.size>0)? jetVector.at(0).bDiscriminator('pfDeepCSVJetTags:probb')+jetVector.at(0).bDiscriminator('pfDeepCSVJetTags:probbb') : -999",     
                        "jetHFlav_1:= ?(jetVector.size>0)? jetVector.at(0).hadronFlavour : -999", 
                        "jetPFlav_1:= ?(jetVector.size>0)? jetVector.at(0).partonFlavour : -999",                                                                             

                        "jetPt_2:= ?(jetVector.size>1)? jetVector.at(1).pt : -999",
                        "jetEta_2:= ?(jetVector.size>1)? jetVector.at(1).eta : -999",
                        "jetPhi_2:= ?(jetVector.size>1)? jetVector.at(1).phi : -999",
                        "dRPhoLeadJet_2:= ?(jetVector.size>1)? deltaR(diPhoton().leadingPhoton.eta, diPhoton().leadingPhoton.phi, jetVector.at(1).eta, jetVector.at(1).phi) : -999",
                        "dRPhoSubLeadJet_2:= ?(jetVector.size>1)? deltaR(diPhoton().subLeadingPhoton.eta, diPhoton().subLeadingPhoton.phi, jetVector.at(1).eta, jetVector.at(1).phi) : -999",
			"btag_2:=  ?(jetVector.size>1)? jetVector.at(1).bDiscriminator('pfDeepCSVJetTags:probb')+jetVector.at(1).bDiscriminator('pfDeepCSVJetTags:probbb') : -999",     
                        "jetHFlav_2:= ?(jetVector.size>1)? jetVector.at(1).hadronFlavour : -999", 
                        "jetPFlav_2:= ?(jetVector.size>1)? jetVector.at(1).partonFlavour : -999", 

                        "jetPt_3:= ?(jetVector.size>2)? jetVector.at(2).pt : -999",
                        "jetEta_3:= ?(jetVector.size>2)? jetVector.at(2).eta : -999",
                        "jetPhi_3:= ?(jetVector.size>2)? jetVector.at(2).phi : -999",
                        "dRPhoLeadJet_3:= ?(jetVector.size>2)? deltaR(diPhoton().leadingPhoton.eta, diPhoton().leadingPhoton.phi, jetVector.at(2).eta, jetVector.at(2).phi) : -999",
                        "dRPhoSubLeadJet_3:= ?(jetVector.size>2)? deltaR(diPhoton().subLeadingPhoton.eta, diPhoton().subLeadingPhoton.phi, jetVector.at(2).eta, jetVector.at(2).phi) : -999",
			"btag_3:=  ?(jetVector.size>2)? jetVector.at(2).bDiscriminator('pfDeepCSVJetTags:probb')+jetVector.at(2).bDiscriminator('pfDeepCSVJetTags:probbb') : -999",     
                        "jetPt_4:= ?(jetVector.size>3)? jetVector.at(3).pt : -999",
                        "jetEta_4:= ?(jetVector.size>3)? jetVector.at(3).eta : -999",
                        "jetPhi_4:= ?(jetVector.size>3)? jetVector.at(3).phi : -999",
                        "dRPhoLeadJet_4:= ?(jetVector.size>3)? deltaR(diPhoton().leadingPhoton.eta, diPhoton().leadingPhoton.phi, jetVector.at(3).eta, jetVector.at(3).phi) : -999",
                        "dRPhoSubLeadJet_4:= ?(jetVector.size>3)? deltaR(diPhoton().subLeadingPhoton.eta, diPhoton().subLeadingPhoton.phi, jetVector.at(3).eta, jetVector.at(3).phi) : -999",
			"btag_4:=  ?(jetVector.size>3)? jetVector.at(3).bDiscriminator('pfDeepCSVJetTags:probb')+jetVector.at(3).bDiscriminator('pfDeepCSVJetTags:probbb') : -999",     
                        "jetPt_5:= ?(jetVector.size>4)? jetVector.at(4).pt : -999",
                        "jetEta_5:= ?(jetVector.size>4)? jetVector.at(4).eta : -999",
                        "jetPhi_5:= ?(jetVector.size>4)? jetVector.at(4).phi : -999",
                        "dRPhoLeadJet_5:= ?(jetVector.size>4)? deltaR(diPhoton().leadingPhoton.eta, diPhoton().leadingPhoton.phi, jetVector.at(4).eta, jetVector.at(4).phi) : -999",
                        "dRPhoSubLeadJet_5:= ?(jetVector.size>4)? deltaR(diPhoton().subLeadingPhoton.eta, diPhoton().subLeadingPhoton.phi, jetVector.at(4).eta, jetVector.at(4).phi) : -999",
			"btag_5:=  ?(jetVector.size>4)? jetVector.at(4).bDiscriminator('pfDeepCSVJetTags:probb')+jetVector.at(4).bDiscriminator('pfDeepCSVJetTags:probbb') : -999",     

			#"jetPt_6:= ?(jetVector.size>5)? jetVector.at(5).pt : -999",
                        #"jetEta_6:= ?(jetVector.size>5)? jetVector.at(5).eta : -999",
                        #"jetPhi_6:= ?(jetVector.size>5)? jetVector.at(5).phi : -999",
                        #"dRPhoLeadJet_6:= ?(jetVector.size>5)? deltaR(diPhoton().leadingPhoton.eta, diPhoton().leadingPhoton.phi, jetVector.at(5).eta, jetVector.at(5).phi) : -999",
                        #"dRPhoSubLeadJet_6:= ?(jetVector.size>5)? deltaR(diPhoton().subLeadingPhoton.eta, diPhoton().subLeadingPhoton.phi, jetVector.at(5).eta, jetVector.at(5).phi) : -999",
                        #"btag_6:= ?(jetBTagValVec.size>5)? jetBTagValVec.at(5) : -999",

			#"jetPt_7:= ?(jetVector.size>6)? jetVector.at(6).pt : -999",
                        #"jetEta_7:= ?(jetVector.size>6)? jetVector.at(6).eta : -999",
                        #"jetPhi_7:= ?(jetVector.size>6)? jetVector.at(6).phi : -999",
                        #"dRPhoLeadJet_7:= ?(jetVector.size>6)? deltaR(diPhoton().leadingPhoton.eta, diPhoton().leadingPhoton.phi, jetVector.at(6).eta, jetVector.at(6).phi) : -999",
                        #"dRPhoSubLeadJet_7:= ?(jetVector.size>6)? deltaR(diPhoton().subLeadingPhoton.eta, diPhoton().subLeadingPhoton.phi, jetVector.at(6).eta, jetVector.at(6).phi) : -999",
                        #"btag_7:= ?(jetBTagValVec.size>6)? jetBTagValVec.at(6) : -999",

			#"jetPt_8:= ?(jetVector.size>7)? jetVector.at(7).pt : -999",
                        #"jetEta_8:= ?(jetVector.size>7)? jetVector.at(7).eta : -999",
                        #"jetPhi_8:= ?(jetVector.size>7)? jetVector.at(7).phi : -999",
                        #"dRPhoLeadJet_8:= ?(jetVector.size>7)? deltaR(diPhoton().leadingPhoton.eta, diPhoton().leadingPhoton.phi, jetVector.at(7).eta, jetVector.at(7).phi) : -999",
                        #"dRPhoSubLeadJet_8:= ?(jetVector.size>7)? deltaR(diPhoton().subLeadingPhoton.eta, diPhoton().subLeadingPhoton.phi, jetVector.at(7).eta, jetVector.at(7).phi) : -999",
                        #"btag_8:= ?(jetBTagValVec.size>7)? jetBTagValVec.at(7) : -999",


                        "bjetPt_1:= ?(bJetVector.size>0)? bJetVector.at(0).pt : -999",
                        "bjetEta_1:= ?(bJetVector.size>0)? bJetVector.at(0).eta : -999",
                        "bjetPhi_1:= ?(bJetVector.size>0)? bJetVector.at(0).phi : -999",
                        "bjetBtag_1 := ?(bJetVector.size>0)? bJetVector.at(0).bDiscriminator('pfDeepCSVJetTags:probb')+ bJetVector.at(0).bDiscriminator('pfDeepCSVJetTags:probbb') : -999",
                        "bjetHFlav_1 := ?(bJetVector.size>0)? bJetVector.at(0).hadronFlavour() : -999",
                        "bjetPFlav_1 := ?(bJetVector.size>0)? bJetVector.at(0).partonFlavour() : -999",
                        "bjetPt_2:= ?(bJetVector.size>1)? bJetVector.at(1).pt : -999", 
                        "bjetEta_2:= ?(bJetVector.size>1)? bJetVector.at(1).eta : -999",
                        "bjetPhi_2:= ?(bJetVector.size>1)? bJetVector.at(1).phi : -999",
                        "bjetBtag_2 := ?(bJetVector.size>1)? bJetVector.at(1).bDiscriminator('pfDeepCSVJetTags:probb')+ bJetVector.at(1).bDiscriminator('pfDeepCSVJetTags:probbb')  : -999",
                        "bjetHFlav_2 := ?(bJetVector.size>1)? bJetVector.at(1).hadronFlavour() : -999",
                        "bjetPFlav_2 := ?(bJetVector.size>1)? bJetVector.at(1).partonFlavour() : -999",

                     ]


ttHGlobalHistograms = ["CMS_hgg_mass>>mass(160,100,180)"
		      ]

minimalVariables+= dipho_variables
minimalNonSignalVariables+= dipho_variables
process.load("flashgg.Taggers.globalVariables_cff")
process.globalVariables.puReWeight = cms.bool(False)

# Only run systematics for signal events
if customize.processId.count("h_") or customize.processId.count("vbf_") or customize.processId.count("Acceptance"): # convention: ggh vbf wzh (wh zh) tth
    print "Signal MC, so adding systematics and dZ"
    variablesToUse = minimalVariables
    if customize.doFiducial:
        variablesToUse.extend(fc.getGenVariables(True))
        variablesToUse.extend(fc.getRecoVariables(True))
        variablesToUse.append("genLeadGenIso := ? diPhoton().leadingPhoton().hasMatchedGenPhoton() ? diPhoton().leadingPhoton().userFloat(\"genIso\") : -99")
        variablesToUse.append("decorrSigmarv := diPhotonMVA().decorrSigmarv")
        variablesToUse.append("leadmva := diPhotonMVA().leadmva")
        variablesToUse.append("subleadmva := diPhotonMVA().subleadmva")

    if customize.doSystematics:
        #for direction in ["Up","Down"]:
            #phosystlabels.append("MvaShift%s01sigma" % direction)
#            phosystlabels.append("MvaLinearSyst%s01sigma" % direction)
            #phosystlabels.append("SigmaEOverEShift%s01sigma" % direction)
            #phosystlabels.append("MaterialCentral%s01sigma" % direction)
            #phosystlabels.append("MaterialForward%s01sigma" % direction)
            #phosystlabels.append("FNUFEB%s01sigma" % direction)
            #phosystlabels.append("FNUFEE%s01sigma" % direction)
            #jetsystlabels.append("JEC%s01sigma" % direction)
            #jetsystlabels.append("JER%s01sigma" % direction)
            #jetsystlabels.append("RMSShift%s01sigma" % direction)
            ##metsystlabels.append("metUncertainty%s01sigma" % direction)
            #variablesToUse.append("UnmatchedPUWeight%s01sigma[1,-999999.,999999.] := weight(\"UnmatchedPUWeight%s01sigma\")" % (direction,direction))
            #variablesToUse.append("MvaLinearSyst%s01sigma[1,-999999.,999999.] := weight(\"MvaLinearSyst%s01sigma\")" % (direction,direction))
            #variablesToUse.append("LooseMvaSF%s01sigma[1,-999999.,999999.] := weight(\"LooseMvaSF%s01sigma\")" % (direction,direction))
            #variablesToUse.append("PreselSF%s01sigma[1,-999999.,999999.] := weight(\"PreselSF%s01sigma\")" % (direction,direction))
            #variablesToUse.append("electronVetoSF%s01sigma[1,-999999.,999999.] := weight(\"electronVetoSF%s01sigma\")" % (direction,direction))
            #variablesToUse.append("TriggerWeight%s01sigma[1,-999999.,999999.] := weight(\"TriggerWeight%s01sigma\")" % (direction,direction))
            #variablesToUse.append("FracRVWeight%s01sigma[1,-999999.,999999.] := weight(\"FracRVWeight%s01sigma\")" % (direction,direction))
            #variablesToUse.append("ElectronWeight%s01sigma[1,-999999.,999999.] := weight(\"ElectronWeight%s01sigma\")" % (direction,direction))
            #variablesToUse.append("MuonWeight%s01sigma[1,-999999.,999999.] := weight(\"MuonWeight%s01sigma\")" % (direction,direction))
            #variablesToUse.append("JetBTagWeight%s01sigma[1,-999999.,999999.] := weight(\"JetBTagWeight%s01sigma\")" % (direction,direction))
            #for r9 in ["HighR9","LowR9"]:
            #    for region in ["EB","EE"]:
            #        phosystlabels.append("ShowerShape%s%s%s01sigma"%(r9,region,direction))
#                    phosystlabels.append("MCSmear%s%s%s01sigma" % (r9,region,direction))
            #        phosystlabels.append("MCScale%s%s%s01sigma" % (r9,region,direction))
            #        for var in ["Rho","Phi"]:
            #            phosystlabels.append("MCSmear%s%s%s%s01sigma" % (r9,region,var,direction))
        systlabels += phosystlabels
        systlabels += jetsystlabels
        systlabels += metsystlabels
    customizeSystematicsForSignal(process)
elif customize.processId == "Data":
    print "Data, so turn off all shifts and systematics, with some exceptions"
    variablesToUse = minimalNonSignalVariables
    if customize.doFiducial:
        variablesToUse.extend(fc.getRecoVariables(True))
    customizeSystematicsForData(process)
else:
    print "Background MC, so store mgg and central only"
    variablesToUse = minimalNonSignalVariables
    customizeSystematicsForBackground(process)

print "--- Systematics  with independent collections ---"
print systlabels
print "-------------------------------------------------"
print "--- Variables to be dumped, including systematic weights ---"
print variablesToUse
print "------------------------------------------------------------"




#from flashgg.Taggers.globalVariables_cff import globalVariables
#globalVariables.extraFloats.rho = cms.InputTag("rhoFixedGridAll")

#cloneTagSequenceForEachSystematic(process,systlabels,phosystlabels,jetsystlabels,jetSystematicsInputTags)
cloneTagSequenceForEachSystematic(process,systlabels,phosystlabels,metsystlabels,jetsystlabels,jetSystematicsInputTags)
process.flashggTagSorter.CreateNoTag = True # MUST be after tag sequence cloning

###### Dumper section

from FWCore.ParameterSet.VarParsing import VarParsing
from flashgg.MetaData.samples_utils import SamplesManager


process.source = cms.Source ("PoolSource",
                             fileNames = cms.untracked.vstring(
#        "file:/eos/cms/store/group/phys_higgs/cmshgg/sethzenz/flashgg/RunIIFall17-3_0_0/3_0_0/DoubleEG/RunIIFall17-3_0_0-3_0_0-v0-Run2017D-17Nov2017-v1/180325_171732/0000/myMicroAODOutputFile_22.root"
        "file:/eos/cms/store/group/phys_higgs/cmshgg/sethzenz/flashgg/RunIIFall17-3_0_0/3_0_0/ttHJetToGG_M125_13TeV_amcatnloFXFX_madspin_pythia8/RunIIFall17-3_0_0-3_0_0-v0-RunIIFall17MiniAOD-94X_mc2017_realistic_v10-v1/180325_144315/0000/myMicroAODOutputFile_13.root"

))


process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string("test.root"))

process.extraDumpers = cms.Sequence()
process.load("flashgg.Taggers.diphotonTagDumper_cfi") ##  import diphotonTagDumper 
import flashgg.Taggers.dumperConfigTools as cfgTools


process.tagsDumper.className = "DiPhotonTagDumper"
process.tagsDumper.src = "flashggSystTagMerger"
#process.tagsDumper.src = "flashggTagSystematics"
#process.tagsDumper.processId = "test"
process.tagsDumper.dumpTrees = True
process.tagsDumper.dumpWorkspace = False
process.tagsDumper.dumpHistos = False
process.tagsDumper.quietRooFit = False
process.tagsDumper.nameTemplate = cms.untracked.string("$PROCESS_$SQRTS_$CLASSNAME_$SUBCAT_$LABEL")
process.tagsDumper.splitPdfByStage0Cat = cms.untracked.bool(customize.doHTXS)

if customize.options.WeightName :
    lheProduct = customize.dataset[1]["LHESourceName"].split("_")
    #print lheProduct
    process.tagsDumper.LHEEventProduct = cms.untracked.InputTag( str(lheProduct[1]) , str(lheProduct[2]) , str(lheProduct[3]) )
    #print process.tagsDumper.LHEEventProduct
    process.tagsDumper.LHEWeightName = cms.untracked.string(customize.options.WeightName)

if(customize.doFiducial):
#    if customize.processId == "Data":
#        fc.addRecoGlobalVariables(process, process.tagsDumper)
#    else:
    fc.addObservables(process, process.tagsDumper, customize.processId )


if customize.doFiducial:
    tagList=[["SigmaMpTTag",3]]
elif customize.tthTagsOnly:
    tagList=[
        ["UntaggedTag",0],
        ["TTHHadronicTag",2],
        ["TTHLeptonicTag",0]
        ]
else:
    tagList=[
        ["NoTag",0],
        ["UntaggedTag",4],
        ["VBFTag",3],
        ["ZHLeptonicTag",0],
        ["WHLeptonicTag",0],
        ["VHLeptonicLooseTag",0],
        ["VHMetTag",0],
        ["VHHadronicTag",0],
        ["TTHHadronicTag",2],
        ["TTHLeptonicTag",0]
        ]

definedSysts=set()
process.tagsDumper.classifierCfg.remap=cms.untracked.VPSet()
for tag in tagList: 
  tagName=tag[0]
  tagCats=tag[1]
  # remap return value of class-based classifier
  process.tagsDumper.classifierCfg.remap.append( cms.untracked.PSet( src=cms.untracked.string("flashgg%s"%tagName), dst=cms.untracked.string(tagName) ) )
  #for tth analysis
  if tagName == "TTHLeptonicTag":
  	if customize.processId == "Data":
		variablesToUse = minimalNonSignalVariables+ttHLeptonic_variables
	else:
		variablesToUse = minimalVariables+ttHLeptonic_variables
  elif tagName == "TTHHadronicTag":
        if customize.processId == "Data":
                variablesToUse = minimalNonSignalVariables+ttHHadronic_variables
        else:
                variablesToUse = minimalVariables+ttHHadronic_variables
  else:
        if customize.processId == "Data":
                variablesToUse = minimalNonSignalVariables
        else:
                variablesToUse = minimalVariables

  for systlabel in systlabels:
      if not systlabel in definedSysts:
          # the cut corresponding to the systematics can be defined just once
          cutstring = "hasSyst(\"%s\") "%(systlabel)
          definedSysts.add(systlabel)
      else:
          cutstring = None
      if systlabel == "":
          currentVariables = variablesToUse
      else:
          currentVariables = systematicVariables
      
      isBinnedOnly = (systlabel !=  "")
      if ( customize.doPdfWeights or customize.doSystematics ) and ( (customize.datasetName() and customize.datasetName().count("HToGG")) or customize.processId.count("h_") or customize.processId.count("vbf_") ) and (systlabel ==  ""):
          print "Signal MC central value, so dumping PDF weights"
          dumpPdfWeights = False #True
          nPdfWeights = 60
          nAlphaSWeights = 2
          nScaleWeights = 9
      else:
          print "Data, background MC, or non-central value, or no systematics: no PDF weights"
          dumpPdfWeights = False
          nPdfWeights = -1
          nAlphaSWeights = -1
          nScaleWeights = -1
      
      cfgTools.addCategory(process.tagsDumper,
                           systlabel,
                           classname=tagName,
                           cutbased=cutstring,
                           subcats=tagCats, 
                           variables=currentVariables,
                           histograms=minimalHistograms,
                           binnedOnly=isBinnedOnly,
                           dumpPdfWeights=dumpPdfWeights,
                           nPdfWeights=nPdfWeights,
                           nAlphaSWeights=nAlphaSWeights,
                           nScaleWeights=nScaleWeights,
                           splitPdfByStage0Cat=customize.doHTXS
                           )

# Require standard diphoton trigger
#JM: FIXME ME: 2017 HLT to be added...
from HLTrigger.HLTfilters.hltHighLevel_cfi import hltHighLevel
if os.environ["CMSSW_VERSION"].count("CMSSW_9_4"):
    process.hltHighLevel= hltHighLevel.clone(HLTPaths = cms.vstring("HLT_Diphoton30_22_R9Id_OR_IsoCaloId_AND_HE_R9Id_Mass90_v*")) # JM ADDED
else:
    process.hltHighLevel= hltHighLevel.clone(HLTPaths = cms.vstring("HLT_Diphoton30_18_R9Id_OR_IsoCaloId_AND_HE_R9Id_Mass90_v*" ))
    
process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )

# ee bad supercluster filter on data
process.load('RecoMET.METFilters.eeBadScFilter_cfi')
process.eeBadScFilter.EERecHitSource = cms.InputTag("reducedEgamma","reducedEERecHits") # Saved MicroAOD Collection (data only)
# Bad Muon filter
process.dataRequirements = cms.Sequence()
if customize.processId == "Data":
        process.dataRequirements += process.hltHighLevel
        process.dataRequirements += process.eeBadScFilter
        #process.dataRequirements += process.noBadGlobalMuons

# Split WH and ZH
process.genFilter = cms.Sequence()
if (customize.processId.count("wh") or customize.processId.count("zh")) and not customize.processId.count("wzh"):
    process.load("flashgg/Systematics/VHFilter_cfi")
    process.genFilter += process.VHFilter
    process.VHFilter.chooseW = bool(customize.processId.count("wh"))
    process.VHFilter.chooseZ = bool(customize.processId.count("zh"))

if (customize.processId == "th_125" or customize.processId == "bbh_125"):
    process.load("flashgg/Systematics/CentralHiggsFilter_cfi")
    process.genFilter += process.CentralHiggsFilter

#pythia8 has an unanticipated EM showering feature, check have two photons from hard scatter
process.penultimateFilter= cms.Sequence()
if customize.processId == "th_125": # for this sample the filter removes also H -> ZG
    process.load("flashgg/Systematics/HardProcessFinalStateFilter_cfi")
#    process.HardProcessFinalStateFilter.debug = True
    process.penultimateFilter += process.HardProcessFinalStateFilter

# Split out prompt-fake or fake-fake
process.finalFilter = cms.Sequence()
if (customize.processId.count("qcd") or customize.processId.count("gjet")) and customize.processId.count("fake"):
    process.load("flashgg/Systematics/PromptFakeFilter_cfi")
    process.finalFilter += process.PromptFakeFilter
    if (customize.processId.count("promptfake")):
        process.PromptFakeFilter.doPromptFake = cms.bool(True)
        process.PromptFakeFilter.doFakeFake =cms.bool(False)
    elif (customize.processId.count("fakefake")):
        process.PromptFakeFilter.doPromptFake =cms.bool(False)
        process.PromptFakeFilter.doFakeFake =cms.bool(True)
    else:
        raise Exception,"Mis-configuration of python for prompt-fake filter"

#https://cmssdt.cern.ch/lxr/source/RecoBTag/Combined/pfDeepCSVDiscriminatorsJetTags/python_cfi.process

#process.pfDeepCSVDiscriminatorsJetTags = cms.EDProducer(
#    'BTagProbabilityToDiscriminator',
#    discriminators = cms.VPSet(
#        cms.PSet(
#            name = cms.string('BvsAll'),
#            numerator = cms.VInputTag(
#                cms.InputTag('pfDeepCSVJetTags', 'probb'),
#                cms.InputTag('pfDeepCSVJetTags', 'probbb'),
#                ),
#            denominator = cms.VInputTag(),
#            )
#        )
#    )


process.p = cms.Path(process.dataRequirements*
                     process.genFilter*
                     process.flashggDiPhotons* # JM ADDED
                     process.flashggUpdatedIdMVADiPhotons* 
                     process.flashggDiPhotonSystematics*  
                     process.flashggMetSystematics* # JM REMOVED BEFORE....
                     process.flashggMuonSystematics*process.flashggElectronSystematics*
#                     (process.flashggUnpackedJets*process.pfDeepCSVDiscriminatorsJetTags*process.jetSystematicsSequence)* 
                     (process.flashggUnpackedJets*process.jetSystematicsSequence)* 
                     (process.flashggTagSequence*process.systematicsTagSequences)*
                     process.flashggSystTagMerger*
                     process.finalFilter*
                     process.tagsDumper)


if customize.doFiducial:
    if ( customize.doPdfWeights or customize.doSystematics ) and ( (customize.datasetName() and customize.datasetName().count("HToGG")) 
                                                                   or customize.processId.count("h_") or customize.processId.count("vbf_") ) and (systlabel ==  ""):
          print "Signal MC central value, so dumping PDF weights"
          dumpPdfWeights = False #True
          nPdfWeights = 60
          nAlphaSWeights = 2
          nScaleWeights = 9
    else:
          print "Data, background MC, or non-central value, or no systematics: no PDF weights"
          dumpPdfWeights = False
          nPdfWeights = -1
          nAlphaSWeights = -1
          nScaleWeights = -1
    if not customize.processId == "Data":
        fc.addGenOnlyAnalysis(process,customize.processId,customize.acceptance,tagList,systlabels,pdfWeights=(dumpPdfWeights,nPdfWeights,nAlphaSWeights,nScaleWeights))

if( not hasattr(process,"options") ): process.options = cms.untracked.PSet()
process.options.allowUnscheduled = cms.untracked.bool(True)

print "--- Dumping modules that take diphotons as input: ---"
mns = process.p.moduleNames()
for mn in mns:
    module = getattr(process,mn)
    if hasattr(module,"src") and type(module.src) == type(cms.InputTag("")) and module.src.value().count("DiPhoton"):
        print str(module),module.src
    elif hasattr(module,"DiPhotonTag"):
        print str(module),module.DiPhotonTag
print
printSystematicInfo(process)

# Detailed tag interpretation information printout (blinded)
process.flashggTagSorter.StoreOtherTagInfo = True
process.flashggTagSorter.BlindedSelectionPrintout = False #True

#### BELOW HERE IS MOSTLY DEBUGGING STUFF

#####################################################################
## Memory and timing, n.b. igprof is very complicated to interpret ##
##################################################################### 

#from Validation.Performance.TimeMemoryInfo import customise as TimeMemoryCustomize
#TimeMemoryCustomize(process)
#process.MessageLogger.cerr.threshold = 'WARNING'

#process.load("IgTools.IgProf.IgProfTrigger")
#process.igprof.reportEventInterval     = cms.untracked.int32(250)
#process.igprof.reportToFileAtBeginJob  = cms.untracked.string("|gzip -c>igprof.begin-job.gz")
#process.igprof.reportToFileAtEvent     = cms.untracked.string("|gzip -c>igprof.%I.%E.%L.%R.event.gz")
#process.p += process.igprof

################################
## Dump merged tags to screen ##
################################

#process.load("flashgg/Taggers/flashggTagTester_cfi")
#process.flashggTagTester.TagSorter = cms.InputTag("flashggSystTagMerger")
#process.flashggTagTester.ExpectMultiples = cms.untracked.bool(True)
#process.p += process.flashggTagTester

############################################
## Additional details on tag sorter steps ##
############################################

#process.flashggTagSorter.Debug = True

if customize.verboseTagDump:
    process.flashggUpdatedIdMVADiPhotons.Debug = True
    process.flashggTagSorter.Debug = True

##############
## Dump EDM ##
##############

#process.out = cms.OutputModule("PoolOutputModule", fileName = cms.untracked.string('CustomizeWillChangeThisAnyway.root'),
#                               outputCommands = cms.untracked.vstring('keep *') # dump everything! small tests only!
#                               )
#process.e = cms.EndPath(process.out)

############################
## Dump the output Python ##
############################
#print process.dumpPython()
#processDumpFile = open('processDump.py', 'w')
#print >> processDumpFile, process.dumpPython()

# set default options if needed
customize.setDefault("maxEvents",1000)
customize.setDefault("targetLumi",41.7e+3)
# call the customization
customize(process)
