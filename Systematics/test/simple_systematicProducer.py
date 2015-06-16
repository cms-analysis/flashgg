import FWCore.ParameterSet.Config as cms
import FWCore.Utilities.FileUtils as FileUtils

process = cms.Process("FLASHggSyst")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.load("Configuration.StandardSequences.GeometryDB_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.GlobalTag.globaltag = 'POSTLS170_V5::All'
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(10000) )
#process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32( 100 )

# Uncomment the following if you notice you have a memory leak
# This is a lightweight tool to digg further
#process.SimpleMemoryCheck = cms.Service("SimpleMemoryCheck",
#                                        ignoreTotal = cms.untracked.int32(1),
#                                        monitorPssAndPrivate = cms.untracked.bool(True)
#                                       )

processId = "ggh"
process.source = cms.Source ("PoolSource",fileNames = cms.untracked.vstring("file:myMicroAODOutputFile_%s.root" % processId),
#                             skipEvents=cms.untracked.uint32(4965)
)

process.RandomNumberGeneratorService = cms.Service("RandomNumberGeneratorService",
                                                   flashggSmearDiPhoton = cms.PSet(initialSeed = cms.untracked.uint32(664))
                                                  )

process.load("flashgg.Systematics.flashggPhotonSmear_cfi")

# Code to artificially scale photon energies to make different mass points for signal fit tests
srcMass = 130.
targetMass = 130.
process.flashggSmearDiPhoton.SystMethods.append(cms.PSet( PhotonMethodName = cms.string("FlashggPhotonScaleString"),
                                                          MethodName = cms.string("FlashggDiPhotonFromPhoton"),
                                                          Label = cms.string("FakeMassScale"),
                                                          NSigmas = cms.vint32(0),
                                                          OverallRange = cms.string("1"),
                                                          BinList = cms.PSet(variables=cms.vstring(),
                                                                             bins=cms.VPSet(cms.PSet (lowBounds = cms.vdouble(), 
                                                                                                      upBounds =  cms.vdouble(), 
                                                                                                      values = cms.vdouble( targetMass/srcMass - 1. ), 
                                                                                                      uncertainties = cms.vdouble( 0. )))),
                                                          Debug = cms.untracked.bool(True)
                                                        )
                                                )

for pset in process.flashggSmearDiPhoton.SystMethods:
    print "=== 1D syst method pset ==="
    print pset
    print

for pset in process.flashggSmearDiPhoton.SystMethods2D:
    print "=== 2D syst method pset ==="
    print pset
    print

process.load("flashgg/Taggers/flashggTagSequence_cfi")
process.load("flashgg/Taggers/flashggTagTester_cfi")

from PhysicsTools.PatAlgos.tools.helpers import cloneProcessingSnippet,massSearchReplaceAnyInputTag

#process.flashggTagSequence += process.flashggTagTester
massSearchReplaceAnyInputTag(process.flashggTagSequence,cms.InputTag("flashggDiPhotons"),cms.InputTag("flashggSmearDiPhoton"))

#process.flashggUntaggedCategory.DiPhotonTag = cms.InputTag("flashggSmearDiPhoton")
#process.flashggTTHHadronicTag.DiPhotonTag = cms.InputTag("flashggSmearDiPhoton")
#process.flashggVBFTag.DiPhotonTag = cms.InputTag("flashggSmearDiPhoton")
#process.flashggVHEtTag.DiPhotonTag = cms.InputTag("flashggSmearDiPhoton")
#process.flashggTTHLeptonicTag.DiPhotonTag = cms.InputTag("flashggSmearDiPhoton")
#process.flashggVHLooseTag.DiPhotonTag = cms.InputTag("flashggSmearDiPhoton")
#process.flashggVHTightTag.DiPhotonTag = cms.InputTag("flashggSmearDiPhoton")
#process.flashggVHHadronicTag.DiPhotonTag = cms.InputTag("flashggSmearDiPhoton")


#vector<flashgg::DiPhotonCandidate>    "flashggSmearDiPhoton"      ""                "FLASHggSyst"     
#vector<flashgg::DiPhotonCandidate>    "flashggSmearDiPhoton"      "MCScaleHighR9EBDown01sigma"   "FLASHggSyst"     
#vector<flashgg::DiPhotonCandidate>    "flashggSmearDiPhoton"      "MCScaleHighR9EBUp01sigma"   "FLASHggSyst"     
#vector<flashgg::DiPhotonCandidate>    "flashggSmearDiPhoton"      "MCScaleHighR9EEDown01sigma"   "FLASHggSyst"     
#vector<flashgg::DiPhotonCandidate>    "flashggSmearDiPhoton"      "MCScaleHighR9EEUp01sigma"   "FLASHggSyst"     
#vector<flashgg::DiPhotonCandidate>    "flashggSmearDiPhoton"      "MCScaleLowR9EBDown01sigma"   "FLASHggSyst"     
#vector<flashgg::DiPhotonCandidate>    "flashggSmearDiPhoton"      "MCScaleLowR9EBUp01sigma"   "FLASHggSyst"     
#vector<flashgg::DiPhotonCandidate>    "flashggSmearDiPhoton"      "MCScaleLowR9EEDown01sigma"   "FLASHggSyst"     
#vector<flashgg::DiPhotonCandidate>    "flashggSmearDiPhoton"      "MCScaleLowR9EEUp01sigma"   "FLASHggSyst"     
#vector<flashgg::DiPhotonCandidate>    "flashggSmearDiPhoton"      "MCSmearHighR9EBPhiDown01sigma"   "FLASHggSyst"     
#vector<flashgg::DiPhotonCandidate>    "flashggSmearDiPhoton"      "MCSmearHighR9EBPhiUp01sigma"   "FLASHggSyst"     
#vector<flashgg::DiPhotonCandidate>    "flashggSmearDiPhoton"      "MCSmearHighR9EBRhoDown01sigma"   "FLASHggSyst"     
#vector<flashgg::DiPhotonCandidate>    "flashggSmearDiPhoton"      "MCSmearHighR9EBRhoUp01sigma"   "FLASHggSyst"     
#vector<flashgg::DiPhotonCandidate>    "flashggSmearDiPhoton"      "MCSmearHighR9EEDown01sigma"   "FLASHggSyst"     
#vector<flashgg::DiPhotonCandidate>    "flashggSmearDiPhoton"      "MCSmearHighR9EEUp01sigma"   "FLASHggSyst"     
#vector<flashgg::DiPhotonCandidate>    "flashggSmearDiPhoton"      "MCSmearLowR9EBPhiDown01sigma"   "FLASHggSyst"     
#vector<flashgg::DiPhotonCandidate>    "flashggSmearDiPhoton"      "MCSmearLowR9EBPhiUp01sigma"   "FLASHggSyst"     
#vector<flashgg::DiPhotonCandidate>    "flashggSmearDiPhoton"      "MCSmearLowR9EBRhoDown01sigma"   "FLASHggSyst"     
#vector<flashgg::DiPhotonCandidate>    "flashggSmearDiPhoton"      "MCSmearLowR9EBRhoUp01sigma"   "FLASHggSyst"     
#vector<flashgg::DiPhotonCandidate>    "flashggSmearDiPhoton"      "MCSmearLowR9EEDown01sigma"   "FLASHggSyst"     
#vector<flashgg::DiPhotonCandidate>    "flashggSmearDiPhoton"      "MCSmearLowR9EEUp01sigma"   "FLASHggSyst"     


process.flashggSystTagMerger = cms.EDProducer("TagMerger",src=cms.VInputTag("flashggTagSorter"))

process.systematicsTagSequences = cms.Sequence()

systlabels = []
for r9 in ["HighR9","LowR9"]:
    for direction in ["Up","Down"]:
        systlabels.append("MCSmear%sEE%s01sigma" % (r9,direction))
        for var in ["Rho","Phi"]:
            systlabels.append("MCSmear%sEB%s%s01sigma" % (r9,var,direction))
        for region in ["EB","EE"]:
            systlabels.append("MCScale%s%s%s01sigma" % (r9,region,direction))

for systlabel in systlabels:
    newseq = cloneProcessingSnippet(process,process.flashggTagSequence,systlabel)
    massSearchReplaceAnyInputTag(newseq,cms.InputTag("flashggSmearDiPhoton"),cms.InputTag("flashggSmearDiPhoton",systlabel))
    process.systematicsTagSequences += newseq
    process.flashggSystTagMerger.src.append(cms.InputTag("flashggTagSorter" + systlabel))

from flashgg.Taggers.flashggTagOutputCommands_cff import tagDefaultOutputCommand

process.out = cms.OutputModule("PoolOutputModule", fileName = cms.untracked.string('myTagOutputFile_%s_%i.root' % (processId,targetMass)),
                               outputCommands = tagDefaultOutputCommand
                               )

process.p = cms.Path(process.flashggSmearDiPhoton*
                     (process.flashggTagSequence+process.systematicsTagSequences)*
                     process.flashggSystTagMerger)

print process.p

process.e = cms.EndPath(process.out)
