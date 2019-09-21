#!/usr/bin/env cmsRun

import FWCore.ParameterSet.Config as cms
import FWCore.Utilities.FileUtils as FileUtils
import FWCore.ParameterSet.VarParsing as VarParsing
from flashgg.Systematics.SystematicDumperDefaultVariables import minimalVariables,minimalHistograms,minimalNonSignalVariables,systematicVariables
from flashgg.Systematics.SystematicDumperDefaultVariables import minimalVariablesHTXS,systematicVariablesHTXS
import os
from flashgg.MetaData.MetaConditionsReader import *

# SYSTEMATICS SECTION
dropVBFInNonGold = False  # for 2015 only!

process = cms.Process("FLASHggSyst")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.load("Configuration.StandardSequences.GeometryDB_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff")
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )
process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32( 1000 )


systlabels = [""]
phosystlabels = []
metsystlabels = []
jetsystlabels = []
elesystlabels = []
musystlabels = []

from flashgg.MetaData.JobConfig import customize
customize.options.register('tthTagsOnly',
                           False,
                           VarParsing.VarParsing.multiplicity.singleton,
                           VarParsing.VarParsing.varType.bool,
                           'tthTagsOnly'
                           )
customize.options.register('doubleHTagsOnly',
                           False,
                           VarParsing.VarParsing.multiplicity.singleton,
                           VarParsing.VarParsing.varType.bool,
                           'doubleHTagsOnly'
                           )
customize.options.register('doubleHReweight',
                           -1,
                           VarParsing.VarParsing.multiplicity.singleton,
                           VarParsing.VarParsing.varType.int,
                           'doubleHReweight'
                           )
customize.options.register('doDoubleHTag',
                           False,
                           VarParsing.VarParsing.multiplicity.singleton,
                           VarParsing.VarParsing.varType.bool,
                           'doDoubleHTag'
                           )
customize.options.register('doDoubleHttHKiller',
                           False,
                           VarParsing.VarParsing.multiplicity.singleton,
                           VarParsing.VarParsing.varType.bool,
                           'doDoubleHttHKiller'
                           )
customize.options.register('ttHKillerSaveInputVariables',
                           False,
                           VarParsing.VarParsing.multiplicity.singleton,
                           VarParsing.VarParsing.varType.bool,
                           'ttHKillerSaveInputVariables'
                           )
customize.options.register('doDoubleHGenAnalysis',
                           False,
                           VarParsing.VarParsing.multiplicity.singleton,
                           VarParsing.VarParsing.varType.bool,
                           'doDoubleHGenAnalysis'
                           )
customize.options.register('doBJetRegression',
                           False,
                           VarParsing.VarParsing.multiplicity.singleton,
                           VarParsing.VarParsing.varType.bool,
                           'doBJetRegression'
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
                           True,
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
                           False,
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
customize.options.register('verboseSystDump',
                           False,
                           VarParsing.VarParsing.multiplicity.singleton,
                           VarParsing.VarParsing.varType.bool,
                           'verboseSystDump'
                           )
customize.options.register('doL1Prefiring',
                           False,
                           VarParsing.VarParsing.multiplicity.singleton,
                           VarParsing.VarParsing.varType.bool,
                           'doL1Prefiring'
                           )


print "Printing defaults"
print 'doFiducial '+str(customize.doFiducial)
print 'acceptance '+str(customize.acceptance)
print 'tthTagsOnly '+str(customize.tthTagsOnly)
# import flashgg customization to check if we have signal or background
from flashgg.MetaData.JobConfig import customize
# set default options if needed
customize.setDefault("maxEvents",-1)
customize.setDefault("targetLumi",1.00e+3)
customize.parse()
customize.metaConditions = MetaConditionsReader(customize.metaConditions)

### Global Tag
from Configuration.AlCa.GlobalTag import GlobalTag
if customize.processId == "Data":
    process.GlobalTag.globaltag = str(customize.metaConditions['globalTags']['data'])
else:
    process.GlobalTag.globaltag = str(customize.metaConditions['globalTags']['MC'])

from flashgg.Systematics.SystematicsCustomize import *
jetSystematicsInputTags = createStandardSystematicsProducers(process , customize)
if dropVBFInNonGold:
    process.flashggVBFTag.SetArbitraryNonGoldMC = True
    process.flashggVBFTag.DropNonGoldData = True
modifyTagSequenceForSystematics(process,jetSystematicsInputTags)

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


# process.load("flashgg/Taggers/flashggTagSequence_cfi")
# process.flashggTagSequence = flashggPrepareTagSequence(customize.metaConditions)

# needed for 0th vertex from microAOD
if customize.tthTagsOnly:
    process.load("flashgg/MicroAOD/flashggDiPhotons_cfi")
    process.flashggDiPhotons.whichVertex = cms.uint32(0)
    process.flashggDiPhotons.useZerothVertexFromMicro = cms.bool(True)

print 'here we print the tag sequence before'
print process.flashggTagSequence
if customize.doFiducial:
    from PhysicsTools.PatAlgos.tools.helpers import cloneProcessingSnippet,massSearchReplaceAnyInputTag
    process.flashggTagSequence.remove(process.flashggVBFTag)
    process.flashggTagSequence.remove(process.flashggTTHDiLeptonTag)
    process.flashggTagSequence.remove(process.flashggTTHLeptonicTag)
    process.flashggTagSequence.remove(process.flashggTTHHadronicTag)
   # process.flashggTagSequence.remove(process.flashggTTHeptonTag)
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

if customize.tthTagsOnly:
    process.flashggTagSequence.remove(process.flashggVBFTag)
    process.flashggTagSequence.remove(process.flashggVHMetTag)
    process.flashggTagSequence.remove(process.flashggWHLeptonicTag)
    process.flashggTagSequence.remove(process.flashggZHLeptonicTag)
    process.flashggTagSequence.remove(process.flashggVHLeptonicLooseTag)
    process.flashggTagSequence.remove(process.flashggVHHadronicTag)
    process.flashggTagSequence.remove(process.flashggUntagged)
    process.flashggTagSequence.remove(process.flashggVBFMVA)
    process.flashggTagSequence.remove(process.flashggVBFDiPhoDiJetMVA)
    process.flashggTagSequence.remove(process.flashggTTHHadronicTag) # Remove ttH tags as well, as we add these in later manually (with modified systematics worfklow)
    process.flashggTagSequence.remove(process.flashggTTHLeptonicTag)
    process.flashggTagSequence.remove(process.flashggTTHDiLeptonTag)
    process.flashggTagSequence.remove(process.flashggTHQLeptonicTag)

else:
    if not customize.doSystematics: # allow memory-intensive ttH MVAs if we are not running systematics
        allowLargettHMVAs(process)

if customize.doDoubleHTag:
    import flashgg.Systematics.doubleHCustomize 
    hhc = flashgg.Systematics.doubleHCustomize.DoubleHCustomize(process, customize, customize.metaConditions)
    minimalVariables += hhc.variablesToDump()
    systematicVariables = hhc.systematicVariables()

process.flashggTHQLeptonicTag.processId = cms.string(str(customize.processId))

print 'here we print the tag sequence after'
print process.flashggTagSequence

if customize.doFiducial:
    print 'we do fiducial and we change tagsorter'
    process.flashggTagSorter.TagPriorityRanges = cms.VPSet(     cms.PSet(TagName = cms.InputTag('flashggSigmaMoMpToMTag')) )

if customize.tthTagsOnly:
    process.flashggTagSorter.TagPriorityRanges = cms.VPSet(   
        cms.PSet(TagName = cms.InputTag('flashggTTHLeptonicTag')),
        cms.PSet(TagName = cms.InputTag('flashggTTHHadronicTag')) 
    )

    print "customize.processId:",customize.processId

    print "Removing FracRVNvtxWeight from syst and adding  PixelSeed"
    
    newvpset = cms.VPSet()
    for pset in process.flashggDiPhotonSystematics.SystMethods:
        if not pset.Label.value().count("FracRVNvtxWeight") :
            print  pset.Label.value()
            newvpset += [pset]
    #from flashgg.Systematics.flashggDiPhotonSystematics_cfi import PixelSeedWeight #FIXME: this does not currently work, so comment it out for now
    #newvpset += [ PixelSeedWeight ]
    
    process.flashggDiPhotonSystematics.SystMethods = newvpset
   

print "customize.processId:",customize.processId
# load appropriate scale and smearing bins here
# systematics customization scripts will take care of adjusting flashggDiPhotonSystematics
#process.load("flashgg.Systematics.escales.escale76X_16DecRereco_2015")

# Or use the official tool instead
useEGMTools(process)

# Only run systematics for signal events
# convention: ggh vbf wzh (wh zh) tth
signal_processes = ["ggh_","vbf_","wzh_","wh_","zh_","bbh_","thq_","thw_","tth_","HHTo2B2G","GluGluHToGG","VBFHToGG","VHToGG","ttHToGG","Acceptance"]
is_signal = reduce(lambda y,z: y or z, map(lambda x: customize.processId.count(x), signal_processes))
#if customize.processId.count("h_") or customize.processId.count("vbf_") or customize.processId.count("Acceptance") or customize.processId.count("hh_"): 
if is_signal:
    print "Signal MC, so adding systematics and dZ"
    if customize.doHTXS:
        variablesToUse = minimalVariablesHTXS
    else:
        variablesToUse = minimalVariables
    if customize.doFiducial:
        variablesToUse.extend(fc.getGenVariables(True))
        variablesToUse.extend(fc.getRecoVariables(True))
        variablesToUse.append("genLeadGenIso := ? diPhoton().leadingPhoton().hasMatchedGenPhoton() ? diPhoton().leadingPhoton().userFloat(\"genIso\") : -99")
        variablesToUse.append("decorrSigmarv := diPhotonMVA().decorrSigmarv")
        variablesToUse.append("leadmva := diPhotonMVA().leadmva")
        variablesToUse.append("subleadmva := diPhotonMVA().subleadmva")

    if customize.doL1Prefiring:
        customizeForL1Prefiring(process, customize.metaConditions)
        variablesToUse.append("prefireProbability := weight(\"prefireProbability\")")
    else:
        process.flashggTagSequence.remove(process.flashggPrefireWeight)


    if customize.doSystematics:
        for direction in ["Up","Down"]:
            phosystlabels.append("MvaShift%s01sigma" % direction)
#            phosystlabels.append("MvaLinearSyst%s01sigma" % direction)
            phosystlabels.append("SigmaEOverEShift%s01sigma" % direction)
            phosystlabels.append("MaterialCentralBarrel%s01sigma" % direction)
            phosystlabels.append("MaterialOuterBarrel%s01sigma" % direction)
            phosystlabels.append("MaterialForward%s01sigma" % direction)
            phosystlabels.append("FNUFEB%s01sigma" % direction)
            phosystlabels.append("FNUFEE%s01sigma" % direction)
            phosystlabels.append("MCScaleGain6EB%s01sigma" % direction)
            phosystlabels.append("MCScaleGain1EB%s01sigma" % direction)
            jetsystlabels.append("JEC%s01sigma" % direction)
            jetsystlabels.append("JER%s01sigma" % direction)
            jetsystlabels.append("PUJIDShift%s01sigma" % direction)
            metsystlabels.append("metJecUncertainty%s01sigma" % direction)
            metsystlabels.append("metJerUncertainty%s01sigma" % direction)
            metsystlabels.append("metPhoUncertainty%s01sigma" % direction)
            metsystlabels.append("metUncUncertainty%s01sigma" % direction)
            variablesToUse.append("UnmatchedPUWeight%s01sigma[1,-999999.,999999.] := weight(\"UnmatchedPUWeight%s01sigma\")" % (direction,direction))
            variablesToUse.append("MvaLinearSyst%s01sigma[1,-999999.,999999.] := weight(\"MvaLinearSyst%s01sigma\")" % (direction,direction))
            variablesToUse.append("LooseMvaSF%s01sigma[1,-999999.,999999.] := weight(\"LooseMvaSF%s01sigma\")" % (direction,direction))
            variablesToUse.append("PreselSF%s01sigma[1,-999999.,999999.] := weight(\"PreselSF%s01sigma\")" % (direction,direction))
            variablesToUse.append("electronVetoSF%s01sigma[1,-999999.,999999.] := weight(\"electronVetoSF%s01sigma\")" % (direction,direction))
            variablesToUse.append("TriggerWeight%s01sigma[1,-999999.,999999.] := weight(\"TriggerWeight%s01sigma\")" % (direction,direction))
            variablesToUse.append("FracRVWeight%s01sigma[1,-999999.,999999.] := weight(\"FracRVWeight%s01sigma\")" % (direction,direction))
            variablesToUse.append("FracRVNvtxWeight%s01sigma[1,-999999.,999999.] := weight(\"FracRVNvtxWeight%s01sigma\")" % (direction,direction))
            variablesToUse.append("ElectronWeight%s01sigma[1,-999999.,999999.] := weight(\"ElectronWeight%s01sigma\")" % (direction,direction))
            if os.environ["CMSSW_VERSION"].count("CMSSW_8_0"):
                variablesToUse.append("MuonWeight%s01sigma[1,-999999.,999999.] := weight(\"MuonWeight%s01sigma\")" % (direction,direction))
                variablesToUse.append("MuonMiniIsoWeight%s01sigma[1,-999999.,999999.] := weight(\"MuonMiniIsoWeight%s01sigma\")" % (direction,direction))
            elif os.environ["CMSSW_VERSION"].count("CMSSW_9_4"):
                variablesToUse.append("MuonIDWeight%s01sigma[1,-999999.,999999.] := weight(\"Muon%sIDWeight%s01sigma\")" % (direction,MUON_ID,direction))
                variablesToUse.append("MuonIsoWeight%s01sigma[1,-999999.,999999.] := weight(\"Muon%sISOWeight%s01sigma\")" % (direction,MUON_ISO,direction))
            variablesToUse.append("JetBTagCutWeight%s01sigma[1,-999999.,999999.] := weight(\"JetBTagCutWeight%s01sigma\")" % (direction,direction))
            variablesToUse.append("JetBTagReshapeWeight%s01sigma[1,-999999.,999999.] := weight(\"JetBTagReshapeWeight%s01sigma\")" % (direction,direction))
            for r9 in ["HighR9","LowR9"]:
                for region in ["EB","EE"]:
                    phosystlabels.append("ShowerShape%s%s%s01sigma"%(r9,region,direction))
#                    phosystlabels.append("MCSmear%s%s%s01sigma" % (r9,region,direction))
                    phosystlabels.append("MCScale%s%s%s01sigma" % (r9,region,direction))
                    for var in ["Rho","Phi"]:
                        phosystlabels.append("MCSmear%s%s%s%s01sigma" % (r9,region,var,direction))
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

if customize.doubleHTagsOnly:
    variablesToUse = minimalVariables
    if customize.processId == "Data":
        variablesToUse = minimalNonSignalVariables

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

# Dump an object called NoTag for untagged events in order to track QCD weights
# Will be broken if it's done for non-central values, so turn this on only for the non-syst tag sorter
process.flashggTagSorter.CreateNoTag = True # MUST be after tag sequence cloning

###### Dumper section

from FWCore.ParameterSet.VarParsing import VarParsing
from flashgg.MetaData.samples_utils import SamplesManager

process.source = cms.Source ("PoolSource",
                             fileNames = cms.untracked.vstring(
                                 "/store/user/spigazzi/flashgg/Era2016_RR-07Aug17_v1/legacyRun2TestV1/DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/Era2016_RR-07Aug17_v1-legacyRun2TestV1-v0-RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3_ext2-v1/190228_142907/0000/myMicroAODOutputFile_610.root"
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
process.tagsDumper.dumpTrees = customize.dumpTrees
process.tagsDumper.dumpWorkspace = customize.dumpWorkspace
process.tagsDumper.dumpHistos = False
process.tagsDumper.quietRooFit = True
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

if customize.processId == "tHq":
    import flashgg.Taggers.THQLeptonicTagVariables as var
    variablesToUse = minimalVariables + var.vtx_variables + var.dipho_variables

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


if customize.doFiducial:
    tagList=[["SigmaMpTTag",3]]
elif customize.tthTagsOnly:
    tagList=[
        ["TTHHadronicTag",4],
        ["TTHLeptonicTag",4]
        ]
elif customize.doubleHTagsOnly:
    tagList = hhc.tagList
    print "taglist is:"
    print tagList
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
        ["TTHHadronicTag",4],
        ["TTHLeptonicTag",4],
        ["THQLeptonicTag",0],
        ["TTHDiLeptonTag",0]
        ]

definedSysts=set()
process.tagsDumper.NNLOPSWeightFile=cms.FileInPath("flashgg/Taggers/data/NNLOPS_reweight.root")
process.tagsDumper.reweighGGHforNNLOPS = cms.untracked.bool(bool(customize.processId.count("ggh")))
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
          if customize.doHTXS:
              currentVariables = systematicVariablesHTXS
          else:    
              currentVariables = systematicVariables
      if tagName == "NoTag":
          if customize.doHTXS:
              currentVariables = ["stage0cat[72,9.5,81.5] := tagTruth().HTXSstage0cat"]
          else:
              currentVariables = []
      isBinnedOnly = (systlabel !=  "")
      if ( customize.doPdfWeights or customize.doSystematics ) and ( (customize.datasetName() and customize.datasetName().count("HToGG")) or customize.processId.count("h_") or customize.processId.count("vbf_") ) and (systlabel ==  "") and not (customize.processId == "th_125" or customize.processId == "bbh_125"):
          print "Signal MC central value, so dumping PDF weights"
          dumpPdfWeights = True
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
from HLTrigger.HLTfilters.hltHighLevel_cfi import hltHighLevel
hlt_paths = []
for dset in customize.metaConditions["TriggerPaths"]:
    if dset in customize.datasetName():
        hlt_paths.extend(customize.metaConditions["TriggerPaths"][dset])
process.hltHighLevel= hltHighLevel.clone(HLTPaths = cms.vstring(hlt_paths))

process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )

process.dataRequirements = cms.Sequence()
if customize.processId == "Data":
        process.dataRequirements += process.hltHighLevel

# Split WH and ZH
process.genFilter = cms.Sequence()
if ((customize.processId.count("wh") or customize.processId.count("zh")) and not (customize.processId.count("powheg"))) and not customize.processId.count("wzh") :
    print "enabling vh filter!!!!!"
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

# Met Filters
process.load('flashgg/Systematics/flashggMetFilters_cfi')

if customize.processId == "Data":
    metFilterSelector = "data"
else:
    metFilterSelector = "mc"

process.flashggMetFilters.requiredFilterNames = cms.untracked.vstring([filter.encode("ascii") for filter in customize.metaConditions["flashggMetFilters"][metFilterSelector]])

if customize.tthTagsOnly:
    process.p = cms.Path(process.dataRequirements*
                         process.flashggMetFilters*
                         process.genFilter*
                         process.flashggDiPhotons* # needed for 0th vertex from microAOD
                         process.flashggDifferentialPhoIdInputsCorrection*
                         process.flashggDiPhotonSystematics*
                         process.flashggMetSystematics*
                         process.flashggMuonSystematics*process.flashggElectronSystematics*
                         (process.flashggUnpackedJets*process.jetSystematicsSequence)*
                         (process.flashggTagSequence*process.systematicsTagSequences)*
                         process.flashggSystTagMerger*
                         process.penultimateFilter*
                         process.finalFilter*
                         process.tagsDumper)
    # Now, we put the ttH tags back in the sequence with modified systematics workflow
    modifySystematicsWorkflowForttH(process, systlabels, phosystlabels, metsystlabels, jetsystlabels)

else :
    process.p = cms.Path(process.dataRequirements*
                         process.flashggMetFilters*
                         process.genFilter*
                         process.flashggDifferentialPhoIdInputsCorrection*
                         process.flashggDiPhotonSystematics*
                         process.flashggMetSystematics*
                         process.flashggMuonSystematics*process.flashggElectronSystematics*
                         (process.flashggUnpackedJets*process.jetSystematicsSequence)*
                         (process.flashggTagSequence*process.systematicsTagSequences)*
                         process.flashggSystTagMerger*
                         process.penultimateFilter*
                         process.finalFilter*
                         process.tagsDumper)

if customize.doBJetRegression:

    bregProducers = []
    doubleHTagProducers = []
    
    from flashgg.Taggers.flashggTags_cff import UnpackedJetCollectionVInputTag
    from flashgg.Taggers.flashggbRegressionProducer_cfi import flashggbRegressionProducer
    recoJetCollections = UnpackedJetCollectionVInputTag

    jetsysts = cms.vstring()
    jetnames = cms.vstring()
    for jetsyst in [systlabels[0]]+jetsystlabels:
        jetsysts.append(jetsyst)
    for icoll,coll in enumerate(recoJetCollections):
        jetnames.append(coll.moduleLabel)
    producer = flashggbRegressionProducer.clone(JetSuffixes = jetsysts)
    producer.JetNames = jetnames
    producer.bRegressionWeightfile = cms.untracked.string(str(os.environ["CMSSW_BASE"]+customize.metaConditions['bRegression']['weightFile']))
    producer.y_mean = customize.metaConditions['bRegression']['y_mean']
    producer.y_std = customize.metaConditions['bRegression']['y_std']
    producer.year = cms.untracked.string(str(customize.metaConditions['bRegression']['year']))

    setattr(process,"bRegProducer",producer)
    bregProducers.append(producer)
    process.bregProducers = cms.Sequence(reduce(lambda x,y: x+y, bregProducers))
    process.p.replace(process.jetSystematicsSequence,process.jetSystematicsSequence*process.flashggUnpackedJets+process.bregProducers)
    

if customize.doDoubleHTag:
    hhc.doubleHTagRunSequence(systlabels,jetsystlabels,phosystlabels)
  

if customize.doFiducial:
    if ( customize.doPdfWeights or customize.doSystematics ) and ( (customize.datasetName() and customize.datasetName().count("HToGG")) 
                                                                   or customize.processId.count("h_") or customize.processId.count("vbf_") ) and (systlabel ==  ""):
          print "Signal MC central value, so dumping PDF weights"
          dumpPdfWeights = True
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
process.flashggTagSorter.BlindedSelectionPrintout = True

### Rerun microAOD sequence on top of microAODs using the parent dataset
if customize.useParentDataset:
    runRivetSequence(process, customize.metaConditions)


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

if customize.verboseTagDump:
    # crashes right now, dunno why - 02 May 2018
    pass
#    process.load("flashgg/Taggers/flashggTagTester_cfi")
#    process.flashggTagTester.TagSorter = cms.InputTag("flashggSystTagMerger")
#    process.flashggTagTester.ExpectMultiples = cms.untracked.bool(True)
#    process.p += process.flashggTagTester

############################################
## Additional details on tag sorter steps ##
############################################

if customize.verboseTagDump:
    process.flashggTagSorter.Debug = True
    customize.maxEvents = 10
                           
if customize.verboseSystDump:
    turnOnAllSystematicsDebug(process)
    customize.maxEVents = 10

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
# call the customization
customize(process)
