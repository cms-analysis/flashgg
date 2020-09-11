
#!/usr/bin/env cmsRun

import FWCore.ParameterSet.Config as cms
import FWCore.Utilities.FileUtils as FileUtils
import FWCore.ParameterSet.VarParsing as VarParsing
from flashgg.Systematics.SystematicDumperDefaultVariables import minimalVariables, minimalHistograms, minimalNonSignalVariables, systematicVariables
from flashgg.Systematics.SystematicDumperDefaultVariables import minimalVariablesHTXS, systematicVariablesHTXS
import os
from flashgg.MetaData.MetaConditionsReader import *
import flashgg.Systematics.fiducialCrossSectionsCustomize as fc

# SYSTEMATICS SECTION
dropVBFInNonGold = False  # for 2015 only!

process = cms.Process("FLASHggSyst")


process.load("FWCore.MessageService.MessageLogger_cfi")
process.load("Configuration.StandardSequences.GeometryDB_cff")
# process.load("Configuration.StandardSequences.MagneticField_cff")
process.load(
    "Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff")

process.maxEvents = cms.untracked.PSet(input=cms.untracked.int32(-1))
process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32(1000)

systlabels = [""]
phosystlabels = []
metsystlabels = []
jetsystlabels = []
elesystlabels = []
musystlabels = []

from flashgg.MetaData.JobConfig import customize

customize.options.register('doMuFilter',
                           True,
                           VarParsing.VarParsing.multiplicity.singleton,
                           VarParsing.VarParsing.varType.bool,
                           'doMuFilter'
                           )
customize.options.register('doFiducial',
                           True,
                           VarParsing.VarParsing.multiplicity.singleton,
                           VarParsing.VarParsing.varType.bool,
                           'doFiducial'
                           )
customize.options.register('doBJetsAndMET',
                           False,
                           VarParsing.VarParsing.multiplicity.singleton,
                           VarParsing.VarParsing.varType.bool,
                           'doBJetsAndMET'
                           )
customize.options.register('doJets',
                           False,
                           VarParsing.VarParsing.multiplicity.singleton,
                           VarParsing.VarParsing.varType.bool,
                           'doJets'
                           )
customize.options.register('acceptance',
                           'NONE',
                           VarParsing.VarParsing.multiplicity.singleton,
                           VarParsing.VarParsing.varType.string,
                           'acceptance'
                           )
customize.options.register('filterNonAcceptedEvents',
                           True,
                           VarParsing.VarParsing.multiplicity.singleton,
                           VarParsing.VarParsing.varType.bool,
                           'filterNonAcceptedEvents'
                           )
customize.options.register('doSystematics',
                           False,
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
customize.options.register('doSigmaMdecorr',
                           True,
                           VarParsing.VarParsing.multiplicity.singleton,
                           VarParsing.VarParsing.varType.bool,
                           'doSigmaMdecorr'
                           )
customize.options.register('analysisType',
                           'mainAnalysis',
                           VarParsing.VarParsing.multiplicity.singleton,
                           VarParsing.VarParsing.varType.string,
                           'analysisType'
                           )
customize.options.register('ignoreNegR9',
                           False,
                           VarParsing.VarParsing.multiplicity.singleton,
                           VarParsing.VarParsing.varType.bool,
                           'ignoreNegR9'
                           )
customize.options.register('applyNNLOPSweight',
                           True,
                           VarParsing.VarParsing.multiplicity.singleton,
                           VarParsing.VarParsing.varType.bool,
                           'applyNNLOPSweight'
                           )

# import flashgg customization to check if we have signal or background
# from flashgg.MetaData.JobConfig import customize
# set default options if needed
customize.setDefault("maxEvents", -1)
customize.setDefault("targetLumi", 1.00e+3)


customize.parse()

# When running BJets and Leptons, use parent dataset to reproduce GenLeptonExtra and GenJetsExtra collections
if customize.doBJetsAndMET and not customize.processId == "Data":
    customize.options.useParentDataset = True

customize.metaConditions = MetaConditionsReader(customize.metaConditions)

# Global Tag
from Configuration.AlCa.GlobalTag import GlobalTag
if customize.processId == "Data":
    process.GlobalTag.globaltag = str(
        customize.metaConditions['globalTags']['data'])
else:
    process.GlobalTag.globaltag = str(
        customize.metaConditions['globalTags']['MC'])

from flashgg.Systematics.SystematicsCustomize import *
process.flashggTagSequence = fc.setupTagSequenceForFiducial(process, customize)
jetSystematicsInputTags = createStandardSystematicsProducers(
    process, customize)
modifyTagSequenceForSystematics(process, jetSystematicsInputTags)

print "Printing options"
print 'doFiducial '+str(customize.doFiducial)
print 'doBJetsAndMET '+str(customize.doBJetsAndMET)
print 'doJets '+str(customize.doJets)
print 'acceptance '+str(customize.acceptance)
print 'doMuFilter '+str(customize.doMuFilter)

fc.leadCut = 1./3.
fc.subLeadCut = 1./4.
fc.isoCut = 10.
fc.etaCut = 2.5
fc.doBJetsAndMET = customize.doBJetsAndMET
fc.doJets = customize.doJets
matchCut = "leadingPhoton.hasMatchedGenPhoton() && subLeadingPhoton.hasMatchedGenPhoton()"
    #    phoIDcut = '(leadingView().phoIdMvaWrtChosenVtx() >0.320 && subLeadingView().phoIdMvaWrtChosenVtx() >0.320)'#remove it for further studies
phoIDcut = '(leadingView().phoIdMvaWrtChosenVtx() >-1. && subLeadingView().phoIdMvaWrtChosenVtx() >-1.)'
accCut = fc.getAccRecoCut()
r9Cut = '(leadingPhoton.full5x5_r9>0. && subLeadingPhoton.full5x5_r9>0.)'

if customize.acceptance == 'IN':
    process.flashggPreselectedDiPhotons.cut = cms.string(str(process.flashggPreselectedDiPhotons.cut.value()) + ' && ' + str(matchCut) + ' && ' + str(phoIDcut) + ' && ' + str(accCut) + ' && ' + str(r9Cut))

if customize.acceptance == 'OUT':
    process.flashggPreselectedDiPhotons.cut = cms.string(str(process.flashggPreselectedDiPhotons.cut.value()) + ' && ' + str(matchCut) + ' && ' + str(phoIDcut) + ' && !' + str(accCut) + ' && ' + str(r9Cut))

if customize.acceptance == 'NONE':
    process.flashggPreselectedDiPhotons.cut = cms.string(
        str(process.flashggPreselectedDiPhotons.cut.value()) + ' && ' + str(phoIDcut) + ' && ' + str(r9Cut))

if customize.acceptance == 'BKG':
    process.flashggPreselectedDiPhotons.cut = cms.string(
        str(process.flashggPreselectedDiPhotons.cut.value()) + ' && ' + str(phoIDcut) + ' && ' + str(matchCut) + ' && ' + str(r9Cut))
    # process.load("flashgg/MicroAOD/flashggDiPhotons_cfi")
    # for opt, value in customize.metaConditions["flashggDiPhotons"].items():
    #     if isinstance(value, unicode):
    #         setattr(process.flashggDiPhotons, opt, str(value))
    #     else:
    #         setattr(process.flashggDiPhotons, opt, value)

print "Here we print the preslection cut"
print process.flashggPreselectedDiPhotons.cut
    
process.flashggDiPhotonMVA.sigmaMdecorrFile = cms.FileInPath(str(customize.metaConditions["sigmaM_M_decorr"]))

if customize.doSigmaMdecorr:
    process.flashggDiPhotonMVA.doSigmaMdecorr = cms.bool(True)
else:
    process.flashggDiPhotonMVA.doSigmaMdecorr = cms.bool(False)
    
process.flashggSigmaMoMpToMTag.BoundariesSigmaMoM = cms.vdouble(
    0., 100.)  # Categorization done outside of fgg


print 'we do fiducial and we change tagsorter'
process.flashggTagSorter.TagPriorityRanges = cms.VPSet(
    cms.PSet(TagName=cms.InputTag('flashggSigmaMoMpToMTag')))

useEGMTools(process)

signal_processes = ["ggh_", "vbf_", "wzh_", "wh_", "zh_", "bbh_", "thq_", "thw_",
                    "tth_", "HHTo2B2G", "GluGluHToGG", "VBFHToGG", "VHToGG", "ttHToGG", "Acceptance"]

is_signal = reduce(lambda y, z: y or z, map(
    lambda x: customize.processId.count(x), signal_processes))

applyL1Prefiring = customizeForL1Prefiring(process, customize.metaConditions, customize.processId)

if is_signal:
    print "Signal MC, so adding systematics and dZ"
    variablesToUse = minimalVariables
    # if customize.doFiducial:
    print("Adding variables")
    variablesToUse.extend(fc.getGenVariables(True))
    variablesToUse.extend(fc.getRecoVariables(True))
    # variablesToUse.append("genLeadGenIso := ? diPhoton().leadingPhoton().hasMatchedGenPhoton() ? diPhoton().leadingPhoton().userFloat(\"genIso\") : -99")
    # variablesToUse.append("decorrSigmarv := diPhotonMVA().decorrSigmarv")
    variablesToUse.append("leadmva := diPhotonMVA().leadmva")
    variablesToUse.append("subleadmva := diPhotonMVA().subleadmva")
    variablesToUse.append("sigmarv := diPhotonMVA().sigmarv")
    variablesToUse.append("sigmawv := diPhotonMVA().sigmawv")
    variablesToUse.append("vtxprob := diPhotonMVA().vtxprob")
    variablesToUse.append("CosPhi := diPhotonMVA().CosPhi")
#        variablesToUse.append("subleadmva := diPhotonMVA().subleadmva")

    if customize.doSystematics:
        systematicVariables.extend(fc.getGenVariables(True))
        systematicVariables.extend(fc.getRecoVariables(True))
#        systematicVariables.append("genLeadGenIso[1,0.0,100.0] := ? diPhoton().leadingPhoton().hasMatchedGenPhoton() ? diPhoton().leadingPhoton().userFloat(\"genIso\") : -99")
#        systematicVariables.append("decorrSigmarv[1,0.0,0.10] := diPhotonMVA().decorrSigmarv")
        systematicVariables.append(
            "leadmva[200,-1.0,1.0] := diPhotonMVA().leadmva")
        systematicVariables.append(
            "subleadmva[200,-1.0,1.0] := diPhotonMVA().subleadmva")

    if customize.doSystematics:
        for direction in ["Up", "Down"]:
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
            if customize.doBJetsAndMET:
                metsystlabels.append("metJecUncertainty%s01sigma" % direction)
                metsystlabels.append("metJerUncertainty%s01sigma" % direction)
                metsystlabels.append("metPhoUncertainty%s01sigma" % direction)
                metsystlabels.append("metUncUncertainty%s01sigma" % direction)
            variablesToUse.append(
                "UnmatchedPUWeight%s01sigma[1,-999999.,999999.] := weight(\"UnmatchedPUWeight%s01sigma\")" % (direction, direction))
            variablesToUse.append(
                "MvaLinearSyst%s01sigma[1,-999999.,999999.] := weight(\"MvaLinearSyst%s01sigma\")" % (direction, direction))
            variablesToUse.append(
                "LooseMvaSF%s01sigma[1,-999999.,999999.] := weight(\"LooseMvaSF%s01sigma\")" % (direction, direction))
            variablesToUse.append(
                "PreselSF%s01sigma[1,-999999.,999999.] := weight(\"PreselSF%s01sigma\")" % (direction, direction))
            variablesToUse.append(
                "electronVetoSF%s01sigma[1,-999999.,999999.] := weight(\"electronVetoSF%s01sigma\")" % (direction, direction))
            variablesToUse.append(
                "TriggerWeight%s01sigma[1,-999999.,999999.] := weight(\"TriggerWeight%s01sigma\")" % (direction, direction))
            variablesToUse.append(
                "FracRVWeight%s01sigma[1,-999999.,999999.] := weight(\"FracRVWeight%s01sigma\")" % (direction, direction))
            variablesToUse.append(
                "FracRVNvtxWeight%s01sigma[1,-999999.,999999.] := weight(\"FracRVNvtxWeight%s01sigma\")" % (direction, direction))
            variablesToUse.append(
                "ElectronWeight%s01sigma[1,-999999.,999999.] := weight(\"ElectronWeight%s01sigma\")" % (direction, direction))
            if os.environ["CMSSW_VERSION"].count("CMSSW_8_0"):
                variablesToUse.append(
                    "MuonWeight%s01sigma[1,-999999.,999999.] := weight(\"MuonWeight%s01sigma\")" % (direction, direction))
                variablesToUse.append(
                    "MuonMiniIsoWeight%s01sigma[1,-999999.,999999.] := weight(\"MuonMiniIsoWeight%s01sigma\")" % (direction, direction))
            elif os.environ["CMSSW_VERSION"].count("CMSSW_9_4"):
                variablesToUse.append(
                    "MuonIDWeight%s01sigma[1,-999999.,999999.] := weight(\"Muon%sIDWeight%s01sigma\")" % (direction, MUON_ID, direction))
                variablesToUse.append(
                    "MuonIsoWeight%s01sigma[1,-999999.,999999.] := weight(\"Muon%sISOWeight%s01sigma\")" % (direction, MUON_ISO, direction))
            variablesToUse.append(
                "JetBTagCutWeight%s01sigma[1,-999999.,999999.] := weight(\"JetBTagCutWeight%s01sigma\")" % (direction, direction))
            variablesToUse.append(
                "JetBTagReshapeWeight%s01sigma[1,-999999.,999999.] := weight(\"JetBTagReshapeWeight%s01sigma\")" % (direction, direction))
            for r9 in ["HighR9", "LowR9"]:
                for region in ["EB", "EE"]:
                    phosystlabels.append(
                        "ShowerShape%s%s%s01sigma" % (r9, region, direction))
#                    phosystlabels.append("MCSmear%s%s%s01sigma" % (r9,region,direction))
                    phosystlabels.append("MCScale%s%s%s01sigma" %
                                         (r9, region, direction))
                    for var in ["Rho", "Phi"]:
                        phosystlabels.append("MCSmear%s%s%s%s01sigma" % (
                            r9, region, var, direction))
        systlabels += phosystlabels
        systlabels += jetsystlabels
        systlabels += metsystlabels
    customizeSystematicsForSignal(process)
elif customize.processId == "Data":
    print "Data, so turn off all shifts and systematics, with some exceptions"
    variablesToUse = minimalNonSignalVariables
    variablesToUse.extend(fc.getRecoVariables(True))
    variablesToUse.append("leadmva := diPhotonMVA().leadmva")
    variablesToUse.append("subleadmva := diPhotonMVA().subleadmva")
    customizeSystematicsForData(process)
else:
    print "Background MC, so store mgg and central only"
    variablesToUse = minimalNonSignalVariables
    variablesToUse.extend(fc.getRecoVariables(True))
    variablesToUse.append("sigmarv := diPhotonMVA().sigmarv")
    variablesToUse.append("sigmawv := diPhotonMVA().sigmawv")
    variablesToUse.append("leadmva := diPhotonMVA().leadmva")
    variablesToUse.append("subleadmva := diPhotonMVA().subleadmva")
    customizeSystematicsForBackground(process)

print('------------------------------------variablesToUse--------------------')
print(variablesToUse)
tagList = [["SigmaMpTTag", 1]]


fc.bookCompositeObjects(process, customize.processId,
                        process.flashggTagSequence)
cloneTagSequenceForEachSystematic(
    process, systlabels, phosystlabels, metsystlabels, jetsystlabels, jetSystematicsInputTags)

# MUST be after tag sequence cloning
process.flashggTagSorter.CreateNoTag = False
process.flashggTagSorter.isGluonFusion = cms.bool(bool(customize.processId.count("ggh")))
process.flashggTagSorter.applyNNLOPSweight = cms.bool(customize.applyNNLOPSweight)

from FWCore.ParameterSet.VarParsing import VarParsing
from flashgg.MetaData.samples_utils import SamplesManager

process.source = cms.Source("PoolSource",
                            fileNames=cms.untracked.vstring(
                                "/store/user/spigazzi/flashgg/Era2016_RR-07Aug17_v1/legacyRun2TestV1/DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/Era2016_RR-07Aug17_v1-legacyRun2TestV1-v0-RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3_ext2-v1/190228_142907/0000/myMicroAODOutputFile_610.root"
                            ))

process.TFileService = cms.Service("TFileService",
                                   fileName=cms.string("test.root"))

# import diphotonTagDumper
process.load("flashgg.Taggers.diphotonTagDumper_cfi")
import flashgg.Taggers.dumperConfigTools as cfgTools


process.tagsDumper.className = "DiPhotonTagDumper"
process.tagsDumper.src = "flashggSystTagMerger"
# process.tagsDumper.src = "flashggTagSystematics"
process.tagsDumper.processId = "test"
process.tagsDumper.dumpTrees = customize.dumpTrees
process.tagsDumper.dumpWorkspace = customize.dumpWorkspace
process.tagsDumper.dumpHistos = False
process.tagsDumper.quietRooFit = True
process.tagsDumper.nameTemplate = cms.untracked.string(
    "$PROCESS_$SQRTS_$CLASSNAME_$SUBCAT_$LABEL")

if customize.options.WeightName:
    lheProduct = customize.dataset[1]["LHESourceName"].split("_")
    # print lheProduct
    process.tagsDumper.LHEEventProduct = cms.untracked.InputTag(
        str(lheProduct[1]), str(lheProduct[2]), str(lheProduct[3]))
    # print process.tagsDumper.LHEEventProduct
    process.tagsDumper.LHEWeightName = cms.untracked.string(
        customize.options.WeightName)

definedSysts = set()
process.tagsDumper.classifierCfg.remap = cms.untracked.VPSet()
# process.tagsDumper.NNLOPSWeightFile = cms.FileInPath("flashgg/Taggers/data/NNLOPS_reweight.root")

# process.tagsDumper.reweighGGHforNNLOPS = cms.untracked.bool(
#     bool(customize.processId.count("ggh")))
# # if ( customize.datasetName() and customize.datasetName().count("GluGlu") and customize.datasetName().count("amcatnlo")):
# if (customize.datasetName() and customize.datasetName().count("GluGlu")):
#     print "Gluon fusion amcatnlo: read NNLOPS reweighting file"
#     process.tagsDumper.dumpNNLOPSweight = cms.untracked.bool(True)
#     process.tagsDumper.NNLOPSWeight = cms.FileInPath(
#         "flashgg/Taggers/data/NNLOPS_reweight.root")
#     if customize.datasetName().count("amcatnlo"):
#         process.tagsDumper.generatorToBeReweightedToNNLOPS = cms.string(
#             "mcatnlo")
#     elif customize.datasetName().count("powheg"):
#         process.tagsDumper.generatorToBeReweightedToNNLOPS = cms.string(
#             "powheg")

# elif (customize.datasetName() and (customize.datasetName().count("HToGG") or customize.processId.count("h_") or customize.processId.count("vbf_") or customize.processId.count("Acceptance"))):
#     print "Other signal: dump NNLOPS weights, but set them to 1"
#     process.tagsDumper.dumpNNLOPSweight = cms.untracked.bool(True)
#     process.tagsDumper.NNLOPSWeight = cms.double(1.0)
# else:
#     print "Data or background: no NNLOPS weights"
#     process.tagsDumper.dumpNNLOPSweight = cms.untracked.bool(False)

for tag in tagList:
    tagName = tag[0]
    tagCats = tag[1]
    # remap return value of class-based classifier
    process.tagsDumper.classifierCfg.remap.append(cms.untracked.PSet(
        src=cms.untracked.string("flashgg%s" % tagName), dst=cms.untracked.string(tagName)))
    for systlabel in systlabels:
        if systlabel not in definedSysts:
            # the cut corresponding to the systematics can be defined just once
            cutstring = "hasSyst(\"%s\") " % (systlabel)
            definedSysts.add(systlabel)
        else:
            cutstring = None
        if systlabel == "":
            currentVariables = variablesToUse
        else:
            currentVariables = systematicVariables
        if tagName == "NoTag":
            currentVariables = []
        isBinnedOnly = (systlabel != "")
        if (customize.doPdfWeights or customize.doSystematics) and ((customize.datasetName() and customize.datasetName().count("HToGG")) or customize.processId.count("h_") or customize.processId.count("vbf_") or customize.processId.count("Acceptance")) and (systlabel == "") and not (customize.processId == "th_125" or customize.processId == "bbh_125" or customize.processId == "thw_125") and not (customize.datasetName() and customize.datasetName().count("DiPho")) and not (customize.datasetName() and customize.datasetName().count("GJet")):
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
                             splitPdfByStage0Bin=False,
                             unbinnedSystematics=True
                             )

filterHLTrigger(process, customize)

process.options = cms.untracked.PSet(wantSummary=cms.untracked.bool(True))

process.dataRequirements = cms.Sequence()
if customize.processId == "Data":
    process.dataRequirements += process.hltHighLevel

# Split WH and ZH
process.genFilter = cms.Sequence()
if ((customize.processId.count("wh") or customize.processId.count("zh")) and not (customize.processId.count("powheg"))) and not customize.processId.count("wzh"):
    print "enabling vh filter!!!!!"
    process.load("flashgg/Systematics/VHFilter_cfi")
    process.genFilter += process.VHFilter
    process.VHFilter.chooseW = bool(customize.processId.count("wh"))
    process.VHFilter.chooseZ = bool(customize.processId.count("zh"))

if (customize.processId == "th_125" or customize.processId == "bbh_125" or customize.processId == "thw_125"):
    process.load("flashgg/Systematics/CentralHiggsFilter_cfi")
    process.genFilter += process.CentralHiggsFilter

# pythia8 has an unanticipated EM showering feature, check have two photons from hard scatter
process.penultimateFilter = cms.Sequence()
if customize.processId == "th_125":  # for this sample the filter removes also H -> ZG
    process.load("flashgg/Systematics/HardProcessFinalStateFilter_cfi")
#    process.HardProcessFinalStateFilter.debug = True
    process.penultimateFilter += process.HardProcessFinalStateFilter

# Split out prompt-fake or fake-fake
process.finalFilter = cms.Sequence()
print "count 3"
if (customize.processId.count("qcd") or customize.processId.count("gjet")) and customize.processId.count("fake"):
    process.load("flashgg/Systematics/PromptFakeFilter_cfi")
    process.finalFilter += process.PromptFakeFilter
    if (customize.processId.count("promptfake")):
        process.PromptFakeFilter.doPromptFake = cms.bool(True)
        process.PromptFakeFilter.doFakeFake = cms.bool(False)
    elif (customize.processId.count("fakefake")):
        process.PromptFakeFilter.doPromptFake = cms.bool(False)
        process.PromptFakeFilter.doFakeFake = cms.bool(True)
    else:
        raise Exception, "Mis-configuration of python for prompt-fake filter"

# Met Filters
process.load('flashgg/Systematics/flashggMetFilters_cfi')

if customize.processId == "Data":
    metFilterSelector = "data"
    filtersInputTag = cms.InputTag("TriggerResults", "", "RECO")
else:
    metFilterSelector = "mc"
    filtersInputTag = cms.InputTag("TriggerResults", "", "PAT")

process.flashggMetFilters.requiredFilterNames = cms.untracked.vstring([filter.encode(
    "ascii") for filter in customize.metaConditions["flashggMetFilters"][metFilterSelector]])
process.flashggMetFilters.filtersInputTag = filtersInputTag

print('-----------------------------------DiPho-----------------------------------------')
print(customize.processId.count("DiPho"))
print(customize.datasetName().count("DiPho"))


if (customize.doPdfWeights or customize.doSystematics) and ((customize.datasetName() and customize.datasetName().count("HToGG")) or customize.processId.count("h_") or customize.processId.count("vbf_") or customize.processId.count("Acceptance")) and not ((customize.datasetName() and customize.datasetName().count("DiPho"))) and not (customize.datasetName() and customize.datasetName().count("GJet")):
        # if ( customize.doPdfWeights or customize.doSystematics ) and ( (customize.datasetName() and customize.datasetName().count("HToGG"))  or customize.processId.count("h_") or customize.processId.count("vbf_") or customize.processId.count("Acceptance")) and (systlabel ==  ""):
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
    
if not customize.processId == "Data" and not ((customize.datasetName() and customize.datasetName().count("DiPho"))) and not (customize.datasetName() and customize.datasetName().count("GJet")):
    mH = None
    ldset = ""
    if customize.datasetName():
        ldset = customize.datasetName().lower()
    if "htogg" in ldset or "tthjettogg" in ldset:
        try:
            mH = float(customize.datasetName().split("_M")[1].split("_")[0])
        except Exception, e:
            print(e, customize.datasetName())
            pass
    # NNLOPSreweight = False
    # genToReweight = None
    # if (customize.datasetName() and customize.datasetName().count("GluGlu")):
    #     print "datasetName contains GluGlu --> NNLOPSrewwight is True"
    #     NNLOPSreweight = True
    #     if customize.datasetName().count("amcatnlo"):
    #             #                print "datasetName contains amcatnlo --> gen to be reweighted is amcatnlo"
    #         genToReweight = "amcatnlo"
    #     if customize.datasetName().count("powheg"):
    #             #                print "datasetName contains powheg --> gen to be reweighted is powheg"
    #         genToReweight = "powheg"
    print 'pdfWeights in worspaceStd'
    fc.addGenOnlyAnalysis(process, customize.processId, process.flashggTagSequence,
                          customize.acceptance, tagList, systlabels,
                          pdfWeights=(dumpPdfWeights, nPdfWeights,nAlphaSWeights, nScaleWeights),
                          mH=mH, filterEvents=customize.filterNonAcceptedEvents)
    pdfWeights = (dumpPdfWeights, nPdfWeights, nAlphaSWeights, nScaleWeights),
    print pdfWeights

if not customize.processId == "Data":
    print("Not data!")
    process.p = cms.Path(process.dataRequirements *
                         process.flashggMetFilters *
                         process.genFilter *
                         # process.flashggDiPhotons*
                         # needed for 0th vertex from microAOD
                         process.flashggDifferentialPhoIdInputsCorrection *
                         process.flashggDiPhotonSystematics *
                         process.flashggMetSystematics*
                         process.flashggMuonSystematics*process.flashggElectronSystematics*
                         (process.flashggUnpackedJets*process.jetSystematicsSequence)*
                         (process.flashggTagSequence*process.systematicsTagSequences)*
                         # process.genSequence*
                         process.flashggSystTagMerger*
                         process.penultimateFilter*
                         process.finalFilter*
                         process.tagsDumper)
else:
    process.p = cms.Path(process.dataRequirements *
                         process.flashggMetFilters *
                         process.genFilter *
                         process.flashggDifferentialPhoIdInputsCorrection *
                         process.flashggDiPhotonSystematics *
                         process.flashggMetSystematics *
                         process.flashggMuonSystematics*process.flashggElectronSystematics *
                         (process.flashggUnpackedJets*process.jetSystematicsSequence) *
                         (process.flashggTagSequence*process.systematicsTagSequences) *
                         process.flashggSystTagMerger *
                         process.penultimateFilter *
                         process.finalFilter *
                         process.tagsDumper)
print("-----------------------------------------------PATH-p----------------------------------")
print(process.p)
fc.addObservables(process, process.tagsDumper, customize.processId, process.flashggTagSequence)
print("-----------------------------------------------PATH-pfid-----------------------------------")
print(process.pfid)


if customize.recalculatePDFWeights and is_signal and not customize.processId.count("bbh"):
    customize.options.useParentDataset = True
    recalculatePDFWeights(process, customize.metaConditions)

if(not hasattr(process, "options")):
    process.options = cms.untracked.PSet()
process.options.allowUnscheduled = cms.untracked.bool(True)

print "--- Dumping modules that take diphotons as input: ---"
mns = process.p.moduleNames()
for mn in mns:
    module = getattr(process, mn)
    if hasattr(module, "src") and type(module.src) == type(cms.InputTag("")) and module.src.value().count("DiPhoton"):
        print str(module), module.src
    elif hasattr(module, "DiPhotonTag"):
        print str(module), module.DiPhotonTag
print
printSystematicInfo(process)

# Detailed tag interpretation information printout (blinded)
process.flashggTagSorter.StoreOtherTagInfo = True
process.flashggTagSorter.BlindedSelectionPrintout = True

if customize.verboseTagDump:
    process.flashggTagSorter.Debug = True
    customize.maxEvents = 10

if customize.verboseSystDump:
    turnOnAllSystematicsDebug(process)
    customize.maxEvents = 10

print(customize.maxEvents)
customize(process)
