#!/usr/bin/env cmsRun

import FWCore.ParameterSet.Config as cms
import FWCore.Utilities.FileUtils as FileUtils
import FWCore.ParameterSet.VarParsing as VarParsing
from flashgg.Systematics.SystematicDumperDefaultVariables import minimalVariables,minimalHistograms,minimalNonSignalVariables,systematicVariables
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
else:
    raise Exception,"Could not find a sensible CMSSW_VERSION for default globaltag"

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(300) )
process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32( 100 )

from flashgg.Systematics.SystematicsCustomize import *
jetSystematicsInputTags = createStandardSystematicsProducers(process)
process.flashggTagSorter.MassCutLower = 60.
process.flashggUntagged.Boundaries = cms.vdouble(-999.,-99.,-9.,-1.)
process.flashggVBFTag.Boundaries = cms.vdouble(9.,999.)
process.flashggTTHLeptonicTag.MVAThreshold = 999.
process.flashggTTHHadronicTag.MVAThreshold = 999.
if dropVBFInNonGold:
    process.flashggVBFTag.SetArbitraryNonGoldMC = True
    process.flashggVBFTag.DropNonGoldData = True
modifyTagSequenceForSystematics(process,jetSystematicsInputTags)

systlabels = [""]
phosystlabels = []
jetsystlabels = []
elesystlabels = []
musystlabels = []

from flashgg.MetaData.JobConfig import customize
customize.options.register('doFiducial',
                           'False',
                           VarParsing.VarParsing.multiplicity.singleton,
                           VarParsing.VarParsing.varType.string,
                           'doFiducial'
                           )

customize.options.register('acceptance',
                           'NONE',
                           VarParsing.VarParsing.multiplicity.singleton,
                           VarParsing.VarParsing.varType.string,
                           'acceptance'
                           )


# import flashgg customization to check if we have signal or background
from flashgg.MetaData.JobConfig import customize
customize.parse()

if customize.doFiducial == 'True':
    matchCut = "leadingPhoton.hasMatchedGenPhoton() && subLeadingPhoton.hasMatchedGenPhoton()"
    phoIDcut = '(leadingView().phoIdMvaWrtChosenVtx() >0.320 && subLeadingView().phoIdMvaWrtChosenVtx() >0.320)'
    accCut = "(leadingPhoton.userFloat(\"genIso\") < 10.0 && subLeadingPhoton.userFloat(\"genIso\") < 10.0 && abs(leadingPhoton.matchedGenPhoton.eta) <2.5 && abs(subLeadingPhoton.matchedGenPhoton.eta) <2.5 && leadingPhoton.matchedGenPhoton.pt / genP4.mass >1.0/3.0 && subLeadingPhoton.matchedGenPhoton.pt / genP4.mass >0.25)"

    print process.flashggPreselectedDiPhotons.cut

    if customize.acceptance == 'IN':
        process.flashggPreselectedDiPhotons.cut = cms.string(str(process.flashggPreselectedDiPhotons.cut)[12:-2] +' && '+ str(matchCut)+ ' && ' + str(accCut))

    if customize.acceptance == 'OUT':
        process.flashggPreselectedDiPhotons.cut = cms.string(str(process.flashggPreselectedDiPhotons.cut)[12:-2] +' && '+ str(matchCut)+ ' && !' + str(accCut))
        
    if customize.acceptance == 'NONE':
        process.flashggPreselectedDiPhotons.cut = cms.string(str(process.flashggPreselectedDiPhotons.cut)[12:-2] +' && '+ str(phoIDcut))
    print "Here we print the preslection cut"
    print process.flashggPreselectedDiPhotons.cut

process.load("flashgg/Taggers/flashggTagSequence_cfi")
print 'here we print the tag sequence before'
print process.flashggTagSequence
if customize.doFiducial == 'True':
    from PhysicsTools.PatAlgos.tools.helpers import cloneProcessingSnippet,massSearchReplaceAnyInputTag
    process.flashggTagSequence.remove(process.flashggVBFTag)
    process.flashggTagSequence.remove(process.flashggTTHLeptonicTag)
    process.flashggTagSequence.remove(process.flashggTTHHadronicTag)
    process.flashggTagSequence.replace(process.flashggUntagged, process.flashggSigmaMoMpToMTag)


print 'here we print the tag sequence after'
print process.flashggTagSequence

if customize.doFiducial == 'True':
    print 'we do fiducial and we change tagsorter'
    process.flashggTagSorter.TagPriorityRanges = cms.VPSet(     cms.PSet(TagName = cms.InputTag('flashggSigmaMoMpToMTag')) )



print "customize.processId:",customize.processId
# load appropriate scale and smearing bins here
# systematics customization scripts will take care of adjusting flashggDiPhotonSystematics
#process.load("flashgg.Systematics.escales.escale76X_16DecRereco_2015")

# Or use the official tool instead
useEGMTools(process)

# Only run systematics for signal events
if customize.processId.count("h_") or customize.processId.count("vbf_") or customize.processId.count("Acceptance"): # convention: ggh vbf wzh (wh zh) tth
    print "Signal MC, so adding systematics and dZ"
    variablesToUse = minimalVariables
    for direction in ["Up","Down"]:
        phosystlabels.append("MvaShift%s01sigma" % direction)
#        phosystlabels.append("MvaLinearSyst%s01sigma" % direction)
        phosystlabels.append("SigmaEOverEShift%s01sigma" % direction)
        phosystlabels.append("MaterialCentral%s01sigma" % direction)
        phosystlabels.append("MaterialForward%s01sigma" % direction)
        phosystlabels.append("FNUFEB%s01sigma" % direction)
        phosystlabels.append("FNUFEE%s01sigma" % direction)
        jetsystlabels.append("JEC%s01sigma" % direction)
        jetsystlabels.append("JER%s01sigma" % direction)
        jetsystlabels.append("RMSShift%s01sigma" % direction)
        variablesToUse.append("UnmatchedPUWeight%s01sigma[1,-999999.,999999.] := weight(\"UnmatchedPUWeight%s01sigma\")" % (direction,direction))
        variablesToUse.append("MvaLinearSyst%s01sigma[1,-999999.,999999.] := weight(\"MvaLinearSyst%s01sigma\")" % (direction,direction))
        variablesToUse.append("LooseMvaSF%s01sigma[1,-999999.,999999.] := weight(\"LooseMvaSF%s01sigma\")" % (direction,direction))
        variablesToUse.append("PreselSF%s01sigma[1,-999999.,999999.] := weight(\"PreselSF%s01sigma\")" % (direction,direction))
        variablesToUse.append("electronVetoSF%s01sigma[1,-999999.,999999.] := weight(\"electronVetoSF%s01sigma\")" % (direction,direction))
        variablesToUse.append("TriggerWeight%s01sigma[1,-999999.,999999.] := weight(\"TriggerWeight%s01sigma\")" % (direction,direction))
        variablesToUse.append("FracRVWeight%s01sigma[1,-999999.,999999.] := weight(\"FracRVWeight%s01sigma\")" % (direction,direction))
        variablesToUse.append("ElectronWeight%s01sigma[1,-999999.,999999.] := weight(\"ElectronWeight%s01sigma\")" % (direction,direction))
        variablesToUse.append("MuonWeight%s01sigma[1,-999999.,999999.] := weight(\"MuonWeight%s01sigma\")" % (direction,direction))
        variablesToUse.append("JetBTagWeight%s01sigma[1,-999999.,999999.] := weight(\"JetBTagWeight%s01sigma\")" % (direction,direction))
        for r9 in ["HighR9","LowR9"]:
            for region in ["EB","EE"]:
                phosystlabels.append("ShowerShape%s%s%s01sigma"%(r9,region,direction))
#                phosystlabels.append("MCSmear%s%s%s01sigma" % (r9,region,direction))
                phosystlabels.append("MCScale%s%s%s01sigma" % (r9,region,direction))
                for var in ["Rho","Phi"]:
                    phosystlabels.append("MCSmear%s%s%s%s01sigma" % (r9,region,var,direction))
    systlabels += phosystlabels
    systlabels += jetsystlabels
    customizeSystematicsForSignal(process)
elif customize.processId == "Data":
    print "Data, so turn of all shifts and systematics, with some exceptions"
    variablesToUse = minimalNonSignalVariables
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

cloneTagSequenceForEachSystematic(process,systlabels,phosystlabels,jetsystlabels,jetSystematicsInputTags)

###### Dumper section

from FWCore.ParameterSet.VarParsing import VarParsing
from flashgg.MetaData.samples_utils import SamplesManager

process.source = cms.Source ("PoolSource",
                             fileNames = cms.untracked.vstring(
        "root://eoscms.cern.ch//eos/cms/store/group/phys_higgs/cmshgg/sethzenz/flashgg/RunIISpring16DR80X-2_2_0-25ns_DYWithPDF/2_2_0/DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/RunIISpring16DR80X-2_2_0-25ns_DYWithPDF-2_2_0-v0-RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/160718_161741/0000/myMicroAODOutputFile_1.root"

#"root://eoscms.cern.ch//eos/cms/store/group/phys_higgs/cmshgg/ferriff/flashgg/RunIISpring16DR80X-2_2_0-25ns_ICHEP16_MiniAODv2/2_2_0/VBFHToGG_M125_13TeV_amcatnlo_pythia8/RunIISpring16DR80X-2_2_0-25ns_ICHEP16_MiniAODv2-2_2_0-v0-RunIISpring16MiniAODv2-PUSpring16RAWAODSIM_reHLT_80X_mcRun2_asymptotic_v14_ext2-v1/160707_150558/0000/myMicroAODOutputFile_25.root"
#"file:/afs/cern.ch/work/s/sethzenz/fromscratch107/CMSSW_8_0_8_patch1/src/flashgg/myMicroAODOutputFile.root"
#"root://eoscms.cern.ch//eos/cms/store/group/phys_higgs/cmshgg/ferriff/flashgg/RunIISpring16DR80X-2_0_0-25ns/2_0_0/VBFHToGG_M-125_13TeV_powheg_pythia8/RunIISpring16DR80X-2_0_0-25ns-2_0_0-v0-RunIISpring16MiniAODv1-PUSpring16RAWAODSIM_80X_mcRun2_asymptotic_2016_v3-v1/160524_093752/0000/myMicroAODOutputFile_1.root"
        #                             "file:myMicroAODOutputFile.root"
        #        "root://eoscms.cern.ch//eos/cms/store/group/phys_higgs/cmshgg/sethzenz/flashgg/RunIISpring15-ReMiniAOD-1_1_0-25ns/1_1_0/VBFHToGG_M-125_13TeV_powheg_pythia8/RunIISpring15-ReMiniAOD-1_1_0-25ns-1_1_0-v0-RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/160105_224017/0000/myMicroAODOutputFile_1.root"
#        "root://eoscms.cern.ch//eos/cms//store/group/phys_higgs/cmshgg/szenz/flashgg/RunIISpring15-ReReco74X-Rerun-1_1_0-25ns/1_2_0/DoubleEG/RunIISpring15-ReReco74X-Rerun-1_1_0-25ns-1_2_0-v0-Run2015D-04Dec2015-v2/160117_214114/0000/myMicroAODOutputFile_10.root"
#        "root://eoscms.cern.ch//eos/cms/store/group/phys_higgs/cmshgg/sethzenz/flashgg/RunIISpring15-ReMiniAOD-1_1_0-25ns/1_1_0/ttHJetToGG_M125_13TeV_amcatnloFXFX_madspin_pythia8/RunIISpring15-ReMiniAOD-1_1_0-25ns-1_1_0-v0-RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/160105_224456/0000/myMicroAODOutputFile_2.root"
        #"root://eoscms.cern.ch//eos/cms//store/group/phys_higgs/cmshgg/sethzenz/flashgg/RunIISpring15-ReMiniAOD-1_1_0-25ns/1_1_0/VHToGG_M120_13TeV_amcatnloFXFX_madspin_pythia8/RunIISpring15-ReMiniAOD-1_1_0-25ns-1_1_0-v0-RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/160105_224138/0000/myMicroAODOutputFile_1.root"
#        "root://eoscms.cern.ch//eos/cms//store/group/phys_higgs/cmshgg/sethzenz/flashgg/RunIIFall15DR76-1_3_0-25ns/1_3_0/DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/RunIIFall15DR76-1_3_0-25ns-1_3_0-v0-RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12_ext1-v1/160126_090235/0000/myMicroAODOutputFile_16.root"
#        "root://eoscms.cern.ch//eos/cms//store/group/phys_higgs/cmshgg/ferriff/flashgg/RunIIFall15DR76-1_3_0-25ns_ext1/1_3_1/ttHJetToGG_M120_13TeV_amcatnloFXFX_madspin_pythia8/RunIIFall15DR76-1_3_0-25ns_ext1-1_3_1-v0-RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/160127_024939/0000/myMicroAODOutputFile_1.root"
#"root://eoscms.cern.ch//eos/cms/store/group/phys_higgs/cmshgg/ferriff/flashgg/RunIIFall15DR76-1_3_0-25ns_ext1/1_3_1/DoubleEG/RunIIFall15DR76-1_3_0-25ns_ext1-1_3_1-v0-Run2015D-16Dec2015-v2/160127_022911/0000/myMicroAODOutputFile_100.root"
#"root://eoscms.cern.ch//eos/cms//store/group/phys_higgs/cmshgg/ferriff/flashgg/RunIIFall15DR76-1_3_0-25ns_ext1/1_3_1/VBFHToGG_M-120_13TeV_powheg_pythia8/RunIIFall15DR76-1_3_0-25ns_ext1-1_3_1-v0-RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/160210_045711/0000/myMicroAODOutputFile_1.root"
#"root://eoscms.cern.ch//eos/cms/store/group/phys_higgs/cmshgg/ferriff/flashgg/RunIIFall15DR76-1_3_0-25ns_ext1/1_3_1/QCD_Pt-30to40_DoubleEMEnriched_MGG-80toInf_TuneCUETP8M1_13TeV_Pythia8/RunIIFall15DR76-1_3_0-25ns_ext1-1_3_1-v0-RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/160127_023721/0000/myMicroAODOutputFile_1.root"
#"root://eoscms.cern.ch//eos/cms/store/group/phys_higgs/cmshgg/ferriff/flashgg/RunIIFall15DR76-1_3_0-25ns_ext1/1_3_1/GJet_Pt-40toInf_DoubleEMEnriched_MGG-80toInf_TuneCUETP8M1_13TeV_Pythia8/RunIIFall15DR76-1_3_0-25ns_ext1-1_3_1-v0-RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/160210_050208/0000/myMicroAODOutputFile_1.root"
#"root://eoscms.cern.ch//eos/cms/store/group/phys_higgs/cmshgg/ferriff/flashgg/RunIISpring16DR80X-2_2_0-25ns_ICHEP16_MiniAODv2/2_2_0/ttHJetToGG_M125_13TeV_amcatnloFXFX_madspin_pythia8_v2/RunIISpring16DR80X-2_2_0-25ns_ICHEP16_MiniAODv2-2_2_0-v0-RunIISpring16MiniAODv2-PUSpring16RAWAODSIM_reHLT_80X_mcRun2_asymptotic_v14-v1/160707_152047/0000/myMicroAODOutputFile_8.root"),skipEvents=cms.untracked.uint32(31500))
))

process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string("test.root"))

process.extraDumpers = cms.Sequence()
process.load("flashgg.Taggers.diphotonTagDumper_cfi") ##  import diphotonTagDumper 
import flashgg.Taggers.dumperConfigTools as cfgTools

process.tagsDumper.className = "DiPhotonTagDumper"
process.tagsDumper.src = "flashggSystTagMerger"
#process.tagsDumper.src = "flashggTagSystematics"
process.tagsDumper.processId = "test"
process.tagsDumper.dumpTrees = True
process.tagsDumper.dumpWorkspace = False
process.tagsDumper.dumpHistos = False
process.tagsDumper.quietRooFit = True
process.tagsDumper.nameTemplate = cms.untracked.string("$PROCESS_$SQRTS_$CLASSNAME_$SUBCAT_$LABEL")

process.flashggPreselectedDiPhotons.variables =  cms.vstring('pfPhoIso03',
                                                             'trkSumPtHollowConeDR03',
                                                             'full5x5_sigmaIetaIeta',
                                                             'full5x5_r9',
                                                             '1-passElectronVeto')


#tagList=[
#["UntaggedTag",4],
#["VBFTag",2],
#["VHTightTag",0],
#["VHLooseTag",0],
#["VHEtTag",0],
#["VHHadronicTag",0],
#["TTHHadronicTag",0],
##["TTHLeptonicTag",0]
#]

if customize.doFiducial == 'True':
    tagList=[["SigmaMpTTag",3]]
else:
    tagList=[
        ["UntaggedTag",4],
        ["VBFTag",2],
        #["VHTightTag",0],
        #["VHLooseTag",0],
        #["VHEtTag",0],
        #["VHHadronicTag",0],
        ["TTHHadronicTag",0],
        ["TTHLeptonicTag",0]
        ]

definedSysts=set()
process.tagsDumper.classifierCfg.remap=cms.untracked.VPSet()
for tag in tagList: 
  tagName=tag[0]
  tagCats=tag[1]
  # remap return value of class-based classifier
  process.tagsDumper.classifierCfg.remap.append( cms.untracked.PSet( src=cms.untracked.string("flashgg%s"%tagName), dst=cms.untracked.string(tagName) ) )
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
      if customize.processId.count("h_") or customize.processId.count("vbf_") and (systlabel ==  ""):
          print "Signal MC central value, so dumping PDF weights"
          dumpPdfWeights = True
          nPdfWeights = 60
          nAlphaSWeights = 2
          nScaleWeights = 9
      else:
          print "Data, background MC, or non-central value: no PDF weights"
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
                           nScaleWeights=nScaleWeights
                           )

# Require standard diphoton trigger
from HLTrigger.HLTfilters.hltHighLevel_cfi import hltHighLevel
#process.hltHighLevel= hltHighLevel.clone(HLTPaths = cms.vstring("HLT_Diphoton30_18_R9Id_OR_IsoCaloId_AND_HE_R9Id_Mass90_v*",
#                                                                "HLT_Diphoton30PV_18PV_R9Id_AND_IsoCaloId_AND_HE_R9Id_DoublePixelVeto_Mass55_v1",
#                                                                "HLT_Diphoton30EB_18EB_R9Id_OR_IsoCaloId_AND_HE_R9Id_DoublePixelVeto_Mass55_v1"
#                                                                ))
#process.hltHighLevel = hltHighLevel.clone(HLTPaths=cms.vstring("HLT_Ele27_eta2p1_WPLoose_Gsf_v*"))
process.hltHighLevel = hltHighLevel.clone(HLTPaths=cms.vstring("HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v*"))

process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )

# ee bad supercluster filter on data
process.load('RecoMET.METFilters.eeBadScFilter_cfi')
process.eeBadScFilter.EERecHitSource = cms.InputTag("reducedEgamma","reducedEERecHits") # Saved MicroAOD Collection (data only)
process.dataRequirements = cms.Sequence()
if customize.processId == "Data":
        process.dataRequirements += process.hltHighLevel
        process.dataRequirements += process.eeBadScFilter

# Split WH and ZH
process.genFilter = cms.Sequence()
if (customize.processId.count("wh") or customize.processId.count("zh")) and not customize.processId.count("wzh"):
    process.load("flashgg/Systematics/VHFilter_cfi")
    process.genFilter += process.VHFilter
    process.VHFilter.chooseW = bool(customize.processId.count("wh"))
    process.VHFilter.chooseZ = bool(customize.processId.count("zh"))

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

process.p = cms.Path(process.dataRequirements*
                     process.genFilter*
                     process.flashggUpdatedIdMVADiPhotons*
                     process.flashggDiPhotonSystematics*
                     process.flashggMuonSystematics*process.flashggElectronSystematics*
                     (process.flashggUnpackedJets*process.jetSystematicsSequence)*
                     (process.flashggTagSequence*process.systematicsTagSequences)*
                     process.flashggSystTagMerger*
                     process.finalFilter*
                     process.tagsDumper)

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
process.flashggTagSorter.BlindedSelectionPrintout = True

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
customize.setDefault("maxEvents",300)
customize.setDefault("targetLumi",1.00e+3)
# call the customization
customize(process)
