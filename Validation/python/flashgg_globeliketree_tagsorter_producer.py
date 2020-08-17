import FWCore.ParameterSet.Config as cms
import FWCore.Utilities.FileUtils as FileUtils
#from flashgg.Systematics.SystematicDumperDefaultVariables import minimalVariables,minimalNonSignalVariables,systematicVariables

process = cms.Process("FLASHggMicroAODAndTag")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.load("Configuration.StandardSequences.GeometryDB_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff")
process.RandomNumberGeneratorService = cms.Service("RandomNumberGeneratorService")
process.GlobalTag.globaltag = '74X_mcRun2_asymptotic_v4'

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(2) )
process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32( 1000 )

process.source = cms.Source("PoolSource",fileNames=cms.untracked.vstring(
#"/store/group/phys_higgs/cmshgg/sethzenz/flashgg/RunIISpring15-ReMiniAOD-BetaV7-25ns/Spring15BetaV7//DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/RunIISpring15-ReMiniAOD-BetaV7-25ns-Spring15BetaV7-v0-RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/151021_151505/0000/myMicroAODOutputFile_99.root", 
#"/store/group/phys_higgs/cmshgg/sethzenz/flashgg/RunIISpring15-ReMiniAOD-BetaV7-25ns/Spring15BetaV7/DoubleEG/RunIISpring15-ReMiniAOD-BetaV7-25ns-Spring15BetaV7-v0-Run2015D-05Oct2015-v1/151021_151712/0000/myMicroAODOutputFile_99.root"
"/store/group/phys_higgs/cmshgg/sethzenz/flashgg/RunIISpring15-ReMiniAOD-1_1_0-25ns/1_1_0/GluGluHToGG_M-125_13TeV_powheg_pythia8/RunIISpring15-ReMiniAOD-1_1_0-25ns-1_1_0-v0-RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/160105_223154/0000/myMicroAODOutputFile_1.root"
))

#from flashgg.MetaData.JobConfig import customize
from flashgg.MetaData.JobConfig import JobConfig

customize = JobConfig(crossSections=["$CMSSW_BASE/src/flashgg/MetaData/data/cross_sections.json"])
customize.setDefault("maxEvents", 10000)
customize.setDefault("targetLumi", 1)
customize.parse()

process.load('RecoMET.METFilters.eeBadScFilter_cfi')
process.eeBadScFilter.EERecHitSource = cms.InputTag("reducedEgamma","reducedEERecHits") # Saved MicroAOD Collection (data only)
process.dataRequirements = cms.Sequence()
if customize.processId == "Data":
        process.dataRequirements += process.hltHighLevel
        process.dataRequirements += process.eeBadScFilter

process.load("flashgg/MicroAOD/flashggMicroAODSequence_cff")
process.load("JetMETCorrections.Configuration.JetCorrectionServices_cff")
process.load("flashgg/Taggers/flashggTagSequence_cfi")

process.flashggTagSequence.remove(process.flashggPreselectedDiPhotons)
process.flashggTagSorter.massCutUpper=cms.untracked.double(100000.)
process.flashggTagSorter.massCutLower=cms.untracked.double(100.)
process.flashggUntagged.Boundaries=cms.untracked.vdouble(-99999.,0.31,0.62,0.86,0.98)

process.load("flashgg/Taggers/flashggTagTester_cfi")

from flashgg.MicroAOD.flashggJets_cfi import flashggBTag, maxJetCollections

flashggUnpackedJets = cms.EDProducer("FlashggVectorVectorJetUnpacker",
                                     JetsTag = cms.InputTag("flashggFinalJets"),
                                     NCollections = cms.uint32(maxJetCollections)
                                     )

UnpackedJetCollectionVInputTag = cms.VInputTag()
for i in range(0,maxJetCollections):
    UnpackedJetCollectionVInputTag.append(cms.InputTag('flashggUnpackedJets',str(i)))

############################
#       Systematics        #
############################
process.load("flashgg.Systematics.flashggDiPhotonSystematics_cfi")
from PhysicsTools.PatAlgos.tools.helpers import cloneProcessingSnippet,massSearchReplaceAnyInputTag
massSearchReplaceAnyInputTag(process.flashggTagSequence,cms.InputTag("flashggDiPhotons"),cms.InputTag("flashggDiPhotonSystematics"))
massSearchReplaceAnyInputTag(process.flashggTagSequence,cms.InputTag("flashggPreselectedDiPhotons"),cms.InputTag("flashggDiPhotonSystematics"))

process.flashggSystTagMerger = cms.EDProducer("TagMerger",src=cms.VInputTag("flashggTagSorter"))
process.systematicsTagSequences = cms.Sequence()
systlabels = [""]
phosystlabels = []

#if customize.processId.count("h_") or customize.processId.count("vbf_"): # convention: ggh vbf wzh tth    
if customize.processId == "signal":
    print "Signal MC, so adding systematics and dZ"
    #variablesToUse = minimalVariables
    for direction in ["Up","Down"]:
        #phosystlabels.append("MvaShift%s01sigma" % direction)
        #phosystlabels.append("SigmaEOverEShift%s01sigma" % direction)
        #jetsystlabels.append("JEC%s01sigma" % direction)
        #jetsystlabels.append("JER%s01sigma" % direction)
        #variablesToUse.append("LooseMvaSF%s01sigma[1,-999999.,999999.] := weight(\"LooseMvaSF%s01sigma\")" % (direction,direction))
        #variablesToUse.append("PreselSF%s01sigma[1,-999999.,999999.] := weight(\"PreselSF%s01sigma\")" % (direction,direction))
        #variablesToUse.append("FracRVWeight%s01sigma[1,-999999.,999999.] := weight(\"FracRVWeight%s01sigma\")" % (direction,direction))
        for r9 in ["HighR9","LowR9"]:
#            phosystlabels.append("MCSmear%sEE%s01sigma" % (r9,direction))
#            for var in ["Rho","Phi"]:
#                phosystlabels.append("MCSmear%sEB%s%s01sigma" % (r9,var,direction))
            for region in ["EB","EE"]:
                phosystlabels.append("MCSmear%s%s%s01sigma" % (r9,region,direction))
                phosystlabels.append("MCScale%s%s%s01sigma" % (r9,region,direction))
#                variablesToUse.append("LooseMvaSF%s%s%s01sigma[1,-999999.,999999.] := weight(\"LooseMvaSF%s%s%s01sigma\")" % (r9,region,direction,r9,region,direction))
#                variablesToUse.append("PreselSF%s%s%s01sigma[1,-999999.,999999.] := weight(\"PreselSF%s%s%s01sigma\")" % (r9,region,direction,r9,region,direction))
    systlabels += phosystlabels
elif customize.processId == "Data":
    print "Data, so turn of all shifts and systematics, except for Photon Scale central value"
    #variablesToUse = minimalNonSignalVariables
    newvpset = cms.VPSet()
    for pset in process.flashggDiPhotonSystematics.SystMethods:
        if pset.Label.value().count("Scale"):
            pset.NoCentralShift = cms.bool(False) # Turn on central shift for data (it is off for MC)                                                                              
            pset.NSigmas = cms.vint32() # Do not perform shift
            newvpset += [pset]
    process.flashggDiPhotonSystematics.SystMethods = newvpset
    systprodlist = [] #[process.flashggMuonSystematics,process.flashggElectronSystematics]
    #systprodlist += [getattr(process,"flashggJetSystematics%i"%i) for i in range(len(UnpackedJetCollectionVInputTag))]
    #for systprod in systprodlist:
    #    systprod.SystMethods = cms.VPSet() # empty everything
else:
    print "Background MC, so store mgg and central only"
    #variablesToUse = minimalNonSignalVariables
    vpsetlist = [process.flashggDiPhotonSystematics.SystMethods] #, process.flashggMuonSystematics.SystMethods, process.flashggElectronSystematics.SystMethods]
    #vpsetlist += [getattr(process,"flashggJetSystematics%i"%i).SystMethods for i in range(len(UnpackedJetCollectionVInputTag))] 
    # i.e. process.flashggJetSystematics0.SystMethods, ...
    for vpset in vpsetlist:
        for pset in vpset:
            pset.NSigmas = cms.vint32() # Do not perform shifts if they will not be read, but still do all central values

#print "--- Systematics  with independent collections ---"
#print systlabels
#print "-------------------------------------------------"
#print "--- Variables to be dumped, including systematic weights ---"
#print variablesToUse
#print "------------------------------------------------------------"

for systlabel in systlabels:
    if systlabel == "":
        continue
    newseq = cloneProcessingSnippet(process,process.flashggTagSequence,systlabel)
    if systlabel in phosystlabels:
        massSearchReplaceAnyInputTag(newseq,cms.InputTag("flashggDiPhotonSystematics"),cms.InputTag("flashggDiPhotonSystematics",systlabel))
    #if systlabel in jetsystlabels:    
    #    for i in range(len(jetSystematicsInputTags)):
    #        massSearchReplaceAnyInputTag(newseq,jetSystematicsInputTags[i],cms.InputTag(jetSystematicsInputTags[i].moduleLabel,systlabel))
    for name in newseq.moduleNames():
        module = getattr(process,name)
        if hasattr(module,"SystLabel"):
            module.SystLabel = systlabel
    process.systematicsTagSequences += newseq
    process.flashggSystTagMerger.src.append(cms.InputTag("flashggTagSorter" + systlabel))

############################
#    Systematics end       #
############################

process.commissioning = cms.EDAnalyzer('FlashggFlashggTreeMakerWithTagSorter',
                                       genEventInfoName = cms.untracked.InputTag('generator'),
                                       VertexTag = cms.untracked.InputTag('offlineSlimmedPrimaryVertices'),
                                       VertexCandidateMapTagDz = cms.InputTag('flashggVertexMapUnique'),
                                       VertexCandidateMapTagAOD = cms.InputTag('flashggVertexMapValidator'),
                                       inputTagJets = UnpackedJetCollectionVInputTag,
                                       DiPhotonTag = cms.InputTag('flashggDiPhotonSystematics'),
                                       #DiPhotonTag = cms.InputTag('flashggDiPhotons'),
                                       rhoFixedGridCollection = cms.InputTag('fixedGridRhoAll'),
                                       #PileUpTag = cms.untracked.InputTag( "addPileupInfo"),
                                       lumiWeight = cms.double(1.0),
                                       sampleIndex = cms.int32(-1000),
                                       #processId = cms.string("poppo"),                                       
                                       puReWeight = cms.bool(True),
                                       dataPu = cms.vdouble(),
                                       mcPu = cms.vdouble(),
                                       #puBins = cms.int32(50),
                                       #minpu = cms.double(0),
                                       #maxpu = cms.double(100),
                                       )

process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string("myMicroAODOutputFile_10.root")
)

process.p = cms.Path(process.dataRequirements*process.flashggDiPhotonSystematics*process.flashggTagSequence*process.commissioning)
#process.p = cms.Path(process.flashggTagSequence*process.commissioning)

customize(process)
